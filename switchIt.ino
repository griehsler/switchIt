#include <ESP8266WebServer.h>
#include "Settings.h"
#include "Network.h"
#include "MQTT.h"
#include "HTMLProvider.h"
#include "GPIO.h"
#include "UPnP.h"

//#define DEBUG
//#define FULLDEBUG

Storage _storage;
Settings _settings(&_storage);
Network _network(&_settings);
ESP8266WebServer server(_settings.httpServerPort);
MQTT _mqtt(&_settings);
HTMLProvider _htmlProvider(&_settings);
GPIO _gpio(&_settings, &_mqtt);
Commands _commands(&_settings, &_gpio);
UPnP _upnp(&server, &_settings, &_htmlProvider, &_commands);

void setup()
{
  _gpio.setupGPIO();
  Serial.begin(115200);
  Serial.println("starting initialization ...");
  _gpio.led(true);
  _settings.loadDeviceSettings();
  _network.setupNetwork();
  prepareHttpServer();
  _upnp.extendWebServer();
  startHttpServer();
  _upnp.connectUDP();
  using namespace std::placeholders; // for `_1`
  _mqtt.setup(std::bind(&Commands::execute, _commands, _1, _2));
  _gpio.led(false);
  _gpio.applyRelayState(_settings.getStoredState());
  Serial.println("initialization finished.");
}

void loop()
{
  _gpio.loop();
  handleHttpRequest();
  _upnp.loop();
  _mqtt.loop();
}

void onSettingsChanged()
{
  ESP.restart();
  //  setupNetwork();
}

