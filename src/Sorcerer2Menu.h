#pragma once

#include "PicoWin.h"
#include "PicoSelect.h"
#include "PicoOptionText.h"
#include "PicoQuickKeyAscii.h"
#include "PicoWiz.h" 
#include "Sorcerer2.h"
#include "SdCardFatFsSpi.h"
#include "PicoTextField.h"

class Sorcerer2;

class Sorcerer2Menu : public PicoWin {
private:
  SdCardFatFsSpi *_sdCard;
  Sorcerer2 *_sorcerer2;
  Sorcerer2DiskDrive *_currentDiskUnit;
  Sorcerer2TapeUnit *_currentTapeUnit;

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
  PicoOption _muteOp;

  PicoSelect _diskUnits;
  PicoOption _diskUnitsOp1;
  PicoOption _diskUnitsOp2;
  PicoOption _diskUnitsOp3;
  PicoOption _diskUnitsOp4;

  PicoSelect _diskUnit;
  PicoOptionText _diskUnitOp1;
  PicoOptionText _diskUnitOp2;
  PicoOptionText _diskUnitOp3;

  PicoSelect _selectDisk;

  PicoTextField _diskName;

  PicoSelect _tapeUnits;
  PicoOption _tapeUnitsOp1;
  PicoOption _tapeUnitsOp2;
  
  PicoSelect _tapeUnit;
  PicoOptionText _tapeUnitOp1;
  PicoOptionText _tapeUnitOp2;
  PicoOptionText _tapeUnitOp3;
  
  PicoSelect _selectTape;
  
  PicoTextField _tapeName;
  
  PicoSelect _rompacUnit;
  PicoOptionText _rompacUnitOp1;
  PicoOptionText _rompacUnitOp2;
  
  PicoSelect _selectRompac;

  PicoWin _message;
  
public:

  Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2);
};
