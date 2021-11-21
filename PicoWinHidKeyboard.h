#pragma once
#include "PicoWin.h"
#include "tusb.h"

class PicoWinHidKeyboard {
public:
  int processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report);
};
