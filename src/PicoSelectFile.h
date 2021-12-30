#pragma once

#include "PicoSelect.h"
#include "FatFsSpiDirReader.h"

class PicoSelectFile : public PicoSelect {
  FatFsSpiDirReader _dir;
public:
  PicoSelectFile(SdCardFatFsSpi* sdCard, const char *folder, int32_t x, int32_t y, int32_t w, int32_t h);
  void reload();
};
