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
#include "vga.h"
#include "Sorcerer2PrepareRgbScanline.h"

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

#define LED_PIN 25
#define SPK_PIN 9
// Very loud!
// #define PWM_WRAP 63 
#define PWM_WRAP 256

struct semaphore dvi_start_sem;
static const sVmode* vmode = NULL;

static uint8_t* charbuf;
static uint8_t* exchr;

static bool showMenu = false;
static bool toggleMenu = false;

// Menu handler
static volatile uint _frames = 0;

void core1_main() {
  sem_acquire_blocking(&dvi_start_sem);
  printf("Core 1 running...\n");

  // TODO fetch the resolution from the mode ?
  VgaInit(vmode,640,480);

  while (1) {

    VgaLineBuf *linebuf = get_vga_line();
    uint32_t* buf = (uint32_t*)&(linebuf->line);
    uint32_t y = linebuf->row;
    if (showMenu) {
      pcw_prepare_vga332_scanline_80(
        buf,
        y,
        linebuf->frame);
    }
    else {
      prepare_rgb_scanline(
        buf, 
        y, 
        charbuf,
        exchr
      );
    }
      
//    pzx_keyscan_row();
    
    if (y == 239) { // TODO use a const / get from vmode
           
      if (toggleMenu) {
        showMenu = !showMenu;
        toggleMenu = false;
//        picomputerJoystick.enabled(!showMenu);
      }
      
      _frames = linebuf->frame;
    }    
  }
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
  if (r == 1) toggleMenu = true;
}

void __not_in_flash_func(main_loop)(){
  // TODO
}

extern "C" int __not_in_flash_func(main)() {
  vreg_set_voltage(VREG_VSEL);
  sleep_ms(10);
  vmode = Video(DEV_VGA, RES_HVGA);
  sleep_ms(100);

  //Initialise I/O
  stdio_init_all();
     
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);

  // Don't use the same DMA channel as the screen  
  set_spi_dma_irq_channel(true, true);

  tusb_init();

  gpio_set_function(SPK_PIN, GPIO_FUNC_PWM);
  const int audio_pin_slice = pwm_gpio_to_slice_num(SPK_PIN);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1.0f); 
  pwm_config_set_wrap(&config, PWM_WRAP);
  pwm_init(audio_pin_slice, &config, true);

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

  uint frames = 0;
  while (1) {
    tuh_task();
    for(int i=0; i < 100; ++i) {
      sorcerer2.stepCpu();
      const uint32_t l = sorcerer2.getSound();    
      pwm_set_gpio_level(SPK_PIN, l);
    }
    sorcerer2.stepDisk();
    if (showMenu && frames != _frames) {
      frames = _frames;
      picoDisplay.refresh();
    }
  }
  __builtin_unreachable();
  
// TODO   main_loop();
}
