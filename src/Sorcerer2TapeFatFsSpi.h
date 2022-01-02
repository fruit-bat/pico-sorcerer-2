#pragma once

#include "Sorcerer2Tape.h"
#include "SdCardFatFsSpi.h"
#include <string>

class Sorcerer2TapeFatFsSpi : public Sorcerer2Tape {
private:
  SdCardFatFsSpi* _sdCard;
  std::string _name;
  FIL _fil;
  bool _open;
  bool _writable;
public:
  Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* name, bool writable);
  virtual ~Sorcerer2TapeFatFsSpi() {}
  virtual uint8_t read();
  virtual void write(uint8_t data);
  virtual bool writable();
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
  virtual const char *name();
};