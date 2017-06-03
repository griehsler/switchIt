#pragma once

#include "Storage.h"
#include "FS.h"

void SPIFFSStorage::ensureInitialized()
{
  if (!_isInitialized)
  {
    _isInitialized = true;
    SPIFFS.begin();
  }
}

String SPIFFSStorage::openFile(String path)
{
  ensureInitialized();

  File file = SPIFFS.open(path, "r");
  if (!file)
  {
    return "";
  }

  String line = file.readString();
  file.close();
  return line;
}

void SPIFFSStorage::storeFile(String path, String content)
{
  ensureInitialized();

  File file = SPIFFS.open(path, "w");
  if (!file)
    Serial.println("Failed to open '" + path + "' for writing!");
  else
  {
    file.print(content);
    file.close();
  }
}

void SPIFFSStorage::deleteAllFiles()
{
  ensureInitialized();

  Serial.print("Formatting filesystem ... ");

  if (!SPIFFS.format())
    Serial.println("Failed to format filesystem!");
  else
    Serial.println("done");
}
