#include "PicoWinBlock.h"
#include "PicoPen.h"

void PicoWinBlock::paint(PicoPen *pen) {
  for (int x = 0; x < ww(); ++x) {
    for (int y = 0; y < wh(); ++y) {
      pen->set(x,y,_c);
    }
  }
}
