const {Atem, AtemStateUtil} = require("atem-connection");
var nconf = require("nconf");
var udp = require('dgram');
var buffer = require('buffer');
var client = udp.createSocket('udp4');
var atemIP = undefined;
const atem = new Atem();
var server = udp.createSocket('udp4');
var incomingPort = 8001;
var outgoingPort = 5657;

var ccuDevices = {};
var tallyDevices = {};

//Attempt to load in the configuration
function loadConfig(callback) {
    nconf.use("file", {file: "./config.json"});
    nconf.load();

    var error = false;
    if(nconf.get("atemIP") === undefined) {nconf.set("atemIP", "127.0.0.1"); error = true;}

    if(error) {
        //Error
        console.log("There is an issue with the configuration file. Please check the configuration file config.json");
        
        nconf.save(function (error) {
            if(error){console.log("An error occurred saving the config file: " + error.message);}
            callback(false);
        });
    }
    else {
        //Load in the settings
        atemIP = nconf.get("atemIP");
        callback(true);
    }
}

//Send out a message to a device
function sendMessage(buffer, ip) {
    client.send(buffer, outgoingPort, ip, (error) => {
        if(error) {
            console.log("Error sending packet to tally @ " + ip);
        }
    });
}

//Send a ping response
function sendPing(ip) {
    sendMessage(new Buffer.from([0xFA]), ip);
}

//Send out the tally data for the basic tally
function sendTallyData(forceSend=false) {
    for(var i in tallyDevices) {
        var isProg = false;
        var isPrev = false;
        var isDSKey = false;
        var isUSKey = false;

        //Downstream keyers
        for(var j in atem.state.video.downstreamKeyers) {
            if((atem.state.video.downstreamKeyers[j].sources.fillSource == tallyDevices[i].inputId || atem.state.video.downstreamKeyers[j].sources.cutSource == tallyDevices[i].inputId) && atem.state.video.downstreamKeyers[j].onAir) {
                isDSKey = true;
                break;
            }
        }
        
        //Check MEs
        for(var j in atem.state.video.ME) {
            if(!tallyDevices[i].ignoredMEs.includes(j)) {
                //Check US keyer
                for(var k in atem.state.video.ME[j].upstreamKeyers) {
                    if((atem.state.video.ME[j].upstreamKeyers[k].fillSource == tallyDevices[i].inputId || atem.state.video.ME[j].upstreamKeyers[k].cutSource  == tallyDevices[i].inputId) && atem.state.video.ME[j].upstreamKeyers[k].onAir) {
                        isUSKey = true;
                        break;
                    }
                }

                if(!isProg) {isProg = atem.state.video.ME[j].programInput == tallyDevices[i].inputId || atem.state.video.ME[j].inTransition;}
                if(!isPrev) {isPrev = atem.state.video.ME[j].previewInput == tallyDevices[i].inputId;}
            }
        }

        //Prog
        if(tallyDevices[i].isProg != isProg || forceSend) {
            tallyDevices[i].isProg = isProg;
            sendMessage(Buffer.from([0x01, isProg, 0x00]), i);
        }
        //Prev
        if(tallyDevices[i].isPrev != isPrev || forceSend) {
            tallyDevices[i].isPrev = isPrev;
            sendMessage(Buffer.from([0x02, isPrev, 0x00]), i);
        }
        //DS Key
        if(tallyDevices[i].isDSKey != isDSKey || forceSend) {
            tallyDevices[i].isDSKey = isDSKey;
            sendMessage(Buffer.from([0x03, isDSKey, 0x00]), i);
        }
        //US Key
        if(tallyDevices[i].isUSKey != isUSKey || forceSend) {
            tallyDevices[i].isUSKey = isUSKey;
            sendMessage(Buffer.from([0x04, isUSKey, 0x00]), i);
        }
    }
}

//Send out the CCU data
function sendCCUData() {
}

//Connect to the ATEM
function connect() {
    var self = this;

    //Set the handlers
    atem.on("info", function(message) {
        console.log("INFO: " + message);
    });
    atem.on("error", function(error) {
        console.log("ERROR: " + error);
    });

    atem.on("connected", () => {
        console.log("Successfully connected to the ATEM");
        sendTallyData();
        sendCCUData();

        atem.on('stateChanged', (state, pathToChange) => {
            sendTallyData();
            sendCCUData();
        });      
    });

    atem.on("disconnected", function() {
        console.log("Disconnected from the ATEM");
    });

    server.on("error", (error) => {
        console.log("A tally server error occurred: " + error);
    });

    server.on("listening", (error) => {
        console.log("The tally server is listening on " + server.address().address + ":" + server.address().port);
    });

    server.on("message", (msg, info) => {
        switch(msg[0]) {
            //Subscribe request for tally function
            case 0xAF: {
                var ignoredMEs = [];
                var ignoredMEsFriendly = "";
                for(var i = 2; i < msg.length; i++) {
                    ignoredMEs[i - 2] = msg[i];
                    ignoredMEsFriendly += msg[i] + ",";
                }
                console.log("Tally subscribe request from " + info.address + " InputID: " + msg[1] + " Ignored MEs: " + ignoredMEsFriendly.slice(0, -1));

                //Add this device to our pool to send to
                tallyDevices[info.address] = {
                    "inputId": msg[1],
                    "ignoredMEs": ignoredMEs,
                    "isProg": false,
                    "isPrev": false,
                    "isDSKey": false,
                    "isUSKey": false
                };

                sendPing(info.address);
                sendTallyData();
                break;
            }
            //Subscribe request for ccu function
            case 0xAD: {
                
            }
            //Ping request
            case 0xFA: {
                sendPing(info.address);
                break;
            }
        }

        // for(var i = 0; i < msg.length; i++) {
        //     console.log(msg[i]);
        // }
    });

    //Connect
    atem.connect(atemIP);
    server.bind(incomingPort);
}

//Main loop
console.log("ATEM tally server by Kardinia Church 2021");
console.log("Attempting to load configuration");

loadConfig(function(success) {
    if(success == true) {
        //Everything seems good lets begin!
        console.log("Success, attempting connection to the ATEM at " + atemIP);
        connect();
    }
    else {
        console.error("Initialization errors occurred, cannot continue");
    }
});