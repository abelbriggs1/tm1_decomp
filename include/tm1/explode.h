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

// Note: these aren't normally accessed outside of `explode.h`, but have to be
// exported anyways to match the linker order.
extern GrSprite fragmenta[];
extern GrSprite fragmentb[];
extern GrSprite fragmentc[];
extern GrSprite fragmentd[];

extern GrSprite AburstInfo[];
extern GrSprite SparkInfo[];
extern GrSprite BurnInfo[];
extern GrSprite FlareInfo[];
extern GrSprite SmokeInfo[];
extern GrSprite ContrailInfo[];
extern GrSprite PlasmaInfo[];
extern GrSprite FlameInfo[];
extern GrSprite GburstInfo[];
extern GrSprite SteamInfo[];

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
