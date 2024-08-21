#include "Sorcerer2PrepareScanvideoScanline.h"
#include "pico/scanvideo/composable_scanline.h"

#define VGA_RGB_555(r,g,b) ((r##UL<<0)|(g##UL<<6)|(b##UL << 11))
#define VGA_BGYR_1111(r,g,b,y) ((y##UL<<2)|(r##UL<<3)|(g##UL<<1)|b##UL)
#define VGA_RGBY_1111(r,g,b,y) ((y##UL<<3)|(r##UL<<2)|(g##UL<<1)|b##UL)
#define VGA_RGB_332(r,g,b) ((r##UL<<5)|(g##UL<<2)|b##UL)
#define VGA_RGB_222(r,g,b) ((r##UL<<4)|(g##UL<<2)|b##UL)

static const uint32_t colour_words[16] = {
#if defined(VGA_ENC_BGYR_1111)
  (VGA_BGYR_1111(0,0,0,0)), // Black
  (VGA_BGYR_1111(0,0,1,0)), // Blue
  (VGA_BGYR_1111(1,0,0,0)), // Red
  (VGA_BGYR_1111(1,0,1,0)), // Magenta
  (VGA_BGYR_1111(0,1,0,0)), // Green
  (VGA_BGYR_1111(0,1,1,0)), // Cyan
  (VGA_BGYR_1111(1,1,0,0)), // Yellow
  (VGA_BGYR_1111(1,1,1,0)), // White
  (VGA_BGYR_1111(0,0,0,0)), // Bright Black
  (VGA_BGYR_1111(0,0,1,1)), // Bright Blue
  (VGA_BGYR_1111(1,0,0,1)), // Bright Red
  (VGA_BGYR_1111(1,0,1,1)), // Bright Magenta
  (VGA_BGYR_1111(0,1,0,1)), // Bright Green
  (VGA_BGYR_1111(0,1,1,1)), // Bright Cyan
  (VGA_BGYR_1111(1,1,0,1)), // Bright Yellow
  (VGA_BGYR_1111(1,1,1,1))  // Bright White
#elif defined(VGA_ENC_RGBY_1111)
  (VGA_RGBY_1111(0,0,0,0)), // Black
  (VGA_RGBY_1111(0,0,1,0)), // Blue
  (VGA_RGBY_1111(1,0,0,0)), // Red
  (VGA_RGBY_1111(1,0,1,0)), // Magenta
  (VGA_RGBY_1111(0,1,0,0)), // Green
  (VGA_RGBY_1111(0,1,1,0)), // Cyan
  (VGA_RGBY_1111(1,1,0,0)), // Yellow
  (VGA_RGBY_1111(1,1,1,0)), // White
  (VGA_RGBY_1111(0,0,0,0)), // Bright Black
  (VGA_RGBY_1111(0,0,1,1)), // Bright Blue
  (VGA_RGBY_1111(1,0,0,1)), // Bright Red
  (VGA_RGBY_1111(1,0,1,1)), // Bright Magenta
  (VGA_RGBY_1111(0,1,0,1)), // Bright Green
  (VGA_RGBY_1111(0,1,1,1)), // Bright Cyan
  (VGA_RGBY_1111(1,1,0,1)), // Bright Yellow
  (VGA_RGBY_1111(1,1,1,1))  // Bright White
#elif defined(VGA_ENC_RGB_222)
  (VGA_RGB_222(0,0,0)), // Black
  (VGA_RGB_222(0,0,2)), // Blue
  (VGA_RGB_222(2,0,0)), // Red
  (VGA_RGB_222(2,0,2)), // Magenta
  (VGA_RGB_222(0,2,0)), // Green
  (VGA_RGB_222(0,2,2)), // Cyan
  (VGA_RGB_222(2,2,0)), // Yellow
  (VGA_RGB_222(2,2,2)), // White
  (VGA_RGB_222(0,0,0)), // Bright Black
  (VGA_RGB_222(0,0,3)), // Bright Blue
  (VGA_RGB_222(3,0,0)), // Bright Red
  (VGA_RGB_222(3,0,3)), // Bright Magenta
  (VGA_RGB_222(0,3,0)), // Bright Green
  (VGA_RGB_222(0,3,3)), // Bright Cyan
  (VGA_RGB_222(3,3,0)), // Bright Yellow
  (VGA_RGB_222(3,3,3))  // Bright White
#elif defined(VGA_ENC_RGB_332)
  (VGA_RGB_332(0,0,0)), // Black
  (VGA_RGB_332(0,0,2)), // Blue
  (VGA_RGB_332(5,0,0)), // Red
  (VGA_RGB_332(5,0,2)), // Magenta
  (VGA_RGB_332(0,5,0)), // Green
  (VGA_RGB_332(0,5,2)), // Cyan
  (VGA_RGB_332(5,5,0)), // Yellow
  (VGA_RGB_332(5,5,2)), // White
  (VGA_RGB_332(0,0,0)), // Bright Black
  (VGA_RGB_332(0,0,3)), // Bright Blue
  (VGA_RGB_332(7,0,0)), // Bright Red
  (VGA_RGB_332(7,0,3)), // Bright Magenta
  (VGA_RGB_332(0,7,0)), // Bright Green
  (VGA_RGB_332(0,7,3)), // Bright Cyan
  (VGA_RGB_332(7,7,0)), // Bright Yellow
  (VGA_RGB_332(7,7,3))  // Bright White  
#else    
  (VGA_RGB_555(0, 0, 0 )), // Black
  (VGA_RGB_555(0, 0, 20)), // Blue
  (VGA_RGB_555(20,0, 0 )), // Red
  (VGA_RGB_555(20,0, 20)), // Magenta
  (VGA_RGB_555(0, 20,0 )), // Green
  (VGA_RGB_555(0, 20,20)), // Cyan
  (VGA_RGB_555(20,20,0 )), // Yellow
  (VGA_RGB_555(20,20,20)), // White
  (VGA_RGB_555(0, 0, 0 )), // Bright Black
  (VGA_RGB_555(0, 0, 31)), // Bright Blue
  (VGA_RGB_555(31,0, 0 )), // Bright Red
  (VGA_RGB_555(31,0, 31)), // Bright Magenta
  (VGA_RGB_555(0, 31,0 )), // Bright Green
  (VGA_RGB_555(0, 31,31)), // Bright Cyan
  (VGA_RGB_555(31,31,0 )), // Bright Yellow
  (VGA_RGB_555(31,31,31))  // Bright White
#endif
};

static const uint32_t bgc = colour_words[0]; // The background colour word
static const uint32_t fgc = colour_words[4]; // The foreground colour word

static uint32_t bbcw[4] {
  bgc | (bgc << 16),
  bgc | (fgc << 16),
  fgc | (bgc << 16),
  fgc | (fgc << 16)
};

static uint32_t cs[2] {
  bgc << 16,
  fgc << 16
};

inline uint32_t* single_color_run(uint32_t *buf, uint32_t width, uint32_t cw) {
  // | jmp color_run | color | count-3 |  buf[0] =
  *buf++ = COMPOSABLE_COLOR_RUN | (cw << 16);
  *buf++ = (width - 3 - 2) | (COMPOSABLE_RAW_2P << 16);
  *buf++ = cw;
  return buf;
}

inline uint32_t* end_run(uint32_t *buf) {
  // end with a black pixel
  *buf++ = COMPOSABLE_RAW_1P;
  *buf++ = COMPOSABLE_EOL_SKIP_ALIGN;
  return buf;
}

void __not_in_flash_func(sorcerer2_prepare_scanvideo_scanline)(
  struct scanvideo_scanline_buffer *scanline_buffer, 
  uint32_t y, 
  uint32_t frame,
  uint8_t* chars,
  uint8_t* exchr  
) {
  uint32_t* buf = scanline_buffer->data;

  buf = single_color_run(buf, 64, bbcw[0]);
    
  const uint32_t li = (y >> 3) << 6;
  int yb = y & 7;
  uint8_t *cl = chars + li;
  for (int x = 0; x < (64 >> 1); ++x) {
    uint32_t ch = (uint32_t)*cl++;
    int p = exchr[(ch << 3) + yb];
    
    *buf++ = COMPOSABLE_RAW_RUN | cs[(p >> 7) & 1];
    *buf++ = (16 - 3) | cs[(p >> 6) & 1];
    *buf++ = bbcw[(p >> 4) & 3];
    *buf++ = bbcw[(p >> 2) & 3];
    *buf++ = bbcw[p & 3];
    
    ch = (uint32_t)*cl++;
    p = exchr[(ch << 3) + yb];
    
    *buf++ = bbcw[(p >> 6) & 3];
    *buf++ = bbcw[(p >> 4) & 3];
    *buf++ = bbcw[(p >> 2) & 3];
    *buf++ = bbcw[p & 3];
  }
  buf = single_color_run(buf, 64, bbcw[0]);
  buf = end_run(buf);
  
  scanline_buffer->data_used = buf - scanline_buffer->data;
  scanline_buffer->status = SCANLINE_OK;  
}
