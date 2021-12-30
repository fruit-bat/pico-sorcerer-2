#pragma once

#include "PicoWin.h"
#include <vector>
#include <string>

class PicoWiz : public PicoWin {
private:
  std::vector<PicoWin *> _stages;  
  std::vector<std::string> _titles;
  PicoWin _title;
  PicoWin _body;
public:
  PicoWiz(int32_t x, int32_t y, int32_t w, int32_t bh);
  void push(PicoWin *stage, const char *title, bool focus);
  void pop(bool focus);
};
