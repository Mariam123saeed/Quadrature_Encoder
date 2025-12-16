# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})#include "HBridge_config.hpp"

else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

# =======================#include "HBridge_config.hpp"


set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(Quadrature_Encoder C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable with all source files
add_executable(Quadrature_Encoder
    App/Quadrature_Encoder.cpp
    HAL/Encoder/encoder_hal.cpp
    HAL/H_Bridge/HBridge_hal.cpp
    Service/Encoder/encoder_service.cpp
)

# Set program name and version
pico_set_program_name(Quadrature_Encoder "Quadrature_Encoder")
pico_set_program_version(Quadrature_Encoder "0.1")

# Enable stdio over UART and USB
pico_enable_stdio_uart(Quadrature_Encoder 1)
pico_enable_stdio_usb(Quadrature_Encoder 1)
#include "HBridge_config.hpp"

# Include directories
target_include_directories(Quadrature_Encoder PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/App
    ${CMAKE_CURRENT_LIST_DIR}/HAL
    ${CMAKE_CURRENT_LIST_DIR}/Service
)

# Link standard Pico libraries
target_link_libraries(Quadrature_Encoder
    pico_stdlib
    hardware_gpio
    hardware_timer
    hardware_pwm
)

# Generate UF2, bin, hex outputs
pico_add_extra_outputs(Quadrature_Encoder)
