#include <pico/printf.h>

#include "Sorcerer2DiskSystem.h"

Sorcerer2DiskSystem::Sorcerer2DiskSystem() :
  _drives({
	  Sorcerer2DiskDrive(0),
	  Sorcerer2DiskDrive(1),
	  Sorcerer2DiskDrive(2),
	  Sorcerer2DiskDrive(3)
  }),
  _activeDriveNumber(0x40),
  _activeDrive(0)
{
}

void Sorcerer2DiskSystem::tick() {
  for (int i = 0; i < 4; ++i ) {
	_drives[ i ].tick();
  }
  if (_activeDrive) {
	if (!_activeDrive->active()) {
		_activeDrive = 0;
		_activeDriveNumber = 0x40;
	}
  }
}

int Sorcerer2DiskSystem::readByte(int address) {
  // printf("Disk system read %04X\n", address);
  switch (address - 0xBE00) {
    case 0: return readReg0();
    case 1: return readReg1();
    case 2: return readReg2();
  }
  return 0;	
}

void Sorcerer2DiskSystem::writeByte(int address, int b) {
  //printf("Disk system write %04X %02X\n", address, b);
  switch (address - 0xBE00) {
    case 0: writeReg0( b ); break;
    case 1: writeReg1( b ); break;
    case 2: writeReg2( b ); break;
  }
}

bool Sorcerer2DiskSystem::dataReady() {
  return _activeDrive ? _activeDrive->dataReady() : false;
}

bool Sorcerer2DiskSystem::home() {
  return _activeDrive ? _activeDrive->home() : false;
}

bool Sorcerer2DiskSystem::active() {
  return !!_activeDrive;
}

void Sorcerer2DiskSystem::stepForward() {
  if (_activeDrive) _activeDrive->stepForward();
}

void Sorcerer2DiskSystem::stepBackward() {
  if (_activeDrive) _activeDrive->stepBackward();
}

void Sorcerer2DiskSystem::readyWrite() {
  if (_activeDrive) _activeDrive->readyWrite();
}

void Sorcerer2DiskSystem::activate(const int n) {
  _activeDriveNumber = n;
  _activeDrive = &_drives[n];
  _activeDrive->activate();
}

void Sorcerer2DiskSystem::writeReg0(const int b) {
  switch ( b ) {
    case 0xA0: break;
    case 0x20: activate( 0 );    break;
    case 0x21: activate( 1 );    break;
    case 0x22: activate( 2 );    break;
    case 0x23: activate( 3 );    break;
    case 0x60: stepBackward();   break;
    case 0x61: stepForward();    break;
    case 0x80: readyWrite();     break;
  }
}

void Sorcerer2DiskSystem::writeReg1(const int b) {
  writeReg0(b);
}

void Sorcerer2DiskSystem::writeReg2(const int b) {
  if (_activeDrive) _activeDrive->writeReg2(b);
}

int Sorcerer2DiskSystem::readReg0() {
  return _activeDrive ? _activeDrive->readReg0() : 0;
}

int Sorcerer2DiskSystem::readReg1() {
  int r = _activeDriveNumber;
  if (active()) r |= 0x20;
  if (home()) r|= 0x08;
  if (dataReady()) r|= 0x80; 
  return r;
}

int Sorcerer2DiskSystem::readReg2() {
  return _activeDrive ? _activeDrive->readReg2() : 0;
}
