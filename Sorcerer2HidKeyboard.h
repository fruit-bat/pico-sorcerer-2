#pragma once

#include "Sorcerer2Keyboard.h"
#include "Sorcerer2.h"
#include "tusb.h"

class Sorcerer2HidKeyboard : public Sorcerer2Keyboard {
  Sorcerer2 *_sorcerer2;  
  bool _reset1;
  bool _reset2;  
public:
  Sorcerer2HidKeyboard();
  void processHidReport(hid_keyboard_report_t const *report);
  void setSorcerer2(Sorcerer2 *sorcerer2) { _sorcerer2 = sorcerer2; }
  
  void setReset1(bool pressed) {
    _reset1 = pressed;
  }

  void setReset2(bool pressed) {
    _reset2 = pressed;
  }
  
  bool resetPressed() {
    return _reset1 && _reset2;
  }  
};
