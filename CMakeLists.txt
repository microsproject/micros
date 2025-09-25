# MIT License
#
# Copyright (c) 2025 MicrOS Project
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
cmake_minimum_required(VERSION 3.15)

if(DEFINED MICROS_BOARD)
    message(STATUS "Building for board: ${MICROS_BOARD}")
    include(${CMAKE_CURRENT_LIST_DIR}/boards/${MICROS_BOARD}/CMakeLists.txt)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/micros-toolchain.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/tools.cmake)

project(micros 
    LANGUAGES C
    VERSION 0.0.1
    DESCRIPTION "MicrOS - minimal RTOS for microcontrollers"
)

add_library(${PROJECT_NAME} STATIC)

target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
)

add_subdirectory(soc/${MICROS_SOC})
add_subdirectory(arch/${MICROS_ARCH})

add_subdirectory(third_party)

if(DEFINED MICROS_SAMPLE)
  add_subdirectory(samples/${MICROS_SAMPLE})
elseif(DEFINED MICROS_APP)
  add_subdirectory(apps/${MICROS_APP})
else()
  message(FATAL_ERROR "No MICROS_SAMPLE or MICROS_APP defined, not building any application")
endif()