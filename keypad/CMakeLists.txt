# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Pico-Deck C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(Pico-Deck
        src/main.c
        src/usb_descriptors.c
)

pico_set_program_name(Pico-Deck "Pico-Deck")
pico_set_program_version(Pico-Deck "0.1")

# stdio disabled — USB is owned by our composite TinyUSB descriptor, not stdio
pico_enable_stdio_uart(Pico-Deck 0)
pico_enable_stdio_usb(Pico-Deck 0)

# Link libraries: stdlib + TinyUSB device stack + BSP
target_link_libraries(Pico-Deck
        pico_stdlib
        tinyusb_device
        tinyusb_board
)

# Include current dir so tusb_config.h / usb_descriptors.h are found
target_include_directories(Pico-Deck PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(Pico-Deck)