#pragma once

#include "PicoWin.h"
#include <functional>

class PicoWinFunc : public PicoWin {
private:
  std::function<void(PicoPen *pen)> _paint;
public:
  PicoWinFunc(int32_t x, int32_t y, int32_t w, int32_t h);
  virtual void paint(PicoPen *pen);
  void onPaint(std::function<void(PicoPen *pen)> paint) { _paint = paint; }
};

