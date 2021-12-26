#pragma once

#include "PicoCharScreen.h"
#include "PicoClippedScreen.h"
#include "PicoWin.h"

class PicoDisplay {

  PicoCharScreen *_screen;
  PicoWin *_root;

public:

  PicoDisplay(
    PicoCharScreen *screen,
    PicoWin *root
  );
  
  void keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);

  void refresh();
};
