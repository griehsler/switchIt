#pragma once

#include <Arduino.h>

class Storage
{
public:
  virtual String readFile(String path) = 0;
  virtual void writeFile(String path, String content) = 0;
  virtual void deleteAllFiles() = 0;
};

class SPIFFSStorage : public Storage
{
public:
  String readFile(String path);
  void writeFile(String path, String content);
  void deleteAllFiles();

private:
  bool _isInitialized = false;
  void ensureInitialized();
};
