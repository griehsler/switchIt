#include "Commands.h"

Commands::Commands(Settings *settings, GPIO *gpio)
{
    _settings = settings;
    _gpio = gpio;
}

void Commands::switchStatus()
{
    _gpio->switchRelay();
}

void Commands::on()
{
    _gpio->relay(true);
}

void Commands::off()
{
    _gpio->relay(false);
}

String Commands::getStatus()
{
    String json = _settings->getStatus(_gpio->getRelayState());
#ifdef FULLDEBUG
    Serial.println("returning status:\n" + json);
#endif
    return json;
}

bool Commands::execute(String name, String *reply)
{
    if (name == CMD_SWITCH)
    {
        switchStatus();
        return true;
    }
    if (name == CMD_ON)
    {
        on();
        return true;
    }
    if (name == CMD_OFF)
    {
        off();
        return true;
    }
    if (name == CMD_STATUS)
    {
        String status = getStatus();
        *reply = status;
        return true;
    }

    return false;
}