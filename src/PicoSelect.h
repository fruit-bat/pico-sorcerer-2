#pragma once

#include "PicoWin.h"
#include "PicoOption.h"
#include <vector>

class PicoSelect : public PicoWin {

private:
  std::vector<PicoOption *> _options;

  int32_t _i, _r, _rh;
  bool _quickKeys;

  void paintRow(PicoPen *pen, bool focused, int32_t i);
  int32_t optionCount() { return (int32_t)_options.size(); }
  void toggleSelection(PicoOption *option);
  
public:

  PicoSelect(int32_t x, int32_t y, int32_t w, int32_t h);
  PicoSelect(int32_t x, int32_t y, int32_t w, int32_t r, int32_t rh);

  virtual void clear(PicoPen *pen) {};
  virtual void paint(PicoPen *pen);
  
  void addOption(PicoOption *option);

  void clearOptions();
  void deleteOptions();
  
  virtual void keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
  
  void enableQuickKeys() { _quickKeys = true; }
};
