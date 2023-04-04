//
// Created by Vladimir Schneider on 2023-02-27.
//

#ifndef UI_POPUPMESSAGE_H
#define UI_POPUPMESSAGE_H

#include "PopupOptionsMenu.h"

class PopupMessage : public PopupOptionsMenu {
    PGM_STR popupMessage;

public:
    PopupMessage(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count);
    void show(PGM_P title, PGM_P message);
    void show(PGM_P title, PGM_P message, uint8_t count);
    void show(PGM_P title, PGM_P message, PGM_STR_TABLE options, uint8_t count);
    virtual uint8_t update();
};

extern PopupMessage okCancelPopup;
extern const char okCancelTitle[] PROGMEM;
extern const char okOpt[] PROGMEM;
extern const char cancelOpt[] PROGMEM;

#endif //UI_POPUPMESSAGE_H
