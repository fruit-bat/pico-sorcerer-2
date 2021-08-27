#pragma once

#include "Sorcerer2Disk.h"

class Sorcerer2DiskDrive {
  Sorcerer2Disk* _disk;
  int _unit;
  int _activeCount;
  int _sectorNumber;
  int _trackNumber;
  bool _newSector;
  int _sectorIndex;
  void deactivate();
  void seekDisk();
public:
  Sorcerer2DiskDrive(int unit);
  bool active();
  void tick();
  bool dataReady();
  bool home();
  void stepForward();
  void stepBackward();
  void readyWrite();
  void activate();
  int readReg0();
  int readReg2();
  void writeReg2(const int b);
  void insert(Sorcerer2Disk* disk);
  Sorcerer2Disk* eject();
  char driveLetter();
};
