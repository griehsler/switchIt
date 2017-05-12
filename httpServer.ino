#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>

const unsigned int httpServerPort = 80;

ESP8266WebServer server(httpServerPort);
ESP8266HTTPUpdateServer httpUpdater;

String dummyPassword = "*keep*old*";

void prepareHttpServer()
{
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/command", handleCommand);
  server.onNotFound(handleNotFound);
  httpUpdater.setup(&server);
  MDNS.addService("http", "tcp", 80);
}

void startHttpServer()
{
  server.begin();
  Serial.println("HTTP server started");
}

void handleHttpRequest()
{
  server.handleClient();
  MDNS.update();
}

void handleRoot()
{
  sendSectionContent(getIndexHtml());
}

void handleConfig()
{
  String message = "";

  bool updatedSettings = server.args() > 0;
  if (updatedSettings)
  {
    String operation = server.arg("operation");
    if (operation == "store")
    {
      otherAPSSID = server.arg("ssid");
      
      String newPassword = server.arg("password");
      if (newPassword != dummyPassword)
        otherAPPassword = newPassword;

      hostName = server.arg("hostname");
      deviceName = server.arg("devicename");

      storeWifiSettings();
      storeDeviceSettings();

      Serial.println("Stored new settings.");
    }
    else if (operation == "reset")
    {
      Serial.println("Performing factory reset");
      deleteAllFiles();
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

  sendSectionContent(getConfigHtml(message));

  if (updatedSettings)
    onSettingsChanged();
}

void handleCommand()
{
  Serial.print("Received command via web: ");
  Serial.println(server.arg("name"));

  String reply;
  if (!executeCommand(server.arg("name"), &reply))
    server.send(400, "text/plain", "unknown command");
  else if (reply && reply != "")
    server.send(200, "text/html", reply);
  else
    handleRoot();
}

String insertTemplateValue(String content, String placeHolder, String value)
{
  // add HTML escaping!
  String pl = "{" + placeHolder + "}";
  content.replace(pl, value);
  return content;
}

void sendSectionContent(String content)
{
  if (!content)
    handleNotFound();

  server.send(200, "text/html", content);
}

void handleNotFound()
{
  server.send(404, "text/plain", "Invalid address!");
  Serial.println("Rejected access to '" + server.uri() + "'.");
}
