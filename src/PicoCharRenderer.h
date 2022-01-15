#pragma once

#ifndef PCS_COLS
#define PCS_COLS 80
#endif

#ifndef PCS_ROWS
#define PCS_ROWS 30
#endif

#ifndef PCS_FRAME_WIDTH
#define PCS_FRAME_WIDTH (8 * PCS_COLS)
#endif

#ifndef PCS_FRAME_HEIGHT
#define PCS_FRAME_HEIGHT (8 * PCS_ROWS)
#endif

// TODO Need a common head to obtain types from
#include "tusb.h"
#include "PicoCharScreen.h"

extern "C" { 
  uint __not_in_flash_func(pcw_prepare_scanline_80)(struct dvi_inst *dvi0, const uint y, const uint ys, const uint frames);
  void pcw_init_renderer();
}

PicoCharScreen *pcw_screen();

