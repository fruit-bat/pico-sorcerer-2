#include "Sorcerer2TapeSystem.h"
#include <pico/printf.h>

Sorcerer2TapeSystem::Sorcerer2TapeSystem() :
  _units{0, 0},
  _control(0)
{
}

void Sorcerer2TapeSystem::attach(int n, Sorcerer2TapeUnit* unit) {
  if (_units[n]) {
    _units[n]->motor(false);
  }
  _units[n] = unit;
  if (unit) {
    unit->motor((0x10 << n) & _control);
    unit->setBaud1200(0x40 & _control);
  }
}

unsigned char Sorcerer2TapeSystem::readStatus() {
  unsigned int status = 0xfc;
  for (int i = 0; i < 2; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit) {
      //
      // This is the tape control line port
      //
      // Bit Function
      // ==================================
      // 0   Ready for input
      // 1   Ready for output
      //
      // Bits are active high
      //        
      if (unit->readyForRead()) status |= 0x02;
      if (unit->readyForWrite()) status |= 0x01;  
    }
  }
  return status;
}

void Sorcerer2TapeSystem::writeData(unsigned char data) {
  for (int i = 0; i < 2; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit && unit->readyForWrite()) unit->writeData(data);
  }
}

unsigned char Sorcerer2TapeSystem::readData() {
  for (int i = 0; i < 2; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit && unit->readyForRead()) return unit->readData();
  }
  return 0;
}

void Sorcerer2TapeSystem::reset() {
  _control = 0;
  for (int i = 0; i < 2; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit) {
      unit->motor(false);
      unit->setBaud1200(true);
    }
  }
}

void Sorcerer2TapeSystem::writeControl(unsigned char data) {
  for (int i = 0; i < 2; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit) {
      unsigned int motorMask = 0x10 << i;
      unsigned int m = motorMask | 0x40;;
      bool change = (data & m) != (_control & m);
      if (change) {
        unit->motor(motorMask & data);
        unit->setBaud1200(0x40 & data);
      }
    }
  }
  _control = data;
}
