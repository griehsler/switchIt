#include <ArduinoJson.h>

StaticJsonBuffer<512> jsonBuffer;

const String wifiSettingsFile = "/config/wifi.json";
const String deviceSettingsFile = "/config/device.json";
const String statusFile = "/status.json";

void storeDeviceSettings()
{
  JsonObject& deviceSettings = jsonBuffer.createObject();
  deviceSettings["hostName"] = hostName;
  deviceSettings["deviceName"] = deviceName;
  String newSettings;
  deviceSettings.prettyPrintTo(newSettings);
  storeFile(deviceSettingsFile, newSettings);
}

void loadDeviceSettings()
{
  String storedSettings = openFile(deviceSettingsFile);
  if (!storedSettings || storedSettings == "")
  {
    Serial.println("Found no device identity settings, using default values:");
    hostName = "SwitchIt" + String(ESP.getFlashChipId());
    deviceName = "light";
  }
  else
  {
    JsonObject& deviceSettings = jsonBuffer.parseObject(storedSettings);
    hostName = deviceSettings["hostName"].as<String>();
    deviceName = deviceSettings["deviceName"].as<String>();
  }

  Serial.println("HostName=" + hostName + ".local, DeviceName=" + deviceName);
}

bool tryLoadWifiSettings()
{
  String storedSettings = openFile(wifiSettingsFile);
  if (!storedSettings || storedSettings == "")
  {
    Serial.println("Found no WIFI client settings.");
    return false;
  }

  JsonObject& deviceSettings = jsonBuffer.parseObject(storedSettings);
  otherAPSSID = deviceSettings["ssid"].as<String>();
  otherAPPassword = deviceSettings["password"].as<String>();

  return true;
}

void storeWifiSettings()
{
  JsonObject& wifiSettings = jsonBuffer.createObject();
  wifiSettings["ssid"] = otherAPSSID;
  wifiSettings["password"] = otherAPPassword;
  String newSettings;
  wifiSettings.prettyPrintTo(newSettings);
  storeFile(wifiSettingsFile, newSettings);
}

String getStatus()
{
  String result;
  JsonObject& statusJson = jsonBuffer.createObject();
  statusJson["state"] = getRelayState();
  statusJson.prettyPrintTo(result);
  return result;
}

void storeState()
{
  storeFile(statusFile, getStatus());
}

void applyState()
{
  String storedStatus = openFile(statusFile);
  if (storedStatus)
  {
    JsonObject& statusJson = jsonBuffer.parseObject(storedStatus);
    if (statusJson.success())
    {
#ifdef DEBUG
      Serial.println("Read startup state, applying it.");
      Serial.println(storedStatus);
#endif
      applyRelayState(statusJson["state"]);
    }
  }
}

