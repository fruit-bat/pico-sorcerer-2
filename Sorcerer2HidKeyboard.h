#pragma once

#include "Sorcerer2Keyboard.h"
#include "tusb.h"

class Sorcerer2HidKeyboard : public Sorcerer2Keyboard {
public:
  Sorcerer2HidKeyboard();
  void processHidReport(hid_keyboard_report_t const *report);
};
