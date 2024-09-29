
set(sorcerer2_hdmi_src
  main.cpp
)

# TODO this should work ok with DVI_N_TMDS_BUFFERS=2 (perhaps need to
# rearrange some pushes/pops) and also as we are monochrome the buffers are 3x
# as big as they need to be 
set(sorcerer2_hdmi_defines
  DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
  PICO_CORE1_STACK_SIZE=0x400
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
  #  SDCARD_SPI_BUS=spi0
  SDCARD_PIN_SPI0_CS=5
  SDCARD_PIN_SPI0_SCK=2
  SDCARD_PIN_SPI0_MOSI=3
  SDCARD_PIN_SPI0_MISO=4
  USE_PS2_KBD
)

set(sorcerer2_demo_dv_defines
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

)

set(sorcerer2_hdmi_libs
  libdvi
  tinyusb_host
  tinyusb_board
  pico_emu_utils
  pico_dvi_menu
)

foreach(target
  sorcerer2_hdmi_hdmi_audio
  sorcerer2_hdmi_pwm_audio
  sorcerer2_demo_dv_hdmi_audio
  sorcerer2_demo_dv_i2s_audio
)
  add_executable(${target}
    ${sorcerer2_common_src}
    ${sorcerer2_hdmi_src}
  )

  target_link_libraries(${target}
    ${sorcerer2_common_libs}
    ${sorcerer2_hdmi_libs}
  )

  # create map/bin/hex file etc.
  pico_add_extra_outputs(${target})

endforeach()

target_compile_definitions(sorcerer2_hdmi_hdmi_audio PRIVATE
  ${sorcerer2_hdmi_defines}
  PICO_HDMI_AUDIO
)

target_compile_definitions(sorcerer2_hdmi_pwm_audio PRIVATE
  ${sorcerer2_hdmi_defines}
  SPK_PIN=20
)

target_compile_definitions(sorcerer2_demo_dv_hdmi_audio PRIVATE
  ${sorcerer2_demo_dv_defines}
  PICO_HDMI_AUDIO
)

target_compile_definitions(sorcerer2_demo_dv_i2s_audio PRIVATE
  ${sorcerer2_demo_dv_defines}
  # Configure the I2S audio 
  PICO_AUDIO_I2S=1
  PICO_AUDIO_I2S_PIO=pio1
  PICO_AUDIO_I2S_PIO_FUNC=GPIO_FUNC_PIO1
  PICO_AUDIO_I2S_DATA=26
  PICO_AUDIO_I2S_BCLK=27  
)

pico_generate_pio_header(sorcerer2_demo_dv_i2s_audio 
  ${sorcerer2_audio_i2s_pio}
)
