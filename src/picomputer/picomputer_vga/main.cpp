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
#include "pico/sem.h"
#include "ps2kbd.h"
#include "Sorcerer2ScanlineVgaRenderLoop.h"

#include "Sorcerer2.h"
#include "Sorcerer2HidKeyboard.h"
#include "Sorcerer2DiskFatFsSpi.h"
#include "Sorcerer2TapeUnit.h"
#include "Sorcerer2Menu.h"

#include "PicoCharRendererVga.h"
#include "PicoWinHidKeyboard.h"
#include "PicoDisplay.h"

#include "bsp/board.h"
#include "tusb.h"
#include <pico/printf.h>
#include "PicoCharRenderer.h"
#include "Sorcerer2Audio.h"
#include "hid_app.h"

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

#ifndef LED_PIN
#define LED_PIN 25
#endif
#ifndef SPK_PIN
#define SPK_PIN 9
#endif
// Very loud!
// #define PWM_WRAP 63 
#define PWM_WRAP 256

struct semaphore dvi_start_sem;
static uint8_t* charbuf;
static uint8_t* exchr;

static bool showMenu = false;
static bool toggleMenu = false;

// Menu handler
static volatile uint _frames = 0;

void __not_in_flash_func(Sorcerer2ScanlineVgaRenderLoopCallbackLine)(uint32_t y) {

}

void __not_in_flash_func(Sorcerer2ScanlineVgaRenderLoopCallbackMenu)(bool state) {

}

void core1_main() {
  sem_acquire_blocking(&dvi_start_sem);
  printf("Core 1 running...\n");

  Sorcerer2ScanlineVgaRenderLoop(
    charbuf,
    exchr,
    _frames,
    showMenu,
    toggleMenu
  );

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

extern "C" void __not_in_flash_func(process_mouse_report)(hid_mouse_report_t const * report) {
}

extern "C"  void __not_in_flash_func(process_mouse_mount)(uint8_t dev_addr, uint8_t instance) {
}

extern "C"  void __not_in_flash_func(process_mouse_unmount)(uint8_t dev_addr, uint8_t instance) {
}

extern "C"  void __not_in_flash_func(process_kbd_mount)(uint8_t dev_addr, uint8_t instance) {
	sorcerer2HidKeyboard.mount();
}

extern "C"  void __not_in_flash_func(process_kbd_unmount)(uint8_t dev_addr, uint8_t instance) {
	sorcerer2HidKeyboard.unmount();
}

extern "C"  void process_kbd_report(hid_keyboard_report_t const *report, hid_keyboard_report_t const *prev_report) {
  int r;
  if (showMenu) {
    r = picoWinHidKeyboard.processHidReport(report, prev_report);
  }
  else {
    r = sorcerer2HidKeyboard.processHidReport(report, prev_report);
  }
  if (r == 1) {
    toggleMenu = true;
    picoWinHidKeyboard.cancelRepeat();
  } 
}

#ifdef USE_PS2_KBD
static Ps2Kbd ps2kbd(
  pio1,
  6,
  process_kbd_report
);
#endif

void __not_in_flash_func(main_loop)() {
  uint frames = 0;

  while (1) {
    tuh_task();
#ifdef USE_PS2_KBD
    ps2kbd.tick();
#endif
    if (!showMenu) {
      for(int i=0; i < 100; ++i) {
        sorcerer2.stepCpu();
      }
      sorcerer2.stepDisk();
    }
    else {
      picoWinHidKeyboard.processKeyRepeat();
      if (frames != _frames) {
        frames = _frames;
        picoDisplay.refresh();
      }
    }
  }

  __builtin_unreachable();
}

extern "C" int main() {
  
  vreg_set_voltage(VREG_VSEL);
  sleep_ms(10);

  Sorcerer2ScanlineVgaRenderLoopInit();

  //Initialise I/O
  stdio_init_all();
     
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  tuh_hid_app_startup();
#ifdef USE_PS2_KBD
  ps2kbd.init_gpio();
#endif

  // Initialise the audio
  sorcerer2AudioInit();

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
  
  printf("Core 1 start\n");
  sem_init(&dvi_start_sem, 0, 1);

  multicore_launch_core1(core1_main);

  sem_release(&dvi_start_sem);

  sorcerer2.reset();

  main_loop();

  __builtin_unreachable();
}
