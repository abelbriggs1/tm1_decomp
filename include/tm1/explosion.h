#ifndef __TM1_EXPLOSION_H__
#define __TM1_EXPLOSION_H__

#include "common.h"
#include "tm1/math.h"

typedef struct Explosion {
    /*0x00*/ VECTOR3 pos;
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
    /*0x30*/ VECTOR3 vel;
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
extern ExpFrame ContrailInfo[6];
extern ExpFrame PlasmaInfo[4];
extern ExpFrame FlameInfo[10];
extern ExpFrame GburstInfo[12];
extern ExpFrame SteamInfo[16];

Explosion* init_explosion(VECTOR3* pos, s32 a1, s32 a2, s32 a3, void* frames, u16 flag);
void animate_explosions(void);
s32 find_free_explosion(void);
void do_simple_spark(VECTOR3* pos);
void do_simple_explosion(VECTOR3* pos);
Explosion* do_blue_explosion(VECTOR3* pos);
void do_flamethrower_burst(VECTOR3* pos);
void do_mini_explosion(VECTOR3* pos);
void do_big_explosion(VECTOR3* pos);
void do_bigger_explosion(VECTOR3* pos);
void do_mondo_explosion(VECTOR3* pos);
void do_missile_plume(VECTOR3* pos);
void do_flames(VECTOR3* pos);
void do_groundburst(VECTOR3* pos);
void do_big_flames(VECTOR3* pos);
void do_afterburner(VECTOR3* pos, VECTOR3* vel);
void do_flare(VECTOR3* pos);
void do_burn(VECTOR3* pos);
void do_flash(VECTOR3* pos);
void do_mflash(VECTOR3* pos);
void do_gun_plume(VECTOR3* pos);
void do_smoke(VECTOR3* pos);
void do_steam(VECTOR3* pos);
void do_big_smoke(VECTOR3* pos);
void do_puff(VECTOR3* pos);
void clear_explosions(void);

#endif // __TM1_EXPLOSION_H__
