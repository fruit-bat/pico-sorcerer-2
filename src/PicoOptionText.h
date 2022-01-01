#pragma once

#include "PicoOption.h"
#include <string>

class PicoOptionText : public PicoOption {
private:
  std::string _text;
public:
  PicoOptionText(const char *text);
  virtual ~PicoOptionText();
  const char * text() { return _text.c_str(); }
};
