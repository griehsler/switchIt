#include "Settings.h"
#include "Network.h"
#include "MQTT.h"
#include "HTTPServer.h"
#include "HTMLProvider.h"
#include "GPIO.h"
#include "UPnP.h"

//#define DEBUG
//#define FULLDEBUG

Storage _storage;
Settings _settings(&_storage);
Network _network(&_settings);
HTMLProvider _htmlProvider(&_settings);
MQTT _mqtt(&_settings);
GPIO _gpio(&_settings, &_mqtt);
Commands _commands(&_settings, &_gpio);
HTTPServer _http(&_settings, &_htmlProvider, &_commands, &_storage);
UPnP _upnp(&_http, &_settings, &_htmlProvider, &_commands);

void setup()
{
  using namespace std::placeholders; // for `_1`

  Serial.begin(115200);
  _gpio.setup();
  Serial.println("starting initialization ...");
  _gpio.led(true);
  _settings.load();
  _network.setup();
  _upnp.setup();
  _http.start();
  _mqtt.setup(std::bind(&Commands::execute, _commands, _1, _2));
  _gpio.led(false);
  _gpio.restoreLastState();
  Serial.println("initialization finished.");
}

void loop()
{
  _gpio.loop();
  _http.loop();
  _upnp.loop();
  _mqtt.loop();
}
