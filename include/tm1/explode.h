#ifndef __TM1_EXPLODE_H__
#define __TM1_EXPLODE_H__

#include "common.h"
#include "tm1/grutils.h"
#include "tm1/math.h"

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
