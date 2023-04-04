//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_POPUP_H
#define UI_POPUP_H

#include "InterfaceHandler.h"

class Popup : public InterfaceHandler {
protected:
    PGM_P optionTitle;
    event_t baseEvent;   // base events have to be defined so that +1 is for option 1, +2 for option 2, ...

public:
    explicit Popup(PGM_P title);

public:
    inline void show(PGM_P title) {
        optionTitle = title;
        show();
    }

    void show(); // used to insert into the interface handler
    void hide(); // used to remove from the interface handler

    inline void setBaseEvent(event_t event) {
        baseEvent = event;
    }

    inline void setTitle(PGM_P title) {
        optionTitle = title;
    }

    virtual uint8_t update();
    virtual void added();
    virtual void removed();
    virtual uint8_t activated(uint8_t wantFlags);

    virtual event_t process(event_t event) = 0;
};

#endif //UI_POPUP_H
