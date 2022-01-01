#pragma once
#include "tusb.h"
#include "PicoDisplay.h"

class PicoWinHidKeyboard {
private:
  PicoDisplay *_display;

  bool keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii);
public:
  PicoWinHidKeyboard(PicoDisplay *display);

  int processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report);
};
