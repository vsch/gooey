//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "FieldEditor.h"
#include "Ssd1306Display.h"
#include "Utils.h"
#include "InterfaceManager.h"
#include "ButtonHandler.h"

int16_t const fieldOffsets_1_5_10[] PROGMEM = {
        -10,
        -5,
        -1,
        1,
        5,
        10,
};

int16_t const fieldOffsets_25_100[] PROGMEM = {
        -100,
        -25,
        25,
        100,
};

int16_t const fieldOffsets_10_100[] PROGMEM = {
        -100,
        -10,
        10,
        100,
};

int16_t const fieldOffsets_100_1000[] PROGMEM = {
        -1000,
        -100,
        100,
        1000,
};

void FieldEditor::clipToMinMax() {
    if (fieldValue < fieldMin) {
        fieldValue = fieldMin;
    } else if (fieldValue > fieldMax) {
        fieldValue = fieldMax;
    }
}

void FieldEditor::adjustValue(int16_t delta) {
    int16_t oldValue = fieldValue;
    fieldValue += delta;
    clipToMinMax();

    if (fieldValue != oldValue) {
        if (fieldUpdater) {
            serialDebugFieldEditorPrintf_P(PSTR("Field %d changed to %d\n"), fieldIndex, fieldValue);
            fieldUpdater->fieldChanged(fieldIndex, 0, fieldValue);
        } else {
            serialDebugFieldEditorPrintf_P(PSTR("Field %d changed, to %d, no updater\n"), fieldIndex, fieldValue);
        }
    } else {
        serialDebugFieldEditorPrintf_P(PSTR("Field %d not changed, old %d, new %ld\n"), fieldIndex, oldValue, fieldValue);
    }
}

event_t FieldEditor::process(event_t event) {
    serialDebugFieldEditorProcessPrintf_P(PSTR("Fed: got %c\n"), event);
    event_t retVal = EVENT_NONE;

    if (event == EVENT_MENU_INC_SELECTION) {
        adjustValue(getOffset(buttonHandler.getRepeatCount() <= 20 || optionCount == 2 ? optionCount / 2 : optionCount / 2 + 1));
    } else if (event == EVENT_MENU_DEC_SELECTION) {
        adjustValue(getOffset(optionCount / 2 - (buttonHandler.getRepeatCount() <= 20 || optionCount == 2 ? 1 : 2)));
    } else if (event == EVENT_MENU_SELECT && !(menuFlags & POPUP_MENU_NO_INPLACE_EDIT) && (interfaceManager.getWantOptions() & INTERFACE_WANT_ADJ_SELECTION)) {
        // treat it as EVENT_MENU_OPTION since we are in place modifying
        retVal = EVENT_CLOSE_MENU;
        Popup::hide();
    } else {
        event_t popUpEvent = PopupMenu::process(event);
        serialDebugFieldEditorProcessPrintf_P(PSTR("Fed: PopupMenu: %c\n"), popUpEvent);

        if (popUpEvent >= EVENT_OPTION_0 && popUpEvent < EVENT_OPTION_9) {
            // these are ours
            serialDebugFieldEditorPrintf_P(PSTR("process %d idx: %d offs: %d\n"), popUpEvent, popUpEvent - EVENT_OPTION_0, getOffset(popUpEvent - EVENT_OPTION_0));

            if (popUpEvent - EVENT_OPTION_0 < optionCount) {
                adjustValue(getOffset(popUpEvent - EVENT_OPTION_0));
            }
        } else if (event == EVENT_MENU_OPTION) {
            // menu closed, we change it to EVENT_CLOSE_MENU so the value is stored
            retVal = EVENT_CLOSE_MENU;
        } else {
            retVal = popUpEvent;
        }
    }

    serialDebugFieldEditorProcessPrintf_P(PSTR("Fed: out %c\n"), retVal);
    return retVal;
}

uint8_t FieldEditor::update() {
    bool retVal = true;
    if ((menuFlags & POPUP_MENU_NO_INPLACE_EDIT) || !(interfaceManager.getWantOptions() & INTERFACE_WANT_ADJ_SELECTION)) {
        // don't have in-place mods, use menu
        retVal = PopupMenu::update();

        display.moveBy((coord_x) (display.getCharW() * 2), 2);

        // show all options and invert the selected one
        display.setColors(GFX_COLOR_WHITE, GFX_COLOR_NONE); // Draw 'normal' text
        display.printValue(PV_2X_SIZE_UNITS_ONLY | (valueDecimals & PV_DECIMALS), fieldValue, valueDivider, NULL);
    }
    return retVal;
}

void FieldEditor::printOption(uint8_t id) {
    uint8_t decimals = valueDecimals;
    int16_t value = (int16_t) (id < optionCount ? pgm_read_word(optionOffsets + id) : 0);

    // only use decimals on values that need them
    if (decimals) {
        int16_t v = (int16_t) (value < 0 ? -value : value);
        if (v % valueDivider == 0) {
            decimals = 0;
        }
    }
    display.printValue((decimals & PV_DECIMALS) | PV_ALWAYS_PRINT_SIGN, value, valueDivider, NULL);
}

int16_t FieldEditor::getOffset(uint8_t id) {
    return id < optionCount ? pgm_read_word(optionOffsets + id) : 0;
}

uint8_t FieldEditor::activated(uint8_t wantFlags) {
    return (PopupMenu::activated(wantFlags) & ~INTERFACE_WANT_ADJ_SELECTION) | INTERFACE_WANT_AUTO_REPEAT_SELECTION | INTERFACE_WANT_AUTO_REPEAT_NAVIGATION
           | (menuFlags & POPUP_MENU_NO_INPLACE_EDIT ? 0 : INTERFACE_WANT_ADJ_SELECTION);
}
