//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "PopupForm.h"
#include "GooeyUtils.h"
#include "FieldEditor.h"
#include "ssd1306_gfx.h"
#include "Ssd1306Display.h"

FieldEditor fieldEditor(0);

event_t PopupForm::process(event_t event) {
    event_t retVal = EVENT_NONE;

    switch (event) {
        case EVENT_MENU_SELECT:
            if (isEnabledOption(currentOption)) {
                processField(currentOption);
            }
            break;

        case EVENT_MENU_OPTION:
            // this is menu cancel, remove this popup menu from interface processing
            hide();
            break;

        case EVENT_MENU_NEXT_SELECTION:
            // this is next option with rollover to 1
            currentOption = (currentOption + 1) % optionCount;
            break;

        case EVENT_MENU_PREV_SELECTION:
            // this is next option with rollover to 1
            currentOption = (currentOption + optionCount - 1) % optionCount;
            break;

        default:
            retVal = event;
    }

    return retVal;
}

void PopupForm::printOption(uint8_t id) {
    PopupOptionsMenu::printOption(id);
    display.moveToX(fieldsXCoord);
    updateField(id, id == currentOption);
}
