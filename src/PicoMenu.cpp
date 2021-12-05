#include "PicoMenu.h"
#include "PicoPen.h"

void PicoMenu::paint(PicoPen *pen) {
  for(uint32_t i = 0; i < _optionCount; ++i) {
   pen->setAttr(i & 3);
   pen->printAtF(0, i, false, "%d  %s", i, _options[i]);
  }
}

