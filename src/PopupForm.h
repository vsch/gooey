//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_POPUPFORM_H
#define UI_POPUPFORM_H

#include "PopupOptionsMenu.h"

class PopupForm : public PopupOptionsMenu {
protected:
    uint8_t fieldsXCoord;

public:
    PopupForm(uint8_t flags, PGM_P title, PGM_STR_TABLE options, uint8_t count);

    virtual event_t process(event_t event);
    virtual void printOption(uint8_t id);
    virtual void updateField(uint8_t index, uint8_t selected) = 0;
    virtual void processField(uint8_t index) = 0;

    inline void setFieldsXCoord(uint8_t fieldsX) {
        fieldsXCoord = fieldsX;
    }

    serialDebugHandlersDeclareId("PopupForm");
};

#endif //UI_POPUPFORM_H
