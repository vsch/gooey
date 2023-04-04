//
// Created by Vladimir Schneider on 2023-02-27.
//

#include "PopupMessage.h"
#include "Ssd1306Display.h"
#include "Utils.h"

const char okCancelTitle[] PROGMEM = { "" };
const char okOpt[] PROGMEM = { "OK" };

//const char cancelOpt[] PROGMEM = {"Cancel"};

PGM_P const okCancelFormOptions[] PROGMEM = {
        okOpt,
//        cancelOpt,
};

PopupMessage okCancelPopup = PopupMessage(okCancelTitle, okCancelTitle, okCancelFormOptions, lengthof(okCancelFormOptions));

PopupMessage::PopupMessage(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count) : PopupOptionsMenu(0, title, options, count) {
    popupMessage = (PGM_STR) message;
    baseEvent = EVENT_MESSAGE_CLOSE_MENU;
}

uint8_t PopupMessage::update() {
    PopupOptionsMenu::update(); // NOLINT(bugprone-parent-virtual-call)

    display.setLeftMargin(display.getX());
    display.setBackColor(GFX_COLOR_NONE); // don't fill in background
    display.printTextCentered(popupMessage, PRINT_CENTERED_Y | PRINT_CENTERED_X);
    return false;
}

void PopupMessage::show(PGM_P title, PGM_P message) {
    popupMessage = (PGM_STR) message;
    Popup::show(title);
}

void PopupMessage::show(PGM_P title, PGM_P message, uint8_t count) {
    popupMessage = (PGM_STR) message;
    PopupOptionsMenu::show(title, optionTable, count);
}

void PopupMessage::show(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count) {
    popupMessage = (PGM_STR) message;
    PopupOptionsMenu::show(title, options, count);
}

