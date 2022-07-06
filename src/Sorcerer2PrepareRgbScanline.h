#pragma once

#include "pico/stdlib.h"

#ifdef __cplusplus
extern "C" {
#endif

void __not_in_flash_func(prepare_rgb_scanline)(
  uint32_t* buf, 
  uint32_t y, 
  uint8_t* chars,
  uint8_t* exchr
);

#ifdef __cplusplus
}
#endif
