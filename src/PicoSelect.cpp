#include "PicoSelect.h"
#include "PicoPen.h"
#include <pico/printf.h>

void PicoSelect::paint(PicoPen *pen) {
  int32_t s0 = _i - (wh() >> 1);
  int32_t s = s0 < 0 ? 0 : s0 > _optionCount - wh() ? _optionCount - wh() : s0;
  for(int32_t r = 0; r < wh(); ++r) {
    const int32_t i = r + s;
    if (i < _optionCount) paintRow(pen, r + s == _i, r, i);
    else clearRow(pen, r);
  }
}

void PicoSelect::paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i) {
  _options[i]->paintRow(pen, focused, y, i, ww());
}

void PicoSelect::clearRow(PicoPen *pen, int32_t y) {
  pen->setAttr(0);
  for (int32_t x = 0; x < ww(); ++x) {
    pen->set(x, y, ' ');
  }
}

void PicoSelect::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
   printf("Select key pressed %d %d %d %c\n", keycode, modifiers, ascii, ascii);
  // up    82 
  // down  81
  
  switch(ascii) {
    case 32: case 13: {
      if (_i >= 0 && _i < _optionCount) {
        if (_options[_i]->toggleSelection() && !_multiple) {
          for(int32_t i = 0; i < _optionCount; ++i) {
            if (i != _i && _options[i]->selected()) _options[i]->selected(false);
          }
        }
        repaint();
      }
      break;
    }
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
