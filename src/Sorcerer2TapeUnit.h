#pragma once

class Sorcerer2Tape;

class Sorcerer2TapeUnit {
  Sorcerer2Tape *_tape;
  
public:

  Sorcerer2TapeUnit();
  virtual ~Sorcerer2TapeUnit() {}

  virtual bool readyForRead() { return true; }
  virtual bool readyForWrite()  { return true; }  
  virtual void writeData(unsigned char data) {}
  virtual unsigned char readData()  { return 0; }
  virtual void motor(bool on) {}
  virtual void setBaud1200(bool) {}
  
  // TODO
  Sorcerer2Tape *insert(Sorcerer2Tape *tape);
  Sorcerer2Tape *tape() { return _tape; }
  Sorcerer2Tape *eject();

};
