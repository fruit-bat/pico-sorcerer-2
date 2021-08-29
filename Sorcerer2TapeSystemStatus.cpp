#include "Sorcerer2TapeSystemStatus.h"

Sorcerer2TapeSystemStatus::Sorcerer2TapeSystemStatus() :
  _units{0, 0, 0, 0}
{
}

void Sorcerer2TapeSystemStatus::attach(int n, Sorcerer2TapeUnit* unit) {
  _units[n] = unit;
}

void Sorcerer2TapeSystemStatus::applyStatus(Sorcerer2TapeUnit *unit, unsigned int *status) {
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
  if (unit->readyForRead()) *status |= 0x01;
  if (unit->readyForWrite()) *status |= 0x02;
}

unsigned char Sorcerer2TapeSystemStatus::read(unsigned short address) {
  unsigned int status = 0xfc;
  for (int i = 0; i < 4; ++i) {
    Sorcerer2TapeUnit *unit = _units[i];
    if (unit) applyStatus(_units[i], &status);
  }
  return status;
}
