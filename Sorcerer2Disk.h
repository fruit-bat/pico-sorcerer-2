#pragma once

class Sorcerer2Disk {
public:
  virtual void seek(long p);
  virtual int read();
  virtual void write(const int b);
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
};

