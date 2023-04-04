//
// Created by Vladimir Schneider on 2023-02-26.
//

#ifndef TRANSMITTER_CONFIGFORM_H
#define TRANSMITTER_CONFIGFORM_H

#include "PopupForm.h"

typedef bool (*fieldBool_f)();
typedef void (*filedUpdater_f)(void *pField);
typedef void (*fieldSetter_f)(void *pField, int16_t value);
typedef void (*fieldProcessor_f)(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);

struct FieldData {
    void *pField;
    const filedUpdater_f updater;
    const filedUpdater_f inPlaceUpdater;
    const fieldSetter_f setter;
    const fieldProcessor_f processor;
    const int16_t *offsetTable PROGMEM;
    uint8_t offsetCount;
    int16_t vMin;
    int16_t vMax;
};

#define FIELD_UINT8(data, offsetTable, vmin, vmax) { &(data), updater_uint8, inplace_updater_uint8, setter_uint8, processor_uint8, (offsetTable), lengthof(offsetTable), vmin, vmax }
#define FIELD_INT8(data, offsetTable, vmin, vmax) { &(data), updater_int8, inplace_updater_int8, setter_int8, processor_int8, (offsetTable), lengthof(offsetTable), vmin, vmax }
#define FIELD_INT16(data, offsetTable, vmin, vmax) { &(data), updater_int16, inplace_updater_int16, setter_int16, processor_int16, (offsetTable), lengthof(offsetTable), vmin, vmax }
#define FIELD_UINT16(data, offsetTable, vmin, vmax) { &(data), updater_uint16, inplace_updater_uint16, setter_uint16, processor_uint16, (offsetTable), lengthof(offsetTable), vmin, vmax }
#define FIELD_BOOL(func, toggle) { (void *)(func), updater_bool, inplace_updater_bool, NULL, (fieldProcessor_f)(toggle), NULL, 0, 0, 1 }
#define FIELD_PERCENT(data) { &(data), updater_percent, inplace_updater_percent, setter_percent, processor_percent, fieldOffsets_1_5_10, lengthof(fieldOffsets_1_5_10), 0, 100 }

class ConfigForm : public PopupForm {
public:
    const FieldData *fieldTable;
    ConfigForm(PGM_P title, PGM_STR_TABLE options, uint8_t count, const FieldData *fields);

    virtual void updateField(uint8_t index, uint8_t selected);
    virtual void processField(uint8_t index);
    virtual event_t process(event_t event);
    virtual void removed();

public:

    // store value after edit
    void setField(uint8_t index, int16_t value);

    void show() {
        PopupForm::show();
    }

    inline uint8_t getOptionCount() {
        return optionCount;
    }
};

extern const char configTitle[] PROGMEM;
extern void updater_uint8(void *pField);
extern void updater_int8(void *pField);
extern void updater_uint16(void *pField);
extern void updater_int16(void *pField);
extern void updater_bool(void *pField);
extern void updater_percent(void *pField);
extern void inplace_updater_uint8(void *pField);
extern void inplace_updater_int8(void *pField);
extern void inplace_updater_uint16(void *pField);
extern void inplace_updater_int16(void *pField);
extern void inplace_updater_bool(void *pField);
extern void inplace_updater_percent(void *pField);
extern void processor_int8(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);
extern void processor_uint8(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);
extern void processor_uint16(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);
extern void processor_int16(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);
extern void processor_percent(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax);
extern void setter_int8(void *pField, int16_t value);
extern void setter_uint8(void *pField, int16_t value);
extern void setter_uint16(void *pField, int16_t value);
extern void setter_int16(void *pField, int16_t value);
extern void setter_percent(void *pField, int16_t value);

#endif //TRANSMITTER_CONFIGFORM_H
