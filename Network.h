#pragma once

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