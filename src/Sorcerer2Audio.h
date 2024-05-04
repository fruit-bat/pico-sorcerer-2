#pragma once

#include "hardware/gpio.h"
#include "hardware/pwm.h"

#ifndef PICO_PWM_AUDIO_FREQ
#define PICO_PWM_AUDIO_FREQ 28000
#endif
#ifndef PICO_HDMI_AUDIO_FREQ
#define PICO_HDMI_AUDIO_FREQ 44100
#endif
#ifndef PICO_I2S_AUDIO_FREQ
#define PICO_I2S_AUDIO_FREQ 44100
#endif
#if defined(PICO_HDMI_AUDIO)
#define PICO_AUDIO_OUT_FREQ PICO_HDMI_AUDIO_FREQ
#elif defined(PICO_AUDIO_I2S)
#define PICO_AUDIO_OUT_FREQ PICO_I2S_AUDIO_FREQ
#else
#define PICO_AUDIO_OUT_FREQ PICO_PWM_AUDIO_FREQ
#endif

void sorcerer2AudioInit();

void sorcerer2AudioHandler(uint32_t vA, bool mute);

uint32_t sorcerer2ReadEar();
bool sorcerer2EarReady();

uint32_t sorcerer2AudioGetVolume();

void sorcerer2AudioSetVolume(uint32_t vol);
bool sorcerer2AudioReady();
