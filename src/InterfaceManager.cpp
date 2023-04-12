//
// Created by Vladimir Schneider on 2023-02-25.
//

#include "Arduino.h"
#include "../gui_config.h"
#include "ssd1306.h"
#include "ssd1306_gfx.h"
#include "Ssd1306Display.h"
#include "InterfaceManager.h"
#include "GuiUtils.h"

Ssd1306Display display;

void InterfaceManager::hadAction() {
    lastActionTimestamp = micros();
}

// Display Task methods
void InterfaceManager::begin() {
    serialDebugInitPuts_P(PSTR("InterfaceManager::begin()"));

#ifdef GFX_PAGED_UPDATES
    for (; gfx_update_page_y1 <= DISPLAY_YSIZE; gfx_start_next_page()) {
        gfx_display();
    }
#else
    gfx_display();
#endif

    serialDebugGfxTwiStatsPuts_P(PSTR("Initial Display"));

    delay(250);
    gfx_set_inverted();
    delay(250);
    gfx_clear_inverted();

    hadAction();
    resume(50);
}

void InterfaceManager::loop() {
    time_t t = micros();

    if (!flashTimeout) {
        flashTimeout = t;
    } else {
        if (is_elapsed(t - flashTimeout, InterfaceManager_get_flash_delay())) {
            flashTimeout = t;
            if (isFastFlashOn()) {
                flags ^= INTERFACE_SLOW_FLASH_OFF;
            }
            flags ^= INTERFACE_FAST_FLASH_OFF;
        }
    }

    if (is_elapsed(t - lastActionTimestamp, InterfaceManager_get_power_down_delay() * 1000)) {
        InterfaceManager_power_down();
        hadAction();
    }

    update();
    resume(64);  // refresh 16 x / sec
}

// display update handling
uint8_t InterfaceManager::update() {
    display.clearDisplay();

#ifdef GFX_PAGED_UPDATES
    for (; gfx_update_page_y1 <= DISPLAY_YSIZE; gfx_start_next_page()) {
#endif

#if SERIAL_DEBUG_HANDLER_UPDATE
    time_t start = micros();
#endif
    profileHandlerUpdateStart();
    uint8_t i;
    for (i = handlerCount; i--;) {
        InterfaceHandler *pHandler = handlers[i];
        if (pHandler) {
            if (!pHandler->update()) break;
        }
    }
    profileHandlerUpdateEnd();
#ifdef SERIAL_DEBUG_HANDLER_UPDATE
    time_t end = micros();
    printf_P(PSTR("Handlers.update():\n   in %ld uSec\n"), end - start);
#endif

#if !defined(CONSOLE_DEBUG) || !defined(TESTING)
    display.display();
#endif

#ifdef GFX_PAGED_UPDATES
    }
#endif

    serialDebugGfxTwiStatsPuts_P(PSTR("Update Display"));
    return false;
}

// event processing
event_t InterfaceManager::process(event_t event) {
    // pass the event to most recent handler first
    event_t nextEvent = event;
    hadAction();

    do {
        event = nextEvent;
        nextEvent = EVENT_NONE;
        flags &= ~INTERFACE_HANDLER_REMOVED;

        uint8_t i;
        for (i = handlerCount; i--;) {
            InterfaceHandler *pHandler = handlers[i];

            if (pHandler) {
#ifdef SERIAL_DEBUG_HANDLERS
                time_t handlerStart = micros();
#endif
                event_t newEvent = pHandler->process(event);

                if (flags & INTERFACE_HANDLER_REMOVED) {
                    serialDebugHandlerAddPrintf_P(PSTR("removed in process() = '%c', %S\n"), newEvent, serialDebugHandlersGetId(pHandler));
                    nextEvent = newEvent;
                    break;
                }
#ifdef SERIAL_DEBUG_HANDLERS
                if (newEvent != event) {
                    time_t handlerEnd = micros();
                    printf_P(PSTR("%S.process('%c') = '%c' in %ld\n"), serialDebugHandlersGetId(pHandler), event, newEvent, handlerEnd - handlerStart);
                }
#endif
                if (newEvent != event) {
                    // restart if there is an event from the top, we don't know exactly which handler was removed
                    nextEvent = newEvent;
                    break;
                }
            }
        }
    } while (nextEvent != EVENT_NONE);

    return EVENT_NONE;
}

// manager methods
void InterfaceManager::add(InterfaceHandler *handler) {
    if (handlerCount >= MAX_DISPLAY_HANDLERS) {
        serialDebugErrorsPrintf_P(PSTR("Cannot add handler %S.\n"), serialDebugHandlersGetId(handler));
    } else {
        uint8_t i = findHandler(handler);

        if (i < handlerCount) {
            // already have it
            serialDebugErrorsPrintf_P(PSTR("Handler %S already added..\n"), serialDebugHandlersGetId(handler));
        } else {
            handlers[handlerCount++] = handler;
            serialDebugHandlerAddPrintf_P(PSTR("added handler[%d] %S\n"), handlerCount - 1, serialDebugHandlersGetId(handler));
            handler->added();
            setMenuWantOptions();
        }
    }
}

void InterfaceManager::setMenuWantOptions() {
    uint8_t wantOptions = wantFlags;
    for (uint8_t i = handlerCount; i--;) {
        wantOptions = handlers[i]->activated(wantOptions);
        if (wantOptions != INTERFACE_WANT_ASK_PREVIOUS) {
            break;
        }
    }

    wantFlags = wantOptions;

    wantOptions = 0;
    for (uint8_t i = 0; i < handlerCount; i++) {
        wantOptions |= handlers[i]->changedWantOptions(wantFlags);
    }
    wantFlags &= wantOptions;
    serialDebugTranslatorWantOptionsPrintf_P(PSTR("Have: 0x%02x\n"), wantFlags);
}

void InterfaceManager::remove(InterfaceHandler *handler) {
    uint8_t i = findHandler(handler);

    if (i < handlerCount) {
        serialDebugHandlerAddPrintf_P(PSTR("removing handler[%d] %S\n"), i, serialDebugHandlersGetId(handler));

        handlers[i] = NULL; // just to be safe

        handlerCount--;
        for (; i < handlerCount; i++) {
            handlers[i] = handlers[i + 1];
            handlers[i + 1] = NULL; // just to be safe
        }
    } else {
        serialDebugHandlerAddPrintf_P(PSTR("did not find handler[%d] %S\n"), i, serialDebugHandlersGetId(handler));
    }

    // call it anyways
    flags |= INTERFACE_HANDLER_REMOVED;
    handler->removed();
    setMenuWantOptions();
    serialDebugHandlerAddPrintf_P(PSTR("Got want options\n"));
}

uint8_t InterfaceManager::findHandler(InterfaceHandler *handler) {
    for (uint8_t i = handlerCount; i--;) {
        if (handlers[i] == handler) return i;
    }
    return -1;
}
