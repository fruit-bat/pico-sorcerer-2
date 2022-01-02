#include "Sorcerer2Menu.h"
#include "PicoPen.h"
#include "Sorcerer2TapeUnitFatFsSpi.h"
#include "Sorcerer2DiskFatFsSpi.h"
#include "Sorcerer2TapeFatFsSpi.h"
#include "FatFsSpiDirReader.h"


Sorcerer2Menu::Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2) :
 PicoWin(0, 0, 80, 30),
  _sdCard(sdCard),
  _sorcerer2(sorcerer2),
  _k1('1'), _k2('2'), _k3('3'), _k4('4'), _k5('5'), 
  _wiz(5, 6, 70, 18),
  _main(0, 0, 70, 5, 3),
  _mainOp1("Disk drives"),
  _mainOp2("Tape players"),
  _mainOp3("ROM Pac"),
  _mainOp4(),
  _diskUnits(0, 0, 70, 6, 3),
  _diskUnit(0, 0, 70, 6, 3),
  _diskUnitOp1("Insert"),
  _diskUnitOp2("Eject"),
  _selectDisk(0, 0, 70, 12, 1),
  _tapeUnits(0, 0, 70, 5, 3),
  _tapeUnit(0, 0, 70, 6, 3),
  _tapeUnitOp1("Insert"),
  _tapeUnitOp2("Eject"),
  _selectTape(0, 0, 70, 12, 1)
{
  addChild(&_wiz, true);
  _wiz.push(&_main, [](PicoPen *pen){ pen->printAt(0, 0, false, "Main menu"); }, true);
  _wiz.onKeydown([=](uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
     return (ascii != 27) || !_wiz.pop(true);
   });
   
  _main.addOption(_mainOp1.addQuickKey(&_k1));
  _main.addOption(_mainOp2.addQuickKey(&_k2));
  _main.addOption(_mainOp3.addQuickKey(&_k3));
  _main.addOption(_mainOp4.addQuickKey(&_k4));
  _main.addOption(_muteOp.addQuickKey(&_k5));
  _main.enableQuickKeys();
  _mainOp1.toggle([=]() {
    _wiz.push(
      &_diskUnits, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Disk drives"); }, 
      true);
  });
  _mainOp2.toggle([=]() {
    _wiz.push(
      &_tapeUnits,
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Tape players"); }, 
      true);
  });
  _mainOp4.toggle([=]() {
    _sorcerer2->toggleModerate();
    _main.repaint();
  });
  _mainOp4.onPaint([=](PicoPen *pen){
    pen->clear();
    pen->printAtF(0, 0, false,"CPU Speed       [ %-12s]", _sorcerer2->moderate() ? "4 Mhz" : "Unmoderated");
  });
  _muteOp.toggle([=]() {
    _sorcerer2->toggleMute();
    _main.repaint();
  });
  _muteOp.onPaint([=](PicoPen *pen){
    pen->clear();
    pen->printAtF(0, 0, false,"Audio           [ %-12s]", _sorcerer2->mute() ? "off" : "on");
  });
  
  _diskUnits.addOption(_diskUnitsOp1.addQuickKey(&_k1));
  _diskUnits.addOption(_diskUnitsOp2.addQuickKey(&_k2));
  _diskUnits.addOption(_diskUnitsOp3.addQuickKey(&_k3));
  _diskUnits.addOption(_diskUnitsOp4.addQuickKey(&_k4));
  _diskUnits.enableQuickKeys();
  
  PicoOption *du[] = {&_diskUnitsOp1, &_diskUnitsOp2, &_diskUnitsOp3, &_diskUnitsOp4};
  for (int i = 0; i < 4; ++i) {
    PicoOption *d = du[i];
    d->onPaint([=](PicoPen *pen){
      Sorcerer2Disk *disk = _sorcerer2->diskSystem()->drive(i)->disk();
      pen->clear();
      pen->printAtF(0, 0, false,"Drive %c   [ %-40s]", "ABCD"[i], disk ? disk->name(): "");
    });
    d->toggle([=]() {
      _wiz.push(
        &_diskUnit, 
        [=](PicoPen *pen){
          Sorcerer2Disk *disk = _sorcerer2->diskSystem()->drive(i)->disk();
          pen->printAtF(0, 0, false,"Drive %c   [ %-40s]", "ABCD"[i], disk ? disk->name(): "");
        },
        true
      );
      _currentDiskUnit = _sorcerer2->diskSystem()->drive(i);
    });
  }

  _diskUnit.addOption(_diskUnitOp1.addQuickKey(&_k1));
  _diskUnit.addOption(_diskUnitOp2.addQuickKey(&_k2));
  _diskUnit.enableQuickKeys();
  _diskUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectDisk, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose disk image"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, "/");
    _selectDisk.deleteOptions();
    dirReader.foreach([=](const FILINFO* info){ 
      for(int i = 0; i < 4; ++i) {
        Sorcerer2Disk *disk = _sorcerer2->diskSystem()->drive(i)->disk();
        if (disk && (strcmp(info->fname, disk->name()) == 0)) return;
      }
      _selectDisk.addOption(new PicoOptionText(info->fname));
    });
   });
  _diskUnitOp2.toggle([=]() {
    // TODO Don't eject disk if drive is active
    Sorcerer2Disk *disk = _currentDiskUnit->eject();
    if (disk) delete disk;
    _wiz.pop(true);
  });
  _selectDisk.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2Disk *disk = _currentDiskUnit->insert(new Sorcerer2DiskFatFsSpi(_sdCard, textOption->text()));
    if (disk) delete disk;
    _wiz.pop(true);
    _wiz.pop(true);
  });

  _tapeUnits.addOption(_tapeUnitsOp1.addQuickKey(&_k1));
  _tapeUnits.addOption(_tapeUnitsOp2.addQuickKey(&_k2));
  _tapeUnits.enableQuickKeys();
  
  PicoOption *tu[] = {&_tapeUnitsOp1, &_tapeUnitsOp2};
  for (int i = 0; i < 2; ++i) {
    PicoOption *t = tu[i];
    t->onPaint([=](PicoPen *pen){
      Sorcerer2Tape *tape = _sorcerer2->tapeSystem()->unit(i)->tape();
      pen->clear();
      pen->printAtF(0, 0, false,"Tape player %d   [ %-40s]", i + 1, tape ? tape->name(): "");
    });
    t->toggle([=]() {
      _wiz.push(
        &_tapeUnit, 
        [=](PicoPen *pen){
          Sorcerer2Tape *tape = _sorcerer2->tapeSystem()->unit(i)->tape();
          pen->printAtF(0, 0, false,"Tape player %d   [ %-40s]", i + 1, tape ? tape->name(): "");
        },
        true
      );
      _currentTapeUnit = _sorcerer2->tapeSystem()->unit(i);
    });
  }
  
  _tapeUnit.addOption(_tapeUnitOp1.addQuickKey(&_k1));
  _tapeUnit.addOption(_tapeUnitOp2.addQuickKey(&_k2));
  _tapeUnit.enableQuickKeys();
  _tapeUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectTape, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose tape"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, "/tapes");
    _selectTape.deleteOptions();
    dirReader.foreach([=](const FILINFO* info){ 
      for(int i = 0; i < 2; ++i) {
        Sorcerer2Tape *tape = _sorcerer2->tapeSystem()->unit(i)->tape();
        if (tape && (strcmp(info->fname, tape->name()) == 0)) return;
      }
      _selectTape.addOption(new PicoOptionText(info->fname));
    });
   });
  _tapeUnitOp2.toggle([=]() {
    // TODO Don't eject tape if drive is active
    Sorcerer2Tape *tape = _currentTapeUnit->eject();
    if (tape) delete tape;
    _wiz.pop(true);
  });
  _selectTape.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    //Sorcerer2Tape *tape = _currentTapeUnit->insert(new Sorcerer2TapeFatFsSpi(_sdCard, textOption->text()));
    //if (disk) delete disk;
    _wiz.pop(true);
    _wiz.pop(true);
  });
  
  
  onPaint([](PicoPen *pen) {
     pen->printAt(0, 0, false, "Exidy Sorcerer 2 emulator");
     pen->printAt(0, 1, false, "on RP2040 Pico Pi");
     pen->printAt(0, 2, false, "Menu System version 0.1");

     pen->printAt(0, 29, false, "F1 to exit menu");
     pen->printAt(80-14, 29, false, "ESC to go back");
   });
}

