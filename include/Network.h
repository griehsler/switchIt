#pragma once

#include <ESP8266WiFi.h>
#include "Settings.h"

class Network
{
public:
  Network(Settings *settings);
  void setup();
  void loop();

private:
  Settings *_settings;
  bool wasConnected = false;
  int reconnectCount = 0;
  bool connectToWifi();
  void startAP();
};