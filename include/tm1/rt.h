#ifndef __TM1_RT_H__
#define __TM1_RT_H__

#include "common.h"
#include <libgpu.h>

/* a display buffer: 128 bytes */
typedef struct Db {
    /*0x00*/ u8* small;
    /*0x04*/ u8* area;
    /*0x08*/ s32 unk8;
    /*0x0C*/ u8* areaEnd;
    /*0x10*/ DRAWENV draw;
    /*0x6C*/ DISPENV disp;
} Db;

/* a queued "return to shell" request */
typedef struct RtReturn {
    /*0x0*/ s32 arg;
    /*0x4*/ s32 mode;
} RtReturn;

/* .bss objects */
extern RtReturn gReturnsPending[4]; /* 0x80194BF0, 0x20 bytes */
extern Db db[2]; /* 0x80194C10 */
extern Db sdb[2]; /* 0x80194D10 */
extern Db rdb[2]; /* 0x80194E10 */

extern u8 gDoubleBuffers[]; /* 0x801B74D0, 0x16BC0 */
extern u8 D_801CE090[]; /* 0x801CE090, 0x16BC0 -- the 2nd draw area */
extern u8 D_801E4C50[]; /* 0x801E4C50, 0x4000  -- small area 0 */
extern u8 D_801E8C50[]; /* 0x801E8C50, 0x4000  -- small area 1 */
extern u8 D_801ECC50[]; /* 0x801ECC50, 0x4000  -- split/rear small 0 */
extern u8 D_801F0C50[]; /* 0x801F0C50, 0x4000  -- split/rear small 1 */

/* gp-relative small globals OWNED by this TU */
extern s32 D_8018BF18; /* gp+20   numPendingReturns */
extern s32 D_8018BF1C; /* gp+24   pendingShell */
extern s32 D_8018BF20; /* gp+28   rearViewOn */
extern s32 D_8018BF24; /* gp+32   rearViewEnabled */
extern s32 split_screen; /* gp+36 */
extern s32 D_8018BF2C; /* gp+40   = 20, only rtMainLoop reads it */
extern s32 D_8018BF30; /* gp+44   fieldCounter */

extern Db* cdb; /* gp+1464 */
extern Db* curSdb; /* gp+1468 */
extern Db* curRdb; /* gp+1472 */

s32 rtMainLoop(void);
void rtReturnToShell(s32 arg, s32 mode);
void rtClearPendingReturns(void);
s32 rtTimeToReturnToShell(void);
void rtUpdateFieldCounter(s32 n);
void rtResetFieldCounter(void);
s32 rtGetFieldCounter(void);
Db* rtGetDb(void);
s32 rtWhichDisplayBuffer(void);
s32 rtWhichDrawBuffer(void);
Db* rtGetCdb(void);
void rtSetCdb(s32 which);
s32 rtWhichBuffer(void);
u8* rtGetDoubleBufferArea(void);
u8* rtGetSmallBufferArea(void);
void rtDoubleBufferInit(void);
Db* rtGetSplitScreenDb(void);
Db* rtGetRearViewDb(void);
void rtSetRearView(s32 on);
void rtToggleRearView(void);
s32 rtIsRearViewOn(void);
s32 rtIsSplitScreenOn(void);
void rtSetSplitScreen(s32 on);
void rtInitTweek(void);

#endif
