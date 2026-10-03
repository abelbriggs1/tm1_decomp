#include "common.h"

#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/ctlpad.h"
#include "tm1/explode.h"
#include "tm1/explosion.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/potholes.h"
#include "tm1/rt.h"
#include "tm1/smooth.h"
#include "tm1/timer.h"
#include "tm1/ua.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sound.h"
#include "tm1/ua_sw.h"
#include "tm1/view.h"
#include <libgte.h>
#include <rand.h>

#define CAR_LOST_BYTE(car) (((u8*)(car))[0x4D])

extern void StartLostCheck(Car* car);
extern void RecomputeCarDeltas(CarStats* s);
extern void UAdashSetDashboardDrawFlag(s32 on);
extern void* sdk_memcpy();
extern s32 GetClosestTriggerPt(Car* car, u8 which);
extern s32 shellGetCurrentLevel(void);
extern void UpdateCarOnSlickSpot(Car* car, u8 which);
extern void CheckSlickSpots(Car* car, u8 which);
extern void CheckHealthStands(Car* car, u8 which);
extern void CalcTireCoordinates(Car* car, u8 which);
extern void CheckCurbs(Car* car, u8 which);
extern void CheckBridges(Car* car, u8 which);
extern void CheckPotHoles(Car* car, u8 which);
extern void CheckMonsterSmash(Car* car, u8 which);
extern void UpdateTirePositions(Car* car, u8 which);
extern void CarInit(Car* car, s32 uaIndex, s32 which);
extern void HdCsTest(Cs* cs, CarHit* hit, s32 doWorld, void* user1, void* user0);
extern void bulDispatchDamage(s32 owner, s16 a, s16 b, s32 damage, VEC3* rot, s32 extra);
extern s16 MakeFakePotHole(void);
extern void CheckForBridge(CarTire* tire, u8 which, s32 height);
extern void SetFXSheet(s32 rate, u8 r, u8 g, u8 b);

#ifdef NON_MATCHING
void CarUpdate(Car* car)
{
    s32 idx;
    s32 rate;
    s32 speed;
    s32 rot;

    idx = car->playerIdx;
    RecomputeCarDeltas(&car->stats);
    rate = GetUpdateRate();
    if ((s16)car->stats.updateRate != (s16)rate) {
        car->motion.vel.y = car->motion.vel.y * (s16)car->stats.updateRate / (s16)rate;
        car->motion.rotDelta.z = car->motion.rotDelta.z * (s16)car->stats.updateRate / (s16)rate;
        car->stats.updateRate = rate;
    }
    car->collision.unk12 = 0;
    car->flags[0x09] = 0;
    *(s16*)&car->flags[0x1E] = GetClosestTriggerPt(car, 1);
    if (car->flags[0x10]) {
        *(u16*)&car->stats.unk18 -= GetFieldsLastFrame();
        if ((s32)((u32) * (u16*)&car->stats.unk18 << 16) <= 0) {
            car->flags[0x10] = 0;
            UAeffectClearFreezeLight(GetPlayerCs3D(car->playerIdx));
        }
    }
    if (car->flags[0x19]) {
        if (car->flags[0x0A] || car->flags[0x17] || car->flags[0x00]
            || shellGetCurrentLevel() == 5) {
            car->flags[0x19] = 0;
            uaswSetCarShadow(car->uaIndex, 0);
        }
    } else if (!car->flags[0x0A] && !car->flags[0x17] && !car->flags[0x00]
        && shellGetCurrentLevel() != 5) {
        car->flags[0x19] = 1;
        uaswSetCarShadow(car->uaIndex, 1);
    }
    speed = car->motion.vel.y;
    UpdateBearing(car, 1);
    CarUpdateDeltas(car, 1);
    if (car->flags[0x0D]) {
        UpdateCarOnSlickSpot(car, 1);
    } else {
        CarCheckDynamics(car);
    }
    car->speedDelta = car->motion.vel.y - speed;
    CheckSlickSpots(car, 1);
    CheckHealthStands(car, 1);
    CalcTireCoordinates(car, 1);
    CheckCurbs(car, 1);
    CheckBridges(car, 1);
    CheckCatapults(car, 1);
    CheckBombDamage(car, 1);
    CheckSpikeDamage(car, 1);
    CheckPotHoles(car, 1);
    if (car->flags[0x1B]) {
        CheckMonsterSmash(car, 1);
    }
    UpdateBounce(car, 1);
    UpdateTirePositions(car, 1);
    if (car->flags[0x00]) {
        UpdateCarDeath(car, 1);
    }
    UAeffectUpdateCarSpeed(car->uaIndex, car->motion.vel.y);
    CarRotUpdate(car, 1);
    CarTransUpdate(car, 1);
    if (car->stats.unk38) {
        CheckHitDetection(car, 1);
    }
    UpdateWeapons(car, 1);
    if ((s16)idx == (s16)GetPlayerTheCameraFollows() || rtIsSplitScreenOn()) {
        CarViewUpdate(car);
    }
    if ((s16)GetFieldsLastFrame() * 760 / 100 * 32 < car->motion.vel.y) {
        rot = car->motion.rotDelta.z;
        if (rot < 0) {
            rot = -rot;
        }
        if (CalcMaxRotBeforeDrift(car, 1) < rot && !car->flags[0x0D]) {
            car->flags[0x02] = 1;
        } else {
            car->flags[0x02] = 0;
        }
    } else {
        car->flags[0x02] = 0;
    }
    if (car->stats.unk34) {
        CarInit(car, car->uaIndex, 1);
        car->stats.unk34 = 0;
    }
    if (uaPlayerCheating()) {
        if (ctlpadSpecial(2, car->playerIdx)) {
            car->stats.cheatA = 1;
            car->stats.cheatTimer = 0;
        } else if (ctlpadSpecial(4, car->playerIdx)) {
            car->stats.cheatB = 1;
            car->stats.cheatTimer = 0;
        } else if (!uaGetTwoPlayerMode() && ctlpadSpecial(2, 1)) {
            car->stats.cheatA = 1;
            car->stats.cheatTimer = 0;
        }
        if (car->stats.cheatA) {
            car->stats.cheatTimer++;
            if ((s16)car->stats.cheatTimer >= 31) {
                car->stats.cheatA = 0;
                car->stats.cheatTimer = 0;
            }
            if (ctlpadSpecial(3, car->playerIdx)) {
                if (car->stats.cheatAArmed) {
                    if (car->stats.unk4C) {
                        car->stats.unk4C = 0;
                    } else {
                        car->stats.unk4C = 1;
                    }
                    car->stats.cheatAArmed = 0;
                    car->stats.cheatTimer = 0;
                }
            } else if (!uaGetTwoPlayerMode() && ctlpadSpecial(3, 1)) {
                if (car->stats.cheatAArmed) {
                    if (uaDrivingAICars()) {
                        uaDriveAICars(0);
                    } else {
                        uaDriveAICars(1);
                    }
                    car->stats.cheatAArmed = 0;
                    car->stats.cheatTimer = 0;
                }
            } else if (ctlpadSpecial(4, car->playerIdx)) {
                if (car->stats.cheatAArmed) {
                    if (*(s32*)&car->stats.lostTimer) {
                        *(s32*)&car->stats.lostTimer = 0;
                    } else {
                        *(s32*)&car->stats.lostTimer = 1;
                        car->flags[0x1A] = 1;
                    }
                }
                car->stats.cheatAArmed = 0;
                car->stats.cheatTimer = 0;
            } else if (ctlpadSpecial(6, car->playerIdx)) {
                if (car->stats.cheatAArmed) {
                    if (car->weap.reset) {
                        car->weap.reset = 0;
                    } else {
                        car->weap.reset = 1;
                    }
                }
                car->stats.cheatAArmed = 0;
                car->stats.cheatTimer = 0;
            } else {
                car->stats.cheatAArmed = 1;
            }
        }
        if (car->stats.cheatB) {
            car->stats.cheatTimer++;
            if ((s16)car->stats.cheatTimer >= 31) {
                car->stats.cheatB = 0;
                car->stats.cheatTimer = 0;
            }
            if (ctlpadSpecial(5, car->playerIdx)) {
                if (car->stats.cheatBArmed) {
                    if (car->stats.unk104 >= 101) {
                        car->stats.unk104 = 50;
                        car->stats.unk108 = 2;
                    } else {
                        car->stats.unk104 = 200;
                        car->stats.unk108 = 4;
                    }
                    car->stats.cheatBArmed = 0;
                    car->stats.cheatTimer = 0;
                }
            } else {
                car->stats.cheatBArmed = 1;
            }
        }
    } else if (car->unk9A) {
        car->stats.cheatTimer++;
        if ((s16)car->stats.cheatTimer >= 31) {
            car->unk9A = 0;
        } else if (ctlpadSpecial(4, car->playerIdx)) {
            car->unk9B = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarUpdate);
#endif

#ifdef NON_MATCHING

void CarUpdateDeltas(Car* car, u8 which)
{
    u8* input;
    u8* flags;
    CarStats* st;
    CarMotion* m;
    CarWeap* weap;
    CarAlt* alt;
    s32 maxTurn;
    s32 maxSpeed;
    s32 acceleration;
    s32 amount;
    s32 speed;
    s32 threshold;

    if (which) {
        input = car->skid;
        flags = car->flags;
        st = &car->stats;
        m = &car->motion;
        weap = &car->weap;
    } else {
        alt = (CarAlt*)car;
        input = alt->skid;
        flags = alt->flags;
        st = &alt->stats;
        m = &alt->motion;
        weap = &alt->weap;
    }
    maxTurn = input[6] || input[7] ? (st->unkD4 * st->unkE8) : st->unkD4;
    maxSpeed = m->vel.y < st->unkA8 ? st->unkA8 : m->vel.y;
    acceleration = st->unkA4;
    if (flags[16]) {
        input[2] = 0;
        input[3] = 0;
    }
    if (input[3] || input[2]) {
        speed = __builtin_abs(m->vel.y);
        threshold = (s32)((u32)(((s16)GetFieldsLastFrame() * 1520) / 100) << 5);
        if (threshold < speed)
            amount = st->unkCC;
        else {
            threshold = (s32)((u32)(((s16)GetFieldsLastFrame() * 1520) / 100) << 5);
            amount = st->unkD0 - ((m->vel.y * (st->unkD0 - st->unkCC)) / threshold);
        }
        if ((input[3] && (s16)m->unk00 <= 0) || (input[2] && (s16)m->unk00 >= 0))
            amount = amount * 2;
        if (input[6] || input[7])
            amount = amount * st->unkE8;
        if (input[3]) {
            m->unk00 = (u16)((u32)m->unk00 + (u32)amount);
            if (maxTurn < (s16)m->unk00)
                m->unk00 = maxTurn;
        } else {
            m->unk00 = (u16)((u32)m->unk00 - (u32)amount);
            if ((s16)m->unk00 < -maxTurn)
                m->unk00 = -maxTurn;
        }
    } else if (m->unk00) {
        amount = (s16)m->unk00;
        if (__builtin_abs(amount) < st->unkD0)
            m->unk00 = 0;
        else if (flags[11])
            m->unk00 = (u16)((u32)m->unk00 - (u32)(amount / 16));
        else
            m->unk00 = amount / 2;
    }
    m->rotDelta.z = ((m->vel.y / 32) * ((s16)m->unk00)) / st->unk7C;
    if (flags[16]) {
        input[5] = 1;
        input[4] = 0;
        flags[5] = 0;
    } else if (which) {
        if (input[4]) {
            if (input[1]) {
                if (st->unkBC < m->vel.y) {
                    input[4] = 0;
                    if (!input[16])
                        input[5] = 1;
                    flags[5] = 1;
                    st->unk14 = 0;
                } else {
                    flags[7] = 1;
                    input[5] = 0;
                }
            } else if (m->vel.y < -st->unkBC) {
                input[4] = 0;
                input[5] = 1;
                flags[5] = 1;
                st->unk14 = 0;
            } else {
                flags[5] = 0;
                input[5] = 0;
            }
        } else {
            flags[5] = 0;
            input[5] = 0;
        }
    }
    if (flags[5]) {
        input[4] = 0;
        if (m->vel.y) {
            if (__builtin_abs(m->vel.y) < st->unkA4) {
                m->vel.y = 0;
                st->unk14 = 0;
            }
        } else {
            st->unk14 = (u16)(st->unk14 + GetFieldsLastFrame());
            m->vel.y = 0;
            if ((s16)st->unk14 >= 22)
                flags[5] = 0;
        }
    }
    if (input[8] && flags[16]) {
        if (st->unkF8 < 91) {
            if ((s16)weap->unk46 > 0) {
                --weap->unk46;
                st->unkF8 = st->unkF8 + 180;
            } else {
                st->unkF8 = 0;
                input[8] = 0;
                goto turbo_checked;
            }
        }
        st->unkF8 = st->unkF8 - 90;
        st->unk18 = 0;
    }
turbo_checked:
    if (input[8]) {
        input[4] = 1;
        if (st->unkF8 <= 0 && (s16)weap->unk46 > 0) {
            --weap->unk46;
            st->unkF8 = 180;
        }
        if (st->unkF8 > 0) {
            maxSpeed = (st->unkA8 * 3) / 2;
            acceleration = st->unkA4 * 4;
            st->unkF8 = st->unkF8 - ((s16)GetFieldsLastFrame());
            if (which)
                uasoundPlayTurboBoost(0, 0);
        } else {
            maxSpeed = st->unkA8;
            acceleration = st->unkA4;
            st->unkF8 = 0;
            if (st->unkA8 < m->vel.y)
                input[4] = 0;
            if (which)
                uasoundStopTurboBoost();
        }
    } else {
        if (which)
            uasoundStopTurboBoost();
        if (st->unkA8 < m->vel.y)
            input[4] = 0;
    }
    if (input[4]) {
        flags[4] = 1;
        flags[8] = 0;
        if (((flags[7] && -st->unkB8 < m->vel.y) || (!flags[7] && m->vel.y < maxSpeed))
            && !flags[1]) {
            if (flags[13])
                amount = st->unkA0 / 2;
            else {
                amount = acceleration;
                if ((flags[7] && m->vel.y < 0) || (!flags[7] && m->vel.y > 0))
                    amount = amount
                        - (((acceleration - st->unkA0) * __builtin_abs(m->vel.y)) / maxSpeed);
            }
            if (flags[7])
                m->vel.y = (m->vel.y - amount);
            else
                m->vel.y = (m->vel.y + amount);
        }
    } else
        flags[4] = 0;
    if (m->vel.y && !flags[4] && !flags[8]) {
        amount = (((st->unkBC - st->unkA0) * __builtin_abs(m->vel.y)) / maxSpeed) / 2;
        if (amount < st->unkA0)
            amount = st->unkA0;
        if (flags[13])
            amount /= 4;
        if (m->vel.y > 0) {
            m->vel.y = m->vel.y - amount;
            if (m->vel.y < 0)
                m->vel.y = 0;
        } else {
            m->vel.y = m->vel.y + amount;
            if (m->vel.y > 0)
                m->vel.y = 0;
        }
    }
    if (input[1]) {
        if (m->vel.y <= 0) {
            flags[7] = 1;
            if (-st->unkB8 < m->vel.y) {
                flags[4] = 0;
                flags[8] = 0;
            }
        }
    } else
        flags[7] = 0;
    if (input[5] && m->vel.y && !input[16]) {
        flags[3] = 1;
        amount = st->unkBC;
        if (flags[13])
            amount /= 4;
        if (m->vel.y < 0) {
            m->vel.y = m->vel.y + amount;
            if (m->vel.y > 0)
                m->vel.y = 0;
        } else {
            m->vel.y = m->vel.y - amount;
            if (m->vel.y < 0)
                m->vel.y = 0;
        }
    } else
        flags[3] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarUpdateDeltas);
#endif

#ifdef NON_MATCHING
void CarUpdateControlPad(Car* car)
{
    u16 idx;
    s32 turbo;
    u8 a;
    u8 b;
    u8 v;

    idx = car->playerIdx;
    turbo = 0;
    if (car->tires[0].unk3 && car->tires[1].unk3 && car->tires[2].unk3) {
        turbo = car->tires[3].unk3 != 0;
    }

    car->skid[0x0D] = GetCtlPad((s8)car->skid[0x26], (s16)idx);
    car->skid[0x0E] = GetCtlPad((s8)car->skid[0x27], (s16)idx);

    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x18], (s16)idx) != 0;
    }
    car->skid[0x11] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1A], (s16)idx) != 0;
    }
    car->skid[0x12] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1C], (s16)idx) != 0;
    }
    car->skid[0x13] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1B], (s16)idx) != 0;
    }
    car->skid[0x14] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x28], (s16)idx) != 0;
    }
    car->skid[0x17] = v;

    car->skid[0x03] = GetCtlPad((s8)car->skid[0x1C], (s16)idx);
    car->skid[0x02] = GetCtlPad((s8)car->skid[0x1B], (s16)idx);

    a = 0;
    if (car->tires[0].unk3) {
        if (car->tires[1].unk3) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
            a = 0;
        } else if (GetFrameCount() & 1) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
            a = 0;
        }
    } else if (car->tires[1].unk3) {
        if (GetFrameCount() & 1) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
            a = 0;
        }
    }

    if (GetCtlPad((s8)car->skid[0x1F], (s16)idx)) {
        a = car->skid[0x03] != 0;
    }
    car->skid[0x06] = a;
    a = 0;
    if (GetCtlPad((s8)car->skid[0x20], (s16)idx)) {
        a = car->skid[0x02] != 0;
    }
    car->skid[0x07] = a;
    a = 0;
    if (GetCtlPad((s8)car->skid[0x21], (s16)idx)) {
        a = turbo ^ 1;
    }
    car->skid[0x08] = a;

    a = 0;
    b = 0;
    if (GetCtlPad((s8)car->skid[0x19], (s16)idx) || GetCtlPad((s8)car->skid[0x1A], (s16)idx)) {
        if (!car->skid[0x0D] && !car->skid[0x0E]) {
            a = 1;
        }
    }
    car->skid[0x01] = a;

    if ((GetCtlPad((s8)car->skid[0x1D], (s16)idx) || GetCtlPad((s8)car->skid[0x18], (s16)idx)
            || car->skid[0x01])
        && !car->skid[0x0D] && !car->skid[0x0E]) {
        b = 1;
    }
    car->skid[0x15] = b;

    v = 0;
    if (car->skid[0x15] && !car->flags[0x10] && !car->flags[0x17]) {
        v = turbo ^ 1;
    }
    car->skid[0x04] = v;

    car->skid[0x09] = GetCtlPad((s8)car->skid[0x22], (s16)idx);
    car->skid[0x0A] = GetCtlPad((s8)car->skid[0x23], (s16)idx);
    car->skid[0x0B] = GetCtlPad((s8)car->skid[0x24], (s16)idx);
    car->skid[0x0C] = GetCtlPad((s8)car->skid[0x25], (s16)idx);

    v = 0;
    if (car->skid[0x03] || car->skid[0x02]) {
        v = 1;
    }
    car->flags[0x06] = v;

    v = 0;
    if ((car->skid[0x06] || car->skid[0x07]) && car->motion.vel.y > 0 && car->skid[0x01]) {
        v = 1;
    }
    car->skid[0x10] = v;

    if (car->skid[0x10]) {
        car->skid[0x05] = 0;
    }
    if (car->stats.unk40 <= 0) {
        car->skid[0x08] = 0;
        car->skid[0x04] = 0;
        car->skid[0x15] = 0;
        car->skid[0x09] = 0;
        car->skid[0x0A] = 0;
        car->skid[0x03] = 0;
        car->skid[0x02] = 0;
        car->skid[0x05] = 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarUpdateControlPad);
#endif

#ifdef NON_MATCHING
void CarRotUpdate(Car* car, u8 which)
{
    SVECTOR rot;
    CarMotion* m;

    if (which) {
        m = &car->motion;
    } else {
        m = &((CarAlt*)car)->motion;
    }

    m->rot.x += m->rotDelta.x;
    m->rot.y += m->rotDelta.y;
    m->rot.z += m->rotDelta.z;
    m->rot2.x += m->rot2Delta.x;
    m->rot2.y += m->rot2Delta.y;
    m->rot2.z += m->rot2Delta.z;
    BoundSAngle(&m->unk00);
    BoundVector(&m->rot.x);
    BoundVector(&m->rot2.x);
    rot.vx = m->rot2.x;
    rot.vy = m->rot2.y;
    rot.vz = m->rot2.z;
    RotMatrixYXZ(&rot, &m->mat);
    rot.vx = m->rot.x;
    rot.vy = m->rot.y;
    rot.vz = m->rot.z;
    RotMatrixYXZ(&rot, &m->mat2);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarRotUpdate);
#endif

void CarTransUpdate(Car* car, u8 which)
{
    VECTOR v;
    VECTOR out;
    CarMotion* m;

    if (which) {
        m = &car->motion;
    } else {
        m = &((CarAlt*)car)->motion;
    }

    v.vx = m->vel.x / 32;
    v.vy = m->vel.y / 32;
    v.vz = m->vel.z / 32;
    mathMulTransVec(&m->mat, &v, &out);
    m->pos.x += out.vx;
    m->pos.y += out.vy;
    m->pos.z += out.vz;
}

void CarCheckDynamics(Car* car)
{
    if (car->flags[1]) {
        UpdateDriftingCar(car);
    } else {
        UpdateNonDriftingCar(car);
    }
}

#ifdef NON_MATCHING
void CarInitMotion(Car* car, u8 which)
{
    u8* f;
    CarMotion* m;

    if (which) {
        f = car->flags;
        m = &car->motion;
    } else {
        f = ((CarAlt*)car)->flags;
        m = &((CarAlt*)car)->motion;
    }

    f[1] = 0;
    m->rotDelta.z = 0;
    m->rot2Delta.z = 0;
    m->rot2.x = m->rot.x;
    m->rot2.y = m->rot.y;
    m->rot2.z = m->rot.z;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarInitMotion);
#endif

void UpdateNonDriftingCar(Car* car)
{
    s32 limit;

    if (car->skid[0x10] && car->motion.vel.y > car->stats.unkB0) {
        goto drift;
    }
    if (car->motion.vel.y > car->stats.unkB0) {
        s32 delta;

        delta = car->motion.rotDelta.z;
        limit = car->stats.unkD8;
        if (delta < 0) {
            delta = -delta;
        }
        limit = limit < delta;
        if (limit) {
            goto drift;
        }
    }
    if (car->skid[0x10]) {
        goto drift;
    }
    SetNoCarDrift(&car->motion, car->flags);
    goto slow;
drift:
    car->flags[1] = 1;
    SetFullCarDrift(&car->motion);
slow:
    SlowDownNonDriftingCar(car, 1);
}

#ifdef NON_MATCHING
void SetNoCarDrift(CarMotion* m, u8* f)
{
    f[1] = 0;
    m->rot2Delta.x = m->rotDelta.x;
    m->rot2Delta.y = m->rotDelta.y;
    m->rot2Delta.z = m->rotDelta.z;
    m->rot2.x = m->rot.x;
    m->rot2.y = m->rot.y;
    m->rot2.z = m->rot.z;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SetNoCarDrift);
#endif

#ifdef NON_MATCHING
s32 CalcMaxRotBeforeDrift(Car* car, u8 which)
{
    CarStats* st;
    CarMotion* m;
    u8* sk;
    u8 hit;
    u8 g;
    s32 d;
    s32 hi;
    s32 t;
    s32 ret;

    if (which) {
        st = &car->stats;
        g = car->flags[4];
        m = &car->motion;
        sk = car->skid;
        hit = 0;
        if (g && car->flags[6] && (car->skid[6] || car->skid[7])) {
            hit = 1;
        }
        if (sk[6] || sk[7]) {
            d = st->unkD4 * st->unkE8;
        } else {
            d = st->unkD4;
        }
    } else {
        st = &((CarAlt*)car)->stats;
        g = ((CarAlt*)car)->flags[4];
        m = &((CarAlt*)car)->motion;
        sk = ((CarAlt*)car)->skid;
        hit = 0;
        if (g && ((CarAlt*)car)->flags[6] && (((CarAlt*)car)->skid[6] || ((CarAlt*)car)->skid[7])) {
            hit = 1;
        }
        if (sk[6] || sk[7]) {
            d = st->unkD4 * st->unkE8;
        } else {
            d = st->unkD4 * 2;
        }
    }

    if (hit) {
        return st->unkCC;
    }

    if (m->vel.y < st->unkA8) {
        hi = st->unkA8;
    } else {
        hi = m->vel.y;
    }
    if (st->unkB0 >= hi) {
        hi = st->unkB0 + 1;
    }

    if (m->vel.y > st->unkB0) {
        t = (m->vel.y / 32) * d / st->unk7C;
        if (d < t) {
            t = d;
        }
        return t - (t - st->unkD0) * (__builtin_abs(m->vel.y) - st->unkB0) / (hi - st->unkB0);
    }
    return d;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CalcMaxRotBeforeDrift);
#endif

#ifdef NON_MATCHING
void SlowDownNonDriftingCar(Car* car, u8 which)
{
    CarStats* st;
    CarMotion* m;
    u8 g;
    s32 d;
    s32 hi;

    if (which) {
        g = car->skid[6];
        st = &car->stats;
        m = &car->motion;
        if (g || car->skid[7]) {
            d = car->stats.unkD4 * car->stats.unkE8;
        } else {
            d = car->stats.unkD4;
        }
    } else {
        g = ((CarAlt*)car)->skid[6];
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        if (g || ((CarAlt*)car)->skid[7]) {
            d = ((CarAlt*)car)->stats.unkD4 * ((CarAlt*)car)->stats.unkE8;
        } else {
            d = ((CarAlt*)car)->stats.unkD4 * 2;
        }
    }

    if (m->vel.y < st->unkA8) {
        hi = st->unkA8;
    } else {
        hi = m->vel.y;
    }

    if (__builtin_abs(m->vel.y) > st->unkB0) {
        d = d - (d - st->unkEC) * __builtin_abs(m->vel.y) / hi;
        if (d < __builtin_abs((s16)m->unk00)) {
            if (m->vel.y > 0) {
                m->vel.y = m->vel.y - st->unkBC;
            } else if (m->vel.y < 0) {
                m->vel.y = m->vel.y + st->unkBC;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SlowDownNonDriftingCar);
#endif

void UpdateDriftingCar(Car* car)
{
    u8 doit;

    doit = 1;
    if (car->tires[0].unk3) {
        if (car->tires[1].unk3) {
            doit = 0;
        } else if (GetFrameCount() & 1) {
            doit = 0;
        }
    } else if (car->tires[1].unk3) {
        if (GetFrameCount() & 1) {
            doit = 0;
        }
    }

    if (doit) {
        if (car->skid[0x10]) {
            car->flags[1] = 1;
            SetFullCarDrift(&car->motion);
        } else if (!car->flags[0xB]) {
            BringBackDriftingCar(car, 1);
        }
        SlowDownDriftingCar(car, 1);
    }
}

#ifdef NON_MATCHING
void SetFullCarDrift(CarMotion* m)
{
    m->rot2Delta.z = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SetFullCarDrift);
#endif

#ifdef NON_MATCHING
void BringBackDriftingCar(Car* car, u8 which)
{
    CarStats* st;
    CarMotion* m;
    u8* f;
    u8* sk;
    s32 ang;
    s32 turn;
    s32 extra;
    s32 r;
    s32 delta;

    if (which) {
        st = &car->stats;
        m = &car->motion;
        f = car->flags;
        sk = car->skid;
    } else {
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        f = ((CarAlt*)car)->flags;
        sk = ((CarAlt*)car)->skid;
    }

    if (__builtin_abs(m->vel.y) < st->unkAC) {
        SetNoCarDrift(m, f);
        return;
    }

    ang = m->rot.z - m->rot2.z;
    BoundAngle(&ang);
    if (__builtin_abs(ang) >= 1366) {
        if ((s16)GetFieldsLastFrame() * 380 / 100 * 32 < m->vel.y) {
            return;
        }
    }

    if (ang > 0) {
        if (sk[3]) {
            turn = 0;
        } else if (sk[2]) {
            turn = (s16)GetFieldsLastFrame() * 5;
        } else {
            turn = (s16)GetFieldsLastFrame() * 2;
        }
    } else {
        if (sk[3]) {
            turn = (s16)GetFieldsLastFrame() * 5;
        } else if (sk[2]) {
            turn = 0;
        } else {
            turn = (s16)GetFieldsLastFrame() * 2;
        }
    }

    if (turn > 0) {
        if (m->vel.y > st->unkB0) {
            extra = 0;
        } else if (st->unkB0 / 2 < m->vel.y) {
            extra = (s16)GetFieldsLastFrame() * 2;
        } else {
            extra = (s16)GetFieldsLastFrame() * 5;
        }
    } else {
        extra = 0;
    }

    delta = (extra + turn) / 2 * 4096 / 360;
    if (f[4]) {
        delta = delta * 2;
    }

    r = SmoothAngleValue(m->rot2.z, m->rot.z, 90);
    if (m->rot2.z < r) {
        m->rot2.z = m->rot2.z + delta;
        BoundAngle(&m->rot2.z);
        if (m->rot2.z < m->rot.z) {
            return;
        }
    } else {
        m->rot2.z = m->rot2.z - delta;
        BoundAngle(&m->rot2.z);
        if (m->rot2.z > m->rot.z) {
            return;
        }
    }
    SetNoCarDrift(m, f);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", BringBackDriftingCar);
#endif

#ifdef NON_MATCHING
void SlowDownDriftingCar(Car* car, u8 which)
{
    s32 ang;
    s32 t;
    s32 d;
    u8* f;
    CarStats* st;
    CarMotion* m;
    u8* sk;

    if (which) {
        f = car->flags;
        st = &car->stats;
        m = &car->motion;
        sk = car->skid;
    } else {
        f = ((CarAlt*)car)->flags;
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        sk = ((CarAlt*)car)->skid;
    }

    ang = m->rot.z - m->rot2.z;
    BoundAngle(&ang);
    t = __builtin_abs(ang);

    t = t & (t | (s32)sk);
    if (t < 682) {
        d = st->unkBC / 4;
    } else if (t < 1024) {
        d = st->unkBC / 2;
    } else if (t < 1365) {
        d = st->unkBC;
    } else if (f[4]) {
        d = st->unkBC * 3;
    } else {
        d = st->unkBC * 2;
    }

    if (m->vel.y > 0) {
        m->vel.y -= d;
        if (m->vel.y < 0) {
            SetNoCarDrift(m, f);
        }
    } else {
        m->vel.y += d;
        if (m->vel.y > 0) {
            SetNoCarDrift(m, f);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SlowDownDriftingCar);
#endif

#ifdef NON_MATCHING
void UpdateBearing(Car* car, u8 which)
{
    s32 ang;
    s32 t;
    CarMotion* m;
    s32* bearing;

    if (which) {
        m = &car->motion;
        bearing = &car->bearing;
    } else {
        m = &((CarAlt*)car)->motion;
        bearing = &((CarAlt*)car)->bearing;
    }

    if (m->vel.y > 0) {
        ang = m->rot.z;
    } else {
        ang = m->rot.z + 2048;
        BoundAngle(&ang);
    }

    t = __builtin_abs(ang);
    if (t < 512) {
        *bearing = 0;
    } else {
        s32 value;

        if (t >= 1537) {
            value = 1;
        } else {
            value = 3;
            if (ang > 0) {
                value = 2;
            }
        }
        *bearing = value;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", UpdateBearing);
#endif

#ifdef NON_MATCHING

void CheckHitDetection(Car* car, u8 which)
{
    CarAlt* alt;
    u8* flags;
    CarStats* st;
    CarCollision* col;
    CarMotion* m;
    CarTire* t;
    Cs* cs;
    CarHit hit;
    PotHole* hole;
    s32 bearing;
    s32 speed;
    s32 denominator;
    s32 height;
    s32 difference;
    s32 rate;
    u32 type;
    int row, column, i;

    if (which) {
        flags = car->flags;
        st = &car->stats;
        col = &car->collision;
        m = &car->motion;
        t = car->tires;
        cs = GetPlayerCs3D((s16)car->playerIdx);
        bearing = car->bearing;
    } else {
        alt = (CarAlt*)car;
        flags = alt->flags;
        st = &alt->stats;
        col = &alt->collision;
        m = &alt->motion;
        t = alt->tires;
        cs = GetAICs3D((s16)alt->playerIdx);
        bearing = alt->bearing;
    }
    cs->rot.vx = m->rot.x;
    cs->rot.vy = m->rot.y;
    cs->rot.vz = m->rot.z;
    cs->pos.vx = m->pos.x;
    cs->pos.vy = m->pos.y;
    cs->pos.vz = m->pos.z;
    for (row = 0; row < 3; ++row)
        for (column = 0; column < 3; ++column)
            cs->mat.m[row][column] = m->mat2.m[row][column];
    for (i = 0; i < 3; ++i)
        cs->mat.t[i] = m->mat2.t[i];
    hit.unk00 = 0;
    HdCsTest(cs, &hit, 1, m->pad90b, m->pad90);
    if (hit.unk00) {
        if (!which && flags[29] && (s16)st->unk22 >= 13)
            hit.unk00 = 0;
        type = (u16)hit.unk0A;
        if (type - 401U < 50U) {
            carGetPickup(car, which, (s16)type, hit.unk0C);
            hit.unk00 = 0;
        } else if ((s16)type == 700) {
            UAeffectBarricade(700, hit.unk0C, bearing);
            speed = __builtin_abs(m->vel.y);
            denominator = speed < st->unkA8 ? st->unkA8 : speed;
            col->unk12 = (s32)((u32)speed * 99U) / denominator;
            hit.unk00 = 0;
        } else if (type - 750U < 33U || (s16)type == 1001) {
            bulDispatchDamage(hit.obj->unkC0, (s16)type, hit.unk0C, 10, &m->rot, 0);
            t[0].unk0A = MakeFakePotHole();
            t[1].unk0A = t[0].unk0A;
            t[2].unk0A = t[0].unk0A;
            t[3].unk0A = t[0].unk0A;
            for (i = 0; i < 4; ++i)
                t[i].unk1 = 1;
            hole = GetPotHoleDat((s16)t[0].unk0A);
            if (__builtin_abs(m->rot.z) < 512) {
                hole->x = t[0].unk38 - 16;
                hole->z = t[0].unk3C - 16;
                hole->a = st->unk74 + 32;
                hole->b = m->vel.y / 4;
            } else if (__builtin_abs(m->rot.z) >= 1537) {
                hole->x = t[1].unk38 - 16;
                hole->z = t[1].unk3C - 24;
                hole->a = st->unk74 + 32;
                hole->b = m->vel.y / 4;
            } else {
                if (m->rot.z > 0) {
                    hole->x = t[1].unk38 - 16;
                    hole->z = t[1].unk3C - 16;
                } else {
                    hole->x = t[0].unk38 - 24;
                    hole->z = t[0].unk3C - 16;
                }
                hole->a = m->vel.y / 4;
                hole->b = st->unk74 + 32;
            }
            hole->c = 40;
            hit.unk00 = 0;
        } else if ((s16)type == 1400) {
            for (i = 0; i < 4; ++i)
                t[i].unk14 = (u16)-20;
            hit.unk00 = 0;
            m->vel.y = 0;
        } else if ((s16)type == 500) {
            height = 160;
            if (!flags[10]) {
                speed = m->vel.y / 32;
                height = st->unk5C + __builtin_abs(speed);
            }
            if (flags[19] || flags[15]) {
                for (i = 0; i < 4; ++i)
                    t[i].unk14 = (u16)-20;
                hit.unk00 = 0;
                m->vel.y = 0;
            }
            switch ((s16)hit.mode) {
            case 0:
                CheckForBridge(&t[0], 1, height);
                break;
            case 1:
                CheckForBridge(&t[1], 1, height);
                break;
            case 2:
                CheckForBridge(&t[3], 1, height);
                break;
            case 3:
                CheckForBridge(&t[2], 1, height);
                break;
            default:
                CheckForBridge(&t[0], 1, height);
                CheckForBridge(&t[1], 1, height);
                break;
            }
            if (t[0].unk0 || t[1].unk0 || t[2].unk0 || t[3].unk0)
                hit.unk00 = 0;
        } else if ((s16)type == 1021) {
            if (!which && !alt->unk16E && !flags[0]) {
                if (alt->unk34 < 800 && alt->uaIndex != 130) {
                    if ((s16)col->unkE < 100 || flags[13] || flags[15])
                        hit.unk00 = 0;
                }
            } else
                hit.unk00 = 0;
        }
    }
    col->unk4 = hit.unk00;
    if (hit.unk00) {
        col->unk0 = 1;
        ++col->unk14;
        col->unk16 = 0;
        if ((s16)hit.mode < cs->unk74)
            CalcHitDynamics(car, &hit, which);
        else
            col->unk0 = 0;
    } else {
        ++col->unk16;
        if ((s16)col->count <= 0)
            col->unk14 = 0;
    }
    if (col->count) {
        if (which && flags[6]) {
            if ((s16)col->count < 120)
                InitNoCollision(col);
            else
                m->rotDelta.z = 0;
        }
        if (col->unk2) {
            m->rot.z = SmoothAngleValue(m->rot.z, (s16)col->unk8, 80);
        } else if (col->unk1 && m->vel.y) {
            difference = m->rot2.z - m->rot.z;
            BoundAngle(&difference);
            difference = __builtin_abs(difference);
            if (difference < 341)
                rate = 90;
            else if (difference < 568)
                rate = 95;
            else if (difference < 910)
                rate = 98;
            else
                rate = 99;
            m->rot.z = SmoothAngleValue(m->rot.z, (s16)col->unk8, rate);
        }

        col->count = (u16)(col->count - GetFieldsLastFrame());
        if ((s16)col->count <= 0 || __builtin_abs(m->vel.y) < st->unkA4
            || __builtin_abs((((s16)col->unk8) - m->rot.z)) < 56)
            InitNoCollision(col);
    }
    if ((s16)col->unkE < 151)
        col->unkE = (u16)((u16)col->unkE + GetFieldsLastFrame());
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CheckHitDetection);
#endif

#ifdef NON_MATCHING
void CalcHitDynamics(Car* car, CarHit* hit, u8 which)
{
    CarStats* st;
    CarMotion* m;
    CarTire* tires;
    u8* f;
    CarCollision* col;
    s32 idx;
    s32 mag;
    s32 dmg;
    s32 num;
    s32 den;
    SVECTOR rot;
    VECTOR delta;
    VECTOR out;
    VEC3 spark;
    MATRIX mat;
    s32 ang;
    s32 rel;
    s32 bounce;
    u8 hitCs;

    if (which) {
        st = &car->stats;
        f = car->flags;
        m = &car->motion;
        col = &car->collision;
        idx = car->playerIdx;
        tires = car->tires;
    } else {
        f = ((CarAlt*)car)->flags;
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        col = &((CarAlt*)car)->collision;
        idx = ((CarAlt*)car)->playerIdx;
        tires = ((CarAlt*)car)->tires;
    }

    if (hit->isCar) {
        if (m->vel.y >= 0) {
            ang = m->unkB0;
        } else {
            ang = m->unkB0 + 2048;
        }
    } else {
        if (hit->unk18 == 0) {
            hit->unk18 = 1;
        }
        ang = ratan2(hit->unk14, hit->unk18);
    }
    BoundAngle(&ang);
    if (m->vel.y >= 0) {
        rel = m->unkB0 - ang;
    } else {
        mag = ang - 2048;
        rel = m->unkB0 - mag;
    }
    BoundAngle(&rel);
    bounce = 2048 - rel * 2;
    BoundAngle(&bounce);

    if (hit->obj != 0 && hit->obj->unkC0 < 100 && hit->obj->unkC0 != 8) {
        hitCs = CheckCSHit(car, which, hit->obj, bounce, (s16)hit->mode);
        col->unk3 = 1;
        m->unkB0 = m->rot2.z;
        ang = m->unkB0;
        BoundAngle(&ang);
    } else {
        col->unk3 = 0;
        mag = m->vel.y;
        if (mag < 0) {
            mag = -mag;
        }
        hitCs = 0;
        if ((s16)GetFieldsLastFrame() * 570 / 100 * 32 < mag) {
            num = rsin(bounce / 4) * __builtin_abs(m->vel.y);
            dmg = __builtin_abs(num) * 10 / st->unkA8 / 4096;
            if (dmg > 0) {
                if (dmg - 3 > 0 && which) {
                    carTakeHit((s16)uaGetCarMatID((s16)idx, which), dmg - 3, (s32)&hit->unk14, 1);
                }
                bulDispatchDamage(hit->obj->unkC0, (s16)hit->unk0A, (s16)hit->unk0C,
                    dmg * st->unk104 / 33, &m->rot, 0);
                if (f[10]) {
                    tires[0].unk3 = 1;
                    tires[1].unk3 = 1;
                    tires[2].unk3 = 1;
                    tires[3].unk3 = 1;
                    if (tires[0].unk26 < 2) {
                        tires[0].unk14 = 0;
                    }
                    if (tires[1].unk26 < 2) {
                        tires[1].unk14 = 0;
                    }
                    if (tires[2].unk26 < 2) {
                        tires[2].unk14 = 0;
                    }
                    if (tires[3].unk26 < 2) {
                        tires[3].unk14 = 0;
                    }
                    tires[0].unk14 = (s16)tires[0].unk14 / 2;
                    tires[1].unk14 = (s16)tires[1].unk14 / 2;
                    tires[2].unk14 = (s16)tires[2].unk14 / 2;
                    tires[3].unk14 = (s16)tires[3].unk14 / 2;
                }
            }
        }
        if (!which) {
            CheckIfLostAICar(car);
            if (bounce >= 1707) {
                bounce = 1706;
            } else if (bounce < -1706) {
                bounce = -1706;
            }
        }
        RicochetOffObject(car, which, 0, bounce, (s16)hit->mode);
    }

    if (hitCs == 0) {
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ang;
        RotMatrixYXZ(&rot, &mat);
        delta.vx = 0;
        if (hit->isCar && col->unk3 == 0) {
            mag = m->vel.y / 32;
            if (mag < 0) {
                mag = m->vel.y / -32;
            }
            hit->unk10 += mag;
        }
        delta.vz = 0;
        delta.vy = -(hit->unk10 + 8);
        mathMulTransVec(&mat, &delta, &out);
        m->pos.x += out.vx;
        m->pos.y += out.vy;
        m->pos.z += out.vz;
        delta.vx = 0;
        delta.vy = 0;
        delta.vz = 0;
        switch ((s16)hit->mode) {
        case 0:
            delta.vx = 8;
            spark.x = tires[0].unk38;
            spark.y = tires[0].unk3C;
            spark.z = tires[0].unk40 + st->unk6C;
            break;
        case 1:
            delta.vx = -8;
            spark.x = tires[1].unk38;
            spark.y = tires[1].unk3C;
            spark.z = tires[1].unk40 + st->unk6C;
            break;
        case 2:
            delta.vx = -8;
            if (hit->isCar) {
                mag = hit->unk10;
                if (mag < 0) {
                    mag = -mag;
                }
                delta.vy = (mag + 8) * 2;
            }
            spark.x = tires[3].unk38;
            spark.y = tires[3].unk3C;
            spark.z = tires[3].unk40 + st->unk6C;
            break;
        case 3:
            delta.vx = 8;
            if (hit->isCar) {
                mag = hit->unk10;
                if (mag < 0) {
                    mag = -mag;
                }
                delta.vy = (mag + 8) * 2;
            }
            spark.x = tires[2].unk38;
            spark.y = tires[2].unk3C;
            spark.z = tires[2].unk40 + st->unk6C;
            break;
        case 4:
            delta.vy = -16;
            spark.x = (tires[0].unk38 + tires[1].unk38) / 2;
            spark.y = (tires[0].unk3C + tires[1].unk3C) / 2;
            spark.z = (tires[0].unk40 + tires[1].unk40) / 2 + st->unk6C;
            break;
        case 5:
            delta.vy = 8;
            spark.x = (tires[2].unk38 + tires[3].unk38) / 2;
            spark.y = (tires[2].unk3C + tires[3].unk3C) / 2;
            spark.z = (tires[2].unk40 + tires[3].unk40) / 2 + st->unk6C;
            break;
        case 6:
            delta.vx = 8;
            spark.x = (tires[0].unk38 + tires[2].unk38) / 2;
            spark.y = (tires[0].unk3C + tires[2].unk3C) / 2;
            spark.z = (tires[0].unk40 + tires[2].unk40) / 2 + st->unk6C;
            break;
        case 7:
            delta.vx = -8;
            spark.x = (tires[1].unk38 + tires[3].unk38) / 2;
            spark.y = (tires[1].unk3C + tires[3].unk3C) / 2;
            spark.z = (tires[1].unk40 + tires[3].unk40) / 2 + st->unk6C;
            break;
        default:
            spark.x = 0;
            spark.y = 0;
            spark.z = tires[0].unk40 + st->unk6C;
            break;
        }
        num = __builtin_abs(m->vel.y) * 3 * 33;
        if (__builtin_abs(m->vel.y) < st->unkA8) {
            den = st->unkA8;
        } else {
            den = __builtin_abs(m->vel.y);
        }
        col->unk12 = num / den;
        mathMulTransVec(&m->mat2, &delta, &out);
        m->pos.x += out.vx;
        m->pos.y += out.vy;
        m->pos.z += out.vz;
        if ((s16)GetFieldsLastFrame() * 380 / 100 * 32 < m->vel.y) {
            do_simple_spark(&spark);
            if (hit->isCar) {
                do_puff(&spark);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CalcHitDynamics);
#endif

#ifdef NON_MATCHING
void CheckIfLostAICar(Car* car)
{
    car->stats.lostTimer++;
    if ((s16)car->stats.lostTimer >= 4) {
        StartLostCheck(car);
    }
    if (CAR_LOST_BYTE(car)) {
        car->stats.unk4A = 80;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CheckIfLostAICar);
#endif

#ifdef NON_MATCHING
u8 CheckCSHit(Car* car, u8 which, Cs* obj, s32 a, s32 b)
{
    u8 isPlayer;
    s16 idx;
    Car* info;
    u8 ret;
    u8 hit;
    s32 bb;

    hit = uaIsCarCS(obj, &isPlayer, &idx);
    bb = b;
    if (hit) {
        if (isPlayer) {
            info = GetPlayerInfo(idx);
        } else {
            info = (Car*)GetAICarInfo(idx);
        }
        DoCsHitCalculations(car, which, info, isPlayer, a, (s16)bb);
        ret = 0;
    } else {
        RicochetOffObject(car, which, 0, a, (s16)b);
        ret = 0;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CheckCSHit);
#endif

#ifdef NON_MATCHING
void DoCsHitCalculations(Car* car, u8 which, Car* other, u8 otherWhich, s32 oang, u16 mode)
{
    u8* f;
    CarStats* st;
    CarMotion* m;
    CarCollision* col;
    u8* of;
    CarStats* ost;
    CarMotion* om;
    CarCollision* ocol;
    u16 idx;
    u16 oidx;
    s32 dead;
    u8 front;
    u8 back;
    u8 driftMe;
    u8 driftOther;
    s32 ang;
    s32 dmg;
    s32 a;
    s32 b;
    s32 e;
    s32 t;
    VEC3 spark;
    s32 rel;

    driftMe = 0;
    driftOther = 0;
    if (which) {
        col = &car->collision;
        f = car->flags;
        st = &car->stats;
        m = &car->motion;
        idx = car->playerIdx;
        dead = car->uaIndex;
    } else {
        f = ((CarAlt*)car)->flags;
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        col = &((CarAlt*)car)->collision;
        idx = ((CarAlt*)car)->playerIdx;
        dead = ((CarAlt*)car)->uaIndex;
    }
    if (otherWhich) {
        of = other->flags;
        ost = &other->stats;
        om = &other->motion;
        ocol = &other->collision;
        oidx = other->playerIdx;
    } else {
        ost = &((CarAlt*)other)->stats;
        of = ((CarAlt*)other)->flags;
        om = &((CarAlt*)other)->motion;
        ocol = &((CarAlt*)other)->collision;
        oidx = ((CarAlt*)other)->playerIdx;
    }
    if (which) {
        if (!otherWhich && !of[0]) {
            of[24] = 1;
        }
    } else if (otherWhich && !f[0]) {
        f[24] = 1;
    }
    front = mode < 2 || (s16)mode == 4;
    back = mode == 2 || mode == 3;
    if (st->monster) {
        if (f[27]) {
            return;
        }
        if ((front && m->vel.y > 0) || (back && m->vel.y < 0)) {
            InitMonsterSmash(car, which, other, otherWhich, (s16)mode);
            return;
        }
    } else if (of[28]) {
        return;
    }

    if (which || otherWhich) {
        a = m->vel.y;
        if (a < 0) {
            a = -a;
        }
        if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a) {
            goto bigHit;
        }
        a = om->vel.y;
        if (a < 0) {
            a = -a;
        }
        if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a) {
        bigHit:
            if (st->unk108 > ost->unk108) {
                if (!f[27]) {
                    carTakeHit(uaGetCarMatID(idx, which), 1, (s32)&m->unkA8, 1);
                }
                a = m->vel.y;
                if (a < 0) {
                    a = -a;
                }
                if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a && !of[27]) {
                    dmg = 12 * m->vel.y / st->unkA8;
                    if (dmg > 0) {
                        carTakeHit(uaGetCarMatID(oidx, otherWhich), dmg, (s32)&m->unkA8, 1);
                    }
                }
            } else if (st->unk108 < ost->unk108) {
                a = om->vel.y;
                if (a < 0) {
                    a = -a;
                }
                if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a && !f[27]) {
                    if (12 * om->vel.y / ost->unkA8 > 0) {
                        carTakeHit(uaGetCarMatID(idx, which), 11, (s32)&om->unkA8, 1);
                    }
                }
                if (!of[27]) {
                    carTakeHit(uaGetCarMatID(oidx, otherWhich), 1, (s32)&om->unkA8, 1);
                }
            } else {
                a = om->vel.y;
                if (a < 0) {
                    a = -a;
                }
                if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a && !f[27]) {
                    dmg = 6 * om->vel.y / ost->unkA8;
                    if (dmg > 0) {
                        carTakeHit(uaGetCarMatID(idx, which), dmg, (s32)&om->unkA8, 1);
                    }
                }
                a = m->vel.y;
                if (a < 0) {
                    a = -a;
                }
                if (((s16)GetFieldsLastFrame() * 570 / 100 * 32) < a && !of[27]) {
                    dmg = 6 * m->vel.y / st->unkA8;
                    if (dmg > 0) {
                        carTakeHit(uaGetCarMatID(oidx, otherWhich), dmg, (s32)&m->unkA8, 1);
                    }
                }
            }
        }
    }

    a = m->vel.y;
    if (a < 0) {
        a = -a;
    }
    if (st->unkA4 < a) {
        a = m->unkB0;
    } else {
        a = m->rot.z;
    }
    b = om->vel.y;
    if (b < 0) {
        b = -b;
    }
    if (ost->unkA4 < b) {
        b = om->unkB0;
    } else {
        b = om->rot.z;
    }
    rel = a - b;
    BoundAngle(&rel);

    if (m->vel.y != 0 && (om->vel.y == 0 || of[0])) {
        of[1] = 1;
        SetFullCarDrift(om);
        om->rot2.z = m->rot2.z;
        ang = 0;
        oang = 0;
        if (st->unk108 > ost->unk108) {
            if (st->unk108 - 1 > ost->unk108) {
                om->vel.y = 3 * m->vel.y;
                m->vel.y = 4 * m->vel.y / 5;
            } else {
                om->vel.y = 2 * m->vel.y;
                m->vel.y /= 2;
            }
        } else if (ost->unk108 > st->unk108) {
            om->vel.y = m->vel.y / 4;
            m->vel.y /= 8;
        } else {
            om->vel.y = m->vel.y;
            m->vel.y /= 2;
        }
    } else if (om->vel.y != 0 && (m->vel.y == 0 || f[0])) {
        f[1] = 1;
        SetFullCarDrift(m);
        m->rot2.z = om->rot2.z;
        ang = 0;
        oang = 0;
        if (ost->unk108 > st->unk108) {
            if (ost->unk108 - 1 > st->unk108) {
                m->vel.y = 3 * om->vel.y;
                om->vel.y = 4 * om->vel.y / 5;
            } else {
                m->vel.y = 2 * om->vel.y;
                om->vel.y /= 2;
            }
        } else if (st->unk108 < ost->unk108) {
            m->vel.y = om->vel.y / 4;
            om->vel.y /= 8;
        } else {
            m->vel.y = om->vel.y;
            om->vel.y /= 2;
        }
    } else {
        e = oang;
        if (e < 0) {
            e = -e;
        }
        if (e < 1024) {
            ang = oang;
            if (st->unk108 > ost->unk108) {
                ang = oang / 2;
                om->vel.y /= 2;
            } else if (st->unk108 < ost->unk108) {
                oang /= 2;
                m->vel.y /= 2;
            }
            e = __builtin_abs(rel);
            if (e < 1024) {
                oang = -oang;
            }
            a = m->vel.y;
            b = om->vel.y;
            if (a < 0) {
                a = -a;
            }
            if (b < 0) {
                b = -b;
            }
            if (2 * b < a) {
                om->rot2.z = m->unkB0 - ang;
                if (m->vel.y < 0) {
                    om->rot2.z -= 2048;
                    oang = 0;
                    if (e < 1024) {
                        om->rot2.z -= 2 * ang;
                        oang = 0;
                    }
                } else {
                    oang = 0;
                    if (e >= 1025) {
                        om->rot2.z -= 2 * ang;
                        oang = 0;
                    }
                }
            } else {
                a = om->vel.y;
                b = m->vel.y;
                if (a < 0) {
                    a = -a;
                }
                if (b < 0) {
                    b = -b;
                }
                if (2 * b < a) {
                    m->rot2.z = om->unkB0 - oang;
                    if (om->vel.y < 0) {
                        m->rot2.z -= 2048;
                        ang = 0;
                        if (e < 1024) {
                            m->rot2.z -= 2 * oang;
                            ang = 0;
                        }
                    } else {
                        ang = 0;
                        if (e >= 1025) {
                            m->rot2.z -= 2 * oang;
                            ang = 0;
                        }
                    }
                }
            }
            a = m->vel.y;
            b = om->vel.y;
            if (a < 0) {
                a = -a;
            }
            if (b < 0) {
                b = -b;
            }
            if (b < a) {
                e = rel;
                if (e < 0) {
                    e = -e;
                }
                if (e < 1024) {
                    om->vel.y = m->vel.y;
                } else {
                    om->vel.y = -m->vel.y;
                }
            } else {
                e = rel;
                if (e < 0) {
                    e = -e;
                }
                if (e < 1024) {
                    m->vel.y = om->vel.y;
                } else {
                    m->vel.y = -om->vel.y;
                }
            }
        } else {
            e = __builtin_abs(rel);
            if (e < 682) {
                a = m->vel.y - om->vel.y;
                b = om->vel.y - m->vel.y;
                if (st->unk108 > ost->unk108) {
                    a /= 2;
                    b *= 2;
                } else if (st->unk108 < ost->unk108) {
                    a *= 2;
                    b /= 2;
                }
                m->rot2.z = m->rot.z;
                m->vel.y -= a;
                om->rot2.z = om->rot.z;
                om->vel.y -= b;
                if ((s16)mode == 1 || (s16)mode == 3) {
                    ang = -170;
                    oang = 170;
                } else {
                    ang = 170;
                    oang = -170;
                }
            } else if (e < 1365) {
                ang = 0;
                oang = 0;
                if (front) {
                    om->vel.y = m->vel.y;
                    om->rot2.z = m->rot2.z;
                    of[1] = 1;
                    SetFullCarDrift(om);
                    if (st->unk108 > ost->unk108) {
                        if (st->unk108 - 1 > ost->unk108) {
                            t = 4 * m->vel.y / 5;
                        } else {
                            t = m->vel.y / 2;
                        }
                    } else {
                        t = m->vel.y / 4;
                    }
                    m->vel.y = t;
                    if (st->unk104 >= ost->unk104 && st->unkC0 < m->vel.y) {
                        driftOther = 1;
                    }
                } else {
                    m->vel.y = om->vel.y;
                    m->rot2.z = om->rot2.z;
                    f[1] = 1;
                    SetFullCarDrift(m);
                    if (ost->unk108 > st->unk108) {
                        if (ost->unk108 - 1 > st->unk108) {
                            t = 4 * om->vel.y / 5;
                        } else {
                            t = om->vel.y / 2;
                        }
                    } else {
                        t = om->vel.y / 4;
                    }
                    om->vel.y = t;
                    if (ost->unk104 >= st->unk104 && ost->unkC0 < om->vel.y) {
                        driftMe = 1;
                    }
                }
                if (st->unk108 > ost->unk108) {
                    if (st->unk108 - 1 > ost->unk108) {
                        om->vel.y *= 3;
                    } else {
                        om->vel.y *= 2;
                    }
                } else if (ost->unk108 > st->unk108) {
                    if (ost->unk108 - 1 < st->unk108) {
                        m->vel.y = 3 * m->vel.y;
                    } else {
                        m->vel.y = 2 * m->vel.y;
                    }
                }
                if (mode == 0) {
                    ang += 170;
                } else {
                    ang -= 170;
                }
            } else {
                a = m->vel.y + om->vel.y;
                b = a;
                if (st->unk108 > ost->unk108) {
                    if (st->unk108 - 1 > ost->unk108) {
                        b = m->vel.y / 8;
                        a *= 3;
                    } else {
                        b = m->vel.y / 4;
                        a *= 2;
                    }
                    of[1] = 1;
                    SetFullCarDrift(om);
                    if (st->unkC0 < m->vel.y) {
                        driftOther = 1;
                    }
                } else if (ost->unk108 > st->unk108) {
                    if (ost->unk108 - 1 > st->unk108) {
                        a = om->vel.y / 8;
                        b *= 3;
                    } else {
                        a = om->vel.y / 4;
                        b *= 2;
                    }
                    f[1] = 1;
                    SetFullCarDrift(m);
                    if (ost->unkC0 < om->vel.y) {
                        driftMe = 1;
                    }
                }
                if (om->vel.y - a < m->vel.y - b) {
                    om->vel.y = m->vel.y;
                    om->rot2.z = om->rot.z - 2048;
                    oang = 0;
                    ang = 0;
                    m->vel.y -= b;
                } else {
                    m->vel.y = om->vel.y;
                    m->rot2.z = m->rot.z - 2048;
                    ang = 0;
                    oang = 0;
                    om->vel.y -= a;
                }

                if ((s16)mode == 1) {
                    ang += 170;
                    oang -= 170;
                } else {
                    ang -= 170;
                    oang += 170;
                }
            }
        }
    }

    if (driftMe) {
        f[23] = 1;
        m->unk02 = m->rot2.z;
        rel = m->rot.z - m->rot2.z;
        BoundAngle(&rel);
        if (rel > 0) {
            m->rotDelta.y = 1;
            t = m->rot2.z + 1024;
        } else {
            m->rotDelta.y = -1;
            t = m->rot2.z - 1024;
        }
        *(s16*)&m->unk04 = t;
    }
    if (driftOther) {
        of[23] = 1;
        om->unk02 = om->rot2.z;
        rel = om->rot.z - om->rot2.z;
        BoundAngle(&rel);
        if (rel > 0) {
            om->rotDelta.y = 1;
            t = om->rot2.z + 1024;
        } else {
            om->rotDelta.y = -1;
            t = om->rot2.z - 1024;
        }
        *(s16*)&om->unk04 = t;
    }
    if ((s16)col->unkE < 150 && (s16)col->unk10 == (s16)oidx) {
        spark.x = (m->pos.x + om->pos.x) / 2;
        spark.y = (m->pos.y + om->pos.y) / 2;
        spark.z = (m->pos.z + om->pos.z) / 2;
        do_simple_spark(&spark);
        m->rot2.z = m->rot.z;
        switch ((s16)mode) {
        case 0:
            ang = (m->vel.y == 0) ? 0x238 : 0;
            m->rot2.z += 512;
            break;
        case 1:
            ang = (m->vel.y == 0) ? -0x238 : 0;
            m->rot2.z -= 512;
            break;
        case 2:
            ang = -512;
            m->rot2.z -= 512;
            break;
        case 3:
            ang = 512;
            m->rot2.z += 512;
            break;
        case 4:
            ang = (m->vel.y == 0) ? 0x155 : 0;
            m->rot2.z += 1024;
            break;
        case 5:
            ang = 0;
            m->rot2.z += 512;
            break;
        case 6:
            ang = 1024;
            m->rot2.z += 512;
            break;
        case 7:
            ang = -1024;
            m->rot2.z -= 512;
            break;
        default:
            ang = 1024;
            m->rot2.z += 1024;
            break;
        }

        if (m->vel.y > 0) {
            a = m->vel.y;
            if (a < 0) {
                a = -a;
            }
            if (a < ((s16)GetFieldsLastFrame() * 1140 / 100 * 32)) {
                t = (s16)GetFieldsLastFrame() * 1140 / 100 * 32;
            } else {
                t = m->vel.y;
                if (t < 0) {
                    t = -t;
                }
            }
        } else {
            a = m->vel.y;
            if (a < 0) {
                a = -a;
            }
            if (a >= -((s16)GetFieldsLastFrame() * 1140 / 100 * 32)) {
                t = -((s16)GetFieldsLastFrame() * 1140 / 100 * 32);
            } else {
                t = m->vel.y;
                if (t < 0) {
                    t = -t;
                }
            }
        }
        m->vel.y = t;
    }
    col->unkE = 0;
    col->unk10 = oidx;
    ocol->unkE = 0;
    ocol->unk10 = idx;
    if (st->unkA8 < m->vel.y) {
        m->vel.y = st->unkA8;
    }
    if (ost->unkA8 < om->vel.y) {
        om->vel.y = ost->unkA8;
    }
    RicochetOffObject(car, which, 1, ang, (s16)mode);
    ocol->unk0 = 1;
    RicochetOffObject(other, otherWhich, 1, oang, (s16)mode);
    RotateCarsAwayFromCollision(m, om);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", DoCsHitCalculations);
#endif

void RotateCarsAwayFromCollision(CarMotion* a, CarMotion* b)
{
    s32 ang;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 bx;
    s32 ay;
    s32 by;

    do {
        ax = a->pos.x;
        bx = b->pos.x;
        ay = a->pos.y;
        by = b->pos.y;
    } while (0);
    dy = ay - by;
    dx = ax - bx;
    if (dy == 0) {
        dy = 1;
    }
    ang = ratan2(dx, dy) - a->rot.z;
    BoundAngle(&ang);
}

#ifdef NON_MATCHING
void RicochetOffObject(Car* car, u8 which, u8 doHit, s32 delta, u16 mode)
{
    CarMotion* m;
    u8* f;
    CarCollision* c;
    u8* skid;
    CarStats* st;
    s32 ang;
    s32 sub;
    u8 ok;

    if (which) {
        m = &car->motion;
        f = car->flags;
        st = &car->stats;
        c = &car->collision;
        skid = car->skid;
    } else {
        m = &((CarAlt*)car)->motion;
        f = ((CarAlt*)car)->flags;
        c = &((CarAlt*)car)->collision;
        skid = ((CarAlt*)car)->skid;
        st = &((CarAlt*)car)->stats;
        ((CarAlt*)car)->unk16D = 1;
    }

    m->rot2.z += delta;
    BoundAngle(&m->rot2.z);
    f[1] = 1;
    SetCollisionBounce(car, which, delta);

    if (which && f[6] && m->vel.y < (s16)GetFieldsLastFrame() * 570 / 100 * 32
        && (mode < 2 || (s16)mode == 4)) {
        c->unk2 = 1;
    } else {
        c->unk2 = 0;
    }

    if (mode < 2 || (s16)mode == 4 || skid[1]) {
        if (c->unk2 || __builtin_abs(delta) < 682) {
            m->rot2.z -= delta / 3;
            c->unk8 = m->rot2.z;
        } else if (__builtin_abs(delta) < 1365) {
            c->unk8 = m->rot2.z - delta / 2;
        } else {
            c->unk8 = m->rot2.z - 2048;
        }
        BoundSAngle(&c->unk8);
        c->unk1 = 1;
    }

    ang = m->rot.z - m->rot2.z;
    BoundAngle(&ang);

    if (c->unk2) {
        if (f[6]) {
            if ((ang < 0 && skid[2]) || (ang > 0 && skid[3])) {
                c->unk8 = m->rot2.z - 2048;
                BoundSAngle(&c->unk8);
            }
        }
    }

    if (which && f[6]) {
        if (skid[2]) {
            c->unk8 = m->rot.z - 512;
        } else {
            c->unk8 = m->rot.z + 512;
        }
        BoundSAngle(&c->unk8);
        c->unk1 = 1;
    }

    if (doHit && st->unkC0 < m->vel.y) {
        if (ang >= 1764) {
            ok = 0;
        } else if (ang >= 683) {
            ok = 1;
        } else if (ang < -1763) {
            ok = 0;
        } else {
            ok = ang < -682;
        }
        if (ok) {
            c->unkC = 1;
        } else {
            c->unkC = 0;
        }
    } else {
        c->unkC = 0;
    }

    sub = 0;
    if (m->vel.y < 0) {
        sub = m->vel.y * 3 / 4;
    } else if (m->vel.y >= (s16)GetFieldsLastFrame() * 570 / 100 * 32) {
        if (__builtin_abs(delta) < 512) {
            sub = m->vel.y / 4;
        } else if (__builtin_abs(delta) < 1024) {
            sub = m->vel.y / 3;
        } else {
            sub = m->vel.y / 2;
        }
    }
    m->vel.y -= sub;
    SetCollisCount(c);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", RicochetOffObject);
#endif

void SetCollisionBounce(Car* car, u8 which, s32 mag)
{
    mag = __builtin_abs(mag);
    if (mag < 227) {
        SetBounce(car, 1, 2, 2, which);
    } else if (mag < 512) {
        SetBounce(car, 2, 2, 2, which);
    } else if (mag < 1024) {
        SetBounce(car, 3, 2, 2, which);
    } else if (mag < 1536) {
        SetBounce(car, 3, 2, 2, which);
    } else {
        SetBounce(car, 5, 2, 2, which);
    }
}

#ifdef NON_MATCHING
void InitNoCollision(CarCollision* c)
{
    c->unk0 = 0;
    c->unk1 = 0;
    c->unk2 = 0;
    c->unk4 = 0;
    c->count = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", InitNoCollision);
#endif

void SetCollisCount(CarCollision* c)
{
    if ((s32)((u32)c->count << 16) <= 0) {
        c->count = 150;
    }
}

#ifdef NON_MATCHING
void CarViewUpdate(Car* car)
{
    VECTOR v;
    VECTOR out;
    Cs* cs;
    s32 tilt;
    s32 dx;
    s32 dy;
    s32 dz;

    if (car->stats.unk3C) {
        uaDrawCar((s16)car->playerIdx, 1);
    } else {
        uaDontDrawCar((s16)car->playerIdx, 1);
    }
    if ((u32)car->unkF4 < 4) {
        GetUpdateRate();
        if (car->unkF4 == 1) {
            if (__builtin_abs(car->dTrans.y - car->vrPos[0].y) >= 17) {
                car->dTrans.x = SmoothValue(car->dTrans.x, car->vrPos[0].x, 0x5A);
                car->dTrans.y = SmoothValue(car->dTrans.y, car->vrPos[0].y, 0x5A);
                car->dTrans.z = SmoothValue(car->dTrans.z, car->vrPos[0].z, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[0].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[0].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[0].vz, 0x5A);
            } else {
                uaDontDrawCar((s16)car->playerIdx, 1);
                car->dTrans.x = car->vrPos[0].x;
                car->dTrans.y = car->vrPos[0].y;
                car->dTrans.z = car->vrPos[0].z;
                car->dRot.vx = car->vrRot[0].vx;
                car->dRot.vy = car->vrRot[0].vy;
                car->dRot.vz = car->vrRot[0].vz;
                if (car->stats.unk3C) {
                    UAdashSetDashboardDrawFlag(1);
                    uaDontDrawCar((s16)car->playerIdx, 1);
                }
            }
        } else if (car->unkF4 == 2) {
            if (__builtin_abs(car->dTrans.y - car->vrPos[1].y) >= 17) {
                car->dTrans.y = SmoothValue(car->dTrans.y, car->vrPos[1].y, 0x5A);
                car->dTrans.z = SmoothValue(car->dTrans.z, car->vrPos[1].z, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[1].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[1].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[1].vz, 0x5A);
            } else {
                car->dTrans.y = car->vrPos[1].y;
                car->dTrans.z = car->vrPos[1].z;
                car->dRot.vx = car->vrRot[1].vx;
                car->dRot.vy = car->vrRot[1].vy;
                car->dRot.vz = car->vrRot[1].vz;
            }
            if (car->motion.rotDelta.z > car->stats.unkD4) {
                tilt = 0x70;
            } else if (car->motion.rotDelta.z < -car->stats.unkD4) {
                tilt = -0x70;
            } else {
                tilt = car->motion.rotDelta.z * 0x70 / car->stats.unkD4;
            }
            car->dTrans.x = SmoothValue(car->dTrans.x, tilt, 0x5A);
        } else {
            if (__builtin_abs(car->dTrans.y - car->vrPos[2].y) >= 17) {
                car->dTrans.y = SmoothValue(car->dTrans.y, car->vrPos[2].y, 0x5A);
                car->dTrans.z = SmoothValue(car->dTrans.z, car->vrPos[2].z, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[2].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[2].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[2].vz, 0x5A);
            } else {
                car->dTrans.y = car->vrPos[2].y;
                car->dTrans.z = car->vrPos[2].z;
                car->dRot.vx = car->vrRot[2].vx;
                car->dRot.vy = car->vrRot[2].vy;
                car->dRot.vz = car->vrRot[2].vz;
            }
            if (car->motion.rotDelta.z > car->stats.unkD4) {
                tilt = 0xA0;
            } else if (car->motion.rotDelta.z < -car->stats.unkD4) {
                tilt = -0xA0;
            } else {
                tilt = car->motion.rotDelta.z * 0xA0 / car->stats.unkD4;
            }
            car->dTrans.x = SmoothValue(car->dTrans.x, tilt, 0x5A);
        }
        viewSetDeltaRot(&car->dRot, (s16)car->playerIdx);
        viewSetDeltaTrans(&car->dTrans, (s16)car->playerIdx);
        return;
    }
    if (car->unkF4 == 4) {
        cs = UAGetCameraCS((s16)car->playerIdx);
        v.vx = 0;
        v.vy = 100;
        v.vz = 0;
        mathMulTransVec(&car->motion.mat2, &v, &out);
        out.vx += car->motion.pos.x;
        out.vy += car->motion.pos.y;
        out.vz += car->motion.pos.z;
        dx = out.vx - cs->pos.vx;
        dy = out.vy - cs->pos.vy;
        dz = out.vz - cs->pos.vz;
        car->dRot.vx = -ratan2(dz, SquareRoot0(dx * dx + dy * dy));
        car->dRot.vy = 0;
        car->dRot.vz = ratan2(dx, dy);
        viewSetDeltaRot(&car->dRot, (s16)car->playerIdx);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarViewUpdate);
#endif

#ifdef NON_MATCHING
void SetBounce(Car* car, s32 level, s32 a, s32 b, u8 which)
{
    CarBounce* bp;
    u32 mag;
    u32 ang;

    if (which) {
        bp = &car->bounce;
    } else {
        bp = &((CarAlt*)car)->bounce;
    }

    bp->unk04 = level;
    bp->unk0C = b;
    if (bp->unk0C <= 0) {
        bp->unk00 = level;
        bp->unk08 = a;
    }
    bp->unk0E = rand() % 11;

    switch (level) {
    case 5:
        mag = 0x71;
        ang = 0x200;
        break;
    case 4:
        mag = 0x44;
        ang = 0x200;
        break;
    case 3:
        mag = 0x2D;
        ang = 0x200;
        break;
    case 2:
        mag = 0x0B;
        ang = 0x155;
        break;
    case 1:

        mag = 2;
        ang = 0xE3;
        break;
    default:
        mag = 2;
        ang = 0xE3;
        break;
    }

    mag = mag - rand() % (mag >> 1);
    if (a == 0 || a == 2) {
        if (rand() & 1) {
            mag = -mag;
        }
        bp->unk18 = mag;
    } else {
        bp->unk18 = 0;
    }
    if (a == 1 || a == 2) {
        if (rand() & 1) {
            mag = -mag;
        }
        bp->unk1C = mag;
    } else {
        bp->unk1C = 0;
    }
    bp->unk12 = ang - rand() % (ang >> 1);
    CarInitBounceDeltas(car, which);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SetBounce);
#endif

#ifdef NON_MATCHING
void CarInitBounceDeltas(Car* car, u8 which)
{
    CarBounce* bp;
    CarMotion* m;

    if (which) {
        bp = &car->bounce;
        m = &car->motion;
    } else {
        bp = &((CarAlt*)car)->bounce;
        m = &((CarAlt*)car)->motion;
    }

    if (m->vel.y / 32) {
        bp->unk14 = bp->unk12 - rand() % (bp->unk12 / 2);
        if (bp->unk18 > 0) {
            bp->unk24 = bp->unk18 - rand() % (bp->unk18 / 2);
        } else if (bp->unk18 < 0) {
            bp->unk24 = bp->unk18 + rand() % (bp->unk18 / 2);
        }
        if (bp->unk1C > 0) {
            bp->unk28 = bp->unk1C - rand() % (bp->unk1C / 2);
        } else if (bp->unk1C < 0) {
            bp->unk28 = bp->unk1C + rand() % (bp->unk1C / 2);
        }
    } else {
        bp->unk04 = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CarInitBounceDeltas);
#endif

#ifdef NON_MATCHING
void UpdateBounce(Car* car, u8 which)
{
    CarBounce* bp;
    CarMotion* m;
    CarStats* st;
    s32 d;
    s32 q;
    s32 v;
    s32 qs;
    s32 amp[4];

    if (which) {
        bp = &car->bounce;
        m = &car->motion;
        st = &car->stats;
    } else {
        bp = &((CarAlt*)car)->bounce;
        m = &((CarAlt*)car)->motion;
        st = &((CarAlt*)car)->stats;
    }

    if (which) {
        d = st->unkD4;
    } else {
        d = st->unkD4 / 10;
    }

    q = m->rotDelta.z * st->unkF4 / d;
    qs = (s16)q;
    v = q;
    if (__builtin_abs(st->unkF4) < __builtin_abs(qs)) {
        if (st->unkF4 > 0) {
            if (qs > 0) {
                v = st->unkF4;
            } else {
                v = -st->unkF4;
            }
        } else {
            if (qs > 0) {
                v = -st->unkF4;
            } else {
                v = st->unkF4;
            }
        }
    }
    bp->unk34 = SmoothValue(bp->unk34, (s16)v, 90);

    if (which && car->unkF4 == 1) {
        bp->unk16 = SmoothValue(bp->unk16, car->speedDelta, 90);
        bp->unk30 = -bp->unk16 / 2;
    } else {
        bp->unk30 = 0;
    }

    if (bp->unk04) {
        bp->unk0E = bp->unk0E + bp->unk14;
        if (bp->unk0E < 4096) {
            amp[0] = bp->unk24 * m->vel.y / (m->vel.y < st->unkA8 ? st->unkA8 : m->vel.y);
            amp[1] = bp->unk28 * m->vel.y / (m->vel.y < st->unkA8 ? st->unkA8 : m->vel.y);
            bp->unk30 += rsin(bp->unk0E) * amp[0] / 4096;
            bp->unk34 += rsin(bp->unk0E) * amp[1] / 4096;
        } else {
            bp->unk0E = bp->unk0E - 4096;
            if (bp->unk0C > 0) {
                bp->unk0C = bp->unk0C - 1;
                bp->unk0C;
                bp->unk24 = bp->unk24 / 2;
                bp->unk28 = bp->unk28 / 2;
            } else {
                SetBounce(car, bp->unk00, bp->unk08, 0, which);
            }
        }
    } else if (bp->unk00) {
        SetBounce(car, bp->unk00, bp->unk08, 0, which);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", UpdateBounce);
#endif

#ifdef NON_MATCHING
void carTakeHit(s32 id, s32 amount, s32 a, s32 flag)
{
    u8 rgb[3];
    s8 isPlayer;
    u16 idx;
    u8 canHit;
    CarWeap* wp;
    s32 fields;
    s32 k;
    s32 rate;
    s32 n;
    s32 d;
    s32 mode;
    s32 dif;
    s32 ua;
    Cs* cs;
    Car* info;
    Car* car;
    CarAlt* alt;
    u8* f;
    CarStats* st;
    u8 enabled;

    if (amount >= 3) {
        fields = (s16)GetFieldsLastFrame();
        k = 16;
    } else {
        fields = (s16)GetFieldsLastFrame();
        k = 6;
    }
    rate = k / fields;

    if (flag & 0xFF) {
        rgb[0] = 0xFF;
        rgb[1] = 0xFF;
        rgb[2] = 0xFF;
    } else {
        rgb[0] = 0xFF;
        rgb[1] = 0;
        rgb[2] = 0;
    }

    if ((u8)uaIsCarMatID((s16)id, &isPlayer, &idx)) {
        if (isPlayer) {
            info = GetPlayerInfo(idx);
            canHit = 1;
            car = GetPlayerInfo(idx);
            mode = car->unkF4;
            st = &car->stats;
            f = car->flags;
            wp = &car->weap;
            cs = GetPlayerCs3D(idx);
            enabled = 1;
            ua = car->uaIndex;
        } else {
            info = (Car*)GetAICarInfo(idx);
            alt = GetAICarInfo(idx);
            wp = &alt->weap;
            cs = GetAICs3D(idx);
            enabled = alt->unk40;
            canHit = alt->unk46;
            mode = 3;
            st = &alt->stats;
            f = alt->flags;
            ua = alt->uaIndex;
        }

        if (enabled && !f[0x16] && !uaHasPlayerBeatThisLevel()) {
            if (mode == 1) {
                SetFXSheet(rate, rgb[0], rgb[1], rgb[2]);
            } else if (amount != -1 && !f[0x10]) {
                UAeffectActHitLights(cs, rate, rgb);
            }

            if (amount >= 0) {
                if (amount >= 8 && (rand() & 7) < amount - 7 && !f[0x1C]) {
                    InitBombDamage(info, isPlayer, 1, flag);
                }
                if (*(s32*)&st->lostTimer) {
                    amount = amount / 2;
                }
                n = rand();
                d = amount / 3;
                if (d >= 2) {
                    if (d < 8) {
                        k = n % d;
                    } else {
                        k = n % 8;
                    }
                } else {
                    k = n % 2;
                }
                explodeCreateFragments(k, (VECTOR*)&cs->pos);
                if (st->unk4C || !isPlayer) {
                    if (isPlayer) {
                        dif = uaGetDifficulty();
                        if (dif == 0) {
                            amount = (s32)((u32)amount * 2u) / 3;
                        }
                        if (dif == 2) {
                            amount = (s32)((u32)amount * 4u) / 3;
                        }
                        if (amount <= 0) {
                            amount = 1;
                        }
                    }
                    st->unk40 = (s32)((u32)st->unk40 - (u32)amount);
                }
                if (f[0x10] && amount >= 6 && !f[0]) {
                    st->unk18 = 0;
                }
            } else {
                switch (amount) {
                case -1:
                    if (!f[0x10]) {
                        f[0x10] = 1;
                        st->unk18 = 0xB4;
                        UAeffectSetFreezeLight(cs);
                    } else {
                        st->unk18 = 0xB4;
                    }
                    break;
                case -2:
                    if (ua != 0x82 && canHit) {
                        f[0x12] = 1;
                        f[0x0D] = 1;
                        st->unk1C = 0xC8;
                    }
                    break;
                case -3:
                    f[0x11] = 1;
                    st->spikeTimer = 0xC8;
                    st->unkFC = st->unkFC / 2;
                    RecomputeCarDeltas(st);
                    break;
                case -4:
                    if (ua != 0x82) {
                        InitCatapult(info, isPlayer);
                        break;
                    }

                case -5:
                    InitBombDamage(info, isPlayer, 1, flag);
                    break;
                case -6:
                    wp->unk42 = 1;
                    break;
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", carTakeHit);
#endif

#ifdef NON_MATCHING
void InitCatapult(Car* car, u8 which)
{
    CarTire* t;
    u8* f;
    CarMotion* m;
    s16 v;

    if (which) {
        t = car->tires;
        f = car->flags;
        m = &car->motion;
    } else {
        t = ((CarAlt*)car)->tires;
        f = ((CarAlt*)car)->flags;
        m = &((CarAlt*)car)->motion;
    }

    if (f[0xB] == 0) {
        if (t[0].unk6 || t[1].unk6 || t[2].unk6 || t[3].unk6) {
            v = 15;
        } else {
            v = 30;
        }
        t[0].unk14 = v;
        t[1].unk14 = v;
        t[2].unk14 = v;
        t[3].unk14 = v;
        if (GetFrameCount() & 1) {
            t[0].unk12 -= 2;
        } else {
            t[1].unk12 -= 2;
        }
        m->vel.y = m->vel.y * 3 / 2;
        t[0].unk3 = 1;
        t[1].unk3 = 1;
        t[2].unk3 = 1;
        t[3].unk3 = 1;
        t[0].catapulted = 1;
        t[1].catapulted = 1;
        t[2].catapulted = 1;
        t[3].catapulted = 1;
        t[0].unk0 = 1;
        t[1].unk0 = 1;
        t[2].unk0 = 1;
        t[3].unk0 = 1;
        f[0xF] = 1;
        f[0xB] = 1;
        f[1] = 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", InitCatapult);
#endif

void CheckCatapults(Car* car, u8 which)
{
    CarTire* t;
    u8* f;

    if (which) {
        t = car->tires;
        f = car->flags;
    } else {
        t = ((CarAlt*)car)->tires;
        f = ((CarAlt*)car)->flags;
    }

    f[0xF] = 0;
    if (t[0].catapulted) {
        f[0xF] = 1;
        UpdateCatapultedTire(&t[0]);
    }
    if (t[1].catapulted) {
        f[0xF] = 1;
        UpdateCatapultedTire(&t[1]);
    }
    if (t[2].catapulted) {
        f[0xF] = 1;
        UpdateCatapultedTire(&t[2]);
    }
    if (t[3].catapulted) {
        f[0xF] = 1;
        UpdateCatapultedTire(&t[3]);
    }
}

void UpdateCatapultedTire(CarTire* t)
{
    if (!t->unk3) {
        t->catapulted = 0;
        t->unk1C = 0;
    }
}

#ifdef NON_MATCHING
void InitBombDamage(Car* car, u8 which, s8 full, u8 noMotion)
{
    CarTire* t;
    CarMotion* m;
    CarStats* st;
    u8* f;
    s32* dmg;
    s32 pct;
    s32 v;

    if (which) {
        t = car->tires;
        pct = car->stats.unk104;
        f = car->flags;
        m = &car->motion;
        st = &car->stats;
        dmg = &car->motion.unkD4;
    } else {
        t = ((CarAlt*)car)->tires;
        pct = ((CarAlt*)car)->stats.unk104;
        f = ((CarAlt*)car)->flags;
        m = &((CarAlt*)car)->motion;
        st = &((CarAlt*)car)->stats;
        dmg = &((CarAlt*)car)->motion.unkD4;
    }

    if (f[11]) {
        return;
    }

    v = (100 - pct) * 28 / 100;
    if (v < 10) {
        v = 10;
    }
    if (!full) {
        v = v / 3;
    }
    if (t[0].unk6 || t[1].unk6 || t[2].unk6 || t[3].unk6) {
        v = 10;
    }
    t[0].unk14 = v;
    t[1].unk14 = v;
    t[2].unk14 = v;
    t[3].unk14 = v;

    switch (rand() & 7) {
    case 0:
        t[0].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z -= 512;
            m->vel.y -= m->vel.y / 4;
        }
        *dmg = -st->unkD0;
        break;
    case 1:
        t[2].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z -= 113;
            m->vel.y -= m->vel.y / 2;
        }
        *dmg = -st->unkD0 / 4;
        break;
    case 2:
        t[1].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z += 512;
            m->vel.y -= m->vel.y / 4;
        }
        *dmg = st->unkD0;
        break;
    case 3:
        t[1].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z += 1024;
            m->vel.y -= m->vel.y / 2;
        }
        *dmg = st->unkD0 * 2;
        break;
    case 4:
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z += 1536;
            m->vel.y -= m->vel.y / 4;
        }
        *dmg = st->unkD0;
        break;
    case 5:
        t[2].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z += 113;
        }
        *dmg = st->unkD0 / 4;
        break;
    case 6:
        t[2].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z -= 1536;
            m->vel.y -= m->vel.y / 4;
        }
        *dmg = -st->unkD0;
        break;
    case 7:
    default:
        t[0].unk12 -= 2;
        t[2].unk12 -= 2;
        if (!noMotion) {
            m->rot2.z -= 1024;
            m->vel.y -= m->vel.y / 2;
        }
        *dmg = -(st->unkD0 * 2);
        break;
    }

    t[0].unk3 = 1;
    t[1].unk3 = 1;
    t[2].unk3 = 1;
    t[3].unk3 = 1;
    t[0].bombDamaged = 1;
    t[1].bombDamaged = 1;
    t[2].bombDamaged = 1;
    t[3].bombDamaged = 1;
    t[0].unk0 = 1;
    t[1].unk0 = 1;
    t[2].unk0 = 1;
    t[3].unk0 = 1;
    f[11] = 1;
    f[1] = 1;
    if (!full) {
        *dmg = *dmg / 3;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", InitBombDamage);
#endif

#ifdef NON_MATCHING
void CheckBombDamage(Car* car, u8 which)
{
    CarTire* t;
    u8* f;
    CarMotion* m;
    s32* p;
    u8 was;

    if (which) {
        t = car->tires;
        m = &car->motion;
        f = car->flags;
        p = &car->motion.unkD4;
    } else {
        t = ((CarAlt*)car)->tires;
        m = &((CarAlt*)car)->motion;
        f = ((CarAlt*)car)->flags;
        p = &((CarAlt*)car)->motion.unkD4;
    }

    was = f[0x13];
    f[0x13] = 0;
    if (t[0].bombDamaged) {
        f[0x13] = 1;
        UpdateBombDamagedTire(&t[0], m);
    }
    if (t[1].bombDamaged) {
        f[0x13] = 1;
        UpdateBombDamagedTire(&t[1], m);
    }
    if (t[2].bombDamaged) {
        f[0x13] = 1;
        UpdateBombDamagedTire(&t[2], m);
    }
    if (t[3].bombDamaged) {
        f[0x13] = 1;
        UpdateBombDamagedTire(&t[3], m);
    }
    if (f[0x13]) {
        m->rotDelta.z += *p;
        if (was) {
            SetNoCarDrift(m, f);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CheckBombDamage);
#endif

void UpdateBombDamagedTire(CarTire* t, CarMotion* m)
{
    if (!t->unk3) {
        t->bombDamaged = 0;
        t->unk1E = 0;
    }
}

#ifdef NON_MATCHING
void CheckSpikeDamage(Car* car, u8 which)
{
    CarStats* s;
    u8* f;

    if (which) {
        s = &car->stats;
        f = car->flags;
    } else {
        s = &((CarAlt*)car)->stats;
        f = ((CarAlt*)car)->flags;
    }

    if (f[0x11] && s->spikeTimer) {
        s->spikeTimer -= GetFieldsLastFrame();
        if ((s16)s->spikeTimer <= 0) {
            f[0x11] = 0;
            s->spikeTimer = 0;
            s->unkFC = s->unk100;
            RecomputeCarDeltas(s);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", CheckSpikeDamage);
#endif

#ifdef NON_MATCHING
void InitMonsterSmash(Car* carA, u8 whichA, Car* carB, u8 whichB, s16 unused_mode)
{
    u8* fa;
    CarStats* sa;
    CarWeap* wa;
    u8* fb;
    CarStats* sb;
    CarMotion* mb;
    u16 idx;
    s32 uaIdx;
    Cs* cs;
    s32 dmg;
    s32 v;
    s32 qs;

    (void)unused_mode;
    if (whichA) {
        fa = carA->flags;
        sa = &carA->stats;
        wa = &carA->weap;
    } else {
        fa = ((CarAlt*)carA)->flags;
        sa = &((CarAlt*)carA)->stats;
        wa = &((CarAlt*)carA)->weap;
    }

    if (whichB) {
        fb = carB->flags;
        sb = &carB->stats;
        mb = &carB->motion;
        idx = carB->playerIdx;
        uaIdx = carB->uaIndex;
    } else {
        fb = ((CarAlt*)carB)->flags;
        sb = &((CarAlt*)carB)->stats;
        mb = &((CarAlt*)carB)->motion;
        idx = ((CarAlt*)carB)->playerIdx;
        uaIdx = ((CarAlt*)carB)->uaIndex;
    }

    if ((s16)wa->ammo[11] < getSpecialWeaponCost(0x28)) {
        return;
    }
    if (fa[27]) {
        return;
    }

    uasoundPlayCarSpecialWeaponLaunchOrInflight(5, 0, 0);
    wa->ammo[11] = wa->ammo[11] - getSpecialWeaponCost(0x28);
    if (whichB) {
        cs = GetPlayerCs3D((s16)idx);
    } else {
        cs = GetAICs3D((s16)idx);
    }
    dmg = (s16)cs->unkC0;
    carTakeHit(dmg, carweapGetMonsterDamage(), 0, 0);
    fb[16] = 1;
    sb->unk18 = 60;
    mb->vel.y = 0;
    fa[27] = 1;
    fb[28] = 1;
    sa->unk30 = uaIdx;
    if (sb->unk84 < sb->unk80) {
        v = sb->unk80;
    } else {
        v = sb->unk84;
    }
    sa->unk2A = v;
    sa->unk2A += sa->unk78 / 2;
    sa->unk2C = sb->unk6C + sb->unk70;
    if (sa->unk2C > 80) {
        sa->unk2C = 80;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", InitMonsterSmash);
#endif

#ifdef NON_MATCHING
void SetCarVRMode(Car* car, u32 mode)
{
    SVECTOR rot;
    VECTOR pos;
    VECTOR base;
    VECTOR off;
    u8 moved;

    if (mode < 5) {
        car->unkF8 = mode;
    }
    moved = 0;
    if (mode < 4) {
        if ((u32)car->unkF4 >= 4) {
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vx = 0;
            pos.vy = 0;
            pos.vz = 0;
            moved = 1;
            UASetCameraPosition((s16)car->playerIdx, (VEC3*)&pos, &rot);
        }
    }

    switch (mode - 1) {
    case 0:
        if (moved) {
            car->dTrans.x = car->vrPos[0].x;
            car->dTrans.y = car->vrPos[0].y;
            car->dTrans.z = car->vrPos[0].z;
            car->dRot.vx = car->vrRot[0].vx;
            car->dRot.vy = car->vrRot[0].vy;
            car->dRot.vz = car->vrRot[0].vz;
        }
        break;
    case 1:
        if (car->stats.unk3C) {
            uaDrawCar((s16)car->playerIdx, 1);
        } else {
            uaDontDrawCar((s16)car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        if (moved) {
            car->dTrans.x = car->vrPos[1].x;
            car->dTrans.y = car->vrPos[1].y;
            car->dTrans.z = car->vrPos[1].z;
            car->dRot.vx = car->vrRot[1].vx;
            car->dRot.vy = car->vrRot[1].vy;
            car->dRot.vz = car->vrRot[1].vz;
        }
        break;
    case 2:
        if (car->stats.unk3C) {
            uaDrawCar((s16)car->playerIdx, 1);
        } else {
            uaDontDrawCar((s16)car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        if (moved) {
            car->dTrans.x = car->vrPos[2].x;
            car->dTrans.y = car->vrPos[2].y;
            car->dTrans.z = car->vrPos[2].z;
            car->dRot.vx = car->vrRot[2].vx;
            car->dRot.vy = car->vrRot[2].vy;
            car->dRot.vz = car->vrRot[2].vz;
        }
        break;
    case 3:
        if (car->stats.unk3C) {
            uaDrawCar((s16)car->playerIdx, 1);
        } else {
            uaDontDrawCar((s16)car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        car->dTrans.x = 0;
        car->dTrans.y = 0;
        car->dTrans.z = 0;
        car->dRot.vx = 0;
        car->dRot.vy = 0;
        car->dRot.vz = 0;
        base.vx = car->vrPos[3].x;
        base.vy = car->vrPos[3].y;
        base.vz = car->vrPos[3].z;
        mathMulTransVec(&car->motion.mat, &base, &off);
        base.vx = car->motion.pos.x + off.vx;
        base.vy = car->motion.pos.y + off.vy;
        base.vz = car->motion.pos.z + off.vz;
        UASetCameraPosition((s16)car->playerIdx, (VEC3*)&base, &car->vrRot[3]);
        break;
    case 4:
        if (car->stats.unk3C) {
            uaDrawCar((s16)car->playerIdx, 1);
        } else {
            uaDontDrawCar((s16)car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        car->dTrans.x = 0;
        car->dTrans.y = 0;
        car->dTrans.z = 0;
        car->dRot.vx = 0;
        car->dRot.vy = 0;
        car->dRot.vz = 0;
        break;
    default:
        sdk_memcpy(&car->dTrans, &car->vrPos[0], 12);
        sdk_memcpy(&car->dRot, &car->vrRot[0], 8);
        uaDontDrawCar((s16)car->playerIdx, 1);
        UAdashSetDashboardDrawFlag(1);
        break;
    }

    viewSetDeltaTrans(&car->dTrans, (s16)car->playerIdx);
    viewSetDeltaRot(&car->dRot, (s16)car->playerIdx);
    car->unkF4 = mode;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", SetCarVRMode);
#endif

#ifdef NON_MATCHING

void UpdateCarDeath(Car* car, u8 which)
{
    CarStats* st;
    CarMotion* m;
    u8* f;
    CarTire* t;
    s32 limit;
    u32 period;
    VEC3 pos;
    VEC3 off;
    s32 ang;
    s32 target;

    if (which) {
        st = &car->stats;
        m = &car->motion;
        f = car->flags;
        t = car->tires;
    } else {
        st = &((CarAlt*)car)->stats;
        m = &((CarAlt*)car)->motion;
        f = ((CarAlt*)car)->flags;
        t = ((CarAlt*)car)->tires;
    }

    if ((s32)((u32)st->deathTimer << 16) > 0) {
        st->deathTimer -= GetFieldsLastFrame();
        if ((s16)((s16)st->deathTimer % 40) == 1) {
            pos.x = m->pos.x;
            pos.y = m->pos.y;
            pos.z = m->pos.z;
            pos.z = pos.z - (st->unk6C / 2);
            do_smoke(&pos);
        }
        if (f[21]) {
            if (m->rot.y >= -1023) {
                m->rotDelta.y = m->rotDelta.y - (st->unkCC - ((st->unkD0 * m->rot.y) / 1024));
                m->vel.y /= 2;
            } else {
                m->rotDelta.y = 0;
            }
        } else if (m->rot.y < 1024) {
            m->rotDelta.y = m->rotDelta.y + (st->unkCC + ((st->unkD0 * m->rot.y) / 1024));
            m->vel.y /= 2;
        } else {
            m->rotDelta.y = 0;
        }
        if (f[20]) {
            if (m->vel.y < st->unkA4) {
                pos.x = m->pos.x;
                pos.y = m->pos.y;
                pos.z = m->pos.z;
                pos.z = (s16)t->unk24;
                do_flames(&pos);
                f[20] = 0;
            }
        } else {
            limit = 3;
            if (shellGetCurrentLevel() == 5 && m->pos.z < 400) {
                limit = 7;
            }
            if ((s16)st->blasts < limit) {
                if ((s16)st->blasts < limit) {
                    period = 4;
                } else {
                    period = 12;
                }
                if (GetFrameCount() % period == 0 && (rand() & 1)) {
                    st->blasts++;
                    f[26] = 1;
                    if (limit == 7) {
                        off.x = (rand() & 0x3F) - 32;
                        off.y = (rand() & 0x3F) - 32;
                        off.z = (rand() & 0xF) - 8;
                        pos.x = m->pos.x;
                        pos.y = m->pos.y;
                        pos.z = m->pos.z;
                        pos.x = pos.x + off.x;
                        pos.y = pos.y + off.y;
                        pos.z = pos.z + off.z;
                        if (!(rand() & 1)) {
                            do_mondo_explosion(&pos);
                        } else {
                            do_bigger_explosion(&pos);
                        }
                    } else {
                        off.x = (rand() & 0x7F) - 64;
                        off.y = (rand() & 0x7F) - 64;
                        off.z = (rand() & 0x20) - 16;
                        pos.x = m->pos.x;
                        pos.y = m->pos.y;
                        pos.z = m->pos.z;
                        pos.x = pos.x + off.x;
                        pos.y = pos.y + off.y;
                        pos.z = pos.z + off.z;
                        pos.x = m->pos.x;
                        pos.y = m->pos.y;
                        pos.z = m->pos.z;
                        if (rand() & 1) {
                            do_big_explosion(&pos);
                        } else {
                            do_bigger_explosion(&pos);
                        }
                    }
                }
            }
        }
    } else {
        m->rotDelta.x = 0;
        m->rotDelta.y = 0;
        m->rotDelta.z = 0;
        m->vel.x = 0;
        m->vel.y -= m->vel.y / 16;
        m->vel.z = 0;

        if (m->rot.y >= 1138) {
            if (m->rot.y >= 1934) {
                return;
            }
            if (m->rot.y < 1536) {
                ang = m->rot.y;
                target = 1024;
            } else {
                ang = m->rot.y;
                target = 2036;
            }
        } else if (m->rot.y >= 114) {
            if (m->rot.y >= 910) {
                return;
            }
            if (m->rot.y < 512) {
                ang = m->rot.y;
                target = 0;
            } else {
                ang = m->rot.y;
                target = 1024;
            }
        } else if (m->rot.y < -113) {
            if (m->rot.y < -909) {
                return;
            }
            if (m->rot.y >= -511) {
                ang = m->rot.y;
                target = 0;
            } else {
                ang = m->rot.y;
                target = 1024;
            }
        } else if (m->rot.y < -1137) {
            if (m->rot.y < -1933) {
                return;
            }
            if (m->rot.y >= -1535) {
                ang = m->rot.y;
                target = -1536;
            } else {
                ang = m->rot.y;
                target = -2036;
            }
        } else {
            return;
        }
        m->rot.y = SmoothAngleValue(ang, target, 98);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car", UpdateCarDeath);
#endif

void TermCar(void)
{
}
