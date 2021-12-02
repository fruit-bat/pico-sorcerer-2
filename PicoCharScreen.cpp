#include "PicoCharScreen.h"

PicoCharScreen::PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h) :
  _chars(chars),
  _s(w * h),
  _rect(0, 0, w, h)
{
  for (int32_t i = 0; i < _s; ++i) _chars[i] = (i % (96)) + 32;
}

