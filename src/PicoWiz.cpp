#include "PicoWiz.h"

PicoWiz::PicoWiz(int32_t x, int32_t y, int32_t w, int32_t h) :
  PicoWin(x, y, w, h) 
{
}

void PicoWiz::push(PicoWin *stage){
  if (_stages.size()) {
    removeChild(_stages.back());
  }
  _stages.push_back(stage);
  addChild(stage);
  
  // TODO give stage (or appropriate sub-element focus)
  
  repaint();
}

void PicoWiz::pop(){
  if (_stages.size() > 1) {
    removeChild(_stages.back());
    _stages.pop_back();
    addChild(_stages.back());
    
    // TODO give _stages.back() focus (or appropriate sub-element focus)

    repaint();    
  }
}
