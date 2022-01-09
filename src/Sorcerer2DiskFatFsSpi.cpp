#include <pico/printf.h>
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
#include "hw_config.h"
#include <cstring>

#include "Sorcerer2DiskFatFsSpi.h"

Sorcerer2DiskFatFsSpi::Sorcerer2DiskFatFsSpi(
  SdCardFatFsSpi* sdCard,
  const char* folder,
  const char* name
) :
  _sdCard(sdCard),
  _name(name),
  _folder(folder),
  _sectorRead(false),
  _offset(0),
  _open(false)
{
}

Sorcerer2DiskFatFsSpi::~Sorcerer2DiskFatFsSpi() {
  close();
}

void Sorcerer2DiskFatFsSpi::seek(long p) {
  _offset = p;
  _sectorRead = false;
}

void Sorcerer2DiskFatFsSpi::readSector() {
  // printf("Disk seek %ld\n", p);
  FRESULT fr = f_lseek(&_fil, _offset);
  if (FR_OK != fr) {
    printf("f_lseek(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
    close();
  }
  // printf("Disk seek pos %lld\n", f_tell(&_fil));
  UINT br;
  fr = f_read(&_fil, _sector, sizeof(_sector), &br); 
  if (FR_OK != fr) {
    printf("f_read(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
    close();
  }
  _sectorRead = true;
}

int Sorcerer2DiskFatFsSpi::read(int sectorIndex) {
  if (!_sectorRead) readSector();
  return _sector[sectorIndex];
}

void Sorcerer2DiskFatFsSpi::write(const int b, int sectorIndex) {
  // printf("Disk write %04X %02X\n", sectorIndex, b);
  _sector[sectorIndex] = b;
  if (sectorIndex == (BYTES_PER_SECTOR - 1)) {
    // printf("Disk write complete sector\n");
    FRESULT fr = f_lseek(&_fil, _offset);
    if (FR_OK != fr) {
      printf("f_lseek(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
      close();
    }
    // printf("Disk seek pos %lld\n", f_tell(&_fil));
    UINT br;
    fr = f_write(&_fil, _sector, sizeof(_sector), &br); 
    if (FR_OK != fr) {
      printf("f_write(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
      close();
    }    
  }
}

bool Sorcerer2DiskFatFsSpi::open() {
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());  
  printf("Drive open %s\n", fname.c_str());
  if (_open) return true;
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  FRESULT fr = f_open(&_fil, fname.c_str(), FA_READ|FA_WRITE);
  if (FR_OK != fr && FR_EXIST != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }
  _offset = 0;
  _sectorRead = false;
  _open = true;
  return true;
}

void Sorcerer2DiskFatFsSpi::close() {
  if (_open) {
    printf("Drive close\n");
    FRESULT fr = f_close(&_fil);
    if (FR_OK != fr) {
      printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    _open = false;
    
    // TODO flush any output

    // Think about when we should unmount the SD card  
  }
}

bool Sorcerer2DiskFatFsSpi::isOpen() {
  // printf("Disk isOpen %c\n", _open ? 'y':'n');
  return _open;
}


bool Sorcerer2DiskFatFsSpi::exists() {
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());
    
  FILINFO fno;
  FRESULT fr = f_stat(fname.c_str(), &fno);

  // check if it already exists
  return fr == FR_OK;
}

bool Sorcerer2DiskFatFsSpi::create() {
 
  if (_open) return false;

  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());  
  printf("Create disk %s\n", fname.c_str());
  
  FIL fil;
  FRESULT fr = f_open(&fil, fname.c_str(), FA_CREATE_NEW|FA_READ|FA_WRITE);
  if (FR_OK != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }

  memset(_sector, 0, sizeof(_sector));
  
  for (int i = 0; i < (NUMBER_OF_TRACKS * SECTORS_PER_TRACK); ++i) {
    UINT br;
    fr = f_write(&fil, _sector, sizeof(_sector), &br); 
    if (FR_OK != fr) {
      printf("f_write(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
      f_close(&fil);
      return false;
    }   
  }
  
  fr = f_close(&fil);
  if (FR_OK != fr) {
    printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    return false;
  }
  
  return true;
}


