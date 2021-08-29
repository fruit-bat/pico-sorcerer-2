#pragma once

#include "Sorcerer2SdCardFatFsSpi.h"
#include "Sorcerer2TapeUnit.h"

class Sorcerer2TapeUnitFatFsSpi : public Sorcerer2TapeUnit {
  Sorcerer2SdCardFatFsSpi* _sdCard;
  const char* _folderName;
  FRESULT dfr;    /* Return value for directory */
  DIR dj;         /* Directory object */
  FILINFO fno;    /* File information */

public:
  Sorcerer2TapeUnitFatFsSpi(Sorcerer2SdCardFatFsSpi* sdCard, const char* folderName);
  
  virtual bool readyForRead();
  virtual bool readyForWrite();  
  virtual void writeData(unsigned char data);
  virtual unsigned char readData();
  virtual void motor(bool on);
  virtual void setBaud1200(bool);
};
