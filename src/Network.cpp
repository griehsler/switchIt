#include "Network.h"

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

Network::Network(Settings *settings)
{
  _settings = settings;
}

void Network::setup()
{
  if (!_settings->otherAPSSID || _settings->otherAPSSID == "" || !connectToWifi())
    startAP();
  else
  {
    if (!MDNS.begin(_settings->hostName.c_str(), WiFi.localIP()))
      Serial.println("Error setting up mDNS responder for device name '" + _settings->hostName + "'!");
    else
      Serial.println("mDNS responder for device name " + _settings->hostName + ".local started.");
  }
}

bool Network::connectToWifi()
{
  Serial.print("trying to connect to '" + _settings->otherAPSSID + "' ");

  WiFi.mode(WIFI_STA);
  WiFi.hostname(_settings->hostName);
  WiFi.begin(_settings->otherAPSSID.c_str(), _settings->otherAPPassword.c_str());

  for (int i = 0; i < 60; i++)
  {
    Serial.print(".");
    if (WiFi.isConnected())
    {
      Serial.println("");
      Serial.print("connected, IP Address: ");
      Serial.println(WiFi.localIP());
      wasConnected = true;
      reconnectCount = 0;
      return true;
    }
    delay(1000);
  }

  Serial.println("");
  Serial.println("timed out.");
  return false;
}

void Network::startAP()
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

void Network::loop()
{
  if (wasConnected && !WiFi.isConnected())
  {
    Serial.println("lost connection to WIFI!");
    if (!connectToWifi())
    {
      reconnectCount++;
      Serial.print("failed for ");
      Serial.print(reconnectCount);
      Serial.println(" times!");

      if (reconnectCount >= 10)
      {
        Serial.println("falling back to AP mode.");
        startAP();
      }
    }
  }
}

