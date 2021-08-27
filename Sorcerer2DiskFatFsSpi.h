#pragma once

#include "Sorcerer2Disk.h"
#include "Sorcerer2SdCardFatFsSpi.h"
#include "Sorcerer2DiskConsts.h"

class Sorcerer2DiskFatFsSpi : public Sorcerer2Disk {
  Sorcerer2SdCardFatFsSpi* _sdCard;
  const char* _name;
  FIL _fil;
  unsigned char _sector[BYTES_PER_SECTOR];
  //bool _sectorRead = false;
  bool _open;
public:
  Sorcerer2DiskFatFsSpi(Sorcerer2SdCardFatFsSpi* sdCard, const char* name);
  virtual void seek(long p);
  virtual int read(int sectorIndex);
  virtual void write(const int b, int sectorIndex);
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
};
