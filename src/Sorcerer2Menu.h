#pragma once

#include "PicoWin.h"
#include "PicoSelect.h"
#include "PicoOptionText.h"
#include "PicoQuickKeyAscii.h"
#include "PicoWiz.h" 
#include "Sorcerer2.h"
#include "SdCardFatFsSpi.h"
#include "PicoTextField.h"
#include <functional>

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
  PicoQuickKeyAscii _k6;

  PicoWiz _wiz;
  PicoSelect _main;
  PicoOptionText _mainOp1;
  PicoOptionText _mainOp2;
  PicoOptionText _mainOp3;
  PicoOption _mainOp4;
  PicoOption _muteOp;
  PicoOptionText _resetOp;

  PicoSelect _diskUnits;
  PicoOption _diskUnitsOp1;
  PicoOption _diskUnitsOp2;
  PicoOption _diskUnitsOp3;
  PicoOption _diskUnitsOp4;
  PicoOptionText _diskUnitsOp5;
  PicoOptionText _diskUnitsOp6;

  PicoSelect _diskUnit;
  PicoOptionText _diskUnitOp1;
  PicoOptionText _diskUnitOp2;
  PicoOptionText _diskUnitOp3;

  PicoSelect _selectDisk;

  PicoTextField _diskName;

  PicoSelect _tapeUnits;
  PicoOption _tapeUnitsOp1;
  PicoOption _tapeUnitsOp2;
  PicoOptionText _tapeUnitsOp3;
  PicoOptionText _tapeUnitsOp4;

  PicoSelect _tapeUnit;
  PicoOptionText _tapeUnitOp1;
  PicoOptionText _tapeUnitOp2;
  PicoOptionText _tapeUnitOp3;
  PicoOption _tapeUnitRec;

  PicoSelect _selectTape;
  
  PicoTextField _tapeName;
  
  PicoSelect _rompacUnit;
  PicoOptionText _rompacUnitOp1;
  PicoOptionText _rompacUnitOp2;
  
  PicoSelect _selectRompac;

  PicoWin _message;

  PicoSelect _confirm;
  PicoOptionText _confirmNo;
  PicoOptionText _confirmYes;

  PicoSelect _selectDelete;
 
  void showError(std::function<void(PicoPen *pen)> message);
  bool deleteSave(const char *folder, const char *file);
  bool createDisk(const char* dnr);
  bool createTape(const char* tnr);
  
  void loadSavedDisks(PicoSelect* select);
  void loadSavedTapes(PicoSelect* select);

  void confirm(
    std::function<void(PicoPen *pen)> message,
    std::function<void()> no,
    std::function<void()> yes
  );
  
  void confirm(
    std::function<void(PicoPen *pen)> message,
    std::function<void()> yes
  );
  
public:

  Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2);
  

};
