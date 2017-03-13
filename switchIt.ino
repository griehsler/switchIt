#define EMULATE_RELAY
#define DEBUG
//#define FULLDEBUG

const String CMD_SWITCH = "switch";
const String CMD_ON = "on";
const String CMD_OFF = "off";

String otherAPSSID;
String otherAPPassword;

void setup()
{
  setupGPIO();
  Serial.begin(115200);
  Serial.println("starting initialization ...");
  led(true);
  setupStorage();
  setupNetwork();
  prepareHttpServer();
  extendWebServer();
  startHttpServer();
  connectUDP();
  Serial.println("initialization finished.");
  led(false);
}

void loop()
{
  handleButton();
  handleHttpRequest();
  handleUPNP();
}

bool executeCommand(String commandName)
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
  return false;
}

void onSettingsChanged()
{
  ESP.restart();
//  setupNetwork();
}

