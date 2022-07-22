#include "Sorcerer2TapeFatFsSpi.h"
#include "ff.h"
#include "ff_util.h"
#include "pico/stdlib.h"

Sorcerer2TapeFatFsSpi::Sorcerer2TapeFatFsSpi(SdCardFatFsSpi* sdCard, const char* folder, const char* name, bool writable) :
  Sorcerer2Tape(name),
  _sdCard(sdCard),
  _folder(folder),
  _open(false),
  _writable(writable),
  _recording(false)
{
}

Sorcerer2TapeFatFsSpi::~Sorcerer2TapeFatFsSpi() {
  close();
}

uint8_t Sorcerer2TapeFatFsSpi::read(){
  if (!_open || _recording) return 0;
  
  UINT br = 0;
  BYTE d;
  FRESULT fr = f_read(&_fil, &d, 1, &br);
  if (br == 0) {
    printf("eof\n");
    close();
    return 0;
  }
  if (fr != FR_OK) {
    printf("f_read(%s) error: (%d)\n", FRESULT_str(fr), fr);
    close();
    return 0;
  }
  return (uint8_t)d;
}

void Sorcerer2TapeFatFsSpi::write(uint8_t data){
  if (!_open || !_recording) return;

  UINT br;
  FRESULT fr = f_write(&_fil, &data, 1, &br); 
  if (FR_OK != fr) {
    printf("f_write(%s) error: %s (%d)\n", name(), FRESULT_str(fr), fr);
    close();
  }
}

bool Sorcerer2TapeFatFsSpi::writable(){
  return _writable;
}

bool Sorcerer2TapeFatFsSpi::open(){
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());
  printf("Tape open %s\n", fname.c_str());
  if (_open) return true;
  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  BYTE mode = _recording ? FA_WRITE | FA_OPEN_APPEND : FA_READ;
  FRESULT fr = f_open(&_fil, fname.c_str(), mode);
  if (FR_OK != fr && FR_EXIST != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }
  _open = true;
  return true;
}

void Sorcerer2TapeFatFsSpi::close(){
  _recording = false;
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
  fname.append("/");
  fname.append(name());
    
  FILINFO fno;
  FRESULT fr = f_stat(fname.c_str(), &fno);

  // check if it already exists
  return fr == FR_OK;
}

bool Sorcerer2TapeFatFsSpi::create() {
 
  if (_open) return false;

  if (!_sdCard->mounted()) {
    if (!_sdCard->mount()) return false;
  }
  
  std::string fname(_folder);
  fname.append("/");
  fname.append(name());  
  printf("Create tape %s\n", fname.c_str());
  
  FIL fil;
  FRESULT fr = f_open(&fil, fname.c_str(), FA_CREATE_NEW|FA_READ|FA_WRITE);
  if (FR_OK != fr) {
    printf("f_open(%s) error: %s (%d)\n", fname.c_str(), FRESULT_str(fr), fr);
    return false;
  }

  fr = f_close(&fil);
  if (FR_OK != fr) {
    printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    return false;
  }
  
  return true;
}

bool Sorcerer2TapeFatFsSpi::record() {
  _recording = _writable && !_open;
  return _recording;
}

