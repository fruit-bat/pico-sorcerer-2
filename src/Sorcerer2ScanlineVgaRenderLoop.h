#pragma once
#include "pico/stdlib.h"

void Sorcerer2ScanlineVgaRenderLoopInit();

void Sorcerer2ScanlineVgaRenderLoop(
    uint8_t* chars,
    uint8_t* exchr,
    volatile uint &frames,
    bool &showMenu,
    bool &toggleMenu);

extern void Sorcerer2ScanlineVgaRenderLoopCallbackLine(uint32_t y);
extern void Sorcerer2ScanlineVgaRenderLoopCallbackMenu(bool state);
