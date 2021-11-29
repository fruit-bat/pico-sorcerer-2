#include "PicoSubWin.h"
#include "PicoPen.h"

void PicoSubWin::refresh(PicoPen *parentPen) {
  PicoPen pen(parentPen, this);
  if (_repaint) {
    pen->clear();
    paintSubTree(&pen);
  }
  else if (_repaintChild) {
    for(PicoWin *win = _firstChild; win != 0; win = win->_nextChild) win->refresh(&pen);
  }
}

void PicoSubWin::paintSubTree(PicoPen *pen) {
  paint(pen);
  for(PicoWin *win = _firstChild; win != 0; win = win->_nextChild) win->paint(&pen);
}
