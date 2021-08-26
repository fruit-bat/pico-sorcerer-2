#include "Sorcerer2.h"
#include "monitor.h"
#include "exchr.h"
#include "diskboot.h"
#include <memory.h>
#include <pico/printf.h>
#include "Sorcerer2HidKeyboard.h"

Sorcerer2::Sorcerer2(Sorcerer2Keyboard *keyboard) : _keyboard(keyboard) 
{
  _Z80.setCallbacks(this, readByte, writeByte, readWord, writeWord, readIO, writeIO);
  memcpy(&_RAM[0xE000], monitor, 0x1000);
  memcpy(&_RAM[0xF800], exchr, 0x400);
  memcpy(&_RAM[0xBC00], diskboot, 0x100);
}


int Sorcerer2::readByte(void * context, int address)
{
  if (address >= 0xBE00 && address < 0xBE10) {
    // Disk system IO
    printf("Disk read %04X\n", address);
  }
  return ((Sorcerer2*)context)->_RAM[address];
}


void Sorcerer2::writeByte(void * context, int address, int value)
{
  if (address >= 0xBE00 && address < 0xBE10) {
    // Disk system IO
    printf("Disk write %04X %02X\n", address, value);
  }
  // Diskboot rom  0xBC00 - 0xBD00
  // Monitor rom   0xC000 - 0xF000
  if (address >= 0xBC00 && address < 0xF000) return; // Diskboot and Monitor ROM
  if (address >= 0xF800 && address < 0xFC00) return; // Character set ROM
  ((Sorcerer2*)context)->_RAM[address] = value;
}

int Sorcerer2::readIO(void * context, int address)
{
  const auto m = (Sorcerer2*)context;
  if ((address & 0xFF) == 0xFE) { 
    const int c = m->_keyboard->read(address); 
    // m->printAtF(0,26,"IO IN:%02X %02X", address, c);
    return c;
  }
  if ((address & 0xff) == 0xFD) return 0xFC;
  return 0xff;
}

void Sorcerer2::writeIO(void * context, int address, int value)
{
  const auto m = (Sorcerer2*)context;
  // m->printAtF(0,27,"IO OU:%02X %02X", address, value);
  if ((address & 0xFF) == 0xFE) m->_keyboard->write(address, value); 

  // not handlded!
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
}

// This is just a guess!
#define CYCLES_PER_DISK_TICK 100000

void Sorcerer2::step()
{
  // printAtF(0,0, "PC:%04X ", _Z80.getPC()); 
  for(int i=0; i < 50; ++i) {
    _cycles += _Z80.step();
    if (_cycles >= CYCLES_PER_DISK_TICK) {
      diskTick();
      _cycles -= CYCLES_PER_DISK_TICK;
    }
  }
}