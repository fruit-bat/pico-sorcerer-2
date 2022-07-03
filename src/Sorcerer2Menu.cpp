#include "Sorcerer2Menu.h"
#include "PicoPen.h"
#include "Sorcerer2TapeUnitFatFsSpi.h"
#include "Sorcerer2DiskFatFsSpi.h"
#include "Sorcerer2TapeFatFsSpi.h"
#include "Sorcerer2RomPacFatFsSpi.h"
#include "FatFsSpiDirReader.h"
#include "hardware/clocks.h"

#define SAVED_DISKS_DIR "/sorcerer2/disks"
#define SAVED_TAPES_DIR "/sorcerer2/tapes"
#define SAVED_ROMPACS_DIR "/sorcerer2/rompacs"

Sorcerer2Menu::Sorcerer2Menu(SdCardFatFsSpi* sdCard, Sorcerer2 *sorcerer2) :
 PicoWin(0, 0, 80, 30),
  _sdCard(sdCard),
  _sorcerer2(sorcerer2),
  _k1('1'), _k2('2'), _k3('3'), _k4('4'), _k5('5'), _k6('6'), 
  _wiz(5, 6, 70, 18),
  _main(0, 0, 70, 6, 3),
  _mainOp1("Disk system"),
  _mainOp2("Tape system"),
  _mainOp3("ROM Pack"),
  _mainOp4(),
  _resetOp("Reset"),
  _diskUnits(0, 0, 70, 6, 3),
  _diskUnitsOp5("Create a disk"),
  _diskUnitsOp6("Delete a disk"),
  _diskUnit(0, 0, 70, 6, 3),
  _diskUnitOp1("Insert a saved disk"),
  _diskUnitOp2("Insert a new disk"),
  _diskUnitOp3("Eject disk"),
  _selectDisk(0, 0, 70, 15, 1),
  _diskName(0, 0,70, 64),
  _tapeUnits(0, 0, 70, 5, 3),
  _tapeUnitsOp3("Create a tape"),
  _tapeUnitsOp4("Delete a tape"),
  _tapeUnit(0, 0, 70, 6, 3),
  _tapeUnitOp1("Insert a saved tape"),
  _tapeUnitOp2("Insert a new tape"),
  _tapeUnitOp3("Eject tape"),
  _tapeUnitRec(),
  _selectTape(0, 0, 70, 15, 1),
  _tapeName(0, 0,70, 64),
  _rompacUnit(0, 0, 70, 6, 3),
  _rompacUnitOp1("Insert"),
  _rompacUnitOp2("Eject"),
  _selectRompac(0, 0, 70, 12, 1),
  _message(0, 0, 70, 12),
  _confirm(0, 0, 70, 6, 3),
  _confirmNo("No"),
  _confirmYes("Yes"),
  _selectDelete(0, 0, 70, 15, 1)
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
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Disk system"); }, 
      true);
  });
  _mainOp2.toggle([=]() {
    _wiz.push(
      &_tapeUnits,
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Tape system"); }, 
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
        _sorcerer2->reset();
      }
    );
  });
  
  _diskUnits.addOption(_diskUnitsOp1.addQuickKey(&_k1));
  _diskUnits.addOption(_diskUnitsOp2.addQuickKey(&_k2));
  _diskUnits.addOption(_diskUnitsOp3.addQuickKey(&_k3));
  _diskUnits.addOption(_diskUnitsOp4.addQuickKey(&_k4));
  _diskUnits.addOption(_diskUnitsOp5.addQuickKey(&_k5));
  _diskUnits.addOption(_diskUnitsOp6.addQuickKey(&_k6));
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
  _diskUnitsOp5.toggle([=]() {
    _currentDiskUnit = 0;
    _wiz.push(
      &_diskName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New disk name:"); },
      true);
  });
  _diskUnitsOp6.toggle([=]() {
    _wiz.push(
      &_selectDelete, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose a saved disk to delete:"); },
      true);
    loadSavedDisks(&_selectDelete);
    _selectDelete.onToggle([=](PicoOption *option) {
      PicoOptionText *textOption = (PicoOptionText *)option;
      confirm(
        [=](PicoPen *pen){
          pen->printAtF(0, 0, false, "Delete saved disk [ %s ] ?", textOption->text());
        },
        [=]() {
          deleteSave(SAVED_DISKS_DIR, textOption->text());
          loadSavedDisks(&_selectDelete);
        }
      );
    });
  });
  
  _diskUnit.addOption(_diskUnitOp1.addQuickKey(&_k1));
  _diskUnit.addOption(_diskUnitOp2.addQuickKey(&_k2));
  _diskUnit.addOption(_diskUnitOp3.addQuickKey(&_k3));
  _diskUnit.enableQuickKeys();
  _diskUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectDisk, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose disk image:"); },
      true);
    loadSavedDisks(&_selectDisk);
   });
  _diskUnitOp2.toggle([=]() {
    _wiz.push(
      &_diskName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New disk name:"); },
      true);
  });
  _diskUnitOp3.toggle([=]() {
    Sorcerer2Disk *disk = _currentDiskUnit->disk();
    if (disk) {
      // TODO Don't eject disk if drive is active
      confirm(
        [=](PicoPen *pen){
          pen->printAtF(0, 0, false, "Eject disk [ %s ] from %c: ?", disk->name(), _currentDiskUnit->driveLetter());
        },
        [=]() {
          _currentDiskUnit->eject();
          delete disk;
        }
      );
    }
  });
  _diskName.onenter([=](const char* dnr) {
    createDisk(dnr);
  });
  
  _selectDisk.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2Disk *disk = _currentDiskUnit->insert(new Sorcerer2DiskFatFsSpi(_sdCard, SAVED_DISKS_DIR, textOption->text()));
    if (disk) delete disk;
    _wiz.pop(true);
  });

  _tapeUnits.addOption(_tapeUnitsOp1.addQuickKey(&_k1));
  _tapeUnits.addOption(_tapeUnitsOp2.addQuickKey(&_k2));
  _tapeUnits.addOption(_tapeUnitsOp3.addQuickKey(&_k3));
  _tapeUnits.addOption(_tapeUnitsOp4.addQuickKey(&_k4));
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
  _tapeUnitsOp3.toggle([=]() {
    _currentTapeUnit =  0;
    _wiz.push(
      &_tapeName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New tape name:"); },
      true);
  });
  _tapeUnitsOp4.toggle([=]() {
    _wiz.push(
      &_selectDelete, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose a saved tape to delete:"); },
      true);
    loadSavedTapes(&_selectDelete);
    _selectDelete.onToggle([=](PicoOption *option) {
      PicoOptionText *textOption = (PicoOptionText *)option;
      confirm(
        [=](PicoPen *pen){
          pen->printAtF(0, 0, false, "Delete saved tape [ %s ] ?", textOption->text());
        },
        [=]() {
          deleteSave(SAVED_TAPES_DIR, textOption->text());
          loadSavedTapes(&_selectDelete);
        }
      );
    });
  });  
  
  _tapeUnit.addOption(_tapeUnitOp1.addQuickKey(&_k1));
  _tapeUnit.addOption(_tapeUnitOp2.addQuickKey(&_k2));
  _tapeUnit.addOption(_tapeUnitOp3.addQuickKey(&_k3));
  _tapeUnit.addOption(_tapeUnitRec.addQuickKey(&_k4));
  _tapeUnit.enableQuickKeys();

  _tapeUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectTape, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose tape:"); },
      true);
    loadSavedTapes(&_selectTape);
   });
  _tapeUnitOp3.toggle([=]() {
    Sorcerer2Tape *tape = _currentTapeUnit->tape();
    // TODO Don't eject tape if drive is active
    if (tape) {
      confirm(
        [=](PicoPen *pen){
          // TODO Say from which tape player
          pen->printAtF(0, 0, false,"Eject tape [ %s ] ?",  tape->name());
        },
        [=]() {
          _currentTapeUnit->eject();
          delete tape;
        }
      );
    }
  });
  _tapeUnitOp2.toggle([=]() {
    _wiz.push(
      &_tapeName, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "New tape name:"); },
      true);
  });
  _tapeUnitRec.onPaint([=](PicoPen *pen){
    Sorcerer2Tape *tape = _currentTapeUnit->tape();
    pen->clear();
    pen->printAtF(0, 0, false, "Record [ %s ]", tape && tape->isRecording() ? "on" : "off");
  });
  _tapeUnitRec.toggle([=]() {
    Sorcerer2Tape *tape = _currentTapeUnit->tape();
    if (tape) {
      if (tape->isRecording()) {
        // TODO Check for errors
        
        confirm(
          [=](PicoPen *pen){
            // TODO Say from which tape player
            pen->printAtF(0, 0, false,"Stop recording tape [ %s ] ?",  tape->name());
          },
          [=]() {
            tape->close();
            tape->open();
          }
        );        
      }
      else {
        
        // TODO Check for errors

        confirm(
          [=](PicoPen *pen){
            // TODO Say from which tape player
            pen->printAtF(0, 0, false,"Record to tape [ %s ] ?",  tape->name());
          },
          [=]() {
            tape->record();
          }
        );
      }
      repaint();
    }
  });
  
  _selectTape.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2Tape *tape = _currentTapeUnit->insert(new Sorcerer2TapeFatFsSpi(_sdCard, SAVED_TAPES_DIR, textOption->text(), true));
    if (tape) delete tape;
    _wiz.pop(true);
  });
  _tapeName.onenter([=](const char* tnr) {
    createTape(tnr);
  });

  _rompacUnit.addOption(_rompacUnitOp1.addQuickKey(&_k1));
  _rompacUnit.addOption(_rompacUnitOp2.addQuickKey(&_k2));
  _rompacUnit.enableQuickKeys();
  _rompacUnitOp1.toggle([=]() {
    _wiz.push(
      &_selectRompac, 
      [](PicoPen *pen){ pen->printAt(0, 0, false, "Choose ROM Pack"); },
      true);
    FatFsSpiDirReader dirReader(_sdCard, SAVED_ROMPACS_DIR);
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
          _sorcerer2->ejectRomPac();
          delete rompac;
        }
      );
    }
  });
  _selectRompac.onToggle([=](PicoOption *option) {
    PicoOptionText *textOption = (PicoOptionText *)option;
    Sorcerer2RomPac *rompac = _sorcerer2->insertRomPac(new Sorcerer2RomPacFatFsSpi(_sdCard, SAVED_ROMPACS_DIR, textOption->text()));
    if (rompac) delete rompac;
    _wiz.pop(true);
  });
  
  _confirm.addOption(_confirmNo.addQuickKey(&_k1));
  _confirm.addOption(_confirmYes.addQuickKey(&_k2));
  _confirm.enableQuickKeys();

  onPaint([](PicoPen *pen) {
     pen->printAt(0, 0, false, "Exidy Sorcerer 2 emulator");
     pen->printAtF(0, 1, false, "on RP2040 Pico Pi at %3.1fMhz", (float)clock_get_hz(clk_sys) / 1000000.0);
     pen->printAt(0, 2, false, "Menu System version 0.2");

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

void Sorcerer2Menu::loadSavedTapes(PicoSelect* select) {
  FatFsSpiDirReader dirReader(_sdCard, SAVED_TAPES_DIR);
  select->deleteOptions();
  dirReader.foreach([=](const FILINFO* info){ 
    for(int i = 0; i < 2; ++i) {
      Sorcerer2Tape *tape = _sorcerer2->tapeSystem()->unit(i)->tape();
      if (tape && (strcmp(info->fname, tape->name()) == 0)) return;
    }
    select->addOption(new PicoOptionText(info->fname));
  });
}

void Sorcerer2Menu::loadSavedDisks(PicoSelect* select) {
  FatFsSpiDirReader dirReader(_sdCard, SAVED_DISKS_DIR);
  select->deleteOptions();
  dirReader.foreach([=](const FILINFO* info){ 
    for(int i = 0; i < 4; ++i) {
      Sorcerer2Disk *disk = _sorcerer2->diskSystem()->drive(i)->disk();
      if (disk && (strcmp(info->fname, disk->name()) == 0)) return;
    }
    select->addOption(new PicoOptionText(info->fname));
  });
}

void Sorcerer2Menu::confirm(
  std::function<void(PicoPen *pen)> message,
  std::function<void()> yes
) {
  _confirm.focus(0);
  _wiz.push(
    &_confirm, 
    message,
    true);
  _confirmYes.toggle([=]() {
    _wiz.pop(true);
    if (yes) yes();
  });
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

bool Sorcerer2Menu::deleteSave(const char *folder, const char *file) {
  std::string fname(folder);
  fname.append("/");
  fname.append(file);
  return f_unlink(fname.c_str()) == FR_OK;
}

bool Sorcerer2Menu::createTape(const char* tnr) {

  // Add extension if missing
  std::string tn(tnr);
  auto l = strlen(tnr);
  
  // Should not be case sensitive
  if (l < 5 || !strncmp(tnr + l - 5, ".tape", 5) == 0) {
    tn.append(".tape");
  }

  Sorcerer2TapeFatFsSpi *ntape = new Sorcerer2TapeFatFsSpi(_sdCard, SAVED_TAPES_DIR, tn.c_str(), true);

  if (ntape->exists()) {
    showError([=](PicoPen *pen){
      pen->printAtF(0, 0, true, "'%s' already exists", tn.c_str());
    });
    delete ntape;
  }
  else if (!ntape->create()) {
    showError([=](PicoPen *pen){
      pen->printAtF(0, 0, true, "Failed to create '%s'", tn.c_str());
    });
    delete ntape;
  }    
  else {
    if (_currentTapeUnit) {
     Sorcerer2Tape *tape = _currentTapeUnit->insert(ntape);
     if (tape) delete tape;
    }
    else {
      delete ntape;
    }
    _tapeName.clear();
    _wiz.pop(true);
    return true;
  }
  return false;
}

bool Sorcerer2Menu::createDisk(const char* dnr) {
  // Add extension if missing
  std::string dn(dnr);
  auto l = strlen(dnr);
  
  // Should not be case sensitive
  if (l < 4 || !strncmp(dnr + l - 4, ".dsk", 4) == 0) {
    dn.append(".dsk");
  }

  Sorcerer2DiskFatFsSpi *ndisk = new Sorcerer2DiskFatFsSpi(_sdCard, SAVED_DISKS_DIR, dn.c_str());

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
    if (_currentDiskUnit) {
      Sorcerer2Disk *disk = _currentDiskUnit->insert(ndisk);
      if (disk) delete disk;
    }
    else {
      delete ndisk;
    }
    _diskName.clear();
    _wiz.pop(true);
    return true;
  }
  return false;
}
