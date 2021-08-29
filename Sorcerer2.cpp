#include "Sorcerer2.h"
#include "monitor.h"
#include "exchr.h"
#include "diskboot.h"
#include <memory.h>
#include <pico/printf.h>
#include "Sorcerer2HidKeyboard.h"

Sorcerer2::Sorcerer2(
  Sorcerer2Keyboard *keyboard,
  Sorcerer2DiskSystem *diskSystem
) : 
  _keyboard(keyboard),
  _diskSystem(diskSystem)
{
  _Z80.setCallbacks(this, readByte, writeByte, readWord, writeWord, readIO, writeIO);
  memcpy(&_RAM[0xE000], monitor, 0x1000);
  memcpy(&_RAM[0xF800], exchr, 0x400);
  if (_diskSystem) {
    memcpy(&_RAM[0xBC00], diskboot, 0x100);
  }
}

int Sorcerer2::readByte(void * context, int address)
{
  if (address >= 0xBE00 && address < 0xBE10) {
    return ((Sorcerer2*)context)->_diskSystem->readByte(address);
  }
  return ((Sorcerer2*)context)->_RAM[address];
}

void Sorcerer2::writeByte(void * context, int address, int value)
{
  auto s = ((Sorcerer2*)context);
  if (address >= 0xBE00 && address < 0xBE10 && s->_diskSystem) {
    ((Sorcerer2*)context)->_diskSystem->writeByte(address, value);
    return;
  }
  
  // Diskboot rom  0xBC00 - 0xBD00
  // Monitor rom   0xC000 - 0xF000
  if (address >= 0xBC00 && address < 0xF000) return; // Diskboot and Monitor ROM
  if (address >= 0xF800 && address < 0xFC00) return; // Character set ROM
  s->_RAM[address] = value;
}

int Sorcerer2::readIO(void * context, int address)
{
  const auto m = (Sorcerer2*)context;
  switch(address & 0xFF) {
    case 0xFE: return m->_keyboard->read(address);
    case 0xFC: return m->_tapeSystem.readData();
    case 0xFD: return m->_tapeSystem.readStatus();
    default: return 0xff;
  }
}

void Sorcerer2::writeIO(void * context, int address, int value)
{
  const auto m = (Sorcerer2*)context;
  switch(address & 0xFF) {
    case 0xFE: {
      m->_keyboard->write(address, value);
      m->_tapeSystem.writeControl(value);
      break;
    }
    case 0xFC: {
      m->_tapeSystem.writeData(value);
      break;
    }
    default: break;
  }  
}

void Sorcerer2::printAt(unsigned int x, unsigned int y, const char *s) {
  const unsigned int m = 64 * 30;
  const unsigned int p = x + (y<<6);
  if (p >= m) return;
  const unsigned int l = strlen(s);
  for(unsigned int i=0; i<l; ++i) {
    ((char *)screenPtr())[(p + i) % m] = s[i];
  }
}

void Sorcerer2::printAtF(unsigned int x, unsigned int y, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, args );
  printAt(x, y, buf);
  va_end(args);
}

void Sorcerer2::reset(unsigned int address)
{
  _Z80.reset();
  _Z80.setPC(address);
  _cycles = 0;
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
#define CYCLES_PER_DISK_TICK 50000

void Sorcerer2::step()
{
  // printAtF(0,0, "PC:%04X ", _Z80.getPC()); 
  for(int i=0; i < 50; ++i) {
    _cycles += _Z80.step();
  }
  if (_cycles >= CYCLES_PER_DISK_TICK) {
    diskTick();
    _cycles -= CYCLES_PER_DISK_TICK;
  }
}

void Sorcerer2::reset() { 
  _tapeSystem.reset();
  if (_diskSystem) _diskSystem->reset();
  reset(0xE000); 
}
