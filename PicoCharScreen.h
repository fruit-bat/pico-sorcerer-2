#pragma once
#include <pico/printf.h>
#include <memory.h>
#include "PicoWin.h"

#define PCS_COLS 80
#define PCS_ROWS 32
#define PCS_CHARS (PCS_COLS * PCS_ROWS)

class PicoCharScreen : public PicoWin {
private:
  uint8_t *_chars;
  int32_t _s;

public:
  PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h);
  
  inline void set(int32_t x, int32_t y, uint8_t c) {
    _chars[x +(y * _w)] = c;
  }

  void clear() {
    memset(_chars, 32, _s);
  }

  void refresh();
};
