cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(picopsxcmd_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(PicoPSXCMD)

pico_set_linker_script(PicoPSXCMD ${CMAKE_SOURCE_DIR}/memmap.ld)
pico_generate_pio_header(PicoPSXCMD ${CMAKE_CURRENT_LIST_DIR}/psxSPI.pio)
pico_generate_pio_header(PicoPSXCMD ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)


# Example source
target_sources(PicoPSXCMD PUBLIC
    ${CMAKE_SOURCE_DIR}/src/led.c
    ${CMAKE_SOURCE_DIR}/src/lfs_disk.c
    ${CMAKE_SOURCE_DIR}/src/lfs_flash_handler.c
    ${CMAKE_SOURCE_DIR}/src/main.c
    ${CMAKE_SOURCE_DIR}/src/memcard_simulator.c
    ${CMAKE_SOURCE_DIR}/src/controller_simulator.c
    ${CMAKE_SOURCE_DIR}/src/memory_card.c
    ${CMAKE_SOURCE_DIR}/src/msc_handler.c
    ${CMAKE_SOURCE_DIR}/src/ram_disk.c
    ${CMAKE_SOURCE_DIR}/src/usb_descriptors.c
    ${CMAKE_SOURCE_DIR}/ff14b/source/diskio.c
    ${CMAKE_SOURCE_DIR}/ff14b/source/ff.c
    ${CMAKE_SOURCE_DIR}/littlefs/lfs.c
    ${CMAKE_SOURCE_DIR}/littlefs/lfs_util.c
)

# Example include
target_include_directories(PicoPSXCMD PUBLIC
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/ff14b/source
    ${CMAKE_SOURCE_DIR}/littlefs
)

pico_enable_stdio_uart(PicoPSXCMD 1)	# enable only UART stdio

target_link_libraries(PicoPSXCMD pico_stdlib pico_multicore pico_time hardware_flash hardware_pio tinyusb_device tinyusb_board)

pico_add_extra_outputs(PicoPSXCMD)