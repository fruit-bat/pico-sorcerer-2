#pragma once

#include "Sorcerer2TapeUnit.h"

class Sorcerer2TapeSystemStatus {
  
  Sorcerer2TapeUnit* _units[4];
  
  void applyStatus(Sorcerer2TapeUnit *unit, unsigned int *status);
  
public:

  Sorcerer2TapeSystemStatus();
  
  void attach(int n, Sorcerer2TapeUnit* unit);
  
  unsigned char read(unsigned short address);
  
};
