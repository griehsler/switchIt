#include <PubSubClient.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

String mqttClientId = String(ESP.getChipId());

void setupMQTT()
{
  if (!_settings.mqttEnabled)
  {
    Serial.println("MQTT is not enabled.");
    return;
  }

  client.setServer(_settings.mqttServer.c_str(), _settings.mqttServerPort);
  client.setCallback(mqttCallback);

  mqttClientId = "ESP8266Client-";
  mqttClientId += String(random(0xffff), HEX);

#ifdef DEBUG
  Serial.print("MQTT client id: ");
  Serial.println(mqttClientId);
#endif

  Serial.print("Connecting to MQTT server ");
  Serial.print(_settings.mqttServer);
  Serial.print(" ... ");
  if (!client.connect(mqttClientId.c_str(), _settings.mqttUserName.c_str(), _settings.mqttPassword.c_str()))
  {
    Serial.print("failed. rc=");
    Serial.println(client.state());
    return;
  }
  else
  {
    Serial.println("done");
    reportStatus();
    client.subscribe(_settings.mqttSubscribeTopic.c_str());
    Serial.print("subscribed to MQTT topic: ");
    Serial.println(_settings.mqttSubscribeTopic);
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
  client.publish(_settings.mqttPublishTopic.c_str(), message.c_str());

#ifdef DEBUG
  Serial.println("published message to MQTT topic '" + _settings.mqttPublishTopic + "': " + message);
#endif
}

void reportStatus()
{
  if (_settings.mqttEnabled && client.connected())
    publishMessage(_settings.getStatus(getRelayState()));
}

bool clientDown = !_settings.mqttEnabled;

void handleMQTT()
{
  if (_settings.mqttEnabled && !clientDown)
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
