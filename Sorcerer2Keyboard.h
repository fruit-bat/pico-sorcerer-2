#pragma once

class Sorcerer2Keyboard {
  
  unsigned char _line[16];
  unsigned int _activeLine;

public:
  Sorcerer2Keyboard() : _activeLine(0) {
    reset();
  }

  void reset() {
    for(int i = 0; i < 16; ++i) _line[i] = 0xff;
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
};
