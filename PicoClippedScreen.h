#pragma once
#include "PicoCharScreen.h"
#include "PicoRectXYXY.h"

class PicoClippedScreen {
 
  PicoCharScreen  *_screen;
  PicoRectXYXY _clip;

public:

  PicoClippedScreen(PicoCharScreen  *screen) :
    _screen(screen),
    _clip(screen->rect())
  {
  }
  
  PicoClippedScreen(PicoClippedScreen  *clippedScreen, PicoRectXYWH *r) :
    _screen(clippedScreen->_screen),
    _clip(clippedScreen->_clip)
  {
    _clip.intersect(r);
  }

  void set(int32_t x, int32_t y, uint8_t c) {
    if (_clip.containsPos(x, y)) {
      _screen->set(x, y, c);
    }
  }
};
