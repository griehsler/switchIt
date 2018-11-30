#include "Settings.h"
#include "Logger.h"
#include "Network.h"
#include "MQTT.h"
#include "HTTPServer.h"
#include "HTMLProvider.h"
#include "GPIO.h"
#include "UPnP.h"

#include <ESP8266WebServer.h>

//#define DEBUG
//#define FULLDEBUG

ESP8266WebServer server(80);

Storage _storage;
Settings _settings(&_storage);
Logger _logger(&_settings);
Network _network(&_settings);
HTMLProvider _htmlProvider(&_settings);
MQTT _mqtt(&_settings);
GPIO _gpio(&_settings, &_logger, &_mqtt);
Commands _commands(&_settings, &_gpio);
HTTPServer _http(&server, &_settings, &_logger, &_htmlProvider, &_commands, &_storage);
UPnP _upnp(&_logger, &_http, &_settings, &_htmlProvider, &_commands);

void setup()
{
  using namespace std::placeholders; // for `_1`

  Serial.begin(115200);
  _gpio.setup();
  Serial.println("starting initialization ...");
  _gpio.led(true);
  _settings.load();
  _network.setup();
  _logger.setup();
  _logger.writeLog(LOG_NOTICE, "starting up");
  _upnp.setup();
  _http.setup();
  _mqtt.setup(std::bind(&Commands::execute, _commands, _1, _2));
  _gpio.led(false);
  _gpio.restoreLastState();
  Serial.println("initialization finished.");
  _logger.writeLog(LOG_INFO, "started successfully");
}

void loop()
{
  _gpio.loop();
  _http.loop();
  _upnp.loop();
  _mqtt.loop();
}
