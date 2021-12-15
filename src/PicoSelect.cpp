#include "PicoSelect.h"
#include "PicoPen.h"
#include <pico/printf.h>

void PicoSelect::paint(PicoPen *pen) {
  int32_t s0 = _i - (wh() >> 1);
  int32_t s = s0 < 0 ? 0 : s0 > _optionCount - wh() ? _optionCount - wh() : s0;
  for(int32_t r = 0; r < wh(); ++r) {
    pen->setAttr(r + s == _i ? 1 : 0);
    pen->printAt(0, r, false, _options[r + s]);
  }
}

void PicoSelect::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
   printf("Select key pressed %d %d %d %c\n", keycode, modifiers, ascii, ascii);
   
  switch(ascii) {
     case 'q': case 'Q': {
       if (_i > 0) { 
         --_i;
         repaint();
       }
       break;
     }
     case 'a': case 'A': {
       if (_i + 1 < _optionCount) {
         ++_i;
         repaint();
       }
       break;
     }
  }
}
