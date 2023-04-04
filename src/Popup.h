//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_POPUP_H
#define UI_POPUP_H

#include "InterfaceHandler.h"
#include "ssd1306_gfx.h"
#include "InterfaceManager.h"

class Popup : public InterfaceHandler {
protected:
    PGM_P optionTitle;
    event_t baseEvent;   // base events have to be defined so that +1 is for option 1, +2 for option 2, ...

public:
    explicit inline Popup(PGM_P title) {
        optionTitle = title;
        baseEvent = EVENT_CLOSE_MENU;
    }

public:
    inline void show(PGM_P title) {
        optionTitle = title;
        show();
    }

    virtual void added() = 0;
    virtual void removed() = 0;
    virtual event_t process(event_t event) = 0;
    virtual uint8_t update();

    inline virtual uint8_t activated(uint8_t wantFlags) {
        return INTERFACE_WANT_MENU_TRANSLATIONS | INTERFACE_WANT_MENU_OPTION;
    }

    void show(); // used to insert into the interface handler
    void hide(); // used to remove from the interface handler

    inline void setBaseEvent(event_t event) {
        baseEvent = event;
    }

    inline void setTitle(PGM_P title) {
        optionTitle = title;
    }
};

#endif //UI_POPUP_H
