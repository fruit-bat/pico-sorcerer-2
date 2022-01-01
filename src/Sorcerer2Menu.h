#pragma once

#include "PicoWin.h"
#include "PicoSelect.h"
#include "PicoOptionText.h"
#include "PicoSelectFile.h"
#include "PicoQuickKeyAscii.h"
#include "PicoWiz.h" 
#include "Sorcerer2.h"
#include "SdCardFatFsSpi.h"

class Sorcerer2;

class Sorcerer2Menu : public PicoWin {
private:
  SdCardFatFsSpi *_sdCard;
  Sorcerer2 *_sorcerer2;
  Sorcerer2DiskDrive *_currentDiskUnit;

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
  PicoOption _mainOp4;

  PicoSelect _diskUnits;
  PicoOption _diskUnitsOp1;
  PicoOption _diskUnitsOp2;
  PicoOption _diskUnitsOp3;
  PicoOption _diskUnitsOp4;

  PicoSelect _diskUnit;
  PicoOptionText _diskUnitOp1;
  PicoOptionText _diskUnitOp2;
  
  PicoSelectFile _selectDisk;
  
  PicoSelect _tapeUnits;
  PicoOptionText _tapeUnitsOp1;
  PicoOptionText _tapeUnitsOp2;

public:

  Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2);
};
