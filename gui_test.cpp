//
// Created by Vladimir Schneider on 2023-03-26.
//
#include <Arduino.h>
#include <Scheduler.h>
#include "gui_config.h"
#include "gui_pin_config.h"
#include "src/InterfaceManager.h"
#include "src/ButtonHandler.h"
#include "src/Events.h"
#include "gui_power_down.h"

InterfaceManager interfaceManager = InterfaceManager();

// InterfaceManager interface
void InterfaceManager_power_down() {
    gui_power_off();
}

uint8_t InterfaceManager_is_preserve_popup_selection() {
    return true;
}

uint16_t InterfaceManager_get_power_down_delay() {
    return 30;
}

uint16_t InterfaceManager_get_flash_delay() {
    return 250;
}

// ButtonHandler button definitions, defines buttons to be handled
uint8_t ButtonHandler_is_button_pressed(uint8_t id) {
    return !digitalRead(id);
}

uint16_t ButtonHandler_get_button_delay(uint8_t id, uint8_t delayType, uint16_t repeatCount) {
    if (delayType == BUTTON_AUTO_REPEAT_DELAY) return 250;
    if (delayType == BUTTON_REPEAT_RATE_DELAY) return 100;
    if (delayType == BUTTON_LONG_PRESS_DELAY) return 500;
}

Button const buttonTable[] PROGMEM = {
        { pinBtnUp,   EVENT_MENU_NEXT_SELECTION, EVENT_NONE, EVENT_MENU_OPTION, EVENT_NONE },
        { pinBtnDown, EVENT_MENU_SELECT,         EVENT_NONE, EVENT_NONE,        EVENT_NONE },
};
uint8_t buttonFlags[lengthof(buttonTable)];
ButtonHandler buttonHandler = ButtonHandler(buttonFlags, buttonTable, lengthof(buttonTable));

Task *const taskTable[] PROGMEM = {
        &buttonHandler,
        &interfaceManager, // keep last
};

uint16_t delayTable[lengthof(taskTable)];
Scheduler scheduler = Scheduler(lengthof(taskTable), reinterpret_cast<PGM_P>(taskTable), delayTable);

void setup() {
    pinMode(pinBtnUp, INPUT_PULLUP);
    pinMode(pinBtnDown, INPUT_PULLUP);

    scheduler.begin();
}

void loop() {
    scheduler.loop();
}
