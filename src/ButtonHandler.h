//
// Created by Vladimir Schneider on 2023-02-25.
//

#ifndef GUI_BUTTONHANDLER_H
#define GUI_BUTTONHANDLER_H

#include "../gui_config.h"
#include <Scheduler.h>

#ifndef CONSOLE_DEBUG

#include <WString.h>

#endif

#include "Events.h"

#define BUTTON_PRESSED            0x01    // set if button is pressed
#define BUTTON_AUTO_REPEATING     0x02    // set if button has passed auto repeat delay
#define BUTTON_LONG_PRESSED       0x04    // set if button is in long press
#define BUTTON_SKIP               0x08    // set if button press was during another button's press, wait for it to release

// CAUTION: code assumes _PR wantFlags are Non _PR ones << 2
#define BUTTON_LONG_PRESSABLE     0x10    // button long pressable flag
#define BUTTON_AUTO_REPEATABLE    0x20    // button repeatable flag
#define BUTTON_LONG_PRESSABLE_PR  0x40    // button long pressable flag as it was when button was pressed
#define BUTTON_AUTO_REPEATABLE_PR 0x80    // button repeatable flag as it was when button was pressed

#define BUTTON_TYPE_FLAGS (BUTTON_LONG_PRESSABLE | BUTTON_AUTO_REPEATABLE)
#define BUTTON_TYPE_FLAGS_PR (BUTTON_TYPE_FLAGS | BUTTON_LONG_PRESSABLE_PR | BUTTON_AUTO_REPEATABLE_PR)

#define BUTTON_AUTO_REPEAT_DELAY  0
#define BUTTON_REPEAT_RATE_DELAY  1
#define BUTTON_LONG_PRESS_DELAY   2

typedef struct Button {
    uint8_t buttonId;
    event_t eventPressed;
    event_t eventReleased;
    event_t eventLongPressed;
    event_t eventLongReleased;
} const *Button_P;

class ButtonHandler : public Task {
    uint8_t buttonCount;                    // button count
    Button_P buttonTable;                  // pointer to structure of button information in PROGMEM
    uint8_t *buttonFlags;                   // wantFlags for all buttons

    uint8_t buttonId;                       // active button id + 1
    uint16_t repeatCount;
    time_t pressedTimestamp;         // timestamp of when button was pressed

    virtual void begin();
    virtual void loop();
    defineSchedulerTaskId("ButtonHandler");
public:
    inline ButtonHandler(uint8_t *flags, Button_P buttons, uint8_t count) {
        buttonCount = count;
        buttonTable = buttons;
        buttonFlags = flags;
        memset(flags, 0, count * sizeof(*flags));

        buttonId = 0;
        repeatCount = 0;
        pressedTimestamp = 0;

    }

    void setButtonType(uint8_t id, uint8_t flags);     // set dynamic wantFlags: BUTTON_LONG_PRESSABLE, BUTTON_AUTO_REPEATABLE

    inline uint16_t getRepeatCount() const {
        return repeatCount;
    }
};

extern ButtonHandler buttonHandler;

extern "C" {
extern uint8_t ButtonHandler_is_button_pressed(uint8_t id);
extern uint16_t ButtonHandler_get_button_delay(uint8_t id, uint8_t delayType, uint16_t repeatCount);
}

#endif // GUI_BUTTONHANDLER_H
