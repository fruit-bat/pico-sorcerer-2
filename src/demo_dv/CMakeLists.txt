
set(sorcerer2_demo_dv_src
  main.cpp
)

add_executable(sorcerer2_demo_dv
  ${sorcerer2_common_src}
  ${sorcerer2_demo_dv_src}
)

target_compile_definitions(sorcerer2_demo_dv PRIVATE
  DVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg
  DVI_VERTICAL_REPEAT=2
  DVI_N_TMDS_BUFFERS=3
  DVI_MONOCHROME_TMDS=1
  DVI_1BPP_BIT_REVERSE=1
  PCS_COLS=80
  PCS_ROWS=30
  LED_PIN=25
  FF_USE_FIND=1
  SDCARD_PIO=pio1
  SDCARD_PIO_SM=1
  SDCARD_PIN_SPI0_CS=22
  SDCARD_PIN_SPI0_SCK=5
  SDCARD_PIN_SPI0_MOSI=18
  SDCARD_PIN_SPI0_MISO=19

#  # compile time configuration of I2S
#  PICO_AUDIO_I2S_MONO_INPUT=1
#  #define for our example code
#  USE_AUDIO_I2S=1
#  PICO_AUDIO_I2S_DMA_IRQ=1
#  PICO_AUDIO_I2S_PIO=1
#  PICO_AUDIO_I2S_DATA=26
#  PICO_AUDIO_I2S_BCLK=27
)

set(sorcerer2_demo_dv_libs
  libdvi
  tinyusb_host
  tinyusb_board
  pico_emu_utils
  pico_dvi_menu
# pico_audio_i2s
)

target_link_libraries(sorcerer2_demo_dv
  ${sorcerer2_common_libs}
  ${sorcerer2_demo_dv_libs}
)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_demo_dv)
