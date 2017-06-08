#include "Storage.h"
#include <FS.h>

void Storage::ensureInitialized()
{
  if (!_isInitialized)
  {
    _isInitialized = true;
    SPIFFS.begin();
  }
}

String Storage::readFile(String path)
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

void Storage::writeFile(String path, String content)
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

void Storage::deleteAllFiles()
{
  ensureInitialized();

  Serial.print("Formatting filesystem ... ");

  if (!SPIFFS.format())
    Serial.println("Failed to format filesystem!");
  else
    Serial.println("done");
}
