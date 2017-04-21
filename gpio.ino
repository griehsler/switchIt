const int relayPin = D1;
const int buttonPin = D3;

bool ledOn;
bool relayOn;
bool buttonPressed;

void setupGPIO() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  led(false);
}

void led(bool on)
{
  if (on)
    digitalWrite(BUILTIN_LED, LOW);
  else
    digitalWrite(BUILTIN_LED, HIGH);

  ledOn = on;
}

void relay(bool on)
{
  if (on)
    digitalWrite(relayPin, HIGH);
  else
    digitalWrite(relayPin, LOW);

  relayOn = on;

#ifdef EMULATE_RELAY
  led(on);
#endif

  storeState();
}

void switchRelay()
{
  relay(!relayOn);
}

void switchLed()
{
  led(!ledOn);
}

String getRelayState()
{
  if (relayOn)
    return "ON";
  else
    return "OFF";
}

void applyRelayState(String state)
{
  relay(state == "ON");
}

void handleButton()
{
#ifdef BUTTON_MODE
  bool currentlyPressed = digitalRead(buttonPin) == LOW;

  if (BUTTON_MODE == "push" && !buttonPressed && currentlyPressed ||
      BUTTON_MODE == "touch" && buttonPressed != currentlyPressed)
  {
    Serial.println("Button pressed, switching");
    String reply;
    executeCommand(CMD_SWITCH, &reply);
  }

  buttonPressed = currentlyPressed;
#endif
}

