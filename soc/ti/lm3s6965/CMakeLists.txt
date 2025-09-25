# SPDX-License-Identifier: MIT
#
target_link_options(${PROJECT_NAME} 
    INTERFACE -T${CMAKE_CURRENT_LIST_DIR}/sections.ld
)

target_sources(${PROJECT_NAME}
  PRIVATE system.c
)