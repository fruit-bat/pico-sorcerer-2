#pragma once
#pragma GCC optimize ("O2")

#include "Z80.h"
#include "Sorcerer2Keyboard.h"
#include "Sorcerer2DiskSystem.h"
#include "Sorcerer2TapeSystem.h"
#include "Sorcerer2RomPac.h"
#include <pico/printf.h>
#include <pico/stdlib.h>

#define CYCLES_PER_DISK_TICK 50000

class Sorcerer2 {
private:
  Z80 _Z80;
  int32_t _cycles;
  int32_t _ta32;
  Sorcerer2TapeSystem _tapeSystem;
  bool _mute;
  uint8_t _centronicsOut;
  Sorcerer2RomPac *_rompac;
  uint32_t _moderate;

  inline uint32_t z80Step(uint32_t tstates) {
    return z80_run(&_Z80, tstates);
  }
  
  inline void z80Power(bool state) {
    z80_power(&_Z80, true);
  }
  
  inline void z80Reset() {
    z80_instant_reset(&_Z80);
  }

  inline uint8_t readByte(uint16_t address) {
    if (address >= 0xBE00 && address < 0xBE10 && _diskSystem) {
      return _diskSystem->readByte(address);
    }
    return _RAM[address];
  }

  inline void writeByte(uint16_t address, uint8_t value) {
    if (address >= 0xBC00) {
        if (address >= 0xC000) {
          if (_rompac && address < 0xE000) return;
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
  
  inline uint8_t readIO(uint16_t address) {
    switch(address & 0xFF) {
      case 0xFE: return _keyboard->read(address);
      case 0xFC: return _tapeSystem.readData();
      case 0xFD: return _tapeSystem.readStatus();
      case 0xFF: return 0xFF; // Centronics
      default: return 0xFF;
    }
  }

  inline void writeIO(uint16_t address, uint8_t value) {
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

  static uint8_t __not_in_flash_func(readByte)(void * context, uint16_t address) {
    return ((Sorcerer2*)context)->readByte(address);
  }
  
  static void __not_in_flash_func(writeByte)(void * context, uint16_t address, uint8_t value) {
    ((Sorcerer2*)context)->writeByte(address, value);
  }
   
  static uint8_t __not_in_flash_func(readIO)(void * context, uint16_t address) {
    return ((Sorcerer2*)context)->readIO(address);
  }

  static void __not_in_flash_func(writeIO)(void * context, uint16_t address, uint8_t value) {
    ((Sorcerer2*)context)->writeIO(address, value);
  }
  
  static uint8_t __not_in_flash_func(readInt)(void * context, uint16_t address) {
    z80_int(&(((Sorcerer2*)context)->_Z80), false);
    return 0xff;
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
  inline uint8_t* screenPtr() { return &_RAM[0xF080]; }
  inline uint8_t* charsPtr() { return &_RAM[0xF800]; }
  void reset(uint16_t address);
  void reset();
  void __not_in_flash_func(step)();
  Sorcerer2TapeSystem *tapeSystem() { return &_tapeSystem; }
  Sorcerer2DiskSystem *diskSystem() { return _diskSystem; }
  void saveMem();
  void loadMem();
  void mute(bool mute) { _mute = mute; }
  void toggleMute() { _mute = !_mute; }
  bool mute() { return _mute; }
  Sorcerer2RomPac* insertRomPac(Sorcerer2RomPac* rompac);
  Sorcerer2RomPac* ejectRomPac();
  Sorcerer2RomPac* romPac() { return _rompac; }

  void moderate(uint32_t mul);
  void toggleModerate();
  uint32_t moderate() { return _moderate; }

  uint8_t inline getCentronics() { return _centronicsOut; }
  uint8_t inline getSound() { return _mute ? 0 : _centronicsOut >> 2; }

  void stepDisk();
  uint32_t stepCpu();

};

