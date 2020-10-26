#pragma once

#include "Storage.h"

class Settings
{
public:
  Settings(Storage *storage);

  const int httpServerPort = 80;

  String otherAPSSID;
  String otherAPPassword;
  String hostName;
  String deviceName;

  bool invertSwitch = false;

  const int BUTTON_DISABLED = 0;
  const int BUTTON_SWITCH = 1;
  const int BUTTON_TOUCH = 2;
  int buttonMode = BUTTON_DISABLED;

  bool syslogEnabled = false;
  String syslogServer;
  int syslogServerPort = 514;

  bool mqttEnabled = false;
  String mqttServer;
  int mqttServerPort = 1883;
  String mqttUserName;
  String mqttPassword;
  String mqttPublishTopic;
  String mqttSubscribeTopic;

  int customButtonGpio = 0;
  int customRelayGpio = 0;

  bool emulateRelay = false;

  void load();
  void store();

  String getStoredState();
  void storeState(String stateCode);

  String getStateSummary(String stateCode);

private:
  Storage *_storage;
  void loadDeviceSettings();
  void loadWifiSettings();
  void storeDeviceSettings();
  void storeWifiSettings();
};
