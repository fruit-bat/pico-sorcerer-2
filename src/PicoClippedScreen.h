#pragma once
#include "PicoCharScreen.h"
#include "PicoRectXYXY.h"

class PicoClippedScreen {
  
friend class PicoPen;

  PicoCharScreen  *_screen;
  PicoRectXYXY _clip;

public:

  PicoClippedScreen(PicoCharScreen  *screen) :
    _screen(screen),
    _clip(screen->rect())
  {
  }
  
  PicoClippedScreen(PicoClippedScreen  *clippedScreen) :
    _screen(clippedScreen->_screen),
    _clip(clippedScreen->_clip)
  {
  }

  void clip(PicoRectXYWH *r) {
    _clip.intersect(r);
  }

  void set(int32_t x, int32_t y, uint16_t c) {
    if (_clip.containsPos(x, y)) {
      _screen->set(x, y, c);
    }
  }
};
