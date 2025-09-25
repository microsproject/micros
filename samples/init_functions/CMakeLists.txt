# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.15)

project(init_functions C)

add_executable(${PROJECT_NAME}.elf main.c)

micros_add_os_to_target(${PROJECT_NAME}.elf)
