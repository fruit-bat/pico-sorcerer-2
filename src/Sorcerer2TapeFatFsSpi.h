#pragma once

#include "Sorcerer2Tape.h"
#include "SdCardFatFsSpi.h"

class Sorcerer2TapeFatFsSpi : public Sorcerer2Tape {
private:
  SdCardFatFsSpi* _sdCard;
  std::string _folder;
  FIL _fil;
  bool _open;
  bool _writable;
  bool _recording;
public:
  Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* folder, const char* name, bool writable);
  virtual ~Sorcerer2TapeFatFsSpi();
  virtual uint8_t read();
  virtual void write(uint8_t data);
  virtual bool writable();
  virtual bool open();
  virtual void close();
  virtual bool isOpen();
  bool exists();
  bool create();
  virtual bool isRecording() { return _recording; }
  virtual bool record();
};
