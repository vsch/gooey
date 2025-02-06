//
// Created by Vladimir Schneider on 2023-02-25.
//

#include "Arduino.h"
#include "GuiUtils.h"
#include "ssd1306_gfx.h"

uint8_t is_elapsed(uint32_t elapsedMicros, uint16_t delayMillis) {
    return elapsedMicros >= delayMillis * 1000L;
}

uint8_t is_elapsed_minutes(uint32_t elapsedMicros, uint32_t delayMillis) {
    return elapsedMicros >= delayMillis * 1000L;
}

// return percent complete in 0..255 with 255 being 100% complete
uint8_t elapsed_progress(uint32_t elapsedMicros, uint16_t delayMillis) {
    if (is_elapsed(elapsedMicros, delayMillis)) {
        return 255;
    }
    return elapsedMicros * 255 / 1000 / delayMillis;
}

void drawTitle(PGM_P pOpt, uint8_t round) {
    gfx_back_color = GFX_COLOR_NONE;
    gfx_set_text_flags(0);
    gfx_cursor_x = round ? 3 : 0;
    gfx_cursor_y = 2;
    gfx_double_hline_to(11, 2);
    gfx_cursor_x += 7;
    gfx_cursor_y -= 2;
    gfx_fputs_P(pOpt);
    gfx_cursor_x += 6;
    gfx_cursor_y += 2;
    gfx_double_hline_to(DISPLAY_XSIZE, 2);

    gfx_cursor_x = 0;
    gfx_cursor_y = DISPLAY_YSIZE - 1;
    if (round) {
        gfx_vline_to(5);
        gfx_line_to(3, 2);
        gfx_cursor_x = 1;
        gfx_cursor_y = 6;
        gfx_line_to(3, 4);
    } else {
        gfx_vline_to(2);
    }
    gfx_margin_left = gfx_cursor_x = 5;
    gfx_cursor_y = gfx_char_y_size;
}

// from https://stackoverflow.com/questions/34187171/fast-integer-square-root-approximation, tweaked for fast conversion
// and eliminate the off by one with a test at the end
uint8_t sqrt16(uint16_t val) {
    uint16_t a, b;

    if (val < 2) return val; /* avoid div/0 */
    a = val < 256 ? 8 : 128;       /* starting point relatively important for fast conversion */
// @formatter:off
    b = val / a; a = (a + b) / 2;
    b = val / a; a = (a + b) / 2;
// @formatter:on
    if (val >= 256) {
// @formatter:off
        b = val / a; a = (a + b) / 2;
        b = val / a; a = (a + b) / 2;
// @formatter:on
    }
    if (a * a > val) {
        a--;
    }
    return a;
}

uint8_t sqrt8(uint8_t val) {
    uint8_t a, b;

    if (val < 2) return val; /* avoid div/0 */
    a = 8;       /* starting point relatively important for fast conversion */
// @formatter:off
    b = val / a; a = (a + b) / 2;
    b = val / a; a = (a + b) / 2;
// @formatter:on
    if (a * a > val) {
        a--;
    }
    return a;
}

