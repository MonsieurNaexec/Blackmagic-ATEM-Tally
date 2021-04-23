const {Atem, AtemStateUtil} = require("atem-connection");
var nconf = require("nconf");
var atemIP = undefined;
const atem = new Atem();
var me = 0;

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
        update();
        stopFlashKeys();

        atem.on('stateChanged', (state, pathToChange) => {
            update();
        });      
    });
    atem.on("disconnected", function() {
        console.log("Disconnected from the ATEM");
        flashKeys();
    });

    //Connect
    atem.connect(atemIP);
}

//Update the status of the device
function update() {
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