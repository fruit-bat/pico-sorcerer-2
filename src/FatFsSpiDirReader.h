#pragma once

#include "SdCardFatFsSpi.h"
#include <string>
#include <functional>

class FatFsSpiDirReader {
private:
  SdCardFatFsSpi* _sdCard;
  std::string _folder;
public:
  FatFsSpiDirReader(SdCardFatFsSpi* sdCard, const char *folder);
  void foreach(std::function <void(const FILINFO* info)> cb);
  void foreach(std::function <void(const char* name)> cb);
};
