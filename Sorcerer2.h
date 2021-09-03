#pragma once
#pragma GCC optimize ("O2")

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
  
  static inline int readByte(void * context, int address)
  {
    if (address >= 0xBE00 && address < 0xBE10) {
      return ((Sorcerer2*)context)->_diskSystem->readByte(address);
    }
    return ((Sorcerer2*)context)->_RAM[address];
  }



  static inline void writeByte(void * context, int address, int value)
  {
    auto s = ((Sorcerer2*)context);
    
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
              if (s->_diskSystem) {
                s->_diskSystem->writeByte(address, value);
                return;
              }
            }
          }
          else if (address < 0xBD00) return;
        }
    }
    s->_RAM[address] = value;

    /*
    if (address >= 0xBC00 && address < 0xBD00) return; // Diskboot
    
    if (address >= 0xBE00 && address < 0xBE10 && s->_diskSystem) {
      s->_diskSystem->writeByte(address, value);
      return;
    }
    
    // Diskboot rom  0xBC00 - 0xBD00
    // Monitor rom   0xC000 - 0xF000
    if (address >= 0xC000 && address < 0xF000) return; // Monitor ROM
    if (address >= 0xF800 && address < 0xFC00) return; // Character set ROM
    s->_RAM[address] = value;
    * */
  }
  
  static inline int readIO(void * context, int address)
  {
    const auto m = (Sorcerer2*)context;
    switch(address & 0xFF) {
      case 0xFE: return m->_keyboard->read(address);
      case 0xFC: return m->_tapeSystem.readData();
      case 0xFD: return m->_tapeSystem.readStatus();
      default: return 0xff;
    }
  }

  static inline void writeIO(void * context, int address, int value)
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
  
  static inline int readWord(void * context, int addr) 
  { 
    return readByte(context, addr) | (readByte(context, addr + 1) << 8); 
  }
  
  static inline void writeWord(void * context, int addr, int value) 
  { 
    writeByte(context, addr, value & 0xFF); writeByte(context, addr + 1, value >> 8); 
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
  void step();
  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);
  Sorcerer2TapeSystem *tapeSystem() { return &_tapeSystem; };
  void saveMem();
  void loadMem();
  void moderate(bool on);
  void toggleModerate();
};

