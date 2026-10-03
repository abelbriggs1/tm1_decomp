#include "common.h"

#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/explosion.h"
#include "tm1/grutils.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/rt.h"
#include "tm1/sound.h"
#include "tm1/ua_sound.h"
#include "tm1/view.h"
#include <libgpu.h>
#include <libgte.h>
#include <rand.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ u8 pad00[3];
    /* 0x03 */ u8 len;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 r1;
    /* 0x0D */ u8 g1;
    /* 0x0E */ u8 b1;
    /* 0x0F */ u8 pad0F;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
} TaserLine;

typedef struct {
    /* 0x00 */ u8 pad00[0x56];
    /* 0x56 */ s16 unk56;
    /* 0x58 */ u8 pad58[2];
} Missile;

typedef struct {
    /* 0x00 */ u8 pad00[0x0A];
    /* 0x0A */ u16 unk0A;
    /* 0x0C */ u16 unk0C;
} HdPnt;

typedef struct {
    /* 0x00 */ s32* pos;
    /* 0x04 */ s32* targetPos;
    /* 0x08 */ s16 life;
    /* 0x0A */ u8 pad0A[2];
    /* 0x0C */ s32 target;
} Taser;

typedef struct {
    /* 0x00 */ u32 kind;
    /* 0x04 */ s16 life;
    /* 0x06 */ s16 type;
    /* 0x08 */ s32 pos[3];
    /* 0x14 */ s32 power;
    /* 0x18 */ s32 idx;
    /* 0x1C */ GrSprite* sprite;
} DropWeap;

typedef struct {
    /* 0x00 */ Cs* cs;
    /* 0x04 */ s16 life;
    /* 0x06 */ u8 pad06[2];
    /* 0x08 */ s32 pos[3];
    /* 0x14 */ s32 world[3];
    /* 0x20 */ s32 rear;
    /* 0x24 */ u8 flag;
    /* 0x25 */ u8 pad25[3];
} FlameThrower;

typedef struct {
    /* 0x00 */ s32 obj;
    /* 0x04 */ u8 kind;
    /* 0x05 */ u8 pad05[3];
    /* 0x08 */ s32 pos[3];
} PickupWeap;

typedef struct {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ s32 pos[3];
} HealthStand;

// TODO: Specify the proper size for these once we migrate; as externs,
// these are generating GP accesses when they shouldn't.
extern GrSprite spikeSprite[];
extern GrSprite oilSprite[];
extern GrSprite catapultSprite[];
extern GrSprite bombSprite[];

extern DropWeap dropweapon[12];
extern FlameThrower flameThrower[5];
extern PickupWeap pickup[50];
extern HealthStand HStand[10];
extern Taser taser[3];

extern s32 weaponPickupAmount[14];

s32 missileDamageFire = 7;
s32 gunDamage = 1;
s32 missileDamageRear = 7;
s32 missileDamagePower = 25;
s32 missileDamageHoming = 15;
s32 missileDamageSinging = 3;
s32 bombDamage = 15;
s32 spikeDamage = 8;
s32 flameDamage = 1;
s32 ammoRegenRate = 80;
s32 flameFuel = 10;
s32 damage30 = 6;
s32 damage60 = 15;
s32 damage10 = 30;
s32 damage20 = 12;
s32 damage90 = 15;
s32 damage70 = 3;
s32 damage80 = 40;
s32 damage110 = 10;
s32 D_8018C034 = 8;
s32 damage130 = 10;
s32 monsterDamage = 12;
s32 damage120 = 16;
s32 weaponCost100 = 5;
s32 weaponCost60 = 3;
s32 weaponCost30 = 5;
s32 weaponCost10 = 7;
s32 weaponCost20 = 4;
s32 weaponCost90 = 4;
s32 weaponCost70 = 7;
s32 weaponCost80 = 9;
s32 weaponCost110 = 5;
s32 weaponCost50 = 6;
s32 weaponCost130 = 5;
s32 weaponCost40 = 6;
s32 weaponCost120 = 7;
s32 taserRange = 81;
s32 weaponRegenDelay = 250;
s32 healthRegenDelay = 4000;
s32 numPickups = 0;
s32 numHealthStands = 0;
s32 nextHealthStand = 0;
s32 pickupTimer = 0;
u8 healthRegenPending = 0;
s32 healthRegenTimer = 0;

extern void uaswSetState(s32 obj, s32 idx, s32 state);
extern void uaswSetNumChildren(s32 obj, s32 idx, s32 n);
extern Car* GetPlayerInfo(s16 idx);
extern void hudAddRadarSig(s32 obj, s32* d, s32 z);
extern Cs* GetPlayerCs3D(s16 idx);
extern Cs* GetAICs3D(s16 idx);
extern Car* GetAICarInfo(s16 idx);
extern s16 GetNumAICars(void);
extern s16 GetNumPlayers(void);
extern void UASetBattleMusicOn(void);
extern void create_bullet(s16 idx, s32* rot, s32* pos, s32 dmg);
extern void s_create_bullet(s16 idx, s32* rot, s32* pos, s32 kind, s32 dmg, s32 a, s32 b);
extern s16 create_SWARM_missile(s16 idx, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_GHOST_missile(s16 idx, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_DEATHSPEAR_missile(s16 idx, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern void uadashMaxCarryCapacity(void);
extern s32 CAR_HD(s32* pos, s32 owner, s32 kind);
extern Missile* get_missile(u32 index);
extern s16 create_FIRE_missile(s32 owner, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_FREEZE_missile(s32 owner, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_POWER_missile(s32 owner, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_SINGING_missile(s32 owner, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_REAR_missile(s32 owner, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern s16 create_HOMING_missile(s32 owner, LVECTOR* tgt, VEC3* rot, s32* pos, s32 dmg);
extern void bulDispatchDamage(s32 hit, s32 a, s32 b, s32 amount, s32* pos, s32 dmg);
extern HdPnt* HdPntTest(s32 owner, s32 flag, s32* pos, s32* hit);
extern s32 shellGetCurrentLevel(void);

void carSetPowerupDelaysBySkillLevel(s32 level)
{
    s32 wd;

    switch (level) {
    case 0:
        healthRegenDelay = 2000;
        wd = 150;
        break;
    case 1:
        healthRegenDelay = 4000;
        wd = 250;
        break;
    default:
        healthRegenDelay = 8000;
        wd = 350;
        break;
    }
    weaponRegenDelay = wd;
}

s32 getSpecialWeaponCost(s32 id)
{
    switch (id) {
    case 100:
        return weaponCost100;
    case 60:
        return weaponCost60;
    case 30:
        return weaponCost30;
    case 10:
        return weaponCost10;
    case 20:
        return weaponCost20;
    case 90:
        return weaponCost90;
    case 70:
        return weaponCost70;
    case 80:
        return weaponCost80;
    case 110:
        return weaponCost110;
    case 50:
        return weaponCost50;
    case 130:
        return weaponCost130;
    case 40:
        return weaponCost40;
    case 120:
        return weaponCost120;
    }
    return 20;
}

#ifdef NON_MATCHING
void UpdateWeapons(Car* car, u8 isPlayer)
{
    CarWeap* pad;
    s32 i;
    s16 t;

    if (isPlayer) {
        UpdatePlayerWeaponPadStatus(car);
        pad = &car->weap;
    } else {
        UpdateAIWeaponPadStatus((CarAlt*)car);
        pad = &((CarAlt*)car)->weap;
    }
    if (pad->reset != 0) {
        for (i = 0; i < 14; i++) {
            pad->ammo[i] = 1;
        }
        pad->ammo[4] = 0;
        pad->ammo[12] = 0;
        pad->ammo[11] = 20;
        pad->unk46 = 2;
    } else {
        t = pad->timer++;
        if (t >= ammoRegenRate) {
            pad->timer = 0;
            if ((s16)pad->ammo[11] + 1 < 20) {
                pad->ammo[11] = pad->ammo[11] + 1;
            } else {
                pad->ammo[11] = 20;
            }
            if (!isPlayer && ((CarAlt*)car)->unk40 == 0
                && ((CarAlt*)car)->stats.unk40 < (((CarAlt*)car)->stats.unk44 >> 2)) {
                ((CarAlt*)car)->stats.unk40 = ((CarAlt*)car)->stats.unk40 + 1;
            }
        }
    }
    UpdateGuns(car, isPlayer);
    UpdateNonGuns(car, isPlayer);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", UpdateWeapons);
#endif

#ifdef NON_MATCHING
void UpdatePlayerWeaponPadStatus(Car* car)
{
    u32 start;

    if (car->skid[0x0B] != 0) {
        if (car->skid[0x0F] == 0) {
            return;
        }
        car->skid[0x0F] = 0;
        start = car->weap.cur;
        car->weap.cur = start + 1;
        for (;;) {
            if (car->weap.cur >= 13) {
                car->weap.cur = 0;
            }
            if ((s16)car->weap.ammo[car->weap.cur] > 0) {
                return;
            }
            if (car->weap.cur == start) {
                return;
            }
            car->weap.cur++;
        }
    } else if (car->skid[0x0C] != 0) {
        if (car->skid[0x0F] == 0) {
            return;
        }
        car->skid[0x0F] = 0;
        start = car->weap.cur;
        if (car->weap.cur == 0) {
            car->weap.cur = 11;
        } else {
            car->weap.cur--;
        }
        if ((s16)car->weap.ammo[car->weap.cur] > 0) {
            return;
        }
        for (;;) {
            if (car->weap.cur == start) {
                return;
            }
            if (car->weap.cur == 0) {
                car->weap.cur = 11;
            } else {
                car->weap.cur--;
            }
            if ((s16)car->weap.ammo[car->weap.cur] > 0) {
                return;
            }
        }
    } else {
        car->skid[0x0F] = 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", UpdatePlayerWeaponPadStatus);
#endif

#ifdef NON_MATCHING
void UpdateAIWeaponPadStatus(CarAlt* car)
{
    s32 n;
    s32 t;
    s32 lim;

    n = 4;
    if (car->unk40 != 0) {
        if (car->unk43 == 0) {
            n = 5;
        }
        if (car->unk44 == 0) {
            n = 3;
        }
        if (car->unk45 == 0) {
            n = 2;
        }
        lim = n * 136;
        t = car->unk2C;
        if (t < 0) {
            t = -t;
        }
        if (t < lim && car->unk34 < 4800) {
            car->skid[9] = 1;
        }
        if (car->uaIndex == 130 && car->unk144 == 1) {
            s32 a;
            a = car->unk2C;
            if (a < 0) {
                a = -a;
            }
            if ((((180 - n * 10) << 12) / 360) < a) {
                car->skid[9] = 1;
            }
        }
        if (car->weap.cur == 2 && n < 3) {
            n += 2;
        }
        if (car->weap.cur == 11 && n < 3 && (car->uaIndex == 70 || car->uaIndex == 110)) {
            n += 2;
        }
        lim = n * 113;
        t = car->unk2C;
        if (t < 0) {
            t = -t;
        }
        if (t < lim) {
            car->skid[10] = 1;
        } else if (car->unk144 == 1) {
            s32 a;
            a = car->unk2C;
            if (a < 0) {
                a = -a;
            }
            if ((((180 - n * 10) << 12) / 360) < a) {
                car->skid[10] = 1;
            }
        }
        if (car->weap.cur < 11) {
            car->weap.ammo[car->weap.cur] = 10;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", UpdateAIWeaponPadStatus);
#endif

#ifdef NON_MATCHING
u16 UpdateGuns(Car* car, u8 isPlayer)
{
    s32 r[3];
    s32 d[3];
    s32 v[3];
    u16 idx;
    CarStats* cfg;
    CarMotion* m;
    u8* sk;
    CarWeap* pad;
    CarBounce* aim;
    s32 snd;
    s32 range;
    MATRIX* mat;

    if (isPlayer) {
        cfg = &car->stats;
        m = &car->motion;
        sk = car->skid;
        pad = &car->weap;
        aim = &car->bounce;
        idx = car->playerIdx + 1;
        snd = GetPlayerInfo((s16)car->playerIdx)->uaIndex;
    } else {
        cfg = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        sk = ((CarAlt*)car)->skid;
        pad = &((CarAlt*)car)->weap;
        aim = &((CarAlt*)car)->bounce;
        idx = ((CarAlt*)car)->playerIdx + 50;
        snd = ((CarAlt*)GetAICarInfo((s16)((CarAlt*)car)->playerIdx))->uaIndex;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.x);
    if ((s16)pad->gunDelay > 0) {
        pad->gunDelay = pad->gunDelay - 1;
        return pad->gunDelay;
    }
    if (!isPlayer) {
        if (rand() % 100 + 1 > (s16)pad->unk3C) {
            sk[9] = 0;
        }
    }
    if ((s16)cfg->unk18 > 0) {
        sk[9] = 0;
    }
    if (isPlayer && pad->gunOverheat != 0) {
        sk[9] = 0;
    }
    if (sk[9] != 0) {
        range = soundGetCalculatedSoundRange();
        uasoundFireCarMachineGuns(snd, range, soundGetCalculatedSoundXPosition());
    } else {
        uasoundStopCarMachineGuns(snd);
        if ((s16)pad->gunHeat > 0) {
            pad->gunHeat = MAX((s16)pad->gunHeat - 1, 0);
            if ((s16)pad->gunHeat < 100) {
                pad->gunOverheat = 0;
            }
        }
        return;
    }
    {
        if (!isPlayer) {
            UASetBattleMusicOn();
        }
        if (pad->unk04 != 0) {
            v[0] = -(s16)cfg->unk110;
        } else {
            v[0] = (s16)cfg->unk110;
        }
        v[1] = (s16)cfg->unk112;
        v[2] = (s16)cfg->unk114;
        mat = &m->mat2;
        pad->unk04 = 1 - pad->unk04;
        mathMulTransVec(mat, v, d);
        d[0] = d[0] + m->pos.x;
        d[1] = d[1] + m->pos.y;
        d[2] = d[2] + m->pos.z;
        r[0] = rsin(m->rot.z - aim->unk38);
        r[1] = rcos(m->rot.z - aim->unk38);
        r[2] = -rsin(m->rot.x - aim->unk30);
        create_bullet((s16)idx, r, d, gunDamage);
        do_mflash(d);
        if (snd == 0x82) {
            if (pad->unk04 != 0) {
                v[0] = 0;
                v[1] = -v[1];
                mathMulTransVec(mat, v, d);
                d[0] = d[0] + m->pos.x;
                d[1] = d[1] + m->pos.y;
                r[2] = -r[2];
                r[0] = -r[0];
                r[1] = -r[1];
                d[2] = d[2] + m->pos.z;
                s_create_bullet((s16)idx, r, d, 2, gunDamage * 2, 20, 0);
                do_mflash(d);
            }
        }
        pad->gunDelay = 3;
        if (!isPlayer) {
            pad->gunDelay = 5;
        } else {
            pad->gunHeat = pad->gunHeat + 6;
            if ((s16)pad->gunHeat >= 199) {
                pad->gunOverheat = 1;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", UpdateGuns);
#endif

#ifdef NON_MATCHING
void UpdateNonGuns(Car* car, u8 isPlayer)
{
    CarStats* cfg;
    u8* sk;
    CarWeap* pad;
    Cs* cs;
    s16 idx;

    if (isPlayer) {
        cfg = &car->stats;
        sk = car->skid;
        pad = &car->weap;
        idx = car->playerIdx + 1;
        cs = GetPlayerCs3D((s16)car->playerIdx);
    } else {
        cfg = &((CarAlt*)car)->stats;
        sk = ((CarAlt*)car)->skid;
        pad = &((CarAlt*)car)->weap;
        idx = ((CarAlt*)car)->playerIdx + 50;
        cs = GetAICs3D((s16)((CarAlt*)car)->playerIdx);
    }
    if ((s16)pad->fireDelay > 0) {
        pad->fireDelay--;
        return;
    }
    if (pad->cur == 12) {
        sk[10] = 0;
    }
    if ((s16)cfg->unk18 > 0) {
        sk[10] = 0;
    }
    if (sk[10] != 0) {
        if (isPlayer == 0 && pad->cur != 11) {
            pad->ammo[pad->cur] = 5;
        }
        if (pad->ammo[pad->cur] != 0) {
            if (isPlayer == 0) {
                UASetBattleMusicOn();
            } else {
                isPlayer = cs->unkC0;
            }
            switch (pad->cur) {
            case 6:
            case 8:
            case 9:
            case 10:
                LaunchDrops(car, isPlayer);
                break;
            case 7:
                fireRearFlameThrower(car, isPlayer);
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                FireMissiles(car, isPlayer);
                break;
            case 11:
                FireSpecials(car, isPlayer);
                break;
            case 12:
                break;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", UpdateNonGuns);
#endif

#ifdef NON_MATCHING
void FireMissiles(Car* car, u8 isPlayer)
{
    s32 d[3];
    s32 v[3];
    LVECTOR* tgt;
    CarStats* cfg;
    CarMotion* m;
    CarWeap* pad;
    u8* flags;
    Cs* cs;
    Missile* mp;
    s32 r;
    s16 idx;

    if (isPlayer) {
        idx = car->playerIdx + 1;
        cfg = &car->stats;
        m = &car->motion;
        pad = &car->weap;
        flags = car->flags;
        cs = GetPlayerCs3D((s16)car->playerIdx);
    } else {
        idx = ((CarAlt*)car)->playerIdx + 50;
        cfg = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        pad = &((CarAlt*)car)->weap;
        flags = ((CarAlt*)car)->flags;
        cs = GetAICs3D((s16)((CarAlt*)car)->playerIdx);
    }
    v[1] = 0;
    v[0] = (s16)cfg->dropPower;
    v[2] = cfg->unk70 + 8;
    mathMulTransVec(&m->mat2, v, d);
    d[0] = d[0] + m->pos.x;
    d[1] = d[1] + m->pos.y;
    d[2] = d[2] + m->pos.z;
    r = -1;
    switch (pad->cur) {
    case 0:
        carLockon(isPlayer, &tgt);
        r = create_FIRE_missile(cs->unkC0, tgt, &m->rot, d, missileDamageFire);
        break;
    case 1:
        r = create_FREEZE_missile(cs->unkC0, &m->rot, d, -1);
        break;
    case 3:
        r = create_POWER_missile(cs->unkC0, &m->rot, d, missileDamagePower);
        break;
    case 4:
        r = create_SINGING_missile(cs->unkC0, &m->rot, d, missileDamageSinging);
        break;
    case 5:
        carLockon2(isPlayer, &tgt);
        r = create_REAR_missile(cs->unkC0, tgt, &m->rot, d, missileDamageRear);
        break;
    case 2:
        carLockon(isPlayer, &tgt);
        r = create_HOMING_missile(cs->unkC0, tgt, &m->rot, d, missileDamageHoming);
        break;
    }
    if (r >= 0) {
        pad->fireDelay = 20;
        flags[9] = 1;
        pad->ammo[pad->cur]--;
        pad->totalAmmo--;
        mp = get_missile((u32)r);
        mp->unk56 = (cfg->unk6C - 24) > 0 ? (cfg->unk6C - 24) : 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", FireMissiles);
#endif

#ifdef NON_MATCHING
void FireSpecials(Car* car, u8 isPlayer)
{
    s32 d[3];
    s32 v[3];
    s32 r[3];
    LVECTOR* tgt;
    CarStats* cfg;
    CarMotion* m;
    CarWeap* pad;
    u8* flags;
    MATRIX* mat;
    s16 idx;
    s32 kind;
    s32 range;
    u8 count;

    if (isPlayer) {
        flags = car->flags;
        idx = car->playerIdx + 1;
        GetPlayerCs3D((s16)car->playerIdx);
        kind = car->uaIndex;
        cfg = &car->stats;
        m = &car->motion;
        pad = &car->weap;
    } else {
        flags = ((CarAlt*)car)->flags;
        idx = ((CarAlt*)car)->playerIdx + 50;
        GetAICs3D((s16)((CarAlt*)car)->playerIdx);
        kind = ((CarAlt*)car)->uaIndex;
        cfg = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        pad = &((CarAlt*)car)->weap;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.x);
    v[0] = 0;
    v[1] = (s16)cfg->unk112;
    v[2] = (s16)cfg->unk114;
    mat = &m->mat2;
    mathMulTransVec(mat, v, d);
    d[0] = d[0] + m->pos.x;
    d[1] = d[1] + m->pos.y;
    d[2] = d[2] + m->pos.z;
    v[0] = 0;
    v[1] = -100;
    v[2] = 0;
    mathMulTransVec(mat, v, r);
    r[0] = rsin(m->rot.z) >> 6;
    r[1] = rcos(m->rot.z) >> 6;
    r[2] = -rsin(m->rot.x) >> 6;
    switch (kind) {
    case 30:
        if ((s16)pad->ammo[11] >= weaponCost30) {
            d[2] = d[2] - 16;
            s_create_bullet(idx, r, d, 4, damage30, 12, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost30;
            uasoundStopCarSpecialWeapon(9);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                9, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 60:
        if ((s16)pad->ammo[11] >= weaponCost60) {
            s_create_bullet(idx, r, d, 6, damage60, 15, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost60;
            uasoundStopCarSpecialWeapon(6);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                6, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 20:
        if ((s16)pad->ammo[11] >= weaponCost20) {
            s_create_bullet(idx, r, d, 17, damage20, 30, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost20;
            uasoundStopCarSpecialWeapon(7);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                7, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 90:
        if ((s16)pad->ammo[11] >= weaponCost90) {
            s_create_bullet(idx, r, d, 19, damage90, 30, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost90;
            uasoundStopCarSpecialWeapon(0);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                0, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 10:
        if ((s16)pad->ammo[11] >= weaponCost10) {
            s_create_bullet(idx, r, d, 15, damage10, 30, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost10;
            uasoundStopCarSpecialWeapon(1);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                1, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 70:
        if ((s16)pad->ammo[11] >= weaponCost70) {
            MATRIX* mat2;
            s32 res;
            carLockon(isPlayer, &tgt);
            mat2 = &m->mat2;
            v[1] = 0;
            v[0] = (s16)cfg->dropPower;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            res = create_SWARM_missile(idx, tgt, &m->rot, d, damage70);
            count = res >= 0;
            v[0] = (s16)cfg->unk110;
            v[1] = (s16)cfg->unk112;
            v[2] = (s16)cfg->unk114;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            if (create_SWARM_missile(idx, tgt, &m->rot, d, damage70) >= 0) {
                count = count + 1;
            }
            v[0] = -(s16)cfg->unk110;
            v[1] = (s16)cfg->unk112;
            v[2] = (s16)cfg->unk114;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            if (create_SWARM_missile(idx, tgt, &m->rot, d, damage70) >= 0) {
                count = count + 1;
            }
            if (count != 0) {
                pad->ammo[11] = pad->ammo[11] - weaponCost70;
                uasoundStopCarSpecialWeapon(4);
                range = soundGetCalculatedSoundRange();
                uasoundPlayCarSpecialWeaponLaunchOrInflight(
                    4, range, soundGetCalculatedSoundXPosition());
            }
        }
        break;
    case 80:
        if ((s16)pad->ammo[11] >= weaponCost80) {
            s_create_bullet(idx, r, d, 21, damage80, 20, 0);
            pad->ammo[11] = pad->ammo[11] - weaponCost80;
            uasoundStopCarSpecialWeapon(11);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                11, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 110:
        if ((s16)pad->ammo[11] >= weaponCost110) {
            carLockon(isPlayer, &tgt);
            v[1] = 0;
            v[0] = (s16)cfg->dropPower;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            if (create_GHOST_missile(idx, tgt, &m->rot, d, damage110) >= 0) {
                pad->ammo[11] = pad->ammo[11] - weaponCost110;
                uasoundStopCarSpecialWeapon(3);
                range = soundGetCalculatedSoundRange();
                uasoundPlayCarSpecialWeaponLaunchOrInflight(
                    3, range, soundGetCalculatedSoundXPosition());
            }
        }
        break;
    case 50:
        if ((s16)pad->ammo[11] >= weaponCost50) {
            if (fire_taser(car, isPlayer) != 0) {
                if (isPlayer) {
                    pad->ammo[11] = pad->ammo[11] - weaponCost50;
                } else {
                    pad->ammo[11] = pad->ammo[11] - weaponCost50 / 2;
                }
                uasoundStopCarSpecialWeapon(8);
                range = soundGetCalculatedSoundRange();
                uasoundPlayCarSpecialWeaponLaunchOrInflight(
                    8, range, soundGetCalculatedSoundXPosition());
            }
        }
        break;
    case 120:
        if ((s16)pad->ammo[11] >= weaponCost120) {
            carLockon(isPlayer, &tgt);
            v[1] = 0;
            v[0] = (s16)cfg->dropPower;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            if (create_DEATHSPEAR_missile(idx, tgt, &m->rot, d, damage120) >= 0) {
                pad->ammo[11] = pad->ammo[11] - weaponCost120;
                uasoundStopCarSpecialWeapon(2);
                range = soundGetCalculatedSoundRange();
                uasoundPlayCarSpecialWeaponLaunchOrInflight(
                    2, range, soundGetCalculatedSoundXPosition());
            }
        }
        break;
    case 130:
        if ((s16)pad->ammo[11] >= weaponCost100) {
            VEC3 rot2;
            carLockon(isPlayer, &tgt);
            v[1] = 0;
            v[0] = (s16)cfg->dropPower;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.x;
            d[1] = d[1] + m->pos.y;
            d[2] = d[2] + m->pos.z;
            create_SWARM_missile(idx, tgt, &m->rot, d, damage130);
            carLockon2(isPlayer, &tgt);
            rot2.x = m->rot.x;
            rot2.y = m->rot.y;
            rot2.z = m->rot.z + 2048;
            create_SWARM_missile(idx, tgt, &rot2, d, damage130);
            s_create_bullet(idx, r, d, 19, damage130, 30, 0);
            fire_taser(car, isPlayer);
            fireFlameThrower(car, isPlayer, 2, r);
            pad->ammo[pad->cur] = pad->ammo[pad->cur] - weaponCost130;
            uasoundStopCarSpecialWeapon(12);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                12, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case 100:
        if ((s16)pad->ammo[11] >= weaponCost100) {
            fireFlameThrower(car, isPlayer, 2, r);
            pad->ammo[pad->cur] = pad->ammo[pad->cur] - weaponCost100;
            uasoundStopCarSpecialWeapon(10);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                10, range, soundGetCalculatedSoundXPosition());
        }
        break;
    }
    pad->fireDelay = (kind == 60 || kind == 30 || kind == 90) ? 10 : 20;
    flags[9] = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", FireSpecials);
#endif

s32 find_free_taser(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (taser[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

void carInitTasers(void)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        taser[i].life = -1;
    }
}

#ifdef NON_MATCHING
s32 fire_taser(Car* car, s32 isPlayer)
{
    CarMotion* m;
    Cs* cs;
    Cs* other;
    s32* bestObj;
    s32 bestId;
    s32 best;
    s32 myId;
    s32 idx;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 t;
    s32 dist;
    s32 numP;
    s32 numA;
    s16 i;
    s32 j;

    bestObj = 0;
    if (isPlayer != 0) {
        m = &car->motion;
        cs = GetPlayerCs3D((s16)car->playerIdx);
    } else {
        m = &((CarAlt*)car)->motion;
        cs = GetAICs3D((s16)((CarAlt*)car)->playerIdx);
    }
    idx = find_free_taser();
    if (idx < 0) {
        return 0;
    }
    bestId = -1;
    numP = GetNumPlayers();
    numA = GetNumAICars();
    myId = cs->unkC0;
    best = taserRange + 400;
    for (i = 0; i < numP; i++) {
        other = GetPlayerCs3D(i);
        dx = m->pos.x - other->pos.vx;
        dy = m->pos.y - other->pos.vy;
        dz = m->pos.z - other->pos.vz;
        if (dx < 0) {
            dx = -dx;
        }
        if (dy < 0) {
            dy = -dy;
        }
        if (dz < 0) {
            dz = -dz;
        }
        if (dy < dz) {
            t = m->pos.z - other->pos.vz;
            if (t < 0) {
                t = -t;
            }
            if (dx < t) {
                goto p_yz;
            }
            goto p_x;
        }
        t = m->pos.y - other->pos.vy;
        if (t < 0) {
            t = -t;
        }
        if (dx < t) {
        p_yz:
            dy = m->pos.y - other->pos.vy;
            dz = m->pos.z - other->pos.vz;
            if (dy < 0) {
                dy = -dy;
            }
            if (dz < 0) {
                dz = -dz;
            }
            if (dy < dz) {
                t = m->pos.z - other->pos.vz;
            } else {
                t = m->pos.y - other->pos.vy;
            }
        } else {
        p_x:
            t = m->pos.x - other->pos.vx;
        }
        if (t < 0) {
            t = -t;
        }
        dist = t >> 3;
        if (other->unkC0 != myId) {
            if (dist < taserRange) {
                if (dist < best) {
                    best = dist;
                    bestId = other->unkC0;
                    bestObj = &GetPlayerInfo(i)->motion.pos.x;
                }
            }
        }
    }
    i = 0;
    for (j = 0; j < numA; j++) {
        if (((CarAlt*)GetAICarInfo(i))->stats.unk40 > 0) {
            other = GetAICs3D(i);
            dx = m->pos.x - other->pos.vx;
            dy = m->pos.y - other->pos.vy;
            dz = m->pos.z - other->pos.vz;
            if (dx < 0) {
                dx = -dx;
            }
            if (dy < 0) {
                dy = -dy;
            }
            if (dz < 0) {
                dz = -dz;
            }
            if (dy < dz) {
                t = m->pos.z - other->pos.vz;
                if (t < 0) {
                    t = -t;
                }
                if (dx < t) {
                    goto a_yz;
                }
                goto a_x;
            }
            t = m->pos.y - other->pos.vy;
            if (t < 0) {
                t = -t;
            }
            if (dx < t) {
            a_yz:
                dy = m->pos.y - other->pos.vy;
                dz = m->pos.z - other->pos.vz;
                if (dy < 0) {
                    dy = -dy;
                }
                if (dz < 0) {
                    dz = -dz;
                }
                if (dy < dz) {
                    t = m->pos.z - other->pos.vz;
                } else {
                    t = m->pos.y - other->pos.vy;
                }
            } else {
            a_x:
                t = m->pos.x - other->pos.vx;
            }
            if (t < 0) {
                t = -t;
            }
            dist = t >> 3;
            if (other->unkC0 != myId) {
                if (dist < taserRange) {
                    if (dist < best) {
                        best = dist;
                        bestId = other->unkC0;
                        bestObj = &((CarAlt*)GetAICarInfo(i))->motion.pos.x;
                    }
                }
            }
        }
        i = i + 1;
    }
    taser[idx].life = 10;
    if (bestId < 0) {
        taser[idx].target = -1;
        taser[idx].targetPos = 0;
    } else {
        taser[idx].target = bestId;
        taser[idx].targetPos = bestObj;
    }
    taser[idx].pos = &m->pos.x;
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", fire_taser);
#endif

#ifdef NON_MATCHING
void draw_tasers(RenderCtx* ctx, s32 view)
{
    s32 tp[3];
    s32 w[3];
    s32 pt[16][3];
    s32 sc[16][3];
    s32 d[3];
    s32 cx;
    s32 cy;
    MATRIX* mat;
    EyeTrans* tr;
    u32* ot;
    s32 hfov;
    s32 vfov;
    TaserLine* p;
    s32 i;
    s32 j;
    s32 z;

    ot = ctx->ot;
    viewGetCenter(view, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    tr = viewGetEyeTrans(view);
    mat = (MATRIX*)viewGetEyeMat(view);
    for (i = 0; i < 3; i++) {
        if (taser[i].life > 0) {
            if (taser[i].target < 0) {
                taser[i].targetPos = tp;
                tp[0] = taser[i].pos[0] + (rcos(taser[i].life * 400) >> 4);
                tp[1] = taser[i].pos[1] + (rsin(taser[i].life * 400) >> 4);
                tp[2] = 0;
                if (shellGetCurrentLevel() == 5) {
                    tp[2] = taser[i].pos[2] - 40;
                }
            }
            d[0] = taser[i].targetPos[0] - taser[i].pos[0];
            d[1] = taser[i].targetPos[1] - taser[i].pos[1];
            d[2] = taser[i].targetPos[2] - taser[i].pos[2];
            pt[0][0] = taser[i].pos[0];
            pt[0][1] = taser[i].pos[1];
            pt[0][2] = taser[i].pos[2] + 30;
            pt[15][0] = taser[i].targetPos[0];
            pt[15][1] = taser[i].targetPos[1];
            pt[15][2] = taser[i].targetPos[2];
            for (j = 1; j < 15; j++) {
                pt[j][0] = taser[i].pos[0] + d[0] * j / 16;
                pt[j][1] = taser[i].pos[1] + d[1] * j / 16;
                pt[j][2] = (d[2] * j / 16 + 30) + taser[i].pos[2];
                pt[j][2] = pt[j][2] - 32 + (rand() & 0x3F);
                pt[j][1] = pt[j][1] - 8 + (rand() & 0xF);
                pt[j][0] = pt[j][0] - 8 + (rand() & 0xF);
            }
            for (j = 0; j < 16; j++) {
                w[0] = pt[j][0] + tr->x;
                w[1] = pt[j][1] + tr->y;
                w[2] = pt[j][2] + tr->z;
                mathMulVec(mat, w, sc[j]);
                z = sc[j][2];
                if (z > 0) {
                    sc[j][0] = cx + sc[j][0] * hfov / z;
                    sc[j][1] = cy + sc[j][1] * vfov / sc[j][2];
                    sc[j][2] = sc[j][2] / 4;
                    if ((u32)(sc[j][0] + 4000) >= 8001) {
                        sc[j][2] = 0;
                    }
                    if ((u32)(sc[j][1] + 4000) >= 8001) {
                        sc[j][2] = 0;
                    }
                }
            }
            if (rand() & 1) {
                do_puff(pt[15]);
            } else {
                do_flash(pt[15]);
            }
            do_simple_spark(pt[(rand() & 7) + 8]);
            do_simple_spark(pt[rand() & 0xF]);
            taser[i].life = taser[i].life - 1;
            if (taser[i].target >= 0) {
                if (taser[i].life == 0) {
                    carTakeHit((s16)taser[i].target, D_8018C034, 0, 0);
                }
            }
            for (j = 0; j < 15; j++) {
                if (sc[j][2] > 0 && sc[j + 1][2] > 0 && sc[j][2] < 4091 && sc[j + 1][2] < 4091) {
                    p = (TaserLine*)ctx->prim;
                    if ((u32)(ctx->prim + 0x50) < (u32)ctx->primEnd) {
                        p->len = 4;
                        p->code = 0x50;
                        ctx->prim = ctx->prim + 0x50;
                        p->r0 = -6 - j * 12;
                        p->g0 = -6 - j * 12;
                        p->b0 = 255;
                        p->r1 = -6 - (j + 1) * 12;
                        p->g1 = -6 - (j + 1) * 12;
                        p->b1 = 255;
                        p->x0 = sc[j][0];
                        p->y0 = sc[j][1];
                        p->x1 = sc[j + 1][0];
                        p->y1 = sc[j + 1][1];
                        AddPrim(ot + sc[j + 1][2], p);
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", draw_tasers);
#endif

#ifdef NON_MATCHING
void LaunchDrops(Car* car, u8 isPlayer)
{
    s32 d[3];
    s32 v[3];
    CarStats* cfg;
    CarMotion* m;
    CarWeap* pad;
    u8* flags;
    CarTire* t;
    s16 idx;
    s32 power;
    s32 range;
    u32 weapon;

    if (isPlayer) {
        cfg = &car->stats;
        m = &car->motion;
        pad = &car->weap;
        flags = car->flags;
        idx = car->playerIdx + 1;
        GetPlayerCs3D((s16)car->playerIdx);
        t = car->tires;
    } else {
        cfg = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        pad = &((CarAlt*)car)->weap;
        flags = ((CarAlt*)car)->flags;
        idx = ((CarAlt*)car)->playerIdx + 50;
        GetAICs3D((s16)((CarAlt*)car)->playerIdx);
        t = ((CarAlt*)car)->tires;
    }
    if ((s16)t[2].unk22 < (s16)t[3].unk22) {
        power = (s16)t[2].unk22;
    } else {
        power = (s16)t[3].unk22;
    }
    v[1] = 0;
    v[2] = 0;
    v[0] = (s16)cfg->dropPower;
    mathMulTransVec(&m->mat2, v, d);
    d[0] = d[0] + m->pos.x;
    d[1] = d[1] + m->pos.y;
    d[2] = d[2] + m->pos.z;
    soundSetRangeAndXPositionFromWorldLoc(d);
    do {
        weapon = pad->cur;
    } while (0);
    if (carDropWeapon(weapon, idx, d, power)) {
        pad->fireDelay = 20;
        flags[9] = 1;
        do {
            pad->ammo[pad->cur]--;
        } while (0);
        do {
            pad->totalAmmo--;
        } while (0);
        uasoundStopCarWeapon(pad->cur);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponLaunchOrInflight(pad->cur, range, soundGetCalculatedSoundXPosition());
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", LaunchDrops);
#endif

void carAddWeapTex(GrObj* obj, s32 id)
{
    switch (id) {
    case 0xD4:
        grutilsParse3DSprite(obj, bombSprite, 1);
        break;
    case 0xD5:
        grutilsParse3DSprite(obj, oilSprite, 1);
        break;
    case 0xD6:
        grutilsParse3DSprite(obj, spikeSprite, 1);
        break;
    case 0xD2:
        grutilsParse3DSprite(obj, catapultSprite, 1);
        break;
    }
}

void carInitDropWeaps(void)
{
    s32 i;

    for (i = 0; i < 12; i++) {
        dropweapon[i].life = 0;
        dropweapon[i].kind = 0;
    }
}

s32 FindFreeDropWeap(void)
{
    s32 i;

    for (i = 0; i < 12; i++) {
        if (dropweapon[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

u8 carDropWeapon(u32 kind, s32 idx, s32* pos, s32 power)
{
    s32 i;
    s32* dst;
    s32 first;
    GrSprite* sprite;

    i = FindFreeDropWeap();
    if (i < 0) {
        return 0;
    }
    dropweapon[i].idx = idx;
    dropweapon[i].kind = kind;
    dropweapon[i].life = 400;
    dropweapon[i].power = power;
    do {
        first = pos[0];
        dst = dropweapon[i].pos;
    } while (0);
    dst[0] = first;
    dst[1] = pos[1];
    dst[2] = pos[2];
    do {
        switch (kind) {
        case 8:
            sprite = bombSprite;
            dropweapon[i].type = -5;
            break;
        case 6:
            sprite = catapultSprite;
            dropweapon[i].type = -4;
            break;
        case 9:
            sprite = oilSprite;
            dropweapon[i].type = -2;
            break;
        case 10:
            sprite = spikeSprite;
            dropweapon[i].type = -3;
            break;
        default:
            dropweapon[i].life = 0;
            return 0;
        }
    } while (0);
    dropweapon[i].sprite = sprite;
    return i + 1;
}

#ifdef NON_MATCHING
void check_dropweap_impact(s32 i)
{
    s32 v[3];
    DropWeap* d;
    s32* pos;
    s32 owner;
    s32 hit;
    s32 range;
    s32 j;

    d = &dropweapon[i];
    owner = 0;
    if ((s16)d->life >= 360) {
        owner = d->idx;
    }
    pos = d->pos;
    d->pos[2] = d->pos[2] + 30;
    hit = CAR_HD(pos, owner, 2);
    d->pos[2] = d->pos[2] - 30;
    if (hit != 0 && hit != 8) {
        soundSetRangeAndXPositionFromWorldLoc(pos);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponExplode(d->kind, range, soundGetCalculatedSoundXPosition(), 0x63);
        if (d->kind == 8) {
            d->pos[2] = d->pos[2] + 50;
            do_big_explosion(pos);
            d->pos[2] = d->pos[2] - 50;
            do_big_explosion(pos);
        }
        if (d->kind == 10) {
            do_smoke(pos);
        }
        if (d->kind != 8) {
            for (j = 0; j < 6; j++) {
                v[0] = d->pos[0] + (rand() & 0x3F) - 32;
                v[1] = d->pos[1] + (rand() & 0x3F) - 32;
                v[2] = d->pos[2];
                do_puff(v);
            }
        }
        if (d->kind != 9) {
            d->life = 0;
        }
        v[0] = 0;
        v[1] = 0;
        v[2] = 100;
        if ((s16)d->type == -3) {
            bulDispatchDamage(hit, 0, 0, spikeDamage, v, -(s32)d->kind);
        } else if ((s16)d->type == -5) {
            bulDispatchDamage(hit, 0, 0, bombDamage, v, -(s32)d->kind);
        }
        bulDispatchDamage(hit, 0, 0, (s16)d->type, v, -(s32)d->kind);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", check_dropweap_impact);
#endif

#ifdef NON_MATCHING
void drawOil(OilPrim* prim, s32* pos, s32 view)
{
    s32 out[4][3];
    s32 v[3];
    s32 c[2];
    s32 hfov;
    s32 vfov;
    MATRIX* mat;
    s32 i;
    s32 z;

    viewGetCenter(view, &c[0], &c[1]);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    mat = (MATRIX*)viewGetEyeMat(view);
    v[0] = pos[0] - 48;
    v[1] = pos[1] - 48;
    v[2] = pos[2];
    mathMulVec(mat, v, out[0]);
    v[0] = pos[0] + 48;
    v[1] = pos[1] - 48;
    v[2] = pos[2];
    mathMulVec(mat, v, out[1]);
    v[0] = pos[0] - 48;
    v[1] = pos[1] + 48;
    v[2] = pos[2];
    mathMulVec(mat, v, out[2]);
    v[0] = pos[0] + 48;
    v[1] = pos[1] + 48;
    v[2] = pos[2];
    mathMulVec(mat, v, out[3]);
    for (i = 0; i < 4; i++) {
        z = out[i][2];
        if (z < 16) {
            z = 16;
        }
        out[i][0] = c[0] + (out[i][0] * hfov) / z;
        out[i][1] = c[1] + (out[i][1] * vfov) / z;
    }
    prim->x0 = out[0][0];
    prim->y0 = out[0][1];
    prim->x1 = out[1][0];
    prim->y1 = out[1][1];
    prim->x2 = out[2][0];
    prim->y2 = out[2][1];
    prim->x3 = out[3][0];
    prim->y3 = out[3][1];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", drawOil);
#endif

#ifdef NON_MATCHING
void displayDropWeapons(RenderCtx* ctx, s32 view)
{
    s32 w[3];
    s32 t[3];
    s32 cx;
    s32 cy;
    OilPrim* p;
    GrSprite* sp;
    EyeTrans* tr;
    MATRIX* mat;
    s32 hfov;
    s32 vfov;
    s32 i;
    s32 z;
    s32 pw;
    s32 a;
    s32 kind;
    s32 otz;
    s32 sc;
    s32 w2;
    s32 x0;
    s32 x1;
    s32 y0;
    s32 y1;

    viewGetCenter(view, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    tr = viewGetEyeTrans(view);
    mat = (MATRIX*)viewGetEyeMat(view);
    for (i = 0; i < 12; i++) {
        if (dropweapon[i].life > 0) {
            z = dropweapon[i].pos[2];
            pw = dropweapon[i].power;
            if (pw < z) {
                if (z - 12 >= pw) {
                    pw = z - 12;
                }
                dropweapon[i].pos[2] = pw;
            }
            dropweapon[i].life = dropweapon[i].life - 1;
            check_dropweap_impact(i);
            w[0] = dropweapon[i].pos[0] + tr->x;
            w[1] = dropweapon[i].pos[1] + tr->y;
            w[2] = dropweapon[i].pos[2] + tr->z;
            mathMulVec(mat, w, t);
            a = t[0];
            if (a < 0) {
                a = -a;
            }
            if (a < 641) {
                a = t[1];
                if (a < 0) {
                    a = -a;
                }
                if (a < 641 && t[2] >= 16 && t[2] < 30001) {
                    p = (OilPrim*)ctx->prim;
                    if ((u8*)p < ctx->primEnd) {
                        ctx->prim = ctx->prim + 160;
                        p->len = 9;
                        p->code = 0x2C;
                        p->code = p->code | 1;
                        sp = dropweapon[i].sprite;
                        p->u0 = sp->u0;
                        p->v0 = sp->v0;
                        p->u1 = sp->u0 + sp->w;
                        p->v1 = sp->v0;
                        p->u2 = sp->u0;
                        p->v2 = sp->v0 + sp->h;
                        p->u3 = sp->u0 + sp->w;
                        p->v3 = sp->v0 + sp->h;
                        p->tpage = sp->tpage;
                        p->clut = sp->clut;
                        kind = dropweapon[i].kind;
                        otz = t[2] >> 3;
                        if (kind == 9) {
                            otz = otz - 1;
                        }
                        if (otz < 0) {
                            otz = 0;
                        }
                        if (kind != 9) {
                            sc = hfov * 16 / t[2];
                            if (sc >= 513) {
                                sc = 512;
                            }
                            if (sc <= 0) {
                                sc = 1;
                            }
                            x0 = cx + t[0] * hfov / t[2] - sc;
                            w2 = sc * 2;
                            p->x0 = x0;
                            y0 = cy + t[1] * vfov / t[2] - w2;
                            x1 = x0 + w2;
                            p->y0 = y0;
                            p->x1 = x1;
                            p->y1 = y0;
                            y1 = y0 + sc;
                            p->x2 = x0;
                            p->y2 = y1;
                            p->x3 = x1;
                            p->y3 = y1;
                        } else {
                            drawOil(p, w, view);
                        }
                        AddPrim(ctx->ot + otz, p);
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", displayDropWeapons);
#endif

#ifdef NON_MATCHING
void carLockon(u8 who, LVECTOR** out)
{
    s32 d[3];
    s32 r[3];
    Cs* cs;
    LVECTOR* p;
    LVECTOR* q;
    s16 n;
    s32 i;
    s32 best;
    s32 z;
    s32 ax;
    s32 az;

    best = 0;
    if (who == 0) {
        *out = &GetPlayerCs3D(0)->pos;
        return;
    }
    p = &GetPlayerCs3D((s16)(who - 1))->pos;
    n = GetNumAICars();
    *out = 0;
    for (i = 0; i < n; i++) {
        cs = GetAICs3D(i);
        q = &cs->pos;
        if (q != 0 && ((CarAlt*)GetAICarInfo(i))->stats.unk40 > 0) {
            d[0] = cs->pos.vx - p->vx;
            d[1] = cs->pos.vy - p->vy;
            d[2] = cs->pos.vz - p->vz;
            mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
            z = r[1];
            if (z >= 12) {
                ax = r[0];
                az = __builtin_abs(z);
                if (ax < 0) {
                    ax = -ax;
                }
                if (az >= ax && az < 8001 && (best <= 0 || z < best)) {
                    best = z;
                    *out = q;
                }
            }
        }
    }
    if (best <= 0) {
        n = GetNumPlayers();
        *out = 0;
        for (i = 0; i < n; i++) {
            cs = GetPlayerCs3D(i);
            q = &cs->pos;
            if (q != 0 && i != who - 1) {
                d[0] = cs->pos.vx - p->vx;
                d[1] = cs->pos.vy - p->vy;
                d[2] = cs->pos.vz - p->vz;
                mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
                z = r[1];
                if (z >= 12) {
                    ax = r[0];
                    az = __builtin_abs(z);
                    if (ax < 0) {
                        ax = -ax;
                    }
                    if (az >= ax && az < 8001 && (best <= 0 || z < best)) {
                        best = z;
                        *out = q;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", carLockon);
#endif

#ifdef NON_MATCHING
void carLockon2(u8 who, LVECTOR** out)
{
    s32 d[3];
    s32 r[3];
    Cs* cs;
    Cs* cs2;
    LVECTOR* p;
    LVECTOR* q;
    s32 n;
    s32 i;
    s32 best;
    s32 z;
    s32 ax;

    best = 0;
    if (who == 0) {
        *out = &GetPlayerCs3D(0)->pos;
        return;
    }
    p = &GetPlayerCs3D((s16)(who - 1))->pos;
    n = GetNumAICars();
    *out = 0;
    for (i = 0; i < n; i++) {
        cs = GetAICs3D(i);
        q = &cs->pos;
        if (q != 0 && ((CarAlt*)GetAICarInfo(i))->stats.unk40 > 0) {
            d[0] = cs->pos.vx - p->vx;
            d[1] = cs->pos.vy - p->vy;
            do {
                d[2] = cs->pos.vz - p->vz;
            } while (0);
            mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
            z = r[1];
            if (z < -11) {
                ax = r[0];
                if (z < 0) {
                    z = -z;
                }
                if (ax < 0) {
                    ax = -ax;
                }
                if (z >= ax && z < 8001 && (best <= 0 || z < best)) {
                    best = z;
                    *out = q;
                }
            }
        }
    }
    if (best <= 0) {
        n = GetNumPlayers();
        *out = 0;
        for (i = 0; i < n; i++) {
            cs2 = GetPlayerCs3D(i);
            q = &cs2->pos;
            if (i + 1 != who && q != 0) {
                d[0] = cs2->pos.vx - p->vx;
                d[1] = cs2->pos.vy - p->vy;
                do {
                    d[2] = cs2->pos.vz - p->vz;
                } while (0);
                mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
                z = r[1];
                if (z < -11) {
                    ax = r[0];
                    if (z < 0) {
                        z = -z;
                    }
                    if (ax < 0) {
                        ax = -ax;
                    }
                    if (z >= ax && z < 8001 && (best <= 0 || z < best)) {
                        best = z;
                        *out = q;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", carLockon2);
#endif

void carInitFlamethrowers(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        flameThrower[i].life = 0;
    }
}

s16 find_free_flamethrower(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        if (flameThrower[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

void fireRearFlameThrower(Car* car, u8 isPlayer)
{
    CarWeap* pad;
    CarMotion* m;
    s32 range;

    if (isPlayer) {
        pad = &car->weap;
        m = &car->motion;
    } else {
        pad = &((CarAlt*)car)->weap;
        m = &((CarAlt*)car)->motion;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.x);
    range = soundGetCalculatedSoundRange();
    uasoundPlayCarWeaponLaunchOrInflight(7, range, soundGetCalculatedSoundXPosition());
    if (fireFlameThrower(car, isPlayer, 0, 0) != 0) {
        do {
            pad->fireDelay = 20;
        } while (0);
        pad->ammo[7] = pad->ammo[7] - 1;
    }
}

#ifdef NON_MATCHING
s16 fireFlameThrower(Car* car, u8 isPlayer, u8 rear, s32* pos)
{
    Cs* cs;
    s16 i;
    s32* dst;
    s32* wp;
    s32 range;

    if (isPlayer) {
        cs = GetPlayerCs3D((s16)car->playerIdx);
    } else {
        cs = GetAICs3D((s16)((CarAlt*)car)->playerIdx);
    }
    i = find_free_flamethrower();
    if (i < 0) {
        return 0;
    }
    flameThrower[i].rear = rear;
    flameThrower[i].cs = cs;
    if (rear == 0) {
        flameThrower[i].life = 12;
    } else {
        dst = flameThrower[i].pos;
        dst[0] = pos[0];
        dst[1] = pos[1];
        dst[2] = pos[2];
        flameThrower[i].life = 25;
        wp = flameThrower[i].world;
        wp[0] = (s32)((u32)cs->pos.vx + (u32)pos[0]);
        wp[1] = (s32)((u32)cs->pos.vy + (u32)pos[1]);
        wp[2] = (s32)((u32)cs->pos.vz + (u32)pos[2]);
        soundSetRangeAndXPositionFromWorldLoc(wp);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponLaunchOrInflight(7, range, soundGetCalculatedSoundXPosition());
    }
    flameThrower[i].flag = 0;
    return i + 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", fireFlameThrower);
#endif

#ifdef NON_MATCHING
void displayFlamethrowers(void)
{
    s32 p[3];
    s32 d[3];
    s32 hit;
    LVECTOR* cp;
    s32 i;
    s32 j;
    s32 n;
    s32 m;
    s32 lim;
    s32 life;
    s32 hitDone;
    s32 v;
    Cs* cs;
    HdPnt* pnt;

    cp = 0;
    for (i = 0; i < 5; i++) {
        life = flameThrower[i].life;
        if (life <= 0) {
            continue;
        }
        if (flameThrower[i].rear != 0) {
            if (flameThrower[i].rear != 2) {
                goto spray;
            }
            v = 32;
        } else {
            v = -25;
        }
        p[0] = 0;
        p[1] = v;
        p[2] = 0;
        flameThrower[i].life = flameThrower[i].life - 1;
        mathMulTransVec(&flameThrower[i].cs->mat, p, d);
        cs = flameThrower[i].cs;
        p[0] = cs->pos.vx + (d[0] << 2);
        p[1] = cs->pos.vy + (d[1] << 2);
        p[2] = cs->pos.vz + d[2];
        n = flameThrower[i].flag;
        hitDone = 0;
        lim = (flameThrower[i].rear << 2) + 12;
        flameThrower[i].flag = n + 1;
        if (lim < n) {
            n = lim;
        }
        cp = &cs->pos;
        if ((u8)n <= 0) {
            continue;
        }
        j = 0;
        do {
            p[0] = p[0] + d[0];
            p[1] = p[1] + d[1];
            p[2] = p[2] + 1 + d[2];
            if (p[2] < 8 && cp->vz > 0) {
                p[2] = 7;
            }
            do_flamethrower_burst(p);
            if ((j & 2) && (u8)hitDone == 0) {
                hit = CAR_HD(p, flameThrower[i].cs->unkC0, 0x11);
                if (hit > 0) {
                    hitDone = 1;
                    bulDispatchDamage(hit, 0, 0, flameDamage, d, 0);
                }
            }
            j = j + 1;
        } while (j < (u8)n);
        continue;
    spray:
        flameThrower[i].life = life - 1;
        flameThrower[i].world[0] = flameThrower[i].world[0] + flameThrower[i].pos[0];
        flameThrower[i].world[1] = flameThrower[i].world[1] + flameThrower[i].pos[1];
        flameThrower[i].world[2] = flameThrower[i].world[2] + flameThrower[i].pos[2];
        if (flameThrower[i].world[2] <= 0 && cp->vz > 0) {
            flameThrower[i].world[2] = 1;
        }
        if (flameThrower[i].flag < 20) {
            flameThrower[i].flag = flameThrower[i].flag + 1;
        }
        n = flameThrower[i].flag;
        j = 0;
        if (flameThrower[i].life & 1) {
            do_flare(flameThrower[i].world);
            j = 0;
        }
        m = (u8)n;
        do {
            p[0] = flameThrower[i].world[0] + ((((rand() & 0xF) - 7) * m) >> 1);
            p[1] = flameThrower[i].world[1] + ((((rand() & 0xF) - 7) * m) >> 1);
            p[2] = flameThrower[i].world[2] + ((((rand() & 0xF) - 7) * m) >> 1);
            if (p[2] <= 0 && cp->vz > 0) {
                p[2] = 1;
            }
            do_flamethrower_burst(p);
            j = j + 1;
        } while (j < 8);
        m = flameFuel - ((u8)n >> 1);
        hit = CAR_HD(p, flameThrower[i].cs->unkC0, (u8)n + 5);
        if (hit > 0) {
            bulDispatchDamage(hit, 0, 0, flameDamage, d, 0);
        } else {
            pnt = HdPntTest(flameThrower[i].cs->unkC0, 0, flameThrower[i].world, &hit);
            if (hit <= 0) {
                continue;
            }
            bulDispatchDamage(hit, (s16)pnt->unk0A, (s16)pnt->unk0C, flameDamage, d, 0);
        }
        flameThrower[i].life = 0;
        do_smoke(flameThrower[i].world);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", displayFlamethrowers);
#endif

#ifdef NON_MATCHING
void carResetGunHeat(Car* car)
{
    car->weap.gunOverheat = 0;
    car->weap.gunHeat = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", carResetGunHeat);
#endif

void carInitPickupWeapons(void)
{
    s32 i;

    for (i = 0; i < 50; i++) {
        pickup[i].kind = 0;
    }
    numPickups = 0;
}

void setPickup(s32* pos, s32 obj, s32 n)
{
    s32* dst;
    s32 first;

    if (n < 51) {
        n = n - 1;
        if (pickup[n].kind == 0) {
            numPickups = numPickups + 1;
            pickup[n].kind = 1;
            do {
                first = pos[0];
                dst = pickup[n].pos;
            } while (0);
            dst[0] = first;
            dst[1] = pos[1];
            dst[2] = pos[2];
            pickup[n].obj = obj;
        }
    }
}

#ifdef NON_MATCHING
void regeneratePickupWeapons(void)
{
    s32 i;
    s32 obj;

    if (pickupTimer < weaponRegenDelay) {
        pickupTimer = pickupTimer + 1;
    } else {
        pickupTimer = 0;
        if (numPickups > 0) {
            i = rand() % numPickups;
            do {
                obj = pickup[i].obj;
            } while (0);
            pickup[i].kind = 1;
            uaswSetNumChildren(obj, i + 1, 1);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", regeneratePickupWeapons);
#endif

#ifdef NON_MATCHING
void carGetPickup(Car* car, u8 isPlayer, s32 kind, s32 idx)
{
    CarWeap* pad;
    CarMotion* m;
    s32 cap;
    s32 amt;
    s32 range;
    s32 i;

    idx = idx - 1;
    if (isPlayer) {
        pad = &car->weap;
        m = &car->motion;
    } else {
        pad = &((CarAlt*)car)->weap;
        m = &((CarAlt*)car)->motion;
    }
    cap = (s16)pad->maxAmmo - (s16)pad->totalAmmo;
    if (rtIsSplitScreenOn() != 0) {
        cap = 30;
    }
    if (cap > 0 || kind == 0x1AE || isPlayer == 0) {
        switch (kind) {
        case 0x191:
            amt = weaponPickupAmount[0];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[0] = pad->ammo[0] + amt;
            break;
        case 0x192:
            amt = weaponPickupAmount[1];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[1] = pad->ammo[1] + amt;
            break;
        case 0x193:
            amt = weaponPickupAmount[2];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[2] = pad->ammo[2] + amt;
            break;
        case 0x194:
            amt = weaponPickupAmount[3];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[3] = pad->ammo[3] + amt;
            break;
        case 0x195:
            amt = weaponPickupAmount[5];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[5] = pad->ammo[5] + amt;
            break;
        case 0x196:
            amt = weaponPickupAmount[4];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[4] = pad->ammo[4] + amt;
            break;
        case 0x1C2:
            pad->ammo[11] = 20;
            break;
        case 0x19C:
            amt = weaponPickupAmount[8];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[8] = pad->ammo[8] + amt;
            break;
        case 0x19D:
            amt = weaponPickupAmount[9];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[9] = pad->ammo[9] + amt;
            break;
        case 0x19E:
            amt = weaponPickupAmount[10];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[10] = pad->ammo[10] + amt;
            break;
        case 0x19A:
            amt = weaponPickupAmount[6];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[6] = pad->ammo[6] + amt;
            break;
        case 0x19B:
            amt = weaponPickupAmount[7];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[7] = pad->ammo[7] + amt;
            break;
        case 0x1AF:
            pad->unk44 = (u32)pad->unk44 + (u32)weaponPickupAmount[13];
            break;
        case 0x1AE:
            pad->unk46 = (u32)pad->unk46 + (u32)weaponPickupAmount[12];
            break;
        }
        soundSetRangeAndXPositionFromWorldLoc(&m->pos.x);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponPickup(kind, range, soundGetCalculatedSoundXPosition());
        uaswSetNumChildren(kind, idx + 1, 0);
        pickup[idx].kind = 0;
        pad->totalAmmo = 0;
        if (rtIsSplitScreenOn() == 0) {
            for (i = 0; i < 11; i++) {
                pad->totalAmmo = pad->totalAmmo + pad->ammo[i];
            }
        }
        if ((s16)pad->totalAmmo == (s16)pad->maxAmmo && isPlayer && kind != 0x1AE) {
            uadashMaxCarryCapacity();
        }
    } else {
        uadashMaxCarryCapacity();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", carGetPickup);
#endif

u8 getBombDamage(void)
{
    return bombDamage;
}

u8 health_stand_active(s32 n)
{
    return HStand[n - 1].active;
}

void animate_health_stand(s32 n)
{
    if (rand() & 1) {
        uaswSetState(0x398, n, 1);
    } else {
        uaswSetState(0x398, n, 0);
    }
}

void deactivate_health_stand(s32 n)
{
    if ((u32)n - 1u < 10u) {
        HStand[n - 1].active = 0;
        uaswSetState(0x398, n, 1);
        numHealthStands = (s32)((u32)numHealthStands - 1u);
    }
}

void reactivate_healthstand(s32 n)
{
    if ((u32)n - 1u < 10u) {
        HStand[n - 1].active = 1;
        uaswSetState(0x398, n, 0);
    }
}

void init_health_stands(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        HStand[i].active = 0;
    }
    nextHealthStand = 0;
    numHealthStands = 0;
}

#ifdef NON_MATCHING
void set_health_stand(s32 n, s32* pos)
{
    s32* dst;
    s32 first;

    if ((u32)n - 1u < 10u) {
        HStand[n - 1].active = 1;
        do {
            first = pos[0];
        } while (0);
        dst = HStand[n - 1].pos;
        dst[0] = first;
        dst[1] = pos[1];
        dst[2] = pos[2];
        numHealthStands = (s32)((u32)numHealthStands + 1u);
        nextHealthStand = (s32)((u32)nextHealthStand + 1u);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", set_health_stand);
#endif

#ifdef NON_MATCHING
void regen_healthstands(void)
{
    Car* pi;
    s32 d[3];
    s32 i;
    s32 t;

    pi = GetPlayerInfo(0);
    do {
        for (i = 0; i < nextHealthStand; i++) {
            if (HStand[i].active != 0) {
                d[0] = (s32)((u32)HStand[i].pos[0] - (u32)pi->motion.pos.x);
                d[1] = (s32)((u32)HStand[i].pos[1] - (u32)pi->motion.pos.y);
                d[2] = (s32)((u32)HStand[i].pos[2] - (u32)pi->motion.pos.z);
                hudAddRadarSig(0x398, d, pi->motion.rot.z);
            }
        }
    } while (0);
    if (nextHealthStand != 0 && numHealthStands <= 0) {
        if (healthRegenPending == 0) {
            healthRegenPending = 1;
            healthRegenTimer = healthRegenDelay;
        } else {
            t = healthRegenTimer;
            if (t == 0) {
                numHealthStands = nextHealthStand;
                healthRegenPending = 0;
                for (i = 0; i < nextHealthStand; i++) {
                    reactivate_healthstand(i + 1);
                }
            } else {
                healthRegenTimer = (s32)((u32)t - 1u);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/interactives", regen_healthstands);
#endif

s32 carweapGetMonsterDamage(void)
{
    return monsterDamage;
}
