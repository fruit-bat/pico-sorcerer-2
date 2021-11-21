#include "PicoWinHidKeyboard.h"

static bool isInReport(hid_keyboard_report_t const *report, const unsigned char keycode) {
  for(unsigned int i = 0; i < 6; i++) {
    if (report->keycode[i] == keycode)  return true;
  }
  return false;
}

int PicoWinHidKeyboard::processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
  
  int r = 0;
  for(unsigned int i = 0; i < 6; ++i) {
    const unsigned char hidKeyCode = report->keycode[i];
    
    // F1 Close menu
    if (hidKeyCode == HID_KEY_F1 && !isInReport(prev_report, HID_KEY_F1)) {
      r = 1;
    }

  }

  return r;
}
