#include "Sorcerer2Tape.h"

Sorcerer2Tape::Sorcerer2Tape(const char* name) :
  _name(name)
{
}

Sorcerer2Tape::~Sorcerer2Tape(){
}

const char *Sorcerer2Tape::name(){
  return _name.c_str();
}
