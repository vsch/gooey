//
// Created by Vladimir Schneider on 2023-03-10.
//

#ifndef GUI_INTERFACEHANDLER_H
#define GUI_INTERFACEHANDLER_H

#include <Arduino.h>
#include "../gui_config.h"
#include "Events.h"

// must be pure virtual class
class InterfaceHandler {
    friend class InterfaceManager;

public:
    virtual void added() = 0;                       // called when added to interface manager
    virtual void removed() = 0;                     // called when added to interface manager
    /**
     * Update handler display
     *
     * @param updatePage    0 means it is the first page update and values should be cached for subsequent page updates (if there are any, GFX_PAGED_UPDATES defined)
     *
     * @return true if other handlers should be given a chance to update the display, false to render whatever is there
     */
    virtual uint8_t update(uint8_t updatePage) = 0;  // return false to stop update calls down the chain of interface handlers
    virtual event_t process(event_t event) = 0;     // return EVENT_NONE if processed or replacement event or same event, if replacement then will restart event processing loop
    virtual uint8_t activated(uint8_t wantFlags) = 0;   // when event handler becomes top dog, return its want options for menu generated events

    // default provided since no one other than event translator needs this
    // return options which will be honoured, this will tell the UI what to expect
    virtual uint8_t changedWantOptions(uint8_t wantOptions) {
        return 0;
    }

#ifdef SERIAL_DEBUG_HANDLERS
    virtual PGM_STR id() = 0;
#endif
};

#endif // GUI_INTERFACEHANDLER_H
