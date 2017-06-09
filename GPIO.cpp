#include "GPIO.h"

GPIO::GPIO(Settings *settings, MQTT *mqtt)
{
  _settings = settings;
  _mqtt = mqtt;
}

void GPIO::setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  led(false);
}

void GPIO::led(bool on)
{
  if (on)
    digitalWrite(BUILTIN_LED, LOW);
  else
    digitalWrite(BUILTIN_LED, HIGH);
}

void GPIO::relay(bool on)
{
  if (_settings->emulateRelay)
    led(on);
  else
  {
    if (on)
      digitalWrite(relayPin, HIGH);
    else
      digitalWrite(relayPin, LOW);
  }

  relayOn = on;

  _mqtt->reportStatus(getRelayState());
  _settings->storeState(getRelayState());
}

void GPIO::switchRelay()
{
  relay(!relayOn);
}

String GPIO::getRelayState()
{
  if (relayOn)
    return "ON";
  else
    return "OFF";
}

void GPIO::applyRelayState(String state)
{
  relay(state == "ON");
}

void GPIO::loop()
{
  bool isSwitch = _settings->buttonMode == _settings->BUTTON_SWITCH;
  bool isTouch = _settings->buttonMode == _settings->BUTTON_TOUCH;

  if (isSwitch || isTouch)
  {
    bool currentlyPressed = digitalRead(buttonPin) == LOW;

    if ((isSwitch && !buttonPressed && currentlyPressed) ||
        (isTouch && buttonPressed != currentlyPressed))
    {
      Serial.println("Button pressed, switching");
      switchRelay();
    }

    buttonPressed = currentlyPressed;
  }
}
