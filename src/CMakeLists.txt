include_directories(${CMAKE_CURRENT_LIST_DIR})

set(sorcerer2_common_src
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2Keyboard.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2HidKeyboard.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2DiskSystem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2DiskDrive.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2DiskFatFsSpi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2TapeSystem.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2TapeUnit.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2TapeUnitFatFsSpi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2Tape.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2RomPac.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2RomPacFatFsSpi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2TapeFatFsSpi.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2Menu.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2Audio.cpp
)

set(sorcerer2_rgb_src
	${CMAKE_CURRENT_LIST_DIR}/Sorcerer2PrepareRgbScanline.cpp
)

set(sorcerer2_scanline_src
	${CMAKE_CURRENT_LIST_DIR}/Sorcerer2PrepareScanvideoScanline.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Sorcerer2ScanlineVgaRenderLoop.cpp
)

set(sorcerer2_audio_i2s_pio
  ${CMAKE_CURRENT_LIST_DIR}/audio_i2s.pio
)

set(sorcerer2_common_libs
	pico_stdlib
	pico_multicore
	pico_util
	pico_emu_utils
  pico_hid_host
  sdcard
  fatfs
  hardware_dma
  Zeta
  Z80
)

add_subdirectory(breadboard_hdmi)
add_subdirectory(picomputer)
