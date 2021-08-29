#include <pico/printf.h>

#include "Sorcerer2DiskDrive.h"
#include "Sorcerer2DiskConsts.h"

Sorcerer2DiskDrive::Sorcerer2DiskDrive(int unit) : 
  _disk(0),
  _unit(unit),
  _activeCount(0),
  _sectorNumber(0),
  _trackNumber(0),
  _newSector(false),
  _sectorIndex(0),
  _dataReady(false)
{
}

void Sorcerer2DiskDrive::insert(Sorcerer2Disk* disk) {
  if (_disk && _disk->isOpen()) _disk->close();
  _disk = disk;
  _dataReady = false;
}

Sorcerer2Disk* Sorcerer2DiskDrive::eject() {
  deactivate();
  Sorcerer2Disk* disk = _disk;
  _disk = 0;
  _dataReady = false;

  return disk;
}
  
bool Sorcerer2DiskDrive::active() {
  return _activeCount > 0;
}

bool Sorcerer2DiskDrive::dataReady() {
  return _dataReady;
}

bool Sorcerer2DiskDrive::home() {
  return _trackNumber == 0;
}

void Sorcerer2DiskDrive::stepForward() {
  _dataReady = false;
  _sectorIndex = 0;

  if ( active() ) {
    _activeCount = ACTIVE_FOR_TICKS;
  }
  
  if (_trackNumber < ( NUMBER_OF_TRACKS - 1 ) ) {
	++_trackNumber;
  }
}

void Sorcerer2DiskDrive::stepBackward() {
  _dataReady = false;
  _sectorIndex = 0;

  if ( active() ) {
    _activeCount = ACTIVE_FOR_TICKS;
  }
  
  if ( _trackNumber > 0 ) {
    --_trackNumber;
  }
}

void Sorcerer2DiskDrive::readyWrite() {
  if ( active() ) {
    _activeCount = ACTIVE_FOR_TICKS;
  }
  	
  // TODO Think about this
}

void Sorcerer2DiskDrive::activate() {
	// TODO Think about this
  _dataReady = false;
  _sectorIndex = 0;
  _sectorNumber = 0;

  if (_disk && _activeCount == 0 && (_disk->isOpen() ||_disk->open())) {
    _activeCount = ACTIVE_FOR_TICKS;
    seekDisk();
  }
}

void Sorcerer2DiskDrive::deactivate() {
  if (_disk && _disk->isOpen()) _disk->close();
  _activeCount = 0;
  _sectorNumber = 0;
  _trackNumber = 0;
  _newSector = false;
  _dataReady = false;
  _sectorIndex = 0;
}
  
void Sorcerer2DiskDrive::tick() {
  if ( active() ) {
    _sectorNumber++;
    
    if (_sectorIndex > 0 && _sectorIndex < BYTES_PER_SECTOR) {
		if(_dataReady) {
			_dataReady = false;
			return;
		}
		if (_sectorIndex !=3) printf("******* DISK UNDERRUN %d*********\n", _sectorIndex);
		
	}
    _dataReady = true;
    
    _sectorIndex = 0;

    if ( _sectorNumber >= SECTORS_PER_TRACK ) {
	  _sectorNumber = 0;
    }
    _newSector = true;

    _activeCount--;

    if ( !active() ) {
      deactivate();
    }
    else {
      seekDisk();
    }
  }
}

int Sorcerer2DiskDrive::readReg0() {
  if ( active() ) {
    _activeCount = ACTIVE_FOR_TICKS;
  }
  int r = _sectorNumber;
  if ( _newSector ) {
    r |= 0x80;
    _newSector = false;
  }
  // printf("Disk drive reg0 %02X\n", r);
  return r;
}
  
int Sorcerer2DiskDrive::readReg2() {
  if ( active() ) {
    _activeCount = ACTIVE_FOR_TICKS;
  }
  if ( _disk && _disk->isOpen() && (_sectorIndex < BYTES_PER_SECTOR) ) {
    return _disk->read(_sectorIndex++) & 0xff;
  }
  return 0;
}

void Sorcerer2DiskDrive::writeReg2(const int b) {
  if ( active() ) {
	_activeCount = ACTIVE_FOR_TICKS;
  }
  if (_disk && _disk->isOpen() && (_sectorIndex < BYTES_PER_SECTOR) ) {
	_disk->write(b, _sectorIndex++);
  }
}

void Sorcerer2DiskDrive::seekDisk() {
  if (_disk && _disk->isOpen()) {
	  _disk->seek(((_trackNumber * SECTORS_PER_TRACK) + _sectorNumber) * BYTES_PER_SECTOR);
  }
}

char Sorcerer2DiskDrive::driveLetter() {
  return "ABCD"[_unit];
}

void Sorcerer2DiskDrive::reset() {
  deactivate();
}
