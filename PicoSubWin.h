#pragma once
#include "PicoWin.h"

class PicoPen;

class PicoSubWin : public PicoWin {

protected:

  int32_t _x, _y;
  PicoWin *_parent;
  PicoSubWin *_nextChild;
  bool _repaint;
  bool _repaintChild;
  
public:

  PicoSubWin(PicoWin *parent, int32_t x, int32_t y, int32_t w, int32_t h) :
    PicoWin(w, h),
    _x(x), _y(y),
    _parent(parent),
    _nextChild(0)
  {
    if (_parent->_firstChild) _parent->_firstChild->_nextChild = this;
    else _parent->_firstChild = this;
  }

  inline int32_t rx() { return _x; }
  inline int32_t ry() { return _y; }
    
  void repaint() {
    _repaint = true;
    // TODO PicoWin does not have a parent!
    for(PicoWin *win = _parent; win != 0; win = win->_parent) win->_repaintChild = true;
  }

  void refresh(PicoPen *parentPen);

  void paintSubTree(PicoPen *pen);
  
  virtual void paint(PicoPen *pen) {}
};
