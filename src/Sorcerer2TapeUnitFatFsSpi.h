#pragma once

#include "SdCardFatFsSpi.h"
#include "Sorcerer2TapeUnit.h"
#include "ff.h"
#include "pico/stdlib.h"

class Sorcerer2TapeUnitFatFsSpi : public Sorcerer2TapeUnit {
  SdCardFatFsSpi* _sdCard;
  const char* _folderName;
  FRESULT dfr;    /* Return value for directory */
  DIR dj;         /* Directory object */
  FILINFO fno;    /* File information */
  FRESULT fr;     /* Return value for file */
  FIL fil;        /* File object */
  bool nextFile();
  bool openFile();
public:
  Sorcerer2TapeUnitFatFsSpi(SdCardFatFsSpi* sdCard, const char* folderName);
  
  virtual bool readyForRead();
  virtual bool readyForWrite();  
  virtual void writeData(unsigned char data);
  virtual unsigned char readData();
  virtual void motor(bool on);
  virtual void setBaud1200(bool);
};
