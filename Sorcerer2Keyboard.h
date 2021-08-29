#pragma once

#include "Sorcerer2Input.h"
#include "Sorcerer2Output.h"

// TODO Sorcerer2Input, Sorcerer2Output look a bit pointless
class Sorcerer2Keyboard : public Sorcerer2Input, Sorcerer2Output {
private:
  unsigned char _line[16];
  unsigned int _activeLine;
  bool _reset1;
  bool _reset2;
  
public:
  Sorcerer2Keyboard() : _activeLine(0), _reset1(false), _reset2(false) {
    reset();
  }

  void reset() {
    for(int i = 0; i < 16; ++i) _line[i] = 0xff;
    _reset1 = false;
    _reset2 = false;
  }

  void press(unsigned int line, unsigned int key) {
    _line[line] &= ~(1 << key);
  }

  void release(unsigned int line, unsigned int key) {
    _line[line] |= 1 << key;
  }

  unsigned char read(unsigned short address) {
    return _line[_activeLine];
  }

  void write(unsigned short address, unsigned char byte) {
    _activeLine = byte & 0xf;
  }
  
  void setReset1(bool pressed) {
    _reset1 = pressed;
  }

  void setReset2(bool pressed) {
    _reset2 = pressed;
  }
  
  bool resetPressed() {
    return _reset1 && _reset2;
  }
};
