#pragma once

#include "Sorcerer2RomPac.h"
#include "SdCardFatFsSpi.h"

class Sorcerer2RomPacFatFsSpi : public Sorcerer2RomPac {
private:
  SdCardFatFsSpi* _sdCard;
  std::string _folder;
  FIL _fil;
  bool _open;
public:
  Sorcerer2RomPacFatFsSpi(SdCardFatFsSpi* sdCard, const char* folder, const char* name);
  virtual ~Sorcerer2RomPacFatFsSpi();
  virtual uint8_t read();
  virtual bool open();
  virtual void close();
};
