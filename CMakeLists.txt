cmake_minimum_required(VERSION 3.20)

project(hell_64_piltover C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB ASM_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/asm/*.S")

# Main product executable (production runtime)
add_executable(hell_64_piltover
        ${ASM_SOURCES}
        src/runtime.c
        include/native_functions.c
        src/vm.c
        include/allocator.h
        src/allocator.c
        include/bytecode.h
)

target_include_directories(hell_64_piltover PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
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

# Testing layer (development only)
option(BUILD_TESTING "Build the test suite" ON)

if (BUILD_TESTING)
    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.c")

    add_executable(run_tests
            ${ASM_SOURCES}
            ${TEST_SOURCES}
            include/native_functions.c
            src/vm.c
            src/allocator.c
    )

    target_include_directories(run_tests PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
    )

    target_compile_options(run_tests PRIVATE
            -ffreestanding
            -fno-stack-protector
    )

    target_link_options(run_tests PRIVATE
            -nostdlib
            -nostartfiles
            -nodefaultlibs
            -Wl,-e,_start
    )

    add_test(NAME run_tests COMMAND run_tests)
endif ()