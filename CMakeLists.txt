# Replace TMDS with 10 bit UART (same baud rate):
# add_definitions(-DDVI_SERIAL_DEBUG=1)
# add_definitions(-DRUN_FROM_CRYSTAL)

add_executable(sorcerer2
	src/main.cpp
	src/Z80.cpp
	src/Sorcerer2.cpp
	src/Sorcerer2Keyboard.cpp
	src/Sorcerer2HidKeyboard.cpp
	src/Sorcerer2DiskSystem.cpp
	src/Sorcerer2DiskDrive.cpp
	src/Sorcerer2DiskFatFsSpi.cpp
	src/Sorcerer2TapeSystem.cpp
	src/Sorcerer2TapeUnit.cpp
	src/Sorcerer2TapeUnitFatFsSpi.cpp
	src/Sorcerer2Tape.cpp
	src/Sorcerer2RomPac.cpp
	src/Sorcerer2RomPacFatFsSpi.cpp
	src/Sorcerer2TapeFatFsSpi.cpp
	src/hid_app.c
	src/hw_config.c
	src/PicoRectXYWH.cpp
	src/PicoRectXYXY.cpp
	src/PicoCharScreen.cpp
	src/PicoClippedScreen.cpp
	src/PicoWinHidKeyboard.cpp
	src/PicoPen.cpp
	src/PicoWin.cpp
	src/PicoDisplay.cpp
	src/PicoWinBlock.cpp
	src/Sorcerer2Menu.cpp
	src/PicoSelect.cpp
	src/PicoOption.cpp
	src/PicoOptionText.cpp
	src/SdCardFatFsSpi.cpp
	src/FatFsSpiDirReader.cpp
	src/PicoQuickKey.cpp
	src/PicoQuickKeyAscii.cpp
	src/PicoWiz.cpp
	src/PicoTextField.cpp
)

target_include_directories(sorcerer2 PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

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

