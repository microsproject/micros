/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#include "micros/log.h"
#include <stdarg.h>
#include <stdio.h>
/*--------------------------------------------------------------------------------------------------------------------*/
#define MICROS_LOG_ESCAPE_CODE_COLOR_RED "\033[31m"
#define MICROS_LOG_ESCAPE_CODE_COLOR_GREEN "\033[32m"
#define MICROS_LOG_ESCAPE_CODE_COLOR_YELLOW "\033[33m"
#define MICROS_LOG_ESCAPE_CODE_COLOR_BLUE "\033[34m"
#define MICROS_LOG_ESCAPE_CODE_COLOR_RESET "\033[0m"
#define MICROS_LOG_LEVEL_STRING_ERROR "E"
#define MICROS_LOG_LEVEL_STRING_WARN "W"
#define MICROS_LOG_LEVEL_STRING_INFO "I"
#define MICROS_LOG_LEVEL_STRING_DEBUG "D"
/*--------------------------------------------------------------------------------------------------------------------*/
void _log(micros_log_level_t level,
          const char* module_name,
          const char* file,
          int line,
          const char* fmt,
          ...) {
    const char* level_str = "";
    const char* color_escape_str = "";
    FILE* output = (level == MICROS_LOG_LEVEL_ERROR) ? stderr : stdout;
    switch (level) {
        case MICROS_LOG_LEVEL_ERROR:
            color_escape_str = MICROS_LOG_ESCAPE_CODE_COLOR_RED;
            level_str = MICROS_LOG_LEVEL_STRING_ERROR;
            break;
        case MICROS_LOG_LEVEL_WARN:
            color_escape_str = MICROS_LOG_ESCAPE_CODE_COLOR_YELLOW;
            level_str = MICROS_LOG_LEVEL_STRING_WARN;
            break;
        case MICROS_LOG_LEVEL_INFO:
            color_escape_str = MICROS_LOG_ESCAPE_CODE_COLOR_GREEN;
            level_str = MICROS_LOG_LEVEL_STRING_INFO;
            break;
        case MICROS_LOG_LEVEL_DEBUG:
            color_escape_str = MICROS_LOG_ESCAPE_CODE_COLOR_BLUE;
            level_str = MICROS_LOG_LEVEL_STRING_DEBUG;
            break;
        default:
            return;  // Invalid log level
    }

    fprintf(output, color_escape_str);
    if (level != MICROS_LOG_LEVEL_DEBUG) {
        fprintf(output, "[%s] [%s] ", level_str, module_name);
    } else {
        fprintf(output, "[%s] [%s] --%s:%d-- ", level_str, module_name, file,
                line);
    }
    va_list args;
    va_start(args, fmt);
    vfprintf(output, fmt, args);
    fprintf(output, MICROS_LOG_ESCAPE_CODE_COLOR_RESET "\n");
    va_end(args);
}
/*--------------------------------------------------------------------------------------------------------------------*/
