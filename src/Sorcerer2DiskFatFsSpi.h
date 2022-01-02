#pragma once

#include "Sorcerer2Disk.h"
#include "SdCardFatFsSpi.h"
#include "Sorcerer2DiskConsts.h"
#include <string>

class Sorcerer2DiskFatFsSpi : public Sorcerer2Disk {
private:
  SdCardFatFsSpi* _sdCard;
  std::string _name;
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
  virtual const char *name() { return _name.c_str(); }  
};
