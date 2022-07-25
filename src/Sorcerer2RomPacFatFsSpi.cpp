#include "Sorcerer2RomPacFatFsSpi.h"
#include "ff.h"
#include "ff_util.h"
#include "pico/stdlib.h"

Sorcerer2RomPacFatFsSpi::Sorcerer2RomPacFatFsSpi(SdCardFatFsSpi* sdCard, const char* folder, const char* name) :
  Sorcerer2RomPac(name),
  _sdCard(sdCard),
  _folder(folder),
  _open(false)
{
}

Sorcerer2RomPacFatFsSpi::~Sorcerer2RomPacFatFsSpi() {
  close();
}

uint8_t Sorcerer2RomPacFatFsSpi::read(){
  if (!_open) return 0;
  
  UINT br = 0;
  BYTE d;
  FRESULT _fr = f_read(&_fil, &d, 1, &br);
  if (br == 0) {
    printf("eof\n");
    close();
    return 0;
  }
  if (_fr != FR_OK) {
    printf("f_read(%s) error: (%d)\n", FRESULT_str(_fr), _fr);
    close();
    return 0;
  }
  return (uint8_t)d;
}

bool Sorcerer2RomPacFatFsSpi::open(){
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());
  printf("Rompac open %s\n", fname.c_str());
  if (_open) return true;
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  BYTE mode = FA_READ; // |FA_WRITE
  FRESULT fr = f_open(&_fil, fname.c_str(), mode);
  if (FR_OK != fr && FR_EXIST != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }
  _open = true;
  return true;
}

void Sorcerer2RomPacFatFsSpi::close(){
  if (_open) {
    printf("Rompac close\n");
    FRESULT fr = f_close(&_fil);
    if (FR_OK != fr) {
      printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    _open = false;
  }  
}
