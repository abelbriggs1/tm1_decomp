#ifndef __TM1_TIMER_H__
#define __TM1_TIMER_H__

#include "common.h"

void InitTimer();
void TermTimer();
u32 GetCurTics();
s16 GetFieldsLastFrame();
s32 GetUpdateRate();
u32 GetFrameCount();
void ResetUpdateRate();

#endif // __TM1_TIMER_H__
