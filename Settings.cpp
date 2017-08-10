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
  StaticJsonBuffer<512> jsonBuffer;

  JsonObject &deviceSettings = jsonBuffer.createObject();
  deviceSettings["hostName"] = hostName;
  deviceSettings["deviceName"] = deviceName;
  deviceSettings["buttonMode"] = buttonMode;

  deviceSettings["syslogEnabled"] = syslogEnabled;
  deviceSettings["syslogServer"] = syslogServer;
  deviceSettings["syslogServerPort"] = syslogServerPort;

  deviceSettings["mqttEnabled"] = mqttEnabled;
  deviceSettings["mqttServer"] = mqttServer;
  deviceSettings["mqttServerPort"] = mqttServerPort;
  deviceSettings["mqttUserName"] = mqttUserName;
  deviceSettings["mqttPassword"] = mqttPassword;
  deviceSettings["mqttSubscribeTopic"] = mqttSubscribeTopic;
  deviceSettings["mqttPublishTopic"] = mqttPublishTopic;

  deviceSettings["emulateRelay"] = emulateRelay;

  String newSettings;
  deviceSettings.prettyPrintTo(newSettings);
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
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &deviceSettings = jsonBuffer.parseObject(storedSettings);

    hostName = deviceSettings["hostName"].as<String>();
    deviceName = deviceSettings["deviceName"].as<String>();
    buttonMode = deviceSettings["buttonMode"].as<int>();

    syslogEnabled = deviceSettings["syslogEnabled"].as<bool>();
    syslogServer = deviceSettings["syslogServer"].as<String>();
    syslogServerPort = deviceSettings["syslogServerPort"].as<int>();

    mqttEnabled = deviceSettings["mqttEnabled"].as<bool>();
    mqttServer = deviceSettings["mqttServer"].as<String>();
    mqttServerPort = deviceSettings["mqttServerPort"].as<int>();
    mqttUserName = deviceSettings["mqttUserName"].as<String>();
    mqttPassword = deviceSettings["mqttPassword"].as<String>();
    mqttSubscribeTopic = deviceSettings["mqttSubscribeTopic"].as<String>();
    mqttPublishTopic = deviceSettings["mqttPublishTopic"].as<String>();

    emulateRelay = deviceSettings["emulateRelay"].as<bool>();

#ifdef FULLDEBUG
    deviceSettings.prettyPrintTo(Serial);
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

  StaticJsonBuffer<512> jsonBuffer;
  JsonObject &deviceSettings = jsonBuffer.parseObject(storedSettings);
  otherAPSSID = deviceSettings["ssid"].as<String>();
  otherAPPassword = deviceSettings["password"].as<String>();
}

void Settings::storeWifiSettings()
{
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject &wifiSettings = jsonBuffer.createObject();
  wifiSettings["ssid"] = otherAPSSID;
  wifiSettings["password"] = otherAPPassword;
  String newSettings;
  wifiSettings.prettyPrintTo(newSettings);
  _storage->writeFile(wifiSettingsFile, newSettings);
}

String Settings::getStateSummary(String statusCode)
{
  String result;
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject &statusJson = jsonBuffer.createObject();
  statusJson["device"] = deviceName;
  statusJson["state"] = statusCode;
  statusJson.prettyPrintTo(result);
  return result;
}

void Settings::storeState(String statusCode)
{
  _storage->writeFile(statusFile, getStateSummary(statusCode));
}

String Settings::getStoredState()
{
  String storedStatus = _storage->readFile(statusFile);
  if (storedStatus)
  {
    StaticJsonBuffer<512> jsonBuffer;
    JsonObject &statusJson = jsonBuffer.parseObject(storedStatus);
    if (statusJson.success())
    {
#ifdef DEBUG
      Serial.println("Read startup state, applying it.");
      Serial.println(storedStatus);
#endif
      return statusJson["state"];
    }
  }
}
