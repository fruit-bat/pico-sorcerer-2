#pragma once

#include "pico/stdlib.h"
#include "pico/scanvideo.h"

#ifdef __cplusplus
extern "C" {
#endif

void sorcerer2_prepare_scanvideo_scanline(
  struct scanvideo_scanline_buffer *scanline_buffer, 
  uint32_t y, 
  uint32_t frame,
  uint8_t* chars,
  uint8_t* exchr  
);

#ifdef __cplusplus
}
#endif
