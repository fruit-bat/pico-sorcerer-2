#include "Sorcerer2RomPac.h"

Sorcerer2RomPac::Sorcerer2RomPac(const char* name) :
  _name(name)
{
}

Sorcerer2RomPac::~Sorcerer2RomPac(){
}

const char *Sorcerer2RomPac::name(){
  return _name.c_str();
}
