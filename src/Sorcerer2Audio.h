#pragma once

#include "Sorcerer2.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
//
// PICO_AUDIO_I2S I2S audio
//
// SPK_PIN PWM audio
//
// See CMakeLists.txt files for configurations
//
#ifdef PICO_AUDIO_I2S
#include "hardware/pio.h"

inline bool is2_audio_ready() {
  return !pio_sm_is_tx_fifo_full(PICO_AUDIO_I2S_PIO, PICO_AUDIO_I2S_SM);
}

inline void is2_audio_put(uint32_t x) {
  *(volatile uint32_t*)&PICO_AUDIO_I2S_PIO->txf[PICO_AUDIO_I2S_SM] = x;
}
#endif

void Sorcerer2AudioInit();

inline bool Sorcerer2AudioReady() {
#ifdef PICO_AUDIO_I2S
  return is2_audio_ready();
#else
  return true;
#endif
}

inline void Sorcerer2AudioToGpio(Sorcerer2& sorcerer2) {
#ifdef PICO_AUDIO_I2S
  const int32_t l = (int32_t)(sorcerer2.getSound() << 8);
  const int32_t v = (l - (1 << (8 + 6 - 1))) & 0xffff;
  is2_audio_put((v << 16) | v);
#else
  #ifdef SPK_PIN
    const uint32_t l = sorcerer2.getSound();    
    pwm_set_gpio_level(SPK_PIN, l);
  #endif
#endif
}

