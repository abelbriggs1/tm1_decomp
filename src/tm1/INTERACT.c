#include "common.h"

#include "tm1/CAR.H"
#include "tm1/CS.H"
#include "tm1/DIFFICUL.H"
#include "tm1/EXPLODE.H"
#include "tm1/GRUTILS.H"
#include "tm1/HDP.H"
#include "tm1/HUD.H"
#include "tm1/INTERACT.H"
#include "tm1/LEVELS.H"
#include "tm1/MATH.H"
#include "tm1/RT.H"
#include "tm1/SHELL.H"
#include "tm1/SOUND.H"
#include "tm1/UA.H"
#include "tm1/UA_DASH.H"
#include "tm1/UA_SOUND.H"
#include "tm1/UA_SW.H"
#include "tm1/VIEW.H"
#include "tm1/WEAPON.H"
#include <ABS.H>
#include <LIBGPU.H>
#include <LIBGTE.H>
#include <RAND.H>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

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

s16 numTriggerPoints = 0;
s16 numCurbs = 0;
s16 numSlickSpots = 0;

// clang-format off
static s32 weaponPickupAmount[20] = {
    4,    2, 2, 2,
    1,    4, 2, 2,
    2,    2, 3, 1,
    1, 0x78, 1, 1,
    1,    1, 1, 1
};
// clang-format on

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

static GrSprite spikeSprite;
static GrSprite oilSprite;
static GrSprite catapultSprite;
static GrSprite bombSprite;

static DropWeap dropweapon[12];
static FlameThrower flameThrower[5];
static PickupWeap pickup[50];
static HealthStand HStand[10];
static Taser taser[3];

void carSetPowerupDelaysBySkillLevel(s32 level)
{
    s32 wd;

    switch (level) {
    case DIFFICULTY_EASY:
        healthRegenDelay = 2000;
        wd = 150;
        break;
    case DIFFICULTY_MEDIUM:
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
    case UA_LOW_RIDER:
        return weaponCost100;
    case UA_LAMBORGHINI:
        return weaponCost60;
    case UA_SEMI_TRUCK:
        return weaponCost30;
    case UA_ICE_CREAM_TRUCK:
        return weaponCost10;
    case UA_TAXI_CAB:
        return weaponCost20;
    case UA_DUNE_BUGGY:
        return weaponCost90;
    case UA_HUMVEE:
        return weaponCost70;
    case UA_HARLEY:
        return weaponCost80;
    case UA_CORVETTE:
        return weaponCost110;
    case UA_POLICE_CAR:
        return weaponCost50;
    case UA_BOSS_CAR:
        return weaponCost130;
    case UA_MONSTER_TRUCK:
        return weaponCost40;
    case UA_MAD_MAX:
        return weaponCost120;
    }
    return 20;
}

void UpdateWeapons(Car* car, u8 isPlayer)
{
    CarWeap* pad;
    s32 i;
    s16 t;

    if (isPlayer) {
        UpdatePlayerWeaponPadStatus(PLAYER_CAR(car));
        pad = &PLAYER_CAR(car)->weap;
    } else {
        UpdateAIWeaponPadStatus(AI_CAR(car));
        pad = &AI_CAR(car)->weap;
    }
    if (pad->reset != 0) {
        for (i = 0; i < WEAPON_AMMO_MAX; i++) {
            pad->ammo[i] = 1;
        }
        pad->ammo[WEAPON_SINGING_MISSILE] = 0;
        pad->ammo[WEAPON_NONE] = 0;
        pad->ammo[WEAPON_SPECIAL] = 20;
        pad->unk46 = 2;
    } else {
        t = pad->timer++;
        if (t >= ammoRegenRate) {
            pad->timer = 0;
            if ((s16)pad->ammo[WEAPON_SPECIAL] + 1 < 20) {
                pad->ammo[WEAPON_SPECIAL] = pad->ammo[WEAPON_SPECIAL] + 1;
            } else {
                pad->ammo[WEAPON_SPECIAL] = 20;
            }
            if (!isPlayer && AI_CAR(car)->unk40 == 0
                && AI_CAR(car)->stats.unk40 < (AI_CAR(car)->stats.unk44 >> 2)) {
                AI_CAR(car)->stats.unk40 = AI_CAR(car)->stats.unk40 + 1;
            }
        }
    }
    UpdateGuns(car, isPlayer);
    UpdateNonGuns(car, isPlayer);
}

void UpdatePlayerWeaponPadStatus(PlayerCar* car)
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
            if (car->weap.cur >= WEAPON_MAX) {
                car->weap.cur = WEAPON_FIRE_MISSILE;
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
        if (car->weap.cur == WEAPON_FIRE_MISSILE) {
            car->weap.cur = WEAPON_SPECIAL;
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
            if (car->weap.cur == WEAPON_FIRE_MISSILE) {
                car->weap.cur = WEAPON_SPECIAL;
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

void UpdateAIWeaponPadStatus(AICar* car)
{
    s32 n;

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
        if (__builtin_abs(car->unk2C) < n * 136 && car->unk34 < 4800) {
            car->skid[9] = 1;
        }
        if (car->uaIndex == UA_BOSS_CAR && car->direction == DIRECTION_REAR) {
            if (__builtin_abs(car->unk2C) > (((180 - n * 10) << 12) / 360)) {
                car->skid[9] = 1;
            }
        }
        if (car->weap.cur == WEAPON_HOMING_MISSILE && n < 3) {
            n += 2;
        }
        if (car->weap.cur == WEAPON_SPECIAL && n < 3
            && (car->uaIndex == UA_HUMVEE || car->uaIndex == UA_CORVETTE)) {
            n += 2;
        }
        if (__builtin_abs(car->unk2C) < n * 113) {
            car->skid[10] = 1;
        } else if (car->direction == DIRECTION_REAR) {
            if (__builtin_abs(car->unk2C) > (((180 - n * 10) << 12) / 360)) {
                car->skid[10] = 1;
            }
        }
        if (car->weap.cur < WEAPON_REGULAR_MAX) {
            car->weap.ammo[car->weap.cur] = 10;
        }
    }
}

void UpdateGuns(Car* car, u8 isPlayer)
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
        cfg = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        sk = PLAYER_CAR(car)->skid;
        pad = &PLAYER_CAR(car)->weap;
        aim = &PLAYER_CAR(car)->bounce;
        idx = PLAYER_CAR(car)->playerIdx + 1;
        snd = GetPlayerInfo((s16)PLAYER_CAR(car)->playerIdx)->uaIndex;
    } else {
        cfg = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        sk = AI_CAR(car)->skid;
        pad = &AI_CAR(car)->weap;
        aim = &AI_CAR(car)->bounce;
        idx = AI_CAR(car)->playerIdx + 50;
        snd = GetAICarInfo((s16)AI_CAR(car)->playerIdx)->uaIndex;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.vx);
    if ((s16)pad->gunDelay > 0) {
        --pad->gunDelay;
        return;
    }
    if (!isPlayer) {
        if (rand() % 100 + 1 > (s16)pad->specialChance) {
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
            pad->gunHeat = MAX(0, (s16)pad->gunHeat - 1);
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
        // Keep muzzle setup together to preserve PsyQ's load scheduling.
        do {
            if (pad->unk04 != 0) {
                v[0] = -(s16)cfg->unk110;
            } else {
                v[0] = (s16)cfg->unk110;
            }
            v[1] = (s16)cfg->unk112;
            v[2] = (s16)cfg->unk114;
        } while (0);
        mat = &m->mat2;
        pad->unk04 = 1 - pad->unk04;
        mathMulTransVec(mat, v, d);
        d[0] = d[0] + m->pos.vx;
        d[1] = d[1] + m->pos.vy;
        d[2] = d[2] + m->pos.vz;
        r[0] = rsin(m->rot.vz - aim->unk38);
        r[1] = rcos(m->rot.vz - aim->unk38);
        r[2] = -rsin(m->rot.vx - aim->unk30);
        create_bullet((s16)idx, r, d, gunDamage);
        do_mflash(d);
        if (snd == 0x82) {
            if (pad->unk04 != 0) {
                v[0] = 0;
                v[1] = -v[1];
                mathMulTransVec(mat, v, d);
                d[0] = d[0] + m->pos.vx;
                d[1] = d[1] + m->pos.vy;
                d[2] = d[2] + m->pos.vz;
                r[0] = -r[0];
                r[1] = -r[1];
                r[2] = -r[2];
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

void UpdateNonGuns(Car* car, u8 isPlayer)
{
    CarStats* cfg;
    u8* sk;
    CarWeap* pad;
    Cs* cs;
    s16 idx;

    if (isPlayer) {
        cfg = &PLAYER_CAR(car)->stats;
        sk = PLAYER_CAR(car)->skid;
        pad = &PLAYER_CAR(car)->weap;
        idx = PLAYER_CAR(car)->playerIdx + 1;
        cs = GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
    } else {
        cfg = &AI_CAR(car)->stats;
        sk = AI_CAR(car)->skid;
        pad = &AI_CAR(car)->weap;
        idx = AI_CAR(car)->playerIdx + 50;
        cs = GetAICs3D((s16)AI_CAR(car)->playerIdx);
    }
    if ((s16)pad->fireDelay > 0) {
        pad->fireDelay--;
        return;
    }
    if (pad->cur == WEAPON_NONE) {
        sk[10] = 0;
    }
    if ((s16)cfg->unk18 > 0) {
        sk[10] = 0;
    }
    if (sk[10] != 0) {
        if (isPlayer == 0 && pad->cur != WEAPON_SPECIAL) {
            pad->ammo[pad->cur] = 5;
        }
        if (pad->ammo[pad->cur] != 0) {
            if (isPlayer == 0) {
                UASetBattleMusicOn();
            } else {
                isPlayer = cs->unkC0;
            }
            switch (pad->cur) {
            case WEAPON_CATAPULT:
            case WEAPON_BOMB:
            case WEAPON_OIL:
            case WEAPON_SPIKE:
                LaunchDrops(car, isPlayer);
                break;
            case WEAPON_FLAMETHROWER:
                fireRearFlameThrower(car, isPlayer);
                break;
            case WEAPON_FIRE_MISSILE:
            case WEAPON_FREEZE_MISSILE:
            case WEAPON_HOMING_MISSILE:
            case WEAPON_POWER_MISSILE:
            case WEAPON_SINGING_MISSILE:
            case WEAPON_REAR_MISSILE:
                FireMissiles(car, isPlayer);
                break;
            case WEAPON_SPECIAL:
                FireSpecials(car, isPlayer);
                break;
            case WEAPON_NONE:
                break;
            }
        }
    }
}

void FireMissiles(Car* car, u8 isPlayer)
{
    s32 d[3];
    s32 v[3];
    VECTOR3* tgt;
    CarStats* cfg;
    CarMotion* m;
    CarWeap* pad;
    u8* flags;
    Cs* cs;
    Missile* mp;
    s32 r;
    s16 idx;

    if (isPlayer) {
        cfg = &PLAYER_CAR(car)->stats;
        idx = PLAYER_CAR(car)->playerIdx + 1;
        m = &PLAYER_CAR(car)->motion;
        pad = &PLAYER_CAR(car)->weap;
        flags = PLAYER_CAR(car)->flags;
        cs = GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
    } else {
        cfg = &AI_CAR(car)->stats;
        idx = AI_CAR(car)->playerIdx + 50;
        m = &AI_CAR(car)->motion;
        pad = &AI_CAR(car)->weap;
        flags = AI_CAR(car)->flags;
        cs = GetAICs3D((s16)AI_CAR(car)->playerIdx);
    }
    v[0] = (s16)cfg->dropPower;
    v[1] = 0;
    v[2] = cfg->unk70 + 8;
    mathMulTransVec(&m->mat2, v, d);
    d[0] = d[0] + m->pos.vx;
    d[1] = d[1] + m->pos.vy;
    d[2] = d[2] + m->pos.vz;
    r = -1;
    switch (pad->cur) {
    case WEAPON_FIRE_MISSILE:
        carLockon(isPlayer, &tgt);
        r = create_FIRE_missile(cs->unkC0, tgt, &m->rot, d, missileDamageFire);
        break;
    case WEAPON_FREEZE_MISSILE:
        r = create_FREEZE_missile(cs->unkC0, &m->rot, d, HIT_FREEZE);
        break;
    case WEAPON_POWER_MISSILE:
        r = create_POWER_missile(cs->unkC0, &m->rot, d, missileDamagePower);
        break;
    case WEAPON_SINGING_MISSILE:
        r = create_SINGING_missile(cs->unkC0, &m->rot, d, missileDamageSinging);
        break;
    case WEAPON_REAR_MISSILE:
        carLockon2(isPlayer, &tgt);
        r = create_REAR_missile(cs->unkC0, tgt, &m->rot, d, missileDamageRear);
        break;
    case WEAPON_HOMING_MISSILE:
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

static inline void CurrentSpecialLaunch(CarWeap* pad, s32 cost, s32 sound)
{
    s32 range;

    pad->ammo[pad->cur] -= cost;
    uasoundStopCarSpecialWeapon(sound);
    range = soundGetCalculatedSoundRange();
    uasoundPlayCarSpecialWeaponLaunchOrInflight(sound, range, soundGetCalculatedSoundXPosition());
}

static inline void SpecialLaunch(CarWeap* pad, s32 cost, s32 sound)
{
    s32 range;

    pad->ammo[WEAPON_SPECIAL] -= cost;
    uasoundStopCarSpecialWeapon(sound);
    range = soundGetCalculatedSoundRange();
    uasoundPlayCarSpecialWeaponLaunchOrInflight(sound, range, soundGetCalculatedSoundXPosition());
}

void FireSpecials(Car* car, u8 isPlayer)
{
    s32 d[3];
    s32 v[3];
    s32 r[3];
    VECTOR3* tgt;
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
        idx = PLAYER_CAR(car)->playerIdx + 1;
        GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
        kind = PLAYER_CAR(car)->uaIndex;
        cfg = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        pad = &PLAYER_CAR(car)->weap;
        flags = PLAYER_CAR(car)->flags;
    } else {
        idx = AI_CAR(car)->playerIdx + 50;
        GetAICs3D((s16)AI_CAR(car)->playerIdx);
        kind = AI_CAR(car)->uaIndex;
        cfg = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        pad = &AI_CAR(car)->weap;
        flags = AI_CAR(car)->flags;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.vx);
    v[0] = 0;
    v[1] = (s16)cfg->unk112;
    v[2] = (s16)cfg->unk114;
    mat = &m->mat2;
    mathMulTransVec(mat, v, d);
    d[0] = d[0] + m->pos.vx;
    d[1] = d[1] + m->pos.vy;
    d[2] = d[2] + m->pos.vz;
    v[0] = 0;
    v[1] = -100;
    v[2] = 0;
    mathMulTransVec(mat, v, r);
    r[0] = rsin(m->rot.vz) >> 6;
    r[1] = rcos(m->rot.vz) >> 6;
    r[2] = -rsin(m->rot.vx) >> 6;
    switch (kind) {
    case UA_SEMI_TRUCK:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost30) {
            d[2] = d[2] - 16;
            s_create_bullet(idx, r, d, 4, damage30, 12, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost30;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_SEMI_TRUCK);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_SEMI_TRUCK, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_LAMBORGHINI:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost60) {
            s_create_bullet(idx, r, d, 6, damage60, 15, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost60;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_LAMBORGHINI);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_LAMBORGHINI, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_TAXI_CAB:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost20) {
            s_create_bullet(idx, r, d, 17, damage20, 30, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost20;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_TAXI_CAB);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_TAXI_CAB, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_DUNE_BUGGY:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost90) {
            s_create_bullet(idx, r, d, 19, damage90, 30, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost90;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_DUNE_BUGGY);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_DUNE_BUGGY, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_ICE_CREAM_TRUCK:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost10) {
            s_create_bullet(idx, r, d, 15, damage10, 30, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost10;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_ICE_CREAM_TRUCK);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_ICE_CREAM_TRUCK, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_HUMVEE:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost70) {
            MATRIX* mat2;
            s32 res;
            carLockon(isPlayer, &tgt);
            mat2 = &m->mat2;
            v[0] = (s16)cfg->dropPower;
            v[1] = 0;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            res = create_SWARM_missile(idx, tgt, &m->rot, d, damage70);
            count = res >= 0;
            v[0] = (s16)cfg->unk110;
            v[1] = (s16)cfg->unk112;
            v[2] = (s16)cfg->unk114;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            if (create_SWARM_missile(idx, tgt, &m->rot, d, damage70) >= 0) {
                count = count + 1;
            }
            v[0] = -(s16)cfg->unk110;
            v[1] = (s16)cfg->unk112;
            v[2] = (s16)cfg->unk114;
            mathMulTransVec(mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            if (create_SWARM_missile(idx, tgt, &m->rot, d, damage70) >= 0) {
                count = count + 1;
            }
            if (count != 0) {
                SpecialLaunch(pad, weaponCost70, SOUND_SPECIAL_HUMVEE);
            }
        }
        break;
    case UA_HARLEY:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost80) {
            s_create_bullet(idx, r, d, 21, damage80, 20, 0);
            pad->ammo[WEAPON_SPECIAL] -= weaponCost80;
            uasoundStopCarSpecialWeapon(SOUND_SPECIAL_HARLEY);
            range = soundGetCalculatedSoundRange();
            uasoundPlayCarSpecialWeaponLaunchOrInflight(
                SOUND_SPECIAL_HARLEY, range, soundGetCalculatedSoundXPosition());
        }
        break;
    case UA_CORVETTE:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost110) {
            carLockon(isPlayer, &tgt);
            v[0] = (s16)cfg->dropPower;
            v[1] = 0;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            if (create_GHOST_missile(idx, tgt, &m->rot, d, damage110) >= 0) {
                SpecialLaunch(pad, weaponCost110, SOUND_SPECIAL_CORVETTE);
            }
        }
        break;
    case UA_POLICE_CAR:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost50) {
            if (fire_taser(car, isPlayer) != 0) {
                if (isPlayer) {
                    pad->ammo[WEAPON_SPECIAL] -= weaponCost50;
                } else {
                    pad->ammo[WEAPON_SPECIAL] = pad->ammo[WEAPON_SPECIAL] - weaponCost50 / 2;
                }
                uasoundStopCarSpecialWeapon(SOUND_SPECIAL_POLICE_CAR);
                range = soundGetCalculatedSoundRange();
                uasoundPlayCarSpecialWeaponLaunchOrInflight(
                    SOUND_SPECIAL_POLICE_CAR, range, soundGetCalculatedSoundXPosition());
            }
        }
        break;
    case UA_MAD_MAX:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost120) {
            carLockon(isPlayer, &tgt);
            v[0] = (s16)cfg->dropPower;
            v[1] = 0;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            if (create_DEATHSPEAR_missile(idx, tgt, &m->rot, d, damage120) >= 0) {
                SpecialLaunch(pad, weaponCost120, SOUND_SPECIAL_MAD_MAX);
            }
        }
        break;
    case UA_BOSS_CAR:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost100) {
            VECTOR3 rot2;
            carLockon(isPlayer, &tgt);
            v[0] = (s16)cfg->dropPower;
            v[1] = 0;
            v[2] = cfg->unk70 + 8;
            mathMulTransVec(&m->mat2, v, d);
            d[0] = d[0] + m->pos.vx;
            d[1] = d[1] + m->pos.vy;
            d[2] = d[2] + m->pos.vz;
            create_SWARM_missile(idx, tgt, &m->rot, d, damage130);
            carLockon2(isPlayer, &tgt);
            rot2.vx = m->rot.vx;
            rot2.vy = m->rot.vy;
            rot2.vz = m->rot.vz + 2048;
            create_SWARM_missile(idx, tgt, &rot2, d, damage130);
            s_create_bullet(idx, r, d, 19, damage130, 30, 0);
            fire_taser(car, isPlayer);
            fireFlameThrower(car, isPlayer, 2, r);
            CurrentSpecialLaunch(pad, weaponCost130, SOUND_SPECIAL_BOSS_CAR);
        }
        break;
    case UA_LOW_RIDER:
        if ((s16)pad->ammo[WEAPON_SPECIAL] >= weaponCost100) {
            fireFlameThrower(car, isPlayer, 2, r);
            CurrentSpecialLaunch(pad, weaponCost100, SOUND_SPECIAL_LOW_RIDER);
        }
        break;
    }
    pad->fireDelay = (kind == 60 || kind == 30 || kind == 90) ? 10 : 20;
    flags[9] = 1;
}

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
    s32 dist;
    s32 numP;
    s32 numA;
    s32 i;

    bestObj = 0;
    if (isPlayer != 0) {
        m = &PLAYER_CAR(car)->motion;
        cs = GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
    } else {
        m = &AI_CAR(car)->motion;
        cs = GetAICs3D((s16)AI_CAR(car)->playerIdx);
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
        dist = MAX(__builtin_abs(m->pos.vx - other->pos.vx),
                   MAX(__builtin_abs(m->pos.vy - other->pos.vy),
                       __builtin_abs(m->pos.vz - other->pos.vz)))
            >> 3;
        if (other->unkC0 != myId) {
            if (dist < taserRange) {
                if (dist < best) {
                    best = dist;
                    bestId = other->unkC0;
                    bestObj = &GetPlayerInfo(i)->motion.pos.vx;
                }
            }
        }
    }
    for (i = 0; i < numA; i++) {
        s32 aiIndex = i;
        if (GetAICarInfo(aiIndex)->stats.unk40 > 0) {
            other = GetAICs3D(aiIndex);
            dist = MAX(__builtin_abs(m->pos.vx - other->pos.vx),
                       MAX(__builtin_abs(m->pos.vy - other->pos.vy),
                           __builtin_abs(m->pos.vz - other->pos.vz)))
                >> 3;
            if (other->unkC0 != myId) {
                if (dist < taserRange) {
                    if (dist < best) {
                        best = dist;
                        bestId = other->unkC0;
                        bestObj = &GetAICarInfo(aiIndex)->motion.pos.vx;
                    }
                }
            }
        }
    }
    taser[idx].life = 10;
    if (bestId < 0) {
        taser[idx].target = -1;
        taser[idx].targetPos = 0;
    } else {
        taser[idx].target = bestId;
        taser[idx].targetPos = bestObj;
    }
    taser[idx].pos = &m->pos.vx;
    return 1;
}

void draw_tasers(RenderCtx* ctx, s32 view)
{
    volatile s32 tp[3];
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
                if (shellGetCurrentLevel() == LEVEL6_ROOFTOPS) {
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
                ((VECTOR3*)pt)[j].vx = taser[i].pos[0] + d[0] * j / 16;
                ((VECTOR3*)pt)[j].vy = taser[i].pos[1] + d[1] * j / 16;
                ((VECTOR3*)pt)[j].vz = (taser[i].pos[2] + 30) + (d[2] * j / 16);
                pt[j][2] -= 32 - (rand() & 0x3F);
                pt[j][1] -= 8 - (rand() & 0xF);
                pt[j][0] -= 8 - (rand() & 0xF);
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
            --taser[i].life;
            if (taser[i].target >= 0) {
                if (taser[i].life == 0) {
                    carTakeHit((s16)taser[i].target, D_8018C034, NULL, 0);
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

static inline void ConsumeWeaponAmmo(CarWeap* pad)
{
    pad->ammo[pad->cur]--;
    pad->totalAmmo--;
}

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
        cfg = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        pad = &PLAYER_CAR(car)->weap;
        flags = PLAYER_CAR(car)->flags;
        idx = PLAYER_CAR(car)->playerIdx + 1;
        GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
        t = PLAYER_CAR(car)->tires;
    } else {
        cfg = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        pad = &AI_CAR(car)->weap;
        flags = AI_CAR(car)->flags;
        idx = AI_CAR(car)->playerIdx + 50;
        GetAICs3D((s16)AI_CAR(car)->playerIdx);
        t = AI_CAR(car)->tires;
    }
    if ((s16)t[2].unk22 < (s16)t[3].unk22) {
        power = (s16)t[2].unk22;
    } else {
        power = (s16)t[3].unk22;
    }
    v[0] = (s16)cfg->dropPower;
    v[1] = 0;
    v[2] = 0;
    mathMulTransVec(&m->mat2, v, d);
    d[0] = d[0] + m->pos.vx;
    d[1] = d[1] + m->pos.vy;
    d[2] = d[2] + m->pos.vz;
    soundSetRangeAndXPositionFromWorldLoc(d);
    weapon = pad->cur;
    if (carDropWeapon(weapon, idx, d, power)) {
        pad->fireDelay = 20;
        flags[9] = 1;
        ConsumeWeaponAmmo(pad);
        uasoundStopCarWeapon(pad->cur);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponLaunchOrInflight(pad->cur, range, soundGetCalculatedSoundXPosition());
    }
}

void carAddWeapTex(GrObj* obj, s32 id)
{
    switch (id) {
    case UA_BOMB:
        grutilsParse3DSprite(obj, &bombSprite, 1);
        break;
    case UA_OIL:
        grutilsParse3DSprite(obj, &oilSprite, 1);
        break;
    case UA_SPIKE:
        grutilsParse3DSprite(obj, &spikeSprite, 1);
        break;
    case UA_CATAPULT:
        grutilsParse3DSprite(obj, &catapultSprite, 1);
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
        case WEAPON_BOMB:
            sprite = &bombSprite;
            dropweapon[i].type = HIT_BOMB;
            break;
        case WEAPON_CATAPULT:
            sprite = &catapultSprite;
            dropweapon[i].type = HIT_CATAPULT;
            break;
        case WEAPON_OIL:
            sprite = &oilSprite;
            dropweapon[i].type = HIT_OIL;
            break;
        case WEAPON_SPIKE:
            sprite = &spikeSprite;
            dropweapon[i].type = HIT_SPIKE;
            break;
        default:
            dropweapon[i].life = 0;
            return 0;
        }
    } while (0);
    dropweapon[i].sprite = sprite;
    return i + 1;
}

void check_dropweap_impact(s32 i)
{
    volatile VECTOR3 v;
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
        if (d->kind == WEAPON_BOMB) {
            d->pos[2] = d->pos[2] + 50;
            do_big_explosion(pos);
            d->pos[2] = d->pos[2] - 50;
            do_big_explosion(pos);
        }
        if (d->kind == WEAPON_SPIKE) {
            do_smoke(pos);
        }
        if (d->kind != WEAPON_BOMB) {
            for (j = 0; j < 6; j++) {
                v.vx = d->pos[0] + (rand() & 0x3F) - 32;
                v.vy = d->pos[1] + (rand() & 0x3F) - 32;
                v.vz = d->pos[2];
                do_puff(&v);
            }
        }
        if (d->kind != WEAPON_OIL) {
            d->life = 0;
        }
        v.vx = 0;
        v.vy = 0;
        v.vz = 100;
        if ((s16)d->type == HIT_SPIKE) {
            bulDispatchDamage(hit, 0, 0, spikeDamage, &v, -(s32)d->kind);
        } else if ((s16)d->type == HIT_BOMB) {
            bulDispatchDamage(hit, 0, 0, bombDamage, &v, -(s32)d->kind);
        }
        bulDispatchDamage(hit, 0, 0, (s16)d->type, &v, -(s32)d->kind);
    }
}

void drawOil(OilPrim* prim, s32* pos, s32 view)
{
    VECTOR3 out[4];
    volatile VECTOR3 v;
    s32 cx;
    s32 cy;
    s32 hfov;
    s32 vfov;
    MATRIX* mat;
    s32 i;
    s32 z;

    viewGetCenter(view, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    mat = (MATRIX*)viewGetEyeMat(view);
    v.vx = pos[0] - 48;
    v.vy = pos[1] - 48;
    v.vz = pos[2];
    mathMulVec(mat, &v, &out[0]);
    v.vx = pos[0] + 48;
    v.vy = pos[1] - 48;
    v.vz = pos[2];
    mathMulVec(mat, &v, &out[1]);
    v.vx = pos[0] - 48;
    v.vy = pos[1] + 48;
    v.vz = pos[2];
    mathMulVec(mat, &v, &out[2]);
    v.vx = pos[0] + 48;
    v.vy = pos[1] + 48;
    v.vz = pos[2];
    mathMulVec(mat, &v, &out[3]);
    for (i = 0; i < 4; i++) {
        z = out[i].vz;
        if (z < 16) {
            z = 16;
        }
        out[i].vx = cx + (out[i].vx * hfov) / z;
        out[i].vy = cy + (out[i].vy * vfov) / z;
    }
    prim->x0 = out[0].vx;
    prim->y0 = out[0].vy;
    prim->x1 = out[1].vx;
    prim->y1 = out[1].vy;
    prim->x2 = out[2].vx;
    prim->y2 = out[2].vy;
    prim->x3 = out[3].vx;
    prim->y3 = out[3].vy;
}

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
    s32 groundZ;
    s32 z;
    s32 pw;
    s32 kind;
    s32 depth;
    s32 sc;
    s32 w2;
    s32 x0;
    s32 y0;
    s32 y1;

    viewGetCenter(view, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    tr = viewGetEyeTrans(view);
    mat = (MATRIX*)viewGetEyeMat(view);
    for (i = 0; i < 12; i++) {
        if (dropweapon[i].life > 0) {
            // Keep the height read separate for PsyQ load scheduling.
            do {
                groundZ = dropweapon[i].pos[2];
            } while (0);
            pw = dropweapon[i].power;
            if (pw < groundZ) {
                if (groundZ - 12 >= pw) {
                    pw = groundZ - 12;
                }
                dropweapon[i].pos[2] = pw;
            }
            --dropweapon[i].life;
            check_dropweap_impact(i);
            w[0] = dropweapon[i].pos[0] + tr->x;
            w[1] = dropweapon[i].pos[1] + tr->y;
            w[2] = dropweapon[i].pos[2] + tr->z;
            mathMulVec(mat, w, t);
            z = t[2];
            if (__builtin_abs(t[0]) < 641) {
                if (__builtin_abs(t[1]) < 641 && z >= 16) {
                    if (z < 30001) {
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
                            depth = z;
                            z >>= 3;
                            if (kind == 9) {
                                z--;
                            }
                            if (z < 0) {
                                z = 0;
                            }
                            if (kind != 9) {
                                sc = hfov * 16 / depth;
                                if (sc >= 513) {
                                    sc = 512;
                                }
                                if (sc <= 0) {
                                    sc = 1;
                                }
                                x0 = cx + t[0] * hfov / depth - sc;
                                w2 = sc * 2;
                                y0 = cy + t[1] * vfov / depth - w2;
                                p->x0 = x0;
                                p->y0 = y0;
                                p->x1 = x0 + w2;
                                p->y1 = y0;
                                y1 = y0 + sc;
                                p->x2 = x0;
                                p->y2 = y1;
                                p->x3 = x0 + w2;
                                p->y3 = y1;
                            } else {
                                drawOil(p, w, view);
                            }
                            AddPrim(ctx->ot + z, p);
                        }
                    }
                }
            }
        }
    }
}

void carLockon(u8 who, VECTOR3** out)
{
    volatile s32 d[3];
    s32 r[3];
    Cs* cs;
    Cs* cs2;
    VECTOR3* p;
    VECTOR3* q;
    s32 n;
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
        if (q != 0 && GetAICarInfo(i)->stats.unk40 > 0) {
            d[0] = cs->pos.vx - p->vx;
            d[1] = cs->pos.vy - p->vy;
            d[2] = cs->pos.vz - p->vz;
            mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
            z = r[1];
            if (z >= 12) {
                ax = r[0];
                az = ABS(z);
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
            cs2 = GetPlayerCs3D(i);
            q = &cs2->pos;
            if (q != 0 && i != who - 1) {
                d[0] = cs2->pos.vx - p->vx;
                d[1] = cs2->pos.vy - p->vy;
                d[2] = cs2->pos.vz - p->vz;
                mathMulVec(&GetPlayerCs3D((s16)(who - 1))->mat, d, r);
                z = r[1];
                if (z >= 12) {
                    ax = r[0];
                    az = ABS(z);
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

void carLockon2(u8 who, VECTOR3** out)
{
    volatile s32 d[3];
    s32 r[3];
    Cs* cs;
    Cs* cs2;
    VECTOR3* p;
    VECTOR3* q;
    s32 n;
    s32 i;
    s32 best;

    best = 0;
    if (who == 0) {
        *out = &GetPlayerCs3D(0)->pos;
        return;
    }
    p = &GetPlayerCs3D((s16)(who - 1))->pos;
    n = GetNumAICars();
    *out = 0;
    for (i = 0; i < n; i++) {
        s32 z;
        s32 ax;

        cs = GetAICs3D(i);
        q = &cs->pos;
        if (q != 0 && GetAICarInfo(i)->stats.unk40 > 0) {
            d[0] = cs->pos.vx - p->vx;
            d[1] = cs->pos.vy - p->vy;
            d[2] = cs->pos.vz - p->vz;
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
            s32 z;
            s32 ax;

            cs2 = GetPlayerCs3D(i);
            q = &cs2->pos;
            if (i + 1 != who && q != 0) {
                d[0] = cs2->pos.vx - p->vx;
                d[1] = cs2->pos.vy - p->vy;
                d[2] = cs2->pos.vz - p->vz;
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
        pad = &PLAYER_CAR(car)->weap;
        m = &PLAYER_CAR(car)->motion;
    } else {
        pad = &AI_CAR(car)->weap;
        m = &AI_CAR(car)->motion;
    }
    soundSetRangeAndXPositionFromWorldLoc(&m->pos.vx);
    range = soundGetCalculatedSoundRange();
    uasoundPlayCarWeaponLaunchOrInflight(
        WEAPON_FLAMETHROWER, range, soundGetCalculatedSoundXPosition());
    if (fireFlameThrower(car, isPlayer, 0, 0) != 0) {
        do {
            pad->fireDelay = 20;
        } while (0);
        pad->ammo[WEAPON_FLAMETHROWER] = pad->ammo[WEAPON_FLAMETHROWER] - 1;
    }
}

s16 fireFlameThrower(Car* car, u8 isPlayer, u8 rear, s32* pos)
{
    Cs* cs;
    s16 i;
    s32* dst;
    s32* wp;
    s32 range;

    if (isPlayer) {
        cs = GetPlayerCs3D((s16)PLAYER_CAR(car)->playerIdx);
    } else {
        cs = GetAICs3D((s16)AI_CAR(car)->playerIdx);
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
        uasoundPlayCarWeaponLaunchOrInflight(
            WEAPON_FLAMETHROWER, range, soundGetCalculatedSoundXPosition());
    }
    flameThrower[i].flag = 0;
    return i + 1;
}

void displayFlamethrowers(void)
{
    s32 p[3];
    s32 d[3];
    s32 hit;
    VECTOR3* cp;
    s32 i;
    s32 j;
    u8 n;
    s32 m;
    s32 lim;
    s32 life;
    s32 hitDone;
    s32 offset;
    s32 temp;
    Cs* cs;
    HdCsHit* pnt;

    cp = 0;
    for (i = 0; i < 5; i++) {
        life = flameThrower[i].life;
        if (life <= 0) {
            continue;
        }
        temp = flameThrower[i].rear;
        if (temp != 0) {
            if (temp != 2) {
                goto spray;
            }
            offset = 32;
        } else {
            offset = -25;
        }
        p[0] = 0;
        p[1] = offset;
        p[2] = 0;
        do {
            flameThrower[i].life--;
            mathMulTransVec(&flameThrower[i].cs->mat, p, d);
        } while (0);
        cs = flameThrower[i].cs;
        p[0] = cs->pos.vx + (d[0] << 2);
        p[1] = cs->pos.vy + (d[1] << 2);
        p[2] = cs->pos.vz + d[2];
        n = flameThrower[i].flag++;
        hitDone = 0;
        lim = (flameThrower[i].rear << 2) + 12;

        cp = &cs->pos;
        if (lim < n) {
            n = lim;
        }
        for (j = 0; j < n; j++) {
            p[0] = p[0] + d[0];
            p[1] = p[1] + d[1];
            p[2] += 1 + d[2];
            if (p[2] < 8 && cp->vz > 0) {
                p[2] = 7;
            }
            do_flamethrower_burst(p);
            if ((j & 2) && (u8)hitDone == 0) {
                hit = CAR_HD(p, flameThrower[i].cs->unkC0, 0x11);
                if (hit > 0) {
                    bulDispatchDamage(hit, 0, 0, flameDamage, d, 0);
                    hitDone = 1;
                }
            }
        }
        continue;
    spray:
        flameThrower[i].life--;
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
        if (flameThrower[i].life & 1) {
            do_flare(flameThrower[i].world);
        }
        for (j = 0; j < 8; j++) {
            p[0] = flameThrower[i].world[0] + ((((rand() & 0xF) - 7) * n) >> 1);
            p[1] = flameThrower[i].world[1] + ((((rand() & 0xF) - 7) * n) >> 1);
            p[2] = flameThrower[i].world[2] + ((((rand() & 0xF) - 7) * n) >> 1);
            if (p[2] <= 0 && cp->vz > 0) {
                p[2] = 1;
            }
            do_flamethrower_burst(p);
        }
        do {
            temp = n >> 1;
            m = flameFuel - temp;
        } while (0);
        hit = CAR_HD(p, flameThrower[i].cs->unkC0, n + 5);
        if (hit > 0) {
            bulDispatchDamage(hit, 0, 0, flameDamage, d, 0);
            flameThrower[i].life = 0;
            do_smoke(flameThrower[i].world);
        } else {
            s32* world = flameThrower[i].world;
            pnt = HdPntTest(flameThrower[i].cs->unkC0, 0, world, &hit);
            if (hit <= 0) {
                continue;
            }
            /* The unused clamp keeps the original PsyQ damage calculation. */
            if (m < 0) {
                m = 0;
            }
            bulDispatchDamage(hit, pnt->tag0, pnt->tag1, flameDamage, d, 0);
            flameThrower[i].life = 0;
            do_smoke(world);
        }
    }
}

void carResetGunHeat(PlayerCar* car)
{
    car->weap.gunOverheat = 0;
    car->weap.gunHeat = 0;
}

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
            uaswSetNumChildren(pickup[i].obj, i + 1, pickup[i].kind = 1);
        }
    }
}

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
        pad = &PLAYER_CAR(car)->weap;
        m = &PLAYER_CAR(car)->motion;
    } else {
        pad = &AI_CAR(car)->weap;
        m = &AI_CAR(car)->motion;
    }
    cap = (s16)pad->maxAmmo - (s16)pad->totalAmmo;
    if (rtIsSplitScreenOn() != 0) {
        cap = 30;
    }
    if (cap > 0 || kind == UA_UNK_430 || isPlayer == 0) {
        switch (kind) {
        case UA_PICKUP_FIRE_MISSILE:
            amt = weaponPickupAmount[WEAPON_FIRE_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_FIRE_MISSILE] = pad->ammo[WEAPON_FIRE_MISSILE] + amt;
            break;
        case UA_PICKUP_FREEZE_MISSILE:
            amt = weaponPickupAmount[WEAPON_FREEZE_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_FREEZE_MISSILE] = pad->ammo[WEAPON_FREEZE_MISSILE] + amt;
            break;
        case UA_PICKUP_HOMING_MISSILE:
            amt = weaponPickupAmount[WEAPON_HOMING_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_HOMING_MISSILE] = pad->ammo[WEAPON_HOMING_MISSILE] + amt;
            break;
        case UA_PICKUP_POWER_MISSILE:
            amt = weaponPickupAmount[WEAPON_POWER_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_POWER_MISSILE] = pad->ammo[WEAPON_POWER_MISSILE] + amt;
            break;
        case UA_PICKUP_REAR_MISSILE:
            amt = weaponPickupAmount[WEAPON_REAR_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_REAR_MISSILE] = pad->ammo[WEAPON_REAR_MISSILE] + amt;
            break;
        case UA_PICKUP_SINGING_MISSILE:
            amt = weaponPickupAmount[WEAPON_SINGING_MISSILE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_SINGING_MISSILE] = pad->ammo[WEAPON_SINGING_MISSILE] + amt;
            break;
        case UA_PICKUP_SPECIAL:
            pad->ammo[WEAPON_SPECIAL] = 20;
            break;
        case UA_PICKUP_BOMB:
            amt = weaponPickupAmount[WEAPON_BOMB];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_BOMB] = pad->ammo[WEAPON_BOMB] + amt;
            break;
        case UA_PICKUP_OIL:
            amt = weaponPickupAmount[WEAPON_OIL];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_OIL] = pad->ammo[WEAPON_OIL] + amt;
            break;
        case UA_PICKUP_SPIKE:
            amt = weaponPickupAmount[WEAPON_SPIKE];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_SPIKE] = pad->ammo[WEAPON_SPIKE] + amt;
            break;
        case UA_PICKUP_CATAPULT:
            amt = weaponPickupAmount[WEAPON_CATAPULT];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_CATAPULT] = pad->ammo[WEAPON_CATAPULT] + amt;
            break;
        case UA_PICKUP_FLAMETHROWER:
            amt = weaponPickupAmount[WEAPON_FLAMETHROWER];
            if (cap < amt) {
                amt = cap;
            }
            pad->ammo[WEAPON_FLAMETHROWER] = pad->ammo[WEAPON_FLAMETHROWER] + amt;
            break;
        case UA_UNK_431:
            pad->unk44 = (u32)pad->unk44 + (u32)weaponPickupAmount[13];
            break;
        case UA_UNK_430:
            pad->unk46 = (u32)pad->unk46 + (u32)weaponPickupAmount[12];
            break;
        }
        soundSetRangeAndXPositionFromWorldLoc(&m->pos.vx);
        range = soundGetCalculatedSoundRange();
        uasoundPlayCarWeaponPickup(kind, range, soundGetCalculatedSoundXPosition());
        uaswSetNumChildren(kind, idx + 1, 0);
        pickup[idx].kind = 0;
        pad->totalAmmo = 0;
        if (rtIsSplitScreenOn() == 0) {
            for (i = 0; i < WEAPON_REGULAR_MAX; i++) {
                pad->totalAmmo = pad->totalAmmo + pad->ammo[i];
            }
        }
        if ((s16)pad->totalAmmo == (s16)pad->maxAmmo && isPlayer && kind != UA_UNK_430) {
            uadashMaxCarryCapacity();
        }
    } else {
        uadashMaxCarryCapacity();
    }
}

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
        uaswSetState(UA_HEALTH_STAND, n, 1);
    } else {
        uaswSetState(UA_HEALTH_STAND, n, 0);
    }
}

void deactivate_health_stand(s32 n)
{
    if ((u32)n - 1u < 10u) {
        HStand[n - 1].active = 0;
        uaswSetState(UA_HEALTH_STAND, n, 1);
        numHealthStands = (s32)((u32)numHealthStands - 1u);
    }
}

void reactivate_healthstand(s32 n)
{
    if ((u32)n - 1u < 10u) {
        HStand[n - 1].active = 1;
        uaswSetState(UA_HEALTH_STAND, n, 0);
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

void set_health_stand(s32 n, s32* pos)
{
    s32* dst;
    s32 first;

    if ((u32)n - 1u < 10u) {
        // Keep activation separate from the position copy for PsyQ scheduling.
        do {
            HStand[n - 1].active = 1;
        } while (0);
        first = pos[0];
        dst = HStand[n - 1].pos;
        dst[0] = first;
        dst[1] = pos[1];
        dst[2] = pos[2];
        nextHealthStand = (s32)((u32)nextHealthStand + 1u);
        numHealthStands = (s32)((u32)numHealthStands + 1u);
    }
}

void regen_healthstands(void)
{
    PlayerCar* pi;
    s32 d[3];
    s32 i;
    s32 t;

    pi = GetPlayerInfo(0);
    i = 0;
    do {
        for (; i < nextHealthStand; i++) {
            if (HStand[i].active != 0) {
                d[0] = (s32)((u32)HStand[i].pos[0] - (u32)pi->motion.pos.vx);
                d[1] = (s32)((u32)HStand[i].pos[1] - (u32)pi->motion.pos.vy);
                d[2] = (s32)((u32)HStand[i].pos[2] - (u32)pi->motion.pos.vz);
                hudAddRadarSig(UA_HEALTH_STAND, d, pi->motion.rot.vz);
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

s32 carweapGetMonsterDamage(void)
{
    return monsterDamage;
}
