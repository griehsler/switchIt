#include "Logger.h"

Logger::Logger(Settings *settings)
{
    _settings = settings;
}

void Logger::setup()
{
    if (_settings->syslogEnabled)
        _UDP.begin(2390);
}

void Logger::writeLog(String severity, String message)
{
    if (!_settings->syslogEnabled)
        return;

    _UDP.beginPacket(_settings->syslogServer.c_str(), _settings->syslogServerPort);
    _UDP.write("<");
    _UDP.write(severity.c_str());
    _UDP.write(">");
    _UDP.write(_settings->hostName.c_str());
    _UDP.write(" lightswitch[]: ");
    _UDP.write(message.c_str());
    _UDP.endPacket();

#ifdef DEBUG
    Serial.print("Sent syslog message: ");
    Serial.println(message);
#endif
}