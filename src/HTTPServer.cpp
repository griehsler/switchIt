#include "HTTPServer.h"

#include <ESP8266mDNS.h>

HTTPServer::HTTPServer(ESP8266WebServer *server, Settings *settings, Logger *logger, HTMLProvider *htmlProvider, Commands *commands, Storage *storage)
{
  _settings = settings;
  _logger = logger;
  _commands = commands;
  _htmlProvider = htmlProvider;
  _storage = storage;

  this->server = server;
}

void HTTPServer::setup()
{
  // Serial.println("HTTP server starting");
  server->on("/", std::bind(&HTTPServer::handleRoot, this));
  server->on("/config", std::bind(&HTTPServer::handleConfig, this));
  server->on("/api/status", std::bind(&HTTPServer::handleApiStatus, this));

  server->onNotFound(std::bind(&HTTPServer::handleNotFound, this));
  _httpUpdater.setup(server, "/update");

  server->begin();
  Serial.println("HTTP server started");
}

void HTTPServer::loop()
{
  server->handleClient();
}

void HTTPServer::handleRoot()
{
  sendSectionContent(_htmlProvider->getIndexHtml());
}

void HTTPServer::handleConfig()
{
  String message = "";

  bool updatedSettings = server->args() > 0;
  if (updatedSettings)
  {
    String operation = server->arg("operation");
    if (operation == "store")
    {
      _settings->otherAPSSID = server->arg("ssid");

      String newPassword = server->arg("password");
      if (newPassword != dummyPassword)
        _settings->otherAPPassword = newPassword;

      _settings->hostName = server->arg("hostname");
      _settings->deviceName = server->arg("devicename");

      _settings->invertSwitch = server->arg("invertswitch") == "enabled";
      _settings->buttonMode = server->arg("buttonmode").toInt();

      _settings->syslogEnabled = server->arg("syslogenabled") == "enabled";
      _settings->syslogServer = server->arg("syslogserver");
      _settings->syslogServerPort = server->arg("syslogserverport").toInt();

      _settings->mqttEnabled = server->arg("mqttenabled") == "enabled";
      _settings->mqttServer = server->arg("mqttserver");
      _settings->mqttServerPort = server->arg("mqttserverport").toInt();
      _settings->mqttUserName = server->arg("mqttusername");
      String newMqttPassword = server->arg("mqttpassword");
      if (newMqttPassword != dummyPassword)
        _settings->mqttPassword = newMqttPassword;
      _settings->mqttSubscribeTopic = server->arg("mqttsubscribe");
      _settings->mqttPublishTopic = server->arg("mqttpublish");

      _settings->customButtonGpio = server->arg("customButtonGpio").toInt();
      _settings->customRelayGpio = server->arg("customRelayGpio").toInt();

      _settings->emulateRelay = server->arg("emulaterelay") == "enabled";

      _settings->store();

      Serial.println("Stored new settings.");
    }
    else if (operation == "reset")
    {
      Serial.println("Performing factory reset");
      _storage->deleteAllFiles();
    }
    else
    {
      Serial.println("Unsupported config operation: " + operation);
      updatedSettings = false;
      message = "Server error!";
    }

    if (message == "")
      message = "Stored new settings";
  }

  sendSectionContent(_htmlProvider->getConfigHtml(message, dummyPassword));

  if (updatedSettings)
    ESP.restart();
}

void HTTPServer::handleApiStatus()
{
  String requestBody;
  String response;

  HTTPMethod method = server->method();

#ifdef DEBUG
  Serial.print("Received ");
  Serial.print(method);
  Serial.println(" on /api/status");
#endif

  switch (method)
  {
  case HTTP_PUT:
    requestBody = server->arg("plain");
    Serial.println("Request body: " + requestBody);
    if (!_commands->execute(requestBody, &response))
    {
      server->send(400, "text/plain", "unknown command: " + requestBody);
      break;
    }
    _logger->writeLog(LOG_INFO, "Received command via api: " + requestBody);
  case HTTP_GET:
    response = _commands->getStatus();
    server->send(200, "text/json", response);
    break;
  default:
    server->send(405);
    break;
  }
}

String HTTPServer::insertTemplateValue(String content, String placeHolder, String value)
{
  // add HTML escaping!
  String pl = "{" + placeHolder + "}";
  content.replace(pl, value);
  return content;
}

void HTTPServer::sendSectionContent(String content)
{
  if (!content)
    handleNotFound();

  server->send(200, "text/html", content);
}

void HTTPServer::handleNotFound()
{
  server->send(404, "text/plain", "Invalid address!");
  Serial.println("Rejected access to '" + server->uri() + "'.");
}
