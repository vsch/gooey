//
// Created by Vladimir Schneider on 2023-02-26.
//

#include "ConfigForm.h"
#include "GooeyUtils.h"
#include "FieldEditor.h"
#include "ssd1306_gfx.h"
#include "Ssd1306Display.h"
#include "InterfaceManager.h"

ConfigForm::ConfigForm(PGM_P title, PGM_STR_TABLE options, uint8_t count, const FieldData *fields) : PopupForm(0, title, options, count) {
    fieldTable = fields;
}

event_t ConfigForm::process(event_t event) {
    if (event == EVENT_CLOSE_MENU) {
        // editor field menu closed
        // this is select and remove self from interfaceManager
        uint8_t id = fieldEditor.fieldId();
        uint16_t value = fieldEditor.value();
        fieldEditor.setFieldUpdater(NULL);
        setField(id, value);
        return EVENT_NONE;
    } else {
        return PopupForm::process(event);
    }
}

// @formatter:off
void updater_uint8(void *pField) { display.print(*(uint8_t *) pField); }
void updater_int8(void *pField) { display.print(*(int8_t *) pField); }
void updater_uint16(void *pField) { display.print(*(uint16_t *) pField); }
void updater_int16(void *pField) { display.print(*(int16_t *) pField); }
void updater_bool(void *pField) { display.print(((fieldBool_f) pField)() ? 'Y' : 'N'); }
void updater_percent(void *pField) { display.printValue(0, *(uint8_t *) pField, 1, F("%")); }
void inplace_updater_uint8(void *pField) { display.print((uint8_t) *(int16_t *) pField); }
void inplace_updater_int8(void *pField) { display.print((int8_t) *(int16_t *) pField); }
void inplace_updater_uint16(void *pField) { display.print((uint16_t) *(int16_t *) pField); }
void inplace_updater_int16(void *pField) { display.print(*(int16_t *) pField); }
void inplace_updater_bool(void *pField) { display.print(*(int16_t *) pField ? 'Y' : 'N'); }
void inplace_updater_percent(void *pField) { display.printValue(0, (uint8_t) *(int16_t *) pField, 1, F("%")); }
void processor_int8(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax) { fieldEditor.show(index, offsetTable, offsetCount, fieldName, *(int8_t *) pField, vMin, vMax); }
void processor_uint8(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax) { fieldEditor.show(index, offsetTable, offsetCount, fieldName, *(uint8_t *) pField, vMin, vMax); }
void processor_uint16(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax) { fieldEditor.show(index, offsetTable, offsetCount, fieldName, (int16_t) *(uint16_t *) pField, vMin, vMax); }
void processor_int16(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax) { fieldEditor.show(index, offsetTable, offsetCount, fieldName, *(int16_t *) pField, vMin, vMax); }
void processor_percent(uint8_t index, const int16_t *offsetTable, uint8_t offsetCount, PGM_STR fieldName, void *pField, int16_t vMin, int16_t vMax) { fieldEditor.show(index, offsetTable, offsetCount, fieldName, *(uint8_t *) pField, vMin, vMax); }
void setter_int8(void *pField, int16_t value) { *(int8_t *) pField = (int8_t)(value < -128 ? -128 : value > 127 ? 127 : value); }
void setter_uint8(void *pField, int16_t value) { *(uint8_t *) pField = value < 0 ? 0 : value > 255 ? 255 : value; }
void setter_uint16(void *pField, int16_t value) { *(uint16_t *) pField = value < 0 ? 0 : value; }
void setter_int16(void *pField, int16_t value) { *(int16_t *) pField = value; }
void setter_percent(void *pField, int16_t value) { *(uint8_t *) pField = value < 0 ? 0 : value > 100 ? 100 : value; }
// @formatter:on

void ConfigForm::updateField(uint8_t index, uint8_t selected) {
    void *pField;

    if (selected && (interfaceManager.getWantOptions() & INTERFACE_WANT_ADJ_SELECTION)) {
        // in place mods
        if (interfaceManager.isBlinkOn()) {
            filedUpdater_f updater = (filedUpdater_f) pgm_read_ptr(&fieldTable[index].inPlaceUpdater);
            int16_t value = fieldEditor.value();
            pField = &value;
            updater(pField);
        }
    } else {
        pField = (void *) pgm_read_ptr(&fieldTable[index].pField);
        filedUpdater_f updater = (filedUpdater_f) pgm_read_ptr(&fieldTable[index].updater);
        updater(pField);
    }
}

void ConfigForm::processField(uint8_t index) {
    // pass our updater to fieldEditor
    fieldEditor.setFieldUpdater(fieldUpdater);

    void *pField = (void *) pgm_read_ptr(&fieldTable[index].pField);
    fieldProcessor_f processor = (fieldProcessor_f) pgm_read_ptr(&fieldTable[index].processor);
    uint8_t offsetCount = pgm_read_byte(&fieldTable[index].offsetCount);
    const int16_t *offsetTable = (const int16_t *)pgm_read_ptr(&fieldTable[index].offsetTable);
    if (offsetCount == 0) {
        // boolean field, processor is a toggle
        ((fieldBool_f) processor)();

        if (fieldUpdater) {
            // inform of value change since no fieldEditor is involved
            fieldUpdater->fieldChanged(index, 0, ((fieldBool_f) pField)());
        }
    } else {
        int16_t vMin = pgm_read_word(&fieldTable[index].vMin);
        int16_t vMax = pgm_read_word(&fieldTable[index].vMax);
        processor(index, offsetTable, offsetCount, getOption(index), pField, vMin, vMax);
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"

void ConfigForm::setField(uint8_t index, int16_t value) {
    fieldEditor.setFieldUpdater(fieldUpdater);

    void *pField = (void *) pgm_read_ptr(&fieldTable[index].pField);
    fieldSetter_f setter = (fieldSetter_f) pgm_read_ptr(&fieldTable[index].setter);

    if (fieldUpdater) {
        // let updater know field was set
        if (!setter) {
            // boolean, just get its value, pField is fieldBool_f to get its current value, which was already set by processField
            value = ((fieldBool_f) pField)();
        }
        fieldUpdater->fieldChanged(index, 1, value);
    }

    // boolean field, no setter
    if (setter) {
        uint16_t vMin = pgm_read_word(&fieldTable[index].vMin);
        uint16_t vMax = pgm_read_word(&fieldTable[index].vMax);
        setter(pField, value);
    }
}

#pragma clang diagnostic pop

void ConfigForm::removed() {
    PopupForm::removed();
    InterfaceManager_save_config();
}
