//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "Arduino.h"
#include "PopupMenu.h"
#include "GuiUtils.h"
#include "Ssd1306Display.h"
#include "InterfaceManager.h"

void PopupMenu::makeVisible(uint8_t index) {
    if (index > topOption + 2) {
        topOption = index - 2;
        if (topOption + 3 > optionCount) {
            topOption = optionCount - 3;
        }
    } else if (index < topOption) {
        topOption = index;
    }
}

void PopupMenu::added() {
    //serialDebugHandlerAddPrintf_P(PSTR("PopupMenu added() %S\n"), serialDebugHandlersGetId(this));
    if (!interfaceManager.isPreservePopupSelection() || currentOption >= optionCount) {
        // reset selection when it is added, if selection not preserved
        currentOption = 0;
        topOption = 0;
    }
}

void PopupMenu::removed() {
    hideOnSelect();
    fieldUpdater = NULL;
}

event_t PopupMenu::process(event_t event) {
    event_t retVal = EVENT_NONE;
    bool enabledOption = isEnabledOption(currentOption);

    switch (event) {
        case EVENT_MENU_SELECT:
            // this is select and remove self from interfaceManager
            //serialDebugHandlerAddPrintf_P(PSTR("PopupMenu select, stayOpen %d, %S\n"), (int16_t)isStayOpenOnSelect(), serialDebugHandlersGetId(this));

            if (optionCount != 0 && enabledOption) {
                retVal = baseEvent + 1 + currentOption; // base events have to be defined so that +1 is for option 1, +2 for option 2, ...
            }

            if ((menuFlags & POPUP_MENU_STAY_OPEN) || !enabledOption) {
                //serialDebugHandlerAddPrintf_P(PSTR("PopupMenu staying open '%c', %S\n"), retVal, serialDebugHandlersGetId(this));
                break;
            }
            // fall through

        case EVENT_MENU_OPTION:
            // this is menu cancel, remove this popup menu from interface processing and if no other events, send base method
            if (optionCount == 0 || (menuFlags & POPUP_MENU_STAY_OPEN)) {
                retVal = baseEvent;
            }
            //serialDebugHandlerAddPrintf_P(PSTR("PopupMenu closing '%c', %S\n"), retVal, serialDebugHandlersGetId(this));
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

uint8_t PopupMenu::update() {
    // show all options and invert the selected one
    // serialDebugHandlerAddPrintf_P(PSTR("PopupMenu update() %S\n"), serialDebugHandlersGetId(this));

    uint8_t i;
    uint8_t addSpc = false;
    uint8_t w, h;
    coord_x x;
    coord_y y;
    uint8_t iMax;

    Popup::update();

    if (menuFlags & POPUP_MENU_OPTION_PER_LINE) {
        makeVisible(currentOption);
        iMax = topOption + 3;
    } else {
        topOption = 0;
        iMax = optionCount;
    }

    coord_y top = display.getCursorY();
    coord_x left = display.getCursorX();

    for (i = topOption; i < iMax; i++) {
        display.setColors(GFX_COLOR_WHITE, GFX_COLOR_NONE); // Draw 'normal' text

        if (addSpc) {
            display.moveXBy((coord_x) (display.getCharW() * 3 / 4));
            addSpc = false;
        }

        if (!someSet(menuFlags, POPUP_MENU_OPTION_PER_LINE)) {
            display.startTextBounds();
            printOption(i);
            display.endTextBounds(&x, &y, &w, &h);

            if (h > display.getCharH() || x + w + display.getCharW() / 2 > DISPLAY_XSIZE) {
                // continue on the next line
                top += display.getCharH();
                display.moveTo(left, top);
                h = display.getCharH();
            }
            addSpc = true;
        } else {
            display.moveTo(left, top);
            top += display.getCharH();
            w = display.getRightMargin() - display.getLeftMargin();
            h = display.getCharH();
        }

        // print it for real
        x = display.getCursorX();
        y = display.getCursorY();

        printOption(i);

        if (i == currentOption) {
            if (someSet(menuFlags, POPUP_MENU_OPTION_PER_LINE)) {
                // invert the whole line if one item per line
                display.moveToX(DISPLAY_XSIZE);
            }

            // invert selected option, if disabled
            //display.setDrawTextBorder();
            display.setColors(GFX_COLOR_INVERT, isEnabledOption(i) ? GFX_COLOR_INVERT : GFX_COLOR_NONE);
            display.moveYBy(-1);
            display.rect((coord_x) (x - 2), (coord_y) (y + h - 1));
            display.moveYBy(1);
        }
    }

    // setup cursor for base class update
    display.moveTo(left, (coord_y) (top + (optionCount == 0 ? 0 : display.getCharH())));
    display.setLeftMargin(left);
    display.setRightMargin(DISPLAY_XSIZE);

    display.setColors(GFX_COLOR_WHITE, GFX_COLOR_NONE); // Draw 'normal' text
    if (fieldUpdater && currentOption < optionCount) {
        // update info for this field
        //serialDebugHandlerAddPrintf_P(PSTR("PopupMenu calling updateField(%d) %S\n"), currentOption, serialDebugHandlersGetId(this));
        fieldUpdater->updateField(currentOption);
    }
    return false;
}


