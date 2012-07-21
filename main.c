#define USE_OR_MASKS
#include "stdint.h"
#include <p18cxxx.h>

#include "leds.h"
#include "minisys.h"

#pragma config WDT = OFF // WDT disabled
#pragma config PBADEN = OFF // PORTB<4:0> pins are configured as digital I/O on Reset
#pragma config DEBUG = OFF // Background debugger disabled, RB6 and RB7  as general purpose I/O
#pragma config CCP2MX = ON // If the Configuration bit is cleared, CCP2 is multiplexed with RB3
#pragma config FOSC = INTOSCIO_EC // IntOsc, EC Osc for USB, Digital I/O on RA6
#pragma config LVP = OFF
#pragma config XINST = ON

#ifdef FASTBUILD
    #include "leds.c"
    #include "minimath.c"
    #include "minisys.c"
    #include "rand.c"
    #include "stars.c"
#endif

void runFrame(void);
void runStart(void);

#define PIN_INPUT (1 << 4)
#define PIN_OUTPUT (1 << 2)
#define PIN_RESET (1 << 1)
#define DIGITS 5

void main(void)
{
    INTCONbits.GIEH = 0; // disable all interrupts
    RCONbits.IPEN = 1; // enable priority levels
    OSCCON = (7 << 4) | 2; // 8MHz int osc

    //TRISC &= ~0x03;

    // setup sysTick at timer 3:
    T3CON = (1 << 4) | (0 << 1) | 1; // prescaler = 1:2, clock source = Fosc/4, enabled
    PIE2bits.TMR3IE = 1; // enable int
    IPR2bits.TMR3IP = 1; // high priority int

    initLeds();

    // enable interrupts
    INTCONbits.GIEL = 1;
    INTCONbits.GIEH = 1;

    TRISA |= PIN_INPUT;
    TRISC &= ~PIN_OUTPUT;
    TRISC |= PIN_RESET;

    runStart();
    while (1) {
        runFrame();
    }
}

//---------------------------------------------------------------//
// Interrupts

// High priority interrupt routine declared in leds.c

// Low priority interrupt routine
#pragma code
#pragma interruptlow InterruptHandlerLow
void InterruptHandlerLow(void)
{
    if (PIR1bits.RCIF) {
        //RxInterrupt();
        PIR1bits.RCIF = 0;
    }
    if (INTCON3bits.INT2F) {
        //RB2Interrupt();
        INTCON3bits.INT2F = 0;
    }
}

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
    _asm GOTO InterruptHandlerHigh _endasm
}

#pragma code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
    _asm GOTO InterruptHandlerLow _endasm
}

//---------------------------------------------------------------//

uint8_t digitMap[10] = {
126	,
36	,
186	,
188	,
228	,
220	,
222	,
52	,
254	,
252	,
};

uint8_t volatile counter[DIGITS];
void displayCounter(void)
{
    int8_t hasValue = 0;
    int8_t i;
    for (i = 0; i < DIGITS; i++) {
        if (hasValue || counter[i]) {
            hasValue = 1;
            digits[i] = digitMap[counter[i]];
        } else {
            digits[i] = 0;
        }
    }
}
void incrCounter(void)
{
    int8_t i = DIGITS - 1;
    while (i >= 0 && (++counter[i] >= 10)) {
        counter[i] -= 10;
        i--;
    }
    displayCounter();
}
void resetCounter(void)
{
    uint8_t i;
    for (i = 0; i < DIGITS; i++)
       counter[i] = 0;
    displayCounter();
}

//---------------------------------------------------------------//

void runStart()
{
    uint8_t i, j;
    miniTimer_t timer;
    
    LATC &= ~PIN_OUTPUT;
    resetCounter();

    for (j = 0; j < 10; j++) {
        StartTimeout(&timer, 180);
        for (i = 0; i < DIGITS; i++) {
            digits[i] = digitMap[(i + j) % 10];
        }
        WaitTimeout(&timer);
    }
    displayCounter();
}

void runFrame()
{
    miniTimer_t timer;

    if (!(PORTA & PIN_INPUT)) {
        StartTimeout(&timer, 17);
        LATC |= PIN_OUTPUT;
        WaitTimeout(&timer);
        StartTimeout(&timer, 18);
        LATC &= ~PIN_OUTPUT;
        WaitTimeout(&timer);

        incrCounter();
    } else if (!(PORTC & PIN_RESET)) {
        resetCounter();
    }
}
