# SPDX-License-Identifier: MIT
#
# Native / POSIX build (host compilers)
# No cross flags; keep warnings and sane defaults for tests/sim.

if(UNIX AND NOT APPLE)
  set(CMAKE_SYSTEM_NAME Linux)
elseif(APPLE)
  set(CMAKE_SYSTEM_NAME Darwin)
else()
  set(CMAKE_SYSTEM_NAME Generic)
endif()

# Use host toolchain
# (Let CMake discover compilers; you can still override CC/CXX env vars)

set(ARCH_WARN "-Wall -Wextra -Werror")
set(ARCH_POSIX "-D_POSIX_C_SOURCE=200809L -pthread")
set(ARCH_SIZE "")
set(ARCH_OTHER "")

set(CMAKE_C_FLAGS_INIT   "${ARCH_WARN} ${ARCH_POSIX} ${ARCH_SIZE} ${ARCH_OTHER} ${MICROS_EXTRA_CFLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_C_FLAGS_INIT}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${MICROS_EXTRA_LDFLAGS}")
