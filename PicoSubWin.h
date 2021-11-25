#pragma once
#include "PicoWin.h"

class PicoSubWin : PicoWin {

  PicoWin *_parent;
  PicoSubWin *_nextChild;
  bool _show;
  
public:

  PicoSubWin(PicoWin *parent, int32_t x, int32_t y, int32_t w, int32_t h) :
    PicoWin(
      parent->_rect._x1 + x,
      parent->_rect._y1 + y,
      parent->_rect._x1 + x + w,
      parent->_rect._y1 + y + h
    ),
    _parent(parent),
    _nextChild(0),
    _show(true)
  {
    if (_parent->_firstChild) _parent->_firstChild->_nextChild = this;
    else _parent->_firstChild = this;
  }

  inline int32_t x() { return _rect._x1 - _parent->_rect._x1; };
  inline int32_t y() { return _rect._y1 - _parent->_rect._y1; };
  

  void printAt(unsigned int x, unsigned int y, const char *str);
  void printAtF(unsigned int x, unsigned int y, const char *fmt, ...);
};

