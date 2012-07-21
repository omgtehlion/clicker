#define USE_OR_MASKS
#include "stdint.h"
#include <p18cxxx.h>

#include "leds.h"
#include "minisys.h"

uint8_t volatile digits[5];
const uint8_t digitPin[5] = { 1, 1 << 1, 1 << 2, 1 << 3, 1 << 5 };
uint8_t digitId;

void initLeds()
{
    LATB = 0xFF;
    TRISB = 0x00;
    LATA = 0x01;
    TRISA = 0xD0;

    digitId = 0;

    // setup timer 2:
    T2CON = (0 << 3) | (1 << 2) | 0x3; // postscaler = 1:1, timer on, prescaler = 1:16
    PR2 = 255; // period
    PIE1bits.TMR2IE = 1; // enable int
    IPR1bits.TMR2IP = 1; // high priority int
}

// High priority interrupt routine
//#pragma tmpdata isr_tmpdata
#pragma code
#pragma interrupt InterruptHandlerHigh nosave=PROD, PCLATH, PCLATU, section("MATH_DATA"), section(".tmpdata")
void InterruptHandlerHigh()
{
    if (PIR1bits.TMR2IF) {
        LATA = 0;
        if (++digitId >= 5) {
            digitId = 0;
        }
        LATB = digits[digitId];
        LATA = digitPin[digitId];
        PIR1bits.TMR2IF = 0;
    }
    if (PIR2bits.TMR3IF) {
        SysTickHandler();
        PIR2bits.TMR3IF = 0;
    }
}
