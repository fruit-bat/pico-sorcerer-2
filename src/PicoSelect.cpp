#include "PicoSelect.h"
#include "PicoPen.h"
#include <pico/printf.h>

PicoSelect::PicoSelect(int32_t x, int32_t y, int32_t w, int32_t h, bool multiple) :
  PicoWin(x,y,w,h),
  _i(0),
  _multiple(multiple),
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
  int32_t s0 = _i - (wh() >> 1);
  int32_t s = s0 < 0 ? 0 : s0 > optionCount() - wh() ? optionCount() - wh() : s0;
  for(int32_t r = 0; r < wh(); ++r) {
    const int32_t i = r + s;
    if (i < optionCount()) paintRow(pen, r + s == _i, r, i);
    else clearRow(pen, r);
  }
}

void PicoSelect::paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i) {
  _options[i]->paintRow(pen, focused, y, i, ww());
}

void PicoSelect::clearRow(PicoPen *pen, int32_t y) {
  pen->setAttr(0);
  for (int32_t x = 0; x < ww(); ++x) pen->set(x, y, ' ');
}

void PicoSelect::toggleSelection(PicoOption *option) {
  if (option->toggleSelection() && !_multiple) {
    for(PicoOption *option2 : _options) {
      if (option != option2 && option2->selected()) option2->selected(false);
    }
  }
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
