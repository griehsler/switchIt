//#define EMULATE_RELAY
//#define DEBUG
//#define FULLDEBUG
//#define BUTTON_MODE "push"
#define BUTTON_MODE "touch"

const String CMD_SWITCH = "switch";
const String CMD_ON = "on";
const String CMD_OFF = "off";
const String CMD_STATUS = "status";

String otherAPSSID;
String otherAPPassword;
String hostName;
String deviceName;
String persistentUuid;

void setup()
{
  setupGPIO();
  Serial.begin(115200);
  Serial.println("starting initialization ...");
  led(true);
  setupStorage();
  loadDeviceSettings();
  setupNetwork();
  prepareHttpServer();
  extendWebServer();
  startHttpServer();
  connectUDP();
  led(false);
  applyState();
  Serial.println("initialization finished.");
}

void loop()
{
  handleButton();
  handleHttpRequest();
  handleUPNP();
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

    String json = getStatus();
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

