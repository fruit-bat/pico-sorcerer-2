#include "Sorcerer2Menu.h"
#include "PicoPen.h"
#include "Sorcerer2TapeUnitFatFsSpi.h"
#include "Sorcerer2DiskFatFsSpi.h"


Sorcerer2Menu::Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2) :
 PicoWin(0, 0, 80, 30),
  _sdCard(sdCard),
  _sorcerer2(sorcerer2),
  _k0('0'), _k1('1'), _k2('2'), _k3('3'), _k4('4'), _k5('5'), 
  _wiz(5, 6, 54, 18),
  _main(0, 0, 54, 5, 3),
  _mainOp1("Disk drives"),
  _mainOp2("Tape players"),
  _mainOp3("ROM Pac"),
  _mainOp4("CPU Speed"),
  _backOp0("Back"),
  _diskUnits(0, 0, 54, 6, 3),
  _diskUnitsOp1("Drive A"),
  _diskUnitsOp2("Drive B"),
  _diskUnitsOp3("Drive C"),
  _diskUnitsOp4("Drive D"),
  _diskUnit(0, 0, 54, 6, 3),
  _diskUnitOp1("Insert"),
  _diskUnitOp2("Eject"),
  _selectDisk(sdCard, "/", 0, 0, 54, 12),
  _tapeUnits(0, 0, 54, 5, 3),
  _tapeUnitsOp1("Tape Player 1"),
  _tapeUnitsOp2("Tape Player 2"),
  _cpuSpeeds(0, 0, 54, 5, 3),
  _cpuSpeedOp1("4 Mhz"),
  _cpuSpeedOp2("Unmoderated")
{
  addChild(&_wiz, true);
  _wiz.push(&_main, "Main menu", true);
  
  _main.addOption(_mainOp1.addQuickKey(&_k1));
  _main.addOption(_mainOp2.addQuickKey(&_k2));
  _main.addOption(_mainOp3.addQuickKey(&_k3));
  _main.addOption(_mainOp4.addQuickKey(&_k4));
  _main.enableQuickKeys();
  _mainOp1.toggle([=]() {
    _wiz.push(&_diskUnits, "Disk drives", true);
  });
  _mainOp2.toggle([=]() {
    _wiz.push(&_tapeUnits, "Tape players", true);
  });
  _mainOp4.toggle([=]() {
    _wiz.push(&_cpuSpeeds, "CPU Speed", true);
  });
  _backOp0.addQuickKey(&_k0);
  _backOp0.toggle([=]() {
    _wiz.pop(true);
  });
  
  _diskUnits.addOption(_diskUnitsOp1.addQuickKey(&_k1));
  _diskUnits.addOption(_diskUnitsOp2.addQuickKey(&_k2));
  _diskUnits.addOption(_diskUnitsOp3.addQuickKey(&_k3));
  _diskUnits.addOption(_diskUnitsOp4.addQuickKey(&_k4));
  _diskUnits.addOption(&_backOp0);
  _diskUnits.enableQuickKeys();
  _diskUnitsOp1.toggle([=]() {
    _wiz.push(&_diskUnit, "Drive A:", true);
    _currentDiskUnit = _sorcerer2->diskSystem()->drive(0);
  });
  _diskUnitsOp2.toggle([=]() {
    _wiz.push(&_diskUnit, "Drive B:", true);
    _currentDiskUnit = _sorcerer2->diskSystem()->drive(1);
  });
  _diskUnitsOp3.toggle([=]() {
    _wiz.push(&_diskUnit, "Drive C:", true);
    _currentDiskUnit = _sorcerer2->diskSystem()->drive(2);
  });
  _diskUnitsOp4.toggle([=]() {
    _wiz.push(&_diskUnit, "Drive D:", true);
    _currentDiskUnit = _sorcerer2->diskSystem()->drive(3);
  });
  
  _diskUnit.addOption(_diskUnitOp1.addQuickKey(&_k1));
  _diskUnit.addOption(_diskUnitOp2.addQuickKey(&_k2));
  _diskUnit.addOption(&_backOp0);
  _diskUnit.enableQuickKeys();
  _diskUnitOp1.toggle([=]() {
    _wiz.push(&_selectDisk, "Choose disk image", true);
    _selectDisk.reload();
   });
  _diskUnitOp2.toggle([=]() {
     // TODO Don't eject disk if drive is active
     Sorcerer2Disk *disk = _currentDiskUnit->eject();
     if (disk) delete disk;
   });
   _selectDisk.onToggle([=](PicoOption *option) {
      PicoOptionText *textOption = (PicoOptionText *)option;
      Sorcerer2Disk *disk = _currentDiskUnit->insert(new Sorcerer2DiskFatFsSpi(_sdCard, textOption->text()));
      if (disk) delete disk;
      _wiz.pop(true);
   });
   
  _tapeUnits.addOption(_tapeUnitsOp1.addQuickKey(&_k1));
  _tapeUnits.addOption(_tapeUnitsOp2.addQuickKey(&_k2));
  _tapeUnits.addOption(&_backOp0);
  _tapeUnits.enableQuickKeys();
  
  _cpuSpeeds.addOption(_cpuSpeedOp1.addQuickKey(&_k1));
  _cpuSpeeds.addOption(_cpuSpeedOp2.addQuickKey(&_k2));
  _cpuSpeeds.addOption(&_backOp0);
  _cpuSpeeds.enableQuickKeys();
  _cpuSpeedOp1.selected([=](){ return _sorcerer2->moderate(); });
  _cpuSpeedOp2.selected([=](){ return !_sorcerer2->moderate(); });
  _cpuSpeedOp1.toggle([=](){ _sorcerer2->moderate(true); });
  _cpuSpeedOp2.toggle([=](){ _sorcerer2->moderate(false); });
  
  onPaint([](PicoPen *pen) {
     pen->printAt(0, 0, false, "Exidy Sorcerer 2 emulator");
     pen->printAt(0, 1, false, "on RP2040 Pico Pi");
     pen->printAt(0, 2, false, "Menu System version 0.1");

     pen->printAt(0, 29, false, "F1 to exit menu");
   });
}

