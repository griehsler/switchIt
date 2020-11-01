#pragma once

#include <LoggerSettings.h>
#include <NetworkSettings.h>
#include <Storage.h>

class Settings: public virtual NetworkSettings, public virtual LoggerSettings
{
public:
  Settings(Storage *storage);

  const int httpServerPort = 80;

  String deviceName;

  bool invertSwitch = false;

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
