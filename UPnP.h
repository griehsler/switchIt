#pragma once

#include "HTTPServer.h"
#include "Settings.h"
#include "HTMLProvider.h"
#include "Commands.h"

#include <WiFiUdp.h>

class UPnP
{
public:
  UPnP(HTTPServer *http, Settings *settings, HTMLProvider *_htmlProvider, Commands *commands);
  void setup();
  void loop();

private:
  const String serviceType = "urn:Belkin:device:**";

  Settings *_settings;
  HTMLProvider *_htmlProvider;
  HTTPServer *_http;
  Commands *_commands;
  WiFiUDP _UDP;
  IPAddress _ipMulti;
  const unsigned int _portMulti = 1900;       // local port to listen on
  char _packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet
  String _serial;
  String _persistentUuid;

  void prepareIds();
  void connectUDP();
  void extendWebServer();

  void respondToSearch();
  void handleBasicEventRequest();
  void handleEventServiceRequest();
  void handleSetupRequest();
};