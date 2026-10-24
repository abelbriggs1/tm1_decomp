#include "common.h"

#include "tm1/AI_CAR.H"
#include "tm1/CAR.H"
#include "tm1/CARUPD.H"
#include "tm1/CAR_INIT.H"
#include "tm1/CS.H"
#include "tm1/CTLPAD.H"
#include "tm1/DIFFICUL.H"
#include "tm1/EXPLODE.H"
#include "tm1/INTERACT.H"
#include "tm1/LEVELS.H"
#include "tm1/MATH.H"
#include "tm1/POTHOLES.H"
#include "tm1/RT.H"
#include "tm1/SHELL.H"
#include "tm1/SMOOTH.H"
#include "tm1/TIMER.H"
#include "tm1/UA.H"
#include "tm1/UAEFFECT.H"
#include "tm1/UA_DASH.H"
#include "tm1/UA_SOUND.H"
#include "tm1/UA_SW.H"
#include "tm1/VIEW.H"
#include "tm1/WEAPON.H"
#include <ABS.H>
#include <LIBGTE.H>
#include <MEMORY.H>
#include <RAND.H>

void CarUpdate(PlayerCar* car)
{
    s16 idx;
    s16 rate;
    s32 speed;
    s32 rot;

    idx = car->playerIdx;
    RecomputeCarDeltas(&car->stats);
    rate = GetUpdateRate();
    if (car->stats.updateRate != rate) {
        car->motion.vel.vy = car->motion.vel.vy * car->stats.updateRate / rate;
        car->motion.rotDelta.vz = car->motion.rotDelta.vz * car->stats.updateRate / rate;
        car->stats.updateRate = rate;
    }
    car->collision.unk12 = 0;
    car->flags[0x09] = 0;
    *(s16*)&car->flags[0x1E] = GetClosestTriggerPt(car, 1);
    if (car->flags[0x10]) {
        car->stats.unk18 -= GetFieldsLastFrame();
        if (car->stats.unk18 << 16 <= 0) {
            car->flags[0x10] = 0;
            UAeffectClearFreezeLight(GetPlayerCs3D(car->playerIdx));
        }
    }
    if (car->flags[0x19]) {
        if (car->flags[0x0A] || car->flags[0x17] || car->flags[0x00]
            || shellGetCurrentLevel() == LEVEL6_ROOFTOPS) {
            car->flags[0x19] = 0;
            uaswSetCarShadow(car->uaIndex, 0);
        }
    } else if (!car->flags[0x0A] && !car->flags[0x17] && !car->flags[0x00]
        && shellGetCurrentLevel() != LEVEL6_ROOFTOPS) {
        car->flags[0x19] = 1;
        uaswSetCarShadow(car->uaIndex, 1);
    }
    speed = car->motion.vel.vy;
    UpdateBearing(car, 1);
    CarUpdateDeltas(car, 1);
    if (car->flags[0x0D]) {
        UpdateCarOnSlickSpot(car, 1);
    } else {
        CarCheckDynamics(car);
    }
    car->speedDelta = car->motion.vel.vy - speed;
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
    UAeffectUpdateCarSpeed(car->uaIndex, car->motion.vel.vy);
    CarRotUpdate(car, 1);
    CarTransUpdate(car, 1);
    if (car->stats.unk38) {
        CheckHitDetection(car, 1);
    }
    UpdateWeapons(car, 1);
    if (idx == GetPlayerTheCameraFollows() || rtIsSplitScreenOn()) {
        CarViewUpdate(car);
    }
    if (GetFieldsLastFrame() * 760 / 100 * 32 < car->motion.vel.vy) {
        rot = abs(car->motion.rotDelta.vz);
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
            if (car->stats.cheatTimer >= 31) {
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
                    if (car->stats.lostTimer) {
                        car->stats.lostTimer = 0;
                    } else {
                        car->stats.lostTimer = 1;
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
            if (car->stats.cheatTimer >= 31) {
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
        if (car->stats.cheatTimer >= 31) {
            car->unk9A = 0;
        } else if (ctlpadSpecial(4, car->playerIdx)) {
            car->unk9B = 0;
        }
    }
}

void CarUpdateDeltas(Car* car, u8 isPlayer)
{
    u8* input;
    u8* flags;
    CarStats* st;
    CarMotion* m;
    CarWeap* weap;
    AICar* alt;
    s32 maxTurn;
    s32 maxSpeed;
    s32 acceleration;
    s32 amount;
    s32 threshold;

    if (isPlayer) {
        input = PLAYER_CAR(car)->skid;
        flags = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        weap = &PLAYER_CAR(car)->weap;
    } else {
        alt = AI_CAR(car);
        input = alt->skid;
        flags = alt->flags;
        st = &alt->stats;
        m = &alt->motion;
        weap = &alt->weap;
    }
    maxTurn = input[6] || input[7] ? (st->unkD4 * st->unkE8) : st->unkD4;
    maxSpeed = m->vel.vy < st->unkA8 ? st->unkA8 : m->vel.vy;
    acceleration = st->unkA4;
    if (flags[16]) {
        input[2] = 0;
        input[3] = 0;
    }
    if (input[3] || input[2]) {
        if (abs(m->vel.vy) > GetFieldsLastFrame() * 1520 / 100 * 32)
            amount = st->unkCC;
        else {
            threshold = (((GetFieldsLastFrame() * 1520) / 100) << 5);
            amount = st->unkD0 - ((m->vel.vy * (st->unkD0 - st->unkCC)) / threshold);
        }
        if ((input[3] && m->unk00 <= 0) || (input[2] && m->unk00 >= 0))
            amount = amount * 2;
        if (input[6] || input[7])
            amount = amount * st->unkE8;
        if (input[3]) {
            m->unk00 = m->unk00 + amount;
            if (maxTurn < m->unk00)
                m->unk00 = maxTurn;
        } else {
            m->unk00 = m->unk00 - amount;
            if (m->unk00 < -maxTurn)
                m->unk00 = -maxTurn;
        }
    } else if (m->unk00) {
        if (abs(m->unk00) < st->unkD0)
            m->unk00 = 0;
        else if (flags[11])
            m->unk00 -= m->unk00 / 16;
        else
            m->unk00 = m->unk00 / 2;
    }
    m->rotDelta.vz = ((m->vel.vy / 32) * (m->unk00)) / st->unk7C;
    if (flags[16]) {
        input[5] = 1;
        input[4] = 0;
        flags[5] = 0;
    } else if (isPlayer) {
        if (input[4]) {
            if (input[1]) {
                if (m->vel.vy > st->unkBC) {
                    input[4] = 0;
                    if (!input[16])
                        input[5] = 1;
                    flags[5] = 1;
                    st->unk14 = 0;
                } else {
                    flags[7] = 1;
                    input[5] = 0;
                }
            } else if (m->vel.vy < -st->unkBC) {
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
        if (m->vel.vy) {
            if (abs(m->vel.vy) < st->unkA4) {
                m->vel.vy = 0;
                st->unk14 = 0;
            }
        } else {
            st->unk14 = st->unk14 + GetFieldsLastFrame();
            m->vel.vy = 0;
            if (st->unk14 >= 22)
                flags[5] = 0;
        }
    }
    if (input[8] && flags[16]) {
        if (st->unkF8 >= 91)
            goto consume_turbo;
        if (weap->unk46 > 0) {
            --weap->unk46;
            st->unkF8 += 180;
        consume_turbo:
            st->unkF8 -= 90;
            st->unk18 = 0;
        } else {
            st->unkF8 = 0;
            input[8] = 0;
        }
    }
    if (input[8]) {
        input[4] = 1;
        if (st->unkF8 <= 0 && weap->unk46 > 0) {
            --weap->unk46;
            st->unkF8 = 180;
        }
        if (st->unkF8 > 0) {
            maxSpeed = (st->unkA8 * 3) / 2;
            acceleration = st->unkA4 * 4;
            st->unkF8 = st->unkF8 - (GetFieldsLastFrame());
            if (isPlayer)
                uasoundPlayTurboBoost(0, 0);
        } else {
            maxSpeed = st->unkA8;
            acceleration = st->unkA4;
            st->unkF8 = 0;
            if (m->vel.vy > st->unkA8)
                input[4] = 0;
            if (isPlayer)
                uasoundStopTurboBoost();
        }
    } else {
        if (isPlayer)
            uasoundStopTurboBoost();
        if (m->vel.vy > st->unkA8)
            input[4] = 0;
    }
    if (input[4]) {
        flags[4] = 1;
        flags[8] = 0;
        if (((flags[7] && -st->unkB8 < m->vel.vy) || (!flags[7] && m->vel.vy < maxSpeed))
            && !flags[1]) {
            if (flags[13])
                amount = st->unkA0 / 2;
            else {
                amount = acceleration;
                if ((flags[7] && m->vel.vy < 0) || (!flags[7] && m->vel.vy > 0))
                    amount = amount - (((acceleration - st->unkA0) * abs(m->vel.vy)) / maxSpeed);
            }
            if (flags[7])
                m->vel.vy = (m->vel.vy - amount);
            else
                m->vel.vy = (m->vel.vy + amount);
        }
    } else
        flags[4] = 0;
    if (m->vel.vy && !flags[4] && !flags[8]) {
        amount = (st->unkBC - st->unkA0) * abs(m->vel.vy) / maxSpeed;
        amount /= 2;
        if (amount < st->unkA0)
            amount = st->unkA0;
        if (flags[13])
            amount /= 4;
        if (m->vel.vy > 0) {
            m->vel.vy = m->vel.vy - amount;
            if (m->vel.vy < 0)
                m->vel.vy = 0;
        } else {
            m->vel.vy = m->vel.vy + amount;
            if (m->vel.vy > 0)
                m->vel.vy = 0;
        }
    }
    if (input[1]) {
        if (m->vel.vy <= 0) {
            flags[7] = 1;
            if (-st->unkB8 < m->vel.vy) {
                flags[4] = 0;
                flags[8] = 0;
            }
        }
    } else
        flags[7] = 0;
    if (input[5] && m->vel.vy && !input[16]) {
        flags[3] = 1;
        amount = st->unkBC;
        if (flags[13])
            amount /= 4;
        if (m->vel.vy < 0) {
            m->vel.vy = m->vel.vy + amount;
            if (m->vel.vy > 0)
                m->vel.vy = 0;
        } else {
            m->vel.vy = m->vel.vy - amount;
            if (m->vel.vy < 0)
                m->vel.vy = 0;
        }
    } else
        flags[3] = 0;
}

void CarUpdateControlPad(PlayerCar* car)
{
    s16 idx;
    u8 turbo;
    u8 a;
    u8 b;
    u8 v;

    idx = car->playerIdx;
    turbo = 0;
    if (car->tires[0].unk3 && car->tires[1].unk3 && car->tires[2].unk3) {
        turbo = car->tires[3].unk3 != 0;
    }

    car->skid[0x0D] = GetCtlPad((s8)car->skid[0x26], idx);
    car->skid[0x0E] = GetCtlPad((s8)car->skid[0x27], idx);

    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x18], idx) != 0;
    }
    car->skid[0x11] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1A], idx) != 0;
    }
    car->skid[0x12] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1C], idx) != 0;
    }
    car->skid[0x13] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x1B], idx) != 0;
    }
    car->skid[0x14] = v;
    v = 0;
    if (car->skid[0x0D]) {
        v = GetCtlPad((s8)car->skid[0x28], idx) != 0;
    }
    car->skid[0x17] = v;

    car->skid[0x03] = GetCtlPad((s8)car->skid[0x1C], idx);
    car->skid[0x02] = GetCtlPad((s8)car->skid[0x1B], idx);

    if (car->tires[0].unk3) {
        if (car->tires[1].unk3) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
        } else if (GetFrameCount() & 1) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
        }
    } else if (car->tires[1].unk3) {
        if (GetFrameCount() & 1) {
            car->skid[0x03] = 0;
            car->skid[0x02] = 0;
        }
    }

    a = 0;
    if (GetCtlPad((s8)car->skid[0x1F], idx)) {
        a = car->skid[0x03] != 0;
    }
    car->skid[0x06] = a;
    a = 0;
    if (GetCtlPad((s8)car->skid[0x20], idx)) {
        a = car->skid[0x02] != 0;
    }
    car->skid[0x07] = a;
    a = 0;
    if (GetCtlPad((s8)car->skid[0x21], idx)) {
        a = turbo ^ 1;
    }
    car->skid[0x08] = a;

    a = 0;
    if (GetCtlPad((s8)car->skid[0x19], idx) || GetCtlPad((s8)car->skid[0x1A], idx)) {
        if (!car->skid[0x0D] && !car->skid[0x0E]) {
            a = 1;
        }
    }
    car->skid[0x01] = a;

    b = 0;
    if ((GetCtlPad((s8)car->skid[0x1D], idx) || GetCtlPad((s8)car->skid[0x18], idx)
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

    car->skid[0x09] = GetCtlPad((s8)car->skid[0x22], idx);
    car->skid[0x0A] = GetCtlPad((s8)car->skid[0x23], idx);
    car->skid[0x0B] = GetCtlPad((s8)car->skid[0x24], idx);
    car->skid[0x0C] = GetCtlPad((s8)car->skid[0x25], idx);

    v = 0;
    if (car->skid[0x03] || car->skid[0x02]) {
        v = 1;
    }
    car->flags[0x06] = v;

    v = 0;
    if ((car->skid[0x06] || car->skid[0x07]) && car->motion.vel.vy > 0 && car->skid[0x01]) {
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

void CarRotUpdate(Car* car, u8 isPlayer)
{
    SVECTOR rot;
    CarMotion* m;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
    } else {
        m = &AI_CAR(car)->motion;
    }

    m->rot.vx += m->rotDelta.vx;
    m->rot.vy += m->rotDelta.vy;
    m->rot.vz += m->rotDelta.vz;
    m->rot2.vx += m->rot2Delta.vx;
    m->rot2.vy += m->rot2Delta.vy;
    m->rot2.vz += m->rot2Delta.vz;
    BoundSAngle(&m->unk00);
    BoundVector(&m->rot.vx);
    BoundVector(&m->rot2.vx);
    rot.vx = m->rot2.vx;
    rot.vy = m->rot2.vy;
    rot.vz = m->rot2.vz;
    RotMatrixYXZ(&rot, &m->mat);
    rot.vx = m->rot.vx;
    rot.vy = m->rot.vy;
    rot.vz = m->rot.vz;
    RotMatrixYXZ(&rot, &m->mat2);
}

void CarTransUpdate(Car* car, u8 isPlayer)
{
    VECTOR v;
    VECTOR out;
    CarMotion* m;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
    } else {
        m = &AI_CAR(car)->motion;
    }

    v.vx = m->vel.vx / 32;
    v.vy = m->vel.vy / 32;
    v.vz = m->vel.vz / 32;
    mathMulTransVec(&m->mat, &v, &out);
    m->pos.vx += out.vx;
    m->pos.vy += out.vy;
    m->pos.vz += out.vz;
}

void CarCheckDynamics(PlayerCar* car)
{
    if (car->flags[1]) {
        UpdateDriftingCar(car);
    } else {
        UpdateNonDriftingCar(car);
    }
}

void CarInitMotion(Car* car, u8 isPlayer)
{
    u8* f;
    CarMotion* m;

    if (isPlayer) {
        f = PLAYER_CAR(car)->flags;
        m = &PLAYER_CAR(car)->motion;
    } else {
        f = AI_CAR(car)->flags;
        m = &AI_CAR(car)->motion;
    }

    f[1] = 0;
    m->rotDelta.vz = 0;
    m->rot2Delta.vz = 0;
    m->rot2.vx = m->rot.vx;
    m->rot2.vy = m->rot.vy;
    m->rot2.vz = m->rot.vz;
}

void UpdateNonDriftingCar(PlayerCar* car)
{
    s32 limit;

    if (car->skid[0x10] && car->motion.vel.vy > car->stats.unkB0) {
        goto drift;
    }
    if (car->motion.vel.vy > car->stats.unkB0) {
        s32 delta;

        delta = car->motion.rotDelta.vz;
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

void SetNoCarDrift(CarMotion* m, u8* f)
{
    f[1] = 0;
    m->rot2Delta.vx = m->rotDelta.vx;
    m->rot2Delta.vy = m->rotDelta.vy;
    m->rot2Delta.vz = m->rotDelta.vz;
    m->rot2.vx = m->rot.vx;
    m->rot2.vy = m->rot.vy;
    m->rot2.vz = m->rot.vz;
}

s32 CalcMaxRotBeforeDrift(Car* car, u8 isPlayer)
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

    if (isPlayer) {
        st = &PLAYER_CAR(car)->stats;
        g = PLAYER_CAR(car)->flags[4];
        m = &PLAYER_CAR(car)->motion;
        sk = PLAYER_CAR(car)->skid;
        hit = 0;
        if (g && PLAYER_CAR(car)->flags[6]
            && (PLAYER_CAR(car)->skid[6] || PLAYER_CAR(car)->skid[7])) {
            hit = 1;
        }
        if (sk[6] || sk[7]) {
            d = st->unkD4 * st->unkE8;
        } else {
            d = st->unkD4;
        }
    } else {
        st = &AI_CAR(car)->stats;
        g = AI_CAR(car)->flags[4];
        m = &AI_CAR(car)->motion;
        sk = AI_CAR(car)->skid;
        hit = 0;
        if (g && AI_CAR(car)->flags[6] && (AI_CAR(car)->skid[6] || AI_CAR(car)->skid[7])) {
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

    if (m->vel.vy < st->unkA8) {
        hi = st->unkA8;
    } else {
        hi = m->vel.vy;
    }
    if (st->unkB0 >= hi) {
        hi = st->unkB0 + 1;
    }

    if (m->vel.vy > st->unkB0) {
        t = (m->vel.vy / 32) * d / st->unk7C;
        if (d < t) {
            t = d;
        }
        return t - (t - st->unkD0) * (abs(m->vel.vy) - st->unkB0) / (hi - st->unkB0);
    }
    return d;
}

void SlowDownNonDriftingCar(Car* car, u8 isPlayer)
{
    CarStats* st;
    CarMotion* m;
    u8 g;
    s32 d;
    s32 hi;

    if (isPlayer) {
        g = PLAYER_CAR(car)->skid[6];
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        if (g || PLAYER_CAR(car)->skid[7]) {
            d = PLAYER_CAR(car)->stats.unkD4 * PLAYER_CAR(car)->stats.unkE8;
        } else {
            d = PLAYER_CAR(car)->stats.unkD4;
        }
    } else {
        g = AI_CAR(car)->skid[6];
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        if (g || AI_CAR(car)->skid[7]) {
            d = AI_CAR(car)->stats.unkD4 * AI_CAR(car)->stats.unkE8;
        } else {
            d = AI_CAR(car)->stats.unkD4 * 2;
        }
    }

    if (m->vel.vy < st->unkA8) {
        hi = st->unkA8;
    } else {
        hi = m->vel.vy;
    }

    if (abs(m->vel.vy) > st->unkB0) {
        d = d - (d - st->unkEC) * abs(m->vel.vy) / hi;
        if (d < abs(m->unk00)) {
            if (m->vel.vy > 0) {
                m->vel.vy = m->vel.vy - st->unkBC;
            } else if (m->vel.vy < 0) {
                m->vel.vy = m->vel.vy + st->unkBC;
            }
        }
    }
}

void UpdateDriftingCar(PlayerCar* car)
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

void SetFullCarDrift(CarMotion* m)
{
    m->rot2Delta.vz = 0;
}

void BringBackDriftingCar(Car* car, u8 isPlayer)
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

    if (isPlayer) {
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        f = PLAYER_CAR(car)->flags;
        sk = PLAYER_CAR(car)->skid;
    } else {
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        f = AI_CAR(car)->flags;
        sk = AI_CAR(car)->skid;
    }

    if (abs(m->vel.vy) < st->unkAC) {
        SetNoCarDrift(m, f);
        return;
    }

    ang = m->rot.vz - m->rot2.vz;
    BoundAngle(&ang);
    if (abs(ang) >= 1366) {
        if (GetFieldsLastFrame() * 380 / 100 * 32 < m->vel.vy) {
            return;
        }
    }

    if (ang > 0) {
        if (sk[3]) {
            turn = 0;
        } else if (sk[2]) {
            turn = GetFieldsLastFrame() * 5;
        } else {
            turn = GetFieldsLastFrame() * 2;
        }
    } else {
        if (sk[3]) {
            turn = GetFieldsLastFrame() * 5;
        } else if (sk[2]) {
            turn = 0;
        } else {
            turn = GetFieldsLastFrame() * 2;
        }
    }

    if (turn > 0) {
        if (m->vel.vy > st->unkB0) {
            extra = 0;
        } else if (st->unkB0 / 2 < m->vel.vy) {
            extra = GetFieldsLastFrame() * 2;
        } else {
            extra = GetFieldsLastFrame() * 5;
        }
    } else {
        extra = 0;
    }

    delta = (extra + turn) / 2 * 4096 / 360;
    if (f[4]) {
        delta = delta * 2;
    }

    r = SmoothAngleValue(m->rot2.vz, m->rot.vz, 90);
    if (m->rot2.vz < r) {
        m->rot2.vz = m->rot2.vz + delta;
        BoundAngle(&m->rot2.vz);
        if (m->rot2.vz < m->rot.vz) {
            return;
        }
    } else {
        m->rot2.vz = m->rot2.vz - delta;
        BoundAngle(&m->rot2.vz);
        if (m->rot2.vz > m->rot.vz) {
            return;
        }
    }
    SetNoCarDrift(m, f);
}

void SlowDownDriftingCar(Car* car, u8 isPlayer)
{
    s32 ang;
    s32 t;
    s32 d;
    u8* f;
    CarStats* st;
    CarMotion* m;
    u8* sk;

    if (isPlayer) {
        f = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        sk = PLAYER_CAR(car)->skid;
    } else {
        f = AI_CAR(car)->flags;
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        sk = AI_CAR(car)->skid;
    }

    ang = m->rot.vz - m->rot2.vz;
    BoundAngle(&ang);
    t = abs(ang);

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

    if (m->vel.vy > 0) {
        m->vel.vy -= d;
        if (m->vel.vy < 0) {
            SetNoCarDrift(m, f);
        }
    } else {
        m->vel.vy += d;
        if (m->vel.vy > 0) {
            SetNoCarDrift(m, f);
        }
    }
}

void UpdateBearing(Car* car, u8 isPlayer)
{
    s32 ang;
    s32 t;
    CarMotion* m;
    s32* bearing;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        bearing = &PLAYER_CAR(car)->bearing;
    } else {
        m = &AI_CAR(car)->motion;
        bearing = &AI_CAR(car)->bearing;
    }

    if (m->vel.vy > 0) {
        ang = m->rot.vz;
    } else {
        ang = m->rot.vz + 2048;
        BoundAngle(&ang);
    }

    t = abs(ang);
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

static inline s32 UpdateCollisionHeading(CarMotion* m, CarCollision* col, s32 angle)
{
    s32 difference;
    s32 result;
    BoundAngle(&angle);
    difference = abs(angle);
    result = 1;
    if (difference < 341) {
        m->rot.vz = SmoothAngleValue(m->rot.vz, (s16)col->unk8, 90);
    } else if (difference < 568) {
        m->rot.vz = SmoothAngleValue(m->rot.vz, (s16)col->unk8, 95);
    } else if (difference < 910) {
        m->rot.vz = SmoothAngleValue(m->rot.vz, (s16)col->unk8, 98);
    } else {
        m->rot.vz = SmoothAngleValue(m->rot.vz, (s16)col->unk8, 99);
    }
    return result;
}
static inline u8 UpdateCollisionAngle(CarMotion* m, CarCollision* col)
{
    u8 result;
    if (col->unk2) {
        result = 1;
        m->rot.vz = SmoothAngleValue(m->rot.vz, (s16)col->unk8, 80);
    } else if (col->unk1 && m->vel.vy) {
        result = UpdateCollisionHeading(m, col, m->rot2.vz - m->rot.vz);
    } else
        result = 1;
    return result;
}
void CheckHitDetection(Car* car, u8 isPlayer)
{
    u8* flags;
    CarStats* st;
    CarCollision* col;
    CarMotion* m;
    CarTire* t;
    Cs* cs;
    HdCsHit hit;
    PotHole* hole;
    s32 bearing;
    s32 speed;
    s32 height;
    s16 type;

    if (isPlayer) {
        flags = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        col = &PLAYER_CAR(car)->collision;
        m = &PLAYER_CAR(car)->motion;
        t = PLAYER_CAR(car)->tires;
        cs = GetPlayerCs3D(PLAYER_CAR(car)->playerIdx);
        bearing = PLAYER_CAR(car)->bearing;
    } else {
        flags = AI_CAR(car)->flags;
        col = &AI_CAR(car)->collision;
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        t = AI_CAR(car)->tires;
        cs = GetAICs3D(AI_CAR(car)->playerIdx);
        bearing = AI_CAR(car)->bearing;
    }
    cs->rot.vx = m->rot.vx;
    cs->rot.vy = m->rot.vy;
    cs->rot.vz = m->rot.vz;
    cs->pos.vx = m->pos.vx;
    cs->pos.vy = m->pos.vy;
    cs->pos.vz = m->pos.vz;
    cs->mat.m[0][0] = m->mat2.m[0][0];
    cs->mat.m[0][1] = m->mat2.m[0][1];
    cs->mat.m[0][2] = m->mat2.m[0][2];
    cs->mat.m[1][0] = m->mat2.m[1][0];
    cs->mat.m[1][1] = m->mat2.m[1][1];
    cs->mat.m[1][2] = m->mat2.m[1][2];
    cs->mat.m[2][0] = m->mat2.m[2][0];
    cs->mat.m[2][1] = m->mat2.m[2][1];
    cs->mat.m[2][2] = m->mat2.m[2][2];
    cs->mat.t[0] = m->mat2.t[0];
    cs->mat.t[1] = m->mat2.t[1];
    cs->mat.t[2] = m->mat2.t[2];
    hit.hit = 0;
    HdCsTest(cs, &hit, 1, &m->lastMat, &m->lastPos.vx);
    if (hit.hit) {
        if (!isPlayer && flags[29] && st->unk22 >= 13)
            hit.hit = 0;
        type = hit.tag0;
        if (UA_IS_PICKUP(type)) {
            carGetPickup(car, isPlayer, type, hit.tag1);
            hit.hit = 0;
        } else if (type == UA_BARRICADE) {
            UAeffectBarricade(UA_BARRICADE, hit.tag1, bearing);
            col->unk12 = abs(m->vel.vy) * 3 * 33
                / (abs(m->vel.vy) < st->unkA8 ? st->unkA8 : abs(m->vel.vy));
            hit.hit = 0;
        } else if (UA_IS_PERSON(type) || type == UA_UNK_1001) {
            bulDispatchDamage(hit.obj->unkC0, type, hit.tag1, 10, &m->rot, 0);
            t[0].unk0A = MakeFakePotHole();
            t[1].unk0A = t[0].unk0A;
            t[2].unk0A = t[0].unk0A;
            t[3].unk0A = t[0].unk0A;
            t[0].unk1 = 1;
            t[1].unk1 = 1;
            t[2].unk1 = 1;
            t[3].unk1 = 1;
            hole = GetPotHoleDat(t[0].unk0A);
            if (abs(m->rot.vz) < 512) {
                hole->x = t[0].unk38 - 16;
                hole->z = t[0].unk3C - 16;
                hole->a = st->unk74 + 32;
                hole->b = m->vel.vy / 4;
            } else if (abs(m->rot.vz) >= 1537) {
                hole->x = t[1].unk38 - 16;
                hole->z = t[1].unk3C - 24;
                hole->a = st->unk74 + 32;
                hole->b = m->vel.vy / 4;
            } else {
                if (m->rot.vz > 0) {
                    hole->x = t[1].unk38 - 16;
                    hole->z = t[1].unk3C - 16;
                } else {
                    hole->x = t[0].unk38 - 24;
                    hole->z = t[0].unk3C - 16;
                }
                hole->a = m->vel.vy / 4;
                hole->b = st->unk74 + 32;
            }
            hole->c = 40;
            hit.hit = 0;
        } else if (type == UA_UNK_1400) {
            t[0].unk14 = -20;
            t[1].unk14 = -20;
            t[2].unk14 = -20;
            t[3].unk14 = -20;
            hit.hit = 0;
            m->vel.vy = 0;
        } else if (type == UA_UNK_500) {
            height = 160;
            if (!flags[10]) {
                speed = m->vel.vy / 32;
                height = st->unk5C + abs(speed);
            }
            if (flags[19] || flags[15]) {
                t[0].unk14 = -20;
                t[1].unk14 = -20;
                t[2].unk14 = -20;
                t[3].unk14 = -20;
                hit.hit = 0;
                m->vel.vy = 0;
            }
            switch (hit.histIdx) {
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
                hit.hit = 0;
        } else if (type == UA_UNK_1021) {
            if (!isPlayer && !AI_CAR(car)->unk16E && !flags[0]) {
                if (AI_CAR(car)->unk34 < UA_UNK_800 && AI_CAR(car)->uaIndex != UA_BOSS_CAR) {
                    if (col->unkE < 100 || flags[13] || flags[15])
                        hit.hit = 0;
                }
            } else
                hit.hit = 0;
        }
    }
    col->unk4 = hit.hit;
    if (hit.hit) {
        col->unk0 = 1;
        col->unk14++;
        col->unk16 = 0;
        if (hit.histIdx < cs->nhist)
            CalcHitDynamics(car, &hit, isPlayer);
        else
            col->unk0 = 0;
    } else {
        col->unk16++;
        if (col->count <= 0)
            col->unk14 = 0;
    }
    if (col->count) {
        if (isPlayer && flags[6]) {
            if (col->count < 120)
                InitNoCollision(col);
            else
                m->rotDelta.vz = 0;
        }
        if (UpdateCollisionAngle(m, col)) {

            col->count = col->count - GetFieldsLastFrame();
            if (col->count <= 0 || abs(m->vel.vy) < st->unkA4 || abs(col->unk8 - m->rot.vz) < 56)
                InitNoCollision(col);
        } else {
            InitNoCollision(col);
        }
    }
    if (col->unkE < 151)
        col->unkE = col->unkE + GetFieldsLastFrame();
}

void CalcHitDynamics(Car* car, HdCsHit* hit, u8 isPlayer)
{
    CarStats* st;
    CarMotion* m;
    CarTire* tires;
    s16 idx;
    s32 mag;
    s32 dmg;
    s32 num;
    s32 den;
    s32 scaledDamage;
    SVECTOR rot;
    VECTOR delta;
    VECTOR out;
    VECTOR3 spark;
    MATRIX mat;
    s32 ang;
    s32 rel;
    s32 bounce;
    u8 hitCs;
    u8* f;
    CarCollision* col;

    if (isPlayer) {
        f = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        col = &PLAYER_CAR(car)->collision;
        m = &PLAYER_CAR(car)->motion;
        idx = PLAYER_CAR(car)->playerIdx;
        tires = PLAYER_CAR(car)->tires;
    } else {
        f = AI_CAR(car)->flags;
        col = &AI_CAR(car)->collision;
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        idx = AI_CAR(car)->playerIdx;
        tires = AI_CAR(car)->tires;
    }

    if (hit->unk04) {
        if (m->vel.vy >= 0) {
            ang = m->unkA8.vz;
        } else {
            ang = m->unkA8.vz + 2048;
        }
    } else {
        if (hit->normal.vy == 0) {
            hit->normal.vy = 1;
        }
        ang = ratan2(hit->normal.vx, hit->normal.vy);
    }
    BoundAngle(&ang);
    if (m->vel.vy >= 0) {
        rel = m->unkA8.vz - ang;
    } else {
        s32 reverseAngle = ang - 2048;
        rel = m->unkA8.vz - reverseAngle;
    }
    BoundAngle(&rel);
    bounce = 2048 - rel * 2;
    BoundAngle(&bounce);

    if (hit->obj != 0 && hit->obj->unkC0 < 100 && hit->obj->unkC0 != 8) {
        hitCs = CheckCSHit(car, isPlayer, hit->obj, bounce, hit->histIdx);
        col->unk3 = 1;
        m->unkA8.vz = m->rot2.vz;
        ang = m->unkA8.vz;
        BoundAngle(&ang);
    } else {
        col->unk3 = 0;
        mag = abs(m->vel.vy);
        hitCs = 0;
        if (GetFieldsLastFrame() * 570 / 100 * 32 < mag) {
            dmg = abs(rsin(bounce / 4) * abs(m->vel.vy)) * 10 / st->unkA8 / 4096;
            if (dmg > 0) {
                if (dmg - 3 > 0 && isPlayer) {
                    carTakeHit(uaGetCarMatID(idx, isPlayer), dmg - 3, &hit->normal, 1);
                }
                scaledDamage = dmg * st->unk104 / 33;
                bulDispatchDamage(hit->obj->unkC0, hit->tag0, hit->tag1, scaledDamage, &m->rot, 0);
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
                    tires[0].unk14 = tires[0].unk14 / 2;
                    tires[1].unk14 = tires[1].unk14 / 2;
                    tires[2].unk14 = tires[2].unk14 / 2;
                    tires[3].unk14 = tires[3].unk14 / 2;
                }
            }
        }
        if (!isPlayer) {
            CheckIfLostAICar(car);
            if (bounce >= 1707) {
                bounce = 1706;
            } else if (bounce < -1706) {
                bounce = -1706;
            }
        }
        RicochetOffObject(car, isPlayer, 0, bounce, hit->histIdx);
    }

    if (hitCs == 0) {
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ang;
        RotMatrixYXZ(&rot, &mat);
        delta.vx = 0;
        if (hit->unk04 && col->unk3 == 0) {
            hit->depth += abs(m->vel.vy / 32);
        }
        delta.vy = -(hit->depth + 8);
        delta.vz = 0;
        mathMulTransVec(&mat, &delta, &out);
        m->pos.vx += out.vx;
        m->pos.vy += out.vy;
        m->pos.vz += out.vz;
        delta.vx = 0;
        delta.vy = 0;
        delta.vz = 0;
        switch (hit->histIdx) {
        case 0:
            delta.vx = 8;
            spark.vx = tires[0].unk38;
            spark.vy = tires[0].unk3C;
            spark.vz = tires[0].unk40 + st->unk6C;
            break;
        case 1:
            delta.vx = -8;
            spark.vx = tires[1].unk38;
            spark.vy = tires[1].unk3C;
            spark.vz = tires[1].unk40 + st->unk6C;
            break;
        case 2:
            delta.vx = -8;
            if (hit->unk04) {
                delta.vy = (abs(hit->depth) + 8) * 2;
            }
            spark.vx = tires[3].unk38;
            spark.vy = tires[3].unk3C;
            spark.vz = tires[3].unk40 + st->unk6C;
            break;
        case 3:
            delta.vx = 8;
            if (hit->unk04) {
                delta.vy = (abs(hit->depth) + 8) * 2;
            }
            spark.vx = tires[2].unk38;
            spark.vy = tires[2].unk3C;
            spark.vz = tires[2].unk40 + st->unk6C;
            break;
        case 4:
            delta.vy = -16;
            spark.vx = (tires[0].unk38 + tires[1].unk38) / 2;
            spark.vy = (tires[0].unk3C + tires[1].unk3C) / 2;
            spark.vz = (tires[0].unk40 + tires[1].unk40) / 2 + st->unk6C;
            break;
        case 5:
            delta.vy = 8;
            spark.vx = (tires[2].unk38 + tires[3].unk38) / 2;
            spark.vy = (tires[2].unk3C + tires[3].unk3C) / 2;
            spark.vz = (tires[2].unk40 + tires[3].unk40) / 2 + st->unk6C;
            break;
        case 6:
            delta.vx = 8;
            spark.vx = (tires[0].unk38 + tires[2].unk38) / 2;
            spark.vy = (tires[0].unk3C + tires[2].unk3C) / 2;
            spark.vz = (tires[0].unk40 + tires[2].unk40) / 2 + st->unk6C;
            break;
        case 7:
            delta.vx = -8;
            spark.vx = (tires[1].unk38 + tires[3].unk38) / 2;
            spark.vy = (tires[1].unk3C + tires[3].unk3C) / 2;
            spark.vz = (tires[1].unk40 + tires[3].unk40) / 2 + st->unk6C;
            break;
        default:
            spark.vx = 0;
            spark.vy = 0;
            spark.vz = tires[0].unk40 + st->unk6C;
            break;
        }
        num = abs(m->vel.vy) * 3 * 33;
        if (abs(m->vel.vy) < st->unkA8) {
            den = st->unkA8;
        } else {
            den = abs(m->vel.vy);
        }
        col->unk12 = num / den;
        mathMulTransVec(&m->mat2, &delta, &out);
        m->pos.vx += out.vx;
        m->pos.vy += out.vy;
        m->pos.vz += out.vz;
        if (GetFieldsLastFrame() * 380 / 100 * 32 < m->vel.vy) {
            do_simple_spark(&spark);
            if (hit->unk04) {
                do_puff(&spark);
            }
        }
    }
}

void CheckIfLostAICar(Car* car)
{
    AI_CAR(car)->unk180++;
    if (AI_CAR(car)->unk180 >= 4) {
        StartLostCheck(car);
    }
    if (AI_CAR(car)->unk4D) {
        AI_CAR(car)->unk182 = 80;
    }
}

u8 CheckCSHit(Car* car, u8 isPlayer, Cs* obj, s32 a, s32 b)
{
    u8 otherIsPlayer;
    s16 idx;
    Car* info;
    u8 ret;
    u8 hit;
    s32 bb;

    hit = uaIsCarCS(obj, &otherIsPlayer, &idx);
    bb = b;
    if (hit) {
        if (otherIsPlayer) {
            info = GetPlayerInfo(idx);
        } else {
            info = GetAICarInfo(idx);
        }
        DoCsHitCalculations(car, isPlayer, info, otherIsPlayer, a, bb);
        ret = 0;
    } else {
        RicochetOffObject(car, isPlayer, 0, a, b);
        ret = 0;
    }
    return ret;
}

void DoCsHitCalculations(
    Car* car, u8 isPlayer, Car* other, u8 otherIsPlayer, s32 collisionAngle, s16 mode)
{
    u8* f;
    CarStats* st;
    CarMotion* m;
    CarStats* ost;
    CarMotion* om;
    u8 front;
    u8 driftMe;
    u8 driftOther;
    u16 idx;
    s16 oidx;
    CarCollision* col;
    CarCollision* ocol;
    u8* of;
    s32 dead;
    u8 back;
    s32 otherBounceAngle;
    s32 bounceAngle;
    s32 impulse;
    s32 otherImpulse;
    s32 angleDifference;
    s32 driftAngle;
    VECTOR3 spark;
    s32 relativeAngle;

    driftMe = 0;
    driftOther = 0;
    if (isPlayer) {
        col = &PLAYER_CAR(car)->collision;
        f = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        idx = PLAYER_CAR(car)->playerIdx;
        dead = PLAYER_CAR(car)->uaIndex;
    } else {
        f = AI_CAR(car)->flags;
        st = &AI_CAR(car)->stats;
        col = &AI_CAR(car)->collision;
        m = &AI_CAR(car)->motion;
        idx = AI_CAR(car)->playerIdx;
        dead = AI_CAR(car)->uaIndex;
    }
    if (otherIsPlayer) {
        of = PLAYER_CAR(other)->flags;
        ost = &PLAYER_CAR(other)->stats;
        ocol = &PLAYER_CAR(other)->collision;
        om = &PLAYER_CAR(other)->motion;
        oidx = PLAYER_CAR(other)->playerIdx;
    } else {
        of = AI_CAR(other)->flags;
        ost = &AI_CAR(other)->stats;
        ocol = &AI_CAR(other)->collision;
        om = &AI_CAR(other)->motion;
        oidx = AI_CAR(other)->playerIdx;
    }
    if (isPlayer) {
        if (!otherIsPlayer && !of[0]) {
            of[24] = 1;
        }
    } else if (otherIsPlayer && !f[0]) {
        f[24] = 1;
    }
    if ((u16)mode < 2 || mode == 4)
        front = 1;
    else
        front = 0;
    back = mode == 2 || mode == 3;
    if (st->monster) {
        if (f[27]) {
            return;
        }
        if ((front && m->vel.vy > 0) || (back && m->vel.vy < 0)) {
            InitMonsterSmash(car, isPlayer, other, otherIsPlayer, mode);
            return;
        }
    } else if (of[28]) {
        return;
    }

    if (isPlayer || otherIsPlayer) {
        s32 damage;
        if (abs(m->vel.vy) > GetFieldsLastFrame() * 570 / 100 * 32
            || abs(om->vel.vy) > GetFieldsLastFrame() * 570 / 100 * 32) {
            if (st->unk108 > ost->unk108) {
                if (!f[27]) {
                    carTakeHit(uaGetCarMatID(idx, isPlayer), 1, &m->unkA8, 1);
                }
                if (abs(m->vel.vy) > (GetFieldsLastFrame() * 570 / 100 * 32) && !of[27]) {
                    damage = 12 * m->vel.vy / st->unkA8;
                    if (damage > 0) {
                        carTakeHit(uaGetCarMatID(oidx, otherIsPlayer), damage, &m->unkA8, 1);
                    }
                }
            } else if (st->unk108 < ost->unk108) {
                if (abs(om->vel.vy) > (GetFieldsLastFrame() * 570 / 100 * 32) && !f[27]) {
                    damage = 12 * om->vel.vy / ost->unkA8;
                    if (damage > 0) {
                        carTakeHit(uaGetCarMatID(idx, isPlayer), 11, &om->unkA8, 1);
                    }
                }
                if (!of[27]) {
                    carTakeHit(uaGetCarMatID(oidx, otherIsPlayer), 1, &om->unkA8, 1);
                }
            } else {
                if (abs(om->vel.vy) > (GetFieldsLastFrame() * 570 / 100 * 32) && !f[27]) {
                    damage = 6 * om->vel.vy / ost->unkA8;
                    if (damage > 0) {
                        carTakeHit(uaGetCarMatID(idx, isPlayer), damage, &om->unkA8, 1);
                    }
                }
                if (abs(m->vel.vy) > (GetFieldsLastFrame() * 570 / 100 * 32) && !of[27]) {
                    damage = 6 * m->vel.vy / st->unkA8;
                    if (damage > 0) {
                        carTakeHit(uaGetCarMatID(oidx, otherIsPlayer), damage, &m->unkA8, 1);
                    }
                }
            }
        }
    }

    {
        s32 heading;
        s32 otherHeading;
        if (st->unkA4 < abs(m->vel.vy))
            heading = m->unkA8.vz;
        else
            heading = m->rot.vz;
        if (ost->unkA4 < abs(om->vel.vy))
            otherHeading = om->unkA8.vz;
        else
            otherHeading = om->rot.vz;
        relativeAngle = heading - otherHeading;
    }
    BoundAngle(&relativeAngle);

    if (m->vel.vy != 0 && (om->vel.vy == 0 || of[0])) {
        of[1] = 1;
        SetFullCarDrift(om);
        om->rot2.vz = m->rot2.vz;
        bounceAngle = 0;
        otherBounceAngle = 0;
        if (st->unk108 > ost->unk108) {
            if (st->unk108 - 1 > ost->unk108) {
                om->vel.vy = 3 * m->vel.vy;
                m->vel.vy = 4 * m->vel.vy / 5;
            } else {
                om->vel.vy = 2 * m->vel.vy;
                m->vel.vy /= 2;
            }
        } else if (ost->unk108 > st->unk108) {
            om->vel.vy = m->vel.vy / 4;
            m->vel.vy /= 8;
        } else {
            om->vel.vy = m->vel.vy;
            m->vel.vy /= 2;
        }
    } else if (om->vel.vy != 0 && (m->vel.vy == 0 || f[0])) {
        f[1] = 1;
        SetFullCarDrift(m);
        m->rot2.vz = om->rot2.vz;
        bounceAngle = 0;
        otherBounceAngle = 0;
        if (ost->unk108 > st->unk108) {
            if (ost->unk108 - 1 > st->unk108) {
                m->vel.vy = 3 * om->vel.vy;
                om->vel.vy = 4 * om->vel.vy / 5;
            } else {
                m->vel.vy = 2 * om->vel.vy;
                om->vel.vy /= 2;
            }
        } else if (st->unk108 < ost->unk108) {
            m->vel.vy = om->vel.vy / 4;
            om->vel.vy /= 8;
        } else {
            m->vel.vy = om->vel.vy;
            om->vel.vy /= 2;
        }
    } else {
        s32 angleMagnitude = abs(collisionAngle);
        if (angleMagnitude < 1024) {
            otherBounceAngle = collisionAngle;
            bounceAngle = otherBounceAngle;
            if (st->unk108 > ost->unk108) {
                bounceAngle = otherBounceAngle / 2;
                om->vel.vy /= 2;
            } else if (st->unk108 < ost->unk108) {
                otherBounceAngle /= 2;
                m->vel.vy /= 2;
            }
            angleDifference = abs(relativeAngle);
            if (angleDifference < 1024) {
                otherBounceAngle = -otherBounceAngle;
            }
            {
                s32 speedA;
                s32 speedB;
                speedA = m->vel.vy;
                speedB = om->vel.vy;
                if (speedA < 0)
                    speedA = -speedA;
                if (speedB < 0)
                    speedB = -speedB;
                if (2 * speedB < speedA) {
                    om->rot2.vz = m->unkA8.vz - bounceAngle;
                    if (m->vel.vy < 0) {
                        om->rot2.vz -= 2048;
                        if (angleDifference < 1024) {
                            om->rot2.vz -= 2 * bounceAngle;
                        }
                    } else {
                        if (angleDifference >= 1025) {
                            om->rot2.vz -= 2 * bounceAngle;
                        }
                    }
                    otherBounceAngle = 0;
                } else {
                    speedA = om->vel.vy;
                    speedB = m->vel.vy;
                    if (speedA < 0)
                        speedA = -speedA;
                    if (speedB < 0)
                        speedB = -speedB;
                    if (2 * speedB < speedA) {
                        m->rot2.vz = om->unkA8.vz - otherBounceAngle;
                        if (om->vel.vy < 0) {
                            m->rot2.vz -= 2048;
                            if (angleDifference < 1024) {
                                m->rot2.vz -= 2 * otherBounceAngle;
                            }
                        } else {
                            if (angleDifference >= 1025) {
                                m->rot2.vz -= 2 * otherBounceAngle;
                            }
                        }
                        bounceAngle = 0;
                    }
                }
                speedA = m->vel.vy;
                speedB = om->vel.vy;
                if (speedA < 0)
                    speedA = -speedA;
                if (speedB < 0)
                    speedB = -speedB;
                if (speedB < speedA) {
                    s32 direction = relativeAngle;
                    if (direction < 0)
                        direction = -direction;
                    if (direction < 1024) {
                        om->vel.vy = m->vel.vy;
                    } else {
                        om->vel.vy = -m->vel.vy;
                    }
                } else {
                    s32 direction = relativeAngle;
                    if (direction < 0)
                        direction = -direction;
                    if (direction < 1024) {
                        m->vel.vy = om->vel.vy;
                    } else {
                        m->vel.vy = -om->vel.vy;
                    }
                }
            }
        } else {
            s32 separation = abs(relativeAngle);
            if (separation < 682) {
                impulse = m->vel.vy - om->vel.vy;
                otherImpulse = om->vel.vy - m->vel.vy;
                if (st->unk108 > ost->unk108) {
                    impulse /= 2;
                    otherImpulse *= 2;
                } else if (st->unk108 < ost->unk108) {
                    impulse *= 2;
                    otherImpulse /= 2;
                }
                m->rot2.vz = m->rot.vz;
                m->vel.vy -= impulse;
                om->rot2.vz = om->rot.vz;
                om->vel.vy -= otherImpulse;
                if (mode == 1 || mode == 3) {
                    bounceAngle = -170;
                    otherBounceAngle = 170;
                } else {
                    bounceAngle = 170;
                    otherBounceAngle = -170;
                }
            } else if (separation < 1365) {
                bounceAngle = 0;
                otherBounceAngle = 0;
                if (front) {
                    om->vel.vy = m->vel.vy;
                    om->rot2.vz = m->rot2.vz;
                    of[1] = 1;
                    SetFullCarDrift(om);
                    if (st->unk108 > ost->unk108) {
                        if (st->unk108 - 1 > ost->unk108) {
                            m->vel.vy = 4 * m->vel.vy / 5;
                        } else {
                            m->vel.vy = m->vel.vy / 2;
                        }
                    } else {
                        m->vel.vy = m->vel.vy / 4;
                    }
                    if (st->unk104 >= ost->unk104 && m->vel.vy > st->unkC0) {
                        driftOther = 1;
                    }
                } else {
                    m->vel.vy = om->vel.vy;
                    m->rot2.vz = om->rot2.vz;
                    f[1] = 1;
                    SetFullCarDrift(m);
                    if (ost->unk108 > st->unk108) {
                        if (ost->unk108 - 1 > st->unk108) {
                            om->vel.vy = 4 * om->vel.vy / 5;
                        } else {
                            om->vel.vy = om->vel.vy / 2;
                        }
                    } else {
                        om->vel.vy = om->vel.vy / 4;
                    }
                    if (ost->unk104 >= st->unk104 && om->vel.vy > ost->unkC0) {
                        driftMe = 1;
                    }
                }
                if (st->unk108 > ost->unk108) {
                    if (st->unk108 - 1 > ost->unk108) {
                        om->vel.vy *= 3;
                    } else {
                        om->vel.vy *= 2;
                    }
                } else if (ost->unk108 > st->unk108) {
                    if (ost->unk108 - 1 < st->unk108) {
                        m->vel.vy = 3 * m->vel.vy;
                    } else {
                        m->vel.vy = 2 * m->vel.vy;
                    }
                }
                if (mode == 0) {
                    bounceAngle += 170;
                } else {
                    bounceAngle -= 170;
                }
            } else {
                otherImpulse = m->vel.vy + om->vel.vy;
                impulse = otherImpulse;
                if (st->unk108 > ost->unk108) {
                    if (st->unk108 - 1 > ost->unk108) {
                        impulse = m->vel.vy / 8;
                        otherImpulse *= 3;
                    } else {
                        impulse = m->vel.vy / 4;
                        otherImpulse *= 2;
                    }
                    of[1] = 1;
                    SetFullCarDrift(om);
                    if (m->vel.vy > st->unkC0) {
                        driftOther = 1;
                    }
                } else if (ost->unk108 > st->unk108) {
                    if (ost->unk108 - 1 > st->unk108) {
                        otherImpulse = om->vel.vy / 8;
                        impulse *= 3;
                    } else {
                        otherImpulse = om->vel.vy / 4;
                        impulse *= 2;
                    }
                    f[1] = 1;
                    SetFullCarDrift(m);
                    if (om->vel.vy > ost->unkC0) {
                        driftMe = 1;
                    }
                }
                if (m->vel.vy - impulse > om->vel.vy - otherImpulse) {
                    om->vel.vy = m->vel.vy;
                    om->rot2.vz = om->rot.vz - 2048;
                    otherBounceAngle = 0;
                    bounceAngle = 0;
                    m->vel.vy -= impulse;
                } else {
                    m->vel.vy = om->vel.vy;
                    m->rot2.vz = m->rot.vz - 2048;
                    bounceAngle = 0;
                    otherBounceAngle = 0;
                    om->vel.vy -= otherImpulse;
                }

                if (mode == 1) {
                    bounceAngle += 170;
                    otherBounceAngle -= 170;
                } else {
                    bounceAngle -= 170;
                    otherBounceAngle += 170;
                }
            }
        }
    }

    if (driftMe) {
        f[23] = 1;
        m->unk02 = m->rot2.vz;
        relativeAngle = m->rot.vz - m->rot2.vz;
        BoundAngle(&relativeAngle);
        if (relativeAngle > 0) {
            m->rotDelta.vy = 1;
            driftAngle = m->rot2.vz + 1024;
        } else {
            m->rotDelta.vy = -1;
            driftAngle = m->rot2.vz - 1024;
        }
        m->unk04 = driftAngle;
    }
    if (driftOther) {
        of[23] = 1;
        om->unk02 = om->rot2.vz;
        relativeAngle = om->rot.vz - om->rot2.vz;
        BoundAngle(&relativeAngle);
        if (relativeAngle > 0) {
            om->rotDelta.vy = 1;
            driftAngle = om->rot2.vz + 1024;
        } else {
            om->rotDelta.vy = -1;
            driftAngle = om->rot2.vz - 1024;
        }
        om->unk04 = driftAngle;
    }
    if (col->unkE < 150 && col->unk10 == oidx) {
        spark.vx = (m->pos.vx + om->pos.vx) / 2;
        spark.vy = (m->pos.vy + om->pos.vy) / 2;
        spark.vz = (m->pos.vz + om->pos.vz) / 2;
        do_simple_spark(&spark);
        m->rot2.vz = m->rot.vz;
        switch (mode) {
        case 0:
            bounceAngle = (m->vel.vy == 0) ? 0x238 : 0;
            m->rot2.vz += 512;
            break;
        case 1:
            bounceAngle = -0x238;
            if (m->vel.vy != 0) {
                bounceAngle = 0;
            }
            m->rot2.vz -= 512;
            break;
        case 2:
            bounceAngle = -512;
            m->rot2.vz -= 512;
            break;
        case 3:
            bounceAngle = 512;
            m->rot2.vz += 512;
            break;
        case 4:
            bounceAngle = (m->vel.vy == 0) ? 0x155 : 0;
            m->rot2.vz += 1024;
            break;
        case 5:
            bounceAngle = 0;
            m->rot2.vz += 512;
            break;
        case 6:
            bounceAngle = 1024;
            m->rot2.vz += 512;
            break;
        case 7:
            bounceAngle = -1024;
            m->rot2.vz -= 512;
            break;
        default:
            bounceAngle = 1024;
            m->rot2.vz += 1024;
            break;
        }

        if (m->vel.vy > 0) {
            m->vel.vy = abs(m->vel.vy) < GetFieldsLastFrame() * 1140 / 100 * 32
                ? GetFieldsLastFrame() * 1140 / 100 * 32
                : abs(m->vel.vy);
        } else {
            m->vel.vy = abs(m->vel.vy) < -(GetFieldsLastFrame() * 1140 / 100 * 32)
                ? abs(m->vel.vy)
                : -(GetFieldsLastFrame() * 1140 / 100 * 32);
        }
    }

    col->unkE = 0;
    col->unk10 = oidx;
    ocol->unkE = 0;
    ocol->unk10 = idx;
    if (m->vel.vy > st->unkA8) {
        m->vel.vy = st->unkA8;
    }
    if (om->vel.vy > ost->unkA8) {
        om->vel.vy = ost->unkA8;
    }
    RicochetOffObject(car, isPlayer, 1, bounceAngle, mode);
    ocol->unk0 = 1;
    RicochetOffObject(other, otherIsPlayer, 1, otherBounceAngle, mode);
    RotateCarsAwayFromCollision(m, om);
}

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
        ax = a->pos.vx;
        bx = b->pos.vx;
        ay = a->pos.vy;
        by = b->pos.vy;
    } while (0);
    dy = ay - by;
    dx = ax - bx;
    if (dy == 0) {
        dy = 1;
    }
    ang = ratan2(dx, dy) - a->rot.vz;
    BoundAngle(&ang);
}

void RicochetOffObject(Car* car, u8 isPlayer, u8 doHit, s32 delta, s16 mode)
{
    CarMotion* m;
    u8* f;
    CarCollision* c;
    u8* skid;
    CarStats* st;
    s32 ang;
    s32 sub;
    u8 ok;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        f = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
        c = &PLAYER_CAR(car)->collision;
        skid = PLAYER_CAR(car)->skid;
    } else {
        m = &AI_CAR(car)->motion;
        f = AI_CAR(car)->flags;
        c = &AI_CAR(car)->collision;
        st = &AI_CAR(car)->stats;
        skid = AI_CAR(car)->skid;
        AI_CAR(car)->unk16D = 1;
    }

    m->rot2.vz += delta;
    BoundAngle(&m->rot2.vz);
    f[1] = 1;
    SetCollisionBounce(car, isPlayer, delta);

    if (isPlayer && f[6] && m->vel.vy < GetFieldsLastFrame() * 570 / 100 * 32
        && ((u16)mode < 2 || mode == 4)) {
        c->unk2 = 1;
    } else {
        c->unk2 = 0;
    }

    if ((u16)mode < 2 || mode == 4 || skid[1]) {
        if (c->unk2 || abs(delta) < 682) {
            m->rot2.vz -= delta / 3;
            c->unk8 = m->rot2.vz;
        } else if (abs(delta) < 1365) {
            c->unk8 = m->rot2.vz - delta / 2;
        } else {
            c->unk8 = m->rot2.vz - 2048;
        }
        BoundSAngle(&c->unk8);
        c->unk1 = 1;
    }

    ang = m->rot.vz - m->rot2.vz;
    BoundAngle(&ang);

    if (c->unk2) {
        if (f[6]) {
            if ((ang < 0 && skid[2]) || (ang > 0 && skid[3])) {
                c->unk8 = m->rot2.vz - 2048;
                BoundSAngle(&c->unk8);
            }
        }
    }

    if (isPlayer && f[6]) {
        if (skid[2]) {
            c->unk8 = m->rot.vz - 512;
        } else {
            c->unk8 = m->rot.vz + 512;
        }
        BoundSAngle(&c->unk8);
        c->unk1 = 1;
    }

    if (doHit && m->vel.vy > st->unkC0) {
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
    if (m->vel.vy < 0) {
        sub = m->vel.vy * 3 / 4;
    } else if (m->vel.vy >= GetFieldsLastFrame() * 570 / 100 * 32) {
        if (abs(delta) < 512) {
            sub = m->vel.vy / 4;
        } else if (abs(delta) < 1024) {
            sub = m->vel.vy / 3;
        } else {
            sub = m->vel.vy / 2;
        }
    }
    m->vel.vy -= sub;
    SetCollisCount(c);
}

void SetCollisionBounce(Car* car, u8 isPlayer, s32 mag)
{
    mag = abs(mag);
    if (mag < 227) {
        SetBounce(car, 1, 2, 2, isPlayer);
    } else if (mag < 512) {
        SetBounce(car, 2, 2, 2, isPlayer);
    } else if (mag < 1024) {
        SetBounce(car, 3, 2, 2, isPlayer);
    } else if (mag < 1536) {
        SetBounce(car, 3, 2, 2, isPlayer);
    } else {
        SetBounce(car, 5, 2, 2, isPlayer);
    }
}

void InitNoCollision(CarCollision* c)
{
    c->unk0 = 0;
    c->unk1 = 0;
    c->unk2 = 0;
    c->unk4 = 0;
    c->count = 0;
}

void SetCollisCount(CarCollision* c)
{
    if (c->count <= 0) {
        c->count = 150;
    }
}

void CarViewUpdate(PlayerCar* car)
{
    VECTOR v;
    VECTOR out;
    Cs* cs;
    s32 tilt;
    s32 dx;
    s32 dy;
    s32 dz;

    if (car->stats.unk3C) {
        uaDrawCar(car->playerIdx, 1);
    } else {
        uaDontDrawCar(car->playerIdx, 1);
    }
    if ((u32)car->unkF4 < VR_MODE_HELICOPTER) {
        GetUpdateRate();
        if (car->unkF4 == VR_MODE_UNK_1) {
            if (abs(car->dTrans.vy - car->vrPos[0].vy) >= 17) {
                car->dTrans.vx = SmoothValue(car->dTrans.vx, car->vrPos[0].vx, 0x5A);
                car->dTrans.vy = SmoothValue(car->dTrans.vy, car->vrPos[0].vy, 0x5A);
                car->dTrans.vz = SmoothValue(car->dTrans.vz, car->vrPos[0].vz, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[0].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[0].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[0].vz, 0x5A);
            } else {
                uaDontDrawCar(car->playerIdx, 1);
                car->dTrans.vx = car->vrPos[0].vx;
                car->dTrans.vy = car->vrPos[0].vy;
                car->dTrans.vz = car->vrPos[0].vz;
                car->dRot.vx = car->vrRot[0].vx;
                car->dRot.vy = car->vrRot[0].vy;
                car->dRot.vz = car->vrRot[0].vz;
                if (car->stats.unk3C) {
                    UAdashSetDashboardDrawFlag(1);
                    uaDontDrawCar(car->playerIdx, 1);
                }
            }
        } else if (car->unkF4 == VR_MODE_UNK_2) {
            if (abs(car->dTrans.vy - car->vrPos[1].vy) >= 17) {
                car->dTrans.vy = SmoothValue(car->dTrans.vy, car->vrPos[1].vy, 0x5A);
                car->dTrans.vz = SmoothValue(car->dTrans.vz, car->vrPos[1].vz, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[1].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[1].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[1].vz, 0x5A);
            } else {
                car->dTrans.vy = car->vrPos[1].vy;
                car->dTrans.vz = car->vrPos[1].vz;
                car->dRot.vx = car->vrRot[1].vx;
                car->dRot.vy = car->vrRot[1].vy;
                car->dRot.vz = car->vrRot[1].vz;
            }
            if (car->motion.rotDelta.vz > car->stats.unkD4) {
                tilt = 0x70;
            } else if (car->motion.rotDelta.vz < -car->stats.unkD4) {
                tilt = -0x70;
            } else {
                tilt = car->motion.rotDelta.vz * 0x70 / car->stats.unkD4;
            }
            car->dTrans.vx = SmoothValue(car->dTrans.vx, tilt, 0x5A);
        } else {
            if (abs(car->dTrans.vy - car->vrPos[2].vy) >= 17) {
                car->dTrans.vy = SmoothValue(car->dTrans.vy, car->vrPos[2].vy, 0x5A);
                car->dTrans.vz = SmoothValue(car->dTrans.vz, car->vrPos[2].vz, 0x5A);
                car->dRot.vx = SmoothValue(car->dRot.vx, car->vrRot[2].vx, 0x5A);
                car->dRot.vy = SmoothValue(car->dRot.vy, car->vrRot[2].vy, 0x5A);
                car->dRot.vz = SmoothValue(car->dRot.vz, car->vrRot[2].vz, 0x5A);
            } else {
                car->dTrans.vy = car->vrPos[2].vy;
                car->dTrans.vz = car->vrPos[2].vz;
                car->dRot.vx = car->vrRot[2].vx;
                car->dRot.vy = car->vrRot[2].vy;
                car->dRot.vz = car->vrRot[2].vz;
            }
            if (car->motion.rotDelta.vz > car->stats.unkD4) {
                tilt = 0xA0;
            } else if (car->motion.rotDelta.vz < -car->stats.unkD4) {
                tilt = -0xA0;
            } else {
                tilt = car->motion.rotDelta.vz * 0xA0 / car->stats.unkD4;
            }
            car->dTrans.vx = SmoothValue(car->dTrans.vx, tilt, 0x5A);
        }
        viewSetDeltaRot(&car->dRot, car->playerIdx);
        viewSetDeltaTrans(&car->dTrans, car->playerIdx);
        return;
    }
    if (car->unkF4 == VR_MODE_HELICOPTER) {
        cs = UAGetCameraCS(car->playerIdx);
        v.vx = 0;
        v.vy = 100;
        v.vz = 0;
        mathMulTransVec(&car->motion.mat2, &v, &out);
        out.vx += car->motion.pos.vx;
        out.vy += car->motion.pos.vy;
        out.vz += car->motion.pos.vz;
        dx = out.vx - cs->pos.vx;
        dy = out.vy - cs->pos.vy;
        dz = out.vz - cs->pos.vz;
        car->dRot.vx = -ratan2(dz, SquareRoot0(dx * dx + dy * dy));
        car->dRot.vy = 0;
        car->dRot.vz = ratan2(dx, dy);
        viewSetDeltaRot(&car->dRot, car->playerIdx);
    }
}

void SetBounce(Car* car, s32 level, s32 a, s32 b, u8 isPlayer)
{
    CarBounce* bp;
    u32 mag;
    u32 ang;

    if (isPlayer) {
        bp = &PLAYER_CAR(car)->bounce;
    } else {
        bp = &AI_CAR(car)->bounce;
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
    bp->unk12 = ang - RANDINT(ang >> 1);
    CarInitBounceDeltas(car, isPlayer);
}

void CarInitBounceDeltas(Car* car, u8 isPlayer)
{
    CarBounce* bp;
    CarMotion* m;

    if (isPlayer) {
        bp = &PLAYER_CAR(car)->bounce;
        m = &PLAYER_CAR(car)->motion;
    } else {
        bp = &AI_CAR(car)->bounce;
        m = &AI_CAR(car)->motion;
    }

    if (m->vel.vy / 32) {
        bp->unk14 = bp->unk12 - RANDINT(bp->unk12 / 2);
        if (bp->unk18 > 0) {
            bp->unk24 = bp->unk18 - RANDINT(bp->unk18 / 2);
        } else if (bp->unk18 < 0) {
            bp->unk24 = bp->unk18 + RANDINT(bp->unk18 / 2);
        }
        if (bp->unk1C > 0) {
            bp->unk28 = bp->unk1C - RANDINT(bp->unk1C / 2);
        } else if (bp->unk1C < 0) {
            bp->unk28 = bp->unk1C + RANDINT(bp->unk1C / 2);
        }
    } else {
        bp->unk04 = 0;
    }
}

void UpdateBounce(Car* car, u8 isPlayer)
{
    CarBounce* bp;
    CarMotion* m;
    CarStats* st;
    s32 d;
    s32 q;
    s16 v;
    s32 qs;
    s32 amp[4];

    if (isPlayer) {
        bp = &PLAYER_CAR(car)->bounce;
        m = &PLAYER_CAR(car)->motion;
        st = &PLAYER_CAR(car)->stats;
    } else {
        bp = &AI_CAR(car)->bounce;
        m = &AI_CAR(car)->motion;
        st = &AI_CAR(car)->stats;
    }

    if (isPlayer) {
        d = st->unkD4;
    } else {
        d = st->unkD4 / 10;
    }

    q = m->rotDelta.vz * st->unkF4 / d;
    qs = (s16)q;
    v = q;
    if (abs(st->unkF4) < abs(qs)) {
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
    bp->unk34 = SmoothValue(bp->unk34, v, 90);

    if (isPlayer && PLAYER_CAR(car)->unkF4 == VR_MODE_UNK_1) {
        bp->unk16 = SmoothValue(bp->unk16, PLAYER_CAR(car)->speedDelta, 90);
        bp->unk30 = -bp->unk16 / 2;
    } else {
        bp->unk30 = 0;
    }

    if (bp->unk04) {
        bp->unk0E = bp->unk0E + bp->unk14;
        if (bp->unk0E < 4096) {
            amp[0] = bp->unk24 * m->vel.vy / (m->vel.vy < st->unkA8 ? st->unkA8 : m->vel.vy);
            amp[1] = bp->unk28 * m->vel.vy / (m->vel.vy < st->unkA8 ? st->unkA8 : m->vel.vy);
            bp->unk30 += rsin(bp->unk0E) * amp[0] / 4096;
            bp->unk34 += rsin(bp->unk0E) * amp[1] / 4096;
        } else {
            bp->unk0E = bp->unk0E - 4096;
            if (bp->unk0C > 0) {
                bp->unk0C--;
                bp->unk24 = bp->unk24 / 2;
                bp->unk28 = bp->unk28 / 2;
            } else {
                SetBounce(car, bp->unk00, bp->unk08, 0, isPlayer);
            }
        }
    } else if (bp->unk00) {
        SetBounce(car, bp->unk00, bp->unk08, 0, isPlayer);
    }
}

void carTakeHit(s16 id, s32 amount, VECTOR3* a, u8 flag)
{
    u8 rgb[3];
    u8 isPlayer;
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
    PlayerCar* car;
    AICar* alt;
    u8* f;
    CarStats* st;
    u8 enabled;

    if (amount >= 3) {
        rate = 16 / GetFieldsLastFrame();
    } else {
        rate = 6 / GetFieldsLastFrame();
    }

    if (flag) {
        rgb[0] = 0xFF;
        rgb[1] = 0xFF;
        rgb[2] = 0xFF;
    } else {
        rgb[0] = 0xFF;
        rgb[1] = 0;
        rgb[2] = 0;
    }

    if ((u8)uaIsCarMatID(id, (s8*)&isPlayer, &idx)) {
        if (isPlayer) {
            info = GetPlayerInfo(idx);
            canHit = 1;
            car = GetPlayerInfo(idx);
            st = &car->stats;
            f = car->flags;
            mode = car->unkF4;
            wp = &car->weap;
            cs = GetPlayerCs3D(idx);
            enabled = 1;
            ua = car->uaIndex;
        } else {
            info = GetAICarInfo(idx);
            alt = GetAICarInfo(idx);
            wp = &alt->weap;
            cs = GetAICs3D(idx);
            enabled = alt->unk40;
            canHit = alt->unk46;
            mode = VR_MODE_UNK_3;
            st = &alt->stats;
            f = alt->flags;
            ua = alt->uaIndex;
        }

        if (enabled && !f[0x16] && !uaHasPlayerBeatThisLevel()) {
            if (mode == VR_MODE_UNK_1) {
                SetFXSheet(rate, rgb[0], rgb[1], rgb[2]);
            } else if (amount != HIT_FREEZE && !f[0x10]) {
                UAeffectActHitLights(cs, rate, rgb);
            }

            if (amount >= 0) {
                if (amount >= 8 && (rand() & 7) < amount - 7 && !f[0x1C]) {
                    InitBombDamage(info, isPlayer, 1, flag);
                }
                if (st->lostTimer) {
                    amount = amount / 2;
                }
                n = rand();
                d = amount / 3;
                k = n % ((d < 2 ? 2 : d) < 8 ? (d < 2 ? 2 : d) : 8);
                explodeCreateFragments(k, (VECTOR*)&cs->pos);
                if (st->unk4C || !isPlayer) {
                    if (isPlayer) {
                        dif = uaGetDifficulty();
                        if (dif == DIFFICULTY_EASY) {
                            amount = (amount * 2) / 3;
                        }
                        if (dif == DIFFICULTY_HARD) {
                            amount = (amount * 4) / 3;
                        }
                        if (amount <= 0) {
                            amount = 1;
                        }
                    }
                    st->unk40 = st->unk40 - amount;
                }
                if (f[0x10] && amount >= 6 && !f[0]) {
                    st->unk18 = 0;
                }
            } else {
                switch (amount) {
                case HIT_FREEZE:
                    if (!f[0x10]) {
                        f[0x10] = 1;
                        st->unk18 = 0xB4;
                        UAeffectSetFreezeLight(cs);
                    } else {
                        st->unk18 = 0xB4;
                    }
                    break;
                case HIT_OIL:
                    if (ua != UA_BOSS_CAR && canHit) {
                        f[0x12] = 1;
                        f[0x0D] = 1;
                        st->unk1C = 0xC8;
                    }
                    break;
                case HIT_SPIKE:
                    f[0x11] = 1;
                    st->spikeTimer = 0xC8;
                    st->unkFC = st->unkFC / 2;
                    RecomputeCarDeltas(st);
                    break;
                case HIT_CATAPULT:
                    if (ua != UA_BOSS_CAR) {
                        InitCatapult(info, isPlayer);
                        break;
                    }

                case HIT_BOMB:
                    InitBombDamage(info, isPlayer, 1, flag);
                    break;
                case HIT_UNK_6:
                    wp->unk42 = 1;
                    break;
                }
            }
        }
    }
}

void InitCatapult(Car* car, u8 isPlayer)
{
    CarTire* t;
    u8* f;
    CarMotion* m;
    s16 v;

    if (isPlayer) {
        t = PLAYER_CAR(car)->tires;
        f = PLAYER_CAR(car)->flags;
        m = &PLAYER_CAR(car)->motion;
    } else {
        t = AI_CAR(car)->tires;
        f = AI_CAR(car)->flags;
        m = &AI_CAR(car)->motion;
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
        m->vel.vy = m->vel.vy * 3 / 2;
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

void CheckCatapults(Car* car, u8 isPlayer)
{
    CarTire* t;
    u8* f;

    if (isPlayer) {
        t = PLAYER_CAR(car)->tires;
        f = PLAYER_CAR(car)->flags;
    } else {
        t = AI_CAR(car)->tires;
        f = AI_CAR(car)->flags;
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

void InitBombDamage(Car* car, u8 isPlayer, s8 full, u8 noMotion)
{
    CarTire* t;
    CarMotion* m;
    CarStats* st;
    u8* f;
    s32* dmg;
    s32 pct;
    s32 v;

    if (isPlayer) {
        t = PLAYER_CAR(car)->tires;
        pct = PLAYER_CAR(car)->stats.unk104;
        f = PLAYER_CAR(car)->flags;
        m = &PLAYER_CAR(car)->motion;
        st = &PLAYER_CAR(car)->stats;
        dmg = &PLAYER_CAR(car)->motion.unkD4;
    } else {
        t = AI_CAR(car)->tires;
        pct = AI_CAR(car)->stats.unk104;
        f = AI_CAR(car)->flags;
        m = &AI_CAR(car)->motion;
        st = &AI_CAR(car)->stats;
        dmg = &AI_CAR(car)->motion.unkD4;
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
            m->rot2.vz -= 512;
            m->vel.vy -= m->vel.vy / 4;
        }
        *dmg = -st->unkD0;
        break;
    case 1:
        t[2].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz -= 113;
            m->vel.vy -= m->vel.vy / 2;
        }
        *dmg = -st->unkD0 / 4;
        break;
    case 2:
        t[1].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz += 512;
            m->vel.vy -= m->vel.vy / 4;
        }
        *dmg = st->unkD0;
        break;
    case 3:
        t[1].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz += 1024;
            m->vel.vy -= m->vel.vy / 2;
        }
        *dmg = st->unkD0 * 2;
        break;
    case 4:
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz += 1536;
            m->vel.vy -= m->vel.vy / 4;
        }
        *dmg = st->unkD0;
        break;
    case 5:
        t[2].unk12 -= 2;
        t[3].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz += 113;
        }
        *dmg = st->unkD0 / 4;
        break;
    case 6:
        t[2].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz -= 1536;
            m->vel.vy -= m->vel.vy / 4;
        }
        *dmg = -st->unkD0;
        break;
    case 7:
    default:
        t[0].unk12 -= 2;
        t[2].unk12 -= 2;
        if (!noMotion) {
            m->rot2.vz -= 1024;
            m->vel.vy -= m->vel.vy / 2;
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

void CheckBombDamage(Car* car, u8 isPlayer)
{
    CarTire* t;
    u8* f;
    CarMotion* m;
    s32* p;
    u8 was;

    if (isPlayer) {
        t = PLAYER_CAR(car)->tires;
        m = &PLAYER_CAR(car)->motion;
        f = PLAYER_CAR(car)->flags;
        p = &PLAYER_CAR(car)->motion.unkD4;
    } else {
        t = AI_CAR(car)->tires;
        m = &AI_CAR(car)->motion;
        f = AI_CAR(car)->flags;
        p = &AI_CAR(car)->motion.unkD4;
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
        m->rotDelta.vz += *p;
        if (was) {
            SetNoCarDrift(m, f);
        }
    }
}

void UpdateBombDamagedTire(CarTire* t, CarMotion* m)
{
    if (!t->unk3) {
        t->bombDamaged = 0;
        t->unk1E = 0;
    }
}

void CheckSpikeDamage(Car* car, u8 isPlayer)
{
    CarStats* s;
    u8* f;

    if (isPlayer) {
        s = &PLAYER_CAR(car)->stats;
        f = PLAYER_CAR(car)->flags;
    } else {
        s = &AI_CAR(car)->stats;
        f = AI_CAR(car)->flags;
    }

    if (f[0x11] && s->spikeTimer) {
        s->spikeTimer -= GetFieldsLastFrame();
        if (s->spikeTimer <= 0) {
            f[0x11] = 0;
            s->spikeTimer = 0;
            s->unkFC = s->unk100;
            RecomputeCarDeltas(s);
        }
    }
}

void InitMonsterSmash(Car* carA, u8 isPlayerA, Car* carB, u8 isPlayerB, s16 unused_mode)
{
    u8* fa;
    CarStats* sa;
    CarWeap* wa;
    u8* fb;
    CarStats* sb;
    CarMotion* mb;
    s16 idx;
    s32 uaIdx;
    Cs* cs;
    s32 dmg;
    s32 v;
    s32 qs;

    (void)unused_mode;
    if (isPlayerA) {
        fa = PLAYER_CAR(carA)->flags;
        sa = &PLAYER_CAR(carA)->stats;
        wa = &PLAYER_CAR(carA)->weap;
    } else {
        fa = AI_CAR(carA)->flags;
        sa = &AI_CAR(carA)->stats;
        wa = &AI_CAR(carA)->weap;
    }

    if (isPlayerB) {
        fb = PLAYER_CAR(carB)->flags;
        sb = &PLAYER_CAR(carB)->stats;
        mb = &PLAYER_CAR(carB)->motion;
        idx = PLAYER_CAR(carB)->playerIdx;
        uaIdx = PLAYER_CAR(carB)->uaIndex;
    } else {
        fb = AI_CAR(carB)->flags;
        sb = &AI_CAR(carB)->stats;
        mb = &AI_CAR(carB)->motion;
        idx = AI_CAR(carB)->playerIdx;
        uaIdx = AI_CAR(carB)->uaIndex;
    }

    if ((s16)wa->ammo[WEAPON_SPECIAL] < getSpecialWeaponCost(UA_MONSTER_TRUCK)) {
        return;
    }
    if (fa[27]) {
        return;
    }

    uasoundPlayCarSpecialWeaponLaunchOrInflight(SOUND_SPECIAL_MONSTER_TRUCK, 0, 0);
    wa->ammo[WEAPON_SPECIAL] = wa->ammo[WEAPON_SPECIAL] - getSpecialWeaponCost(UA_MONSTER_TRUCK);
    if (isPlayerB) {
        cs = GetPlayerCs3D(idx);
    } else {
        cs = GetAICs3D(idx);
    }
    dmg = (s16)cs->unkC0;
    carTakeHit(dmg, carweapGetMonsterDamage(), NULL, 0);
    fb[16] = 1;
    sb->unk18 = 60;
    mb->vel.vy = 0;
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

void SetCarVRMode(PlayerCar* car, u32 mode)
{
    SVECTOR rot;
    VECTOR pos;
    VECTOR base;
    VECTOR off;
    u8 moved;

    if (mode < VR_MODE_FIXED) {
        car->unkF8 = mode;
    }
    if (mode < VR_MODE_HELICOPTER) {
        if ((u32)car->unkF4 >= VR_MODE_HELICOPTER) {
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vx = 0;
            pos.vy = 0;
            pos.vz = 0;
            UASetCameraPosition(car->playerIdx, (VECTOR3*)&pos, &rot);
            moved = 1;
        } else {
            moved = 0;
        }
    } else {
        moved = 0;
    }

    switch (mode - 1) {
    case 0:
        if (moved) {
            car->dTrans.vx = car->vrPos[0].vx;
            car->dTrans.vy = car->vrPos[0].vy;
            car->dTrans.vz = car->vrPos[0].vz;
            car->dRot.vx = car->vrRot[0].vx;
            car->dRot.vy = car->vrRot[0].vy;
            car->dRot.vz = car->vrRot[0].vz;
        }
        break;
    case 1:
        if (car->stats.unk3C) {
            uaDrawCar(car->playerIdx, 1);
        } else {
            uaDontDrawCar(car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        if (moved) {
            car->dTrans.vx = car->vrPos[1].vx;
            car->dTrans.vy = car->vrPos[1].vy;
            car->dTrans.vz = car->vrPos[1].vz;
            car->dRot.vx = car->vrRot[1].vx;
            car->dRot.vy = car->vrRot[1].vy;
            car->dRot.vz = car->vrRot[1].vz;
        }
        break;
    case 2:
        if (car->stats.unk3C) {
            uaDrawCar(car->playerIdx, 1);
        } else {
            uaDontDrawCar(car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        if (moved) {
            car->dTrans.vx = car->vrPos[2].vx;
            car->dTrans.vy = car->vrPos[2].vy;
            car->dTrans.vz = car->vrPos[2].vz;
            car->dRot.vx = car->vrRot[2].vx;
            car->dRot.vy = car->vrRot[2].vy;
            car->dRot.vz = car->vrRot[2].vz;
        }
        break;
    case 3:
        if (car->stats.unk3C) {
            uaDrawCar(car->playerIdx, 1);
        } else {
            uaDontDrawCar(car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        car->dTrans.vx = 0;
        car->dTrans.vy = 0;
        car->dTrans.vz = 0;
        car->dRot.vx = 0;
        car->dRot.vy = 0;
        car->dRot.vz = 0;
        base.vx = car->vrPos[3].vx;
        base.vy = car->vrPos[3].vy;
        base.vz = car->vrPos[3].vz;
        mathMulTransVec(&car->motion.mat, &base, &off);
        base.vx = car->motion.pos.vx + off.vx;
        base.vy = car->motion.pos.vy + off.vy;
        base.vz = car->motion.pos.vz + off.vz;
        UASetCameraPosition(car->playerIdx, (VECTOR3*)&base, &car->vrRot[3]);
        break;
    case 4:
        if (car->stats.unk3C) {
            uaDrawCar(car->playerIdx, 1);
        } else {
            uaDontDrawCar(car->playerIdx, 1);
        }
        UAdashSetDashboardDrawFlag(0);
        car->dTrans.vx = 0;
        car->dTrans.vy = 0;
        car->dTrans.vz = 0;
        car->dRot.vx = 0;
        car->dRot.vy = 0;
        car->dRot.vz = 0;
        break;
    default:
        car->dTrans = car->vrPos[0];
        car->dRot = car->vrRot[0];
        uaDontDrawCar(car->playerIdx, 1);
        UAdashSetDashboardDrawFlag(1);
        break;
    }

    viewSetDeltaTrans(&car->dTrans, car->playerIdx);
    viewSetDeltaRot(&car->dRot, car->playerIdx);
    car->unkF4 = mode;
}

void UpdateCarDeath(Car* car, u8 isPlayer)
{
    CarStats* st;
    CarMotion* m;
    u8* f;
    CarTire* t;
    s32 limit;
    u32 period;
    VECTOR3 pos;
    VECTOR3 off;
    s32 value;

    if (isPlayer) {
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
        f = PLAYER_CAR(car)->flags;
        t = PLAYER_CAR(car)->tires;
    } else {
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
        f = AI_CAR(car)->flags;
        t = AI_CAR(car)->tires;
    }

    if (st->deathTimer > 0) {
        st->deathTimer -= GetFieldsLastFrame();
        value = (st->deathTimer % 40);
        if (value == 1) {
            pos.vx = m->pos.vx;
            pos.vy = m->pos.vy;
            pos.vz = m->pos.vz;
            pos.vz = pos.vz - (st->unk6C / 2);
            do_smoke(&pos);
        }
        if (f[21]) {
            if (m->rot.vy >= -1023) {
                s32 rollRate;
                rollRate = st->unkD0 * m->rot.vy / -1024;
                value = st->unkCC;
                value += rollRate;
                m->rotDelta.vy -= value;
                m->vel.vy /= 2;
            } else {
                m->rotDelta.vy = 0;
            }
        } else if (m->rot.vy < 1024) {
            s32 rollRate;
            rollRate = st->unkD0 * m->rot.vy / 1024;
            value = st->unkCC;
            value += rollRate;
            m->rotDelta.vy += value;
            m->vel.vy /= 2;
        } else {
            m->rotDelta.vy = 0;
        }
        if (f[20]) {
            if (m->vel.vy < st->unkA4) {
                pos.vx = m->pos.vx;
                pos.vy = m->pos.vy;
                pos.vz = m->pos.vz;
                pos.vz = t->unk24;
                do_flames(&pos);
                f[20] = 0;
            }
        } else {
            limit = 3;
            if (shellGetCurrentLevel() == LEVEL6_ROOFTOPS && m->pos.vz < 400) {
                limit = 7;
            }
            if (st->blasts < limit) {
                if (st->blasts < limit) {
                    period = 4;
                } else {
                    period = 12;
                }
                if (GetFrameCount() % period == 0 && (rand() & 1)) {
                    st->blasts++;
                    f[26] = 1;
                    if (limit == 7) {
                        off.vx = RANDINT_POW2(64) - 32;
                        off.vy = RANDINT_POW2(64) - 32;
                        off.vz = RANDINT_POW2(16) - 8;
                        pos.vx = m->pos.vx;
                        pos.vy = m->pos.vy;
                        pos.vz = m->pos.vz;
                        pos.vx = pos.vx + off.vx;
                        pos.vy = pos.vy + off.vy;
                        pos.vz = pos.vz + off.vz;
                        if (!RANDINT_POW2(2)) {
                            do_mondo_explosion(&pos);
                        } else {
                            do_bigger_explosion(&pos);
                        }
                    } else {
                        off.vx = RANDINT_POW2(128) - 64;
                        off.vy = RANDINT_POW2(128) - 64;
                        // Possible bug in the original code here.
                        // The other offsets are evenly distributed in `[-64, 64)`,
                        // while this `vz` offset can only be either `-16` or `16`, with
                        // `16` being extremely rare.
                        // The fix would be to use `& 0x1F` instead.
                        off.vz = (rand() & 0x20) - 16;
                        pos.vx = m->pos.vx;
                        pos.vy = m->pos.vy;
                        pos.vz = m->pos.vz;
                        pos.vx = pos.vx + off.vx;
                        pos.vy = pos.vy + off.vy;
                        pos.vz = pos.vz + off.vz;
                        pos.vx = m->pos.vx;
                        pos.vy = m->pos.vy;
                        pos.vz = m->pos.vz;
                        if (RANDINT_POW2(2)) {
                            do_big_explosion(&pos);
                        } else {
                            do_bigger_explosion(&pos);
                        }
                    }
                }
            }
        }
    } else {
        m->rotDelta.vx = 0;
        m->rotDelta.vy = 0;
        m->rotDelta.vz = 0;
        m->vel.vx = 0;
        m->vel.vy -= m->vel.vy / 16;
        m->vel.vz = 0;

        if (m->rot.vy >= 1138) {
            if (m->rot.vy >= 1934) {
                return;
            }
            if (m->rot.vy < 1536) {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 1024, 98);
            } else {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 2036, 98);
            }
        } else if (m->rot.vy >= 114) {
            if (m->rot.vy >= 910) {
                return;
            }
            if (m->rot.vy < 512) {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 0, 98);
            } else {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 1024, 98);
            }
        } else if (m->rot.vy < -113) {
            if (m->rot.vy < -909) {
                return;
            }
            if (m->rot.vy >= -511) {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 0, 98);
            } else {
                m->rot.vy = SmoothAngleValue(m->rot.vy, 1024, 98);
            }
        } else if (m->rot.vy < -1137) {
            if (m->rot.vy < -1933) {
                return;
            }
            if (m->rot.vy >= -1535) {
                m->rot.vy = SmoothAngleValue(m->rot.vy, -1536, 98);
            } else {
                m->rot.vy = SmoothAngleValue(m->rot.vy, -2036, 98);
            }
        } else {
            return;
        }
    }
}

void TermCar(void)
{
}
