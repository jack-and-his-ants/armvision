cmake_minimum_required(VERSION 3.10)
project(armvision C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(CheckCCompilerFlag)
check_c_compiler_flag(-march=native HAS_MARCH_NATIVE)

set(SOURCES
        src/main.c
        src/bmp.c
        src/filters.c
        src/utils.c
        src/benchmark.c
        src/filters_asm.c
        src/assembly/clamp.s
        src/assembly/grayscale.s
        src/assembly/monochrome_asm.s
        src/assembly/negative_asm.s
        src/neon/grayscale_neon.s
        src/neon/monochrome_neon.s
        src/neon/negative_neon.s
)

set(HEADERS
        include/bmp.h
        include/filters.h
        include/image.h
        include/utils.h
        include/benchmark.h
        include/filters_asm.h
        include/filters_neon.h
)

add_executable(armvision ${SOURCES} ${HEADERS})

target_include_directories(armvision PRIVATE include)

if(HAS_MARCH_NATIVE)
    target_compile_options(armvision PRIVATE -O3 -march=native)
else()
    target_compile_options(armvision PRIVATE -O3)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
        target_compile_options(armvision PRIVATE -mfloat-abi=hard -mfpu=neon)
    endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_SYSTEM_PROCESSOR MATCHES "arm|aarch64")
    message(STATUS "Building for Apple Silicon - NEON is enabled by default")
endif()