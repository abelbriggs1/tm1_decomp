#ifndef __TM1_UA_SW_H__
#define __TM1_UA_SW_H__

#include "common.h"
#include "tm1/cs.h"

typedef struct DbSwitch {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 id;
    /* 0x04 */ u16 sub;
    /* 0x06 */ u8 numStates;
    /* 0x07 */ u8 state;
    /* 0x08 */ u8 pad08[8];
    /* 0x10 */ u16 num;
    /* 0x12 */ u8 numTransChildren;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ u8 numGroupsChildren;
    /* 0x15 */ u8 pad15;
    /* 0x16 */ u16 shadowSub;
    /* 0x18 */ struct DbSwitch* child;
} DbSwitch;

extern DbSwitch* carSwitch[15];
extern DbSwitch* carTireSwitch[15][3];
extern DbSwitch* gWeaponPickup[50];
extern DbSwitch* destroySwitch[30];
extern DbSwitch* destroyGroup[30];
extern DbSwitch* barricadeSwitch[4];
extern DbSwitch* healthstandSwitch[10];
extern DbSwitch* carHeadlightSwitch[15][3];
extern DbSwitch* carShadowSwitch[15][3];

void uaswInitDbSwitch(DbSwitch* node, s32 type);
void uaswSetState(s32 id, s32 num, u32 state);
void uaswSetSwitch(DbSwitch* s, u32 state);
void uaswInitCarSwitch(Cs* cs, s32 id);
void uaswSetCarState(s32 id, u32 state, s8 unused);
void uaswInitCarTire(DbSwitch* n);
void uaswToggleCarTireState(s32 idx, s32 dir);
void uaswInitCarShadow(DbSwitch* n);
void uaswSetCarShadow(s32 id, s32 on);
void uaswInitDbEditNumChildren(DbSwitch* node, s32 type);
void uaswSetNumChildren(s32 id, s32 num, s32 n);
void uaswSetNumGroupsChildren(DbSwitch* s, s32 n);
void uaswSetNumTransChildren(DbSwitch* s, s32 n);
void uaswInit(void);
void uaswCarHeadlightsOnOff(u32 id, s32 mode);

#endif /* __TM1_UA_SW_H__ */
