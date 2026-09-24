#ifndef __TM1_EXPLOSION_H__
#define __TM1_EXPLOSION_H__

#include "common.h"

typedef struct VEC3 {
    s32 x;
    s32 y;
    s32 z;
} VEC3;

typedef struct Explosion {
    /*0x00*/ VEC3 pos;
    /*0x0C*/ u8 unk0C;
    /*0x0D*/ u8 pad0D[3];
    /*0x10*/ s32 unk10;
    /*0x14*/ s32 unk14;
    /*0x18*/ s32 life; /* < 0 == slot free */
    /*0x1C*/ s32 unk1C;
    /*0x20*/ s32 unk20;
    /*0x24*/ void* frames;
    /*0x28*/ s16 unk28;
    /*0x2A*/ u16 unk2A;
    /*0x2C*/ u8 unk2C;
    /*0x2D*/ u8 pad2D[3];
    /*0x30*/ VEC3 vel;
    /*0x3C*/ u8 idx;
    /*0x3D*/ u8 pad3D[3];
} Explosion;

typedef struct ExpFrame {
    /*0x00*/ s32 unk0;
    /*0x04*/ s32 unk4;
} ExpFrame;

// TODO: Determine where these are defined.
extern Explosion pyro[80];

extern ExpFrame AburstInfo[1];
extern ExpFrame D_8019C640[15];
extern ExpFrame SparkInfo[4];
extern ExpFrame BurnInfo[8];
extern ExpFrame FlareInfo[4];
extern ExpFrame SmokeInfo[28];
extern ExpFrame FlameInfo[10];
extern ExpFrame GburstInfo[12];
extern ExpFrame SteamInfo[16];

#endif // __TM1_EXPLOSION_H__
