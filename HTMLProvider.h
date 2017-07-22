#pragma once

#include <Arduino.h>
#include "Settings.h"

class HTMLProvider
{
public:
  HTMLProvider(Settings *settings);
  String getIndexHtml();
  String getConfigHtml(String message, String dummyPassword);
  String getEventServiceXml();
  String getSetupXml(String name, String uuid, String serialNumber);
  String getSsdpSearchResponse(String serverUrl, String uuid, String st);

private:
  Settings *_settings;
  String toChecked(bool value);
  String commonHead();
};