#pragma once

class Sorcerer2TapeUnit {
  
public:
  virtual bool readyForRead();
  virtual bool readyForWrite();  
  virtual void writeData(unsigned char data);
  virtual unsigned char readData();
  virtual void motor(bool on);
  virtual void setBaud1200(bool);
};
