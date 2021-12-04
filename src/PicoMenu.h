#pragma once

#include "PicoWin.h"

class PicoMenu : public PicoWin {
  
  const char **_options;
  uint32_t _optionCount;

public:

  PicoMenu(int32_t x, int32_t y, int32_t w, int32_t h, const char **options, uint32_t optionCount) :
    PicoWin(x,y,w,h),
    _options(options),
    _optionCount(optionCount)
  {
  }

  virtual void paint(PicoPen *pen);
  
  virtual int32_t getOption() { return -1; }
  virtual void setOption(int32_t option) {}
};
