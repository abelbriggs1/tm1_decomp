#include "common.h"

// TODO: Determine what's going on with some of these functions.
// They appear to be thunked, but also jump into each other.

#ifdef NON_MATCHING
void BoundVector(s32* v)
{
    while (v[0] >= 2048) {
        v[0] = v[0] - 4096;
    }
    while (v[0] < -2048) {
        v[0] = v[0] + 4096;
    }
    while (v[1] >= 2048) {
        v[1] = v[1] - 4096;
    }
    while (v[1] < -2048) {
        v[1] = v[1] + 4096;
    }
    while (v[2] >= 2048) {
        v[2] = v[2] - 4096;
    }
    while (v[2] < -2048) {
        v[2] = v[2] + 4096;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", BoundVector);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", func_80150DE8);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", FUN_80150df8);
#endif

#ifdef NON_MATCHING
void BoundSVector(u16* v)
{
    while ((s16)v[0] >= 2048) {
        v[0] = v[0] - 4096;
    }
    while ((s16)v[0] < -2048) {
        v[0] = v[0] + 4096;
    }
    while ((s16)v[1] >= 2048) {
        v[1] = v[1] - 4096;
    }
    while ((s16)v[1] < -2048) {
        v[1] = v[1] + 4096;
    }
    while ((s16)v[2] >= 2048) {
        v[2] = v[2] - 4096;
    }
    while ((s16)v[2] < -2048) {
        v[2] = v[2] + 4096;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", BoundSVector);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", func_80150EF8);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", FUN_80150f08);
#endif

#ifdef NON_MATCHING
void BoundAngle(s32* a)
{
    while (a[0] >= 2048) {
        a[0] = a[0] - 4096;
    }
    while (a[0] < -2048) {
        a[0] = a[0] + 4096;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", BoundAngle);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", func_80151038);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", FUN_80151048);
#endif

#ifdef NON_MATCHING
void BoundSAngle(u16* a)
{
    while ((s16)a[0] >= 2048) {
        a[0] = a[0] - 4096;
    }
    while ((s16)a[0] < -2048) {
        a[0] = a[0] + 4096;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", BoundSAngle);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", func_80151098);
INCLUDE_ASM("asm/nonmatchings/tm1/smooth", FUN_801510a8);
#endif

s32 SmoothAngleValue(s32 from, s32 to, s32 pct)
{
    s32 r;

    if (from < -1024 && to > 1024) {
        to = to - 4096;
        r = (from * pct + to * (100 - pct)) / 100;
        BoundAngle(&r);
    } else if (from > 1024 && to < -1024) {
        to = to + 4096;
        r = (from * pct + to * (100 - pct)) / 100;
        BoundAngle(&r);
    } else {
        r = (from * pct + to * (100 - pct)) / 100;
    }
    return r;
}

s32 SmoothValue(s32 from, s32 to, s32 pct)
{
    return (from * pct + to * (100 - pct)) / 100;
}

s32 SmoothValue2(s32 a, s32 b, s32 c)
{
    return SmoothValue(c, b, 99 - (c - a) * 100 / ((b - a) * 10));
}
