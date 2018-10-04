#pragma once

#include "Settings.h"
#include "HTMLProvider.h"
#include "Commands.h"
#include "Storage.h"
#include "Logger.h"

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

class HTTPServer
{
public:
  ESP8266WebServer *server;

  HTTPServer(Settings *settings, Logger *logger, HTMLProvider *htmlProvider, Commands *commands, Storage *storage);
  void setup();
  void loop();

private:
  Settings *_settings;
  Logger *_logger;
  HTMLProvider *_htmlProvider;
  Commands *_commands;
  Storage *_storage;
  ESP8266HTTPUpdateServer _httpUpdater;
  const String dummyPassword = "*keep*old*";

  void handleRoot();
  void handleConfig();
  void handleNotFound();
  void handleCommand();
  String insertTemplateValue(String content, String placeHolder, String value);
  void sendSectionContent(String content);
};