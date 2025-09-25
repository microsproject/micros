# SPDX-License-Identifier: MIT
#
function(micros_add_os_to_target target_name)
  target_link_libraries(${target_name} PRIVATE micros)

  if(NOT TARGET ${target_name})
    message(FATAL_ERROR "Target ${target_name} does not exist")
  endif()

  add_custom_command(
    TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${target_name}> ${target_name}.bin
  )

  add_custom_command(
    TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${target_name}> ${target_name}.hex
  )

  add_custom_command(
    TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target_name}> | tee ${target_name}.size
  )
    
endfunction()
