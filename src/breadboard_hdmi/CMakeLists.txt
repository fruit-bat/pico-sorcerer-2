
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

