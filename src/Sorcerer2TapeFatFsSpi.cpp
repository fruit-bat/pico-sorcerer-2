#include "Sorcerer2TapeFatFsSpi.h"
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
#include "hw_config.h"

Sorcerer2TapeFatFsSpi::Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* folder, const char* name, bool writable) :
  Sorcerer2Tape(name),
  _sdCard(sdCard),
  _folder(folder),
  _open(false),
  _writable(writable)
{
}

Sorcerer2TapeFatFsSpi::~Sorcerer2TapeFatFsSpi() {
  close();
}

uint8_t Sorcerer2TapeFatFsSpi::read(){
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

void Sorcerer2TapeFatFsSpi::write(uint8_t data){
  printf("writing to tape (%d)\n", data);

}

bool Sorcerer2TapeFatFsSpi::writable(){
  return _writable;
}

bool Sorcerer2TapeFatFsSpi::open(){
  std::string fname(_folder);
  fname.append(name());
  printf("Tape open %s\n", fname.c_str());
  if (_open) return true;
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  BYTE mode = FA_READ | FA_OPEN_ALWAYS; // |FA_WRITE
  FRESULT fr = f_open(&_fil, fname.c_str(), mode);
  if (FR_OK != fr && FR_EXIST != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }
  _open = true;
  return true;
}

void Sorcerer2TapeFatFsSpi::close(){
  if (_open) {
    printf("Tape close\n");
    FRESULT fr = f_close(&_fil);
    if (FR_OK != fr) {
      printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    _open = false;
  }  
}

bool Sorcerer2TapeFatFsSpi::isOpen(){
  return _open;
}

bool Sorcerer2TapeFatFsSpi::exists() {
  std::string fname(_folder);
  fname.append(name());
    
  FILINFO fno;
  FRESULT fr = f_stat(fname.c_str(), &fno);

  // check if it already exists
  return fr == FR_OK;
}
