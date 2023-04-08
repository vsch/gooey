//
// Created by Vladimir Schneider on 2023-03-30.
//

#ifndef GUI_TYPE_DEFS_H
#define GUI_TYPE_DEFS_H

#define ENCODER_T_MIN   (INT16_MIN)
#define ENCODER_T_MAX   (INT16_MAX)
typedef int16_t encoder_t;  // encoder startPosition values

#ifndef CONSOLE_DEBUG
typedef unsigned long time_t; // timestamps
#else
#include <time.h>
#endif

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#endif // GUI_TYPE_DEFS_H
