/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef MICROS_LOG_H
#define MICROS_LOG_H
/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @file log.h
 * @brief Simple logging utilities for MicrOS
 *
 * This header provides macros for logging messages at different severity
 * levels. The logging can be enabled or disabled via the MICROS_LOG_ENABLE
 * macro.
 *
 * @defgroup logging Logging
 * @{
 */
/*--------------------------------------------------------------------------------------------------------------------*/
typedef enum {
    MICROS_LOG_LEVEL_NONE = 0,
    MICROS_LOG_LEVEL_ERROR = 1,
    MICROS_LOG_LEVEL_WARN = 2,
    MICROS_LOG_LEVEL_INFO = 3,
    MICROS_LOG_LEVEL_DEBUG = 4
} micros_log_level_t;
/*--------------------------------------------------------------------------------------------------------------------*/
void _log(micros_log_level_t level,
          const char* module_name,
          const char* file,
          int line,
          const char* fmt,
          ...);
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef CONFIG_MICROS_LOG_ENABLE
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_NONE
#endif
#ifndef CONFIG_MICROS_LOG_LEVEL
#define CONFIG_MICROS_LOG_LEVEL MICROS_LOG_LEVEL_ERROR
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
#define MICROS_LOG_REGISTER(module_name, level)                \
    static const char* _micros_log_module_name = #module_name; \
    static const micros_log_level_t _micros_log_level = level

/*--------------------------------------------------------------------------------------------------------------------*/
#if CONFIG_MICROS_LOG_LEVEL < MICROS_LOG_LEVEL_NONE || \
    CONFIG_MICROS_LOG_LEVEL > MICROS_LOG_LEVEL_DEBUG
#error "Invalid CONFIG_MICROS_LOG_LEVEL value"
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
#if CONFIG_MICROS_LOG_LEVEL >= MICROS_LOG_LEVEL_DEBUG
#define D(...)                                                              \
    do {                                                                    \
        if (_micros_log_level >= MICROS_LOG_LEVEL_DEBUG) {                  \
            _log(MICROS_LOG_LEVEL_DEBUG, _micros_log_module_name, __FILE__, \
                 __LINE__, __VA_ARGS__);                                    \
        }                                                                   \
    } while (0)
#else
#define D(...) \
    do {       \
    } while (0)
#endif

#if CONFIG_MICROS_LOG_LEVEL >= MICROS_LOG_LEVEL_INFO
#define I(...)                                                             \
    do {                                                                   \
        if (_micros_log_level >= MICROS_LOG_LEVEL_INFO) {                  \
            _log(MICROS_LOG_LEVEL_INFO, _micros_log_module_name, __FILE__, \
                 __LINE__, __VA_ARGS__);                                   \
        }                                                                  \
    } while (0)
#else
#define I(...) \
    do {       \
    } while (0)
#endif

#if CONFIG_MICROS_LOG_LEVEL >= MICROS_LOG_LEVEL_WARN
#define W(...)                                                             \
    do {                                                                   \
        if (_micros_log_level >= MICROS_LOG_LEVEL_WARN) {                  \
            _log(MICROS_LOG_LEVEL_WARN, _micros_log_module_name, __FILE__, \
                 __LINE__, __VA_ARGS__);                                   \
        }                                                                  \
    } while (0)
#else
#define W(...) \
    do {       \
    } while (0)
#endif

#if CONFIG_MICROS_LOG_LEVEL >= MICROS_LOG_LEVEL_ERROR
#define E(...)                                                              \
    do {                                                                    \
        if (_micros_log_level >= MICROS_LOG_LEVEL_ERROR) {                  \
            _log(MICROS_LOG_LEVEL_ERROR, _micros_log_module_name, __FILE__, \
                 __LINE__, __VA_ARGS__);                                    \
        }                                                                   \
    } while (0)
#else
#define E(...) \
    do {       \
    } while (0)
#endif
/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @}
 */
#endif /* MICROS_LOG_H */
/*--------------------------------------------------------------------------------------------------------------------*/