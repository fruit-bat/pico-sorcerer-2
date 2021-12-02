#include "PicoPen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void PicoPen::printAt(int32_t x, int32_t y, bool wrap, uint8_t *str) {
  if (!wrap && !_clip.containsRow(y)) return;
  int32_t xc = x, yc = y;
  while(*str) {
    set(xc++, yx, *str++);
    if (xc == _w) {
      if (wrap) { yc++; xc = 0;; }
    }
  }
}

void PicoPen::printAtF(int32_t x, int32_t y, bool wrap, uint8_t *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, args );
  printAt(x, y, buf);
  va_end(args);
}
