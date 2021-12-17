#pragma once

#include "PicoWin.h"
#include "PicoOption.h"

class PicoSelect : public PicoWin {

private:
  PicoOption **_options;
  int32_t _optionCount;
  int32_t _i;
  bool _multiple;

  void paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i);
  void clearRow(PicoPen *pen, int32_t y);

public:

  PicoSelect(int32_t x, int32_t y, int32_t w, int32_t h, PicoOption **options, uint32_t optionCount, bool multiple) :
    PicoWin(x,y,w,h),
    _options(options),
    _optionCount(optionCount),
    _i(0),
    _multiple(multiple)
  {
  }
  
  virtual void clear(PicoPen *pen) {};
  virtual void paint(PicoPen *pen);
  
  virtual void keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  
};
