//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "Popup.h"
#include "GooeyUtils.h"
#include "InterfaceManager.h"
#include "ButtonHandler.h"

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
