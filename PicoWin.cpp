#include "PicoWin.h"


void PicoWin::printAt(unsigned int x, unsigned int y, const char *s) {

}

void PicoWin::printAtF(unsigned int x, unsigned int y, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, args );
  printAt(x, y, buf);
  va_end(args);
}
