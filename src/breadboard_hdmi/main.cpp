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
#include "hardware/pwm.h"
#include "ps2kbd.h"
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
#include "Sorcerer2TapeUnit.h"
#include "Sorcerer2Menu.h"

#include "PicoWinHidKeyboard.h"
#include "PicoDisplay.h"

#include "bsp/board.h"
#include "tusb.h"
#include <pico/printf.h>
#include "PicoCharRenderer.h"
#include "Sorcerer2Audio.h"

#define UART_ID uart0
#define BAUD_RATE 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define UART_TX_PIN 0
#define UART_RX_PIN 1

// DVDD 1.2V (1.1V seems ok too)
#define FRAME_WIDTH 640
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

struct dvi_inst dvi0;
struct semaphore dvi_start_sem;

#define CHAR_COLS 64
#define CHAR_ROWS 30

static uint8_t* charbuf;
static uint8_t* exchr;

static bool showMenu = false;
static bool toggleMenu = false;

// Menu handler
static volatile uint _frames = 0;

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

void __not_in_flash_func(core1_scanline_callback)(unsigned int) {
  static uint y = 1;
  static uint ys = 0;
  uint rs = showMenu ? pcw_prepare_scanline_80(&dvi0, y, ys, _frames) : prepare_scanline_64(charbuf, y, ys);
  if (0 == (++y & 7)) {
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

  prepare_scanline_64(charbuf, 0, 0);
  // The text display is completely IRQ driven (takes up around 30% of cycles @
  // VGA). We could do something useful, or we could just take a nice nap
  while (1) 
    __wfi();
  __builtin_unreachable();
}

static SdCardFatFsSpi sdCard0(0);
static Sorcerer2TapeUnit tapeUnit1;
static Sorcerer2TapeUnit tapeUnit2;

static Sorcerer2DiskSystem sorcerer2DiskSystem;
static Sorcerer2HidKeyboard sorcerer2HidKeyboard;
static Sorcerer2 sorcerer2(
  &sorcerer2HidKeyboard,
  &sorcerer2DiskSystem
);
static Sorcerer2Menu picoRootWin(&sdCard0, &sorcerer2);
static PicoDisplay picoDisplay(pcw_screen(), &picoRootWin);
static PicoWinHidKeyboard picoWinHidKeyboard(&picoDisplay);

void print(hid_keyboard_report_t const *report) {
	printf("HID key report modifiers %2.2X report ", report->modifier);
	for(int i = 0; i < 6; ++i) printf("%2.2X", report->keycode[i]);
	printf("\n");
}

extern "C"  void __not_in_flash_func(process_kbd_mount)(uint8_t dev_addr, uint8_t instance) {
	sorcerer2HidKeyboard.mount();
}

extern "C"  void __not_in_flash_func(process_kbd_unmount)(uint8_t dev_addr, uint8_t instance) {
	sorcerer2HidKeyboard.unmount();
}

extern "C"  void process_kbd_report(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
#if 0
  // Some help debugging keyboard input/drivers
	printf("PREV ");print(prev_report);
	printf("CURR ");print(report);
#endif

  int r;
  if (showMenu) {
    r = picoWinHidKeyboard.processHidReport(report, prev_report);
  }
  else {
    r = sorcerer2HidKeyboard.processHidReport(report, prev_report);
  }
  if (r == 1) toggleMenu = true;
}

static Ps2Kbd ps2kbd(
  pio1,
  6,
  process_kbd_report
);

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
  sleep_ms(1000);
  
  printf("Starting TinyUSB\n");
  tusb_init();
  ps2kbd.init_gpio();

  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  // Initialise the audio
  Sorcerer2AudioInit();

  // Initialise the menu renderer
  pcw_init_renderer();
  
  charbuf = sorcerer2.screenPtr();
  exchr = sorcerer2.charsPtr();
  sorcerer2DiskSystem.drive(0)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "/sorcerer2/disks/", "diskA.dsk"));
  sorcerer2DiskSystem.drive(1)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "/sorcerer2/disks/", "diskB.dsk"));
  sorcerer2DiskSystem.drive(2)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "/sorcerer2/disks/", "diskC.dsk"));
  sorcerer2DiskSystem.drive(3)->insert(new Sorcerer2DiskFatFsSpi(&sdCard0, "/sorcerer2/disks/", "diskD.dsk"));
  sorcerer2.tapeSystem()->attach(0, &tapeUnit1);
  sorcerer2.tapeSystem()->attach(1, &tapeUnit2);
  sorcerer2HidKeyboard.setSorcerer2(&sorcerer2);
  
  printf("Configuring DVI\n");

  dvi0.timing = &DVI_TIMING;
  dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
  dvi0.scanline_callback = core1_scanline_callback;
  dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

  printf("Prepare first scanline\n");


  printf("Core 1 start\n");
  sem_init(&dvi_start_sem, 0, 1);
  hw_set_bits(&bus_ctrl_hw->priority, BUSCTRL_BUS_PRIORITY_PROC1_BITS);
  multicore_launch_core1(core1_main);

  sem_release(&dvi_start_sem);

  sorcerer2.reset();

  uint frames = 0;
  while (1) {
    tuh_task();
    ps2kbd.tick();
    if (!showMenu) {
      for(int i=0; i < 100; ++i) {
        sorcerer2.stepCpu();
        if (Sorcerer2AudioReady()) {
          Sorcerer2AudioToGpio(sorcerer2);
        }
      }
      sorcerer2.stepDisk();
    }
    else if (frames != _frames) {
      frames = _frames;
      picoDisplay.refresh();
    }
  }
  __builtin_unreachable();
}
