#ifndef __TM1_EXPLODE_H__
#define __TM1_EXPLODE_H__

#include "common.h"
#include "tm1/grutils.h"
#include "tm1/math.h"

// TODO: Consolidate; some of these are probably SDK types.

/* 0x80199DB8, 20 x 36 bytes */
typedef struct Fragment {
    /*0x00*/ VECTOR3 pos;
    /*0x0C*/ VECTOR3 vel;
    /*0x18*/ u8 frame;
    /*0x19*/ u8 pad19[3];
    /*0x1C*/ s32 life;
    /*0x20*/ GrSprite* anim;
} Fragment;

typedef struct ArmorIconInfo {
    u8 unk0;
    u8 unk1;
    u8 pad2[6];
} ArmorIconInfo;

void explodeStoreFlameAnimation(void* data);
void explodeStoreExplosionAnimation(void* data);
void explodeStoreGburstAnimation(void* data);
void explodeStoreSmokeAnimation(void* data);
void explodeStoreBurnAnimation(void* data);
void explodeStoreSparkAnimation(void* data);
void explodeStoreContrailAnimation(void* data);
void explodeStoreFlareAnimation(void* data);
void explodeStorePlasmaAnimation(void* data);
void explodeLoadFragTexture(s32 id, GrObj* data);
void explodeCreateFragments(s32 count, VECTOR* pos);
ArmorIconInfo* explodeGetArmorIcon(void);

#endif
