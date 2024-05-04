#include "Sorcerer2Audio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

#ifndef INITIAL_VOL
#define INITIAL_VOL 0x100
#endif
static uint32_t _vol = INITIAL_VOL;

#if !defined(PICO_HDMI_AUDIO) && !defined(PICO_AUDIO_I2S)
#include "pico/time.h"
// PWM Audio stuff
#define ZX_AUDIO_BUF_SIZE_BITS 7
#define ZX_AUDIO_BUF_SIZE (1<<ZX_AUDIO_BUF_SIZE_BITS)
#define ZX_AUDIO_BUF_MOD_MASK (ZX_AUDIO_BUF_SIZE-1)

// Sample for delayed deliver to PWM output
typedef struct {
  uint16_t vA; 
} es_audio_sample_t;

typedef struct {
  es_audio_sample_t sample[ZX_AUDIO_BUF_SIZE];
  uint32_t ri;
  uint32_t wi;
} es_audio_buf_t;

void es_audio_buf_init(es_audio_buf_t *b) {
  b->ri = 0;
  b->wi = 0;
}

inline bool es_audio_buf_ready_for_write(es_audio_buf_t *b) {
  return ((b->ri - b->wi) & ZX_AUDIO_BUF_MOD_MASK) != 1; 
}

inline bool es_audio_buf_ready_for_read(es_audio_buf_t *b) {
  return b->ri != b->wi; 
}

inline void es_audio_buf_read_next(es_audio_buf_t *b) {
  b->ri = (b->ri + 1) & ZX_AUDIO_BUF_MOD_MASK;  
}

inline void es_audio_buf_write_next(es_audio_buf_t *b) {
  b->wi = (b->wi + 1) & ZX_AUDIO_BUF_MOD_MASK;  
}

inline es_audio_sample_t *es_audio_buf_read_ptr(es_audio_buf_t *b) {
  return &b->sample[b->ri];
}

inline es_audio_sample_t *es_audio_buf_write_ptr(es_audio_buf_t *b) {
  return &b->sample[b->wi];
}

static es_audio_buf_t es_audio_buf;
static struct repeating_timer timer;
bool repeating_timer_callback(struct repeating_timer *timer)
{
  static es_audio_sample_t last;
  uint32_t vA;
  
  if (es_audio_buf_ready_for_read(&es_audio_buf))
  {
    es_audio_sample_t *buf = es_audio_buf_read_ptr(&es_audio_buf);
    last = *buf;
    vA = buf->vA;
    es_audio_buf_read_next(&es_audio_buf);
  }
  else {
    vA = last.vA;
  }
#ifdef SPK_PIN
  pwm_set_gpio_level(SPK_PIN, vA);
#endif
  return true;
}

#define PWM_WRAP (255)

static void init_pwm_pin(uint32_t pin) { 
  gpio_set_function(pin, GPIO_FUNC_PWM);
  const int audio_pin_slice = pwm_gpio_to_slice_num(pin);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1.0f); 
  pwm_config_set_wrap(&config, PWM_WRAP);
  pwm_init(audio_pin_slice, &config, true);
}

static void init_audio_output_timer() {
  es_audio_buf_init(&es_audio_buf);
  add_repeating_timer_us(-(1000000/PICO_PWM_AUDIO_FREQ), repeating_timer_callback, NULL, &timer);
}
#endif
// END PWM buffer stuff

#ifdef PICO_AUDIO_I2S
#include "audio_i2s.pio.h"
#include "hardware/pio.h"
#include "audio_i2s.pio.h"

static void update_pio_frequency(uint32_t sample_freq, PIO audio_pio, uint pio_sm) {
  uint32_t system_clock_frequency = clock_get_hz(clk_sys);
  assert(system_clock_frequency < 0x40000000);
  uint32_t divider = system_clock_frequency * 4 / (sample_freq * 3); // avoid arithmetic overflow
  assert(divider < 0x1000000);
  pio_sm_set_clkdiv_int_frac(audio_pio, pio_sm, divider >> 8u, divider & 0xffu);
}

static void init_is2_audio() {
  gpio_set_function(PICO_AUDIO_I2S_DATA, PICO_AUDIO_I2S_PIO_FUNC);
  gpio_set_function(PICO_AUDIO_I2S_BCLK, PICO_AUDIO_I2S_PIO_FUNC);
  gpio_set_function(PICO_AUDIO_I2S_BCLK + 1, PICO_AUDIO_I2S_PIO_FUNC);
  uint offset = pio_add_program(PICO_AUDIO_I2S_PIO, &audio_i2s_program);
  audio_i2s_program_init(PICO_AUDIO_I2S_PIO, PICO_AUDIO_I2S_SM, offset, PICO_AUDIO_I2S_DATA, PICO_AUDIO_I2S_BCLK);
  update_pio_frequency(PICO_I2S_AUDIO_FREQ, PICO_AUDIO_I2S_PIO, PICO_AUDIO_I2S_SM);
  pio_sm_set_enabled(PICO_AUDIO_I2S_PIO, PICO_AUDIO_I2S_SM, true);
}

inline bool is2_audio_ready() {
  return !pio_sm_is_tx_fifo_full(PICO_AUDIO_I2S_PIO, PICO_AUDIO_I2S_SM);
}

inline void is2_audio_put(uint32_t x) {
  *(volatile uint32_t*)&PICO_AUDIO_I2S_PIO->txf[PICO_AUDIO_I2S_SM] = x;
}
#endif

#ifdef PICO_HDMI_AUDIO
#include "dvi.h"

extern struct dvi_inst dvi0;

#define AUDIO_BUFFER_SIZE   256
audio_sample_t      audio_buffer[AUDIO_BUFFER_SIZE];

#if (PICO_HDMI_AUDIO_FREQ == 32000)
#define HDMI_N     4096     // From HDMI standard for 32kHz
#elif (PICO_HDMI_AUDIO_FREQ == 44100)
#define HDMI_N     6272     // From HDMI standard for 44.1kHz
#else
#define HDMI_N     6144     // From HDMI standard for 48kHz
#endif

static void init_hdmi_audio() {
  dvi_get_blank_settings(&dvi0)->top    = 0;
  dvi_get_blank_settings(&dvi0)->bottom = 0;
  dvi_audio_sample_buffer_set(&dvi0, audio_buffer, AUDIO_BUFFER_SIZE);
  dvi_set_audio_freq(
    &dvi0, 
    PICO_HDMI_AUDIO_FREQ, 
    dvi0.timing->bit_clk_khz*HDMI_N/(PICO_HDMI_AUDIO_FREQ/100)/128,
    HDMI_N
  );
  increase_write_pointer(&dvi0.audio_ring, get_write_size(&dvi0.audio_ring, true));
}
#endif

#ifdef EAR_PIN
#include "es_ear_in.pio.h"

static  PIO ear_pio = pio1;
static  uint ear_sm = 0;

static void init_ear_in() {
  uint offset = pio_add_program(ear_pio, &es_ear_in_program);
  ear_sm = pio_claim_unused_sm(ear_pio, true);
  es_ear_in_program_init(ear_pio, ear_sm, offset, EAR_PIN, 1000000);
}
#endif

uint32_t __not_in_flash_func(sorcerer2ReadEar)() {
#ifdef EAR_PIN
    return ear_pio->rxf[ear_sm];
#else
    return 0;
#endif
}

bool __not_in_flash_func(sorcerer2EarReady)() {
#ifdef EAR_PIN
    return (ear_pio->fstat & (1u << (PIO_FSTAT_RXEMPTY_LSB + ear_sm))) == 0;
#else
    return true;
#endif
}

void sorcerer2AudioInit() {

#if defined(PICO_AUDIO_I2S)
  init_is2_audio();
#elif defined(PICO_HDMI_AUDIO)
  init_hdmi_audio();
#else  
  init_audio_output_timer();
  #ifdef SPK_PIN
    init_pwm_pin(SPK_PIN);
  #endif
#endif

#ifdef EAR_PIN
  init_ear_in();
#endif
}

void __not_in_flash_func(sorcerer2AudioHandler)(uint32_t vA, bool mute) {
#if defined(PICO_HDMI_AUDIO) || defined(PICO_AUDIO_I2S)
  uint32_t ll, rr;
  if (mute) {
    ll = rr = 0;
  }
  else {
    const int32_t l = (int32_t)(vA << 8);
    const int32_t v = (l - (1 << (8 + 6 - 1))) & 0xffff;
    ll = (__mul_instruction(_vol, v) >> 8) & 0xffff;
    rr = (__mul_instruction(_vol, v) >> 8) & 0xffff;
  }
#if defined(PICO_HDMI_AUDIO)
  audio_sample_t *audio_ptr = get_write_pointer(&dvi0.audio_ring);
	audio_ptr->channels[0] = ll;
	audio_ptr->channels[1] = rr;
	increase_write_pointer(&dvi0.audio_ring, 1);
#if defined(PICO_AUDIO_I2S)
  if (is2_audio_ready()) {
    is2_audio_put((ll << 16) | rr);
  }  
#endif
#elif defined(PICO_AUDIO_I2S)
  is2_audio_put((ll << 16) | rr);
#endif
#else
  es_audio_sample_t *buf = es_audio_buf_write_ptr(&es_audio_buf);
  if (mute) {
    buf->vA = 0;
  }
  else {
    buf->vA = vA;
  }
  es_audio_buf_write_next(&es_audio_buf);
#endif
}

uint32_t sorcerer2AudioGetVolume() { return _vol; }

void sorcerer2AudioSetVolume(uint32_t vol) { _vol = vol; }

bool __not_in_flash_func(sorcerer2AudioReady)() {
#if defined(PICO_HDMI_AUDIO)
  return get_write_size(&dvi0.audio_ring, true) > 0;
#elif defined(PICO_AUDIO_I2S)
  return is2_audio_ready();
#else
  return es_audio_buf_ready_for_write(&es_audio_buf);
#endif
}

