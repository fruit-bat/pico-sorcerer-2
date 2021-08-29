#include "Sorcerer2TapeUnitFatFsSpi.h"
#include <pico/printf.h>

Sorcerer2TapeUnitFatFsSpi::Sorcerer2TapeUnitFatFsSpi(Sorcerer2SdCardFatFsSpi* sdCard, const char* folderName) : 
  _sdCard(sdCard),
  _folderName(folderName),
  dfr(FR_NOT_READY)
{
}

bool Sorcerer2TapeUnitFatFsSpi::readyForRead(){
  return _sdCard->mounted() /* && TODO */;
}

bool Sorcerer2TapeUnitFatFsSpi::readyForWrite(){
  return false;
}


void Sorcerer2TapeUnitFatFsSpi::writeData(unsigned char data){
  // TODO
  printf("Tape write\n");
}


unsigned char Sorcerer2TapeUnitFatFsSpi::readData(){
  printf("Tape read\n");
  return 0;
}


void Sorcerer2TapeUnitFatFsSpi::motor(bool on){
  printf("Tape motor %s\n", (on ? "on" : "off"));
  if (on) {
    if (!_sdCard->mounted()) {
      if (!_sdCard->mount()) {
        printf("Failed to mount SD card for tape unit\n");

        return;
      }
    }
    if (dfr != FR_OK) {
      dfr = f_findfirst(&dj, &fno, _folderName, "*.tape"); /* Start to search for tape files */

      while (dfr == FR_OK && fno.fname[0]) {         /* Repeat while an item is found */
          printf("%s\n", fno.fname);                /* Print the object name */
          dfr = f_findnext(&dj, &fno);               /* Search for next item */
      }
    }
  }
  else {
    // TODO close stuff
    if (_sdCard->mounted()) {
      f_closedir(&dj); 
    }
    dfr = FR_NOT_READY;
  }
}


void Sorcerer2TapeUnitFatFsSpi::setBaud1200(bool baud1200){
    printf("Tape baud %s\n", (baud1200 ? "1200" : "300"));

  // Don't need to take any action here
}

