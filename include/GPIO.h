#pragma once

#include <Arduino.h>
#include <Logger.h>
#include "MQTT.h"
#include "Settings.h"

class GPIO
{
public:
  int relayPin = D1;
  int buttonPin = D3;

  GPIO(Settings *settings, Logger *logger, MQTT *mqtt);
  void setup();
  void led(bool on);
  void relay(bool on);
  void switchRelay();
  String getRelayState();
  void restoreLastState();
  void loop();

private:
  Settings *_settings;
  Logger *_logger;
  MQTT *_mqtt;
  bool relayOn;
  bool buttonPressed;
  bool lastPressed;
  bool longPressed;
  unsigned long longPressStart;
  unsigned long debounceDelay = 100;
};