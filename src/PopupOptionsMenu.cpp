//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "Arduino.h"
#include "PopupOptionsMenu.h"
#include "GooeyUtils.h"
#include "Ssd1306Display.h"

void PopupOptionsMenu::printOption(uint8_t id) {
    display.print(getOption(id));
}

PGM_STR PopupOptionsMenu::getOption(uint8_t id) const {
    return (PGM_STR) pgm_read_ptr(optionTable + id);
}

