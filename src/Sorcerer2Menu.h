#pragma once

#include "PicoWin.h"
#include "PicoSelect.h"
#include "PicoOptionText.h"
#include "PicoSelectFile.h"
#include "PicoQuickKeyAscii.h"
#include "PicoWiz.h" 
#include "Sorcerer2.h"

class Sorcerer2;

class Sorcerer2Menu : public PicoWin {
private:
  Sorcerer2 *_sorcerer2;

  PicoQuickKeyAscii _k0;
  PicoQuickKeyAscii _k1;
  PicoQuickKeyAscii _k2;
  PicoQuickKeyAscii _k3;
  PicoQuickKeyAscii _k4;
  PicoQuickKeyAscii _k5;
  
  PicoWiz _wiz;
  PicoSelect _main;
  PicoOptionText _mainOp1;
  PicoOptionText _mainOp2;
  PicoOptionText _mainOp3;
  PicoOptionText _mainOp4;

  PicoSelect _diskUnits;
  PicoOptionText _diskUnitsOp0;
  PicoOptionText _diskUnitsOp1;
  PicoOptionText _diskUnitsOp2;
  PicoOptionText _diskUnitsOp3;
  PicoOptionText _diskUnitsOp4;

  PicoSelect _tapeUnits;
  PicoOptionText _tapeUnitsOp0;
  PicoOptionText _tapeUnitsOp1;
  PicoOptionText _tapeUnitsOp2;
  
  PicoSelect _cpuSpeeds;
  PicoOptionText _cpuSpeedOp0;
  PicoOptionText _cpuSpeedOp1;
  PicoOptionText _cpuSpeedOp2;

public:

  Sorcerer2Menu(Sorcerer2 *sorcerer2);

  virtual void paint(PicoPen *pen);
};
