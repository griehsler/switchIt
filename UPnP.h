#pragma once

#include "HTTPServer.h"
#include "Settings.h"
#include "HTMLProvider.h"
#include "Commands.h"
#include "Logger.h"
#include <WiFiUdp.h>

class UPnP
{
public:
  UPnP(Logger *logger, HTTPServer *http, Settings *settings, HTMLProvider *_htmlProvider, Commands *commands, GPIO *gpio);
  void setup();
  void loop();

private:
  const String serviceType = "urn:Belkin:device:**";
  // const String serviceType = "upnp:rootdevice";
  const String searchServiceType = "ssdp:all";

  Settings *_settings;
  Logger *_logger;
  HTMLProvider *_htmlProvider;
  HTTPServer *_http;
  Commands *_commands;
  GPIO *_gpio;
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