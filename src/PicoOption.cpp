#include "PicoOption.h"

bool PicoOption::toggleSelection() {
  selected(!_selected);
  return _selected;
}

PicoOption* PicoOption::addQuickKey(PicoQuickKey *quickKey) {
  _quickKeys.push_back(quickKey);
  return this;
}

PicoOption::~PicoOption() {
}

bool PicoOption::isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  for (PicoQuickKey * quickKey : _quickKeys) {
   printf("Checking option pressed %d %d %d %c\n", keycode, modifiers, ascii, ascii);
    printf("quickKey %ld\n", quickKey);
    if (quickKey->isQuickKey(keycode, modifiers, ascii)) return true;
    printf("nope\n");
    
  }
  return false;
}

void PicoOption::selected(bool s) {
  _selected = s;
  if (_onselect) _onselect();
}
