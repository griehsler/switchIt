#include "HTTPServer.h"

#include <ESP8266mDNS.h>

HTTPServer::HTTPServer(Settings *settings, HTMLProvider *htmlProvider, Commands *commands, Storage *storage)
{
  _settings = settings;
  _commands = commands;
  _htmlProvider = htmlProvider;
  _storage = storage;

  server = ESP8266WebServer(_settings->httpServerPort);
}

void HTTPServer::prepareHttpServer()
{
  server.on("/", std::bind(&HTTPServer::handleRoot, this));
  server.on("/config", std::bind(&HTTPServer::handleConfig, this));
  server.on("/command", std::bind(&HTTPServer::handleCommand, this));
  server.onNotFound(std::bind(&HTTPServer::handleNotFound, this));
  _httpUpdater.setup(&server);
  MDNS.addService("http", "tcp", 80);
}

void HTTPServer::startHttpServer()
{
  server.begin();
  Serial.println("HTTP server started");
}

void HTTPServer::loop()
{
  server.handleClient();
  MDNS.update();
}

void HTTPServer::handleRoot()
{
  sendSectionContent(_htmlProvider->getIndexHtml());
}

void HTTPServer::handleConfig()
{
  String message = "";

  bool updatedSettings = server.args() > 0;
  if (updatedSettings)
  {
    String operation = server.arg("operation");
    if (operation == "store")
    {
      _settings->otherAPSSID = server.arg("ssid");

      String newPassword = server.arg("password");
      if (newPassword != dummyPassword)
        _settings->otherAPPassword = newPassword;

      _settings->hostName = server.arg("hostname");
      _settings->deviceName = server.arg("devicename");

      _settings->buttonMode = server.arg("buttonmode").toInt();

      _settings->mqttEnabled = server.arg("mqttenabled") == "enabled";
      _settings->mqttServer = server.arg("mqttserver");
      _settings->mqttServerPort = server.arg("mqttserverport").toInt();
      _settings->mqttUserName = server.arg("mqttusername");
      String newMqttPassword = server.arg("mqttpassword");
      if (newMqttPassword != dummyPassword)
        _settings->mqttPassword = newMqttPassword;
      _settings->mqttSubscribeTopic = server.arg("mqttsubscribe");
      _settings->mqttPublishTopic = server.arg("mqttpublish");

      _settings->emulateRelay = server.arg("emulaterelay") == "enabled";

      _settings->storeWifiSettings();
      _settings->storeDeviceSettings();

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

void HTTPServer::handleCommand()
{
  Serial.print("Received command via web: ");
  Serial.println(server.arg("name"));

  String reply;
  if (!_commands->execute(server.arg("name"), &reply))
    server.send(400, "text/plain", "unknown command");
  else if (reply && reply != "")
    server.send(200, "text/html", reply);
  else
    handleRoot();
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

  server.send(200, "text/html", content);
}

void HTTPServer::handleNotFound()
{
  server.send(404, "text/plain", "Invalid address!");
  Serial.println("Rejected access to '" + server.uri() + "'.");
}