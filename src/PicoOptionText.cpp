#include "PicoOptionText.h"

PicoOptionText::PicoOptionText(const char *text) : _text(text)
{
}

PicoOptionText::~PicoOptionText() {
}

void PicoOptionText::paintRow(PicoPen *pen, bool focused, int32_t i, int32_t ww) {
  pen->setAttr(selected() ? 1 : 0);
  const char *s = _text.c_str();
  
  if (_quickKeys.size()) {
    const char q = _quickKeys[0]->ascii();
    const char c = q > 32 ? q : '.';
    pen->printAtF(0, 0, false, " %c %c....", (focused ? '>' : ' '), c);
  }
  else {
    pen->printAtF(0, 0, false, " %c ", (focused ? '>' : ' '));
  }
  
  for (int32_t x = 7; x < ww; ++x) {
    pen->set(x, 0, *s ? *s++ : ' ');
  }
}
