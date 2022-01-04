#include "Sorcerer2TapeUnit.h"
#include "Sorcerer2Tape.h"

Sorcerer2TapeUnit::Sorcerer2TapeUnit() : 
  _tape(0),
  _motorOn(false)
{
}

bool Sorcerer2TapeUnit::readyForRead() { 
  return _tape && _motorOn && _tape->isOpen(); 
}

bool Sorcerer2TapeUnit::readyForWrite()  { 
  return _tape && _motorOn && _tape->isOpen() && _tape->writable(); 
}  

void Sorcerer2TapeUnit::writeData(unsigned char data) {
  if (_tape) _tape->write(data);
}

unsigned char Sorcerer2TapeUnit::readData() { 
  return _tape ? _tape ->read() : 0;
}

void Sorcerer2TapeUnit::motor(bool on) {
  _motorOn = on;
  if (_tape) {
    if (on) _tape->open(); else _tape->close();
  }
}

void Sorcerer2TapeUnit::setBaud1200(bool) {
  // TODO
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
