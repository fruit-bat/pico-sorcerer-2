#include "PicoSelectFile.h"
#include "PicoOptionText.h"

PicoSelectFile::PicoSelectFile(SdCardFatFsSpi* sdCard, const char *folder, int32_t x, int32_t y, int32_t w, int32_t h, bool multiple) :
  PicoSelect(x, y, w, h, 1),
  _dir(sdCard, folder)
{
  
}

void PicoSelectFile::reload() {
  deleteOptions();
  _dir.foreach([=](const char *fn) { 
    addOption(new PicoOptionText(fn));
  });
}
