#pragma once

#include "Sorcerer2TapeUnit.h"

class Sorcerer2TapeSystem {
  
  Sorcerer2TapeUnit* _units[2];
  
  unsigned char _control;
    
public:

  Sorcerer2TapeSystem();
  
  void attach(int n, Sorcerer2TapeUnit* unit);
  
  unsigned char readStatus();
  void writeControl(unsigned char data);
  
  void writeData(unsigned char data);
  unsigned char readData();
  
  void reset();

  Sorcerer2TapeUnit* unit(int unit) { return _units[unit]; }
};
