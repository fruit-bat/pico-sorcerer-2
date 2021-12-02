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
  void readyWrite();
  void activate(const int n);
  void writeReg0(const int b);
  void writeReg1(const int b);
  void writeReg2(const int b);
  int readReg0();
  int readReg1();
  int readReg2();
public:
  Sorcerer2DiskSystem();
  void tick();
  int readByte(int address);
  void writeByte(int address, int value);
  Sorcerer2DiskDrive* drive(int unit) { return &_drives[unit]; }
  void reset();
};
