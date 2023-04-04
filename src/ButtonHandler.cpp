//
// Created by Vladimir Schneider on 2023-02-25.
//

#include "Arduino.h"
#include "Events.h"
#include "InterfaceManager.h"
#include "GooeyUtils.h"
#include "ButtonHandler.h"

extern InterfaceManager interfaceManager;

void ButtonHandler::begin() {
    serialDebugInitPuts_P(PSTR("ButtonHandler::begin()"));
    resume(250);
}

void ButtonHandler::loop() {
    uint8_t i;
    event_t event = EVENT_NONE;
    event_t secondEvent = EVENT_NONE;
    time_t timestamp = micros();

    for (i = 0; i < buttonCount; i++) {
        Button btn = {};
        btn.buttonId = pgm_read_byte(&buttonTable[i].buttonId);
        btn.eventPressed = pgm_read_byte(&buttonTable[i].eventPressed);
        btn.eventReleased = pgm_read_byte(&buttonTable[i].eventReleased);
        btn.eventLongPressed = pgm_read_byte(&buttonTable[i].eventLongPressed);
        btn.eventLongReleased = pgm_read_byte(&buttonTable[i].eventLongReleased);

        uint8_t flags = buttonFlags[i];

        if (!ButtonHandler_is_button_pressed(btn.buttonId)) {
            // button is released
            if (flags & BUTTON_PRESSED) {
                // just released
                if (!(flags & BUTTON_SKIP) && event == EVENT_NONE) {
                    // no active button, generate release event or if long pressable and not in long press, generate pressed event
                    buttonId = 0; // no more active button
                    repeatCount = 0;

                    if ((flags & BUTTON_LONG_PRESSABLE_PR) && !(flags & BUTTON_LONG_PRESSED)) {
                        // not enough for long press, generate press and release
                        serialDebugButtonsPuts_P(PSTR("button long pressable early release\n"));

                        event = btn.eventPressed;
                        secondEvent = btn.eventReleased;
                        repeatCount++;
                    } else {
                        event = flags & BUTTON_LONG_PRESSED ? btn.eventLongReleased : btn.eventReleased;
                    }
                } else {
                    // nothing, marked as skip or already have an event
                    serialDebugButtonsPuts_P(PSTR("button release skipped\n"));
                }

                // reset all wantFlags
                flags = 0;
            } else {
                // nothing to do, was already released
            }
        } else {
            // button pressed
            if (flags & BUTTON_PRESSED) {
                // was already pressed
                // see if in long press or auto-repeat or if have active button mark as skip
                time_t elapsedTime = timestamp - pressedTimestamp; // always positive

                if (flags & BUTTON_LONG_PRESSABLE_PR) {
                    if (flags & BUTTON_LONG_PRESSED) {
                        // already informed of long press

                    } else {
                        if (is_elapsed(elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_LONG_PRESS_DELAY, repeatCount))) {
                            serialDebugButtonsPrintf_P(PSTR("long pressed, elapsed %ld longPressDelay %d\n"), elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_LONG_PRESS_DELAY, repeatCount));

                            flags |= BUTTON_LONG_PRESSED;      // set long pressed and generate event
                            event = btn.eventLongPressed;
                            repeatCount++;
                        }
                    }
                } else if (flags & BUTTON_AUTO_REPEATABLE_PR) {
                    if (flags & BUTTON_AUTO_REPEATING) {
                        // in auto-repeat, if elapsed > repeat delay, generate event and reset pressedMicros
                        if (is_elapsed(elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_REPEAT_RATE_DELAY, repeatCount))) {
                            serialDebugButtonsPrintf_P(PSTR("button auto-repeat, elapsed %ld repeatRateDelay %d\n"), elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_REPEAT_RATE_DELAY, repeatCount));

                            pressedTimestamp = timestamp;
                            event = btn.eventPressed;
                            repeatCount++;
                        }
                    } else {
                        if (is_elapsed(elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_AUTO_REPEAT_DELAY, repeatCount))) {
                            // auto repeat delay met, set flag and generate first repeat
                            serialDebugButtonsPrintf_P(PSTR("button auto repeat delay, elapsed %ld autoRepeatDelay %d\n"), elapsedTime, ButtonHandler_get_button_delay(btn.buttonId, BUTTON_AUTO_REPEAT_DELAY, repeatCount));

                            flags |= BUTTON_AUTO_REPEATING;
                            pressedTimestamp = timestamp;
                            event = btn.eventPressed;
                            repeatCount++;
                        }
                    }
                } else {
                    // nothing to do, not repeatable or long pressable so event was generated on press
                }
            } else {
                // just pressed
                serialDebugButtonsPrintf_P(PSTR("button %d pressed\n"), btn.buttonId);

                flags |= BUTTON_PRESSED;

                if (!buttonId) {
                    // if long pressable, need to wait for it to release before can generate clicked event
                    buttonId = i + 1; // take the spot
                    pressedTimestamp = timestamp;

                    // copy pressable and repeatable wantFlags so their change while button is held down won't have an effect
                    flags &= ~(BUTTON_LONG_PRESSABLE_PR | BUTTON_AUTO_REPEATABLE_PR);
                    if (flags & BUTTON_LONG_PRESSABLE) flags |= BUTTON_LONG_PRESSABLE_PR;
                    if (flags & BUTTON_AUTO_REPEATABLE) flags |= BUTTON_AUTO_REPEATABLE_PR;

                    if (flags & BUTTON_LONG_PRESSABLE_PR) {
                        // have to wait before generating press, until after the long press delay or release

                    } else {
                        // can generate it now and repeat after delay
                        serialDebugButtonsPuts_P(PSTR("button press event\n"));
                        event = btn.eventPressed;
                    }
                } else {
                    // if have active button mark as skip
                    serialDebugButtonsPuts_P(PSTR("button press skipped\n"));
                    flags |= BUTTON_SKIP;
                }
            }
        }

        // save updated wantFlags
        if ((buttonFlags[i] & ~BUTTON_TYPE_FLAGS) != (flags & ~BUTTON_TYPE_FLAGS)) {
            // preserve button type wantFlags
            serialDebugButtonsPrintf_P(PSTR("updated wantFlags button %d to 0x%X\n"), btn.buttonId, (buttonFlags[i] & BUTTON_TYPE_FLAGS_PR) | (flags & ~BUTTON_TYPE_FLAGS_PR));
        }
        buttonFlags[i] &= BUTTON_TYPE_FLAGS;
        buttonFlags[i] |= flags & ~BUTTON_TYPE_FLAGS;
    }

    if (event != EVENT_NONE) {
        // process event
        serialDebugButtonEventsPrintf_P(PSTR("ButtonHandler.loop generating event '%c'\n"), event);

        interfaceManager.process(event);
        if (secondEvent != EVENT_NONE) {
            serialDebugButtonEventsPrintf_P(PSTR("   second event '%c'\n"), secondEvent);
            interfaceManager.process(secondEvent);
        }
    }

    resume(10);
}

void ButtonHandler::setButtonType(uint8_t id, uint8_t flags) {
    for (int i = 0; i < buttonCount; i++) {
        uint8_t btnPin = pgm_read_byte(&buttonTable[i].buttonId);
        if (btnPin == id) {
            buttonFlags[i] &= ~BUTTON_TYPE_FLAGS;
            buttonFlags[i] |= flags & BUTTON_TYPE_FLAGS;
            serialDebugButtonEventsPrintf_P(PSTR("changed type button %d to 0x%X\n"), btnPin, buttonFlags[i]);
            break;
        }
    }
}
