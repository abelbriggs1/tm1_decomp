#ifndef __TM1_UA_DASH_H__
#define __TM1_UA_DASH_H__

#include "common.h"
#include <libgpu.h>
#include <libgte.h>

#include "tm1/grutils.h"
#include "tm1/rt.h"

typedef struct DashElem {
    /* 0x00 */ SPRT sprt;
    /* 0x14 */ u32 tpage;
} DashElem; /* 0x18 */

typedef struct Dashboard {
    /* 0x000 */ u_long* data[2];
    /* 0x008 */ RECT src[2];
    /* 0x018 */ RECT dst[2];
    /* 0x028 */ DR_MODE dmode[5];
    /* 0x064 */ s32 count;
    /* 0x068 */ DashElem elems[5];
    /* 0x0E0 */ POLY_F4 f0;
    /* 0x0F8 */ POLY_F4 f1;
    /* 0x110 */ POLY_G4 g0;
    /* 0x134 */ POLY_G4 g1;
    /* 0x158 */ DR_MODE statMode;
    /* 0x164 */ POLY_F4 f2;
    /* 0x17C */ DR_MODE wepMode[12];
    /* 0x20C */ s32 wepCnt;
    /* 0x210 */ SPRT wepSpr[12];
    /* 0x300 */ DR_MODE lifeMode;
    /* 0x30C */ s32 lifeCnt;
    /* 0x310 */ SPRT lifeSpr[5];
    /* 0x374 */ SPRT specSpr[5];
    /* 0x3D8 */ DR_MODE numMode;
    /* 0x3E4 */ SPRT killSpr;
    /* 0x3F8 */ SPRT totSpr;
    /* 0x40C */ SPRT ammoDig[3];
    /* 0x448 */ SPRT machDig[2];
    /* 0x470 */ DR_MODE selMode;
    /* 0x47C */ GrSprite dig[10];
    /* 0x4CC */ DR_MODE machMode;
    /* 0x4D8 */ GrSprite dig2[10];
    /* 0x528 */ POLY_FT4 wheel;
    /* 0x550 */ SPRT needle[3];
    /* 0x58C */ DR_MODE speedMode;
    /* 0x598 */ DashElem harley[3];
    /* 0x5E0 */ DR_MODE hmode[3];
    /* 0x604 */ POLY_FT4 hb[2];
    /* 0x654 */ SPRT lives[3];
    /* 0x690 */ DR_MODE livesMode;
    /* 0x69C */ SPRT bullets[6];
    /* 0x714 */ DR_MODE bulMode;
    /* 0x720 */ POLY_F4 mirrors[4];
} Dashboard; /* 0x780 */

/* Scene node holding the dashboard icon sprite objects. */
typedef struct DashNode {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[0x10];
    /* 0x14 */ u8 nChild;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ GrObj* child[8];
} DashNode;

/* Quad record in a GrObj polygon list. */
typedef struct DashFace {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ s16 vi0;
    /* 0x06 */ s16 vi1;
    /* 0x08 */ s16 vi2;
    /* 0x0A */ s16 vi3;
    /* 0x0C */ u8 pad0C[3];
    /* 0x0F */ u8 uvsel;
    /* 0x10 */ GrTexUV uv;
} DashFace;

typedef struct DashCockpitBuf {
    /* 0x00 */ GrScreenSprite spr[7];
    /* 0x70 */ u_long* pix[2];
} DashCockpitBuf;

typedef struct DashS3 {
    s16 v[3];
} DashS3;

typedef struct DashB3 {
    u8 v[3];
} DashB3;

void UAdashDrawDashboard(DVECTOR* pos);
void UAdashHealthColor(s32 tier, POLY_G4* p);
s32 UAdashTierFromPercentageOfHealth(s32 pct, s32 setHoles);
void uadashMaxCarryCapacity(void);
void uadashClearCarryCapacity(void);
void UAdashDrawInstruments(s32 player, Db* cdb);
void UAdashInitDashboardIcons(DashNode* node);
void UAdashInitDashboard(u_long* tim);
u_long* uadashGetTopBlitAddr(void);
void uadashInitCockpit(u_long* tim);
void uadashReloadDashboard(u_long* tim, s32 idx);
void uadashReloadHarleyDashboard(u_long* tim, s32 idx);
void uadashReloadSteeringWheel(u_long* tim);
void uadashLoadHarleyTms(u_long* tims);
void uadashLoadCockpit(s32 car);
void UAdashSetDashboardDrawFlag(s32 flag);
void uadashInitSteeringWheel(GrObj* obj);
void uadashInitHarleyHandleBars(void);
void uadashDrawSteeringWheel(u32* ot);
void uadashDrawHarleyHandleBars(u32* ot);
void uadashDrawSpeed(
    s32 speed, s32 unused1, SPRT* unused2, GrSprite* unused3, DR_MODE* unused4, u32* ot);
void uadashInitHarley(void);
char* uadashGetCarSign(s32 car);
void uadashInitRearViewMirror(void);
void uadashInitBulletHoles(void);
void UAdashDrawRearViewMirror(u32* ot);
void uadashToggleRemainingCarsList(void);
void uadashListRemainingCars(u32* ot);

#endif /* __TM1_UA_DASH_H__ */
