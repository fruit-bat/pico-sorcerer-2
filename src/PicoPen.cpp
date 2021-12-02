#include "PicoPen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void PicoPen::printAt(int32_t x, int32_t y, bool wrap, uint8_t *str) {
  if (_screen->_clip.empty()) return;
  int32_t xc = x, yc = y;
  if (wrap) {
    while(xc >= _rect._w) {
      xc -= _rect._w;
      yc++;
    }
  }
  else {
    if (!_screen->_clip.containsRow(y)) return;
  }
  while(*str) {
    set(xc++, yc, *str++);
    if (xc == _rect._w) {
      if (wrap) { yc++; xc = 0;; }
    }
  }
}

void PicoPen::printAtF(int32_t x, int32_t y, bool wrap, uint8_t *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  uint8_t buf[128];
  vsnprintf((char*)buf, 128, (char*)fmt, args );
  printAt(x, y, wrap, buf);
  va_end(args);
}
