#pragma once

#include "Z80.h"
#include "Sorcerer2Keyboard.h"
#include "Sorcerer2DiskSystem.h"
#include "Sorcerer2TapeSystem.h"

class Sorcerer2 {
private:
  Z80 _Z80;
  int _cycles;
  uint32_t _tu4;
  int32_t _ta4;
  Sorcerer2TapeSystem _tapeSystem;
  bool _moderate;
  
  static int readByte(void * context, int address);
  static void writeByte(void * context, int address, int value);

  static int readWord(void * context, int addr)              { return readByte(context, addr) | (readByte(context, addr + 1) << 8); }
  static void writeWord(void * context, int addr, int value) { writeByte(context, addr, value & 0xFF); writeByte(context, addr + 1, value >> 8); }

  static int readIO(void * context, int address);
  static void writeIO(void * context, int address, int value);

  uint8_t _RAM[1<<16];
  uint8_t _buf[1<<15];
  
  Sorcerer2Keyboard *_keyboard;
  Sorcerer2DiskSystem *_diskSystem;
  
  void diskTick();

public:
  Sorcerer2(
    Sorcerer2Keyboard *keyboard,
    Sorcerer2DiskSystem *diskSystem
  );
  inline unsigned char* screenPtr() { return &_RAM[0xF080]; }
  inline unsigned char* charsPtr() { return &_RAM[0xF800]; }
  void reset(unsigned int address);
  void reset();
  void step();
  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);
  Sorcerer2TapeSystem *tapeSystem() { return &_tapeSystem; };
  void saveMem();
  void loadMem();
  void moderate(bool on);
  void toggleModerate();
};

