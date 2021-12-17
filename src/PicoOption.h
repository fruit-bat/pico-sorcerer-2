#pragma once

#include "PicoPen.h"

class PicoOption {
private:

  bool _selected;

public:

  PicoOption() : _selected(false) {}
  
  virtual ~PicoOption() {}
  
  PicoOption(bool selected) : _selected(selected) {}

  virtual void paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i, int32_t ww) {}
  
  virtual void selected(bool s) { _selected = s; }
  
  bool selected() { return _selected; }
  
  bool toggleSelection();
};
