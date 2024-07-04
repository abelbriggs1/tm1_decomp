#include "tm1/timer.h"

#include "common.h"

#include "sdk/kernel.h"
#include "sdk/libapi.h"

#define TARGET_UPDATE_RATE 60
#define RCNT2_MAX_VALUE 0xFFFF /* Reset RCNT2 every 65535 ticks. */
#define RCNT2_TICKS_PER_FRAME 70618
#define RCNT2_TICKS_PER_HALF_FRAME 35309
#define RCNT2_TICKS_TO_USECS(ticks) ((ticks * 236) / 1000)

s16 gFieldsLastFrame = 2;
/* Current target number of updates per second. */
s16 gUpdateRate = 20;
/* Total number of frames rendered since startup. */
u32 gFrameCount = 0;
/* Total number of RCNT2 ticks since startup. */
u32 gTotalTics = 0;
/* Number of RCNT2 ticks since last frame. */
u32 gFrameTime = 0;
/* Tick value at the start of the previous frame. */
u32 gLastTics = 0;
/* Tick value at the start of this frame. */
u32 gCurTics = 0;

static u32 event; /* Event ID for RCNT2 timer count resets. */
static u32 updateRate;

// Forward declare so we can use in `InitTimer()`.
void SysClkIntHandler();

void InitTimer()
{
    EnterCriticalSection();
    event = OpenEvent(RCntCNT2, EvSpINT, RCntMdINTR, SysClkIntHandler);
    ExitCriticalSection();
    EnableEvent(event);

    SetRCnt(RCntCNT2, RCNT2_MAX_VALUE, RCntMdINTR);
    StartRCnt(RCntCNT2);
    gFrameCount = 0;
    updateRate = TARGET_UPDATE_RATE;
}

void TermTimer()
{
    StopRCnt(RCntCNT2);
    DisableEvent(event);
    CloseEvent(event);
}

u32 GetCurTics() { return gTotalTics + GetRCnt(RCntCNT2); }

INCLUDE_ASM("asm/nonmatchings/tm1/timer", SetFrameStart);

s16 GetFieldsLastFrame() { return gFieldsLastFrame; }

s16 GetUpdateRate() { return gUpdateRate; }

// Returns the number of microseconds since the last frame.
INCLUDE_ASM("asm/nonmatchings/tm1/timer", GetFrameTime);
// u32 GetFrameTime() { return RCNT2_TICKS_TO_USECS(gFrameTime); }

u32 GetFrameCount() { return gFrameCount; }

void ResetUpdateRate() { gLastTics = GetCurTics(); }

void SysClkIntHandler() { gTotalTics = gTotalTics + RCNT2_MAX_VALUE; }

INCLUDE_ASM("asm/nonmatchings/tm1/timer", FrameTimeToUpdateRate);
// u32 FrameTimeToUpdateRate(s32 unk)
// {
//     u32 unk1 = (unk + RCNT2_TICKS_PER_HALF_FRAME) / RCNT2_TICKS_PER_FRAME;
//     u32 unk2 = updateRate / unk1;
//     if ((s32)unk2 < 1) {
//         unk2 = 1;
//     }
//     return unk2;
// }
