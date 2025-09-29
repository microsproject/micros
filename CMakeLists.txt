# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.15)


set(MICROS_ROOT ${CMAKE_SOURCE_DIR})
if(NOT MICROS_BOARD)
    message(FATAL_ERROR "Please specify a MICROS_BOARD, e.g. -DMICROS_BOARD=lm,stm32vldiscovery")
endif()

set(GENERATED_PATH ${CMAKE_BINARY_DIR}/micros_generated)
execute_process(
    COMMAND python ${MICROS_ROOT}/device/tools/micros_device.py ${GENERATED_PATH} ${MICROS_BOARD}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE parser_output
    ERROR_VARIABLE parser_error
    RESULT_VARIABLE ret
)
if(NOT ret EQUAL 0)
    message(FATAL_ERROR "Device description parser failed with code ${ret}\nOutput:\n${parser_output}\nError:\n${parser_error}")
endif()

set(GENERATED_CMAKE ${GENERATED_PATH}/micros_generated.cmake)
include(${GENERATED_CMAKE})


if(DEFINED MICROS_BOARD)
    message(STATUS "Building for board: ${MICROS_BOARD}")
    include(${CMAKE_CURRENT_LIST_DIR}/board/${MICROS_BOARD}/CMakeLists.txt)
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