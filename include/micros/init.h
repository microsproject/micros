/*--------------------------------------------------------------------------------------------------------------------*/
/* SPDX-License-Identifier: MIT */
/*--------------------------------------------------------------------------------------------------------------------*/
/**
 * @file init.h
 * @brief MicrOS Initialization Framework
 *
 * This header provides macros and utilities for declaring functions
 * that should be executed during system initialization.
 */
/*--------------------------------------------------------------------------------------------------------------------*/
#ifndef MICROS_INIT_H
#define MICROS_INIT_H
/*--------------------------------------------------------------------------------------------------------------------*/
#define MICROS_REGISTER_INIT_FUNCTION(func_name, priority)                 \
    __attribute__((section(".init_array." #priority), used)) static void ( \
        *__init_##func_name)(void) = func_name
/*--------------------------------------------------------------------------------------------------------------------*/
#define MICROS_REGISTER_INIT_EARLY_FUNCTION(func_name, priority)              \
    __attribute__((section(".preinit_array." #priority), used)) static void ( \
        *__preinit_##func_name)(void) = func_name
/*--------------------------------------------------------------------------------------------------------------------*/
#define MICROS_REGISTER_FINISH_FUNCTION(func_name, priority)               \
    __attribute__((section(".fini_array." #priority), used)) static void ( \
        *__fini_##func_name)(void) = func_name
/*--------------------------------------------------------------------------------------------------------------------*/
#endif /* MICROS_INIT_H */
/*--------------------------------------------------------------------------------------------------------------------*/
