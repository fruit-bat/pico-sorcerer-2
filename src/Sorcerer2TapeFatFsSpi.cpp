#include "Sorcerer2TapeFatFsSpi.h"

Sorcerer2TapeFatFsSpi::Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* name, bool writable) :
  Sorcerer2Tape(name),
  _sdCard(sdCard),
  _open(false),
  _writable(writable)
{
}

uint8_t Sorcerer2TapeFatFsSpi::read(){
  return 0;
}

void Sorcerer2TapeFatFsSpi::write(uint8_t data){
}

bool Sorcerer2TapeFatFsSpi::writable(){
  return _writable;
}

bool Sorcerer2TapeFatFsSpi::open(){
  return false;
}

void Sorcerer2TapeFatFsSpi::close(){
}

bool Sorcerer2TapeFatFsSpi::isOpen(){
  return false;
}

