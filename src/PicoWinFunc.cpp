#include "PicoWinFunc.h"

PicoWinFunc::PicoWinFunc(int32_t x, int32_t y, int32_t w, int32_t h) :
  PicoWin(x, y, w, h)
{
}

void PicoWinFunc::paint(PicoPen *pen) {
  if (_paint) _paint(pen);
}
