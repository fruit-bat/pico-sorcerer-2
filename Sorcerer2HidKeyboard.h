#pragma once

#include "Sorcerer2Keyboard.h"
#include "Sorcerer2.h"
#include "tusb.h"

class Sorcerer2HidKeyboard : public Sorcerer2Keyboard {
  Sorcerer2 *_sorcerer2;  
public:
  Sorcerer2HidKeyboard();
  void processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_key_report);
  void setSorcerer2(Sorcerer2 *sorcerer2) { _sorcerer2 = sorcerer2; }
};
