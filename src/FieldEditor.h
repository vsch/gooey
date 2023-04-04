//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef UI_FIELDEDITOR_H
#define UI_FIELDEDITOR_H

#include "PopupForm.h"
#include "PopupOptionsMenu.h"
#include "ssd1306_gfx.h"

#define FIELD_TYPE_1_5_10       0
#define FIELD_TYPE_25_100       1
#define FIELD_TYPE_100_1000     2
#define FIELD_TYPE_05_1_5       3

#define FIELD_TYPE_NO_INPLACE_EDIT   0x80  // don't use inplace edit, even when available

class FieldEditor : public PopupMenu {
    uint8_t fieldTypeFlags;
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

    void show(uint8_t id, uint8_t fieldType, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax);
    void show(uint8_t id, uint8_t fieldType, PGM_STR name, int16_t value, int16_t vMin, int16_t vMax, int16_t vDivider, uint8_t vDecimals);

    virtual void printOption(uint8_t id);

    inline uint8_t fieldId() const {
        return fieldIndex;
    }

    inline int16_t value() const {
        return fieldValue;
    }

    int16_t getOffset(uint8_t id);

    virtual event_t process(event_t event);
    virtual uint8_t update();
    virtual void added();
    virtual uint8_t activated(uint8_t wantFlags);

    serialDebugHandlersDeclareId("FieldEditor");

    void clipToMinMax();
};

extern FieldEditor fieldEditor;

#endif //UI_FIELDEDITOR_H
