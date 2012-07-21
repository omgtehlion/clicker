#define USE_OR_MASKS
#include "stdint.h"
#include <p18cxxx.h>

#include "minisys.h"

uint32_t msTicks = 0;

void ResetMcu() {
    _asm reset _endasm
}

void delay(uint8_t ms)
{
    uint32_t target = msTicks + ms;
    if ((msTicks >> 24) & 0x80) {
        ResetMcu(); // die if we are too old ))
    }
    while (msTicks < target) {}
}

void delay8(uint16_t ms)
{
    uint32_t target = msTicks + ms;
    if ((msTicks >> 24) & 0x80) {
        ResetMcu(); // die if we are too old ))
    }
    while (msTicks < target) {}
}

void StartTimeout(miniTimer_t* timer, uint16_t ms)
{
    if ((msTicks >> 24) & 0x80) {
        ResetMcu(); // die if we are too old ))
    }
    *timer = msTicks + ms;
}

void WaitTimeout(miniTimer_t* timer)
{
    while (msTicks < *timer) {}
}
