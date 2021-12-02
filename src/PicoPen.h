#pragma once

#include "PicoClippedScreen.h"
#include "PicoRectXYXY.h"
#include "PicoRectXYWH.h"

class PicoPen {

  PicoRectXYWH _rect;
  PicoClippedScreen* _screen;

public:

  PicoPen(PicoClippedScreen* screen, PicoRectXYWH *r) :
    _rect(r),
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

  void printAt(int32_t x, int32_t y, bool wrap, uint8_t *str);
  void printAtF(int32_t x, int32_t y, bool wrap, uint8_t *fmt, ...);
};
