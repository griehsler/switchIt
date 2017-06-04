#pragma once

#include <PubSubClient.h>
#include <functional>
#include "Settings.h"

#define MQTT_MESSAGE_CALLBACK std::function<void(String, String*)> _callback

class MQTT
{
  public:
    MQTT(Settings *settings);
    void setup(MQTT_MESSAGE_CALLBACK);
    void reportStatus(String statusCode);
    void loop();

  private:
    Settings *_settings;
    PubSubClient _client;
    MQTT_MESSAGE_CALLBACK;
    String _mqttClientId;

    void publishMessage(String message);
    void mqttCallback(char *topic, byte *payload, unsigned int length);
};