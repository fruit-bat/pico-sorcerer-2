
set(sorcerer2_hdmi_src
  main.cpp
)

add_executable(sorcerer2_hdmi
  ${sorcerer2_common_src}
  ${sorcerer2_hdmi_src}
)

# TODO this should work ok with DVI_N_TMDS_BUFFERS=2 (perhaps need to
# rearrange some pushes/pops) and also as we are monochrome the buffers are 3x
# as big as they need to be 
target_compile_definitions(sorcerer2_hdmi PRIVATE
  DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
  DVI_VERTICAL_REPEAT=2
  DVI_N_TMDS_BUFFERS=3
  DVI_MONOCHROME_TMDS=1
  DVI_1BPP_BIT_REVERSE=1
  PCS_COLS=80
  PCS_ROWS=30
  LED_PIN=25
  SPK_PIN=20
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

target_link_libraries(sorcerer2_hdmi
  ${sorcerer2_common_libs}
  ${sorcerer2_hdmi_libs}
)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_hdmi)
