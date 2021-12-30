#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/structs/bus_ctrl.h"
#include "hardware/structs/ssi.h"
#include "hardware/dma.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"  // pwm 
#include "pico/sem.h"
extern "C" {
#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "tmds_encode.h"
}
#include "Sorcerer2.h"
#include "Sorcerer2HidKeyboard.h"
#include "Sorcerer2DiskFatFsSpi.h"
#include "Sorcerer2TapeUnitFatFsSpi.h"
#include "Sorcerer2Menu.h"

#include "MagneticFont.h"
#include "PicoWinHidKeyboard.h"
#include "PicoDisplay.h"

#include "bsp/board.h"
#include "tusb.h"
#include <pico/printf.h>

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

// Should be 25 for pico ?
// #define LED_PIN 16
#define SPK_PIN 20
#define PWM_WRAP (512)
#define PWM_MID (PWM_WRAP>>1)

struct dvi_inst dvi0;
struct semaphore dvi_start_sem;

#define CHAR_COLS 64
#define CHAR_ROWS 30

static uint8_t* charbuf;
static uint8_t* exchr;

#define PCS_COLS 80
#define PCS_ROWS 30
static uint16_t charScreen[PCS_COLS * PCS_ROWS];
static uint8_t charFont[256 * 8];
static bool showMenu = true;
static bool toggleMenu = false;

// Menu handler
static volatile uint _frames = 0;
static uint __not_in_flash_func(prepare_scanline_80)(const uint16_t *chars, const uint y, const uint ys) {
  static uint8_t scanbuf[PCS_COLS];
	const uint16_t m = (_frames >> 5) & 1;
  const uint cr = y & 7;
  for (uint i = 0; i < PCS_COLS; ++i) {
    const uint16_t ca = chars[i + ys];
    const uint8_t cf = charFont[((ca & 0xff) << 3) + cr];
    // bit 8 inverse video
    // bit 9 flash
    const uint16_t z = ((ca >> 8) ^ ((ca >> 9) & m)) & 1; 
    scanbuf[i] = cf ^ __mul_instruction(z, 0xff);
  }
  uint32_t *tmdsbuf;
  queue_remove_blocking(&dvi0.q_tmds_free, &tmdsbuf);
  tmds_encode_1bpp((const uint32_t*)scanbuf, tmdsbuf, FRAME_WIDTH);
  queue_add_blocking(&dvi0.q_tmds_valid, &tmdsbuf);
  return PCS_COLS;
}

// Screen handler
static uint __not_in_flash_func(prepare_scanline_64)(const uint8_t *chars, const uint y, const uint ys) {
  static uint8_t scanbuf[FRAME_WIDTH / 8];

  const uint cr = y & 7;
  for (uint i = 0; i < CHAR_COLS; ++i) {
    scanbuf[i + 8] = exchr[(chars[i + ys] << 3) + cr];
  }
  uint32_t *tmdsbuf;
  queue_remove_blocking(&dvi0.q_tmds_free, &tmdsbuf);
  tmds_encode_1bpp((const uint32_t*)scanbuf, tmdsbuf, FRAME_WIDTH);
  queue_add_blocking(&dvi0.q_tmds_valid, &tmdsbuf);
  return CHAR_COLS;
}

void __not_in_flash_func(core1_scanline_callback)() {
  static uint y = 1;
  static uint ys = 0;
  uint rs = showMenu ? prepare_scanline_80((const uint16_t *)&charScreen, y++, ys) : prepare_scanline_64(charbuf, y++, ys);
  if (0 == (y & 7)) {
    ys += rs;
  }
  if (y == FRAME_HEIGHT) {
    _frames++;
    y = 0;
    ys = 0;
    if (toggleMenu) {
      showMenu = !showMenu;
      toggleMenu = false;
    }
  }
}

void core1_main() {
  dvi_register_irqs_this_core(&dvi0, DMA_IRQ_1);
  sem_acquire_blocking(&dvi_start_sem);

  dvi_start(&dvi0);

  // The text display is completely IRQ driven (takes up around 30% of cycles @
  // VGA). We could do something useful, or we could just take a nice nap
  while (1) 
    __wfi();
  __builtin_unreachable();
}

static SdCardFatFsSpi sdCard0(0);
static Sorcerer2TapeUnitFatFsSpi tapeUnit0(&sdCard0, "tapes");
static Sorcerer2DiskSystem sorcerer2DiskSystem;
static Sorcerer2HidKeyboard sorcerer2HidKeyboard;
static Sorcerer2 sorcerer2(
  &sorcerer2HidKeyboard,
  &sorcerer2DiskSystem
);
static PicoCharScreen picoCharScreen((uint16_t *)&charScreen, PCS_COLS, PCS_ROWS);
static Sorcerer2Menu picoRootWin(&sdCard0, &sorcerer2);
static PicoDisplay picoDisplay(&picoCharScreen, &picoRootWin);
static PicoWinHidKeyboard picoWinHidKeyboard(&picoDisplay);

extern "C"  void process_kbd_report(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
  int r;
  if (showMenu) {
    r = picoWinHidKeyboard.processHidReport(report, prev_report);
  }
  else {
    r = sorcerer2HidKeyboard.processHidReport(report, prev_report);
  }
  if (r == 1) toggleMenu = true;
}

extern "C" int __not_in_flash_func(main)() {
  vreg_set_voltage(VREG_VSEL);
  sleep_ms(10);
#ifdef RUN_FROM_CRYSTAL
  set_sys_clock_khz(12000, true);
#else
  // Run system at TMDS bit clock
  set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);
#endif

  setup_default_uart();
  tusb_init();
    
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  gpio_set_function(SPK_PIN, GPIO_FUNC_PWM);
  const int audio_pin_slice = pwm_gpio_to_slice_num(SPK_PIN);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1.0f); 
  pwm_config_set_wrap(&config, PWM_WRAP);
  pwm_init(audio_pin_slice, &config, true);

  memcpy(&charFont[32*8], MagneticFont, sizeof(MagneticFont));
  charbuf = sorcerer2.screenPtr();
  exchr = sorcerer2.charsPtr();
  sorcerer2DiskSystem.drive(0)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "diskA.dsk"));
  sorcerer2DiskSystem.drive(1)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "diskB.dsk"));
  sorcerer2DiskSystem.drive(2)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "diskC.dsk"));
  sorcerer2DiskSystem.drive(3)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "diskD.dsk"));
  sorcerer2.tapeSystem()->attach(0, &tapeUnit0);
  sorcerer2HidKeyboard.setSorcerer2(&sorcerer2);
  
  printf("Configuring DVI\n");

  dvi0.timing = &DVI_TIMING;
  dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
  dvi0.scanline_callback = core1_scanline_callback;
  dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

  printf("Prepare first scanline\n");

  prepare_scanline_64(charbuf, 0, 0);

  printf("Core 1 start\n");
  sem_init(&dvi_start_sem, 0, 1);
  hw_set_bits(&bus_ctrl_hw->priority, BUSCTRL_BUS_PRIORITY_PROC1_BITS);
  multicore_launch_core1(core1_main);


  sem_release(&dvi_start_sem);

  sorcerer2.reset();

  uint frames = 0;
  while (1) {
    tuh_task();
    for(int i=0; i < 100; ++i) {
      sorcerer2.stepCpu();
      const uint32_t l = sorcerer2.getCentronics() >> 2;    
      pwm_set_gpio_level(SPK_PIN, l);
    }
    sorcerer2.stepDisk();
    if (showMenu && frames != _frames) {
      frames = _frames;
      picoDisplay.refresh();
    }
  }
  __builtin_unreachable();
}
