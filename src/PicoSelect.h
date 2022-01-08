#pragma once

#include "PicoWin.h"
#include "PicoOption.h"
#include <vector>

class PicoSelect : public PicoWin {

private:
  std::vector<PicoOption *> _options;
  std::function<void(PicoOption *option)> _toggle;

  int32_t _i, _r, _rh;
  bool _quickKeys;

  void paintRow(PicoPen *pen, bool focused, int32_t i);
  int32_t optionCount() { return (int32_t)_options.size(); }
  void toggleSelection(PicoOption *option);
  
public:

  PicoSelect(int32_t x, int32_t y, int32_t w, int32_t r, int32_t rh);

  void addOption(PicoOption *option);
  void clearOptions();
  void deleteOptions();
  void enableQuickKeys() { _quickKeys = true; }
  void onToggle(std::function<void(PicoOption *option)> toggle) { _toggle = toggle; }
  void focus(int32_t i);
};
