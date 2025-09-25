# SPDX-License-Identifier: MIT
#
# MicrOS unified toolchain router
# Usage (ARM Cortex-M4F example):
#   cmake -S . -B build \
#     -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/micros-toolchain.cmake \
#     -DMICROS_ARCH=arm/cortex_m -DMICROS_CPU=cortex-m4 -DMICROS_FPU=fpv4-sp-d16 \
#     -DMICROS_FLOAT_ABI=softfp -DMICROS_USE_NANO=ON

# Tell CMake we're cross-compiling (for non-POSIX targets)
if(NOT DEFINED MICROS_ARCH)
  set(MICROS_ARCH "posix" CACHE STRING "MicrOS architecture (posix, arm/cortex_m, riscv/rv32)")
endif()

# Common cache knobs
set(MICROS_USE_NANO    ON  CACHE BOOL  "Link newlib-nano (-specs=nano.specs)")
set(MICROS_PRINTF_FLOAT OFF CACHE BOOL "Pull in printf float symbols (-u _printf_float)")
set(MICROS_LTO         OFF CACHE BOOL  "Enable -flto")
set(MICROS_EXTRA_CFLAGS "" CACHE STRING "Extra C flags")
set(MICROS_EXTRA_LDFLAGS "" CACHE STRING "Extra LD flags")

# Route to per-arch toolchain config
if(MICROS_ARCH STREQUAL "arm/cortex_m")
  include(${CMAKE_CURRENT_LIST_DIR}/arm-gnu-eabi.cmake)
elseif(MICROS_ARCH STREQUAL "posix")
  include(${CMAKE_CURRENT_LIST_DIR}/posix.cmake)
elseif(MICROS_ARCH STREQUAL "riscv/rv32")
  include(${CMAKE_CURRENT_LIST_DIR}/riscv-gnu-elf.cmake)
else()
  message(FATAL_ERROR "Unknown MICROS_ARCH='${MICROS_ARCH}'. Expected: posix | arm/cortex_m | riscv/rv32")
endif()
