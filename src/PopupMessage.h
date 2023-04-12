//
// Created by Vladimir Schneider on 2023-02-27.
//

#ifndef GUI_POPUPMESSAGE_H
#define GUI_POPUPMESSAGE_H

#include "PopupOptionsMenu.h"

class PopupMessage : public PopupOptionsMenu {
    PGM_STR popupMessage;

public:
    inline PopupMessage(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count) : PopupOptionsMenu(0, title, options, count) {
        popupMessage = (PGM_STR) message;
        baseEvent = EVENT_MESSAGE_CLOSE_MENU;
    }

    inline void show(PGM_P title, PGM_P message) {
        popupMessage = (PGM_STR) message;
        Popup::show(title);
    }

    inline void show(PGM_P title, PGM_P message, uint8_t count) {
        popupMessage = (PGM_STR) message;
        PopupOptionsMenu::show(title, optionTable, count);
    }

    inline void show(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count) {
        popupMessage = (PGM_STR) message;
        PopupOptionsMenu::show(title, options, count);
    }

    virtual uint8_t update(uint8_t useCachedState);
};

extern PopupMessage okCancelPopup;
extern const char okCancelTitle[] PROGMEM;
extern const char okOpt[] PROGMEM;
extern const char cancelOpt[] PROGMEM;


#endif // GUI_POPUPMESSAGE_H
