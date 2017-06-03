#pragma once

#include <Arduino.h>

class Storage
{
public:
  virtual String openFile(String path) = 0;
  virtual void storeFile(String path, String content) = 0;
  virtual void deleteAllFiles() = 0;
};

class SPIFFSStorage : public Storage
{
public:
  String openFile(String path);
  void storeFile(String path, String content);
  void deleteAllFiles();

private:
  bool _isInitialized = false;
  void ensureInitialized();
};
