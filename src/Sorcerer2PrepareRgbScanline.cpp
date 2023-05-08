#include "Sorcerer2PrepareRgbScanline.h"

#define VGA_RGBY_1111(r,g,b,y) ((y##UL<<3)|(r##UL<<2)|(g##UL<<1)|b##UL)
#define VGA_RGB_332(r,g,b) ((r##UL<<5)|(g##UL<<2)|b##UL)
#define VGA_RGB_222(r,g,b) ((r##UL<<4)|(g##UL<<2)|b##UL)

#define X4(a) (a | (a << 8) | (a << 16) | (a << 24))

static uint32_t colour_words[2] = {
#if defined(VGA_ENC_RGBY_1111)
  X4(VGA_RGBY_1111(0,0,0,0)), // Black
//  X4(VGA_RGBY_1111(0,0,1,0)), // Blue
//  X4(VGA_RGBY_1111(1,0,0,0)), // Red
//  X4(VGA_RGBY_1111(1,0,1,0)), // Magenta
 // X4(VGA_RGBY_1111(0,1,0,0)), // Green
//  X4(VGA_RGBY_1111(0,1,1,0)), // Cyan
//  X4(VGA_RGBY_1111(1,1,0,0)), // Yellow
  X4(VGA_RGBY_1111(1,1,1,0)), // White
//  X4(VGA_RGBY_1111(0,0,0,0)), // Bright Black
//  X4(VGA_RGBY_1111(0,0,1,1)), // Bright Blue
//  X4(VGA_RGBY_1111(1,0,0,1)), // Bright Red
//  X4(VGA_RGBY_1111(1,0,1,1)), // Bright Magenta
//  X4(VGA_RGBY_1111(0,1,0,1)), // Bright Green
//  X4(VGA_RGBY_1111(0,1,1,1)), // Bright Cyan
//  X4(VGA_RGBY_1111(1,1,0,1)), // Bright Yellow
//  X4(VGA_RGBY_1111(1,1,1,1))  // Bright White
#elif defined(VGA_ENC_RGB_222)
  X4(VGA_RGB_222(0,0,0)), // Black
//  X4(VGA_RGB_222(0,0,2)), // Blue
//  X4(VGA_RGB_222(2,0,0)), // Red
//  X4(VGA_RGB_222(2,0,2)), // Magenta
//  X4(VGA_RGB_222(0,2,0)), // Green
//  X4(VGA_RGB_222(0,2,2)), // Cyan
//  X4(VGA_RGB_222(2,2,0)), // Yellow
  X4(VGA_RGB_222(2,2,2)), // White
//  X4(VGA_RGB_222(0,0,0)), // Bright Black
//  X4(VGA_RGB_222(0,0,3)), // Bright Blue
//  X4(VGA_RGB_222(3,0,0)), // Bright Red
//  X4(VGA_RGB_222(3,0,3)), // Bright Magenta
//  X4(VGA_RGB_222(0,3,0)), // Bright Green
//  X4(VGA_RGB_222(0,3,3)), // Bright Cyan
//  X4(VGA_RGB_222(3,3,0)), // Bright Yellow
//  X4(VGA_RGB_222(3,3,3))  // Bright White
#else // default RGB_332
  X4(VGA_RGB_332(0,0,0)), // Black
//  X4(VGA_RGB_332(0,0,2)), // Blue
//  X4(VGA_RGB_332(5,0,0)), // Red
//  X4(VGA_RGB_332(5,0,2)), // Magenta
//  X4(VGA_RGB_332(0,5,0)), // Green
//  X4(VGA_RGB_332(0,5,2)), // Cyan
//  X4(VGA_RGB_332(5,5,0)), // Yellow
  X4(VGA_RGB_332(5,5,2)), // White
//  X4(VGA_RGB_332(0,0,0)), // Bright Black
//  X4(VGA_RGB_332(0,0,3)), // Bright Blue
//  X4(VGA_RGB_332(7,0,0)), // Bright Red
//  X4(VGA_RGB_332(7,0,3)), // Bright Magenta
//  X4(VGA_RGB_332(0,7,0)), // Bright Green
//  X4(VGA_RGB_332(0,7,3)), // Bright Cyan
//  X4(VGA_RGB_332(7,7,0)), // Bright Yellow
//  X4(VGA_RGB_332(7,7,3))  // Bright White
#endif
};

static uint32_t nibble_masks[16] = {
  0x00000000,
  0xff000000,
  0x00ff0000,
  0xffff0000,
  0x0000ff00,
  0xff00ff00,
  0x00ffff00,
  0xffffff00,
  0x000000ff,
  0xff0000ff,
  0x00ff00ff,
  0xffff00ff,
  0x0000ffff,
  0xff00ffff,
  0x00ffffff,
  0xffffffff,
};

void __not_in_flash_func(prepare_rgb_scanline)(
  uint32_t* buf, 
  uint32_t y, 
  uint8_t* chars,
  uint8_t* exchr
  ) {
    const uint32_t bcw = colour_words[0]; // The background colour word
    const uint32_t fcw = colour_words[1]; // The foreground colour word
    const uint32_t cr = y & 7;
    const uint32_t ys = (y >> 3) << 6;

    for (int i = 0; i < 16; ++i) *buf++ = bcw;

    for (int i = 0; i < 64; ++i) {
      uint8_t p = exchr[(chars[i + ys] << 3) + cr]; // fetch a byte of pixel data
      uint32_t fgm;
      uint32_t bgm;
      fgm = nibble_masks[(p >> 4) & 15];
      bgm = ~fgm;
      *buf++ = (fgm & fcw) | (bgm & bcw);     
      fgm = nibble_masks[(p >> 0) & 15];
      bgm = ~fgm;
      *buf++ = (fgm & fcw) | (bgm & bcw);             
    }
    
    for (int i = 0; i < 16; ++i) *buf++ = bcw;
}


