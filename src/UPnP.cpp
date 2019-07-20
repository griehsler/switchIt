#include "UPnP.h"

#include <functional>
#include <ESP8266mDNS.h>

//#define DEBUG
//#define FULLDEBUG

UPnP::UPnP(Logger *logger, HTTPServer *http, Settings *settings, HTMLProvider *htmlProvider, Commands *commands, GPIO *gpio)
{
  _logger = logger;
  _http = http;
  _settings = settings;
  _htmlProvider = htmlProvider;
  _commands = commands;
  _gpio = gpio;

  _ipMulti = IPAddress(239, 255, 255, 250);

  prepareIds();
}

void UPnP::setup()
{
  extendWebServer();
  connectUDP();
}

void UPnP::connectUDP()
{
  Serial.print("Connecting to UDP ... ");

  if (_UDP.beginMulticast(WiFi.localIP(), _ipMulti, _portMulti))
  {
    Serial.println("successful");
    MDNS.addService("ssdp", "udp", _portMulti);
  }
  else
    Serial.println("failed!");
}

void UPnP::respondToSearch()
{
#ifdef DEBUG
  Serial.print("Responding to search request back to ");
  Serial.print(_UDP.remoteIP());
  Serial.print(":");
  Serial.println(_UDP.remotePort());
#endif

  IPAddress localIP = WiFi.localIP();
  char s[16];
  sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  String serverUrl = String(s) + ":" + String(_settings->httpServerPort);

  String response = _htmlProvider->getSsdpSearchResponse(serverUrl, _persistentUuid, serviceType);
  _UDP.beginPacket(_UDP.remoteIP(), _UDP.remotePort());
  _UDP.write(response.c_str());
  _UDP.endPacket();

#ifdef DEBUG
  Serial.println("Response sent");
#endif
#ifdef FULLDEBUG
  Serial.print(response);
#endif
}

void UPnP::loop()
{
  int packetSize = _UDP.parsePacket();

  if (packetSize)
  {
    int len = _UDP.read(_packetBuffer, 255);
    if (len > 0)
      _packetBuffer[len] = 0;

    String request = _packetBuffer;

#ifdef FULLDEBUG
    Serial.println("");
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");

    IPAddress remote = _UDP.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
        Serial.print(".");
    }

    Serial.print(", port ");
    Serial.println(_UDP.remotePort());
    Serial.println("Request:");
    Serial.println(request);
#endif

    if (request.indexOf("M-SEARCH") != -1 && request.indexOf(searchServiceType) != -1)
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

  _serial = String(uuid);
  _persistentUuid = "Socket-1_0-" + _serial;
}

void UPnP::extendWebServer()
{
#ifdef DEBUG
  Serial.print("Registering UPnP resources in http server ... ");
#endif

  _http->server->on("/upnp/control/basicevent1", HTTP_POST, std::bind(&UPnP::handleBasicEventRequest, this));
  _http->server->on("/eventservice.xml", HTTP_GET, std::bind(&UPnP::handleEventServiceRequest, this));
  _http->server->on("/setup.xml", HTTP_GET, std::bind(&UPnP::handleSetupRequest, this));

#ifdef DEBUG
  Serial.println("done");
#endif
}

void UPnP::handleBasicEventRequest()
{
  String request = _http->server->arg(0);

#ifdef DEBUG
  Serial.println("Responding to  /upnp/control/basicevent1 ...");
#endif

  String reply = "";
  if (request.indexOf("GetBinaryState") > 0)
  {
#ifdef DEBUG
    Serial.println("Got state inquiry request via UPNP");
#endif

    String answer = "0";
    if (_gpio->getRelayState() == "ON")
      answer = "1";
    reply = _htmlProvider->getSwitchCommandResponse("Get", answer);
  }
  else if (request.indexOf("<BinaryState>1</BinaryState>") > 0)
  {
#ifdef DEBUG
    Serial.println("Got turn on request via UPNP");
#endif
    _logger->writeLog(LOG_INFO, "Got turn on request via UPNP");
    _commands->on();
    reply = _htmlProvider->getSwitchCommandResponse("Set", "1");
  }

  else if (request.indexOf("<BinaryState>0</BinaryState>") > 0)
  {
#ifdef DEBUG
    Serial.println("Got turn off request via UPNP");
#endif
    _logger->writeLog(LOG_INFO, "Got turn off request via UPNP");
    _commands->off();
    reply = _htmlProvider->getSwitchCommandResponse("Set", "0");
  }

  _http->server->send(200, "text/xml", reply);

#ifdef FULLDEBUG
  Serial.print("response:");
  Serial.println(reply);
#endif
}

void UPnP::handleEventServiceRequest()
{
#ifdef DEBUG
  Serial.println("Responding to eventservice.xml ...");
#endif

  String content = _htmlProvider->getEventServiceXml();
  _http->server->send(200, "text/plain", content.c_str());

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

  String content = _htmlProvider->getSetupXml(_settings->deviceName, _persistentUuid, _serial);
  _http->server->send(200, "text/xml", content.c_str());

#ifdef DEBUG
  Serial.println("Sending:");
  Serial.println(content);
#endif
}
