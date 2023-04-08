//
// Created by Vladimir Schneider on 2023-02-25.
//

#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#pragma ide diagnostic ignored "modernize-use-nullptr"
#pragma ide diagnostic ignored "modernize-use-auto"

#include "gui_type_defs.h"

// enable serial debugging
// by itself only adds (flash/ram) bytes 138/34 if blocking or 810/210 in interrupt version
// burst transmit adds 76/0 bytes blocking, 98/0 in interrupt version
// puts_P adds 100 bytes flash
// printf_P adds 1492 bytes flash
//
// So the minimal option is No Interrupts, use puts_P, for a total of 238/34 (flash/ram) bytes used plus string length in the message

#ifndef CONSOLE_DEBUG
// CAUTION: SERIAL_DEBUG slows everything down
//#define SERIAL_DEBUG

#if defined(__cplusplus) && defined(DEBUG_MODE)

// only available as a class and in DEBUG_MODE
//#define PROFILE_HANDLER_UPDATE

#endif

#include <stdio.h>
#include "print.h"

#endif

#ifdef SERIAL_DEBUG
#ifndef DEBUG_MODE
#error SERIAL_DEBUG enabled for non DEBUG_MODE
#endif // DEBUG_MODE

#define SERIAL_DEBUG_VERSION            // print version to console on power up/reset
#define SERIAL_DEBUG_ERRORS             // send only critical errors
//#define SERIAL_DEBUG_INIT               // initialization events

// stats for TWI writes,
// NOTE: blocking takes almost 14ms to send display buffer,
//  non-blocking 16 uSec, and since this is the last send until next display refresh
//  it results in no cpu cycles lost to display update
//#define SERIAL_DEBUG_ADC
//#define SERIAL_DEBUG_BUTTONS          // button handler terse
//#define SERIAL_DEBUG_BUTTON_EVENTS      // button handler event generation
//#define SERIAL_DEBUG_CONFIG_VALUES    // if need to print config values on load
//#define SERIAL_DEBUG_EVENT_TRANSLATION          // button handler terse
//#define SERIAL_DEBUG_FIELD_EDITOR
//#define SERIAL_DEBUG_FIELD_EDITOR_FIELD_CHANGED
//#define SERIAL_DEBUG_FIELD_EDITOR_PROCESS
//#define SERIAL_DEBUG_GFX_STATS          // graphics rendering times, every time display updates
//#define SERIAL_DEBUG_GFX_TWI_STATS
//#define SERIAL_DEBUG_HANDLER_UPDATE     // display update of handler code
//#define SERIAL_DEBUG_HANDLERS           // interface manager handler processing
//#define SERIAL_DEBUG_HANDLER_ADD          // interface manager handler add/remove
//#define SERIAL_DEBUG_MOTOR
//#define SERIAL_DEBUG_MOTOR_CALIBRATION
//#define SERIAL_DEBUG_MOTOR_ENCODER
//#define SERIAL_DEBUG_PORTION
//#define SERIAL_DEBUG_PORTION          // portion control
//#define SERIAL_DEBUG_PORTION_POSITION   // encoder position desired vs current, only when motor is not stopped
//#define SERIAL_DEBUG_POWER_DOWN   // encoder position desired vs current, only when motor is not stopped
//#define SERIAL_DEBUG_PRINT_VALUE
//#define SERIAL_DEBUG_TRANSLATOR_WANT_OPTIONS   // interface manager handler processing

//#define DEBUG_SCHEDULER
//#define DEBUG_SCHEDULER_ERRORS
//#define DEBUG_SCHEDULER_DELAYS
//#define DEBUG_SCHEDULER_RUN

#endif

#ifdef SERIAL_DEBUG
#define serialDebugPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPrintf_P(...) ((void)0)
#define serialDebugPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_VERSION
#define serialDebugVersionPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugVersionPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugVersionPrintf_P(...) ((void)0)
#define serialDebugVersionPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_ERRORS
#define serialDebugErrorsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugErrorsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugErrorsPrintf_P(...) ((void)0)
#define serialDebugErrorsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_INIT
#define serialDebugInitPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugInitPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugInitPrintf_P(...) ((void)0)
#define serialDebugInitPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_GFX_TWI_STATS
#define serialDebugGfxTwiStatsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugGfxTwiStatsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugGfxTwiStatsPrintf_P(...) ((void)0)
#define serialDebugGfxTwiStatsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_GFX_STATS
#define serialDebugGfxStatsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugGfxStatsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugGfxStatsPrintf_P(...) ((void)0)
#define serialDebugGfxStatsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_HANDLER_UPDATE
#define serialDebugHandlerUpdatePrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugHandlerUpdatePuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugHandlerUpdatePrintf_P(...) ((void)0)
#define serialDebugHandlerUpdatePuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_ADC
#define serialDebugAdcPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugAdcPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugAdcPrintf_P(...) ((void)0)
#define serialDebugAdcPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_BUTTONS
#define serialDebugButtonsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugButtonsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugButtonsPrintf_P(...) ((void)0)
#define serialDebugButtonsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_BUTTON_EVENTS
#define serialDebugButtonEventsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugButtonEventsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugButtonEventsPrintf_P(...) ((void)0)
#define serialDebugButtonEventsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_EVENT_TRANSLATION
#define serialDebugEventTranslationPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugEventTranslationPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugEventTranslationPrintf_P(...) ((void)0)
#define serialDebugEventTranslationPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_TRANSLATOR_WANT_OPTIONS
#define serialDebugTranslatorWantOptionsPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugTranslatorWantOptionsPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugTranslatorWantOptionsPrintf_P(...) ((void)0)
#define serialDebugTranslatorWantOptionsPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_CONFIG_VALUES
#define serialDebugConfigValuesPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugConfigValuesPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugConfigValuesPrintf_P(...) ((void)0)
#define serialDebugConfigValuesPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_FIELD_EDITOR
#define serialDebugFieldEditorPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugFieldEditorPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugFieldEditorPrintf_P(...) ((void)0)
#define serialDebugFieldEditorPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_FIELD_EDITOR_FIELD_CHANGED
#define serialDebugFieldEditorFieldChangedPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugFieldEditorFieldChangedPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugFieldEditorFieldChangedPrintf_P(...) ((void)0)
#define serialDebugFieldEditorFieldChangedPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_FIELD_EDITOR_PROCESS
#define serialDebugFieldEditorProcessPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugFieldEditorProcessPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugFieldEditorProcessPrintf_P(...) ((void)0)
#define serialDebugFieldEditorProcessPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_HANDLERS

#define serialDebugHandlersPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugHandlersPuts_P(...) puts_P(__VA_ARGS__)
#define serialDebugHandlersDeclareId(str) virtual PGM_STR id() { return F(#str); }
#define serialDebugHandlersGetId(pHandler) (pHandler)->id()

#else

#define serialDebugHandlersPrintf_P(...) ((void)0)
#define serialDebugHandlersPuts_P(...) ((void)0)
#define serialDebugHandlersDeclareId(str)
#define serialDebugHandlersGetId(pHandler) ((PGM_P)str_empty)

#endif // SERIAL_DEBUG_HANDLERS

#ifdef SERIAL_DEBUG_MOTOR
#define serialDebugMotorPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugMotorPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugMotorPrintf_P(...) ((void)0)
#define serialDebugMotorPuts_P(...) ((void)0)
#endif

#if defined(SERIAL_DEBUG_HANDLER_ADD) || defined(SERIAL_DEBUG_HANDLERS)
#define serialDebugHandlerAddPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugHandlerAddPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugHandlerAddPrintf_P(...) ((void)0)
#define serialDebugHandlerAddPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_MOTOR_CALIBRATION
#define serialDebugMotorCalibrationPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugMotorCalibrationPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugMotorCalibrationPrintf_P(...) ((void)0)
#define serialDebugMotorCalibrationPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_MOTOR_ENCODER
#define serialDebugMotorEncoderPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugMotorEncoderPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugMotorEncoderPrintf_P(...) ((void)0)
#define serialDebugMotorEncoderPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_PORTION
#define serialDebugPortionPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPortionPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPortionPrintf_P(...) ((void)0)
#define serialDebugPortionPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_PORTION
#define serialDebugPortionPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPortionPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPortionPrintf_P(...) ((void)0)
#define serialDebugPortionPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_PORTION_POSITION
#define serialDebugPortionPositionPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPortionPositionPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPortionPositionPrintf_P(...) ((void)0)
#define serialDebugPortionPositionPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_POWER_DOWN
#define serialDebugPowerDownPrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPowerDownPuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPowerDownPrintf_P(...) ((void)0)
#define serialDebugPowerDownPuts_P(...) ((void)0)
#endif

#ifdef SERIAL_DEBUG_PRINT_VALUE
#define serialDebugPrintValuePrintf_P(...) printf_P(__VA_ARGS__)
#define serialDebugPrintValuePuts_P(...) puts_P(__VA_ARGS__)
#else
#define serialDebugPrintValuePrintf_P(...) ((void)0)
#define serialDebugPrintValuePuts_P(...) ((void)0)
#endif

#ifdef PROFILE_HANDLER_UPDATE

#include "util/TimeIt.h"

#define profileHandlerUpdateDeclare TimeIt profileHandlerUpdate
extern profileHandlerUpdateDeclare;
#define profileHandlerUpdateStart()  profileHandlerUpdate.start()
#define profileHandlerUpdateEnd()  profileHandlerUpdate.end()
#define profileHandlerUpdatePrintf_P(...) printf_P(__VA_ARGS__)
#else
#define profileHandlerUpdateDeclare
#define profileHandlerUpdateStart()  (void)0
#define profileHandlerUpdateEnd()  (void)0
#define profileHandlerUpdatePrintf_P(...) (void)0
#endif

#endif // GUI_CONFIG_H
