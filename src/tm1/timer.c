#include "tm1/timer.h"

#include "common.h"

#include "sdk/kernel.h"
#include "sdk/libapi.h"

#define TARGET_UPDATE_RATE 60
#define RCNT2_MAX_VALUE 0xFFFF /* Reset RCNT2 every 65535 ticks. */
#define RCNT2_TICKS_PER_FRAME 70618
#define RCNT2_TICKS_PER_HALF_FRAME 35309

extern s16 gFieldsLastFrame;
extern s16 gUpdateRate; /* Current target number of updates per second. */
extern u32 gFrameCount; /* Total number of frames rendered since startup. */
extern u32 gTotalTics; /* Total number of RCNT2 ticks since startup. */
extern u32 gFrameTime; /* Number of RCNT2 ticks since last frame. */
extern u32 gLastTics; /* Tick value at the start of the previous frame. */
extern u32 gCurTics; /* Tick value at the start of this frame. */
extern u32 event; /* Event ID for RCNT2 timer count resets. */

extern u32 updateRate; /* Best-case target update rate. */

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
// u32 GetFrameTime() { return (gFrameTime * 0xEC) / 1000; }

u32 GetFrameCount() { return gFrameCount; }

void ResetUpdateRate() { gLastTics = GetCurTics(); }

void SysClkIntHandler() { gTotalTics = gTotalTics + RCNT2_MAX_VALUE; }

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
