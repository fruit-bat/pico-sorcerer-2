#pragma once

#include "SdCardFatFsSpi.h"
#include <string>

class FatFsSpiDirReader {
  SdCardFatFsSpi* _sdCard;
  std::string _folder;
  void reload();
public:
  FatFsSpiDirReader(SdCardFatFsSpi* sdCard, const char *folder);
  ~FatFsSpiDirReader();
};
