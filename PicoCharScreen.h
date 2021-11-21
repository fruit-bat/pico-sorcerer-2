#pragma once
#include <pico/printf.h>

#define PCS_COLS 80
#define PCS_ROWS 32
#define PCS_CHARS (PCS_COLS * PCS_ROWS)

class PicoCharScreen {
private:
  uint8_t *_chars;

public:
  PicoCharScreen(uint8_t *chars);
  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);
};
