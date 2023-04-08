//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef GUI_POPUPFORM_H
#define GUI_POPUPFORM_H

#include "PopupOptionsMenu.h"

class PopupForm : public PopupOptionsMenu {
protected:
    uint8_t fieldsXCoord;

public:
    inline PopupForm(uint8_t flags, PGM_P title, PGM_STR_TABLE options, uint8_t count) : PopupOptionsMenu(flags | POPUP_MENU_STAY_OPEN | POPUP_MENU_OPTION_PER_LINE, title, options, count) {
        fieldsXCoord = DISPLAY_XSIZE * 3 / 4;
    }

    virtual event_t process(event_t event);
    virtual void printOption(uint8_t id);
    virtual void updateField(uint8_t index, uint8_t selected) = 0;
    virtual void processField(uint8_t index) = 0;

    inline void setFieldsXCoord(uint8_t fieldsX) {
        fieldsXCoord = fieldsX;
    }

    serialDebugHandlersDeclareId("PopupForm");
};

#endif // GUI_POPUPFORM_H
