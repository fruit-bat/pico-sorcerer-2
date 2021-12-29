#pragma once

#include "PicoPen.h"
#include "PicoQuickKey.h"
#include <vector>
#include <functional>

class PicoOption {
protected:

  std::vector<PicoQuickKey *> _quickKeys;
  std::function<bool()> _selected;
  std::function<void()> _toggle;

public:

  PicoOption();

  virtual ~PicoOption();
  
  PicoOption* addQuickKey(PicoQuickKey *quickKey);

  virtual void paintRow(PicoPen *pen, bool focused, int32_t i, int32_t ww) {}

  bool selected();

  void toggle();
  
  bool isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  
  void toggle(std::function<void()> toggle) { _toggle = toggle; }
  
  void selected(std::function<bool()> selected) { _selected = selected; }
};
