#pragma once

#include <Arduino.h>

class Storage
{
public:
  String readFile(String path);
  void writeFile(String path, String content);
  void deleteAllFiles();

private:
  bool _isInitialized = false;
  void ensureInitialized();
};
