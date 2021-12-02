#include "PicoRectXYXY.h"
#include "PicoRectXYWH.h"

void PicoRectXYXY::intersect(PicoRectXYWH *c) {
  const int32_t cx2 = c->_x + c->_w;
  const int32_t cy2 = c->_y + c->_h;
  if (c->_x > _x1) _x1 = c->_x;
  if (c->_y > _y1) _y1 = c->_y;
  if (cx2 < _x2) _x2 = cx2;
  if (cy2 < _y2) _y1 = cy2;
}

PicoRectXYXY::PicoRectXYXY(PicoRectXYWH *c) :
  _x1(c->_x), _y1(c->_y), _x2(c->_x + c->_w), _y2(c->_y + c->_h)
{
}
