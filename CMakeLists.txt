#
# Created by VC365 on 8/22/25.
#
cmake_minimum_required(VERSION 3.31)
project(grub-qemu C)

set(CMAKE_C_STANDARD 11)


add_executable(grub-qemu main.c
        src/temp.h
)
