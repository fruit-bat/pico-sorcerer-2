#include "Sorcerer2Menu.h"
#include "PicoPen.h"


Sorcerer2Menu::Sorcerer2Menu(Sorcerer2 *sorcerer2) :
 PicoWin(0, 0, 80, 30),
  _sorcerer2(sorcerer2),
  _k0('0'), _k1('1'), _k2('2'), _k3('3'), _k4('4'), _k5('5'), 
  _wiz(5, 5, 54, 22),
  _main(0, 0, 54, 5, 3),
  _mainOp1("Disk drives"),
  _mainOp2("Tape players"),
  _mainOp3("ROM Pac"),
  _mainOp4("CPU Speed"),
  _diskUnits(0, 0, 54, 6, 2),
  _diskUnitsOp0("Back"),
  _diskUnitsOp1("Drive A"),
  _diskUnitsOp2("Drive B"),
  _diskUnitsOp3("Drive C"),
  _diskUnitsOp4("Drive D"),
  _tapeUnits(0, 0, 54, 5, 3),
  _tapeUnitsOp0("Back"),
  _tapeUnitsOp1("Tape Player 1"),
  _tapeUnitsOp2("Tape Player 2"),
  _cpuSpeeds(0, 0, 54, 5, 3),
  _cpuSpeedOp0("Back"),
  _cpuSpeedOp1("4 Mhz"),
  _cpuSpeedOp2("Unmoderated")
{
  addChild(&_wiz, true);
  _wiz.push(&_main, true);
  
  _main.addOption(_mainOp1.addQuickKey(&_k1));
  _main.addOption(_mainOp2.addQuickKey(&_k2));
  _main.addOption(_mainOp3.addQuickKey(&_k3));
  _main.addOption(_mainOp4.addQuickKey(&_k4));
  _main.enableQuickKeys();
  _mainOp1.toggle([=]() {
    _wiz.push(&_diskUnits, true);
  });
  _mainOp2.toggle([=]() {
    _wiz.push(&_tapeUnits, true);
  });
  _mainOp4.toggle([=]() {
    _wiz.push(&_cpuSpeeds, true);
  });

  _diskUnits.addOption(_diskUnitsOp1.addQuickKey(&_k1));
  _diskUnits.addOption(_diskUnitsOp2.addQuickKey(&_k2));
  _diskUnits.addOption(_diskUnitsOp3.addQuickKey(&_k3));
  _diskUnits.addOption(_diskUnitsOp4.addQuickKey(&_k4));
  _diskUnits.addOption(_diskUnitsOp0.addQuickKey(&_k0));
  _diskUnits.enableQuickKeys();
  _diskUnitsOp0.toggle([=]() {
    _wiz.pop(true);
  });
  
  _tapeUnits.addOption(_tapeUnitsOp1.addQuickKey(&_k1));
  _tapeUnits.addOption(_tapeUnitsOp2.addQuickKey(&_k2));
  _tapeUnits.addOption(_tapeUnitsOp0.addQuickKey(&_k0));
  _tapeUnits.enableQuickKeys();
  _tapeUnitsOp0.toggle([=]() {
    _wiz.pop(true);
  });
  
  _cpuSpeeds.addOption(_cpuSpeedOp1.addQuickKey(&_k1));
  _cpuSpeeds.addOption(_cpuSpeedOp2.addQuickKey(&_k2));
  _cpuSpeeds.addOption(_cpuSpeedOp0.addQuickKey(&_k0));
  _cpuSpeeds.enableQuickKeys();
  _cpuSpeedOp0.toggle([=](){ _wiz.pop(true); });
  _cpuSpeedOp1.selected([=](){ return _sorcerer2->moderate(); });
  _cpuSpeedOp2.selected([=](){ return !_sorcerer2->moderate(); });
  _cpuSpeedOp1.toggle([=](){ _sorcerer2->moderate(true); });
  _cpuSpeedOp2.toggle([=](){ _sorcerer2->moderate(false); });
}

void Sorcerer2Menu::paint(PicoPen *pen) {
  pen->printAt(0, 0, false, "Exidy Sorcerer 2 emulator");
  pen->printAt(0, 1, false, "on RP2040 Pico Pi");
  pen->printAt(0, 2, false, "Menu System version 0.1");

  pen->printAt(0, 29, false, "F1 to exit menu");
}

