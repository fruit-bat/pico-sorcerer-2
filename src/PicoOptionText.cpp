#include "PicoOptionText.h"
#include <string.h>

PicoOptionText::PicoOptionText(const char *text) {
  _text = new char[strlen(text)+1]();
  strcpy(_text, text);  
}

PicoOptionText::~PicoOptionText() {
  delete _text;
}

void PicoOptionText::paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i, int32_t ww) {
  pen->setAttr((focused ? 1 : 0) | (selected() ? 2 : 0));
  const char *s = _text;
  for (int32_t x = 0; x < ww; ++x) {
    pen->set(x, y, *s ? *s++ : ' ');
  }
}
