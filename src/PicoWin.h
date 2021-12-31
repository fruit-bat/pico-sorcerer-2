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
  std::function<void(PicoPen *pen)> _paint;
  std::function<void(PicoPen *pen)> _clear;

  bool bubbleKeyPress(uint8_t keycode, uint8_t modifiers, uint8_t ascii);

public:

  PicoWin(int32_t x, int32_t y, int32_t w, int32_t h);
  virtual ~PicoWin() {}

  void addChild(PicoWin *child, bool focus);
  void removeChild(PicoWin *child);
  void focus();
  void repaint();
  void paint(PicoPen *pen);
  void clear(PicoPen *pen);
  void refresh(PicoPen *parentPen);
  bool keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
 
  int32_t wx() { return _rect._x; }
  int32_t wy() { return _rect._y; }
  int32_t ww() { return _rect._w; }
  int32_t wh() { return _rect._h; }
  
  void onPaint(std::function<void(PicoPen *pen)> paint) { _paint = paint; }
  void onClear(std::function<void(PicoPen *pen)> clear) { _clear = clear; }
  void onKeydown(std::function<bool(uint8_t keycode, uint8_t modifiers, uint8_t ascii)> onkeydown) { _onkeydown = onkeydown; };
};
