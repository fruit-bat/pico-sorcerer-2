# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(sorcerer2
	main.cpp
	Z80.cpp
	Sorcerer2.cpp
	Sorcerer2Keyboard.cpp
	Sorcerer2HidKeyboard.cpp
	Sorcerer2DiskSystem.cpp
	Sorcerer2DiskDrive.cpp
	Sorcerer2DiskFatFsSpi.cpp
	Sorcerer2SdCardFatFsSpi.cpp
	Sorcerer2TapeSystem.cpp
	Sorcerer2TapeUnit.cpp
	Sorcerer2TapeUnitFatFsSpi.cpp
	hid_app.c
	hw_config.c
	PicoRectXYWH.cpp
	PicoCharScreen.cpp
	PicoWinHidKeyboard.cpp
)

target_include_directories(sorcerer2 PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# TODO this should work ok with DVI_N_TMDS_BUFFERS=2 (perhaps need to
# rearrange some pushes/pops) and also as we are monochrome the buffers are 3x
# as big as they need to be 
target_compile_definitions(sorcerer2 PRIVATE
	DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
	DVI_VERTICAL_REPEAT=2
	DVI_N_TMDS_BUFFERS=3
	DVI_MONOCHROME_TMDS=1
	DVI_1BPP_BIT_REVERSE=1
	)

add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

target_link_libraries(sorcerer2
	pico_stdlib
	pico_multicore
	pico_util
	libdvi
	tinyusb_host
	tinyusb_board
	FatFs_SPI
)

# create map/bin/hex file etc.
pico_add_extra_outputs(sorcerer2)

