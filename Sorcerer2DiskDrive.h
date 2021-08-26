#pragma once

class Sorcerer2DiskDrive {

public:
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
};
