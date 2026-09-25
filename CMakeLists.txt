cmake_minimum_required(VERSION 4.3)

project(hell_64_piltover C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(hell_64_piltover
        start.S
        runtime.c
        include/native_functions.h
)

target_compile_options(hell_64_piltover PRIVATE
        -ffreestanding
        -fno-stack-protector
)

target_link_options(hell_64_piltover PRIVATE
        -nostdlib
        -nostartfiles
        -nodefaultlibs
        -Wl,-e,_start
)