#pragma once

#include "PicoQuickKey.h"

class PicoQuickKeyAscii : public PicoQuickKey {
private:
  char _quickKey;
public:
  PicoQuickKeyAscii(char quickKey);
  virtual bool isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  virtual ~PicoQuickKeyAscii() {}
};
