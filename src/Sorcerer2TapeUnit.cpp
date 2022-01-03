#include "Sorcerer2TapeUnit.h"
#include "Sorcerer2Tape.h"

Sorcerer2TapeUnit::Sorcerer2TapeUnit() : 
  _tape(0)
{
}

Sorcerer2Tape *Sorcerer2TapeUnit::insert(Sorcerer2Tape *tape) {
  Sorcerer2Tape *r = _tape;
  if (_tape) {
    _tape->close();
  }
  _tape = tape;
  return r;  
}

  
Sorcerer2Tape *Sorcerer2TapeUnit::eject() {
  Sorcerer2Tape *r = _tape;
  if (_tape) {
    _tape->close();
    _tape = 0;
  }
  return r; 
}
