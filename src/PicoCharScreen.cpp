#include "PicoCharScreen.h"
#include <memory.h>

PicoCharScreen::PicoCharScreen(uint16_t *chars, uint32_t w, uint32_t h) :
  _chars(chars),
  _s(w * h),
  _rect(0, 0, w, h)
{
}

void PicoCharScreen::clear() {
  for (int32_t i = 0; i < _s; ++i) _chars[i] = 32;
}
