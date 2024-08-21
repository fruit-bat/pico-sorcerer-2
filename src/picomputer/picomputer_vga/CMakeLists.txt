set(sorcerer2_scanline_vga_src
  ${sorcerer2_common_src}
  ${sorcerer2_scanline_src}
  ${picomputer_common_src}
  main.cpp
)

set(sorcerer2_scanline_vga_libs
  ${sorcerer2_common_libs}
  pico_emu_utils
  pico_scanvideo_menu
  tinyusb_host
  tinyusb_board
  hardware_pio
  hardware_pwm
  pico_scanvideo_dpi
)

add_executable(sorcerer2_picomputer_vga_1111_ps2
  ${sorcerer2_scanline_vga_src}
)

target_compile_definitions(sorcerer2_picomputer_vga_1111_ps2 PRIVATE
  # Configure the SPI sd card to use SPI0
  SDCARD_PIN_SPI0_CS=5
  SDCARD_PIN_SPI0_SCK=2
  SDCARD_PIN_SPI0_MOSI=3
  SDCARD_PIN_SPI0_MISO=4  
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=20
  # VGA 1111
  VGA_ENC_RGBY_1111
  PICO_SCANVIDEO_COLOR_PIN_COUNT=4u
  PICO_SCANVIDEO_DPI_PIXEL_RCOUNT=2u
  PICO_SCANVIDEO_DPI_PIXEL_GCOUNT=1u
  PICO_SCANVIDEO_DPI_PIXEL_BCOUNT=1u
  PICO_SCANVIDEO_DPI_PIXEL_RSHIFT=2u
  PICO_SCANVIDEO_DPI_PIXEL_GSHIFT=1u
  PICO_SCANVIDEO_DPI_PIXEL_BSHIFT=0u
  PICO_SCANVIDEO_COLOR_PIN_BASE=12u
  # Enable the PS/2 keyboard
  USE_PS2_KBD
  # TODO work what this is
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=700
)

target_link_libraries(sorcerer2_picomputer_vga_1111_ps2
  ${sorcerer2_scanline_vga_libs}
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_1111_ps2 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_1111_ps2 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_1111_ps2)

add_executable(sorcerer2_picomputer_vga_222
  ${sorcerer2_scanline_vga_src}
)

target_compile_definitions(sorcerer2_picomputer_vga_222 PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=9
  # Configure the SPI sd card to use PIO1
  SDCARD_PIO=pio1
  SDCARD_PIO_SM=1  
  SDCARD_PIN_SPI0_CS=13
  SDCARD_PIN_SPI0_SCK=10
  SDCARD_PIN_SPI0_MOSI=11
  SDCARD_PIN_SPI0_MISO=12  
  # VGA 222
  VGA_ENC_RGB_222
  PICO_SCANVIDEO_COLOR_PIN_COUNT=6u
  PICO_SCANVIDEO_DPI_PIXEL_RCOUNT=2u
  PICO_SCANVIDEO_DPI_PIXEL_GCOUNT=2u
  PICO_SCANVIDEO_DPI_PIXEL_BCOUNT=3u
  PICO_SCANVIDEO_DPI_PIXEL_RSHIFT=4u
  PICO_SCANVIDEO_DPI_PIXEL_GSHIFT=2u
  PICO_SCANVIDEO_DPI_PIXEL_BSHIFT=0u
  PICO_SCANVIDEO_COLOR_PIN_BASE=0u
  PICOMPUTER_VGA
  # TODO work what this is
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=700  
)

target_link_libraries(sorcerer2_picomputer_vga_222
  ${sorcerer2_scanline_vga_libs}
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_222 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_222 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_222)

add_executable(sorcerer2_picomputer_vga_332
  ${sorcerer2_scanline_vga_src}
)

target_compile_definitions(sorcerer2_picomputer_vga_332 PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=9
  # Configure the SPI sd card to use PIO1
  SDCARD_PIO=pio1
  SDCARD_PIO_SM=1  
  SDCARD_PIN_SPI0_CS=13
  SDCARD_PIN_SPI0_SCK=10
  SDCARD_PIN_SPI0_MOSI=11
  SDCARD_PIN_SPI0_MISO=12
  # VGA 332  
  VGA_ENC_RGB_332
  PICO_SCANVIDEO_COLOR_PIN_COUNT=8u
  PICO_SCANVIDEO_DPI_PIXEL_RCOUNT=3u
  PICO_SCANVIDEO_DPI_PIXEL_GCOUNT=3u
  PICO_SCANVIDEO_DPI_PIXEL_BCOUNT=2u
  PICO_SCANVIDEO_DPI_PIXEL_RSHIFT=5u
  PICO_SCANVIDEO_DPI_PIXEL_GSHIFT=2u
  PICO_SCANVIDEO_DPI_PIXEL_BSHIFT=0u
  PICO_SCANVIDEO_COLOR_PIN_BASE=0u
  PICOMPUTER_VGA
  FF_USE_FIND=1
  # TODO work what this is
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=700  
)

target_link_libraries(sorcerer2_picomputer_vga_332
  ${sorcerer2_scanline_vga_libs}
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_332 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_332 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_332)

#
# Pimoroni VGA pico demo
#
add_executable(sorcerer2_pimoroni_vga_demo
  ${sorcerer2_scanline_vga_src}
)

pico_generate_pio_header(sorcerer2_pimoroni_vga_demo
  ${sorcerer2_audio_i2s_pio}
)

target_compile_definitions(sorcerer2_pimoroni_vga_demo PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  # Configure the I2S audio
  PICO_AUDIO_I2S=1
  PICO_AUDIO_I2S_SM=2
  PICO_AUDIO_I2S_PIO=pio1
  PICO_AUDIO_I2S_PIO_FUNC=GPIO_FUNC_PIO1
  PICO_AUDIO_I2S_DATA=26
  PICO_AUDIO_I2S_BCLK=27

  # Configure the SPI sd card to use PIO
  SDCARD_PIO=pio1
  SDCARD_PIO_SM=1
  SDCARD_PIN_SPI0_CS=22
  SDCARD_PIN_SPI0_SCK=5
  SDCARD_PIN_SPI0_MOSI=18
  SDCARD_PIN_SPI0_MISO=19

  FF_USE_FIND=1
  # TODO work what this is
  PICO_SCANVIDEO_MAX_SCANLINE_BUFFER_WORDS=700  
)

target_link_libraries(sorcerer2_pimoroni_vga_demo
  ${sorcerer2_scanline_vga_libs}
)

pico_enable_stdio_usb(sorcerer2_pimoroni_vga_demo 0)
pico_enable_stdio_uart(sorcerer2_pimoroni_vga_demo 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_pimoroni_vga_demo)

