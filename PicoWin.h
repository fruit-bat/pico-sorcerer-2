#pragma once
#include "PicoRect.h"
#include "PicoSubWin.h"

class PicoWin {

protected:
  PicoRect _rect;
  PicoSubWin *_firstChild;

public:

  PicoWin(int32_t x1, int32_t y1, int32_t x2, int32_t y2) :
    _rect(x1, y1, x2, y2)
  {
  }

  inline int32_t w() { return _rect.w(); };
  inline int32_t h() { return _rect.h(); };

  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);

  
};

