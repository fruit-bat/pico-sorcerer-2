#include "PicoWin.h"
#include "PicoPen.h"

PicoWin::PicoWin(int32_t x, int32_t y, int32_t w, int32_t h) :
  _parent(0),
  _firstChild(0),
  _nextChild(0),
  _focus(0),
  _rect(x, y, w, h),
  _repaint(false),
  _repaintChild(false)
{
  _clear = [=](PicoPen *pen){
    pen->clear();
  };
}

void PicoWin::repaint() {
  _repaint = true;
  for(PicoWin *win = _parent; win; win = win->_parent) win->_repaintChild = true;
}

void PicoWin::focus() {
  if (_parent) {
    _parent->_focus = this;
    _parent->focus();
  }
}

void PicoWin::addChild(PicoWin *child, bool focus) {
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
  if (focus) child->focus();
  child->repaint();
}

void PicoWin::removeChild(PicoWin *child) {
  
  if (!child || child->_parent != this) return;
  
  child->_parent = 0;
  
  if (child == _focus) {
    _focus = 0;
  }
  
  if (child == _firstChild) {
    _firstChild = child->_nextChild;
    child->_nextChild  = 0;
  }
  else {
    PicoWin *c = _firstChild;
    if (c) {
      while(c->_nextChild) {
        if (c->_nextChild == child) {
          c->_nextChild = child->_nextChild;
          child->_nextChild  = 0;
          break;
        }
        c = c->_nextChild;
      }
    }
  }
}

void PicoWin::paint(PicoPen *pen) {
  if (_paint) _paint(pen);
}

void PicoWin::refresh(PicoPen *parentPen) {
  PicoPen pen(parentPen, &_rect);
  if (_repaint) {
    _clear(&pen);
    paintSubTree(&pen);
  }
  else if (_repaintChild) {
    _repaintChild = false;
    for(PicoWin *win = _firstChild; win; win = win->_nextChild) win->refresh(&pen);
  }
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

void PicoWin::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  if (_focus) {
    _focus->keyPressed(keycode, modifiers, ascii);
  }
  else {
    if (_onkeydown) _onkeydown(keycode, modifiers, ascii);
  }
}
