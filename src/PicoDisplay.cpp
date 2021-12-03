#include "PicoDisplay.h"
#include "PicoPen.h"

PicoDisplay::PicoDisplay(
  PicoCharScreen *screen,
  PicoWin *root) :
  _screen(screen),
  _root(root),
  _focus(root)
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

void PicoDisplay::keyPressed(uint8_t keyCode) {
  if (_focus) {
    _focus->keyPressed(keyCode);
  }
}

