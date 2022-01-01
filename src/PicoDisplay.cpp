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

bool PicoDisplay::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  return !_root || _root->keyPressed(keycode, modifiers, ascii);
}

