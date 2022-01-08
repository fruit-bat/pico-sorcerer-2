#pragma once
#include "PicoWin.h"
#include <string.h>
#include <functional>

class PicoTextField : public PicoWin {
  std::string _text;
  int32_t _cursor;
  int32_t _start;
  int32_t _maxchars;
  std::function<void(const char *)> _onenter;

public:
  PicoTextField(int32_t x, int32_t y, int32_t w, int32_t maxchars);
  int32_t textlen() { return (int32_t)_text.size(); }
  void clear();
  void onenter(std::function<void(const char *)> onenter) { _onenter = onenter; }

};
