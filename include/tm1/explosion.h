#ifndef __TM1_EXPLOSION_H__
#define __TM1_EXPLOSION_H__

#include "common.h"
#include "tm1/explode.h"
#include "tm1/grutils.h"
#include "tm1/math.h"

#define MAX_FRAGMENTS 20
#define MAX_EXPLOSIONS 80

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

/* 0x80199DB8, 20 x 36 bytes */
typedef struct Fragment {
    /*0x00*/ VECTOR3 pos;
    /*0x0C*/ VECTOR3 vel;
    /*0x18*/ u8 frame;
    /*0x19*/ u8 pad19[3];
    /*0x1C*/ s32 life;
    /*0x20*/ GrSprite* anim;
} Fragment;

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
