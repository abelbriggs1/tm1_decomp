#include "common.h"

#include <libgte.h>

#include "tm1/ai_car.h"
#include "tm1/car.h"
#include "tm1/car_init.h"
#include "tm1/car_update.h"
#include "tm1/cs.h"
#include "tm1/interactives.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/timer.h"
#include "tm1/ua.h"
#include "tm1/ua_dash.h"
#include "tm1/view.h"

extern MATRIX D_80170D94;

#ifdef NON_MATCHING
void CarInit(Car* car, s32 uaIndex, u8 which)
{
    car->uaIndex = uaIndex;
    car->stats.unk00 = 1;
    CarInitDynamics(car);
    uadashClearCarryCapacity();
    if (which) {
        InitWeapons(car, &car->weap, 1);
        car->stats.unkF8 = 0;
    } else {
        car->weap.ammo[11] = 20;
    }
    car->weap.gunHeat = 0;
    car->weap.gunOverheat = 0;
    CarInitVRModes(car);
    car->unkC0 = 0;
    SetBounce(car, 2, 2, 0, 1);
    car->speedDelta = 0;
    InitControlPad(car->skid);
    uaPadInit(car, PadGetConfig(car->playerIdx));
    car->stats.cheatA = 0;
    car->stats.cheatAArmed = 0;
    car->stats.cheatB = 0;
    car->stats.cheatBArmed = 0;
    car->stats.unk01 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", CarInit);
#endif

#ifdef NON_MATCHING
void CarInitDynamics(Car* car)
{
    InitMotionFlags(car->flags);
    car->unk98 = 0;
    car->unk99 = 0;
    car->motion.vel.x = 0;
    car->motion.vel.y = 0;
    car->motion.vel.z = 0;
    car->motion.pos.x = 0;
    car->motion.pos.y = 0;
    car->motion.pos.z = car->stats.unk6C;
    car->motion.rot.x = 0;
    car->motion.rot.y = 0;
    car->motion.rot.z = 0;
    car->stats.unk38 = 1;
    InitNoCollision(&car->collision);
    InitPlayerPositions(car, car->playerIdx == 0);
    car->motion.rot2Delta.x = 0;
    car->motion.rot2Delta.y = 0;
    car->motion.rot2Delta.z = 0;
    car->motion.rot2.x = 0;
    car->motion.rot2.y = 0;
    car->motion.rot2.z = 0;
    car->motion.mat2.m[0][0] = D_80170D94.m[0][0];
    car->motion.mat2.m[0][1] = D_80170D94.m[0][1];
    car->motion.mat2.m[0][2] = D_80170D94.m[0][2];
    car->motion.mat2.m[1][0] = D_80170D94.m[1][0];
    car->motion.mat2.m[1][1] = D_80170D94.m[1][1];
    car->motion.mat2.m[1][2] = D_80170D94.m[1][2];
    car->motion.mat2.m[2][0] = D_80170D94.m[2][0];
    car->motion.mat2.m[2][1] = D_80170D94.m[2][1];
    car->motion.mat2.m[2][2] = D_80170D94.m[2][2];
    car->motion.mat2.t[0] = D_80170D94.t[0];
    car->motion.mat2.t[1] = D_80170D94.t[1];
    car->motion.mat2.t[2] = D_80170D94.t[2];
    car->motion.mat.m[0][0] = D_80170D94.m[0][0];
    car->motion.mat.m[0][1] = D_80170D94.m[0][1];
    car->motion.mat.m[0][2] = D_80170D94.m[0][2];
    car->motion.mat.m[1][0] = D_80170D94.m[1][0];
    car->motion.mat.m[1][1] = D_80170D94.m[1][1];
    car->motion.mat.m[1][2] = D_80170D94.m[1][2];
    car->motion.mat.m[2][0] = D_80170D94.m[2][0];
    car->motion.mat.m[2][1] = D_80170D94.m[2][1];
    car->motion.mat.m[2][2] = D_80170D94.m[2][2];
    car->motion.mat.t[0] = D_80170D94.t[0];
    car->motion.mat.t[1] = D_80170D94.t[1];
    car->motion.mat.t[2] = D_80170D94.t[2];
    CarInitStrength(&car->stats, 100);
    CarInitDeltas(&car->stats, car->uaIndex);
    car->stats.unk40 = car->stats.unk44;
    InitTireInfo(car->tires);
    if (shellGetCurrentLevel() == 5) {
        InitTireGroup(car->tires, 11);
    } else {
        InitTireGroup(car->tires, 0);
    }
    CarInitMotion(car, 1);
    InitHitPoints(car, 1);
    *(s16*)&car->flags[0x1E] = GetClosestTriggerPt(car, 1);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", CarInitDynamics);
#endif

#ifdef NON_MATCHING
void InitMotionFlags(u8* flags)
{
    flags[0] = 0;
    flags[1] = 0;
    flags[2] = 0;
    flags[3] = 0;
    flags[4] = 0;
    flags[6] = 0;
    flags[7] = 0;
    flags[8] = 0;
    flags[9] = 0;
    flags[10] = 0;
    flags[11] = 0;
    flags[12] = 0;
    flags[13] = 0;
    flags[14] = 0;
    flags[15] = 0;
    flags[16] = 0;
    flags[17] = 0;
    flags[18] = 0;
    flags[19] = 0;
    flags[20] = 0;
    flags[21] = 0;
    flags[22] = 0;
    flags[23] = 0;
    flags[24] = 0;
    flags[25] = 1;
    flags[26] = 0;
    flags[27] = 0;
    flags[29] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitMotionFlags);
#endif

#ifdef NON_MATCHING
void InitWeapons(Car* car, CarWeap* weap, u8 which)
{
    s32 id;
    s16 i;

    if (which == 0) {
        id = ((CarAlt*)car)->uaIndex;
    } else {
        id = car->uaIndex;
    }
    weap->gunDelay = 0;
    weap->fireDelay = 0;
    weap->unk04 = 0;
    for (i = 0; i < 14; i++) {
        weap->ammo[i] = 0;
    }
    weap->cur = 11;
    weap->ammo[11] = 20;
    weap->ammo[0] = 2;
    weap->totalAmmo = 2;
    switch (id) {
    case 10:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 20:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 30:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 40:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 50:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 60:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 70:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 80:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 90:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 100:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 110:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 120:
        weap->maxAmmo = 28;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    case 130:
        weap->maxAmmo = 60;
        weap->unk48 = getSpecialWeaponCost(id);
        break;
    default:
        weap->maxAmmo = 10;
        weap->unk48 = 20;
        break;
    }
    weap->unk46 = 2;
    carInitDropWeaps();
    carInitFlamethrowers();
    carInitTasers();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitWeapons);
#endif

#ifdef NON_MATCHING
void InitPlayerPositions(Car* car, u8 isP1)
{
    switch (shellGetCurrentLevel()) {
    case 0:
    case 6:
    default:
        if (isP1) {
            car->motion.pos.x = 0x460;
            car->motion.pos.y = 0x460;
            car->motion.rot.z = 0x200;
            car->motion.pos.z = 0;
        } else {
            car->motion.pos.x = 0xE60;
            car->motion.pos.y = 0xE60;
            car->motion.rot.z = -0x600;
            car->motion.pos.z = 0;
        }
        break;
    case 1:
        if (isP1) {
            car->motion.pos.x = -0x28A0;
            car->motion.pos.y = -0x11D0;
            car->motion.rot.z = 0xC1;
            car->motion.pos.z = 0;
        } else {
            car->motion.pos.x = -0x2580;
            car->motion.pos.y = 0x960;
            car->motion.rot.z = -0x400;
            car->motion.pos.z = 0;
        }
        break;
    case 2:
        if (isP1) {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = 0x1C20;
                car->motion.pos.y = -0x2390;
                car->motion.rot.z = 0x400;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = -0x4110;
                car->motion.pos.y = 0x1F40;
                car->motion.rot.z = 0x800;
                car->motion.pos.z = 0;
            }
        } else {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = 0x2BC0;
                car->motion.pos.y = -0x2390;
                car->motion.rot.z = -0x400;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = -0x40E8;
                car->motion.pos.y = -0x17C0;
                car->motion.rot.z = 0;
                car->motion.pos.z = 0;
            }
        }
        break;
    case 3:
        if (isP1) {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = 0x12C0;
                car->motion.pos.y = -0x12C0;
                car->motion.rot.z = 0;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = 0x2580;
                car->motion.pos.y = 0xC0;
                car->motion.rot.z = -0x400;
                car->motion.pos.z = 0;
            }
        } else {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = -0xB60;
                car->motion.pos.y = -0x960;
                car->motion.rot.z = -0x400;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = 0x12C0;
                car->motion.pos.y = 0xC0;
                car->motion.rot.z = 0x400;
                car->motion.pos.z = 0;
            }
        }
        break;
    case 4:
        if (isP1) {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = 0x1C20;
                car->motion.pos.y = 0x2EE0;
                car->motion.rot.z = 0x400;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = 0x3EA0;
                car->motion.pos.y = 0x1EF0;
                car->motion.rot.z = 0x400;
                car->motion.pos.z = 0;
            }
        } else {
            if (rtIsSplitScreenOn() == 0) {
                car->motion.pos.x = 0x12C0;
                car->motion.pos.y = 0x4B0;
                car->motion.rot.z = 0;
                car->motion.pos.z = 0;
            } else {
                car->motion.pos.x = 0x3DE0;
                car->motion.pos.y = 0x690;
                car->motion.rot.z = 0x400;
                car->motion.pos.z = 0;
            }
        }
        break;
    case 5:
        if (isP1) {
            car->motion.pos.x = 0x3CD8;
            car->motion.pos.y = 0x250;
            car->motion.rot.z = -0x1C7;
        } else {
            car->motion.pos.x = 0x3520;
            car->motion.pos.y = 0x640;
            car->motion.rot.z = 0x6AA;
        }
        car->motion.pos.z = 0x1E50;
        break;
    }
    car->motion.rotDelta.x = 0;
    car->motion.rotDelta.y = 0;
    car->motion.rotDelta.z = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitPlayerPositions);
#endif

#ifdef NON_MATCHING
void InitHitPoints(Car* car, u8 isPlayer)
{
    CarStats* stats;
    Cs* cs;

    if (isPlayer) {
        stats = &car->stats;
        cs = GetPlayerCs3D(car->playerIdx);
    } else {
        stats = &((CarAlt*)car)->stats;
        cs = GetAICs3D(((CarAlt*)car)->playerIdx);
    }
    cs->nhist = 5;
    cs->hist[0].vx = -stats->unk74 / 2;
    cs->hist[0].vy = stats->unk80;
    cs->hist[0].vz = 24 - stats->unk6C;
    cs->hist[1].vx = stats->unk74 / 2;
    cs->hist[1].vy = stats->unk80;
    cs->hist[1].vz = 24 - stats->unk6C;
    cs->hist[2].vx = stats->unk74 / 2;
    cs->hist[2].vy = stats->unk84;
    cs->hist[2].vz = 24 - stats->unk6C;
    cs->hist[3].vx = -stats->unk74 / 2;
    cs->hist[3].vy = stats->unk84;
    cs->hist[3].vz = 24 - stats->unk6C;
    cs->hist[4].vx = 0;
    cs->hist[4].vy = stats->unk80;
    cs->hist[4].vz = 24 - stats->unk6C;
    cs->hist[5].vx = 0;
    cs->hist[5].vy = stats->unk84;
    cs->hist[5].vz = 24 - stats->unk6C;
    cs->hist[6].vx = -stats->unk74 / 2;
    cs->hist[6].vy = 0;
    cs->hist[6].vz = 24 - stats->unk6C;
    cs->hist[7].vx = stats->unk74 / 2;
    cs->hist[7].vy = 0;
    cs->hist[7].vz = 24 - stats->unk6C;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitHitPoints);
#endif

#ifdef NON_MATCHING
void CarInitDeltas(CarStats* stats, s32 uaIndex)
{
    s32 x;
    s32 y;

    stats->unkDC = 25;
    stats->unkE8 = 3;
    stats->unk34 = 0;
    stats->unk3C = 1;
    stats->damageThreshold = 75;
    stats->unk18 = 0;
    stats->spikeTimer = 0;
    stats->unk1C = 0;
    stats->blasts = 0;
    stats->unk22 = 0;
    *(s32*)&stats->lostTimer = 0;
    stats->unk16 = 0;
    stats->unk03 = 0;
    if (stats->unk00 == 0) {
        stats->unk4C = 1;
    }
    stats->monster = 0;
    switch (uaIndex) {
    case 10:
        InitIceCreamTruck(stats);
        break;
    case 30:
        InitSemiTruck(stats);
        break;
    case 20:
        InitTaxiCab(stats);
        break;
    case 40:
        InitMonsterTruck(stats);
        break;
    case 60:
        InitLamborghini(stats);
        break;
    case 50:
        InitPoliceCar(stats);
        break;
    case 90:
        InitDuneBuggy(stats);
        break;
    case 70:
        InitHumvee(stats);
        break;
    case 100:
        InitLowRider(stats);
        break;
    case 120:
        InitMadMax(stats);
        break;
    case 80:
        InitHarley(stats);
        break;
    case 110:
        InitCorvette(stats);
        break;
    case 130:
        InitBossCar(stats);
        break;
    case -1:
    default:
        InitHeliCar(stats);
        break;
    }
    x = stats->unk80;
    y = stats->unk84;
    if (x < 0) {
        x = -x;
    }
    if (y < 0) {
        y = -y;
    }
    stats->unk78 = x + y;
    RecomputeCarDeltas(stats);
    stats->updateRate = 60;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", CarInitDeltas);
#endif

#ifdef NON_MATCHING
void InitIceCreamTruck(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 2;
    stats->unk88 = 0x5A;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x14;
    stats->unk98 = stats->unk88 / 2;
    stats->unkC8 = 2;
    stats->unkE0 = 0x4B;
    stats->unkE4 = 3;
    stats->unk6C = 0x20;
    stats->unk70 = 0x46;
    stats->unk74 = 0x40;
    stats->unk7C = 0x46;
    stats->unk80 = 0x38;
    stats->unk84 = -0x50;
    stats->unk110 = -0x24;
    stats->unk112 = 0x4B;
    stats->unk114 = 8;
    stats->dropPower = 0;
    stats->unk11A = 0x20;
    stats->unk11C = 0x2C;
    stats->unk108 = 2;
    stats->unk04 = 3;
    stats->unk44 = 0x73;
    stats->unk50 = 0x32;
    stats->unk104 = 0x1E;
    stats->unk1E = 0x5A;
    stats->unk5C = 7;
    stats->unk24 = 0x258;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitIceCreamTruck);
#endif

#ifdef NON_MATCHING
void InitSemiTruck(CarStats* stats)
{
    stats->unk9C = 4;
    stats->unkC4 = 5;
    stats->unk88 = 0x5A;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x1B;
    stats->unk98 = stats->unk88 / 2;
    stats->unkC8 = 3;
    stats->unkE0 = 0xF;
    stats->unkE4 = 0xA;
    stats->unkE8 = 2;
    stats->unk6C = 0x20;
    stats->unk70 = 0x37;
    stats->unk74 = 0x40;
    stats->unk7C = 0x6E;
    stats->unk80 = 0x64;
    stats->unk84 = -0x64;
    stats->unk110 = -0x13;
    stats->unk112 = 0x64;
    stats->unk114 = 0xA;
    stats->dropPower = 0;
    stats->unk11A = 0x24;
    stats->unk11C = 0x3A;
    stats->unk10C = 0x960;
    stats->unk108 = 3;
    stats->unk04 = 3;
    stats->unk44 = 0x8C;
    stats->unk50 = 0x32;
    stats->unk104 = 0x5A;
    stats->unk1E = 0x28;
    stats->unk5C = 6;
    stats->unk24 = 0x258;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitSemiTruck);
#endif

#ifdef NON_MATCHING
void InitTaxiCab(CarStats* stats)
{
    stats->unk9C = 0xA;
    stats->unkC4 = 3;
    stats->unk88 = 0x6E;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x1E;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0x14;
    stats->unkE4 = 7;
    stats->unk6C = 0x10;
    stats->unk70 = 0x1E;
    stats->unk74 = 0x30;
    stats->unk7C = 0x4E;
    stats->unk80 = 0x36;
    stats->unk84 = -0x4E;
    stats->unk110 = -0x19;
    stats->unk112 = 0x3D;
    stats->unk114 = 0xB;
    stats->dropPower = 0;
    stats->unk11A = 0;
    stats->unk11C = 0x2A;
    stats->unk10C = 0x230;
    stats->unk108 = 2;
    stats->unk04 = 2;
    stats->unk44 = 0x64;
    stats->unk50 = 0x32;
    stats->unk104 = 0x1E;
    stats->unk1E = 0x28;
    stats->unk5C = 7;
    stats->unk24 = 0x2D0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitTaxiCab);
#endif

#ifdef NON_MATCHING
void InitMonsterTruck(CarStats* stats)
{
    stats->unk9C = 0xA;
    stats->unkC4 = 2;
    stats->unk88 = 0x5A;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x19;
    stats->unk98 = stats->unk88 / 2;
    stats->unkC8 = 2;
    stats->unkE0 = 0x23;
    stats->unkE4 = 4;
    stats->unkE8 = 2;
    stats->unk6C = 0x42;
    stats->unk70 = 0x1B;
    stats->unk74 = 0x56;
    stats->unk7C = 0x52;
    stats->unk80 = 0x40;
    stats->unk84 = -0x50;
    stats->unk110 = 0x23;
    stats->unk112 = 0x1F;
    stats->unk114 = -0x14;
    stats->dropPower = 0;
    stats->unk11A = 4;
    stats->unk11C = 0x27;
    stats->unk10C = 0x640;
    stats->unk108 = 3;
    stats->unk04 = 3;
    stats->unk44 = 0x78;
    stats->unk50 = 0x32;
    stats->unk104 = 0x46;
    stats->unk1E = 0x5A;
    stats->unk5C = 0xA;
    stats->unk24 = 0x258;
    stats->monster = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitMonsterTruck);
#endif

#ifdef NON_MATCHING
void InitLamborghini(CarStats* stats)
{
    stats->unk9C = 0xD;
    stats->unkC4 = 4;
    stats->unk88 = 0x87;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x84;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0x14;
    stats->unkE4 = 6;
    stats->unk6C = 0x10;
    stats->unk70 = 0xF;
    stats->unk74 = 0x36;
    stats->unk7C = 0x48;
    stats->unk80 = 0x38;
    stats->unk84 = -0x3D;
    stats->unk110 = -0x1C;
    stats->unk112 = 0x2E;
    stats->unk114 = 4;
    stats->dropPower = 0;
    stats->unk11A = 4;
    stats->unk11C = 0x1E;
    stats->unk10C = -0x230;
    stats->unk108 = 1;
    stats->unk04 = 1;
    stats->unk44 = 0x64;
    stats->unk50 = 0x32;
    stats->unk104 = 0x14;
    stats->unk1E = 0xF;
    stats->unk5C = 0x14;
    stats->unk24 = 0x370;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitLamborghini);
#endif

#ifdef NON_MATCHING
void InitPoliceCar(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 3;
    stats->unk88 = 0x73;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x67;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0x19;
    stats->unkE4 = 5;
    stats->unk6C = 0xF;
    stats->unk70 = 0x25;
    stats->unk74 = 0x30;
    stats->unk7C = 0x52;
    stats->unk80 = 0x3F;
    stats->unk84 = -0x4B;
    stats->unk110 = -0x1D;
    stats->unk112 = 0x2B;
    stats->unk114 = 0xF;
    stats->dropPower = 0;
    stats->unk11A = 0;
    stats->unk11C = 0x2C;
    stats->unk10C = 0x320;
    stats->unk108 = 2;
    stats->unk04 = 2;
    stats->unk44 = 0x6E;
    stats->unk50 = 0x32;
    stats->unk104 = 0x1E;
    stats->unk1E = 0x28;
    stats->unk5C = 6;
    stats->unk24 = 0x320;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitPoliceCar);
#endif

#ifdef NON_MATCHING
void InitDuneBuggy(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 3;
    stats->unk88 = 0x6E;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x37;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0x1E;
    stats->unkE4 = 8;
    stats->unk6C = 0x14;
    stats->unk70 = 0x19;
    stats->unk74 = 0x40;
    stats->unk7C = 0x3A;
    stats->unk80 = 0x37;
    stats->unk84 = -0x21;
    stats->unk110 = -0xE;
    stats->unk112 = 0x29;
    stats->unk114 = 8;
    stats->dropPower = 0;
    stats->unk11A = 4;
    stats->unk11C = 0x21;
    stats->unk10C = -0x320;
    stats->unk108 = 1;
    stats->unk04 = 1;
    stats->unk44 = 0x64;
    stats->unk50 = 0x32;
    stats->unk104 = 0xA;
    stats->unk1E = 0x3C;
    stats->unk5C = 5;
    stats->unk24 = 0x280;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitDuneBuggy);
#endif

#ifdef NON_MATCHING
void InitHumvee(CarStats* stats)
{
    stats->unk9C = 6;
    stats->unkC4 = 3;
    stats->unk88 = 0x5B;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x6E;
    stats->unk98 = stats->unk88 / 2;
    stats->unkC8 = 2;
    stats->unkE0 = 0xA;
    stats->unkE4 = 4;
    stats->unkE8 = 2;
    stats->unk6C = 0x20;
    stats->unk70 = 0x26;
    stats->unk74 = 0x46;
    stats->unk7C = 0x3F;
    stats->unk80 = 0x47;
    stats->unk84 = -0x4F;
    stats->unk110 = -0x1F;
    stats->unk112 = 0x4A;
    stats->unk114 = 0xA;
    stats->dropPower = 0;
    stats->unk11A = 0xC;
    stats->unk11C = 0x24;
    stats->unk10C = 0x7D0;
    stats->unk108 = 3;
    stats->unk04 = 3;
    stats->unk44 = 0x78;
    stats->unk50 = 0x32;
    stats->unk104 = 0x50;
    stats->unk1E = 0x3C;
    stats->unk5C = 0xF;
    stats->unk24 = 0x230;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitHumvee);
#endif

#ifdef NON_MATCHING
void InitLowRider(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 5;
    stats->unk88 = 0x78;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x55;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0x14;
    stats->unkE4 = 8;
    stats->unk6C = 0xE;
    stats->unk70 = 0x12;
    stats->unk74 = 0x34;
    stats->unk7C = 0x50;
    stats->unk80 = 0x3B;
    stats->unk84 = -0x4B;
    stats->unk110 = -0x1B;
    stats->unk112 = 0x3C;
    stats->unk114 = 8;
    stats->dropPower = 0;
    stats->unk11A = 3;
    stats->unk11C = 0x1E;
    stats->unk10C = 0x320;
    stats->unk108 = 2;
    stats->unk04 = 2;
    stats->unk44 = 0x6E;
    stats->unk50 = 0x32;
    stats->unk104 = 0x28;
    stats->unk1E = 0x6E;
    stats->unk5C = 6;
    stats->unk24 = 0x320;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitLowRider);
#endif

#ifdef NON_MATCHING
void InitMadMax(CarStats* stats)
{
    stats->unk9C = 6;
    stats->unkC4 = 3;
    stats->unk88 = 0x78;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x4B;
    stats->unk98 = stats->unk88 / 2;
    stats->unkC8 = 2;
    stats->unkE0 = 0x14;
    stats->unkE4 = 7;
    stats->unk6C = 0x10;
    stats->unk70 = 0x13;
    stats->unk74 = 0x34;
    stats->unk7C = 0x42;
    stats->unk80 = 0x34;
    stats->unk84 = -0x40;
    stats->unk110 = -0x18;
    stats->unk112 = 0x13;
    stats->unk114 = 4;
    stats->dropPower = 0;
    stats->unk11A = -0xE;
    stats->unk11C = 0x26;
    stats->unk10C = 0x320;
    stats->unk108 = 2;
    stats->unk04 = 2;
    stats->unk44 = 0x6E;
    stats->unk50 = 0x32;
    stats->unk104 = 0x32;
    stats->unk1E = 0x19;
    stats->unk5C = 0xA;
    stats->unk24 = 0x2F8;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitMadMax);
#endif

#ifdef NON_MATCHING
void InitHarley(CarStats* stats)
{
    stats->unk9C = 7;
    stats->unkC4 = 5;
    stats->unk88 = 0x7D;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x6E;
    stats->unk98 = stats->unk88 * 7 / 8;
    stats->unkC8 = 2;
    stats->unkE0 = -0x50;
    stats->unkE4 = 5;
    stats->unk6C = 0x10;
    stats->unk70 = 7;
    stats->unk74 = 0x32;
    stats->unk7C = 0x35;
    stats->unk80 = 0x2A;
    stats->unk84 = -0x1D;
    stats->unk110 = -0x11;
    stats->unk112 = 0x12;
    stats->unk114 = 0xC;
    stats->dropPower = 0;
    stats->unk11A = -0xA;
    stats->unk11C = 0x46;
    stats->unk10C = -0x320;
    stats->unk108 = 1;
    stats->unk04 = 1;
    stats->unk44 = 0x64;
    stats->unk50 = 0x32;
    stats->unk104 = 5;
    stats->unk1E = 0x4B;
    stats->unk5C = 0xF;
    stats->unk24 = 0x2D0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitHarley);
#endif

#ifdef NON_MATCHING
void InitCorvette(CarStats* stats)
{
    stats->unk9C = 0xA;
    stats->unkC4 = 3;
    stats->unk88 = 0x82;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x6E;
    stats->unk98 = stats->unk88 * 3 / 4;
    stats->unkC8 = 2;
    stats->unkE0 = 0xF;
    stats->unkE4 = 3;
    stats->unk6C = 0x10;
    stats->unk70 = 0x11;
    stats->unk74 = 0x34;
    stats->unk7C = 0x42;
    stats->unk80 = 0x30;
    stats->unk84 = -0x3E;
    stats->unk110 = -0x17;
    stats->unk112 = 0x31;
    stats->unk114 = 4;
    stats->dropPower = 0;
    stats->unk11A = -0xE;
    stats->unk11C = 0x1E;
    stats->unk10C = -0x190;
    stats->unk108 = 1;
    stats->unk04 = 1;
    stats->unk44 = 0x64;
    stats->unk50 = 0x32;
    stats->unk104 = 0x14;
    stats->unk1E = 0xF;
    stats->unk5C = 0xD;
    stats->unk24 = 0x348;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitCorvette);
#endif

#ifdef NON_MATCHING
void InitBossCar(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 3;
    stats->unk88 = 0x73;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x96;
    stats->unk98 = stats->unk88 / 3;
    stats->unkC8 = 2;
    stats->unkE0 = 0x14;
    stats->unkE4 = 5;
    stats->unk6C = 0x1B;
    stats->unk70 = 0x42;
    stats->unk74 = 0x6C;
    stats->unk7C = 0x52;
    stats->unk80 = 0x7E;
    stats->unk84 = -0x7E;
    stats->unk110 = -0x12;
    stats->unk112 = 0x69;
    stats->unk114 = 0x1B;
    stats->dropPower = 0;
    stats->unk11A = 0;
    stats->unk11C = 0x32;
    stats->unk10C = 0x320;
    stats->unk108 = 3;
    stats->unk04 = 3;
    stats->unk44 = 0xFA;
    stats->unk50 = 0x32;
    stats->unk104 = 0x46;
    stats->unk1E = 0xA;
    stats->unk5C = 0x14;
    stats->unk24 = 0x348;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitBossCar);
#endif

#ifdef NON_MATCHING
void InitHeliCar(CarStats* stats)
{
    stats->unk9C = 5;
    stats->unkC4 = 5;
    stats->unk88 = 0x12C;
    stats->unk8C = stats->unk88 * 2 / 3;
    stats->unk90 = 0x67;
    stats->unk98 = stats->unk88 / 3;
    stats->unkC8 = 4;
    stats->unkE0 = 0x19;
    stats->unkE4 = 5;
    stats->unkE8 = 4;
    stats->unk6C = 0xF;
    stats->unk70 = 0x25;
    stats->unk74 = 0x30;
    stats->unk7C = 0x52;
    stats->unk80 = 0x3F;
    stats->unk84 = -0x4B;
    stats->unk110 = -0x1D;
    stats->unk112 = 0x2B;
    stats->unk114 = 0xF;
    stats->dropPower = 0;
    stats->unk11A = 0;
    stats->unk11C = 0x2C;
    stats->unk10C = 0x320;
    stats->unk108 = 2;
    stats->unk04 = 2;
    stats->unk44 = 0x6E;
    stats->unk50 = 0x32;
    stats->unk104 = 0x1E;
    stats->unk1E = 0xA;
    stats->unk5C = 0x1A;
    stats->unk24 = 0x320;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", InitHeliCar);
#endif

void CarInitTweeking(void)
{
}

void GetCarName(void)
{
}

#ifdef NON_MATCHING
void CarInitVRModes(Car* car)
{
    car->vrPos[1].x = 0;
    car->vrPos[1].y = -240;
    car->vrPos[1].z = 112;
    car->vrRot[1].vx = 182;
    car->vrRot[1].vy = 0;
    car->vrRot[1].vz = 0;
    car->vrPos[5].x = 0;
    car->vrRot[5].vy = 0;
    car->vrRot[5].vz = 0;
    car->vrRot[5].vx = 68;
    switch (car->uaIndex) {
    case 10:
        car->vrPos[5].y = -240;
        car->vrPos[5].z = 112;
        break;
    case 30:
        car->vrPos[5].y = -256;
        car->vrPos[5].z = 96;
        break;
    case 20:
        car->vrPos[5].y = -208;
        car->vrPos[5].z = 72;
        break;
    case 40:
        car->vrPos[5].y = -200;
        car->vrPos[5].z = 64;
        break;
    case 60:
        car->vrPos[5].y = -152;
        car->vrPos[5].z = 48;
        break;
    case 50:
        car->vrPos[5].y = -192;
        car->vrPos[5].z = 72;
        break;
    case 90:
        car->vrPos[5].y = -136;
        car->vrPos[5].z = 48;
        break;
    case 70:
        car->vrPos[5].y = -152;
        car->vrPos[5].z = 56;
        break;
    case 100:
        car->vrPos[5].y = -208;
        car->vrPos[5].z = 48;
        break;
    case 120:
        car->vrPos[5].y = -176;
        car->vrPos[5].z = 48;
        break;
    case 80:
        car->vrPos[5].y = -240;
        car->vrPos[5].z = 48;
        break;
    case 110:
        car->vrPos[5].y = -168;
        car->vrPos[5].z = 48;
        break;
    case 130:
    default:
        break;
    }
    if (viewGetFov() < 60) {
        car->vrPos[1].y = 140 * car->vrPos[1].y / 100;
        car->vrRot[1].vx = 100 * car->vrRot[1].vx / 140;
    }
    car->vrPos[2].y = 155 * car->vrPos[1].y / 100;
    car->vrPos[2].z = 110 * car->vrPos[1].z / 100;
    car->vrRot[2].vx = 100 * car->vrRot[1].vx / 140;
    car->vrPos[0].x = (s16)car->stats.dropPower;
    car->vrPos[0].y = car->stats.unk11A;
    car->vrPos[0].z = car->stats.unk11C;
    car->vrRot[0].vx = 0;
    car->vrRot[0].vy = 0;
    car->vrRot[0].vz = 0;
    car->vrPos[3].x = 0;
    car->vrPos[3].y = -640;
    car->vrPos[3].z = 320;
    car->vrRot[3].vx = 0;
    car->vrRot[3].vy = 0;
    car->vrRot[3].vz = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", CarInitVRModes);
#endif

#ifdef NON_MATCHING
void RecomputeCarDeltas(CarStats* stats)
{
    s32 rate;
    s32 k;

    stats->unkA0 = stats->unk9C;
    k = stats->unkFC * 6;
    stats->unkA4 = stats->unkA0 * k / 100;
    stats->unkBC = stats->unkA0 * stats->unkC4;
    if (stats->unk00 == 0) {
        stats->unkA4 = stats->unkA4 / 2;
    }
    stats->unkCC = stats->unkC8;
    stats->unkD0 = stats->unkCC * stats->unkE4 * stats->unkFC / 100;
    rate = GetUpdateRate();
    if (rate != 0 && rate != 60) {
        stats->unkA0 = stats->unkA0 * 60 / rate;
        stats->unkA4 = stats->unkA4 * 60 / rate;
        stats->unkBC = stats->unkBC * 60 / rate;
        stats->unkCC = stats->unkCC * 60 / rate;
        stats->unkD0 = stats->unkD0 * 60 / rate;
    }
    stats->unkAC = stats->unkA4 * 3;
    stats->unkEC = stats->unkD0 * 5;
    stats->unkF0 = stats->unkD0 * 2;
    stats->unkF4 = stats->unkE0 * 682 / 100;
    stats->unkD4 = stats->unkCC * stats->unkDC;
    if (stats->unk00 == 0) {
        stats->unkD4 = stats->unkD4 * 5;
    }
    stats->unkD8 = stats->unkD4 / 2;
    k = GetFieldsLastFrame() * 19;
    stats->unkA8 = stats->unk88 * k / 100 * (stats->unkFC * 32) / 100;
    k = GetFieldsLastFrame() * 19;
    stats->unkB8 = stats->unk8C * k / 100 * (stats->unkFC * 32) / 100;
    k = GetFieldsLastFrame() * 19;
    stats->unkB0 = stats->unk90 * k / 100 * (stats->unkFC * 32) / 100;
    k = GetFieldsLastFrame() * 19;
    stats->unkB4 = stats->unk94 * k / 100 * (stats->unkFC * 32) / 100;
    k = GetFieldsLastFrame() * 19;
    stats->unkC0 = stats->unk98 * k / 100 * (stats->unkFC * 32) / 100;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", RecomputeCarDeltas);
#endif

#ifdef NON_MATCHING
void CarInitStrength(CarStats* stats, s32 strength)
{
    stats->unkFC = strength;
    stats->unk100 = stats->unkFC;
    RecomputeCarDeltas(stats);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_init", CarInitStrength);
#endif
