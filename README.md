# ATEM Tally
A simple tally light using the ESP to provide tally states from a Blackmagic ATEM switcher.

This project has 3 parts, the server, the basic tally package, and a tally that also supports CCU control of a Blackmagic camera using it's bluetooth connection.

# Project Parts
## [The Server](./tree/main/CCUTally)
This is required by either tally type. This provides the connection to the ATEM and translates the commands for the tallys to read

## [The Basic Tally]()
This is the basic tally using the NodeMCU

### Features
* Preview/Live tally updates
* Fail safe detection. Detects if Wifi has diconnected or connection has been lost to the server
* ME ignoring. Can be configured to ignore certain MEs if required or show tallys based on all/multiple MEs
* Custom colours
* Stage/user leds
* Uses network broadcast so defining ip addresses is not needed, they just have to be on the same network
* The show blue feature which shows a blue light to the stage to show where the camera is

## [The CCU Tally]()
This is a more feature rich tally using the ESP32.

### Features
* Tally status using independent tally light boxes
* Control of the camera's CCU parameters over the camera's bluetooth connection
* Serial configuration editor so you don't need to recompile the code every time you need to change a setting

# Installation
1. First install the server as it is required by both tally types
2. Open the tally type you wish to use and follow the specific building, compiling, and configuration steps
3. The tally should connect to the server and provide tallying!

