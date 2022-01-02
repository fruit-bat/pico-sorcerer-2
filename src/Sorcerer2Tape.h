#pragma once
#include "tusb.h"

class Sorcerer2Tape {
public:
  virtual ~Sorcerer2Tape() {}
  virtual uint8_t read();
  virtual void write(uint8_t data);
  virtual bool writable();
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
  virtual const char *name();
};
