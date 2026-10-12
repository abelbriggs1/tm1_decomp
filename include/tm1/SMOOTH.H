#ifndef __TM1_SMOOTH_H__
#define __TM1_SMOOTH_H__

#include "common.h"

void BoundVector(s32* v);
void BoundSVector(u16* v);
void BoundAngle(s32* a);
void BoundSAngle(u16* a);
s32 SmoothAngleValue(s32 from, s32 to, s32 pct);
s32 SmoothValue(s32 from, s32 to, s32 pct);
s32 SmoothValue2(s32 a, s32 b, s32 c);

#endif // __TM1_SMOOTH_H__
