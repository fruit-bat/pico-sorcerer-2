#pragma once
#include <pico/printf.h>

class PicoRectXYWH {

public:

  int32_t _x,_y,_w,_h;

  inline PicoRectXYWH(int32_t x, int32_t y, int32_t w, int32_t h) :
    _x(x), _y(y), _w(w), _h(h)
  {
  }

  inline PicoRectXYWH(PicoRectXYWH *c) :
    _x(c->_x), _y(c->_y), _w(c->_w), _h(c->_h)
  {
  }

  inline bool notEmpty() {
    return _w > 0 && _h > 0;
  }
  
  inline bool empty() {
    return _w <= 0 && _h <= 0;
  }
  
  inline bool containsRow(int32_t y) {
    return y >= _y && y < (_y + _h);
  }
  
  inline bool containsCol(int32_t x) {
    return x >= _x && x < (_x + _h);
  }

  inline bool containsPos(int32_t x, int32_t y) {
    return containsRow(y) && containsCol(x);
  }

  inline int32_t w() {
    return _w;
  }

  inline int32_t h() {
    return _h;
  }
};
