#include "Settings.h"
#include <ArduinoJson.h>

const String wifiSettingsFile = "/config/wifi.json";
const String deviceSettingsFile = "/config/device.json";
const String statusFile = "/status.json";

Settings::Settings(Storage *storage)
{
  _storage = storage;
}

void Settings::load()
{
  loadDeviceSettings();
  loadWifiSettings();
}

void Settings::store()
{
  storeDeviceSettings();
  storeWifiSettings();
}

void Settings::storeDeviceSettings()
{
  StaticJsonDocument<1024> deviceSettings;

  deviceSettings["deviceName"] = deviceName;
  deviceSettings["buttonMode"] = buttonMode;
  deviceSettings["invertSwitch"] = invertSwitch;

  LoggerSettings::writeToJson(deviceSettings);

  deviceSettings["mqttEnabled"] = mqttEnabled;
  deviceSettings["mqttServer"] = mqttServer;
  deviceSettings["mqttServerPort"] = mqttServerPort;
  deviceSettings["mqttUserName"] = mqttUserName;
  deviceSettings["mqttPassword"] = mqttPassword;
  deviceSettings["mqttSubscribeTopic"] = mqttSubscribeTopic;
  deviceSettings["mqttPublishTopic"] = mqttPublishTopic;

  deviceSettings["customButtonGpio"] = customButtonGpio;
  deviceSettings["customRelayGpio"] = customRelayGpio;

  deviceSettings["emulateRelay"] = emulateRelay;

  String newSettings;
  serializeJsonPretty(deviceSettings, newSettings);
  _storage->writeFile(deviceSettingsFile, newSettings);
}

void Settings::loadDeviceSettings()
{
  String storedSettings = _storage->readFile(deviceSettingsFile);
  if (!storedSettings || storedSettings == "")
  {
    Serial.println("Found no device identity settings, using default values:");
    hostName = "SwitchIt" + String(ESP.getFlashChipId());
    deviceName = "light";
  }
  else
  {
    StaticJsonDocument<1024> deviceSettings;
    deserializeJson(deviceSettings, storedSettings);

    deviceName = deviceSettings["deviceName"].as<String>();
    buttonMode = deviceSettings["buttonMode"].as<int>();
    invertSwitch = deviceSettings["invertSwitch"].as<bool>();

    LoggerSettings::readFromJson(deviceSettings);

    mqttEnabled = deviceSettings["mqttEnabled"].as<bool>();
    mqttServer = deviceSettings["mqttServer"].as<String>();
    mqttServerPort = deviceSettings["mqttServerPort"].as<int>();
    mqttUserName = deviceSettings["mqttUserName"].as<String>();
    mqttPassword = deviceSettings["mqttPassword"].as<String>();
    mqttSubscribeTopic = deviceSettings["mqttSubscribeTopic"].as<String>();
    mqttPublishTopic = deviceSettings["mqttPublishTopic"].as<String>();

    customButtonGpio = deviceSettings["customButtonGpio"].as<int>();
    customRelayGpio = deviceSettings["customRelayGpio"].as<int>();

    emulateRelay = deviceSettings["emulateRelay"].as<bool>();

#ifdef FULLDEBUG
    serializeJsonPretty(deviceSettings, Serial);
#endif
  }

  Serial.println("HostName=" + hostName + ", DeviceName=" + deviceName);
}

void Settings::loadWifiSettings()
{
  String storedSettings = _storage->readFile(wifiSettingsFile);
  if (!storedSettings || storedSettings == "")
  {
    Serial.println("Found no WIFI client settings.");
    return;
  }

  StaticJsonDocument<512> wifiSettings;
  deserializeJson(wifiSettings, storedSettings);
  NetworkSettings::readFromJson(wifiSettings);
}

void Settings::storeWifiSettings()
{
  StaticJsonDocument<512> wifiSettings;
  NetworkSettings::writeToJson(wifiSettings);
  String newSettings;
  serializeJsonPretty(wifiSettings, newSettings);
  _storage->writeFile(wifiSettingsFile, newSettings);
}

String Settings::getStateSummary(String statusCode)
{
  String result;
  StaticJsonDocument<512> statusJson;
  statusJson["device"] = deviceName;
  statusJson["state"] = statusCode;
  serializeJsonPretty(statusJson, result);
  return result;
}

void Settings::storeState(String statusCode)
{
  _storage->writeFile(statusFile, getStateSummary(statusCode));
}

String Settings::getStoredState()
{
  String storedStatus = _storage->readFile(statusFile);

  if (storedStatus && storedStatus != "")
  {
    StaticJsonDocument<512> statusJson;
    ;
    if (deserializeJson(statusJson, storedStatus) == DeserializationError::Ok)
    {
#ifdef DEBUG
      Serial.println("Read startup state, applying it.");
      Serial.println(storedStatus);
#endif
      return statusJson["state"];
    }
  }

  return "OFF";
}
