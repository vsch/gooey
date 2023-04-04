//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_POPUPOPTIONSMENU_H
#define UI_POPUPOPTIONSMENU_H

#include "PopupMenu.h"
#include "ssd1306_gfx.h"
#include "Utils.h"

#define POPUP_MENU_STAY_OPEN    0x01    // don't close on item select

class PopupOptionsMenu : public PopupMenu {
protected:
    PGM_STR_TABLE optionTable;

public:
    PopupOptionsMenu(uint8_t flags, PGM_P title, PGM_STR_TABLE options, uint8_t count);
    virtual void printOption(uint8_t id);
    PGM_STR getOption(uint8_t id) const;
    void show(PGM_P title, PGM_STR_TABLE options, uint8_t count);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

    void show();

#pragma clang diagnostic pop

    serialDebugHandlersDeclareId("PopupOptionsMenu");
};

#endif //UI_POPUPOPTIONSMENU_H
