#ifndef __TM1_EXPLODE_H__
#define __TM1_EXPLODE_H__

#include "common.h"
#include "tm1/grutils.h"

// TODO: Consolidate; some of these are probably SDK types.

/* one entry of a parsed 3D-sprite animation table  */
typedef GrSprite AnimFrame;

/* 0x80199DB8, 20 x 36 bytes */
typedef struct FragVec {
    s32 x;
    s32 y;
    s32 z;
} FragVec;

typedef struct Fragment {
    /*0x00*/ FragVec pos;
    /*0x0C*/ FragVec vel;
    /*0x18*/ u8 frame;
    /*0x19*/ u8 pad19[3];
    /*0x1C*/ s32 life;
    /*0x20*/ AnimFrame* anim;
} Fragment;

typedef struct ArmorIconInfo {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
} ArmorIconInfo;

void explodeCreateFragments(s32 count, VECTOR* pos);
ArmorIconInfo* explodeGetArmorIcon(void);

#endif
