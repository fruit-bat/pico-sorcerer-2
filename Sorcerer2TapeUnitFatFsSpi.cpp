#include "Sorcerer2TapeUnitFatFsSpi.h"
#include <pico/printf.h>
#include "pico/stdlib.h"

Sorcerer2TapeUnitFatFsSpi::Sorcerer2TapeUnitFatFsSpi(Sorcerer2SdCardFatFsSpi* sdCard, const char* folderName) : 
  _sdCard(sdCard),
  _folderName(folderName),
  dfr(FR_NOT_READY),
  fr(FR_NOT_READY)
{
}

bool Sorcerer2TapeUnitFatFsSpi::readyForRead(){
  return _sdCard->mounted() && dfr == FR_OK && fr == FR_OK;
}

bool Sorcerer2TapeUnitFatFsSpi::readyForWrite(){
  return false;
}

void Sorcerer2TapeUnitFatFsSpi::writeData(unsigned char data){
  // TODO
  printf("Tape write\n");
}

unsigned char Sorcerer2TapeUnitFatFsSpi::readData(){
  if (dfr == FR_OK) {
    while (fr == FR_OK) {
      char d;
      UINT br = 0;
      fr = f_read(&fil, &d, 1, &br);
      if (fr != FR_OK || br == 0) {
        if (!nextFile()) break;
      }
      return d;
    }
  }  
  return 0;
}

bool Sorcerer2TapeUnitFatFsSpi::openFile() {
  printf("open file...\n");
  fr = FR_NOT_READY;
  if (dfr == FR_OK && fno.fname[0]) {
    char name[280];
    sprintf(name, "/tapes/%s", fno.fname);
    printf("Tape file %s\n", name);
    fr = f_open(&fil, name, FA_READ|FA_OPEN_EXISTING);
    if (FR_OK != fr && FR_EXIST != fr) {
      printf("f_open(%s) error: %s (%d)\n", name, FRESULT_str(fr), fr);
      return false;
    }
    printf("Tape file %s ok!\n", name);
    fr = FR_OK;
    return true;
  }
  else {
    return false;
  }
}

bool Sorcerer2TapeUnitFatFsSpi::nextFile() {
  
  if (fr == FR_OK) {
    f_close(&fil);
  }
  
  fr = FR_NOT_READY;
  
  if (dfr == FR_OK && fno.fname[0]) {
    dfr = f_findnext(&dj, &fno); // Search for next item
    if (!fno.fname[0]) dfr = FR_NOT_READY;
  }
  else {
    dfr = FR_NOT_READY;
  }
  
  return openFile();
}

void Sorcerer2TapeUnitFatFsSpi::motor(bool on) {
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
      openFile();
    }
  }
  else {
    // close stuff
    if (_sdCard->mounted()) {
      if (fr == FR_OK) {
        f_close(&fil);
      }
      if (dfr == FR_OK) {
        f_closedir(&dj);
      }
    }
    dfr = FR_NOT_READY;
    fr = FR_NOT_READY;
  }
}

void Sorcerer2TapeUnitFatFsSpi::setBaud1200(bool baud1200) {
    printf("Tape baud %s\n", (baud1200 ? "1200" : "300"));

  // Don't need to take any action here
}
