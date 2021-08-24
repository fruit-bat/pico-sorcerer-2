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
#include "pico/sem.h"
extern "C" {
#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "tmds_encode.h"
}
#include "Sorcerer2.h"
#include "Sorcerer2HidKeyboard.h"
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
#define LED_PIN 16

struct dvi_inst dvi0;
struct semaphore dvi_start_sem;

#define CHAR_COLS 64
#define CHAR_ROWS 30

unsigned char* charbuf;
unsigned char* exchr;

extern "C" void hid_app_task();
extern void testsd();

// Screen handler

static inline unsigned char bit_reverse(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

static inline void prepare_scanline(const unsigned char *chars, uint y) {
	static uint8_t scanbuf[FRAME_WIDTH / 8];
	for (uint i = 0; i < CHAR_COLS; ++i) {
		uint c = chars[i + ((y >> 3) << 6)];
		scanbuf[i + 8] = exchr[(c << 3) + (y & 7)];
	}
	uint32_t *tmdsbuf;
	queue_remove_blocking(&dvi0.q_tmds_free, &tmdsbuf);
	tmds_encode_1bpp((const uint32_t*)scanbuf, tmdsbuf, FRAME_WIDTH);
	queue_add_blocking(&dvi0.q_tmds_valid, &tmdsbuf);
}

void core1_scanline_callback() {
	static uint y = 1;
	prepare_scanline(charbuf, y);
	y = (y + 1) % FRAME_HEIGHT;
}

void __not_in_flash("main") core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_1);
	sem_acquire_blocking(&dvi_start_sem);

	dvi_start(&dvi0);

	// The text display is completely IRQ driven (takes up around 30% of cycles @
	// VGA). We could do something useful, or we could just take a nice nap
	while (1) 
		__wfi();
	__builtin_unreachable();
}

static Sorcerer2HidKeyboard sorcerer2HidKeyboard;
static Sorcerer2 sorcerer2(&sorcerer2HidKeyboard);

extern "C" void printAt(unsigned int x, unsigned int y, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[128];
  vsnprintf(buf, 128, fmt, args );
  sorcerer2.printAt(x, y, buf);
  va_end(args);
}

extern "C" void sorcerer_keyboard_handler(hid_keyboard_report_t const *report) {
  sorcerer2HidKeyboard.processHidReport(report);
}

extern "C" int __not_in_flash("main") main() {
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

	printf("Configuring DVI\n");

	charbuf = sorcerer2.screenPtr();
	exchr = sorcerer2.charsPtr();

	dvi0.timing = &DVI_TIMING;
	dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
	dvi0.scanline_callback = core1_scanline_callback;
	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

	printf("Prepare first scanline\n");

	prepare_scanline(charbuf, 0);

	printf("Core 1 start\n");
	sem_init(&dvi_start_sem, 0, 1);
	hw_set_bits(&bus_ctrl_hw->priority, BUSCTRL_BUS_PRIORITY_PROC1_BITS);
	multicore_launch_core1(core1_main);


	sem_release(&dvi_start_sem);

	sorcerer2.reset(0xE000);
//	unsigned int a = 0;

	testsd();

	while (1) {
		tuh_task();
		sorcerer2.step();
		hid_app_task();
//		printAt(15,15, "%d", a);
//		a = (a + 1) & 0xff;
	}
	__builtin_unreachable();
}
