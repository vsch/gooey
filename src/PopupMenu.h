//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef GUI_POPUPMENU_H
#define GUI_POPUPMENU_H

#include "Popup.h"

#define POPUP_MENU_STAY_OPEN        0x01    // don't close on item select
#define POPUP_MENU_OPTION_PER_LINE  0x02    // no wrapping, one option per line
#define POPUP_MENU_NO_INPLACE_EDIT  0x80    // don't use inplace edit, even when available

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
    inline PopupMenu(uint8_t flags, PGM_P title, uint8_t count) : Popup(title) {
        currentOption = 0;
        topOption = 0;
        optionCount = count;
        menuFlags = flags;
        fieldUpdater = NULL;
    }

    virtual event_t process(event_t event);
    virtual uint8_t update();
    virtual void added();
    virtual void removed();
    inline virtual uint8_t activated(uint8_t wantFlags) {
        return Popup::activated(wantFlags) | INTERFACE_WANT_AUTO_REPEAT_NAVIGATION | ((menuFlags & POPUP_MENU_OPTION_PER_LINE) ? INTERFACE_WANT_VERTICAL_MENU : 0);
    }

    virtual void printOption(uint8_t id) = 0;

    inline virtual bool isEnabledOption(uint8_t id) {
        return true;
    }

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

#endif // GUI_POPUPMENU_H
