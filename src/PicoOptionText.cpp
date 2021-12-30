#include "PicoOptionText.h"

PicoOptionText::PicoOptionText(const char *text) : _text(text)
{
  onPaint([=](PicoPen *pen) {
    const char *s = _text.c_str();
    for (int32_t x = 0; x < pen->cw(); ++x) {
      pen->set(x, 0, *s ? *s++ : ' ');
    }
  });
}

PicoOptionText::~PicoOptionText() {
}
