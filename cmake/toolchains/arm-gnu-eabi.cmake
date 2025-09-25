# SPDX-License-Identifier: MIT
#
# ARM Cortex-M (bare-metal) using GNU Arm Embedded
# Required cache vars you can pass on the command line:
#   -DMICROS_CPU=cortex-m4
#   -DMICROS_FPU=fpv4-sp-d16   (optional)
#   -DMICROS_FLOAT_ABI=softfp  (soft|softfp|hard)
# Optional:
#   -DMICROS_GNU_ARM_PREFIX=arm-none-eabi

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Tool prefix (allow override)
set(MICROS_GNU_ARM_PREFIX "${MICROS_GNU_ARM_PREFIX}" CACHE STRING "GNU Arm tool prefix (e.g. arm-none-eabi)")
if(NOT MICROS_GNU_ARM_PREFIX OR MICROS_GNU_ARM_PREFIX STREQUAL "")
  set(MICROS_GNU_ARM_PREFIX arm-none-eabi)
endif()

# Compilers
set(CMAKE_C_COMPILER   ${MICROS_GNU_ARM_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${MICROS_GNU_ARM_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${MICROS_GNU_ARM_PREFIX}-gcc)
set(CMAKE_AR           ${MICROS_GNU_ARM_PREFIX}-ar)
set(CMAKE_OBJCOPY      ${MICROS_GNU_ARM_PREFIX}-objcopy)
set(CMAKE_OBJDUMP      ${MICROS_GNU_ARM_PREFIX}-objdump)
set(CMAKE_SIZE         ${MICROS_GNU_ARM_PREFIX}-size)

# CPU/FPU flags
set(MICROS_CPU       "${MICROS_CPU}"       CACHE STRING "ARM CPU (e.g., cortex-m4, cortex-m33)")
set(MICROS_FPU       "${MICROS_FPU}"       CACHE STRING "FPU name (e.g., fpv4-sp-d16, fpv5-sp-d16)")
set(MICROS_FLOAT_ABI "${MICROS_FLOAT_ABI}" CACHE STRING "float ABI (soft, softfp, hard)")

if(NOT MICROS_CPU)
  message(FATAL_ERROR "MICROS_CPU is required for arm/cortex_m (e.g., -DMICROS_CPU=cortex-m4)")
endif()

set(ARCH_CPU_FLAGS "-mcpu=${MICROS_CPU} -mthumb")
if(MICROS_FPU)
  list(APPEND ARCH_CPU_FLAGS "-mfpu=${MICROS_FPU}")
endif()
if(MICROS_FLOAT_ABI)
  list(APPEND ARCH_CPU_FLAGS "-mfloat-abi=${MICROS_FLOAT_ABI}")
endif()

# Common C flags
set(ARCH_WARN "-Wall -Wextra -Werror")
set(ARCH_SIZE "-ffunction-sections -fdata-sections")
set(ARCH_OTHER "-ffreestanding -fno-builtin")

if(MICROS_LTO)
  list(APPEND ARCH_OTHER "-flto")
endif()

set(CMAKE_C_FLAGS_INIT     "${ARCH_CPU_FLAGS} ${ARCH_WARN} ${ARCH_SIZE} ${ARCH_OTHER} ${MICROS_EXTRA_CFLAGS}")
set(CMAKE_CXX_FLAGS_INIT   "${CMAKE_C_FLAGS_INIT} -fno-rtti -fno-exceptions")
set(CMAKE_ASM_FLAGS_INIT   "${ARCH_CPU_FLAGS}")

# Linker flags (no syscalls by default; kernel provides stubs)
set(SPECS_FLAGS "")
if(MICROS_USE_NANO)
  set(SPECS_FLAGS "-specs=nano.specs")
else()
  set(SPECS_FLAGS "-specs=nosys.specs")
endif()

set(LD_GC "-Wl,--gc-sections")
set(LD_MAP "-Wl,-Map=${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")

if(MICROS_PRINTF_FLOAT)
  # pull floating printf if using nano
  list(APPEND CMAKE_EXE_LINKER_FLAGS_INIT "-u _printf_float")
endif()

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "${ARCH_CPU_FLAGS} ${LD_GC} ${LD_MAP} ${SPECS_FLAGS} ${MICROS_EXTRA_LDFLAGS}")
    message(STATUS "CMAKE_EXE_LINKER_FLAGS_INIT='${CMAKE_EXE_LINKER_FLAGS_INIT}'")

# Convenience output formats (ELF→BIN/HEX) — opt-in via a post-build command in targets
set(MICROS_ELF2BIN ${CMAKE_OBJCOPY} -O binary)
set(MICROS_ELF2HEX ${CMAKE_OBJCOPY} -O ihex)
