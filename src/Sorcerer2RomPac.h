#pragma once
#include "tusb.h"
#include <string>

class Sorcerer2RomPac {
  std::string _name;
public:
  Sorcerer2RomPac(const char* name);
  virtual ~Sorcerer2RomPac();
  
  virtual uint8_t read() { return 0; }
  virtual bool open() { return false; }
  virtual void close() {}
  const char *name();
};
