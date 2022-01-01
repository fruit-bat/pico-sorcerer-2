#pragma once
#include <pico/printf.h>
#include "PicoRectXYWH.h"

class PicoCharScreen {
private:
  uint16_t *_chars;
  int32_t _s;
  PicoRectXYWH _rect;
  
public:

  PicoCharScreen(uint16_t *chars, uint32_t w, uint32_t h);
  
  inline void set(int32_t x, int32_t y, uint16_t c) {
    _chars[x +(y * _rect._w)] = c;
  }

  void clear();

  PicoRectXYWH *rect() { return &_rect; } 
};
