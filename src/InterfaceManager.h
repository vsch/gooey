//
// Created by Vladimir Schneider on 2023-02-25.
//

#ifndef GUI_INTERFACEMANAGER_H
#define GUI_INTERFACEMANAGER_H

#include "Arduino.h"
#include <Scheduler.h>
#include "ssd1306.h"
#include "Ssd1306Display.h"
#include "../gui_config.h"
#include "GuiUtils.h"
#include "InterfaceHandler.h"

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arguino mini and Pro mini: A4(SDA), A5(SCL)
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset id # (or -1 if sharing Arduino reset id)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define MAX_DISPLAY_HANDLERS 10

#define INTERFACE_WANT_MENU_TRANSLATIONS        0x01
#define INTERFACE_WANT_AUTO_REPEAT_SELECTION    0x02
#define INTERFACE_WANT_AUTO_REPEAT_NAVIGATION   0x04
#define INTERFACE_WANT_MENU_OPTION              0x08
#define INTERFACE_WANT_VERTICAL_MENU            0x10
#define INTERFACE_WANT_ADJ_SELECTION            0x20  // this is +/- to adjust values, if enabled values will be adjusted in place

#define INTERFACE_WANT_ASK_PREVIOUS             0xff

#define INTERFACE_SLOW_FLASH_OFF                0x01
#define INTERFACE_FAST_FLASH_OFF                0x02
#define INTERFACE_HANDLER_REMOVED               0x04  // set if handler is removed, reset by start process loop

extern "C" {
extern void InterfaceManager_power_down();
extern uint8_t InterfaceManager_is_preserve_popup_selection();
extern uint16_t InterfaceManager_get_power_down_delay();
extern uint16_t InterfaceManager_get_flash_delay();
}

class InterfaceManager : public Task {
    InterfaceHandler *handlers[MAX_DISPLAY_HANDLERS];
    uint8_t handlerCount;
    time_t flashTimeout;
    uint8_t flags;
    uint8_t wantFlags;
    time_t lastActionTimestamp;
    uint8_t currentPage;
    uint8_t powerOffDisabled;       // when non-zero, power down is disabled

    uint8_t updatePage(uint8_t updateDisplay);
public:
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"

    inline InterfaceManager() {
        memset(handlers, 0, sizeof(handlers));
        handlerCount = 0;
        flags = 0;
        wantFlags = 0;
        flashTimeout = 0;
        currentPage = 0;
        powerOffDisabled = 0;
    }

#pragma clang diagnostic pop

    void hadAction();
    void update();

    event_t process(event_t event);

    void add(InterfaceHandler *handler);
    void remove(InterfaceHandler *handler);
    uint8_t findHandler(InterfaceHandler *handler);

#ifdef CONSOLE_DEBUG
    inline uint8_t count() const {
            return handlerCount;
    };

    inline void removeHandlers(uint8_t keepCount) {
        while (keepCount < handlerCount) {
            remove(handlers[handlerCount - 1]);
        }
    }
#endif

    inline uint8_t isSlowFlashOn() const {
        return noneSet(flags, INTERFACE_SLOW_FLASH_OFF);
    }

    inline uint8_t isFastFlashOn() const {
        return noneSet(flags, INTERFACE_FAST_FLASH_OFF);
    }

    inline uint8_t isBlinkOn() const {
        return !allSet(flags, (INTERFACE_SLOW_FLASH_OFF | INTERFACE_FAST_FLASH_OFF));
    }

    inline bool isPreservePopupSelection() const {
        return InterfaceManager_is_preserve_popup_selection();
    }

    inline uint8_t getWantOptions() const {
        return wantFlags;
    }

    void enterProtectedPower();
    void leaveProtectedPower();

    /**
     * Dump the full screen to steam as ASCII text in the following form
     * /------------------------------------------------------------------\
     * |                   @@@         @                 @@@    @@    @@  |
     * |                  @   @        @                @   @  @  @  @  @ |
     * |   @@@@@@@@@      @   @ @   @ @@@    @@@        @   @  @     @    |
     * |  @               @   @ @   @  @    @   @       @   @ @@@   @@@   |
     * | @ @@@@@@@@@      @@@@@ @   @  @    @   @       @   @  @     @    |
     * |@ @               @   @ @  @@  @  @ @   @       @   @  @     @    |
     * |@@                @   @  @@ @   @@   @@@         @@@   @     @    |
     * \------------------------------------------------------------------/
     *
     * In paged update mode will invoke update() of current handlers for every page
     * in non-paged update will simply dump the current display buffer.
     *
     * @param stream     stream where to dump the screen
     */
    void printScreen(FILE *stream);

    uint8_t getPowerOffDisabled() const;

protected:
    virtual void loop();
    virtual void begin();
    void setMenuWantOptions();

    defineSchedulerTaskId("InterfaceManager");
};

extern InterfaceManager interfaceManager;

#endif // GUI_INTERFACEMANAGER_H
