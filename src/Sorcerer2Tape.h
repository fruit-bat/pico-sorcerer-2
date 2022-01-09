#pragma once
#include "tusb.h"
#include <string>

class Sorcerer2Tape {
  std::string _name;
public:
  Sorcerer2Tape(const char* name);
  virtual ~Sorcerer2Tape();
  
  virtual uint8_t read() { return 0; }
  virtual void write(uint8_t data) {}
  virtual bool writable() { return false; }
  virtual bool open() { return false; }
  virtual bool record() { return false; }
  virtual void close() {}
  virtual bool isOpen()  { return false; }
  virtual bool isRecording()  { return false; }
  const char *name();
};
