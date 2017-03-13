const int relayPin = D1;
const int buttonPin = D8;
bool ledOn;
bool relayOn;

void setupGPIO() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  led(false);
  relay(false);
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
}

void switchRelay()
{
  relay(!relayOn);
}

void switchLed()
{
  led(!ledOn);
}

bool buttonPressed = false;

void handleButton()
{
  /*
  bool currentlyPressed = digitalRead(buttonPin) == LOW;
  led(currentlyPressed);
  delay(500);
  
    Serial.print("Button is being pressed: ");
    if (currentlyPressed)
      Serial.println("true");
    else
      Serial.println("false");

    if (!buttonPressed && currentlyPressed)
      switchLed();
  */
//  buttonPressed = currentlyPressed;
}

