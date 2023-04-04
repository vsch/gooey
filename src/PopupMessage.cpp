//
// Created by Vladimir Schneider on 2023-02-27.
//

#include "PopupMessage.h"
#include "Ssd1306Display.h"
#include "Utils.h"

const char okCancelTitle[] PROGMEM = { "" };
const char okOpt[] PROGMEM = { "OK" };

const char cancelOpt[] PROGMEM = { "Cancel" };

PGM_P const okCancelFormOptions[] PROGMEM = {
        okOpt,
        cancelOpt,
};

PopupMessage okCancelPopup = PopupMessage(okCancelTitle, okCancelTitle, okCancelFormOptions, lengthof(okCancelFormOptions));

uint8_t PopupMessage::update() {
    PopupOptionsMenu::update();

    display.setLeftMargin(display.getX());
    display.setBackColor(GFX_COLOR_NONE); // don't fill in background
    display.printTextCentered(popupMessage, PRINT_CENTERED_Y | PRINT_CENTERED_X);
    return false;
}

