//
// Created by Vladimir Schneider on 2023-02-25.
//

#ifndef UI_UTILS_H
#define UI_UTILS_H

#include <stdint.h>

#ifndef CONSOLE_DEBUG

#include <avr/pgmspace.h>

#endif

#include "Arduino.h"
#include "ssd1306_gfx.h"

#define PRINT_CENTERED_Y 1
#define PRINT_CENTERED_X 2

#ifdef __cplusplus

extern "C" {
#endif

extern void drawTitle(PGM_P pOpt, uint8_t round);

#define lengthof(a)  (sizeof(a)/sizeof((a)[0]))     // get length of array
#define memberOffset(t, m)  (&((t *)0)->m)           // get offset of type's member
typedef const char *const *PGM_STR_TABLE;

extern uint8_t is_elapsed(uint32_t elapsedMicros, uint16_t delayMillis);
extern uint8_t elapsed_progress(uint32_t elapsedMicros, uint16_t delayMillis);

inline uint8_t charBetween(char c, char c1, char c2) {
    return (uint8_t) c >= (uint8_t) c1 && (uint8_t) c <= (uint8_t) c2;
}

// fast calculate integer value of square root
uint8_t gfx_sqrt16(uint16_t val);
uint8_t gfx_sqrt8(uint8_t val);

#ifdef __cplusplus
}
#endif

#endif //UI_UTILS_H
