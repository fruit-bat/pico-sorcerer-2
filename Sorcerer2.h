#pragma once
#pragma GCC optimize ("O2")

#include "Z80.h"
#include "Sorcerer2Keyboard.h"
#include "Sorcerer2DiskSystem.h"
#include "Sorcerer2TapeSystem.h"
#include <pico/printf.h>
#include <pico/stdlib.h>

#define CYCLES_PER_DISK_TICK 50000

class Sorcerer2 {
private:
  Z80 _Z80;
  int _cycles;
  uint32_t _tu4;
  int32_t _ta4;
  Sorcerer2TapeSystem _tapeSystem;
  bool _moderate;
  uint8_t _centronicsOut;
  
  inline int readByte(int address)
  {
    if (address >= 0xBE00 && address < 0xBE10 && _diskSystem) {
      return _diskSystem->readByte(address);
    }
    return _RAM[address];
  }

  inline void writeByte(int address, int value)
  {
    if (address >= 0xBC00) {
        if (address >= 0xC000) {
          if (address >= 0xF800) {
            if (address < 0xFC00) return;
          }
          else if (address < 0xF000) { 
            return;
          }
        }
        else {
          if (address >= 0xBE00) {
            if (address < 0xBE10) {
              if (_diskSystem) {
                _diskSystem->writeByte(address, value);
                return;
              }
            }
          }
          else if (address < 0xBD00) return;
        }
    }
    _RAM[address] = value;

    /*
    if (address >= 0xBC00 && address < 0xBD00) return; // Diskboot
    
    if (address >= 0xBE00 && address < 0xBE10 && _diskSystem) {
      _diskSystem->writeByte(address, value);
      return;
    }
    
    // Diskboot rom  0xBC00 - 0xBD00
    // Monitor rom   0xC000 - 0xF000
    if (address >= 0xC000 && address < 0xF000) return; // Monitor ROM
    if (address >= 0xF800 && address < 0xFC00) return; // Character set ROM
    _RAM[address] = value;
    * */
  }
  
  inline int readIO(int address)
  {
    switch(address & 0xFF) {
      case 0xFE: return _keyboard->read(address);
      case 0xFC: return _tapeSystem.readData();
      case 0xFD: return _tapeSystem.readStatus();
      case 0xFF: return 0xFF; // Centronics
      default: return 0xFF;
    }
  }

  inline void writeIO(int address, int value)
  {
    switch(address & 0xFF) {
      case 0xFE: {
        _keyboard->write(address, value);
        _tapeSystem.writeControl(value);
        break;
      }
      case 0xFC: {
        _tapeSystem.writeData(value);
        break;
      }
      case 0xFF: {
        _centronicsOut = value;
        //printf("Centronics out %02X %d\n", _centronicsOut, (int)((int8_t)_centronicsOut));
      }
      default: break;
    }
  }

  inline int readWord(int addr) { 
    return readByte(addr) | (readByte((addr + 1) & 0xffff) << 8);
  }
  
  inline void writeWord(int addr, int value) { 
    writeByte(addr, value & 0xFF); 
    writeByte((addr + 1) & 0xffff, value >> 8);
  }
  
  static inline int readByte(void * context, int address) {
    return ((Sorcerer2*)context)->readByte(address);
  }

  static inline void writeByte(void * context, int address, int value) {
    ((Sorcerer2*)context)->writeByte(address, value);
  }
  
  // TODO Can addr ever be odd (if not readWord can be simplified)? 
  static inline int readWord(void * context, int addr) { 
    return ((Sorcerer2*)context)->readWord(addr); 
  }
  
  // TODO Can addr ever be odd (if not writeWord can be simplified)?
  static inline void writeWord(void * context, int addr, int value) { 
    ((Sorcerer2*)context)->writeWord(addr, value);
  }
  
  static inline int readIO(void * context, int address)
  {
    //printf("readIO %04X\n", address);
    const auto m = (Sorcerer2*)context;
    return m->readIO(address);
  }

  static inline void writeIO(void * context, int address, int value)
  {
    //printf("writeIO %04X %02X\n", address, value);
    const auto m = (Sorcerer2*)context;
    m->writeIO(address, value);
  }

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
  void __not_in_flash_func(step)();
  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);
  Sorcerer2TapeSystem *tapeSystem() { return &_tapeSystem; };
  void saveMem();
  void loadMem();
  void moderate(bool on);
  void toggleModerate();
  uint8_t inline getCentronics() { return _centronicsOut; }
  
  inline void stepCpu()
  {
    // printAtF(0,0, "PC:%04X ", _Z80.getPC()); 
      int c = _Z80.step();
      _cycles += c;
      if (_moderate) {
        uint32_t tu4 = time_us_32() << 2;
        _ta4 += c - tu4 + _tu4; // +ve too fast, -ve too slow
        _tu4 = tu4;
        if (_ta4 >= 4) busy_wait_us_32(_ta4 >> 2);
        if (_ta4 < -100000) _ta4 = -100000;
      }
  }

  inline void stepDisk()
  {
    if (_cycles >= CYCLES_PER_DISK_TICK) {
      diskTick();
      _cycles -= CYCLES_PER_DISK_TICK;
    }
  }

};

