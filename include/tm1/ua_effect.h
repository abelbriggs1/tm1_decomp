#ifndef __TM1_UA_EFFECT_H__
#define __TM1_UA_EFFECT_H__

#include "common.h"
#include "tm1/cs.h"

typedef struct EffNode {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u8 numWords;
    /* 0x03 */ u8 pad03;
    /* 0x04 */ s32 env;
    /* 0x08 */ u8 pad08[4];
    /* 0x0C */ u8* prim;
    /* 0x10 */ s32 kind;
    /* 0x14 */ s16 index;
} EffNode;

typedef struct WheelNode {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ s16 rot[9];
    /* 0x16 */ u8 pad16[0xE];
    /* 0x24 */ u16 wheelId;
} WheelNode;

typedef struct HitLight {
    /* 0x00 */ s32 count;
    /* 0x04 */ Cs* node;
} HitLight; /* 0x08 */

/* The {u1, v1, tpage} word of a textured quad primitive. */
typedef struct UVWord {
    /* 0x00 */ u8 u;
    /* 0x01 */ u8 v;
    /* 0x02 */ u16 page;
} UVWord; /* 0x04 */

void UAeffectInit(void);
void UAeffectUpdateMonsterTruckWheels(s32 speed);
void UAeffectUpdateTVs(void);
void UAeffectInitTV(EffNode* node);
void UAeffectInitMonsterTruckWheel(WheelNode* node);
void UAeffectUpdateSpecialEffects(void);
void UAeffectUpdateCarSpeed(s32 carId, s32 speed);
void UAeffectActHitLights(Cs* node, s32 count, u8* unused_rgb);
void UAeffectUpdateHitLights(void);
void UAeffectBarricade(s32 a0, s32 a1, s32 state);
void effectResetEffects(void);
void UAeffectSetFreezeLight(Cs* node);
void UAeffectClearFreezeLight(Cs* node);
void UAeffectInitHealthStandLightning(EffNode* node);
void UAeffectUpdateHealthStandLightning(void);

#endif /* __TM1_UA_EFFECT_H__ */
