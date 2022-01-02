#pragma once

class Sorcerer2Tape;

class Sorcerer2TapeUnit {
  
public:
  virtual bool readyForRead();
  virtual bool readyForWrite();  
  virtual void writeData(unsigned char data);
  virtual unsigned char readData();
  virtual void motor(bool on);
  virtual void setBaud1200(bool);
  
  // TODO
  Sorcerer2Tape *tape() { return 0; }
  Sorcerer2Tape *eject() { return 0; }

};
