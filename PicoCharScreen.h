#pragma once
#include <pico/printf.h>
#include "PicoWin.h"

#define PCS_COLS 80
#define PCS_ROWS 32
#define PCS_CHARS (PCS_COLS * PCS_ROWS)

class PicoCharScreen : PicoWin {
private:
  uint8_t *_chars;
  int32_t _s;

public:
  PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h);
  
  void write( x, int32_t y, uint32_t w, uint8_t *s);
  void write(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t c);
};
