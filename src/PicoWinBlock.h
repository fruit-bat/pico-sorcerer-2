#pragma once

#include "PicoWin.h"

class PicoWinBlock : public PicoWin {
  
  uint8_t _c;

public:

  PicoWinBlock(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t c) :
    PicoWin(x,y,w,h),
    _c(c)
  {
  }

  virtual void paint(PicoPen *pen);

};
