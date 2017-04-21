#include "FS.h"

void setupStorage()
{
  SPIFFS.begin();
}

String openFile(String path)
{
  File file = SPIFFS.open(path, "r");
  if (!file)
  {
    return "";
  }

  String line = file.readString();
  file.close();
  return line;
}

void storeFile(String path, String content)
{
  File file = SPIFFS.open(path, "w");
  if (!file)
    Serial.println("Failed to open '" + path + "' for writing!");
  else
  {
    file.print(content);
    file.close();
  }
}

void deleteAllFiles()
{
  Serial.print("Formatting filesystem ... ");

  if (!SPIFFS.format())
    Serial.println("Failed to format filesystem!");
  else
    Serial.println("done");
}

