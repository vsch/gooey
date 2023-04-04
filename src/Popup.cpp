//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "Popup.h"
#include "Utils.h"
#include "InterfaceManager.h"
#include "ButtonHandler.h"

Popup::Popup(PGM_P title) {
    optionTitle = title;
    baseEvent = EVENT_CLOSE_MENU;
}

void Popup::show() {
    interfaceManager.add(this);
}

void Popup::hide() {
    interfaceManager.remove(this);
}

uint8_t Popup::update() {
    // print title
    if (optionTitle) {
        drawTitle((PGM_P) optionTitle, 1);
    }
    return false;
}

void Popup::added() {
}

void Popup::removed() {
}

uint8_t Popup::activated(uint8_t wantFlags) {
    return INTERFACE_WANT_MENU_TRANSLATIONS | INTERFACE_WANT_MENU_OPTION;
}

