#include "Sorcerer2ScanlineVgaRenderLoop.h"
#include "PicoCharRendererVga.h"
#include "PicoCharRendererScanvideo.h"
#include "hardware/clocks.h"
#include "Sorcerer2PrepareScanvideoScanline.h"

#ifdef VGA_VSYNC
#define VGA_H_SYNC_POLARITY 1
#else
#define VGA_H_SYNC_POLARITY 5
#endif

const scanvideo_timing_t vga_timing_640x480_60 =
{
    .clock_freq = 25000000,
    .h_active = 640,
    .v_active = 480,

    .h_front_porch = 16,
    .h_pulse = 96,
    .h_total = 800,
    .h_sync_polarity = VGA_H_SYNC_POLARITY, // 1=HSYNC & VSYNC, 5=CSYNC
    .v_front_porch = 10,
    .v_pulse = 2,
    .v_total = 525,
    .v_sync_polarity = 1,

    .enable_clock = 0,
    .clock_polarity = 0,

    .enable_den = 0
};

const scanvideo_mode_t vga_mode_640x240_60 =
{
  .default_timing = &vga_timing_640x480_60,
  .pio_program = &video_24mhz_composable,
  .width = 640,
  .height = 240,
  .xscale = 1,
  .yscale = 2,
};

#define VGA_MODE vga_mode_640x240_60

void Sorcerer2ScanlineVgaRenderLoopInit()
{
  set_sys_clock_khz(VGA_MODE.default_timing->clock_freq / 100, true);
  sleep_ms(10);
}

void __not_in_flash_func(Sorcerer2ScanlineVgaRenderLoop)(
  uint8_t* chars,
  uint8_t* exchr,
  volatile uint &frames,
  bool &showMenu,
  bool &toggleMenu
) {
  scanvideo_setup(&VGA_MODE);
  scanvideo_timing_enable(true);

  while (true) {
    struct scanvideo_scanline_buffer *scanline_buffer = scanvideo_begin_scanline_generation(true);
    uint32_t frame_num = scanvideo_frame_number(scanline_buffer->scanline_id);
    uint32_t y = scanvideo_scanline_number(scanline_buffer->scanline_id);

    if (y == 0) {
           
      if (toggleMenu) {
        showMenu = !showMenu;
        toggleMenu = false;
        Sorcerer2ScanlineVgaRenderLoopCallbackMenu(showMenu);
      }
      
      frames = frame_num;
    }

    if (showMenu) {
      pcw_prepare_scanvideo_scanline_80(
        scanline_buffer,
        y,
        frame_num);
    }
    else { 
      sorcerer2_prepare_scanvideo_scanline(
        scanline_buffer, 
        y, 
        frame_num,
        chars,
        exchr
      );
    }

    // Release the rendered buffer into the wild
    scanvideo_end_scanline_generation(scanline_buffer);

    Sorcerer2ScanlineVgaRenderLoopCallbackLine(y);
  }

  __builtin_unreachable();
}
