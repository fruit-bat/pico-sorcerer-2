#include "Sorcerer2.h"
#include "monitor.h"
#include "exchr.h"
#include "diskboot.h"
#include <memory.h>

Sorcerer2::Sorcerer2(
  Sorcerer2Keyboard *keyboard,
  Sorcerer2DiskSystem *diskSystem
) : 
  _moderate(true),
  _RAM{0},
  _keyboard(keyboard),
  _diskSystem(diskSystem)
{
  z80Power(true);
  _Z80.context = this;
  _Z80.read = readByte;
  _Z80.fetch = readByte;
  _Z80.fetch_opcode = readByte;
  _Z80.write = writeByte;
  _Z80.in = readIO;
  _Z80.out = writeIO;
  _Z80.inta = readInt;
  memcpy(&_RAM[0xE000], monitor, 0x1000);
  memcpy(&_RAM[0xF800], exchr, 0x400);
  if (_diskSystem) {
    memcpy(&_RAM[0xBC00], diskboot, 0x100);
  }
}

void Sorcerer2::reset(uint16_t address)
{
  z80Reset();

  Z80_PC(_Z80) = address;
  _cycles = 0;
  _centronicsOut = 0;
}

void Sorcerer2::diskTick()
{
  if (_diskSystem) _diskSystem->tick();
}

// This is just a guess at the CPU cycles for a sector to pass under the read head
//
//   Disk rotation rate 300rpm (based on micropolis docs)
//   CPU 4MHz
//   16 sectors per track
//
//    (4 * 10^6) / ( (300 / 60) * 16)
//  = (4 * 10^6) / ( 5 * 16) 
//  = 10^6 / ( 5 * 4 )
//  = 10^5 / 2
//  = 5 * 10^4
//

void Sorcerer2::reset() { 
  _tapeSystem.reset();
  if (_diskSystem) _diskSystem->reset();
  reset(0xE000); 
  _tu4 = time_us_32() << 2;
  _ta4 = 0;
}

void Sorcerer2::saveMem() {
  memcpy(_buf, _RAM + 0x100, 1<<15);
}

void Sorcerer2::loadMem() {
  memcpy(_RAM + 0x100, _buf, 1<<15);
}

bool Sorcerer2::moderate() {
  return _moderate;
}

void Sorcerer2::moderate(bool on) {
  if (on == _moderate) return;
  
  if (on) {
    _tu4 = time_us_32() << 2;
    _ta4 = 0;
  }
  
  _moderate = on;
}

void Sorcerer2::toggleModerate() {
  moderate(!_moderate);
}

Sorcerer2RomPac* Sorcerer2::insertRomPac(Sorcerer2RomPac* rompac) {
  Sorcerer2RomPac* r = _rompac;
  if (rompac->open()) {
    for (int i = 0; i < 1024 * 8; ++i) {
      _RAM[0xC000 + i] = rompac->read();
    }
    rompac->close();
    _rompac = rompac;
  }
  else {
    _rompac = 0;
  }
  return r;
}

Sorcerer2RomPac* Sorcerer2::ejectRomPac() {
  Sorcerer2RomPac* r = _rompac;
  for (int i = 0; i < 1024 * 8; ++i) {
    _RAM[0xC000 + i] = 0;
  }
  _rompac = 0;
  return r;  
}


