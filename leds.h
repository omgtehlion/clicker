#ifndef __LEDS_H__
#define __LEDS_H__

#include "stdint.h"

#define LED_COUNT 14

extern volatile uint8_t digits[];
void initLeds(void);
void InterruptHandlerHigh(void);

#endif // __LEDS_H__
