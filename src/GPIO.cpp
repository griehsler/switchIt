#include "GPIO.h"

GPIO::GPIO(Settings *settings, Logger *logger, MQTT *mqtt)
{
  _settings = settings;
  _logger = logger;
  _mqtt = mqtt;
}

void GPIO::setup()
{
  longPressStart = millis();

  if (_settings->customButtonGpio > 0)
    buttonPin = _settings->customButtonGpio;
  if (_settings->customRelayGpio > 0)
    relayPin = _settings->customRelayGpio;

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  led(false);
}

void GPIO::led(bool on)
{
  if (on)
    digitalWrite(LED_BUILTIN, LOW);
  else
    digitalWrite(LED_BUILTIN, HIGH);
}

void GPIO::relay(bool on)
{
  if (_settings->emulateRelay)
    led(on);
  else
  {
    if (on ^ _settings->invertSwitch)
      digitalWrite(relayPin, HIGH);
    else
      digitalWrite(relayPin, LOW);
  }

  relayOn = on;

  String newState = getRelayState();
  _logger->writeLog(LOG_INFO, "switched state to: " + newState);
  if (_mqtt)
    _mqtt->reportStatus(newState);
  _settings->storeState(newState);
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

void GPIO::restoreLastState()
{
  String storedState = _settings->getStoredState();
  relay(storedState == "ON");
}

void GPIO::loop()
{
  bool isSwitch = _settings->buttonMode == _settings->BUTTON_SWITCH;
  bool isTouch = _settings->buttonMode == _settings->BUTTON_TOUCH;

  if (isSwitch || isTouch)
  {
    bool currentlyPressed = digitalRead(buttonPin) == LOW;

    if (currentlyPressed != lastPressed)
    {
      lastPressed = currentlyPressed;
      longPressStart = millis();
#ifdef DEBUG
      Serial.println("press state changed");
#endif
    }
#ifdef FULLDEBUG
    Serial.print("time sincelast state change: ");
    Serial.println(millis() - longPressStart);
#endif
    if (millis() - longPressStart > debounceDelay)
    {
      longPressed = lastPressed;
    }

    if ((isSwitch && !buttonPressed && longPressed) ||
        (isTouch && buttonPressed != longPressed))
    {
      Serial.println("Button pressed, switching");
      _logger->writeLog(LOG_INFO, "Button pressed, switching");
      switchRelay();
    }

    buttonPressed = longPressed;
  }
}
