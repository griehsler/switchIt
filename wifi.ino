#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

void setupNetwork()
{
  if (!tryLoadWifiSettings() || !connectToWifi())
    startAP();
  else
  {
  if (!MDNS.begin(hostName.c_str(), WiFi.localIP()))
    Serial.println("Error setting up mDNS responder for device name '" + hostName + "'!");
  else
    Serial.println("mDNS responder for device name " + hostName + ".local started.");
  }
}

bool connectToWifi()
{
  Serial.print("trying to connect to '" + otherAPSSID + "' ");

  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostName);
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
  IPAddress ip(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  Serial.println("starting WIFI AP");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, subnet);
  WiFi.softAP(ssid.c_str(), "Test1234");
  Serial.println("started WIFI AP '" + ssid + "'");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}
