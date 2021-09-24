#include "Sorcerer2HidKeyboard.h"
#include "stdlib.h"

struct Sorcerer2HidKey {
  unsigned char keycode;
  unsigned char line;
  unsigned char key;
};

Sorcerer2HidKey KEYS[] = {
  { HID_KEY_BACKSPACE, 11, 0 },
  { HID_KEY_TAB, 1, 3 },
  { HID_KEY_ENTER, 11, 1},
  { HID_KEY_SHIFT_LEFT, 0, 4},
  { HID_KEY_SHIFT_RIGHT, 0, 4},
  { HID_KEY_CONTROL_LEFT, 0, 2},
  { HID_KEY_CONTROL_RIGHT, 0, 2},
  { HID_KEY_CAPS_LOCK, 0, 3},
  { HID_KEY_ESCAPE, 0, 0},
  { HID_KEY_SPACE, 1, 2},
  { HID_KEY_STOP, 1, 4}, // RUNSTOP
  { HID_KEY_F9, 1, 4}, // RUNSTOP
  { HID_KEY_DELETE, 15, 0},
  { HID_KEY_0, 9, 4},
  { HID_KEY_1, 2, 4},
  { HID_KEY_2, 3, 4},
  { HID_KEY_3, 4, 4},
  { HID_KEY_4, 4, 3},
  { HID_KEY_5, 5, 4},
  { HID_KEY_6, 6, 4},
  { HID_KEY_7, 7, 4},
  { HID_KEY_8, 8, 4},
  { HID_KEY_9, 8, 3},
  { HID_KEY_SEMICOLON, 9, 2},
  { HID_KEY_A, 2, 2},
  { HID_KEY_B, 5, 0},
  { HID_KEY_C, 3, 0},
  { HID_KEY_D, 3, 1},
  { HID_KEY_E, 4, 2},
  { HID_KEY_F, 4, 0},
  { HID_KEY_G, 5, 2},
  { HID_KEY_H, 6, 2},
  { HID_KEY_I, 7, 1},
  { HID_KEY_J, 7, 2},
  { HID_KEY_K, 7, 0},
  { HID_KEY_L, 8, 1},
  { HID_KEY_M, 6, 0},
  { HID_KEY_N, 6, 1},
  { HID_KEY_O, 8, 2},
  { HID_KEY_P, 9, 3},
  { HID_KEY_Q, 2, 3},
  { HID_KEY_R, 4, 1},
  { HID_KEY_S, 3, 2},
  { HID_KEY_T, 5, 3},
  { HID_KEY_U, 7, 3},
  { HID_KEY_V, 5, 1},
  { HID_KEY_W, 3, 3},
  { HID_KEY_X, 2, 0},
  { HID_KEY_Y, 6, 3},
  { HID_KEY_Z, 2, 1},
  { HID_KEY_ALT_RIGHT, 0, 1},
  { HID_KEY_CLEAR, 1, 0}, // ??
  { HID_KEY_F8, 1, 0}, // ??
  { HID_KEY_F5, 1, 1}, // REPEAT
  { HID_KEY_GRAVE, 11, 3}, // ^
  { HID_KEY_MINUS, 11, 4},
  { HID_KEY_COMMA, 8, 0},
  { HID_KEY_PERIOD, 9, 1},
  { HID_KEY_SLASH, 9, 0},
  { HID_KEY_BRACKET_LEFT, 10, 3}, // [
  { HID_KEY_BRACKET_RIGHT, 10, 2}, // ]
  { HID_KEY_BACKSLASH, 10, 0},
  { HID_KEY_EUROPE_2, 10, 0}, // backslash
  { HID_KEY_APOSTROPHE, 10, 1}, // clear/run stop?
  { HID_KEY_EUROPE_1, 10, 4}, // # -> :
  { HID_KEY_KEYPAD_0, 13, 0},
  { HID_KEY_KEYPAD_1, 13, 1},
  { HID_KEY_KEYPAD_2, 14, 1},
  { HID_KEY_KEYPAD_3, 15, 4},
  { HID_KEY_KEYPAD_4, 13, 2},
  { HID_KEY_KEYPAD_5, 14, 2},
  { HID_KEY_KEYPAD_6, 14, 3},
  { HID_KEY_KEYPAD_7, 13, 4},
  { HID_KEY_KEYPAD_8, 13, 3},
  { HID_KEY_KEYPAD_9, 14, 4},
  { HID_KEY_KEYPAD_MULTIPLY, 12, 1},
  { HID_KEY_KEYPAD_ADD, 12, 0},
  { HID_KEY_KEYPAD_SUBTRACT, 12, 3},
  { HID_KEY_KEYPAD_DIVIDE, 12, 2},
  { HID_KEY_F7, 15, 2}, // Unknown
  { HID_KEY_KEYPAD_DECIMAL, 14, 0},
  { HID_KEY_F6, 15, 1}, // Unknown
  { HID_KEY_KEYPAD_ENTER, 11, 2}, // line feed
  { HID_KEY_NUM_LOCK, 15, 3} // =
};

static bool KEYS_SORTED  = false;

static int keys_comparator(const Sorcerer2HidKey *k1, const Sorcerer2HidKey *k2) {
  int kk1 = (int)k1->keycode;
  int kk2 = (int)k2->keycode;
  return kk2 - kk1;
}

static int keys_v_comparator(const void *k1, const void *k2) {
  return keys_comparator((Sorcerer2HidKey *)k1, (Sorcerer2HidKey *)k2);
}

const static unsigned int KEYS_LEN = sizeof(KEYS) / sizeof(struct Sorcerer2HidKey);

const static unsigned int KEY_SIZE = sizeof(struct Sorcerer2HidKey);

static void sort_keys() {
  if (!KEYS_SORTED) {
    qsort(KEYS, KEYS_LEN, KEY_SIZE, keys_v_comparator);
  }
  KEYS_SORTED = true;
}

static Sorcerer2HidKey* findKey(const unsigned char keycode) {
  if (keycode <= 1) return 0;
  const Sorcerer2HidKey k0 = {keycode, 0, 0};
  return (Sorcerer2HidKey *) bsearch(&k0, KEYS, KEYS_LEN, KEY_SIZE, keys_v_comparator);
}

static bool isInReport(hid_keyboard_report_t const *report, const unsigned char keycode) {
  for(unsigned int i = 0; i < 6; i++) {
    if (report->keycode[i] == keycode)  return true;
  }
  return false;
}

Sorcerer2HidKeyboard::Sorcerer2HidKeyboard() {
  sort_keys();
}

static unsigned char LOCK_KEYS[] = {
  HID_KEY_CAPS_LOCK
//  HID_KEY_NUM_LOCK
};

void Sorcerer2HidKeyboard::processHidReport(hid_keyboard_report_t const *report) {
  static hid_keyboard_report_t prev = { 0, 0, {0} };
  static bool lock_flags[sizeof(LOCK_KEYS)] = {false};
  reset();
  if (report->keycode[0] == 1) return;
  const unsigned char m = report->modifier;
  for (int i = 0; i < 8; ++i) {
    if (m & (1 << i)) {
      const Sorcerer2HidKey *k = findKey(0xE0 | i);
      if (k) press(k->line, k->key);
    }
  }
  
  bool reset1 = false;
  bool reset2 = false;
    
  for(unsigned int i = 0; i < 6; ++i) {
    const unsigned char hidKeyCode = report->keycode[i];
    
    // F11 & F12 both down together for reset
    bool checkReset = false;
    reset1 |= hidKeyCode == HID_KEY_F11;
    reset2 |= hidKeyCode == HID_KEY_F12;
    checkReset |= reset1 && !isInReport(&prev, HID_KEY_F11);
    checkReset |= reset2 && !isInReport(&prev, HID_KEY_F12);
    
    if (reset1 && reset2 && checkReset) {
      _sorcerer2->reset();
    }
    
    // F2 Copy lower mem
    if (hidKeyCode == HID_KEY_F2 && !isInReport(&prev, HID_KEY_F2)) {
      _sorcerer2->saveMem();
    }
    
    // F3 Restore lower mem
    if (hidKeyCode == HID_KEY_F3 && !isInReport(&prev, HID_KEY_F3)) {
      _sorcerer2->loadMem();
    }
    
    // F4 toggle moderate
    if (hidKeyCode == HID_KEY_F4 && !isInReport(&prev, HID_KEY_F4)) {
      _sorcerer2->toggleModerate();
    }
        
    const Sorcerer2HidKey *k = findKey(hidKeyCode);
    if (k) {
      const unsigned char keycode = k->keycode;
      bool isLockKey = false;
      for (unsigned int j = 0; j < sizeof(LOCK_KEYS); ++j) {
        if (keycode == LOCK_KEYS[j] && !isInReport(&prev, keycode)) {
          lock_flags[j] = !lock_flags[j];
          isLockKey = true;
        }
      }
      if (!isLockKey) {
        press(k->line, k->key);
      }
    }
  }
  for (unsigned int j = 0; j < sizeof(LOCK_KEYS); ++j) {
    if (lock_flags[j]) {
      const Sorcerer2HidKey *k = findKey(LOCK_KEYS[j]);
      if (k) press(k->line, k->key);
    }
  }
  prev = *report;
}
