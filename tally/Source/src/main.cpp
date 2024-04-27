/*
  ATEM Tally by Kardinia Church 2020
  A simple tally light that shows an input's tally state using NodeRed as a server

  https://github.com/Kardinia-Church/ATEM-Tally

  main.cpp file responsible for the main entry point and code functions
*/

#define VERSION strip.Color(255, 0, 255)

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

#define SERIAL_BAUD 115200
#define INCOMING_PORT 5657
#define OUTGOING_PORT 8001
#define DEBUG false

#define CMD_SUBSCRIBE 0xAF
#define CMD_PING 0xFA
#define CMD_PROGRAM 0x01
#define CMD_PREVIEW 0x02
#define CMD_DSKEY 0x03
#define CMD_USKEY 0x04

WiFiUDP udp;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, 0, NEO_GRB + NEO_KHZ800);
IPAddress serverIP(255, 255, 255, 255);
unsigned long lastMessage = millis();
bool programTally = false;
bool previewTally = false;
bool usKeyTally = false;
bool dsKeyTally = false;
bool pingSent = false;

#include "settings.h"

settings config;

String getSSID()
{
  String ssid = "";
  for (uint8_t i = 0; i < config.wifi_ssid_size; i++)
    ssid += config.wifi_ssid[i];
  return ssid;
}

String getKey()
{
  String password = "";
  for (uint8_t i = 0; i < config.wifi_key_size; i++)
    password += config.wifi_key[i];
  return password;
}

void setFrontColor(uint32_t color)
{
  strip.setPixelColor(3, color);
  strip.setPixelColor(4, color);
  strip.setPixelColor(5, color);
}
void setBackColor(uint32_t color)
{
  strip.setPixelColor(0, color);
  strip.setPixelColor(1, color);
  strip.setPixelColor(2, color);
}

void setColor(uint32_t color)
{
  strip.setPixelColor(0, color);
  strip.setPixelColor(1, color);
  strip.setPixelColor(2, color);
  strip.setPixelColor(3, color);
  strip.setPixelColor(4, color);
  strip.setPixelColor(5, color);
}

void setColor(uint32_t front, uint32_t back)
{
  strip.setPixelColor(0, back);
  strip.setPixelColor(1, back);
  strip.setPixelColor(2, back);
  strip.setPixelColor(3, front);
  strip.setPixelColor(4, front);
  strip.setPixelColor(5, front);
}

// Send a request to subscribe
void subscribe()
{
  Serial.println("Subscribe");
  // Subscribe cmd, inputId, ignoreME(s)
  udp.beginPacket(serverIP, OUTGOING_PORT);
  uint8_t buffer[] = {
      CMD_SUBSCRIBE,
      config.cameraID,
      config.watchMEs,
  };

  udp.write(buffer, sizeof(buffer));
  udp.endPacket();
  lastMessage = millis();
}

// Send a ping request
void ping()
{
  pingSent = true;
  Serial.println("Send ping");
  udp.beginPacket(serverIP, OUTGOING_PORT);
  uint8_t buffer[1] = {CMD_PING};
  udp.write(buffer, 1);
  udp.endPacket();
  lastMessage = millis();
}

// Process the incoming packet
byte packetBuffer[UDP_TX_PACKET_MAX_SIZE];
void receivePacket()
{
  int packetSize = udp.parsePacket();
  if (packetSize > 0)
  {
    udp.read(packetBuffer, packetSize);
    for (int i = 0; i < packetSize; ++i)
    {
      Serial.print(packetBuffer[i], HEX);
      Serial.print(' '); // Ajouter un espace entre chaque octet
    }
    Serial.println();
    switch (packetBuffer[0])
    {
    case CMD_PROGRAM:
    {
      programTally = packetBuffer[1] == 0x01;
      pingSent = false;
      lastMessage = millis();
      break;
    }
    case CMD_PREVIEW:
    {
      previewTally = packetBuffer[1] == 0x01;
      pingSent = false;
      lastMessage = millis();
      break;
    }
    case CMD_DSKEY:
    {
      dsKeyTally = packetBuffer[1] == 0x01;
      pingSent = false;
      lastMessage = millis();
      break;
    }
    case CMD_USKEY:
    {
      usKeyTally = packetBuffer[1] == 0x01;
      pingSent = false;
      lastMessage = millis();
      break;
    }
    case CMD_PING:
    {
      pingSent = false;
      lastMessage = millis();
      Serial.println("RESPONSE");
      break;
    }
    }

    // Do colours
    if ((dsKeyTally && config.keyerDSEnabled) || (usKeyTally && config.keyerUSEnabled))
    {
      // Tally colour
      setColor(keyerColor);
    }
    else if (programTally && config.programEnabled)
    {
      // Program colour
      setColor(liveColor);
    }
    else if (previewTally && config.previewEnabled)
    {
      // Preview colour
      setColor(config.showBlue ? blueColor : offColor, standbyColor);
    }
    else if (config.showBlue)
    {
      // Blue colour
      setColor(blueColor, offColor);
    }
    else
    {
      // All off
      setColor(offColor);
    }
  }
  else if (lastMessage + 5000 < millis())
  {
    if (pingSent)
    {
      setColor(blueColor);
      subscribe();
    }
    else
    {
      ping();
    }
  }
}

// Process the incoming commands
String command = "";
String argument = "";
uint8_t commandState = 0;

void sendData(String key, String value)
{
  Serial.println("$" + key + ":" + value);
}
void sendCommand(String cmd)
{
  Serial.println("$" + cmd);
}
void sendMessage(String msg)
{
  Serial.println("#" + msg);
}

void executeCommand()
{
  if (command == "state")
  {
    sendData("ssid", getSSID());
    sendData("key", getKey());
    sendData("camera_id", String(config.cameraID));
  }
  else if (command == "ssid")
  {
    if (argument.length() > 19)
      sendMessage("Too long!");
    else
    {
      argument.toCharArray(config.wifi_ssid, argument.length() + 1);
      config.wifi_ssid_size = argument.length();
    }
    sendData("ssid", getSSID());
  }
  else if (command == "key")
  {
    if (argument.length() > 19)
      sendMessage("Too long!");
    else
    {
      argument.toCharArray(config.wifi_key, argument.length() + 1);
      config.wifi_key_size = argument.length();
    }
    sendData("key", getKey());
  }
  else if (command == "id")
  {
    config.cameraID = argument.toInt();
    sendData("camera_id", String(config.cameraID));
  }
  else if (command == "save")
  {
    EEPROM.put(0, config);
    EEPROM.commit();
    ESP.restart();
  }
  else if (command == "reset")
  {
    config = default_settings;
  }

  command = "";
  argument = "";
  commandState = 0;
}

void receiveSerial()
{
  while (Serial.available())
  {
    char incommingByte = Serial.read();
    if (commandState == 0 && incommingByte == ':')
    {
      commandState = 1;
      return;
    }
    if (incommingByte == ';' || incommingByte == '\n' || incommingByte == '\r')
    {
      executeCommand();
      return;
    }
    if (commandState == 0)
      command += incommingByte;
    else if (commandState == 1)
      argument += incommingByte;
  }
}

void setup()
{
  EEPROM.begin(sizeof(settings));
  EEPROM.get(0, config);

  String ssid = getSSID();
  String password = getKey();

  Serial.begin(SERIAL_BAUD);
  Serial.println("Connecting to " + String(ssid));
  strip.begin();
  delay(100);
  setColor(VERSION, offColor);
  strip.show();
  delay(2000);
  WiFi.begin(ssid, password);
  WiFi.softAPdisconnect(true);

  int s = 0;
  int count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() % 1000 < 500 && s == 0)
    {
      setBackColor(offColor);
      Serial.print(".");
      s = 1;
    }
    else if (millis() % 1000 >= 500 && s == 1)
    {
      setBackColor(blueColor);
      s = 0;

      // If we have failed many times restart
      if (count++ > 20)
      {
        ESP.restart();
      }
    }
    strip.show();
    receiveSerial();
  }

  // Show the input id flashes = input id
  setBackColor(offColor);
  strip.show();
  delay(1000);
  for (int i = 0; i < config.cameraID; i++)
  {
    setBackColor(liveColor);
    strip.show();
    delay(200);
    setBackColor(offColor);
    strip.show();
    delay(400);
  }

  setBackColor(standbyColor);
  strip.show();
  delay(1000);

  udp.begin(INCOMING_PORT);
  subscribe();
  setBackColor(offColor);
  if (config.showBlue)
  {
    setFrontColor(blueColor);
  }
  else
  {
    setFrontColor(offColor);
  }
  Serial.print("My IP:");
  Serial.println(WiFi.localIP());
  Serial.println("Done");
}

void loop()
{
  // Check the wifi status
  if (WiFi.status() != WL_CONNECTED)
  {
    ESP.restart();
  }

  receivePacket();
  receiveSerial();
  strip.show();
}