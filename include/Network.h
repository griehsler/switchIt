#pragma once

#include <ESP8266WiFi.h>
#include "Settings.h"

class Network
{
public:
  Network(Settings *settings);
  void setup();

private:
  Settings *_settings;
  bool connectToWifi();
  void startAP();
};