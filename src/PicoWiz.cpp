#include "PicoWiz.h"
#include "PicoPen.h"

PicoWiz::PicoWiz(int32_t x, int32_t y, int32_t w, int32_t bh) :
  PicoWin(x, y, w, bh + 3),
  _title(0, 0, w, 3),
  _body(0, 3, w, bh)
{
  _title.onPaint([=](PicoPen *pen) {
    if (_titles.size()) {
      pen->printAt(0, 0, false, _titles.back().c_str());
    }
  });
  addChild(&_title, false);
  addChild(&_body, false);
}

void PicoWiz::push(PicoWin *stage, const char *title, bool focus){
  if (_stages.size()) {
    _body.removeChild(_stages.back());
  }
  _stages.push_back(stage);
  _titles.push_back(title);
  _body.addChild(stage, focus);
  repaint();
}

void PicoWiz::pop(bool focus){
  if (_stages.size() > 1) {
    _body.removeChild(_stages.back());
    _stages.pop_back();
    _titles.pop_back();
    _body.addChild(_stages.back(), focus);
    repaint();    
  }
}
