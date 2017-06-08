#pragma once

#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include "Settings.h"
#include "HTMLProvider.h"
#include "Commands.h"

class UPnP
{
  public:
    UPnP(ESP8266WebServer *server, Settings *settings, HTMLProvider *_htmlProvider, Commands *commands);
    void connectUDP();
    void extendWebServer();
    void loop();

  private:
    const String serviceType = "urn:Belkin:device:**";

    Settings *_settings;
    HTMLProvider *_htmlProvider;
    ESP8266WebServer *_server;
    Commands *_commands;
    WiFiUDP UDP;
    IPAddress ipMulti;
    const unsigned int portMulti = 1900;       // local port to listen on
    char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet
    String serial;
    String persistentUuid;

    void prepareIds();
    void respondToSearch();
    void handleBasicEventRequest();
    void handleEventServiceRequest();
    void handleSetupRequest();
};