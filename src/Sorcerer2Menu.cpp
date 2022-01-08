#include "Sorcerer2Menu.h"
#include "PicoPen.h"
#include "Sorcerer2TapeUnitFatFsSpi.h"
#include "Sorcerer2DiskFatFsSpi.h"
#include "Sorcerer2TapeFatFsSpi.h"
#include "Sorcerer2RomPacFatFsSpi.h"
#include "FatFsSpiDirReader.h"


Sorcerer2Menu::Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2) :
 PicoWin(0, 0, 80, 30),
  _sdCard(sdCard),
  _sorcerer2(sorcerer2),
  _k1('1'), _k2('2'), _k3('3'), _k4('4'), _k5('5'), _k6('6'), 
  _wiz(5, 6, 70, 18),
  _main(0, 0, 70, 6, 3),
  _mainOp1("Disk drives"),
  _mainOp2("Tape players"),
  _mainOp3("ROM Pack"),
  _mainOp4(),
  _resetOp("Reset"),
  _diskUnits(0, 0, 70, 6, 3),
  _diskUnit(0, 0, 70, 6, 3),
  _diskUnitOp1("Insert"),
  _diskUnitOp2("Eject"),
  _diskUnitOp3("New"),
  _selectDisk(0, 0, 70, 12, 1),
  _diskName(0, 0,70, 64),
  _tapeUnits(0, 0, 70, 5, 3),
  _tapeUnit(0, 0, 70, 6, 3),
  _tapeUnitOp1("Insert"),
  _tapeUnitOp2("Eject"),
  _tapeUnitOp3("New"),
  _selectTape(0, 0, 70, 12, 1),
  _tapeName(0, 0,70, 64),
  _rompacUnit(0, 0, 70, 6, 3),
  _rompacUnitOp1("Insert"),
  _rompacUnitOp2("Eject"),
  _selectRompac(0, 0, 70, 12, 1),
  _message(0, 0, 70, 12),
  _confirm(0, 0, 70, 6, 3),
  _confirmNo("No"),
  _confirmYes("Yes")
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
  _main.addOption(_resetOp.addQuickKey(&_k6));
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
  _mainOp3.toggle([=]() {
    _wiz.push(
      &_rompacUnit,
      [=](PicoPen *pen){ pen->printAtF(0, 0, false,"ROM Pack        [ %-40s]", _sorcerer2->romPac() ? _sorcerer2->romPac()->name() : ""); }, 
      true);
  });
  _mainOp3.onPaint([=](PicoPen *pen){
    Sorcerer2RomPac *rompac = _sorcerer2->romPac();
    pen->clear();
    pen->printAtF(0, 0, false,"ROM Pack        [ %-40s]", rompac ? rompac->name() : "");
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
  _resetOp.toggle([=]() {
    confirm(
      [=](PicoPen *pen){
        pen->printAtF(0, 0, false, "Reset Sorcerer 2 ?");
      },
      [=]() {
      },
      [=]() {
        _sorcerer2->reset();
      }
    );
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
  _diskUnit.addOption(_diskUnitOp3.addQuickKey(&_k3));
  _diskUnit.enableQuickKeys();
  _diskUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectDisk, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose disk image"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, "/sorcerer2/disks");
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
    Sorcerer2Disk *disk = _currentDiskUnit->disk();
    if (disk) {
      // TODO Don't eject disk if drive is active
      confirm(
        [=](PicoPen *pen){
          pen->printAtF(0, 0, false, "Eject disk [ %s ] from %c: ?", disk->name(), _currentDiskUnit->driveLetter());
        },
        [=]() {
        },
        [=]() {
          _currentDiskUnit->eject();
          delete disk;
        }
      );
    }
  });
  _diskUnitOp3.toggle([=]() {
    _wiz.push(
      &_diskName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New disk"); },
      true);
  });
  _diskName.onenter([=](const char* dnr) {

    // Add extension if missing
    std::string dn(dnr);
    auto l = strlen(dnr);
    
    // Should not be case sensitive
    if (l < 4 || !strncmp(dnr + l - 4, ".dsk", 4) == 0) {
      dn.append(".dsk");
    }

    Sorcerer2DiskFatFsSpi *ndisk = new Sorcerer2DiskFatFsSpi(_sdCard, "/sorcerer2/disks/", dn.c_str());

    // check if it already exists
    if (ndisk->exists()) {
      showError([=](PicoPen *pen){
        pen->printAtF(0, 0, true, "'%s' already exists", dn.c_str());
      });
      delete ndisk;
    }
    else if (!ndisk->create()) {
      showError([=](PicoPen *pen){
        pen->printAtF(0, 0, true, "Failed to create '%s'", dn.c_str());
      });
      delete ndisk;
    }
    else {
      Sorcerer2Disk *disk = _currentDiskUnit->insert(ndisk);
      if (disk) delete disk;
      _diskName.clear();
      _wiz.pop(true);
    }
  });
  
  _selectDisk.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2Disk *disk = _currentDiskUnit->insert(new Sorcerer2DiskFatFsSpi(_sdCard, "/sorcerer2/disks/", textOption->text()));
    if (disk) delete disk;
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
  _tapeUnit.addOption(_tapeUnitOp3.addQuickKey(&_k3));
  _tapeUnit.enableQuickKeys();
  _tapeUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectTape, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose tape"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, "/sorcerer2/tapes");
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
    Sorcerer2Tape *tape = _currentTapeUnit->tape();
    // TODO Don't eject tape if drive is active
    if (tape) {
      confirm(
        [=](PicoPen *pen){
          // TODO Say from which tape player
          pen->printAtF(0, 0, false,"Eject tape [ %s ] ?",  tape->name());
        },
        []() {
        },
        [=]() {
          _currentTapeUnit->eject();
          delete tape;
        }
      );
    }
  });
  _tapeUnitOp3.toggle([=]() {
    _wiz.push(
      &_tapeName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New tape"); },
      true);
  });
  
  _selectTape.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2Tape *tape = _currentTapeUnit->insert(new Sorcerer2TapeFatFsSpi(_sdCard, "/sorcerer2/tapes/", textOption->text(), true));
    if (tape) delete tape;
    _wiz.pop(true);
  });
  _tapeName.onenter([=](const char* tnr) {

    // Add extension if missing
    std::string tn(tnr);
    auto l = strlen(tnr);
    
    // Should not be case sensitive
    if (l < 5 || !strncmp(tnr + l - 5, ".tape", 5) == 0) {
      tn.append(".tape");
    }

    Sorcerer2TapeFatFsSpi *ntape = new Sorcerer2TapeFatFsSpi(_sdCard, "/sorcerer2/tapes/", tn.c_str(), true);

    // check if it already exists
    if (ntape->exists()) {
      showError([=](PicoPen *pen){
        pen->printAtF(0, 0, true, "'%s' already exists", tn.c_str());
      });
      delete ntape;
    }
    else {
       Sorcerer2Tape *tape = _currentTapeUnit->insert(ntape);
       if (tape) delete tape;
       _tapeName.clear();
       _wiz.pop(true);
    }
  });

  _rompacUnit.addOption(_rompacUnitOp1.addQuickKey(&_k1));
  _rompacUnit.addOption(_rompacUnitOp2.addQuickKey(&_k2));
  _rompacUnit.enableQuickKeys();
  _rompacUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectRompac, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose ROM Pack"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, "/sorcerer2/rompacs");
    _selectRompac.deleteOptions();
    dirReader.foreach([=](const FILINFO* info) {
      _selectRompac.addOption(new PicoOptionText(info->fname));
    });
   });
  _rompacUnitOp2.toggle([=]() {
    Sorcerer2RomPac *rompac = _sorcerer2->romPac();
    if (rompac) {
      confirm(
        [=](PicoPen *pen){
          pen->printAtF(0, 0, false, "Eject ROM Pack [ %s ] ?", rompac->name());
        },
        [=]() {
        },
        [=]() {
          _sorcerer2->ejectRomPac();
          delete rompac;
        }
      );
    }
  });
  _selectRompac.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2RomPac *rompac = _sorcerer2->insertRomPac(new Sorcerer2RomPacFatFsSpi(_sdCard, "/sorcerer2/rompacs/", textOption->text()));
    if (rompac) delete rompac;
    _wiz.pop(true);
  });
  
  _confirm.addOption(_confirmNo.addQuickKey(&_k1));
  _confirm.addOption(_confirmYes.addQuickKey(&_k2));
  _confirm.enableQuickKeys();

  onPaint([](PicoPen *pen) {
     pen->printAt(0, 0, false, "Exidy Sorcerer 2 emulator");
     pen->printAt(0, 1, false, "on RP2040 Pico Pi");
     pen->printAt(0, 2, false, "Menu System version 0.1");

     pen->printAt(0, 29, false, "F1 to exit menu");
     pen->printAt(80-14, 29, false, "ESC to go back");
   });
}

void Sorcerer2Menu::showError(std::function<void(PicoPen *pen)> message) {
  _wiz.push(
    &_message, 
    [](PicoPen *pen){ pen->printAt(0, 0, false, "Error:"); },
    true);
  _message.onPaint(message);
}

void Sorcerer2Menu::confirm(
  std::function<void(PicoPen *pen)> message,
  std::function<void()> no,
  std::function<void()> yes
) {
  _confirm.focus(0);
  _wiz.push(
    &_confirm, 
    message,
    true);
  _confirmNo.toggle([=]() {
    _wiz.pop(true);
    if (no) no();
  });
  _confirmYes.toggle([=]() {
    _wiz.pop(true);
    if (yes) yes();
  });
}

