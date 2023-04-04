//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "Arduino.h"
#include "PopupOptionsMenu.h"
#include "Utils.h"
#include "Ssd1306Display.h"

PopupOptionsMenu::PopupOptionsMenu(uint8_t flags, PGM_P title, PGM_STR_TABLE options, uint8_t count) : PopupMenu(flags, title, count) {
    optionTable = options;
}

void PopupOptionsMenu::printOption(uint8_t id) {
    display.print(getOption(id));
}

PGM_STR PopupOptionsMenu::getOption(uint8_t id) const {
    return (PGM_STR) pgm_read_ptr(optionTable + id);
}

void PopupOptionsMenu::show(PGM_P title, PGM_STR_TABLE options, uint8_t count) {
    optionTable = options;
    optionCount = count;
    Popup::show(title);
}

void PopupOptionsMenu::show() {
    Popup::show();
}
