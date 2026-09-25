cmake_minimum_required(VERSION 4.3)

project(hell_64_piltover C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB ASM_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/asm/*.S")

add_executable(hell_64_piltover
        ${ASM_SOURCES}
        runtime.c
        include/native_functions.c
        vm/vm.c
)

target_include_directories(hell_64_piltover PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/vm/include
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