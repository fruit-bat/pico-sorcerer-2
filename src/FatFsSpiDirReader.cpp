#include <pico/printf.h>
#include "FatFsSpiDirReader.h"

FatFsSpiDirReader::FatFsSpiDirReader(SdCardFatFsSpi* sdCard, const char *folder) :
  _sdCard(sdCard),
  _folder(folder)
{
}

FatFsSpiDirReader::~FatFsSpiDirReader() {
}

void FatFsSpiDirReader::reload() {
  if (!_sdCard->mounted()) {

    // TODO clear down existing list
    
    if (!_sdCard->mount()) {
      printf("Failed to mount SD card\n");
      return;
    }
  }
  
  DIR dj;         /* Directory object */
  FILINFO fno;    /* File information */
  FRESULT dfr = f_findfirst(&dj, &fno, _folder.c_str(), "*.*");

  while (dfr == FR_OK && fno.fname[0]) {
    printf("file %s\n", fno.fname);
    
    // ZxSpectrumFile *fsi = new ZxSpectrumFile(fno.fname);
    
    dfr = f_findnext(&dj, &fno); // Search for next item
  }
}
