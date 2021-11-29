#pragma once

#include "PicoCharScreen.h"
#include "PicoRect.h"
#include "PicoSubWin.h"

class PicoPen {

  PicoCharScreen* _screen;
  PicoRect _clip;
  int32_t _x, _y, _w, _h;

public:

  PicoPen(PicoCharScreen* screen) :
    _screen(screen),
    _x(0),
    _y(0),
    _w(screen->w()),
    _h(screen->h()),
    _clip(0, 0, screen->w(), screen->h())
  {
  }

  PicoPen(PicoPen* parentPen, PicoSubWin *subWin) :
    _screen(parentPen->_screen),
    _x(parentPen->_x + subWin->rx()),
    _y(parentPen->_y + subWin->ry()),
    _w(subWin->w()),
    _h(subWin->h()),
    _clip(_x, _y, _x + _h, _y + _h)
  {
    _clip.intersect(parentPen->_clip);
  }

  void set(int32_t rx, int32_t ry, uint8_t c) {
    int32_t sx = _x + rx;
    int32_t sy = _y + ry;
    if (_clip.containsPos(sx, sy)) {
      _screen->set(sx, sy, c);
    }
  }

  void clear() {
    for(int32_t y = _clip._y1; y < _clip._y2; ++y) {
      for(int32_t x = _clip._x1; x < _clip._x2; ++x) {
        _screen->set(x, y, 32);
      }
    }
  }

  void printAt(int32_t x, int32_t y, bool wrap, uint8_t *str);
  void printAtF(int32_t x, int32_t y, bool wrap, uint8_t *fmt, ...);
};
