#include "common.h"

#include "tm1/timer.h"

extern s16 gFieldsLastFrame;
extern s16 gUpdateRate;
extern u32 gFrameCount;
extern u32 gTotalTics;
extern u32 gFrameTime;
extern u32 gLastTics;
extern u32 gCurTics;

extern u32 updateRate;

extern u32 GetRCnt(u32 unk1);

INCLUDE_ASM("asm/nonmatchings/tm1/timer", InitTimer);

INCLUDE_ASM("asm/nonmatchings/tm1/timer", TermTimer);

u32 GetCurTics() { return gTotalTics + GetRCnt(0xF2000002); }

INCLUDE_ASM("asm/nonmatchings/tm1/timer", SetFrameStart);

s16 GetFieldsLastFrame() { return gFieldsLastFrame; }

s16 GetUpdateRate() { return gUpdateRate; }

INCLUDE_ASM("asm/nonmatchings/tm1/timer", GetFrameTime);
// u32 GetFrameTime() { return (gFrameTime * 0xEC) / 1000; }

u32 GetFrameCount() { return gFrameCount; }

void ResetUpdateRate() { gLastTics = GetCurTics(); }

// INCLUDE_ASM("asm/nonmatchings/tm1/timer", SysClkIntHandler);
void SysClkIntHandler() { gTotalTics = gTotalTics + 0xFFFF; }

INCLUDE_ASM("asm/nonmatchings/tm1/timer", FrameTimeToUpdateRate);
// u32 FrameTimeToUpdateRate(s32 unk)
// {
//     u32 unk1 = (unk + 0x89ED) / 0x113DA;
//     u32 unk2 = updateRate / unk1;
//     if ((s32)unk2 < 1) {
//         unk2 = 1;
//     }
//     return unk2;
// }
