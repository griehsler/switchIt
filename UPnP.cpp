#include "UPnP.h"

#include <functional>
#include <ESP8266mDNS.h>

UPnP::UPnP(HTTPServer *http, Settings *settings, HTMLProvider *htmlProvider, Commands *commands)
{
  http = http;
  _settings = settings;
  _htmlProvider = htmlProvider;
  _commands = commands;

  ipMulti = IPAddress(239, 255, 255, 250);
}

void UPnP::connectUDP()
{
  prepareIds();

  Serial.print("Connecting to UDP ... ");

  if (UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti))
  {
    Serial.println("successful");
    MDNS.addService("ssdp", "udp", portMulti);
  }
  else
    Serial.println("failed!");
}

void UPnP::respondToSearch()
{
#ifdef DEBUG
  Serial.print("Responding to search request back to ");
  Serial.print(UDP.remoteIP());
  Serial.print(":");
  Serial.println(UDP.remotePort());
#endif

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  String serverUrl = String(s) + ":" + String(_settings->httpServerPort);

  String response = _htmlProvider->getSsdpSearchResponse(serverUrl, persistentUuid, serviceType);
  UDP.beginPacket(UDP.remoteIP(), UDP.remotePort());
  UDP.write(response.c_str());
  UDP.endPacket();

#ifdef DEBUG
  Serial.println("Response sent");
#endif
#ifdef DEBUG
  Serial.print(response);
#endif
}

void UPnP::loop()
{
  int packetSize = UDP.parsePacket();

  if (packetSize)
  {
    int len = UDP.read(packetBuffer, 255);
    if (len > 0)
      packetBuffer[len] = 0;

    String request = packetBuffer;

#ifdef FULLDEBUG
    Serial.println("");
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");

    IPAddress remote = UDP.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
        Serial.print(".");
    }

    Serial.print(", port ");
    Serial.println(UDP.remotePort());
    Serial.println("Request:");
    Serial.println(request);
#endif

    if (request.indexOf("M-SEARCH") != -1 && request.indexOf(serviceType) != -1)
      respondToSearch();
  }
}

void UPnP::prepareIds()
{
  uint32_t chipId = ESP.getChipId();
  char uuid[64];
  sprintf_P(uuid, PSTR("38323636-4558-4dda-9188-cda0e6%02x%02x%02x"),
            (uint16_t)((chipId >> 16) & 0xff),
            (uint16_t)((chipId >> 8) & 0xff),
            (uint16_t)chipId & 0xff);

  serial = String(uuid);
  persistentUuid = "Socket-1_0-" + serial;
}

void UPnP::extendWebServer()
{
  _http->server.on("/upnp/control/basicevent1", HTTP_POST, std::bind(&UPnP::handleBasicEventRequest, this));
  _http->server.on("/eventservice.xml", HTTP_GET, std::bind(&UPnP::handleEventServiceRequest, this));
  _http->server.on("/setup.xml", HTTP_GET, std::bind(&UPnP::handleSetupRequest, this));
}

void UPnP::handleBasicEventRequest()
{
  String request = _http->server.arg(0);

#ifdef DEBUG
  Serial.println("Responding to  /upnp/control/basicevent1 ...");

  for (int x = 0; x <= _http->server.args(); x++)
  {
    Serial.println(_http->server.arg(x));
  }

  Serial.print("request:");
  Serial.println(request);
#endif

  String reply = "";
  if (request.indexOf("<BinaryState>1</BinaryState>") > 0)
  {
    Serial.println("Got Turn on request via UPNP");
    _commands->on();
  }

  if (request.indexOf("<BinaryState>0</BinaryState>") > 0)
  {
    Serial.println("Got Turn off request via UPNP");
    _commands->off();
  }

  _http->server.send(200, "text/plain", reply);
}

void UPnP::handleEventServiceRequest()
{
#ifdef DEBUG
  Serial.println("Responding to eventservice.xml ...");
#endif

  String content = _htmlProvider->getEventServiceXml();
  _http->server.send(200, "text/plain", content.c_str());

#ifdef DEBUG
  Serial.println("Sending:");
  Serial.println(content);
#endif
}

void UPnP::handleSetupRequest()
{
#ifdef DEBUG
  Serial.println("Responding to setup.xml ...");
#endif

  String content = _htmlProvider->getSetupXml(_settings->deviceName, persistentUuid, serial);
  _http->server.send(200, "text/xml", content.c_str());

#ifdef DEBUG
  Serial.println("Sending:");
  Serial.println(content);
#endif
}
