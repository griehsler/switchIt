const String wifiSettingsFile = "/config/wifi.txt";
const String deviceSettingsFile = "/config/device.txt";
const char separator = '#';

void storeDeviceSettings()
{
  String newSettings = hostName + separator + deviceName;
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
    int separatorPos = storedSettings.indexOf(separator);
    if (separatorPos == -1)
    {
      Serial.print("Could not find the separator ");
      Serial.print(separator);
      Serial.println(" in settings data.");
      return;
    }

    hostName = storedSettings.substring(0, separatorPos);
    deviceName = storedSettings.substring(separatorPos + 1);
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

  int separatorPos = storedSettings.indexOf(separator);
  if (separatorPos == -1)
  {
    Serial.print("Could not find the separator ");
    Serial.print(separator);
    Serial.println(" in settings data.");
    return false;
  }

  otherAPSSID = storedSettings.substring(0, separatorPos);
  otherAPPassword = storedSettings.substring(separatorPos + 1);

  return true;
}

void storeWifiSettings()
{
  String newSettings = otherAPSSID + separator + otherAPPassword;
  storeFile(wifiSettingsFile, newSettings);
}
