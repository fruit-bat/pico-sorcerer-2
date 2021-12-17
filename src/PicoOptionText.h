#pragma once

#include "PicoOption.h"
#include <string>

class PicoOptionText : public PicoOption {
private:
  std::string _text;
public:
  PicoOptionText(const char *text);
  virtual ~PicoOptionText();
  virtual void paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i, int32_t ww);
};
