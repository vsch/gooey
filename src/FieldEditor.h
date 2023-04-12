//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef GUI_FIELDEDITOR_H
#define GUI_FIELDEDITOR_H

#include "PopupForm.h"
#include "PopupOptionsMenu.h"
#include "ssd1306_gfx.h"

class FieldEditor : public PopupMenu {
    uint8_t fieldIndex;
    int16_t fieldValue;
    int16_t fieldMin;
    int16_t fieldMax;
    const int16_t *optionOffsets;
    int16_t valueDivider;
    uint8_t valueDecimals;

protected:
    void adjustValue(int16_t delta);

public:
    explicit FieldEditor(uint8_t dummy);

    void show(uint8_t id, const int16_t *offsetTable PROGMEM, uint8_t offsetCount, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax);
    void show(uint8_t id, const int16_t *offsetTable PROGMEM, uint8_t offsetCount, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax, int16_t vDivider, uint8_t vDecimals);

    virtual void printOption(uint8_t id);

    inline uint8_t fieldId() const {
        return fieldIndex;
    }

    inline int16_t value() const {
        return fieldValue;
    }

    /**
     * Disable in-place editing even when it is available.
     */
    inline void setNoEditInPlace() {
        menuFlags |= POPUP_MENU_NO_INPLACE_EDIT;
    }

    int16_t getOffset(uint8_t id);

    virtual event_t process(event_t event);
    virtual uint8_t update(uint8_t useCachedState);
    virtual uint8_t activated(uint8_t wantFlags);

    serialDebugHandlersDeclareId("FieldEditor");

    void clipToMinMax();
};

extern FieldEditor fieldEditor;
extern int16_t const fieldOffsets_1_5_10[6] PROGMEM;
extern int16_t const fieldOffsets_25_100[4] PROGMEM;
extern int16_t const fieldOffsets_10_100[4] PROGMEM;
extern int16_t const fieldOffsets_100_1000[4] PROGMEM;

inline FieldEditor::FieldEditor(uint8_t dummy) : PopupMenu(POPUP_MENU_STAY_OPEN, spcString, lengthof(fieldOffsets_1_5_10)) {
    (void) dummy;

    menuFlags = 0;
    fieldIndex = 0;
    fieldValue = 0;
    fieldMin = 0;
    fieldMax = 0;
    optionOffsets = fieldOffsets_1_5_10;
    valueDivider = 1;
    valueDecimals = 0;
}

inline void FieldEditor::show(uint8_t id, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax) {
    show(id, offsetTable, offsetCount, name, value, vMin, vMax, 1, 0);
}

inline void FieldEditor::show(uint8_t id, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax, int16_t vDivider, uint8_t vDecimals) {
    menuFlags &= ~POPUP_MENU_NO_INPLACE_EDIT;
    fieldIndex = id;
    optionTitle = reinterpret_cast<const char *>(name);
    fieldValue = value;
    fieldMin = vMin;
    fieldMax = vMax;
    valueDivider = vDivider;
    valueDecimals = vDecimals;
    optionCount = offsetCount;
    optionOffsets = offsetTable;
    stayOpenOnSelect();
    Popup::show();
}



#endif // GUI_FIELDEDITOR_H
