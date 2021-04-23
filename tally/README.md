# ATEM Tally
This project is a tally light for a BlackMagic ATEM production switcher. This uses a NodeMCU and server to provide the translation.

## Features
* Preview/Live tally updates
* Fail safe detection. Detects if Wifi has diconnected or connection has been lost to the server
* ME ignoring. Can be configured to ignore certain MEs if required or show tallys based on all/multiple MEs
* Custom colours
* Stage/user leds
* Uses network broadcast so defining ip addresses is not needed, they just have to be on the same network
* Show blue feature which shows a blue light to the stage to show where the camera is

# How to use
There are 2 major parts that are needed to make this work, this is the tally(s) it's self, and the server which talks directly to the ATEM
## Server Side
The server is required to talk to the ATEM. The server then sends out messages to the tally(s) which will show the state.

[Click here](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/tree/main/server) to see information on how to install the server

## Tallys
### Required Hardware
The following hardware is required to build a single tally
* 1x - [NodeMCU V3 ESP8266](https://components101.com/development-boards/nodemcu-esp8266-pinout-features-and-datasheet)
* 2x - PL9823 5mm Pixel RGB LED
### 3D Case
* If you wish to use the 3D printed case provided print the bottom.stl and top.stl files found in the [3Ds folder](https://github.com/Kardinia-Church/ATEM-Tally/tree/subscription-based/3Ds)
### Wiring it up
![Circuit Diagram](https://github.com/Kardinia-Church/Blackmagic-ATEM-Tally/blob/main/tally/images/circuitDiagram.png)
* Note the first LED in the chain is the user LED
### Programming the NodeMCU
* In order to program the NodeMCU you either need to download [Visual Studio Code](https://code.visualstudio.com/) and install the [PlatformIO](https://platformio.org/install/ide?install=vscode) extension which is used to compile the project
* Download the source code from GitHub [here!](https://github.com/Kardinia-Church/ATEM-Tally/releases)
* Open Platform IO home in Visual Studio Code and open the project found in the ``Source`` folder
* Open the ```settings.h``` file found in the ```src``` folder. This contains the settings that can be configured
* At the minimum the ```ssid``` and ```password``` must be set to your wifi network. This must be the same network as the server!
The following variables can also be set:
* ```inputID``` - The input id
* ```ssid``` - The wifi SSID to connect to
* ```password``` - The wifi password
* ```ignoredMEs``` - What MEs to ignore. By default this is ME 2, 3, 4 (So ME1 will be the tally ME).
* ```previewEnabled``` - Should preview be enabled?
* ```programEnabled``` - Should program be enabled?
* ```keyerDSEnabled``` - Should the DS keyer be enabled?
* ```keyerUSEnabled``` - Should the US keyer be enabled
* ```showBlue``` - Should a blue led appear on the stage led to indicate location?
* ```liveColor``` - What colour should live be?
* ```keyerColor``` - What colour should live on a keyer be?
* ```standbyColor``` - What colour should preview/standby be?
* ```blueColor``` - What colour should the indicator be?
* ```offColor``` - What colour should off be?
* Finally upload the code to the NodeMCU using the arrow that points to the right on the bottom bar
If successful the NodeMCU should either be blank or display a tally state. If it is showing blue or another colour see the error codes below.

## Errors
### Colours
* Flashing Blue - Connecting to Wifi
* Flashing blue then flashing another color (version color) - This means it timed out connecting to Wifi and has rebooted. Check your Wifi settings / Wifi
* Solid blue - This means the tally is attempting to "subscribe" to the server if this stays on then there is an issue connecting to the server. Check if the tally is on the same network and if the server is running correctly
