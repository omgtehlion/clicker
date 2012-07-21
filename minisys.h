#ifndef __MINISYS_H__
#define __MINISYS_H__

#include "stdint.h"

typedef uint32_t miniTimer_t;
extern uint32_t msTicks;

void SysTickHandler(void);
void ResetMcu(void);

void delay(uint8_t ms);
void delay8(uint16_t ms);

void StartTimeout(miniTimer_t* timer, uint16_t ms);
void WaitTimeout(miniTimer_t* timer);

#define TMR3_PERIOD 989
#define SysTickHandler()                        \
{                                               \
    TMR3H = (65536 - TMR3_PERIOD) >> 8;         \
    TMR3L = (65536 - TMR3_PERIOD) & 0xFF;       \
    msTicks++;                                  \
}

#endif // __MINISYS_H__
