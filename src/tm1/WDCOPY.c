#include "common.h"

#include "tm1/WDCOPY.H"

void wdCopy(s32* dst, s32* src, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++) {
        *dst = *src;
        src++;
        dst++;
    }
}
