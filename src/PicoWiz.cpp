#include "PicoWiz.h"

PicoWiz::PicoWiz(int32_t x, int32_t y, int32_t w, int32_t h) :
  PicoWin(x, y, w, h) 
{
}

void PicoWiz::push(PicoWin *stage, bool focus){
  if (_stages.size()) {
    removeChild(_stages.back());
  }
  _stages.push_back(stage);
  addChild(stage, focus);
  repaint();
}

void PicoWiz::pop(bool focus){
  if (_stages.size() > 1) {
    removeChild(_stages.back());
    _stages.pop_back();
    addChild(_stages.back(), focus);
    repaint();    
  }
}
