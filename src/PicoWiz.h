#pragma once

#include "PicoWin.h"
#include <vector>

class PicoWiz :PicoWin {
private:
  std::vector<PicoWin *> _stages;

public:
  PicoWiz(int32_t x, int32_t y, int32_t w, int32_t h);
  void push(PicoWin *stage);
  void pop();
};
