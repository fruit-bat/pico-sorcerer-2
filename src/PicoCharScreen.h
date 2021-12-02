#pragma once
#include <pico/printf.h>
#include "PicoRectXYWH.h"

class PicoCharScreen {
private:
  uint8_t *_chars;
  int32_t _s;
  PicoRectXYWH _rect;
  
public:

  PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h);
  
  inline void set(int32_t x, int32_t y, uint8_t c) {
    _chars[x +(y * _rect._w)] = c;
  }

  void clear();

  PicoRectXYWH *rect() { return &_rect; } 
};
