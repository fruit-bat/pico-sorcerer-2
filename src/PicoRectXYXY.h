#pragma once
#include <pico/printf.h>

class PicoRectXYWH;

class PicoRectXYXY {

public:

  int32_t _x1,_y1,_x2,_y2;

  PicoRectXYXY(PicoRectXYWH *c);

  inline PicoRectXYXY(int32_t x1, int32_t y1, int32_t x2, int32_t y2) :
    _x1(x1), _y1(y1), _x2(x2), _y2(y2)
  {
  }

  inline PicoRectXYXY(PicoRectXYXY *c) :
    _x1(c->_x1), _y1(c->_y1), _x2(c->_x2), _y2(c->_y2)
  {
  }
  
  void intersect(PicoRectXYWH *c);

  void intersect(PicoRectXYXY *c) {
    if (c->_x1 > _x1) _x1 = c->_x1;
    if (c->_y1 > _y1) _y1 = c->_y1;
    if (c->_x2 < _x2) _x2 = c->_x2;
    if (c->_y2 < _y2) _y1 = c->_y2;
  }

  inline bool notEmpty() {
    return _x2 > _x1 && _y2 > _y1;
  }
  
  inline bool empty() {
    return _x2 <= _x1 && _y2 <= _y1;
  }

  inline bool containsRow(int32_t y) {
    return y >= _y1 && y < _y2;
  }
  
  inline bool containsCol(int32_t x) {
    return x >= _x1 && x < _x2;
  }

  inline bool containsPos(int32_t x, int32_t y) {
    return containsRow(y) && containsCol(x);
  }

  inline int32_t w() {
    return _x2 - _x1;
  }

  inline int32_t h() {
    return _y2 - _y1;
  }
};
