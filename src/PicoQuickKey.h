#pragma once
#include "tusb.h"

class PicoQuickKey {
public:
  PicoQuickKey() {}
  virtual bool isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  virtual ~PicoQuickKey() {}
};
