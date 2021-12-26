#include "PicoDisplay.h"
#include "PicoPen.h"

PicoDisplay::PicoDisplay(
  PicoCharScreen *screen,
  PicoWin *root) :
  _screen(screen),
  _root(root)
{
  _root->repaint();
}

void PicoDisplay::refresh() {
  PicoClippedScreen clippedScreen(_screen);
  PicoPen pen(&clippedScreen);
  if (_root) {
    _root->refresh(&pen);
  }
}

void PicoDisplay::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  if (_root) {
    _root->keyPressed(keycode, modifiers, ascii);
  }
}

