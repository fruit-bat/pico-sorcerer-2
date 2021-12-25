#pragma once

#include "PicoPen.h"
#include "PicoQuickKey.h"
#include <vector>
#include <functional>

class PicoOption {
private:

  bool _selected;
  std::vector<PicoQuickKey *> _quickKeys;
  std::function<bool()> _onselect;

public:

  PicoOption() : _selected(false) {}
  
  PicoOption* addQuickKey(PicoQuickKey *quickKey);

  virtual ~PicoOption();

  PicoOption(bool selected) : _selected(selected) {}

  virtual void paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i, int32_t ww) {}

  void selected(bool s);

  bool selected() { return _selected; }

  bool toggleSelection();
  
  bool isQuickKey(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
};
