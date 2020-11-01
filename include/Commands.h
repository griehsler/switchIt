#pragma once

#include <Arduino.h>
#include "GPIO.h"
#include "Settings.h"

class Commands
{
public:
  Commands(Settings *settings, GPIO *gpio);
  void switchStatus();
  void on();
  void off();
  String getStatus();
  bool execute(String name, String *reply);

  const String CMD_STATUS = "status";

private:
  Settings *_settings;
  GPIO *_gpio;

  const String CMD_SWITCH = "switch";
  const String CMD_ON = "on";
  const String CMD_OFF = "off";
};