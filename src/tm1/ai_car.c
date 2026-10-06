#include "common.h"

#include <libgte.h>
#include <rand.h>

#include "tm1/ai_car_init.h"
#include "tm1/ai_car_update.h"
#include "tm1/car.h"
#include "tm1/car_init.h"
#include "tm1/car_update.h"
#include "tm1/math.h"
#include "tm1/shell.h"
#include "tm1/smooth.h"
#include "tm1/timer.h"
#include "tm1/trigger_pts.h"
#include "tm1/ua.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sound.h"
#include "tm1/ua_sw.h"

#include "tm1/ai_car.h"

s16 numAICarsInBattle = 0;
static s16 aiCarsInBattle[2];

#ifdef NON_MATCHING
void AICarUpdate(AICar* car)
{
    s32 diff;
    s32 rot;
    s32 maxRot;
    s32 fl;
    s32 sp;

    if (car->stats.unk03 != 0) {
        car->stats.triggerPt = GetClosestTriggerPt(car, 0);
        InitTriggerPoint(car);
        car->stats.unk03 = 0;
    }
    RecomputeCarDeltas(&car->stats);
    if (car->stats.unk18 >= 0) {
        car->stats.unk18 -= GetFieldsLastFrame();
        if (car->stats.unk18 <= 0) {
            car->flags[0x10] = 0;
            UAeffectClearFreezeLight(GetAICs3D(car->playerIdx));
        }
    }
    if (car->flags[0x19] != 0) {
        if (car->flags[0x0A] != 0 || car->flags[0x17] != 0 || car->flags[0x1B] != 0
            || car->flags[0x00] != 0 || shellGetCurrentLevel() == 5) {
            car->flags[0x19] = 0;
            uaswSetCarShadow(car->uaIndex, 0);
        }
    } else if (car->flags[0x0A] == 0 && car->flags[0x17] == 0 && car->flags[0x1B] == 0
        && car->flags[0x00] == 0 && shellGetCurrentLevel() != 5) {
        car->flags[0x19] = 1;
        uaswSetCarShadow(car->uaIndex, 1);
    }
    car->collision.unk12 = 0;
    car->flags[0x09] = 0;
    AICarUpdateHealthTier(car);
    if (car->driving != 0 && car->flags[0x00] == 0) {
        UpdateBearing(car, 0);
        AICarUpdateClosestPlayer(car);
        AICarUpdatePlayerRange(car);
        AICarUpdateCurrPtRange(car);
        AICarUpdatePlayerDir(car);
        AICarUpdateDrivingProfile(car);
        AICarUpdateAttackProfile(car);
    } else if (car->flags[0x00] != 0) {
        car->unk34 = 0;
    }
    AICarUpdateTransition(car);
    UAeffectUpdateCarSpeed(car->uaIndex, car->motion.vel.vy);
    if (car->driving != 0) {
        CarRotUpdate(car, 0);
        CarTransUpdate(car, 0);
    }
    if (car->stats.unk38 != 0 && (car->unk40 != 0 || shellGetCurrentLevel() == 5)) {
        CheckHitDetection(car, 0);
        if (car->collision.unk0 != 0) {
            diff = __builtin_abs(car->motion.rot.vz - car->motion.rot2.vz);
            if (diff >= 683) {
                car->unk4B = 1;
            }
        }
    }
    if (car->stats.unk34 != 0) {
        s32 idx = car->uaIndex;
        AICarInit(car, idx, 1);
        car->stats.unk34 = 0;
    }
    fl = GetFieldsLastFrame();
    sp = car->motion.vel.vy;
    if ((((fl * 0x2F8) / 100) << 5) < sp && car->flags[0x0D] == 0) {
        rot = car->unk1C4;
        rot = __builtin_abs(rot);
        if (rot < 683 || car->unk160 == 0) {
            maxRot = __builtin_abs(car->motion.rotDelta.vz);
            if (CalcMaxRotBeforeDrift(car, 0) < maxRot) {
                goto block_46;
            }
            goto block_47;
        }
    block_46:
        car->flags[0x02] = 1;
        return;
    }
block_47:
    car->flags[0x02] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdate);
#endif

#ifdef NON_MATCHING
void AICarUpdateHealthTier(AICar* car)
{
    if (car->stats.unk40 < car->unk104) {
        car->unk02 = 0;
    } else if (car->stats.unk40 < car->unk105) {
        car->unk02 = 1;
    } else if (car->stats.unk40 < car->unk106) {
        car->unk02 = 2;
    } else {
        car->unk02 = 3;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateHealthTier);
#endif

void AICarUpdateClosestPlayer(AICar* car)
{
    car->unk0E = GetClosestPlayer(&car->motion.pos, -1);
}

#ifdef NON_MATCHING
void AICarUpdatePlayerRange(AICar* car)
{
    VECTOR pos;
    u16 dead0;
    u16 dead1;
    PlayerCar* info;

    GetPlayerPosition(car->unk0E, (VECTOR3*)&pos);
    pos.vx -= car->motion.pos.vx;
    pos.vy -= car->motion.pos.vy;
    car->unk34 = SquareRoot0((pos.vx * pos.vx) + (pos.vy * pos.vy));
    info = GetPlayerInfo(car->unk0E);
    dead0 = car->unk172;
    dead1 = info->unkC0;

    if (car->unk4C == 0) {
        if (car->unk34 > car->unk38) {
            car->unk178 += GetFieldsLastFrame();
            car->unk17A = 0;
        } else if (car->unk34 < car->unk38) {
            car->unk178 = 0;
            car->unk17A += GetFieldsLastFrame();
        } else {
            car->unk178 = 0;
            car->unk17A = 0;
        }
    }
    if (car->flags[0x0B] == 0) {
        car->unk38 = car->unk34;
    }
    if (car->unk34 >= 6401) {
        car->unk40 = 0;
        if (car->heliFlag == 0) {
            AICarOutOfBattle(car);
        }
    } else {
        car->unk40 = 1;
        if (car->unk41 == 0) {
            AICarInBattle(car);
        }
        if (car->unk34 < car->unk18) {
            car->unk42 = 1;
        } else {
            car->unk42 = 0;
        }
        if (car->unk34 < car->unk1C) {
            car->unk43 = 1;
            car->unk44 = 1;
            car->unk45 = 1;
            car->unk46 = 1;
        } else {
            car->unk43 = 0;
            if (car->unk34 < car->unk20) {
                car->unk44 = 1;
                car->unk45 = 1;
                car->unk46 = 1;
            } else {
                car->unk44 = 0;
                if (car->unk34 < car->unk24) {
                    car->unk45 = 1;
                    car->unk46 = 1;
                } else {
                    car->unk45 = 0;
                    if (car->unk34 < car->unk28) {
                        if (car->unk46 == 0) {
                            car->flags[0x18] = 1;
                        }
                        car->unk46 = 1;
                    } else {
                        car->unk46 = 0;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdatePlayerRange);
#endif

#ifdef NON_MATCHING
void AICarUpdateCurrPtRange(AICar* car)
{
    VECTOR v;
    VECTOR out;
    VECTOR delta;
    u16 distance;

    distance = car->unk186;
    v.vy = 0;
    v.vz = 0;
    v.vx = (s16)distance;
    mathMulTransVec(&car->motion.mat2, &v, &out);
    delta.vx = (car->unk1A0 + out.vx) - car->motion.pos.vx;
    delta.vy = (car->unk1A4 + out.vy) - car->motion.pos.vy;
    car->unk1E4 = SquareRoot0((delta.vx * delta.vx) + (delta.vy * delta.vy));
    if (delta.vy == 0) {
        delta.vy = 1;
    }
    car->unk1EC = ratan2(delta.vx, delta.vy);
    BoundAngle(&car->unk1EC);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateCurrPtRange);
#endif

#ifdef NON_MATCHING
void AICarUpdateNextPtAngle(AICar* car)
{
    VECTOR delta;

    delta.vx = car->unk1A8 - car->motion.pos.vx;
    delta.vy = car->unk1AC - car->motion.pos.vy;
    car->unk1E8 = SquareRoot0((delta.vx * delta.vx) + (delta.vy * delta.vy));
    if (delta.vy == 0) {
        delta.vy = 1;
    }
    car->unk1C8 = ratan2(delta.vx, delta.vy);
    BoundAngle(&car->unk1C8);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateNextPtAngle);
#endif

#ifdef NON_MATCHING
void AICarUpdateDrivingProfile(AICar* car)
{
    s16 r;
    s32 dur;

    if (car->routeVal > 0) {
        car->routeVal = car->routeVal - GetFieldsLastFrame();
        return;
    }
    if (car->unk41 != 0 || numAICarsInBattle < 2) {
        if (car->chosen != 0) {
            if (car->unk01 == 0 && car->unk46 != 0) {
                car->chosen = 0;
            }
            car->unk01 = 0;
            dur = car->route[(s8)car->unk02].val;
        } else {
            r = rand() % 100;
            if (r < car->route[(s8)car->unk02].prio) {
                car->unk01 = 0;
                dur = car->route[(s8)car->unk02].val;
            } else if (r < car->route[(s8)car->unk02].unk04) {
                car->unk01 = 1;
                dur = car->route[(s8)car->unk02].unk10;
            } else {
                goto tier3;
            }
        }
    } else {
    tier3:
        car->unk01 = 2;
        dur = car->route[(s8)car->unk02].unk14;
    }
    car->routeVal = dur;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateDrivingProfile);
#endif

#ifdef NON_MATCHING
void AICarInitTransition(AICar* car)
{
    s32 d;

    car->flags[0x1D] = 0;
    if (car->unk180 != 0) {
        car->stats.unk22++;
        if ((s16)car->stats.unk22 >= 7 && car->unk34 >= 3201) {
            car->flags[0x1D] = 1;
        }
    } else {
        car->stats.unk22 = 0;
    }
    if (triggerPtGroups.count >= 2 && car->flags[0x0A] == 0 && car->unk34 >= 3201) {
        d = __builtin_abs(triggerPtGroups.pos[car->unk176].y - car->motion.pos.vz);
        if ((car->stats.unk6C * 2) < d) {
            car->stats.unk03 = 1;
            if (shellGetCurrentLevel() == 5) {
                if (car->motion.pos.vx >= 0x15E0) {
                    if (car->motion.pos.vx < 0x2581) {
                        if (car->motion.pos.vy >= 0x1770) {
                            if (car->motion.pos.vy < 0x1839) {
                                car->stats.unk03 = 0;
                            }
                        }
                    }
                }
            }
        }
    }
    car->unk180 = 0;
    if (car->unk49 != 0) {
        car->unk49 = 0;
        car->unk10 = 0;
        car->stats.unkFC = car->stats.unk100;
        RecomputeCarDeltas(&car->stats);
    }
    car->unk178 = 0;
    car->unk17A = 0;
    if (car->unk17E >= 601 && car->unk34 >= 3201) {
        car->stats.unk03 = 1;
    }
    car->unk17E = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarInitTransition);
#endif

#ifdef NON_MATCHING
void AICarUpdateTransition(AICar* car)
{
    InitControlPad(car->skid);
    if (car->heliFlag == 0 && car->flags[0x00] == 0) {
        car->unk17E = car->unk17E + GetFieldsLastFrame();
    }
    if (car->unk17E >= 601) {
        StartLostCheck(car);
    }
    if (car->unk49 != 0) {
        HelpLostCar(car);
    }
    if (car->unk16D != 0) {
        BringBackAICarInCollision(car);
    } else if (car->driving != 0 && car->flags[0x00] == 0) {
        if (car->flags[0x0D] != 0) {
            UpdateCarOnSlickSpot(car, 0);
        }
        if (car->unk160 != 0) {
            AICarTurn(car);
        } else {
            AICarDriveBetweenPts(car);
            AICarUpdateSwerve(car);
            if (car->unk41 != 0 && car->unk01 == 0 && car->unk178 > (s16)car->stats.unk1E
                && car->unk144 == 1 && car->unk18A >= 81) {
                AIInitTurnFlags(car);
                GetNextTriggerPoint(car, car->unk170);
                AICarInitTransition(car);
                CalcTurnStart(car, 0);
            }
        }
        AICarUpdateControlPad(car);
        CarUpdateDeltas(car, 0);
    }
    CheckSlickSpots(car, 0);
    CheckHealthStands(car, 0);
    CalcTireCoordinates(car, 0);
    CheckCurbs(car, 0);
    CheckBridges(car, 0);
    CheckCatapults(car, 0);
    CheckBombDamage(car, 0);
    CheckSpikeDamage(car, 0);
    CheckPotHoles(car, 0);
    if (car->flags[0x1B] != 0) {
        CheckMonsterSmash(car, 0);
    }
    UpdateBounce(car, 0);
    UpdateTirePositions(car, 0);
    if (car->flags[0x00] != 0) {
        UpdateCarDeath(car, 0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateTransition);
#endif

#ifdef NON_MATCHING
void NoBeadOnPlayer(AICar* car)
{
    if (car->heliFlag != 0 && car->uaIndex != -1) {
        GetNewPointToDriveTo(car);
        car->heliFlag = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", NoBeadOnPlayer);
#endif

#ifdef NON_MATCHING
void GetNewPointToDriveTo(AICar* car)
{
    s16 pt;
    u16 current;
    s32 point;

    pt = GetClosestTriggerPt(car, 0);
    current = car->unk172;
    do {
    } while (0);
    point = pt;
    if (point == (s16)current) {
        pt = -1;
    }
    GetNextTriggerPoint(car, pt);
    AICarInitTransition(car);
    CalcTurnStart(car, 0);
    do {
        car->unk160 = 1;
    } while (0);
    if (car->uaIndex != -1) {
        car->heliFlag = 0;
    }
    car->unk4B = 0;
    SetNoCarDrift(&car->motion, car->flags);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", GetNewPointToDriveTo);
#endif

#ifdef NON_MATCHING
void HelpLostCar(AICar* car)
{
    car->unk10 += GetFieldsLastFrame();
    if (car->unk10 >= 181 || car->unk180 >= 4 || car->unk4D != 0) {
        GetNewPointToDriveTo(car);
        car->unk4D = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", HelpLostCar);
#endif

#ifdef NON_MATCHING
void AIInitTurnFlags(AICar* car)
{
    s32 angle;

    do {
        car->unk160 = 1;
        angle = 0x800;
    } while (0);
    do {
        car->unk161 = 0;
        car->unk1CC = angle;
    } while (0);
    do {
        car->unk1D0 = 0x7FFF;
        return;
    } while (0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AIInitTurnFlags);
#endif

#ifdef NON_MATCHING
void AICarTurn(AICar* car)
{
    s32 diff;
    s32 t;
    s32 a;
    u8 turn;

    AICarUpdateNextPtAngle(car);
    turn = 0;
    if (GetBeadOnPlayer(car, car->unk1E8)) {
        diff = car->motion.rot.vz - car->unk30;
        car->heliFlag = 1;
        if ((s8)car->stats.unk04 >= GetPlayerInfo(car->unk0E)->stats.unk108
            && (s16)(rand() % 100) < car->attack[(s8)car->unk02].unk00) {
            car->unk48 = 1;
        } else {
            car->unk48 = 0;
        }
    } else {
        diff = car->motion.rot.vz - car->unk1C8;
        NoBeadOnPlayer(car);
    }
    BoundAngle(&diff);
    t = __builtin_abs(diff);
    if (t < 57) {
        turn = 1;
    } else if (t < 342) {
        a = car->unk1CC;
        if (a < 0) {
            a = -a;
        }
        if (a < t) {
            turn = 1;
        } else {
            turn = 0;
            car->unk1CC = diff;
        }
    }
    if (turn != 0) {
        UpdateCurrentTriggerPt(car);
        car->motion.unk00 = 0;
        if (car->unk16C != 0) {
            car->motion.vel.vy = car->stats.unkA4;
        }
        goto noDrift;
    }
    if (!TimeToComeOutOfTurn(car, diff)) {
        if (diff > 0) {
            car->skid[2] = 1;
        } else if (diff < 0) {
            car->skid[3] = 1;
        }
    } else {
        car->skid[3] = 0;
        car->skid[2] = 0;
    }
    if (car->unk16C != 0) {
        SetFullCarDrift(&car->motion);
        if (car->skid[2] != 0) {
            car->skid[7] = 1;
        } else if (car->skid[3] != 0) {
            car->skid[6] = 1;
        }
        goto tail;
    }
noDrift:
    if (car->flags[0x01] == 0) {
        SetNoCarDrift(&car->motion, car->flags);
    }
tail:
    if (car->motion.vel.vy > car->stats.unkB4) {
        car->skid[5] = 1;
        car->skid[4] = 0;
    } else {
        car->skid[4] = 1;
        car->skid[5] = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarTurn);
#endif

#ifdef NON_MATCHING
u8 TimeToComeOutOfTurn(AICar* car, s32 diff)
{
    s32 rot;
    s32 d;
    s32 dd;
    s32 hi;
    s32 lo;
    s32 spd;
    u8 res;

    rot = car->motion.rotDelta.vz;
    d = __builtin_abs(diff);
    if (rot < 0) {
        rot = -rot;
    }
    if (d >= (rot * 2)) {
        if (d >= 1025) {
            res = 0;
        } else if (d >= 57) {
            if (car->skid[6] != 0) {
                hi = car->stats.unkD4 * car->stats.unkE8;
            } else if (car->skid[7] != 0) {
                hi = car->stats.unkD4 * car->stats.unkE8;
            } else {
                hi = car->stats.unkD4 * 2;
            }
            dd = __builtin_abs(diff);
            lo = car->stats.unkD0 + (((hi - car->stats.unkD0) * dd) / 1024);
            if (car->motion.vel.vy < car->stats.unkA8) {
                spd = car->stats.unkA8;
            } else {
                spd = car->motion.vel.vy;
            }
            lo += ((hi - lo) * (spd - car->motion.vel.vy)) / spd;
            res = lo < __builtin_abs((s16)car->motion.unk00);
        } else {
            res = 1;
        }
    } else {
        res = 1;
    }
    if (__builtin_abs((s16)car->motion.unk00) < 5) {
        res = 0;
    }
    return res;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", TimeToComeOutOfTurn);
#endif

#ifdef NON_MATCHING
void AICarDriveBetweenPts(AICar* car)
{
    s32 dist;
    s32 spd;
    s32 t;
    s32 fields;
    s32 half;

    if (car->flags[0x01] == 0) {
        SetNoCarDrift(&car->motion, car->flags);
    }
    if (car->unk200 == 0) {
        UpdateTurnStart(car);
    }
    if (car->unk1E4 < car->unk1B4) {
        if (car->unk200 != 0) {
            GetNextTriggerPoint(car, -1);
            AICarInitTransition(car);
            CalcTurnStart(car, 1);
            car->unk200 = 0;
            return;
        }
        AIInitTurnFlags(car);
        return;
    }
    if (car->unk1E4 < car->unk200) {
        GetNextTriggerPoint(car, -1);
        AICarInitTransition(car);
        CalcTurnStart(car, 1);
        car->unk200 = 0;
    }
    if (car->heliFlag != 0) {
        if (car->unk48 != 0) {
            dist = car->unk34;
            spd = GetBufferZoneSpeed(car);
        } else {
            spd = 0;
            dist = car->unk34 - car->unk18;
        }
        car->unk161 = 0;
    } else {
        dist = car->unk1E4;
        spd = car->stats.unkB4;
    }
    if (dist <= 0) {
        s32 a = car->unk2C;
        a = __builtin_abs(a);
        if (a < 113) {
            goto set53;
        }
        if (car->uaIndex == -1) {
            goto set53;
        }
        car->skid[4] = 1;
        car->heliFlag = 0;
        return;
    }
    t = car->motion.vel.vy;
    t = __builtin_abs(t);
    do {
    } while (0);
    fields = GetFieldsLastFrame();
    if (t < ((fields * 380) / 100) || dist > 0x1FFFFF) {
        goto set52;
    }
    if (car->unk176 != tPoints[car->unk172].type) {
        spd = GetMinSpeedNeeded(tPoints[car->unk170].type, tPoints[car->unk172].type);
        if (car->motion.vel.vy < spd) {
            car->skid[4] = 1;
            if (car->stats.unkA8 < spd) {
                car->skid[8] = 1;
            }
        }
        return;
    }
    half = car->stats.unkBC / 2;
    if ((car->motion.vel.vy - spd) < (((dist << 5) / car->motion.vel.vy) * half)) {
        goto set52;
    }
    goto set53;
set52:
    car->skid[4] = 1;
    return;
set53:
    car->skid[5] = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarDriveBetweenPts);
#endif

#ifdef NON_MATCHING
void UpdateTurnStart(AICar* car)
{
    car->unk1B4 = car->unk1B0;
    if (car->motion.vel.vy < car->stats.unkB4) {
        car->unk1B4 = (car->unk1B0 * car->motion.vel.vy) / car->stats.unkB4;
        if (car->unk1B4 < 160) {
            car->unk1B4 = 160;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", UpdateTurnStart);
#endif

#ifdef NON_MATCHING
void AICarInitSwerve(AICar* car)
{
    VECTOR d;
    s32 v;
    s32 amt;

    car->unk204 = car->bearing;
    CalcDistFromRoadCenter(car, (s32*)&d.vx, 1);
    car->unk161 = 1;
    if (car->unk204 == 0) {
        car->unk188 = d.vx - car->unk186;
        if (car->unk188 <= 0) {
            car->unk162 = 0;
        } else {
            car->unk162 = 1;
        }
    } else if (car->unk204 == 1) {
        car->unk188 = d.vx - car->unk186;
        if (car->unk188 <= 0) {
            car->unk162 = 1;
        } else {
            car->unk162 = 0;
        }
    } else if (car->unk204 == 2) {
        car->unk188 = d.vy - car->unk186;
        if (car->unk188 > 0) {
            car->unk162 = 0;
        } else if (car->unk188 < 0) {
            car->unk162 = 1;
        }
    } else {
        car->unk188 = d.vy - car->unk186;
        if (car->unk188 > 0) {
            car->unk162 = 1;
        } else if (car->unk188 < 0) {
            car->unk162 = 0;
        }
    }
    v = car->unk188;
    if (v >= 241) {
        v = 240;
    } else if (v < -240) {
        v = -240;
    }
    amt = (__builtin_abs(v) * 170) / 240;
    AICarUpdateCurrPtRange(car);
    if (car->unk162 != 0) {
        car->unk1B8 = car->unk1EC - amt;
    } else {
        car->unk1B8 = car->unk1EC + amt;
    }
    BoundAngle(&car->unk1B8);
    car->unk163 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarInitSwerve);
#endif

#ifdef NON_MATCHING
void AICarUpdateSwerve(AICar* car)
{
    VECTOR d;
    s32 v;

    CalcDistFromRoadCenter(car, (s32*)&d.vx, 1);
    if (car->unk163 == 0) {
        if (car->unk204 == 0) {
            if (car->unk162 != 0) {
                if ((d.vx - (car->unk188 / 2)) < 0) {
                    car->unk163 = 1;
                }
            } else if ((d.vx - (car->unk188 / 2)) > 0) {
                car->unk163 = 1;
            }
        } else if (car->unk204 == 1) {
            if (car->unk162 != 0) {
                if ((d.vx - (car->unk188 / 2)) > 0) {
                    car->unk163 = 1;
                }
            } else if ((d.vx - (car->unk188 / 2)) < 0) {
                car->unk163 = 1;
            }
        } else if (car->unk204 == 2) {
            if (car->unk162 == 0) {
                if ((d.vy - (car->unk188 / 2)) < 0) {
                    car->unk163 = 1;
                }
            } else if ((d.vy - (car->unk188 / 2)) > 0) {
                car->unk163 = 1;
            }
        } else {
            if (car->unk162 != 0) {
                if ((d.vy - (car->unk188 / 2)) < 0) {
                    car->unk163 = 1;
                }
            } else if ((d.vy - (car->unk188 / 2)) > 0) {
                car->unk163 = 1;
            }
        }
        if (car->unk163 != 0) {
            car->unk1B8 = car->unk1BC;
        }
    } else {
        v = __builtin_abs(car->motion.rot.vz - car->unk1BC);
        if (v < 22) {
            car->unk161 = 0;
        }
    }
    car->unk163 = 1;
    car->unk161 = 0;
    if (car->heliFlag != 0) {
        if (GetBeadOnPlayer(car, car->unk1E4)) {
            car->motion.rot.vz = SmoothAngleValue(car->motion.rot.vz, car->unk30, 0x5A);
        } else {
            car->heliFlag = 0;
        }
    } else if (car->unk161 != 0) {
        car->motion.rot.vz = SmoothAngleValue(car->motion.rot.vz, car->unk1B8, 0x5A);
    } else {
        car->motion.rot.vz = SmoothAngleValue(car->motion.rot.vz, car->unk1EC, 0x5A);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateSwerve);
#endif

#ifdef NON_MATCHING
u8 GetBeadOnPlayer(AICar* car, s32 range)
{
    PlayerCar* info;
    s16 pt;
    s32 d;
    u8 result;
    u16 ppt;

    pt = car->unk176;
    info = GetPlayerInfo(car->unk0E);
    ppt = info->unkC0;
    d = __builtin_abs(car->motion.pos.vz - info->motion.pos.vz);
    result = 0;
    if (pt == (s16)ppt) {
        if (car->unk18A >= 81 && (s16)car->stats.unk22 < 6 && d < 200) {
            if (uaUsingLanes()) {
                if (car->unk01 == 0) {
                    result = car->unk34 < range;
                }
            } else if (car->unk01 == 0 && car->unk34 < range * 2) {
                result = 1;
            }
        }
    }
    if (car->uaIndex == -1) {
        result = 1;
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", GetBeadOnPlayer);
#endif

#ifdef NON_MATCHING
s32 GetBufferZoneSpeed(AICar* car)
{
    PlayerCar* info;
    s32 diff;
    s32 t;
    s32 spd;

    info = GetPlayerInfo(car->unk0E);
    diff = car->motion.rot.vz - info->motion.rot.vz;
    BoundAngle(&diff);
    if (car->unk48 == 0) {
        t = diff;
        if (t < 0) {
            t = -t;
        }
        if (t < 1024 && info->motion.vel.vy > 0) {
            return info->motion.vel.vy;
        }
        if (info->motion.vel.vy >= 0) {
            spd = __builtin_abs(info->motion.vel.vy);
            if (spd < ((GetFieldsLastFrame() * 190) / 100)) {
                return (GetFieldsLastFrame() * 190) / 100;
            }
        }
    }
    return car->stats.unkB4;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", GetBufferZoneSpeed);
#endif

#ifdef NON_MATCHING
void AICarUpdateControlPad(AICar* car)
{
    car->skid[3] = car->skid[3] != 0 && car->flags[0x0B] == 0;
    car->skid[2] = car->skid[2] != 0 && car->flags[0x0B] == 0;
    car->skid[5] = car->skid[5] != 0 || car->flags[0x10] != 0;
    car->skid[4] = car->skid[4] != 0 && car->flags[0x10] == 0 && car->flags[0x17] == 0;
    car->skid[1] = car->skid[1] != 0 && car->flags[0x10] == 0 && car->flags[0x17] == 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdateControlPad);
#endif

#ifdef NON_MATCHING
void AICarOutOfBattle(AICar* car)
{
    s16 i;
    s16 j;

    for (i = 0; i < numAICarsInBattle; i++) {
        if (aiCarsInBattle[i] == (s16)car->playerIdx) {
            for (j = 0; j < numAICarsInBattle; j++) {
                aiCarsInBattle[i] = aiCarsInBattle[j];
            }
            numAICarsInBattle--;
            break;
        }
    }
    car->unk41 = 0;
    UpdateAICarsInBattleLaneDist();
    uasoundStopCarSounds(car->uaIndex);
    car->unk42 = 0;
    car->unk43 = 0;
    car->unk44 = 0;
    car->unk45 = 0;
    car->unk46 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarOutOfBattle);
#endif

#ifdef NON_MATCHING
void AICarInBattle(AICar* car)
{
    s16 i;
    u8 found;
    u16 id;

    found = 0;
    if (numAICarsInBattle < 2) {
        for (i = 0; i < numAICarsInBattle; i++) {
            if (aiCarsInBattle[i] == (s16)car->playerIdx) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            id = car->playerIdx;
            aiCarsInBattle[numAICarsInBattle] = id;
            numAICarsInBattle++;
            car->unk41 = 1;
            UpdateAICarsInBattleLaneDist();
        }
    } else {
        AICarOutOfBattle(car);
        if (car->unk01 == 1 && car->unk144 == 0) {
            AIInitTurnFlags(car);
            GetNextTriggerPoint(car, car->unk170);
            AICarInitTransition(car);
            CalcTurnStart(car, 0);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarInBattle);
#endif

#ifdef NON_MATCHING
void UpdateAICarsInBattleLaneDist(void)
{
    s16 i;
    s16 t;
    s32 n;
    AICar* car;

    i = 0;
    if (numAICarsInBattle > 0) {
        do {
            car = GetAICarInfo(aiCarsInBattle[i]);
            if (uaUsingLanes()) {
                if (i == 0) {
                    if (car->unk18A >= 121) {
                        car->unk184 = 56;
                        car->unk16B = 1;
                    } else {
                        car->unk184 = 0;
                        car->unk16B = 0;
                    }
                    car->unk16A = 0;
                } else if (i == 1) {
                    if (car->unk18A >= 121) {
                        car->unk184 = -56;
                        car->unk16A = 1;
                    } else {
                        car->unk184 = 0;
                        car->unk16A = 0;
                    }
                    car->unk16B = 0;
                } else {
                    car->unk184 = 0;
                    car->unk16B = 0;
                    car->unk16A = 0;
                }
            } else {
                car->unk184 = 0;
            }
            t = i + 1;
            do {
            } while (0);
            n = numAICarsInBattle;
            i = t;
        } while (t < n);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", UpdateAICarsInBattleLaneDist);
#endif

#ifdef NON_MATCHING
void AICarUpdatePlayerDir(AICar* car)
{
    PlayerCar* info;
    VECTOR delta;
    s32 ang;

    info = GetPlayerInfo(car->unk0E);
    if (car->flags[0x0B] == 0) {
        delta.vx = info->motion.pos.vx - car->motion.pos.vx;
        delta.vy = info->motion.pos.vy - car->motion.pos.vy;
        if (delta.vy == 0) {
            delta.vy = 1;
        }
        car->unk30 = ratan2(delta.vx, delta.vy);
        BoundAngle(&car->unk30);
    }
    car->unk2C = car->unk30 - car->motion.rot.vz;
    BoundAngle(&car->unk2C);
    ang = car->unk2C;
    if (ang < 0) {
        ang = -ang;
    }
    if (ang >= 1537) {
        car->unk144 = 1;
    } else if (car->unk2C >= 513) {
        car->unk144 = 3;
    } else if (car->unk2C < -512) {
        car->unk144 = 2;
    } else {
        car->unk144 = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarUpdatePlayerDir);
#endif

#ifdef NON_MATCHING
void BringBackAICarInCollision(AICar* car)
{
    u8 done;

    done = 0;
    if (car->flags[0x00] != 0) {
        car->unk182 = 0x50;
        car->unk4D = 0;
    }
    if (car->unk4D != 0) {
        if (car->flags[0x10] == 0) {
            car->unk182 = car->unk182 + GetFieldsLastFrame();
            SetNoCarDrift(&car->motion, car->flags);
            car->motion.vel.vy = SmoothValue(
                car->motion.vel.vy, ((-(GetFieldsLastFrame() * 760)) / 100) << 5, 0x5F);
            AICarUpdateNextPtAngle(car);
            car->motion.rot.vz = SmoothAngleValue(car->motion.rot.vz, car->unk1C8, 0x5F);
            if (car->unk182 >= 81) {
                car->unk16D = 0;
                car->unk4D = 0;
                car->unk182 = 0;
                car->motion.vel.vy = 0;
            }
        }
    } else {
        if (car->stats.unk38 == 0) {
            car->collision.count = car->collision.count - GetFieldsLastFrame();
        }
        if ((s16)car->collision.count > 0 && car->motion.vel.vy != 0) {
            if (car->motion.vel.vy > 0) {
                car->motion.vel.vy = car->motion.vel.vy - (car->stats.unkBC / 2);
                if (car->motion.vel.vy < 0) {
                    car->motion.vel.vy = 0;
                }
            } else {
                car->motion.vel.vy = (car->stats.unkBC / 2) + car->motion.vel.vy;
                if (car->motion.vel.vy > 0) {
                    car->motion.vel.vy = 0;
                }
            }
        } else {
            done = 1;
        }
        if (done != 0) {
            SetNoCarDrift(&car->motion, car->flags);
            InitNoCollision(&car->collision);
            if (car->unk4B != 0) {
                car->unk4D = 1;
            } else {
                car->unk4D = 0;
                car->unk16D = 0;
            }
            GetNewPointToDriveTo(car);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", BringBackAICarInCollision);
#endif

#ifdef NON_MATCHING
void AICarHeliUpdate(AICar* car)
{
    PlayerCar* info;
    VECTOR v;
    VECTOR out;
    VECTOR w;
    s32 speed;
    s32 t;
    s32 ang;

    info = GetPlayerInfo(car->unk0E);
    AICarUpdateClosestPlayer(car);
    AICarUpdatePlayerDir(car);
    InitControlPad(car->skid);
    RecomputeCarDeltas(&car->stats);
    if (car->unk160 != 0) {
        AICarTurn(car);
    } else {
        car->unk161 = 0;
        SetNoCarDrift(&car->motion, car->flags);
        v.vx = 0;
        v.vy = -600;
        v.vz = 0;
        mathMulTransVec(&info->motion.mat2, &v, &out);
        w.vx = out.vx + info->motion.pos.vx;
        w.vy = out.vy + info->motion.pos.vy;
        v.vx = w.vx - car->motion.pos.vx;
        v.vy = w.vy - car->motion.pos.vy;
        car->unk34 = SquareRoot0((v.vx * v.vx) + (v.vy * v.vy));
        speed = 0;
        if (car->unk34 >= 800) {
            if (car->unk34 < 1600) {
                s32 t1 = info->motion.vel.vy;
                t1 = __builtin_abs(t1);
                if (t1 < car->stats.unkAC) {
                    speed = car->stats.unkAC;
                } else {
                    speed = __builtin_abs(info->motion.vel.vy);
                }
            } else {
                s32 t2 = info->motion.vel.vy;
                t2 = __builtin_abs(t2);
                if ((t2 * 2) < car->stats.unkAC) {
                    speed = car->stats.unkAC;
                } else {
                    s32 t3 = info->motion.vel.vy;
                    t3 = __builtin_abs(t3);
                    speed = t3 * 2;
                }
            }
        }
        if (speed > 0) {
            s32 cur = __builtin_abs(car->motion.vel.vy);
            if (cur < ((GetFieldsLastFrame() * 380) / 100) || car->unk34 > 0x1FFFFF
                || (car->motion.vel.vy - speed)
                    < (((car->unk34 << 5) / car->motion.vel.vy) * (car->stats.unkBC / 10))) {
                car->skid[4] = 1;
            } else {
                car->skid[5] = 1;
            }
        } else {
            car->skid[5] = 1;
        }
        t = car->unk2C;
        t = __builtin_abs(t);
        if (t >= 1537) {
            if (car->motion.vel.vy > car->stats.unkAC) {
                car->skid[5] = 1;
                car->skid[4] = 0;
            }
            if (car->unk2C > 0) {
                ang = car->motion.rot.vz + 341;
            } else {
                ang = car->motion.rot.vz - 227;
            }
        } else {
            ang = SmoothAngleValue(car->motion.rot.vz, car->unk30, 0x5F);
        }
        car->motion.rot.vz = ang;
    }
    CarUpdateDeltas(car, 0);
    CarRotUpdate(car, 0);
    CarTransUpdate(car, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", AICarHeliUpdate);
#endif

#ifdef NON_MATCHING
void UpdateNumAICarsInBattle(void)
{
    s16 i;
    s16 t;
    s32 n;
    AICar* car;

    i = 0;
    if (numAICarsInBattle > 0) {
        do {
            car = GetAICarInfo(aiCarsInBattle[i]);
            if (car->stats.unk40 <= 0) {
                AICarOutOfBattle(car);
                return;
            }
            t = i + 1;
            do {
            } while (0);
            n = numAICarsInBattle;
            i = t;
        } while (t < n);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", UpdateNumAICarsInBattle);
#endif

s16 GetNumAICarsInBattle(void)
{
    return numAICarsInBattle;
}

#ifdef NON_MATCHING
void StartLostCheck(AICar* car)
{
    car->stats.unkFC = car->stats.unk100 / 2;
    RecomputeCarDeltas(&car->stats);
    car->unk49 = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", StartLostCheck);
#endif

Car* GetClosestCarToPlayer(s16 player, u8* isPlayer)
{
    VECTOR3* pos;
    Car* car;

    pos = &GetPlayerInfo(player)->motion.pos;
    if (GetNumPlayers() == 1) {
        *isPlayer = 0;
        car = GetAICarInfo(GetClosestAICar(pos));
        if (car == NULL || AI_CAR(car)->unk41 != 0) {
            return car;
        }
        car = NULL;
    } else {
        *isPlayer = 1;
        car = GetPlayerInfo(GetClosestPlayer(pos, player));
    }
    return car;
}

#ifdef NON_MATCHING
void InitControlPad(u8* pad)
{
    pad[0x0] = 0;
    pad[0x1] = 0;
    pad[0x3] = 0;
    pad[0x2] = 0;
    pad[0x5] = 0;
    pad[0x4] = 0;
    pad[0x6] = 0;
    pad[0x7] = 0;
    pad[0x8] = 0;
    pad[0x9] = 0;
    pad[0xA] = 0;
    pad[0xB] = 0;
    pad[0xC] = 0;
    pad[0xD] = 0;
    pad[0xE] = 0;
    pad[0xF] = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", InitControlPad);
#endif

void TermAICar(void)
{
}

#ifdef NON_MATCHING
void InitAICarsInBattle(void)
{
    s16 i;

    numAICarsInBattle = 0;
    for (i = 0; i < 2; i++) {
        aiCarsInBattle[i] = -1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car", InitAICarsInBattle);
#endif
