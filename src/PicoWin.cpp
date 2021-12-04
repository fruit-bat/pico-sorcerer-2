#include "PicoWin.h"
#include "PicoPen.h"

PicoWin::PicoWin(int32_t x, int32_t y, int32_t w, int32_t h) :
  _parent(0),
  _rect(x, y, w, h),
  _firstChild(0),
  _nextChild(0),
  _repaint(false),
  _repaintChild(false)
{
}

void PicoWin::repaint() {
  _repaint = true;
  for(PicoWin *win = _parent; win; win = win->_parent) win->_repaintChild = true;
}

void PicoWin::addChild(PicoWin *child) {
  PicoWin *c = _firstChild;
  child->_nextChild = 0;
  child->_parent = this;
  if (c) {
    while(c->_nextChild) c = c->_nextChild;
    c->_nextChild = child;
  }
  else {
    _firstChild = child;
  }
  child->repaint();
}

void PicoWin::refresh(PicoPen *parentPen) {
  PicoPen pen(parentPen, &_rect);
  if (_repaint) {
    clear(&pen);
    paintSubTree(&pen);
  }
  else if (_repaintChild) {
    _repaintChild = false;
    for(PicoWin *win = _firstChild; win; win = win->_nextChild) win->refresh(&pen);
  }
}

void PicoWin::clear(PicoPen *pen) {
  pen->clear();
}

void PicoWin::paintSubTree(PicoPen *pen) {
  _repaint = false;
  _repaintChild = false;
  paint(pen);
  for(PicoWin *win = _firstChild; win; win = win->_nextChild) {
    PicoPen pen2(pen, &(win->_rect));
    win->paintSubTree(&pen2);
  }
}

void PicoWin::keyPressed(uint8_t keyCode) {
  if (handleKeyPressed(keyCode) && _parent) _parent->keyPressed(keyCode); 
}
