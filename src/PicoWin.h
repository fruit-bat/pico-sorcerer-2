#pragma once

#include "PicoRectXYWH.h"

class PicoPen;

class PicoWin {
  
  PicoWin *_parent;
  PicoRectXYWH _rect;
  PicoWin *_firstChild;
  PicoWin *_nextChild;
  bool _repaint;
  bool _repaintChild;
  
  void paintSubTree(PicoPen *pen);

public:

  PicoWin(int32_t x, int32_t y, int32_t w, int32_t h);

  void addChild(PicoWin *child);

  void repaint();

  void refresh(PicoPen *parentPen);

  void keyPressed(uint8_t keyCode);

  virtual void clear(PicoPen *pen);
  
  virtual void paint(PicoPen *pen) {}

  virtual bool handleKeyPressed(uint8_t keyCode) { return true; }
  
  int32_t wx() { return _rect._x; }
  int32_t wy() { return _rect._y; }
  int32_t ww() { return _rect._w; }
  int32_t wh() { return _rect._h; }

};
