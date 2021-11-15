#include "Sorcerer2.h"
#include "monitor.h"
#include "exchr.h"
#include "diskboot.h"
#include <memory.h>

Sorcerer2::Sorcerer2(
  Sorcerer2Keyboard *keyboard,
  Sorcerer2DiskSystem *diskSystem
) : 
  _moderate(false),
  _RAM{0},
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

void Sorcerer2::saveMem()
{
  memcpy(_buf, _RAM + 0x100, 1<<15);
}

void Sorcerer2::loadMem()
{
  memcpy(_RAM + 0x100, _buf, 1<<15);
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
