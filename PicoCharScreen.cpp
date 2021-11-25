#include "PicoCharScreen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

PicoCharScreen::PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h) :
  PicoWin(
    0,
    0,
    w,
    h
  ),
  _chars(chars),
  _s(w * h)
{
  for (uint32_t i = 0; i < _s; ++i) _chars[i] = (i % (96)) + 32;
}

void PicoCharScreen::write(int32_t x, int32_t y, uint32_t w, uint8_t *s){
  const uint8_t si = (y * PCS_COLS) + x;
  if (si >= PCS_CHARS) return;
  uint8_t *p = _chars + si;
  for(uint32_t i = 0; i < PCS_COLS && *s != 0 && i < w; ++i) *p++ = *s++; 
}

void PicoCharScreen::write(int32_t x, int32_t y, uint32_t w, uint32_t h, uint8_t c) {

}
