#include "PicoWinHidKeyboard.h"

static uint8_t const keycode2ascii[128][2] =  { 
    {0     , 0      }, /* 0x00 */ 
    {0     , 0      }, /* 0x01 */ 
    {0     , 0      }, /* 0x02 */ 
    {0     , 0      }, /* 0x03 */ 
    {'a'   , 'A'    }, /* 0x04 */ 
    {'b'   , 'B'    }, /* 0x05 */ 
    {'c'   , 'C'    }, /* 0x06 */ 
    {'d'   , 'D'    }, /* 0x07 */ 
    {'e'   , 'E'    }, /* 0x08 */ 
    {'f'   , 'F'    }, /* 0x09 */ 
    {'g'   , 'G'    }, /* 0x0a */ 
    {'h'   , 'H'    }, /* 0x0b */ 
    {'i'   , 'I'    }, /* 0x0c */ 
    {'j'   , 'J'    }, /* 0x0d */ 
    {'k'   , 'K'    }, /* 0x0e */ 
    {'l'   , 'L'    }, /* 0x0f */ 
    {'m'   , 'M'    }, /* 0x10 */ 
    {'n'   , 'N'    }, /* 0x11 */ 
    {'o'   , 'O'    }, /* 0x12 */ 
    {'p'   , 'P'    }, /* 0x13 */ 
    {'q'   , 'Q'    }, /* 0x14 */ 
    {'r'   , 'R'    }, /* 0x15 */ 
    {'s'   , 'S'    }, /* 0x16 */ 
    {'t'   , 'T'    }, /* 0x17 */ 
    {'u'   , 'U'    }, /* 0x18 */ 
    {'v'   , 'V'    }, /* 0x19 */ 
    {'w'   , 'W'    }, /* 0x1a */ 
    {'x'   , 'X'    }, /* 0x1b */ 
    {'y'   , 'Y'    }, /* 0x1c */ 
    {'z'   , 'Z'    }, /* 0x1d */ 
    {'1'   , '!'    }, /* 0x1e */ 
    {'2'   , '@'    }, /* 0x1f */ 
    {'3'   , '#'    }, /* 0x20 */ 
    {'4'   , '$'    }, /* 0x21 */ 
    {'5'   , '%'    }, /* 0x22 */ 
    {'6'   , '^'    }, /* 0x23 */ 
    {'7'   , '&'    }, /* 0x24 */ 
    {'8'   , '*'    }, /* 0x25 */ 
    {'9'   , '('    }, /* 0x26 */ 
    {'0'   , ')'    }, /* 0x27 */ 
    {'\r'  , '\r'   }, /* 0x28 */ 
    {'\x1b', '\x1b' }, /* 0x29 */ 
    {'\b'  , '\b'   }, /* 0x2a */ 
    {'\t'  , '\t'   }, /* 0x2b */ 
    {' '   , ' '    }, /* 0x2c */ 
    {'-'   , '_'    }, /* 0x2d */ 
    {'='   , '+'    }, /* 0x2e */ 
    {'['   , '{'    }, /* 0x2f */ 
    {']'   , '}'    }, /* 0x30 */ 
    {'\\'  , '|'    }, /* 0x31 */ 
    {'#'   , '~'    }, /* 0x32 */ 
    {';'   , ':'    }, /* 0x33 */ 
    {'\''  , '\"'   }, /* 0x34 */ 
    {'`'   , '~'    }, /* 0x35 */ 
    {','   , '<'    }, /* 0x36 */ 
    {'.'   , '>'    }, /* 0x37 */ 
    {'/'   , '?'    }, /* 0x38 */ 
                                  
    {0     , 0      }, /* 0x39 */ 
    {0     , 0      }, /* 0x3a */ 
    {0     , 0      }, /* 0x3b */ 
    {0     , 0      }, /* 0x3c */ 
    {0     , 0      }, /* 0x3d */ 
    {0     , 0      }, /* 0x3e */ 
    {0     , 0      }, /* 0x3f */ 
    {0     , 0      }, /* 0x40 */ 
    {0     , 0      }, /* 0x41 */ 
    {0     , 0      }, /* 0x42 */ 
    {0     , 0      }, /* 0x43 */ 
    {0     , 0      }, /* 0x44 */ 
    {0     , 0      }, /* 0x45 */ 
    {0     , 0      }, /* 0x46 */ 
    {0     , 0      }, /* 0x47 */ 
    {0     , 0      }, /* 0x48 */ 
    {0     , 0      }, /* 0x49 */ 
    {2     , 2      }, /* 0x4a Home */ 
    {0     , 0      }, /* 0x4b */ 
    {127   , 127    }, /* 0x4c Del */ 
    {3     , 3      }, /* 0x4d End */ 
    {0     , 0      }, /* 0x4e */ 
    {128   , 128    }, /* 0x4f Right */ 
    {129   , 129    }, /* 0x50 Left */ 
    {130   , 130    }, /* 0x51 Down */ 
    {131   , 131    }, /* 0x52 Up */ 
    {0     , 0      }, /* 0x53 */ 
                                  
    {'/'   , '/'    }, /* 0x54 */ 
    {'*'   , '*'    }, /* 0x55 */ 
    {'-'   , '-'    }, /* 0x56 */ 
    {'+'   , '+'    }, /* 0x57 */ 
    {'\r'  , '\r'   }, /* 0x58 */ 
    {'1'   , 0      }, /* 0x59 */ 
    {'2'   , 0      }, /* 0x5a */ 
    {'3'   , 0      }, /* 0x5b */ 
    {'4'   , 0      }, /* 0x5c */ 
    {'5'   , '5'    }, /* 0x5d */ 
    {'6'   , 0      }, /* 0x5e */ 
    {'7'   , 0      }, /* 0x5f */ 
    {'8'   , 0      }, /* 0x60 */ 
    {'9'   , 0      }, /* 0x61 */ 
    {'0'   , 0      }, /* 0x62 */ 
    {'0'   , 0      }, /* 0x63 */ 
    {'='   , '='    }, /* 0x67 */ 
};

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

bool PicoWinHidKeyboard::keyPressed(uint8_t keycode, uint8_t modifiers, uint8_t ascii) {
  return _display->keyPressed(keycode, modifiers, ascii);
}

int PicoWinHidKeyboard::processHidReport(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
  
  int r = 0;
  for(unsigned int i = 0; i < 6; ++i) {
    const unsigned char hidKeyCode = report->keycode[i];
    if (hidKeyCode) {
      const bool isInPrev = isInReport(prev_report, hidKeyCode);
      
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
          // TODO Turn CAPS light on/off
          if (hidKeyCode == HID_KEY_CAPS_LOCK) _capslock = !_capslock;
          
          // not existed in previous report means the current key is pressed
          bool const is_shift =  _capslock || (report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT));
          uint8_t ch = keycode2ascii[report->keycode[i]][is_shift ? 1 : 0];
          if (keyPressed(hidKeyCode, report->modifier, ch) && (ch == 27)) r = 1;
        }
      }
    }
  }

  return r;
}
