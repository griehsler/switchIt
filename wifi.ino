#include <ESP8266WiFi.h>

void setupNetwork()
{
  if (!tryLoadWifiSettings() || !connectToWifi())
    startAP();
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
  String ssid = "SwitchIt" + String(ESP.getFlashChipId());
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
