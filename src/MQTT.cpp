#include "MQTT.h"

MQTT::MQTT(Settings *settings)
{
  _settings = settings;
}

void MQTT::setup(MQTT_MESSAGE_CALLBACK)
{
  using namespace std::placeholders; // for `_1`

  if (!_settings->mqttEnabled)
  {
    Serial.println("MQTT is not enabled.");
    return;
  }

  this->_callback = _callback;

  _client.setServer(_settings->mqttServer.c_str(), _settings->mqttServerPort);
  _client.setCallback(std::bind(&MQTT::mqttCallback, this, _1, _2, _3));

  connect();
}

void MQTT::mqttCallback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String message = (char *)payload;

  Serial.print("MQTT Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  String response;
  _callback(message, &response);

  if (response && response.length() > 0)
    publishMessage(response);
}

bool MQTT::connect()
{
  String mqttClientId = "ESP8266Client-";
  mqttClientId += String(random(0xffff), HEX);

  Serial.print("MQTT client id: ");
  Serial.println(mqttClientId);

  Serial.print("Connecting to MQTT server ");
  Serial.print(_settings->mqttServer);
  Serial.print(" ... ");

  if (!_client.connect(mqttClientId.c_str(), _settings->mqttUserName.c_str(), _settings->mqttPassword.c_str()))
  {
    Serial.print("failed. rc=");
    Serial.println(_client.state());
    return false;
  }

  Serial.println("done");
  _client.subscribe(_settings->mqttSubscribeTopic.c_str());
  Serial.print("subscribed to MQTT topic: ");
  Serial.println(_settings->mqttSubscribeTopic);
  return true;
}

void MQTT::publishMessage(String message)
{
  _client.publish(_settings->mqttPublishTopic.c_str(), message.c_str());

#ifdef DEBUG
  Serial.println("published message to MQTT topic '" + _settings->mqttPublishTopic + "': " + message);
#endif
}

void MQTT::reportStatus(String statusCode)
{
  if (_settings->mqttEnabled && _client.connected())
  {
    publishMessage(_settings->getStateSummary(statusCode));
  }
}

void MQTT::loop()
{
  if (_settings->mqttEnabled)
  {
#ifdef FULLDEBUG
    Serial.println("check MQTT connection status");
#endif
    bool connected = _client.connected();
    if (!connected)
    {
      Serial.println("Lost MQTT connection!");
      connected = connect();
    }

    if (connected)
    {
#ifdef FULLDEBUG
      Serial.println("MQTT loop client");
#endif
      _client.loop();
    }
  }
}
