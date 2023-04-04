//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_POPUPMENU_H
#define UI_POPUPMENU_H

#include "Popup.h"

#define POPUP_MENU_STAY_OPEN        0x01    // don't close on item select
#define POPUP_MENU_OPTION_PER_LINE  0x02    // no wrapping, one option per line

struct FieldUpdater {
    // print given field at current cursorX and cursorY coordinates
    // or nothing if no info for the selection
    virtual void updateField(uint8_t index) = 0;
    virtual void fieldChanged(uint8_t index, uint8_t setField, int16_t value) = 0;
};

class PopupMenu : public Popup {
protected:
    uint8_t menuFlags;
    uint8_t currentOption;
    uint8_t topOption;
    uint8_t optionCount;
    FieldUpdater *fieldUpdater;

    void makeVisible(uint8_t index);
public:
    PopupMenu(uint8_t flags, PGM_P title, uint8_t count);

    virtual event_t process(event_t event);
    virtual uint8_t update();
    virtual void added();
    virtual void removed();
    virtual void printOption(uint8_t id) = 0;
    virtual bool isEnabledOption(uint8_t id);
    virtual uint8_t activated(uint8_t wantFlags);

    inline uint8_t getCurrentOption() const {
        return currentOption;
    }

    inline uint8_t setCurrentOption(uint8_t selection) {
        uint8_t retVal = currentOption;
        currentOption = selection;
        return retVal;
    }

    inline void setFieldUpdater(FieldUpdater *updater) {
        fieldUpdater = updater;
    }

    inline uint8_t isStayOpenOnSelect() const {
        return menuFlags & POPUP_MENU_STAY_OPEN;
    }

    inline void stayOpenOnSelect() {
        menuFlags |= POPUP_MENU_STAY_OPEN;
    }

    inline void hideOnSelect() {
        menuFlags &= ~POPUP_MENU_STAY_OPEN;
    }

    serialDebugHandlersDeclareId("PopupMenu");
};

#endif //UI_POPUPMENU_H
