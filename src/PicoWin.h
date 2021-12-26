#pragma once

#include "PicoRectXYWH.h"
#include <functional>

class PicoPen;

class PicoWin {
  
  PicoWin *_parent;
  PicoWin *_firstChild;
  PicoWin *_nextChild;
  PicoWin *_focus;

  PicoRectXYWH _rect;

  bool _repaint;
  bool _repaintChild;
  
  void paintSubTree(PicoPen *pen);
  
  std::function<bool(uint8_t keycode, uint8_t modifiers, uint8_t ascii)> _onkeydown;
  std::function<bool()> _onclose;

public:

  PicoWin(int32_t x, int32_t y, int32_t w, int32_t h);

  void addChild(PicoWin *child, bool focus);
  void removeChild(PicoWin *child);
  void focus();

  void repaint();

  void refresh(PicoPen *parentPen);

  // TODO this should return a boolean and be passed to the display if not handled by a window
  // TODO should probably not be virtual
  virtual void keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);

  virtual void clear(PicoPen *pen);
  
  virtual void paint(PicoPen *pen) {}
 
  int32_t wx() { return _rect._x; }
  int32_t wy() { return _rect._y; }
  int32_t ww() { return _rect._w; }
  int32_t wh() { return _rect._h; }

};
