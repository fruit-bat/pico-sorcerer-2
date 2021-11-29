#include "PicoCharScreen.h"
#include "PicoPen.h"

PicoCharScreen::PicoCharScreen(uint8_t *chars, uint32_t w, uint32_t h) :
  PicoWin(w, h),
  _chars(chars),
  _s(w * h)
{
  for (uint32_t i = 0; i < _s; ++i) _chars[i] = (i % (96)) + 32;
}

void PicoCharScreen::refresh() {
  if (_repaintChild) {
    PicoPen pen(this);    
    for(PicoSubWin *win = _firstChild; win != 0; win = win->_nextChild) {
      win->refresh(&pen);
    }
  }
}
