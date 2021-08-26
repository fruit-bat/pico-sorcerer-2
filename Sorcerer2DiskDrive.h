#pragma once

class Sorcerer2DiskDrive {

public:
  bool active();
  void tick();
  bool dataReady();
  bool home();
  void stepForward();
  void stepBackward();
};
