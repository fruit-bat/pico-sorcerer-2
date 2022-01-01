#pragma once

class Sorcerer2Disk {
public:
  virtual ~Sorcerer2Disk() {}
  virtual void seek(long p);
  virtual int read(int sectorIndex);
  virtual void write(const int b, int sectorIndex);
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
  virtual const char *name();
};

