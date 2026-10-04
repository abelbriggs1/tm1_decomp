#ifndef __TM1_INTERACTIVES_H__
#define __TM1_INTERACTIVES_H__

#include "common.h"
#include "tm1/car.h"
#include "tm1/grutils.h"
#include "tm1/math.h"

typedef struct {
    /* 0x00 */ u8 pad00[3];
    /* 0x03 */ u8 len;
    /* 0x04 */ u8 pad04[3];
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
    /* 0x14 */ u8 u1;
    /* 0x15 */ u8 v1;
    /* 0x16 */ u16 tpage;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
    /* 0x1C */ u8 u2;
    /* 0x1D */ u8 v2;
    /* 0x1E */ u16 pad1E;
    /* 0x20 */ s16 x3;
    /* 0x22 */ s16 y3;
    /* 0x24 */ u8 u3;
    /* 0x25 */ u8 v3;
    /* 0x26 */ u16 pad26;
} OilPrim;

typedef struct {
    /* 0x00 */ u32* ot;
    /* 0x04 */ u8 pad04[4];
    /* 0x08 */ u8* prim;
    /* 0x0C */ u8* primEnd;
} RenderCtx;

void carSetPowerupDelaysBySkillLevel(s32 level);
s32 getSpecialWeaponCost(s32 id);
void UpdateWeapons(Car* car, u8 isPlayer);
void UpdatePlayerWeaponPadStatus(PlayerCar* car);
void UpdateAIWeaponPadStatus(AICar* car);
u16 UpdateGuns(Car* car, u8 isPlayer);
void UpdateNonGuns(Car* car, u8 isPlayer);
void FireMissiles(Car* car, u8 isPlayer);
void FireSpecials(Car* car, u8 isPlayer);
s32 find_free_taser(void);
void carInitTasers(void);
s32 fire_taser(Car* car, s32 isPlayer);
void draw_tasers(RenderCtx* ctx, s32 view);
void LaunchDrops(Car* car, u8 isPlayer);
void carAddWeapTex(GrObj* obj, s32 id);
void carInitDropWeaps(void);
s32 FindFreeDropWeap(void);
u8 carDropWeapon(u32 kind, s32 idx, s32* pos, s32 power);
void check_dropweap_impact(s32 i);
void drawOil(OilPrim* prim, s32* pos, s32 view);
void displayDropWeapons(RenderCtx* ctx, s32 view);
void carLockon(u8 who, VECTOR3** out);
void carLockon2(u8 who, VECTOR3** out);
void carInitFlamethrowers(void);
s16 find_free_flamethrower(void);
void fireRearFlameThrower(Car* car, u8 isPlayer);
s16 fireFlameThrower(Car* car, u8 isPlayer, u8 rear, s32* pos);
void displayFlamethrowers(void);
void carResetGunHeat(PlayerCar* car);
void carInitPickupWeapons(void);
void setPickup(s32* pos, s32 obj, s32 n);
void regeneratePickupWeapons(void);
void carGetPickup(Car* car, u8 isPlayer, s32 kind, s32 idx);
u8 getBombDamage(void);
u8 health_stand_active(s32 n);
void animate_health_stand(s32 n);
void deactivate_health_stand(s32 n);
void reactivate_healthstand(s32 n);
void init_health_stands(void);
void set_health_stand(s32 n, s32* pos);
void regen_healthstands(void);
s32 carweapGetMonsterDamage(void);

#endif // __TM1_INTERACTIVES_H__
