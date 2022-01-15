#include "PicoCharRenderer.h"
#include "PicoFontCushion.h"

extern "C" {
#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "tmds_encode.h"
}

static uint16_t charScreen[PCS_COLS * PCS_ROWS];
static uint8_t charFont[256 * 8];
static PicoCharScreen picoCharScreen(charScreen, PCS_COLS, PCS_ROWS);

uint pcw_prepare_scanline_80(struct dvi_inst *dvi0, const uint y, const uint ys, const uint frames) {
  static uint8_t scanbuf[PCS_COLS];
  const uint16_t m = (frames >> 5) & 1;
  const uint cr = y & 7;
  for (uint i = 0; i < PCS_COLS; ++i) {
    const uint16_t ca = charScreen[i + ys];
    const uint8_t cf = charFont[((ca & 0xff) << 3) + cr];
    // bit 8 inverse video
    // bit 9 flash
    const uint16_t z = ((ca >> 8) ^ ((ca >> 9) & m)) & 1; 
    scanbuf[i] = cf ^ __mul_instruction(z, 0xff);
  }
  uint32_t *tmdsbuf;
  queue_remove_blocking(&dvi0->q_tmds_free, &tmdsbuf);
  tmds_encode_1bpp((const uint32_t*)scanbuf, tmdsbuf, PCS_FRAME_WIDTH);
  queue_add_blocking(&dvi0->q_tmds_valid, &tmdsbuf);
  return PCS_COLS;
}

void pcw_init_renderer() {
  memcpy(&charFont[32*8], PicoFontCushion, sizeof(PicoFontCushion));
}

PicoCharScreen *pcw_screen() {
  return &picoCharScreen;
}
