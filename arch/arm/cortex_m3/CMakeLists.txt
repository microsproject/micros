# SPDX-License-Identifier: MIT
#
target_link_options(${PROJECT_NAME} 
    INTERFACE -T${CMAKE_CURRENT_LIST_DIR}/linker.ld
)

target_sources(${PROJECT_NAME}
  PUBLIC boot.c
  PUBLIC kernel.c
  PUBLIC syscalls.c
)

target_link_libraries(${PROJECT_NAME}
  PUBLIC cmsis
)
