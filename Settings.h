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

  bool mqttEnabled = false;
  String mqttServer;
  int mqttServerPort = 1883;
  String mqttUserName;
  String mqttPassword;
  String mqttPublishTopic;
  String mqttSubscribeTopic;

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
