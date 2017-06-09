#pragma once

#include <Arduino.h>
#include "Settings.h"
#include "MQTT.h"

class GPIO
{
public:
  int relayPin = D1;
  int buttonPin = D3;

  GPIO(Settings *settings, MQTT *mqtt);
  void setup();
  void led(bool on);
  void relay(bool on);
  void switchRelay();
  String getRelayState();
  void applyRelayState(String state);
  void loop();

private:
  Settings *_settings;
  MQTT *_mqtt;
  bool relayOn;
  bool buttonPressed;
};