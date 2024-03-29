
add_executable(sorcerer2_picomputer_vga_1111
  ${sorcerer2_common_src}
  ${sorcerer2_rgb_src}
  ${picomputer_common_src}
  main.cpp
)

target_compile_definitions(sorcerer2_picomputer_vga_1111 PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=9
  VGA_ENC_RGBY_1111
  VGA_GPIO_SYNC=4
  VGA_GPIO_OUTNUM=4
  PICOMPUTER_VGA
)

target_link_libraries(sorcerer2_picomputer_vga_1111
  ${sorcerer2_common_libs}
  pico_emu_utils
  pico_vga_menu
  pico_vga_332
  tinyusb_host
  tinyusb_board
  hardware_pio
  hardware_pwm
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_1111 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_1111 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_1111)

add_executable(sorcerer2_picomputer_vga_1111_ps2
  ${sorcerer2_common_src}
  ${sorcerer2_rgb_src}
  ${picomputer_common_src}
  main.cpp
)

target_compile_definitions(sorcerer2_picomputer_vga_1111_ps2 PRIVATE
  ${picomputer_vga_defines}
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
  VGA_GPIO_FIRST=12
  VGA_GPIO_SYNC=16
  VGA_GPIO_OUTNUM=4
  # Enable the PS/2 keyboard
  USE_PS2_KBD    
)

target_link_libraries(sorcerer2_picomputer_vga_1111_ps2
  ${sorcerer2_common_libs}
  pico_emu_utils
  pico_vga_menu
  pico_vga_332
  tinyusb_host
  tinyusb_board
  hardware_pio
  hardware_pwm
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_1111_ps2 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_1111_ps2 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_1111_ps2)

add_executable(sorcerer2_picomputer_vga_222
  ${sorcerer2_common_src}
  ${sorcerer2_rgb_src}
  ${picomputer_common_src}
  main.cpp
)

target_compile_definitions(sorcerer2_picomputer_vga_222 PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=9
  VGA_ENC_RGB_222
  VGA_GPIO_SYNC=6
  VGA_GPIO_OUTNUM=6
  PICOMPUTER_VGA
)

target_link_libraries(sorcerer2_picomputer_vga_222
  ${sorcerer2_common_libs}
  pico_emu_utils
  pico_vga_menu
  pico_vga_332
  tinyusb_host
  tinyusb_board
  hardware_pio
  hardware_pwm
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_222 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_222 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_222)




add_executable(sorcerer2_picomputer_vga_332
  ${sorcerer2_common_src}
  ${sorcerer2_rgb_src}
  ${picomputer_common_src}
  main.cpp
)

target_compile_definitions(sorcerer2_picomputer_vga_332 PRIVATE
  PCS_COLS=80 
  PCS_ROWS=30
  SPK_PIN=9
  VGA_ENC_RGB_332
  PICOMPUTER_VGA
  FF_USE_FIND=1
)

target_link_libraries(sorcerer2_picomputer_vga_332
  ${sorcerer2_common_libs}
  pico_emu_utils
  pico_vga_menu
  pico_vga_332
  tinyusb_host
  tinyusb_board
  hardware_pio
  hardware_pwm
)

pico_enable_stdio_usb(sorcerer2_picomputer_vga_332 0)
pico_enable_stdio_uart(sorcerer2_picomputer_vga_332 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2_picomputer_vga_332)


