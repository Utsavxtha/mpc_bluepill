cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME minor_)

include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

set(CMAKE_EXE_LINKER_FLAGS "-u _printf_float")

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    robotlib
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    Core/Src/dma.c
    # Add user sources here
    Core/Src/car.cpp
    robotlib/communication/uart.cpp
    robotlib/controllers/pid.cpp
    robotlib/common/printf_config.c
    robotlib/common/processor_type.h
    robotlib/actuators/motor.cpp
    robotlib/sensors/encoder.cpp
    robotlib/sensors/bno08_spi.cpp
    robotlib/crypto/crc8.c
    robotlib/crypto/crc8.cpp
    robotlib/crypto/easy_crc.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    robotlib
    Drivers/CMSIS/DSP/Include
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ARM_MATH_CM3
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
# list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)

set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F103XX_FLASH.ld")

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    "-T${LINKER_SCRIPT}"
    "-Wl,--gc-sections" # Remove unused code
    "--specs=nosys.specs" # Provides empty stubs for sys calls (fixes _sbrk)
    "-Wl,--print-memory-usage" # Optional: helps you see RAM/Flash usage
)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    "-mcpu=cortex-m3"
    "-mthumb"
)

# Generate binary
add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O binary ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    DEPENDS ${CMAKE_PROJECT_NAME}
    COMMENT "Converting ELF to binary"
)

# Binary target
add_custom_target(bin ALL
    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    COMMENT "Generating binary file"
)
