cmake_minimum_required(VERSION 4.2)
project(armvision C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(armvision
        include/bmp.h
        include/filters.h
        src/main.c
        src/bmp.c
        src/filters.c
        include/image.h)
