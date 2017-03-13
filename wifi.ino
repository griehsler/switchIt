#include <ESP8266WiFi.h>

const String wifiSettingsFile = "/config/wifi.txt";
const char separator = '#';

void setupNetwork()
{
  if (!tryLoadWifiSettings() || !connectToWifi())
    startAP();
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

bool connectToWifi()
{
  Serial.print("trying to connect to '" + otherAPSSID + "' ");

  WiFi.mode(WIFI_STA);
  WiFi.begin(otherAPSSID.c_str(), otherAPPassword.c_str());

  for (int i = 0; i < 60; i++)
  {
    Serial.print(".");
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("");
      Serial.print("connected, IP Address: ");
      Serial.println(WiFi.localIP());
      return true;
    }
    delay(1000);
  }

  Serial.println("");
  Serial.println("timed out.");
  return false;
}

void startAP()
{
  String ssid = "WiLight" + String(ESP.getFlashChipId());
  IPAddress ip(192, 168, 0, 1);
  IPAddress subnet(255, 255, 255, 0);

  Serial.println("starting WIFI AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid.c_str(), "test1234");
  Serial.println("started WIFI AP '" + ssid + "'");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void storeWiFiSettings()
{
  String newSettings = otherAPSSID + separator + otherAPPassword;
  storeFile(wifiSettingsFile, newSettings);
}

