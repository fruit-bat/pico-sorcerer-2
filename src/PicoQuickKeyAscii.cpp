#include "PicoQuickKeyAscii.h"
#include <ctype.h>

PicoQuickKeyAscii::PicoQuickKeyAscii(char quickKey) :  _quickKey(toupper(quickKey))
{
}

bool PicoQuickKeyAscii::isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  printf("comparing %d to %d (%d)\n", _quickKey, toupper(ascii), ascii);
  return toupper(ascii) == _quickKey;
}

