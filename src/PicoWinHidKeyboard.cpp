#include "PicoWinHidKeyboard.h"

static uint8_t const keycode2ascii[128][2] =  { HID_KEYCODE_TO_ASCII };

static bool isInReport(hid_keyboard_report_t const *report, const unsigned char keycode) {
  for(unsigned int i = 0; i < 6; i++) {
    if (report->keycode[i] == keycode)  return true;
  }
  return false;
}

PicoWinHidKeyboard::PicoWinHidKeyboard(PicoDisplay *display) :
  _display(display)
{
}

void PicoWinHidKeyboard::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  _display->keyPressed(keycode, modifiers, ascii);
}

int PicoWinHidKeyboard::processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
  
  int r = 0;
  for(unsigned int i = 0; i < 6; ++i) {
    const unsigned char hidKeyCode = report->keycode[i];
    if (hidKeyCode) {
      const bool isInPrev = isInReport(prev_report, HID_KEY_F1);
      
      // F1 Close menu
      if (hidKeyCode == HID_KEY_F1) {
        if (!isInPrev) r = 1;
      }
      else
      {
        if (isInPrev)
        {
          // exist in previous report means the current key is holding
        }
        else {
          // not existed in previous report means the current key is pressed
          bool const is_shift =  report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
          uint8_t ch = keycode2ascii[report->keycode[i]][is_shift ? 1 : 0];
          keyPressed(hidKeyCode, report->modifier, ch);
        }
      }
    }
  }

  return r;
}
