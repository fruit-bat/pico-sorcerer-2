#include "PicoOptionText.h"

PicoOptionText::PicoOptionText(const char *text) : _text(text)
{
}

PicoOptionText::~PicoOptionText() {
}

void PicoOptionText::paintRow(PicoPen *pen, bool focused, int32_t y, int32_t i, int32_t ww) {
  pen->setAttr((focused ? 2 : 0) | (selected() ? 1 : 0));
  const char *s = _text.c_str();
  for (int32_t x = 0; x < ww; ++x) {
    pen->set(x, y, *s ? *s++ : ' ');
  }
}
