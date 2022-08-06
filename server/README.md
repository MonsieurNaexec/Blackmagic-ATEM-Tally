# Blackmagic ATEM Tally Server
The server module for the Blackmagic ATEM Tally Server.

# Installation
* Install [nodejs](https://nodejs.org/en/)
* ```git clone https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally```
* ```cd server```
* ```npm install```
* Run ```node index.js``` for the first time. This will create a config file
* Edit the ```config.json``` file with your desired settings

# Running
* Execute the following command to start the server ```node app.js```

# Starting on boot
In order to start the plugin on boot we'll use PM2
* Install [pm2](https://pm2.keymetrics.io/). ```sudo npm install -g pm2```
* cd into the server directory
* ```sudo pm2 start app.js```
* ```sudo pm2 startup systemd```
* ```sudo pm2 save```