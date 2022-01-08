#include "PicoTextField.h"
#include "PicoPen.h"

PicoTextField::PicoTextField(int32_t x, int32_t y, int32_t w, int32_t maxchars) :
  PicoWin(x, y, w, 1),
  _maxchars(maxchars)
{
  onKeydown([=](uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
    printf("Text field key pressed %d %d %d %c\n", keycode, modifiers, ascii, ascii);
    
    switch(ascii) {
      case 2: { // Home
        if (_cursor > 0) {
          _cursor = 0;
          _start = _cursor;
          repaint();
        }
        return false;
      }
      case 3: { // End
        if (_cursor < textlen()) {
          _cursor = textlen();
          const int32_t e = (_cursor - _start) - ww();
          if (e > 0) {
            _start += e;
          }
          repaint();
        }
        return false;
      }
      case 129: { // left
        if (_cursor > 0) {
          _cursor--;
          if (_cursor < _start) {
            _start = _cursor;
          }
          repaint();
        }
        return false;
      }
      case 128: { // right
        if (_cursor < textlen()) {
          _cursor++;
          const int32_t e = (_cursor - _start) - ww();
          if (e > 0) {
            _start += e;
          }
          repaint();
        }
        return false;
      }
      case '\r': { // return or enter
        if (_onenter) _onenter(_text.c_str());
        return true;
      }
      case 127: { // del
        if (_cursor < textlen()) {
          _text.erase(_cursor, 1);
          repaint();
        }
        return false;
      }
      default: {
        if (ascii >= 32 && ascii < 127 && textlen() < _maxchars) {
          _text.insert(_cursor, 1, ascii);
          _cursor++;
          repaint();
          return false;
        }
      }
    };
    
    return true;
  });
  
  onPaint([=](PicoPen *pen) {
    // Write the entire text field
    for(int32_t i = 0; i < ww(); ++i) {
      const int32_t j = i + _start;
      pen->setAttrFlash(j == _cursor);
      pen->set(i, 0, j < textlen() ? _text[j] : ' ');
    }
  });
  
  // Don't flicker
  onClear([=](PicoPen *pen) {});  
}

void PicoTextField::clear() {
  _text.clear();
  _cursor = 0;
  _start = 0;
}
