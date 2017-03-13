#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

const unsigned int httpServerPort = 80;
ESP8266WebServer server(httpServerPort);
ESP8266HTTPUpdateServer httpUpdater;

void prepareHttpServer()
{
  server.on("/", handleRoot);
  server.on("/config", handleConfig);
  server.on("/command", handleCommand);
  server.onNotFound(handleNotFound);
  httpUpdater.setup(&server);
}

void startHttpServer()
{
  server.begin();
  Serial.println("HTTP server started");
}

void handleHttpRequest()
{
  server.handleClient();
}

void handleRoot()
{
  sendSectionContent(getSectionContent("index"));
}

void handleConfig()
{
  String message = "";

  bool updatedSettings = server.args() > 0;
  if (updatedSettings)
  {
    Serial.println("Assigning new WIFI settings.");
    otherAPSSID = server.arg("ssid");
    otherAPPassword = server.arg("password");

    message = "Stored new settings";
  }

  String sectionContent = getSectionContent("config");
  sectionContent = insertTemplateValue(sectionContent, "message", message);
  sectionContent = insertTemplateValue(sectionContent, "ssid", otherAPSSID);
  sendSectionContent(sectionContent);

  if (updatedSettings)
  {
    storeWiFiSettings();
    onSettingsChanged();
  }
}

void handleCommand()
{
  Serial.print("Received command via web: ");
  Serial.println(server.arg("name"));
  
  if (!executeCommand(server.arg("name")))
    server.send(400, "text/plain", "unknown command");
  else
    handleRoot();
}

String getSectionContent(String sectionName)
{
  return openFile("/http/" + sectionName + ".html");
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
