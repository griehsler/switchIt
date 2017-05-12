#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);
char msg[50];

String mqttServer = "newton.home";
bool mqttEnabled = true;
int mqttServerPort = 1883;
String mqttClientId = String(ESP.getChipId());
String mqttUserName = "tester";
String mqttPassword = "test";
String mqttPublishTopic = "light/status";
String mqttSubscribeTopic = "light/tester/command";

void setupMQTT()
{
  if (!mqttEnabled)
  {
    Serial.println("MQTT is not enabled.");
    return;
  }

  client.setServer(mqttServer.c_str(), mqttServerPort);
  client.setCallback(mqttCallback);

  mqttClientId = "ESP8266Client-";
  mqttClientId += String(random(0xffff), HEX);

#ifdef DEBUG
  Serial.print("MQTT client id: ");
  Serial.println(mqttClientId);
#endif

  Serial.print("Connecting to MQTT server ");
  Serial.print(mqttServer);
  Serial.print(" ... ");
  if (!client.connect(mqttClientId.c_str(), mqttUserName.c_str(), mqttPassword.c_str()))
  {
    Serial.print("failed. rc=");
    Serial.println(client.state());
    return;
  }
  else
  {
    Serial.println("done");
    reportStatus();
    client.subscribe(mqttSubscribeTopic.c_str());
    Serial.print("subscribed to MQTT topic: ");
    Serial.println(mqttSubscribeTopic);
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  payload[length] = '\0';
  String message = (char*)payload;

  Serial.print("MQTT Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  String response;
  executeCommand(message, &response);

  if (response && response.length() > 0)
    publishMessage(response);
}

void publishMessage(String message)
{
  client.publish(mqttPublishTopic.c_str(), message.c_str());

#ifdef DEBUG
  Serial.println("published message to MQTT topic '" + mqttPublishTopic + "': " + message);
#endif
}

void reportStatus()
{
  if (mqttEnabled && client.connected())
    publishMessage(getStatus());
}

bool clientDown = !mqttEnabled;

void handleMQTT()
{
  if (mqttEnabled && !clientDown)
  {
    if (!client.connected())
    {
      Serial.println("Lost MQTT connection!");
      clientDown = true;
    }
    else
      client.loop();
  }
}

