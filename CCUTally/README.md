# ATEM CCU 2 BT
A ESP32 project to convert CCU parameters into a bluetooth emulation to control Blackmagic cameras via bluetooth wirelessly!

# Still in development
This project is still in development so it is missing certain features and has some bugs.

## What's working?
So far this project supports:
- Controlling CCU parameters using a the [server package]() as a middle man to transfer the packets
- Serial configuration
- Tallying
- Direct ATEM connection for both CCU and tallying

This is now running on our wireless camera rig with great success!

## What's not working?
At the moment the following is not working or not finished:
- Web configuration
- Clearing a connected camera acts funky sometimes. If you have this issue try using the ESP32 tool to clear all memory
- Changing parameters on the camera will not update the ATEM. It's only ATEM->Camera
- If bluetooth disconnects it may not reconnect and a powercycle will be required

# Installation 
In order to use the project a ESP32 is required, this project was built on a ESP-WROOM-32 from Jaycar.

- Please make sure the device is powered from the same power source as the camera as it may not reconnect when the camera is power cycled (through mains)

- Download or clone the project files from Github
- Open the project in [PlatformIO](https://platformio.org/) using [Microsoft's Visual Studio Code](https://code.visualstudio.com/)
- Select your device's COM port and upload!

- Next connect to the device from a serial monitor on 115200 baud or use the wifi network to configure the device (currently not supported)
- If using the web configuration tool set all the values required.

- If using the serial monitor send anything to open the menu
- Set 1 (Change the WIFI settings) and 2 (Change the ATEM Connection Preference) as a minimum to setup the device following the prompts
- Once this is complete the device will attempt to find the camera via bluetooth please keep the camera near to the device
- A prompt should come up asking for a pin. This is displayed in the camera's bluetooth settings please enter it and hit enter.
- Hopefully the device will reboot and connect to everything!
- The camera id is set in the camera's settings under the setup page

## Hardware
### Tally feature
If desired one can connect a string of NeoPixels (we use 2 PL9823's) with a stage led and user led to show the tally connected to pin ```G26```

### 3D Files
Included are 3D printable files for both the tally lights and controller. The tally light file can be printed for two PL9823s to be used as a user led and stage led. The controller file can be used just to enclose your ESP32 with mounting points or to be used for the tally system.

# How to use
The device once configured should connect and act as expected. If not the device can be configured using the following methods:
- ```Web Configuration``` Press the restart button 3 times to open a AP which when connected to can be used to configure the device. If not taken to the page navigate to ```192.168.1.1``` CURRENTLY NOT SUPPORTED!
- ```Serial Configuraion``` Connect to the device via USB and open a serial terminal at ```115200```. Once connected sending anything will open the configuration menu.

# Troubleshooting
## Tally operation codes
If the tally feature is enabled the following (if default settings are used) will occur:
- ```First color on power up``` is the version
- ```Orange at power up``` the device is connecting to required devices
- ```Turquoise color``` the device is in AP mode

## Tally Error Codes
If the tally feature is enabled the following (if default settings are used) will occur:
- ```Fast flashing blue 20 times before reboot``` Wifi failed to connect or was lost
- ```Fast flashing blue 20 times on startup``` Bluetooth failed to connect
- ```Fast flashing red 20 times on startup``` Tally feature failed to connect
- ```Fast flashing yellow 20 times on startup``` A camera ID was not set

# Special Thanks
This project is made possible by the following
- [SchoolPost](https://github.com/schoolpost/) for [BlueMagic32](https://github.com/schoolpost/BlueMagic32) which provides the bluetooth functionality of this project
- [SKAARHOJ](https://www.skaarhoj.com/) for the research and listing of the commands to interface with the ATEM