cmake_minimum_required(VERSION 4.2)
project(armvision C ASM)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(armvision
        include/bmp.h
        include/filters.h
        src/main.c
        src/bmp.c
        src/filters.c
        include/image.h
        src/assembly/clamp.s
        src/assembly/grayscale.s
        src/assembly/monochrome_asm.s
        include/utils.h
        src/utils.c
        include/benchmark.h
        src/benchmark.c
        include/filters_asm.h
        src/filters_asm.c)
