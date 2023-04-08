//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef GUI_POPUPOPTIONSMENU_H
#define GUI_POPUPOPTIONSMENU_H

#include "PopupMenu.h"
#include "ssd1306_gfx.h"
#include "GuiUtils.h"

#define POPUP_MENU_STAY_OPEN    0x01    // don't close on item select

class PopupOptionsMenu : public PopupMenu {
protected:
    PGM_STR_TABLE optionTable;

public:
    inline PopupOptionsMenu(uint8_t flags, PGM_P title, PGM_STR_TABLE options, uint8_t count) : PopupMenu(flags, title, count) {
        optionTable = options;
    }

    virtual void printOption(uint8_t id);
    PGM_STR getOption(uint8_t id) const;

    inline void show(PGM_P title, PGM_STR_TABLE options, uint8_t count) {
        optionTable = options;
        optionCount = count;
        Popup::show(title);
    }


#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

    inline void show() {
        Popup::show();
    }

#pragma clang diagnostic pop

    serialDebugHandlersDeclareId("PopupOptionsMenu");
};


#endif // GUI_POPUPOPTIONSMENU_H
