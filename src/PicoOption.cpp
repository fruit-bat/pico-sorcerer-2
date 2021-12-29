#include "PicoOption.h"

PicoOption::PicoOption()
{
}

bool PicoOption::selected() { 
  return _selected && _selected();
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

void PicoOption::toggle() {
  if (_toggle) {
    printf("Executing selection listener...\n");
    _toggle();
  }
}
