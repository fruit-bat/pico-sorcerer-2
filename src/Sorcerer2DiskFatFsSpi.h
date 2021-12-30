#pragma once

#include "Sorcerer2Disk.h"
#include "SdCardFatFsSpi.h"
#include "Sorcerer2DiskConsts.h"

class Sorcerer2DiskFatFsSpi : public Sorcerer2Disk {
  SdCardFatFsSpi* _sdCard;
  const char* _name;
  FIL _fil;
  uint8_t _sector[BYTES_PER_SECTOR];
  bool _sectorRead;
  long _offset;
  bool _open;
  void readSector();
public:
  Sorcerer2DiskFatFsSpi(SdCardFatFsSpi* sdCard, const char* name);
  virtual ~Sorcerer2DiskFatFsSpi();
  virtual void seek(long p);
  virtual int read(int sectorIndex);
  virtual void write(const int b, int sectorIndex);
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
};
