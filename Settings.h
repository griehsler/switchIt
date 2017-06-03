#pragma once

#include <Arduino.h>
#include "Storage.h"

class Settings
{
public:
  String otherAPSSID;
  String otherAPPassword;
  String hostName;
  String deviceName;

  const int BUTTON_DISABLED = 0;
  const int BUTTON_SWITCH = 1;
  const int BUTTON_TOUCH = 2;
  int buttonMode = BUTTON_DISABLED;

  bool mqttEnabled = false;
  String mqttServer;
  int mqttServerPort = 1883;
  String mqttUserName;
  String mqttPassword;
  String mqttPublishTopic;
  String mqttSubscribeTopic;

  bool emulateRelay = false;

  void storeDeviceSettings();
  void loadDeviceSettings();
  bool tryLoadWifiSettings();
  void storeWifiSettings();
  String getStatus(String stateCode);
  void storeState(String stateCode);
  String getStoredState();

  Settings(Storage *storage);

private:
  Storage *_storage;
};
