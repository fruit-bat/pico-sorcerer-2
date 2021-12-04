#pragma once

#include "PicoClippedScreen.h"
#include "PicoRectXYXY.h"
#include "PicoRectXYWH.h"

class PicoPen {

  friend class PicoWin;

  PicoRectXYWH _rect;

  // In theory there could be more than one screen (screen->_next)
  PicoClippedScreen* _screen;

public:

  PicoPen(PicoClippedScreen* screen) :
    _rect(0,0,0,0),
    _screen(screen)
  {
  }

  PicoPen(PicoPen* parentPen, PicoRectXYWH *r) :
    _rect(
      parentPen->_rect._x + r->_x,
      parentPen->_rect._y + r->_y,
      r->_w,
      r->_h
    ),
    _screen(parentPen->_screen)
  {
    _screen->clip(&_rect);
  }

  void set(int32_t x, int32_t y, uint8_t c) {
    _screen->set(_rect._x + x, _rect._y + y, c);
  }

  void clear() {
    PicoRectXYXY *clip = &_screen->_clip;
    for(int32_t y = clip->_y1; y < clip->_y2; ++y) {
      for(int32_t x = clip->_x1; x < clip->_x2; ++x) {
        _screen->set(x, y, 32);
      }
    }
  }

  void printAt(int32_t x, int32_t y, bool wrap, const char *str);
  
  void printAtF(int32_t x, int32_t y, bool wrap, const char *fmt, ...);
};
