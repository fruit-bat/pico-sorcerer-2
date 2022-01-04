#pragma once

class Sorcerer2Tape;

class Sorcerer2TapeUnit {
private: 
  
  Sorcerer2Tape *_tape;
  bool _motorOn;
  
public:

  Sorcerer2TapeUnit();
  virtual ~Sorcerer2TapeUnit() {}

  virtual bool readyForRead();
  virtual bool readyForWrite();  
  virtual void writeData(unsigned char data);
  virtual unsigned char readData();
  virtual void motor(bool on);
  virtual void setBaud1200(bool);
  
  Sorcerer2Tape *insert(Sorcerer2Tape *tape);
  Sorcerer2Tape *tape() { return _tape; }
  Sorcerer2Tape *eject();

};
