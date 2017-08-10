#pragma once

#include <Arduino.h>
#include <WiFiUdp.h>
#include "Settings.h"

#define LOG_NOTICE "13"
#define LOG_INFO "14"

class Logger
{
public:
  Logger(Settings * settings);
  void writeLog(String severity, String message);
  void setup();

private:
  Settings *_settings;
  WiFiUDP _UDP;
};
