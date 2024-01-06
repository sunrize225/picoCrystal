cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(picoCrystal)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

include_directories(include)

add_executable(picoCrystal
    examples/test.c
    src/picoCrystal.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(picoCrystal PRIVATE pico_stdlib hardware_gpio)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(picoCrystal)