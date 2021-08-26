#pragma once

#include "Sorcerer2DiskDrive.h"

class Sorcerer2DiskSystem {
  Sorcerer2DiskDrive _drives[4];
  int _activeDriveNumber;
  Sorcerer2DiskDrive* _activeDrive;
  bool active();
  bool dataReady();
  bool home();
  void stepForward();
  void stepBackward();
public:
  Sorcerer2DiskSystem();
  void tick();
  int readByte(int address);
  void writeByte(int address, int value);
};
