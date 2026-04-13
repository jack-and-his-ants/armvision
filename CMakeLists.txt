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
        include/utils.h
        src/utils.c)
