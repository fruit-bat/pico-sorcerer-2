
set(sorcerer2_demo_dv_src
  main.cpp
)

add_executable(sorcerer2_demo_dv
  ${sorcerer2_common_src}
  ${sorcerer2_demo_dv_src}
)

pico_generate_pio_header(sorcerer2_demo_dv 
  ${sorcerer2_audio_i2s_pio}
)

target_compile_definitions(sorcerer2_demo_dv PRIVATE
  DVI_DEFAULT_SERIAL_CONFIG=pimoroni_demo_hdmi_cfg
  PICO_CORE1_STACK_SIZE=0x400
  DVI_VERTICAL_REPEAT=2
  DVI_N_TMDS_BUFFERS=3
  DVI_MONOCHROME_TMDS=1
  DVI_1BPP_BIT_REVERSE=1
  PCS_COLS=80
  PCS_ROWS=30
  LED_PIN=25
  # Tell FATFS we want to find files
  FF_USE_FIND=1
  # Configure the SPI sd card to use PIO
  SDCARD_PIO=pio1
  SDCARD_PIO_SM=1
  SDCARD_PIN_SPI0_CS=22
  SDCARD_PIN_SPI0_SCK=5
  SDCARD_PIN_SPI0_MOSI=18
  SDCARD_PIN_SPI0_MISO=19
  # Configure the I2S audio 
  PICO_AUDIO_I2S=1
  PICO_AUDIO_I2S_SM=2
  PICO_AUDIO_I2S_PIO=pio1
  PICO_AUDIO_I2S_PIO_FUNC=GPIO_FUNC_PIO1
  PICO_AUDIO_I2S_DATA=26
  PICO_AUDIO_I2S_BCLK=27  
)

set(sorcerer2_demo_dv_libs
  libdvi
  tinyusb_host
  tinyusb_board
  pico_emu_utils
  pico_dvi_menu
)

target_link_libraries(sorcerer2_demo_dv
  ${sorcerer2_common_libs}
  ${sorcerer2_demo_dv_libs}
)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_demo_dv)
