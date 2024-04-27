/*
  ATEM Tally by Kardinia Church 2020
  A simple tally light that shows an input's tally state using NodeRed as a server

  https://github.com/Kardinia-Church/ATEM-Tally

  settings.h The settings file for the tally
*/

#define ME_1 0b0001
#define ME_2 0b0010
#define ME_3 0b0100
#define ME_4 0b1000

struct settings
{
  char wifi_ssid[20];
  uint8_t wifi_ssid_size;
  char wifi_key[20];
  uint8_t wifi_key_size;
  uint8_t cameraID;
  uint8_t watchMEs;
  bool previewEnabled;
  bool programEnabled;
  bool keyerDSEnabled;
  bool keyerUSEnabled;
  bool showBlue;
  uint8_t ledIntensity;
};

settings default_settings = {
    "",
    0,
    "",
    0,
    1,
    ME_1,
    true,
    true,
    false,
    false,
    false,
    0xFF,
};

// Colors Green, Red, Blue
int liveColor = strip.Color(255, 0, 0);    // What colour should live be?
int keyerColor = strip.Color(120, 255, 0); // What colour should live on a keyer be?
int standbyColor = strip.Color(0, 255, 0); // What colour should preview/standby be?
int blueColor = strip.Color(0, 0, 255);    // What colour should the indicator be?
int offColor = strip.Color(0, 0, 0);       // What colour should off be?