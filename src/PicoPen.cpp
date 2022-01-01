#include "PicoPen.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void PicoPen::printAt(int32_t x, int32_t y, bool wrap, const char *str) {
  if (_screen._clip.empty()) {
    printf("Empty clip\n");
    return;
  }
  int32_t xc = x, yc = y;
  if (wrap) {
    while(xc >= _rect._w) {
      xc -= _rect._w;
      yc++;
    }
  }
  while(*str) {
    if (*str == '\n') {
      xc = 0;
      yc++;
    }
    else {
      set(xc++, yc, *str++);
      if (xc == _rect._w) {
        if (wrap) { yc++; xc = 0; }
      }
    }
  }
}

void PicoPen::printAtF(int32_t x, int32_t y, bool wrap, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
      printf("Print buf %s\n", buf);

  vsnprintf(buf, 128, fmt, args );
  printAt(x, y, wrap, buf);
  va_end(args);
}
