#include "PicoSelect.h"
#include "PicoPen.h"
#include <pico/printf.h>

PicoSelect::PicoSelect(int32_t x, int32_t y, int32_t w, int32_t h) :
  PicoWin(x,y,w,h),
  _i(0),
  _quickKeys(false)
{
}

void PicoSelect::addOption(PicoOption *option) {
  _options.push_back(option);
  repaint();
}

void PicoSelect::clearOptions() {
  _options.clear();
  repaint();
}

void PicoSelect::deleteOptions() {
  for (auto option : _options) {
    delete option;
  }
  clearOptions();
}

void PicoSelect::paint(PicoPen *pen) {
  const int32_t s0 = _i - (wh() >> 1);  
  const int32_t s1 = optionCount() - wh();
  const int32_t s = s0 <= 0 || s1 <= 0 ? 0 : (s0 > s1 ? s1 : s0);
  printf("s0 = %ld, s = %ld\n", s0, s); 
  for(int32_t r = 0; r < wh(); ++r) {
    const int32_t i = r + s;
    PicoPen rpen(pen, 0, r, ww(), 1);
    if (!rpen.clipped()) {
      if (i < optionCount()) {
        paintRow(&rpen, r + s == _i, i);
      }
      else {
        rpen.clear();
      }
    }
  }
}

void PicoSelect::paintRow(PicoPen *pen, bool focused, int32_t i) {
  _options[i]->paintRow(pen, focused, i, ww());
}

void PicoSelect::toggleSelection(PicoOption *option) {
  option->toggle();
  repaint();
}

void PicoSelect::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
   printf("Select key pressed %d %d %d %c\n", keycode, modifiers, ascii, ascii);
  // up    82 
  // down  81
  if (_quickKeys) {
    for (int i = 0; i < optionCount(); ++ i) {
      PicoOption * option = _options[i];
      if (option->isQuickKey(keycode, modifiers, ascii)) {
        printf("Toggling %d %d %d %c\n", keycode, modifiers, ascii, ascii);
        _i = i;
        toggleSelection(option);
      }
    }
  }
  
  switch(ascii) {
    case 32: case 13: {
      if (_i >= 0 && _i < optionCount()) {
        toggleSelection(_options[_i]);
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
      if (_i + 1 < optionCount()) {
        ++_i;
        repaint();
      }
      break;
    }
  }
}
