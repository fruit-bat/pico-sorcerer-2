#pragma once

#include "sd_card.h"

class Sorcerer2SdCardFatFsSpi {
  int _unit;
  sd_card_t *_sdcard;
public:
  Sorcerer2SdCardFatFsSpi(int unit);
  bool mount();
  void unmount();
  bool mounted() { return !!_sdcard; }
};
