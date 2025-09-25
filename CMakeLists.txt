# SPDX-License-Identifier: MIT
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
add_subdirectory(micros)

add_subdirectory(third_party)

if(DEFINED MICROS_SAMPLE)
  add_subdirectory(samples/${MICROS_SAMPLE})
elseif(DEFINED MICROS_APP)
  add_subdirectory(apps/${MICROS_APP})
else()
  message(FATAL_ERROR "No MICROS_SAMPLE or MICROS_APP defined, not building any application")
endif()