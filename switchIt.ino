#include "Settings.h"
#include "MQTT.h"
#include "HTMLProvider.h"

//#define DEBUG
//#define FULLDEBUG

const String CMD_SWITCH = "switch";
const String CMD_ON = "on";
const String CMD_OFF = "off";
const String CMD_STATUS = "status";

SPIFFSStorage _storage;
Settings _settings(&_storage);
MQTT _mqtt(&_settings);
HTMLProvider _htmlProvider(&_settings);

void setup()
{
  setupGPIO();
  Serial.begin(115200);
  Serial.println("starting initialization ...");
  led(true);
  _settings.loadDeviceSettings();
  setupNetwork();
  prepareHttpServer();
  extendWebServer();
  startHttpServer();
  connectUDP();
  _mqtt.setup(executeCommand);
  led(false);
  applyRelayState(_settings.getStoredState());
  Serial.println("initialization finished.");
}

void loop()
{
  handleButton();
  handleHttpRequest();
  handleUPNP();
  _mqtt.loop();
}

bool executeCommand(String commandName, String* reply)
{
  if (commandName == CMD_SWITCH)
  {
    switchRelay();
    return true;
  }
  if (commandName == CMD_ON)
  {
    relay(true);
    return true;
  }
  if (commandName == CMD_OFF)
  {
    relay(false);
    return true;
  }
  if (commandName == CMD_STATUS)
  {

    String json = _settings.getStatus(getRelayState());
#ifdef FULLDEBUG
    Serial.println("returning status:\n" + json);
#endif
    *reply = json;
    return true;
  }
  return false;
}

void onSettingsChanged()
{
  ESP.restart();
  //  setupNetwork();
}

