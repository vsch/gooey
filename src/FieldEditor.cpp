//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "FieldEditor.h"
#include "Ssd1306Display.h"
#include "Utils.h"
#include "InterfaceManager.h"
#include "ButtonHandler.h"

int16_t const edit_1_5_10_FieldOffsets[] PROGMEM = {
        -10,
        -5,
        -1,
        1,
        5,
        10,
};

int16_t const edit_25_100_FieldOffsets[] PROGMEM = {
        -100,
        -25,
        25,
        100,
};

int16_t const edit_10_100_FieldOffsets[] PROGMEM = {
        -100,
        -10,
        10,
        100,
};

int16_t const edit_100_1000_FieldOffsets[] PROGMEM = {
        -1000,
        -100,
        100,
        1000,
};

int16_t const edit_05_1_5_FieldOffsets[] PROGMEM = {
        -5 * ENCODER_TICKS_PER_ML,
        -ENCODER_TICKS_PER_ML,
        -(ENCODER_TICKS_PER_ML / 2),
        ENCODER_TICKS_PER_ML / 2,
        ENCODER_TICKS_PER_ML,
        5 * ENCODER_TICKS_PER_ML,
};

FieldEditor::FieldEditor(uint8_t dummy) : PopupMenu(POPUP_MENU_STAY_OPEN, spcString, lengthof(edit_1_5_10_FieldOffsets)) {
    (void) dummy;

    fieldTypeFlags = FIELD_TYPE_1_5_10;
    fieldIndex = 0;
    fieldValue = 0;
    fieldMin = 0;
    fieldMax = 0;
    optionOffsets = edit_1_5_10_FieldOffsets;
    valueDivider = 1;
    valueDecimals = 0;
}

void FieldEditor::show(uint8_t id, uint8_t fieldType, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax) {
    show(id, fieldType, name, value, vMin, vMax, 1, 0);
}

void FieldEditor::show(uint8_t id, uint8_t fieldType, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax, int16_t vDivider, uint8_t vDecimals) {
    fieldIndex = id;
    optionTitle = reinterpret_cast<const char *>(name);
    fieldValue = value;
    fieldMin = vMin;
    fieldMax = vMax;
    valueDivider = vDivider;
    valueDecimals = vDecimals;
    fieldTypeFlags = fieldType;

    switch (fieldType & ~FIELD_TYPE_NO_INPLACE_EDIT) {
        case FIELD_TYPE_1_5_10 :
            optionCount = lengthof(edit_1_5_10_FieldOffsets);
            optionOffsets = edit_1_5_10_FieldOffsets;
            break;

        case FIELD_TYPE_25_100 :
            optionCount = lengthof(edit_25_100_FieldOffsets);
            optionOffsets = edit_25_100_FieldOffsets;
            break;

        case FIELD_TYPE_100_1000 :
            optionCount = lengthof(edit_100_1000_FieldOffsets);
            optionOffsets = edit_100_1000_FieldOffsets;
            break;

        case FIELD_TYPE_05_1_5 :
            optionCount = lengthof(edit_05_1_5_FieldOffsets);
            optionOffsets = edit_05_1_5_FieldOffsets;
            valueDivider = ENCODER_TICKS_PER_ML;
            valueDecimals = 1;
            break;

        default:
            break;
    }

    stayOpenOnSelect();
    //serialDebugHandlerAddPrintf_P(PSTR("FieldEditor stayOpen %d %S\n"), isStayOpenOnSelect(), serialDebugHandlersGetId(this));
    Popup::show();
}

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
    } else if (event == EVENT_MENU_SELECT && !(fieldTypeFlags & FIELD_TYPE_NO_INPLACE_EDIT) && (interfaceManager.getWantOptions() & INTERFACE_WANT_ADJ_SELECTION)) {
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
    if ((fieldTypeFlags & FIELD_TYPE_NO_INPLACE_EDIT) || !(interfaceManager.getWantOptions() & INTERFACE_WANT_ADJ_SELECTION)) {
        // don't have in-place mods, use menu
        retVal = PopupMenu::update();

        display.moveBy((coord_x) (display.getCharW() * 2), 2);

        // show all options and invert the selected one
        display.setColors(GFX_COLOR_WHITE, GFX_COLOR_NONE); // Draw 'normal' text
        display.printValue(PV_2X_SIZE_UNITS_ONLY | (valueDecimals & PV_DECIMALS), fieldValue, valueDivider, NULL);
    }
    return retVal;
}

void FieldEditor::added() {
    //serialDebugHandlerAddPrintf_P(PSTR("FieldEditor added() %S\n"), serialDebugHandlersGetId(this));
    PopupMenu::added();
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
    return Popup::activated(wantFlags) | INTERFACE_WANT_AUTO_REPEAT_SELECTION | INTERFACE_WANT_AUTO_REPEAT_NAVIGATION
           | (fieldTypeFlags & FIELD_TYPE_NO_INPLACE_EDIT ? 0 : INTERFACE_WANT_ADJ_SELECTION);
}
