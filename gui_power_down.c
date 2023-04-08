//
// Created by Vladimir Schneider on 2023-03-26.
//

#ifndef CONSOLE_DEBUG

#include "avr/sleep.h"

#endif

#include "Arduino.h"
#include "gui_power_down.h"
#include "gui_config.h"
#include "gui_pin_config.h"

/**
 * Handle Port D pin change interrupts PCINT16 - PCINT23
 *
 * Called when the unit is in power down sleep mode
 *
 * Disables sleep, disables Port D pc interrupts and disables button up/down pc interrupts
 */
#ifndef CONSOLE_DEBUG
ISR(PCINT2_vect) {
    sleep_disable();//Disable sleep mode

    PCICR &= ~0b00000100; // disable Port D id change interrupts
//    PCMSK1 &= ~0b00001100; // disable port C pins 2,3 change interrupts
    PCMSK2 &= ~0b11000000; // disable port D pins 6,7 change interrupts
}

#endif

void power_off() {
#ifndef CONSOLE_DEBUG
    serialDebugPowerDownPuts_P(PSTR("powering down."));
    delay(10); // to allow completion of sending

    gfx_display_off();
    sleep_enable();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    digitalWrite(LED_BUILTIN, LOW);

    // disable and turn off modules
    ADCSRA &= 0x7f;                 // disable ADC, needed before turning it off in PRR
    PRR = 0xEF;                     // turn off all modules

    // enable id change interrupts
    cli();
    PCICR |= 0b00000100;            // enable Port D id change interrupts
    PCICR &= ~0b00000010;           // disable Port C id change interrupts
//    PCMSK0 |= 0b00000000;         // PCINT0..7
//    PCMSK1 |= 0b00001100;         // PCINT8..15   enable 10 & 11 (encoder clk dt)
    PCMSK2 |= 0b11000000;           // PCINT16..23, enable 22 & 23
    sei();
    sleep_cpu();

    PRR = 0x00;                     // turn on all modules
    ADCSRA |= 0x80;                 // enable ADC

    delay(1);
    PCICR |= 0b00000010;            // enable Port C id change interrupts

#ifdef DEBUG_MODE_LED
    if (isPowerLED()) {
        digitalWrite(LED_BUILTIN, HIGH);
    }
#endif

    gfx_display_on();

    do {
        delay(20);
    } while (!digitalRead(pinBtnUp) || !digitalRead(pinBtnDown));
    serialDebugPowerDownPuts_P(PSTR("buttons released."));
#endif
}

