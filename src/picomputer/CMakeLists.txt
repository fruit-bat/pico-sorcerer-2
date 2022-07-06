include_directories(${CMAKE_CURRENT_LIST_DIR})

set(picomputer_common_src
	${CMAKE_CURRENT_LIST_DIR}/hw_config.c
)

add_subdirectory(picomputer_vga)

