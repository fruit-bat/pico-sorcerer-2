#pragma once
#include "PicoRect.h"

class PicoSubWin;

class PicoWin {
  friend PicoSubWin;
protected:

  int32_t _w, _h;
  PicoSubWin *_firstChild;
  bool _repaintChild;

public:

  PicoWin(int32_t w, int32_t h) :
    _w(w), _h(h)
  {
  }

  inline int32_t w() { return _w; }
  inline int32_t h() { return _h; }
  inline PicoSubWin *firstChild() { return _firstChild; }
};

