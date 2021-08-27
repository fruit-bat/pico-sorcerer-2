#include <pico/printf.h>
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
#include "hw_config.h"

#include "Sorcerer2DiskFatFsSpi.h"

Sorcerer2DiskFatFsSpi::Sorcerer2DiskFatFsSpi(
  Sorcerer2SdCardFatFsSpi* sdCard,
  const char* name
) :
  _sdCard(sdCard),
  _name(name),
  _open(false)
{
}

void Sorcerer2DiskFatFsSpi::seek(long p) {
  // printf("Disk seek %ld\n", p);
  FRESULT fr = f_lseek(&_fil, p);
  if (FR_OK != fr) {
    printf("f_lseek(%s) error: %s (%d)\n", _name, FRESULT_str(fr), fr);
    close();
  }
  // printf("Disk seek pos %lld\n", f_tell(&_fil));
  UINT br;
  fr = f_read(&_fil, _sector, sizeof(_sector), &br); 
  if (FR_OK != fr) {
    printf("f_read(%s) error: %s (%d)\n", _name, FRESULT_str(fr), fr);
    close();
  }
}

int Sorcerer2DiskFatFsSpi::read(int sectorIndex) {
  int v = _sector[sectorIndex];
  // printf("Disk read %02X\n", v);
  return v;
}

void Sorcerer2DiskFatFsSpi::write(const int b, int sectorIndex) {
  printf("Disk write %04X %02X\n", sectorIndex, b);
  
}

bool Sorcerer2DiskFatFsSpi::open() {
  printf("Drive open\n");
  if (_open) return true;
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  FRESULT fr = f_open(&_fil, _name, FA_READ);
  if (FR_OK != fr && FR_EXIST != fr) {
    printf("f_open(%s) error: %s (%d)\n", _name, FRESULT_str(fr), fr);
    return false;
  }
  _open = true;
  return true;
}

void Sorcerer2DiskFatFsSpi::close() {
  printf("Drive close\n");
  FRESULT fr = f_close(&_fil);
  if (FR_OK != fr) {
    printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
  }
  _open = false;
  
  // TODO flush any output

  // Think about when we should unmount the SD card  
}

bool Sorcerer2DiskFatFsSpi::isOpen() {
  // printf("Disk isOpen %c\n", _open ? 'y':'n');
  return _open;
}
