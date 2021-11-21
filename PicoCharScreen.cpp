#include "PicoCharScreen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

PicoCharScreen::PicoCharScreen(uint8_t *chars) :
  _chars(chars)
{
  for (int i = 0; i < PCS_CHARS; ++i) _chars[i] = (i % (256 -32)) + 32;
}


void PicoCharScreen::printAt(unsigned int x, unsigned int y, const char *s) {
  const unsigned int m = 64 * 30;
  const unsigned int p = x + (y<<6);
  if (p >= m) return;
  const unsigned int l = strlen(s);
  for(unsigned int i=0; i<l; ++i) {
    ((uint8_t *)&_chars)[(p + i) % m] = s[i];
  }
}

void PicoCharScreen::printAtF(unsigned int x, unsigned int y, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, args );
  printAt(x, y, buf);
  va_end(args);
}
