#pragma once

#include "PicoWin.h"

class PicoSelect : public PicoWin {
  
  const char **_options;
  int32_t _optionCount;
  int32_t _i;

public:

  PicoSelect(int32_t x, int32_t y, int32_t w, int32_t h, const char **options, uint32_t optionCount) :
    PicoWin(x,y,w,h),
    _options(options),
    _optionCount(optionCount),
    _i(0)
  {
  }

  virtual void paint(PicoPen *pen);
  
  virtual void keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  
};
