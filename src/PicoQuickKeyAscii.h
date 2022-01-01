#pragma once

#include "PicoQuickKey.h"

// TODO combine with PicoQuickKey?
class PicoQuickKeyAscii : public PicoQuickKey {
private:
  char _quickKey;
public:
  PicoQuickKeyAscii(char quickKey);
  virtual bool isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  virtual ~PicoQuickKeyAscii() {}
  virtual char ascii() { return _quickKey; }
};
