# ATEM Tally
A tally light using the ESP8266/ESP32 to provide tally states from a Blackmagic ATEM switcher.

This project has 3 parts:
## 1. [The Server](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/server)
Connects to the Blackmagic ATEM and provides a server for the tallys to connect to. This allows the heavy lifting to be done by a more powerful computer and means that there is only 1 connection to the ATEM for many tally lights.
## 2. [The Basic Tally](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/tally)
The basic tally uses an ESP8266 and provides live/preview feedback to the camera operator.

### Features
* Preview/Live tally updates
* Fail safe detection. Detects if Wifi has disconnected or connection has been lost to the server
* ME ignoring. Can be configured to ignore certain MEs if required or show tallys based on all/multiple MEs
* Custom colours
* Stage/user leds
* Uses network broadcast so defining ip addresses is not needed, they just have to be on the same network
* The show blue feature which shows a blue light to the stage to show where the camera is


## 3. [The CCU Tally](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/CCUTally)
This tally uses an ESP32 and provides both the tally feature as well as CCU control of a Blackmagic camera over bluetooth.

### Features
* Tally status using independent tally light boxes
* Control of the camera's CCU parameters over the camera's bluetooth connection
* Serial configuration editor so you don't need to recompile the code every time you need to change a setting

# Limitations
Please note this project was not completed and i cannot work on it anymore. The following limitations are known:
1. The CCU tally is not completed, see [The CCU Tally](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/CCUTally) for more information.
1. Currently the CCU is not supported with the server, NodeRed is only supported.
2. The direct ATEM connection does not work.

# Installation
1. First install the [server](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/server) as it is required by both tally types
2. Open the tally type you wish to use ([Basic](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/tally) or [CCU](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/CCUTally)) and follow the specific building, compiling, and configuration steps
3. The tally should connect to the server and provide tallying!
