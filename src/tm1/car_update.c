#include "common.h"

#include <libgte.h>
#include <rand.h>

#include "tm1/ai_car.h"
#include "tm1/bridges.h"
#include "tm1/car.h"
#include "tm1/car_update.h"
#include "tm1/explode.h"
#include "tm1/explosion.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/shell.h"
#include "tm1/smooth.h"
#include "tm1/timer.h"
#include "tm1/trigger_pts.h"
#include "tm1/ua.h"

static s32 potHoleTics = 0;

#ifdef NON_MATCHING
s16 GetClosestTriggerPt(Car* car, u8 isPlayer)
{
    s16 group;
    s16 i;
    s16 best;
    s32 bestDist;
    s32 dx;
    s32 dz;
    CarMotion* m;

    best = 0;
    if (isPlayer) {
        group = PLAYER_CAR(car)->unkC0;
        m = &PLAYER_CAR(car)->motion;
    } else {
        group = AI_CAR(car)->unk176;
        m = &AI_CAR(car)->motion;
    }
    bestDist = 0x7FFFFFFF;
    for (i = 0; i < numTriggerPoints; i++) {
        if (tPoints[i].type != group && group < 11) {
            continue;
        }
        dx = m->pos.vx - tPoints[i].x;
        dz = m->pos.vy - tPoints[i].z;
        if (dx < 0) {
            dx = -dx;
        }
        if (dz < 0) {
            dz = -dz;
        }
        dx += dz;
        if (dx < bestDist) {
            best = i;
            bestDist = dx;
        }
    }
    return best;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetClosestTriggerPt);
#endif

#ifdef NON_MATCHING
void CalcTireCoordinates(Car* car, u8 isPlayer)
{
    CarMotion* m;
    CarStats* st;
    CarTire* tires;
    s16 g0;
    s16 g1;
    s16 g2;
    s16 g3;
    s32 lim;
    s32 v;
    s32 y;
    s32 dy;
    s32 dd;
    VECTOR vin;
    VECTOR vout;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        st = &PLAYER_CAR(car)->stats;
        tires = PLAYER_CAR(car)->tires;
    } else {
        m = &AI_CAR(car)->motion;
        st = &AI_CAR(car)->stats;
        tires = AI_CAR(car)->tires;
    }
    vin.vx = -st->unk74 / 2;
    vin.vy = st->unk80;
    vin.vz = -st->unk6C;
    mathMulTransVec(&m->mat2, &vin, (VECTOR*)&tires[0].unk2C);
    tires[0].unk38 = m->pos.vx + tires[0].unk2C.vx;
    tires[0].unk3C = m->pos.vy + tires[0].unk2C.vy;
    vin.vx = st->unk74 / 2;
    vin.vy = st->unk80;
    vin.vz = -st->unk6C;
    mathMulTransVec(&m->mat2, &vin, (VECTOR*)&tires[1].unk2C);
    tires[1].unk38 = m->pos.vx + tires[1].unk2C.vx;
    tires[1].unk3C = m->pos.vy + tires[1].unk2C.vy;
    vin.vx = -st->unk74 / 2;
    vin.vy = st->unk84;
    vin.vz = -st->unk6C;
    mathMulTransVec(&m->mat2, &vin, (VECTOR*)&tires[2].unk2C);
    tires[2].unk38 = m->pos.vx + tires[2].unk2C.vx;
    tires[2].unk3C = m->pos.vy + tires[2].unk2C.vy;
    vin.vx = st->unk74 / 2;
    vin.vy = st->unk84;
    vin.vz = -st->unk6C;
    mathMulTransVec(&m->mat2, &vin, (VECTOR*)&tires[3].unk2C);
    tires[3].unk38 = m->pos.vx + tires[3].unk2C.vx;
    tires[3].unk3C = m->pos.vy + tires[3].unk2C.vy;

    vin.vx = m->vel.vx / 32;
    vin.vy = m->vel.vy / 32;
    vin.vz = m->vel.vz / 32;
    mathMulTransVec(&m->mat, &vin, &vout);
    tires[0].unk38 += vout.vx;
    tires[0].unk3C += vout.vy;
    tires[0].unk40 += vout.vz;
    tires[1].unk38 += vout.vx;
    tires[1].unk3C += vout.vy;
    tires[1].unk40 += vout.vz;
    tires[2].unk38 += vout.vx;
    tires[2].unk3C += vout.vy;
    tires[2].unk40 += vout.vz;
    tires[3].unk38 += vout.vx;
    tires[3].unk3C += vout.vy;
    tires[3].unk40 += vout.vz;

    g0 = GetGroupTestPointIsIn(tires[0].unk38, tires[0].unk3C, m->pos.vz);
    g1 = GetGroupTestPointIsIn(tires[1].unk38, tires[1].unk3C, m->pos.vz);
    g2 = GetGroupTestPointIsIn(tires[2].unk38, tires[2].unk3C, m->pos.vz);
    g3 = GetGroupTestPointIsIn(tires[3].unk38, tires[3].unk3C, m->pos.vz);
    v = m->vel.vy / 32;
    if (v < 0) {
        v = -m->vel.vy / 32;
    }
    lim = st->unk5C + v;

    if (tires[0].unk10 != g0) {
        y = triggerPtGroups.pos[g0].y;
        dy = tires[0].unk40 - y;
        dd = y - triggerPtGroups.pos[tires[0].unk10].y;
        if (dd > 0) {
            if ((-lim < dy || tires[1].unk10 == g0 || tires[2].unk10 == g0 || tires[3].unk10 == g0)
                && m->vel.vy != 0) {
                tires[0].unk10 = g0;
                if (tires[0].unk0 == 0) {
                    tires[0].unk0 = 1;
                    if (tires[0].unk08 >= numCheckBridges) {
                        tires[0].unk08--;
                    } else {
                        tires[0].unk08++;
                    }
                }
                tires[0].unk26 = 0;
                tires[0].unk16 = dy;
                tires[0].unk24 = triggerPtGroups.pos[g0].y;
            }
        } else {
            tires[0].unk10 = g0;
            if (dd < 0) {
                tires[0].unk0 = 1;
                tires[0].unk14 = 0;
                tires[0].unk26 = 0;
                tires[0].unk16 = dy;
                tires[0].unk24 = triggerPtGroups.pos[g0].y;
                tires[0].unk3 = 1;
            }
        }
    }
    if (tires[1].unk10 != g1) {
        y = triggerPtGroups.pos[g1].y;
        dy = tires[1].unk40 - y;
        dd = y - triggerPtGroups.pos[tires[1].unk10].y;
        if (dd > 0) {
            if ((-lim < dy || tires[0].unk10 == g1 || tires[2].unk10 == g1 || tires[3].unk10 == g1)
                && m->vel.vy != 0) {
                tires[1].unk10 = g1;
                if (tires[1].unk0 == 0) {
                    tires[1].unk0 = 1;
                    if (tires[1].unk08 >= numCheckBridges) {
                        tires[1].unk08--;
                    } else {
                        tires[1].unk08++;
                    }
                }
                tires[1].unk26 = 0;
                tires[1].unk16 = dy;
                tires[1].unk24 = triggerPtGroups.pos[g1].y;
            }
        } else {
            tires[1].unk10 = g1;
            if (dd < 0) {
                tires[1].unk0 = 1;
                tires[1].unk14 = 0;
                tires[1].unk26 = 0;
                tires[1].unk16 = dy;
                tires[1].unk24 = triggerPtGroups.pos[g1].y;
                tires[1].unk3 = 1;
            }
        }
    }
    if (tires[2].unk10 != g2) {
        y = triggerPtGroups.pos[g2].y;
        dy = tires[2].unk40 - y;
        dd = y - triggerPtGroups.pos[tires[2].unk10].y;
        if (dd > 0) {
            if ((-lim < dy || tires[0].unk10 == g2 || tires[1].unk10 == g2 || tires[3].unk10 == g2)
                && m->vel.vy != 0) {
                tires[2].unk10 = g2;
                if (tires[2].unk0 == 0) {
                    tires[2].unk0 = 1;
                    if (tires[2].unk08 >= numCheckBridges) {
                        tires[2].unk08--;
                    } else {
                        tires[2].unk08++;
                    }
                }
                tires[2].unk26 = 0;
                tires[2].unk16 = dy;
                tires[2].unk24 = triggerPtGroups.pos[g2].y;
            }
        } else {
            tires[2].unk10 = g2;
            if (dd < 0) {
                tires[2].unk0 = 1;
                tires[2].unk14 = 0;
                tires[2].unk26 = 0;
                tires[2].unk16 = dy;
                tires[2].unk24 = triggerPtGroups.pos[g2].y;
                tires[2].unk3 = 1;
            }
        }
    }
    if (tires[3].unk10 != g3) {
        y = triggerPtGroups.pos[g3].y;
        dy = tires[3].unk40 - y;
        dd = y - triggerPtGroups.pos[tires[3].unk10].y;
        if (dd > 0) {
            if (-lim < dy || tires[0].unk10 == g3 || tires[1].unk10 == g3 || tires[2].unk10 == g3) {
                if (m->vel.vy != 0) {
                    tires[3].unk10 = g3;
                    if (tires[3].unk0 == 0) {
                        tires[3].unk0 = 1;
                        if (tires[3].unk08 >= numCheckBridges) {
                            tires[3].unk08--;
                        } else {
                            tires[3].unk08++;
                        }
                    }
                    tires[3].unk26 = 0;
                    tires[3].unk16 = dy;
                    tires[3].unk24 = triggerPtGroups.pos[g3].y;
                }
            }
        } else {
            tires[3].unk10 = g3;
            if (dd < 0) {
                tires[3].unk0 = 1;
                tires[3].unk14 = 0;
                tires[3].unk26 = 0;
                tires[3].unk16 = dy;
                tires[3].unk24 = triggerPtGroups.pos[g3].y;
                tires[3].unk3 = 1;
            }
        }
    }
    if (isPlayer) {
        if (g0 == g1 && g0 == g2 && g0 == g3) {
            PLAYER_CAR(car)->unkC0 = g0;
        }
    } else {
        AI_CAR(car)->unk176 = g0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CalcTireCoordinates);
#endif

#ifdef NON_MATCHING
s16 GetGroupTestPointIsIn(s32 x, s32 z, s32 y)
{
    s16 group;
    s32 i;
    s16 n;

    if (uaUsingGroupGroundHeight()) {
        n = triggerPtGroups.count;
        group = n;
        for (i = 0; i < n; i++) {
            if (x < triggerPtGroups.pos[i].x) {
                continue;
            }
            if (triggerPtGroups.pos[i].x + triggerPtGroups.w[i] < x) {
                continue;
            }
            if (z < triggerPtGroups.pos[i].z) {
                continue;
            }
            if (triggerPtGroups.pos[i].z + triggerPtGroups.d[i] < z) {
                continue;
            }
            group = i;
            if (triggerPtGroups.pos[i].y < y + 80) {
                return group;
            }
        }
    } else {
        group = 0;
    }
    return group;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetGroupTestPointIsIn);
#endif

#ifdef NON_MATCHING
void CheckBridges(Car* car, u8 isPlayer)
{
    CarMotion* m;
    CarStats* st;
    u8* flags;
    CarTire* tires;
    u8 padFlag;
    CarCollision* collision;
    s32 d;
    s32 h;
    s32 v;
    u8 ok;
    u8 any3;
    u8 any0;
    s32 hit;
    s16 other;
    s16 mine;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        collision = &PLAYER_CAR(car)->collision;
        padFlag = PLAYER_CAR(car)->skid[0x15];
        st = &PLAYER_CAR(car)->stats;
        flags = PLAYER_CAR(car)->flags;
        tires = PLAYER_CAR(car)->tires;
    } else {
        m = &AI_CAR(car)->motion;
        collision = &AI_CAR(car)->collision;
        padFlag = 0;
        st = &AI_CAR(car)->stats;
        flags = AI_CAR(car)->flags;
        tires = AI_CAR(car)->tires;
    }
    d = m->vel.vy / 32;
    if (flags[10] == 0) {
        h = st->unk5C + __builtin_abs(d);
    } else {
        h = 0xA0;
    }

    if (tires[0].unk0) {
        if (GetBridgeGroundHeight(&tires[0], tires[0].unk08, h) == 0) {
            tires[0].unk22 = tires[0].unk24;
        }
        if (tires[0].unk3 != 0 && tires[0].unk08 < numCheckBridges) {
            CheckForBridge(&tires[0], 0, h);
        }
        if (tires[0].bombDamaged == 0 && tires[0].catapulted == 0) {
            if (d < (s16)tires[0].unk14) {
                tires[0].unk14 = d;
            } else if ((s16)tires[0].unk14 < -d) {
                tires[0].unk14 = -d;
            }
        }
        GetBridgeTireHeight(&tires[0], h);
        if (isPlayer && tires[0].unk3 == 0 && tires[0].unk26 < 2) {
            tires[0].unk14 = (bridges[tires[0].unk08].unk0A * m->vel.vy) / st->unkA8;
        }
        if ((tires[0].unk12 & 7) == 0) {
            tires[0].unk12++;
        }
    } else {
        tires[0].unk14 = -1;
        CheckForBridge(&tires[0], 0, h);
        if (tires[0].unk3 != 0) {
            GetBridgeTireHeight(&tires[0], h);
        }
    }

    if (tires[1].unk0) {
        if (GetBridgeGroundHeight(&tires[1], tires[1].unk08, h) == 0) {
            tires[1].unk22 = tires[1].unk24;
        }
        if (tires[1].unk3 != 0 && tires[1].unk08 < numCheckBridges) {
            CheckForBridge(&tires[1], 0, h);
        }
        if (tires[1].bombDamaged == 0 && tires[1].catapulted == 0) {
            if (d < (s16)tires[1].unk14) {
                tires[1].unk14 = d;
            } else if ((s16)tires[1].unk14 < -d) {
                tires[1].unk14 = -d;
            }
        }
        GetBridgeTireHeight(&tires[1], h);
        if (isPlayer && tires[1].unk3 == 0 && tires[1].unk26 < 2) {
            tires[1].unk14 = (bridges[tires[1].unk08].unk0A * m->vel.vy) / st->unkA8;
        }
        if ((tires[1].unk12 & 7) == 0) {
            tires[1].unk12++;
        }
    } else {
        tires[1].unk14 = -1;
        CheckForBridge(&tires[1], 0, h);
        if (tires[1].unk3 != 0) {
            GetBridgeTireHeight(&tires[1], h);
        }
    }

    if (tires[2].unk0) {
        if (GetBridgeGroundHeight(&tires[2], tires[2].unk08, h) == 0) {
            tires[2].unk22 = tires[2].unk24;
        }
        if (tires[2].unk3 != 0 && tires[2].unk08 < numCheckBridges) {
            CheckForBridge(&tires[2], 0, h);
        }
        if (tires[2].bombDamaged == 0 && tires[2].catapulted == 0) {
            if (d < (s16)tires[2].unk14) {
                tires[2].unk14 = d;
            } else if ((s16)tires[2].unk14 < -d) {
                tires[2].unk14 = -d;
            }
        }
        GetBridgeTireHeight(&tires[2], h);
        if (isPlayer && tires[2].unk3 == 0 && tires[2].unk26 < 2) {
            tires[2].unk14 = (bridges[tires[2].unk08].unk0A * m->vel.vy) / st->unkA8;
        }
    } else {
        tires[2].unk14 = -1;
        CheckForBridge(&tires[2], 0, h);
        if (tires[2].unk3 != 0) {
            GetBridgeTireHeight(&tires[2], h);
        }
    }

    if (tires[3].unk0) {
        if (GetBridgeGroundHeight(&tires[3], tires[3].unk08, h) == 0) {
            tires[3].unk22 = tires[3].unk24;
        }
        if (tires[3].unk3 != 0 && tires[3].unk08 < numCheckBridges) {
            CheckForBridge(&tires[3], 0, h);
        }
        if (tires[3].bombDamaged == 0 && tires[3].catapulted == 0) {
            if (d < (s16)tires[3].unk14) {
                tires[3].unk14 = d;
            } else if ((s16)tires[3].unk14 < -d) {
                tires[3].unk14 = -d;
            }
        }
        GetBridgeTireHeight(&tires[3], h);
        if (isPlayer && tires[3].unk3 == 0 && tires[3].unk26 < 2) {
            tires[3].unk14 = (bridges[tires[3].unk08].unk0A * m->vel.vy) / st->unkA8;
        }
    } else {
        tires[3].unk14 = -1;
        CheckForBridge(&tires[3], 0, h);
        if (tires[3].unk3 != 0) {
            GetBridgeTireHeight(&tires[3], h);
        }
    }

    ok = 0;
    if (isPlayer) {
        if (flags[23] == 0 && flags[11] == 0) {
            v = m->vel.vy;
            if (v < 0) {
                v = -v;
            }
            ok = (v >= st->unkAC) || (padFlag == 0);
        }
    } else if (flags[23] == 0) {
        ok = flags[0] != 0;
    }
    if (ok) {
        if (m->rot.vx >= 57) {
            if (m->rot.vx >= 228) {
                m->vel.vy = m->vel.vy + 3 * st->unkA0;
            } else if (m->rot.vx >= 114) {
                m->vel.vy = m->vel.vy + 2 * st->unkA0;
            } else {
                m->vel.vy = m->vel.vy + st->unkA0;
            }
        } else if (m->rot.vx < -56) {
            if (m->rot.vx < -227) {
                m->vel.vy = m->vel.vy - 3 * st->unkA0;
            } else if (m->rot.vx < -113) {
                m->vel.vy = m->vel.vy - 2 * st->unkA0;
            } else {
                m->vel.vy = m->vel.vy - st->unkA0;
            }
        }
    }
    if (isPlayer) {
        v = m->vel.vy;
        if (v < 0) {
            v = -v;
        }
        if (st->unkAC < v) {
            if (m->rot.vy >= 171) {
                if (m->vel.vy <= 0) {
                    if (m->rotDelta.vz < st->unkEC) {
                        m->rotDelta.vz = m->rotDelta.vz + st->unkCC / 2;
                    }
                } else if (-st->unkEC < m->rotDelta.vz) {
                    m->rotDelta.vz = m->rotDelta.vz - st->unkCC / 2;
                }
            } else if (m->rot.vy < -170) {
                if (m->vel.vy > 0) {
                    if (m->rotDelta.vz < st->unkEC) {
                        m->rotDelta.vz = m->rotDelta.vz + st->unkCC / 2;
                    }
                } else if (-st->unkEC < m->rotDelta.vz) {
                    m->rotDelta.vz = m->rotDelta.vz - st->unkCC / 2;
                }
            }
        }
    }

    any3 = 0;
    if (tires[0].unk3 != 0 || tires[1].unk3 != 0 || tires[2].unk3 != 0 || tires[3].unk3 != 0) {
        any3 = 1;
    }
    flags[11] = any3;
    if (flags[11]) {
        SetFullCarDrift(m);
    }
    any0 = 0;
    hit = 0;
    if (tires[0].unk0 != 0 || tires[1].unk0 != 0 || tires[2].unk0 != 0 || tires[3].unk0 != 0) {
        any0 = 1;
        hit = 1;
    }
    if (hit) {
        if (flags[10] == 0) {
            collision->unk12 = 0xE;
        }
    } else if (flags[10] != 0) {
        SetBounce(car, 3, 2, 1, isPlayer);
        collision->unk12 = 0x27;
    }
    flags[10] = any0;
    if (flags[10] == 0) {
        return;
    }

    if (tires[0].unk6 != 0 && (tires[1].unk0 != 0 || tires[2].unk0 != 0 || tires[3].unk0 != 0)) {
        mine = tires[0].unk08;
        if (tires[1].unk0 != 0) {
            other = tires[1].unk08;
        } else if (tires[2].unk0 != 0) {
            other = tires[2].unk08;
        } else {
            other = tires[3].unk08;
        }
        if (mine == other) {
            tires[0].unk0 = 1;
        }
    }
    if (tires[1].unk6 != 0 && (tires[0].unk0 != 0 || tires[2].unk0 != 0 || tires[3].unk0 != 0)) {
        mine = tires[1].unk08;
        if (tires[0].unk0 != 0) {
            other = tires[0].unk08;
        } else if (tires[2].unk0 != 0) {
            other = tires[2].unk08;
        } else {
            other = tires[3].unk08;
        }
        if (mine == other) {
            tires[1].unk0 = 1;
        }
    }
    if (tires[2].unk6 != 0 && (tires[0].unk0 != 0 || tires[1].unk0 != 0 || tires[3].unk0 != 0)) {
        mine = tires[2].unk08;
        if (tires[0].unk0 != 0) {
            other = tires[0].unk08;
        } else if (tires[1].unk0 != 0) {
            other = tires[1].unk08;
        } else {
            other = tires[3].unk08;
        }
        if (mine == other) {
            tires[2].unk0 = 1;
        }
    }
    if (tires[3].unk6 != 0 && (tires[0].unk0 != 0 || tires[1].unk0 != 0 || tires[2].unk0 != 0)) {
        mine = tires[3].unk08;
        if (tires[0].unk0 != 0) {
            other = tires[0].unk08;
        } else if (tires[1].unk0 != 0) {
            other = tires[1].unk08;
        } else {
            other = tires[2].unk08;
        }
        if (mine == other) {
            tires[3].unk0 = 1;
        }
    }
    if (tires[0].unk0 != 0 && numCheckBridges < tires[0].unk08) {
        if (tires[1].unk0 == 0) {
            GetBridgeGroundHeight(&tires[1], tires[0].unk08, h);
        }
        if (tires[2].unk0 == 0) {
            GetBridgeGroundHeight(&tires[2], tires[0].unk08, h);
        }
        if (tires[3].unk0 == 0) {
            GetBridgeGroundHeight(&tires[3], tires[0].unk08, h);
        }
    }
    if (tires[1].unk0 != 0 && numCheckBridges < tires[1].unk08) {
        if (tires[0].unk0 == 0) {
            GetBridgeGroundHeight(&tires[0], tires[1].unk08, h);
        }
        if (tires[2].unk0 == 0) {
            GetBridgeGroundHeight(&tires[2], tires[1].unk08, h);
        }
        if (tires[3].unk0 == 0) {
            GetBridgeGroundHeight(&tires[3], tires[1].unk08, h);
        }
    }
    if (tires[2].unk0 != 0 && numCheckBridges < tires[2].unk08) {
        if (tires[0].unk0 == 0) {
            GetBridgeGroundHeight(&tires[0], tires[2].unk08, h);
        }
        if (tires[1].unk0 == 0) {
            GetBridgeGroundHeight(&tires[1], tires[2].unk08, h);
        }
        if (tires[3].unk0 == 0) {
            GetBridgeGroundHeight(&tires[3], tires[2].unk08, h);
        }
    }
    if (tires[3].unk0 != 0 && numCheckBridges < tires[3].unk08) {
        if (tires[0].unk0 == 0) {
            GetBridgeGroundHeight(&tires[0], tires[3].unk08, h);
        }
        if (tires[1].unk0 == 0) {
            GetBridgeGroundHeight(&tires[1], tires[3].unk08, h);
        }
        if (tires[2].unk0 == 0) {
            GetBridgeGroundHeight(&tires[2], tires[3].unk08, h);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckBridges);
#endif

#ifdef NON_MATCHING
void CheckForBridge(CarTire* tire, u8 which, s32 h)
{
    s16 i;
    s16 n;

    if (which) {
        n = numBridges;
    } else {
        n = numCheckBridges;
    }
    for (i = 0; i < n; i++) {
        if (GetBridgeGroundHeight(tire, i, h)) {
            break;
        }
    }
    if (tire->unk0 == 0) {
        tire->unk22 = tire->unk24;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckForBridge);
#endif

#ifdef NON_MATCHING
u8 GetBridgeGroundHeight(CarTire* tire, s16 idx, s32 h)
{
    u8 r;
    s32 k;
    s32 x0;
    s32 z0;
    s16 i;

    r = 0;
    if (idx < 0) {
        return r;
    }
    if (numBridges < idx) {
        return r;
    }
    x0 = bridges[idx].minX;
    if (tire->unk38 >= x0) {
        z0 = bridges[idx].minZ;
        if (tire->unk3C >= z0 && x0 + bridges[idx].extentX >= tire->unk38
            && z0 + bridges[idx].extentZ >= tire->unk3C) {
            tire->unk22 = tire->unk24;
            k = bridges[idx].rampDir;
            if (k == 0) {
                tire->unk22 = tire->unk22 + bridges[idx].y0
                    + (bridges[idx].y1 - bridges[idx].y0) * (tire->unk3C - bridges[idx].minZ)
                        / bridges[idx].extentZ;
            } else if (k == 1) {
                tire->unk22 = tire->unk22
                    + (bridges[idx].y1
                        + (bridges[idx].y0 - bridges[idx].y1)
                            * (bridges[idx].minZ + bridges[idx].extentZ - tire->unk3C)
                            / bridges[idx].extentZ);
            } else if (k == 2) {
                tire->unk22 = tire->unk22 + bridges[idx].y0
                    + (bridges[idx].y1 - bridges[idx].y0) * (tire->unk38 - bridges[idx].minX)
                        / bridges[idx].extentX;
            } else {
                tire->unk22 = tire->unk22
                    + (bridges[idx].y1
                        + (bridges[idx].y0 - bridges[idx].y1)
                            * (bridges[idx].minX + bridges[idx].extentX - tire->unk38)
                            / bridges[idx].extentX);
            }
            if (h < (s16)tire->unk22 - tire->unk40) {
                tire->unk6 = 1;
                tire->unk08 = idx;
                tire->unk22 = tire->unk24;
                return 0;
            }
            tire->unk08 = idx;
            tire->unk6 = 0;
            tire->unk0E = bridges[idx].unk08;
            if (tire->unk0 == 0) {
                if ((s16)tire->unk22 < (s16)tire->unk24) {
                    tire->unk3 = 1;
                }
                tire->unk26 = 0;
            }
            tire->unk0 = 1;
            return 1;
        }
    }
    if (tire->unk08 != idx) {
        return r;
    }
    if (tire->unk0 == 0) {
        if (tire->unk6 == 0) {
            return r;
        }
        tire->unk6 = 0;
        return r;
    }
    if (tire->unk3 == 0) {
        tire->unk3 = 1;
        tire->unk22 = tire->unk24;
        tire->unk12 = 1;
        if (tire->unk26 < 2) {
            tire->unk14 = bridges[tire->unk08].unk0A;
        }
    }
    i = 0;
    while (bridges[idx].obj[i] != -1) {
        r = GetBridgeGroundHeight(tire, bridges[idx].obj[i], h);
        i++;
        if (i >= 4) {
            return r;
        }
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetBridgeGroundHeight);
#endif

#ifdef NON_MATCHING
void GetBridgeTireHeight(CarTire* tire, s32 h)
{
    s32 d;

    if (tire->unk3) {
        if ((s16)tire->unk12 > 0) {
            tire->unk28 = ~(GetFieldsLastFrame() * 2) * ((s16)tire->unk12 * (s16)tire->unk12);
        } else {
            tire->unk28 = 0;
        }
        if (tire->unk28 < -4096 - ((s16)tire->unk14 * 32)) {
            tire->unk28 = -4096 - ((s16)tire->unk14 * 32);
        } else {
            tire->unk12++;
        }
        d = tire->unk28;
        tire->unk16 = tire->unk16 + (tire->unk14 + d / 32);
        if ((s16)tire->unk24 + tire->unk16 <= (s16)tire->unk22) {
            tire->unk3 = 0;
            if (GetBridgeGroundHeight(tire, tire->unk08, h) == 0) {
                tire->unk0 = 0;
                tire->unk3 = 0;
                tire->unk16 = 0;
                tire->unk0E = 0;
                CheckForBridge(tire, 0, h);
                if (tire->unk0 == 0) {
                    tire->unk22 = tire->unk24;
                    tire->unk16 = 0;
                    tire->unk14 = -1;
                }
            }
        }
        tire->unk26 = 0;
    } else {
        tire->unk14 = tire->unk22 - (tire->unk24 + tire->unk16);
        (tire->unk26)++;
        if (h < (s16)tire->unk14) {
            tire->unk14 = h;
        } else if ((s16)tire->unk14 < -h) {
            tire->unk14 = -h;
        }
        tire->unk16 = tire->unk22 - tire->unk24;
        tire->unk12 = 1;
        tire->unk3 = 0;
        if (GetBridgeGroundHeight(tire, tire->unk08, h) == 0) {
            tire->unk0 = 0;
            tire->unk3 = 0;
            tire->unk16 = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetBridgeTireHeight);
#endif

void CheckPotHoles(Car* car, u8 isPlayer)
{
    CarTire* tires;
    u8* flags;
    CarCollision* collision;

    if (isPlayer) {
        tires = PLAYER_CAR(car)->tires;
        flags = PLAYER_CAR(car)->flags;
        collision = &PLAYER_CAR(car)->collision;
    } else {
        tires = AI_CAR(car)->tires;
        flags = AI_CAR(car)->flags;
        collision = &AI_CAR(car)->collision;
    }
    if (tires[0].unk3 == 0) {
        CheckForPotHole(&tires[0], collision);
    }
    if (tires[1].unk3 == 0) {
        CheckForPotHole(&tires[1], collision);
    }
    if (tires[2].unk3 == 0) {
        CheckForPotHole(&tires[2], collision);
    }
    if (tires[3].unk3 == 0) {
        CheckForPotHole(&tires[3], collision);
    }
    flags[12] = tires[0].unk1 || tires[1].unk1 || tires[2].unk1 || tires[3].unk1;
}

#ifdef NON_MATCHING
void CheckForPotHole(CarTire* tire, CarCollision* collision)
{
    s16 i;
    u8 found;

    found = 0;
    for (i = 0; i < numCheckPotHoles; i++) {
        if (tire->unk38 < potHoles[i].x) {
            continue;
        }
        if (tire->unk3C < potHoles[i].z) {
            continue;
        }
        if (potHoles[i].x + potHoles[i].a < tire->unk38) {
            continue;
        }
        if (potHoles[i].z + potHoles[i].b < tire->unk3C) {
            continue;
        }
        if (tire->unk1 == 0) {
            collision->unk12 = 0x1D;
        }
        found = 1;
        tire->unk1 = 1;
        tire->unk0A = i;
        break;
    }
    if (found) {
        if (potHoles[(s16)tire->unk0A].active) {
            tire->unk18 = potHoles[(s16)tire->unk0A].c;
        } else {
            tire->unk18 = (potHoles[(s16)tire->unk0A].c
                              * (((tire->unk38 - potHoles[(s16)tire->unk0A].x) % 3)
                                  + ((tire->unk3C - potHoles[(s16)tire->unk0A].z) % 3)))
                / 6;
        }
    } else {
        tire->unk18 = 0;
    }
    tire->unk1 = found;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckForPotHole);
#endif

void CheckCurbs(Car* car, u8 isPlayer)
{
    CarTire* tires;
    u8* flags;
    CarCollision* collision;

    if (isPlayer) {
        tires = PLAYER_CAR(car)->tires;
        flags = PLAYER_CAR(car)->flags;
        collision = &PLAYER_CAR(car)->collision;
    } else {
        tires = AI_CAR(car)->tires;
        flags = AI_CAR(car)->flags;
        collision = &AI_CAR(car)->collision;
    }
    if (tires[0].unk3 == 0) {
        CheckForCurb(&tires[0], collision);
    }
    if (tires[1].unk3 == 0) {
        CheckForCurb(&tires[1], collision);
    }
    if (tires[2].unk3 == 0) {
        CheckForCurb(&tires[2], collision);
    }
    if (tires[3].unk3 == 0) {
        CheckForCurb(&tires[3], collision);
    }
    flags[14] = tires[0].unk2 || tires[1].unk2 || tires[2].unk2 || tires[3].unk2;
}

#ifdef NON_MATCHING
void CheckForCurb(CarTire* tire, CarCollision* collision)
{
    s16 i;
    u8 found;

    found = 0;
    for (i = 0; i < numCurbs; i++) {
        if (tire->unk38 < curbs[i].x) {
            continue;
        }
        if (tire->unk3C < curbs[i].z) {
            continue;
        }
        if (curbs[i].x + curbs[i].w < tire->unk38) {
            continue;
        }
        if (curbs[i].z + curbs[i].h < tire->unk3C) {
            continue;
        }
        if (tire->unk2 == 0) {
            collision->unk12 = 0x11;
        }
        found = 1;
        tire->unk2 = 1;
        tire->unk0C = i;
        break;
    }
    if (found) {
        tire->unk1A = curbs[tire->unk0C].t;
    } else {
        tire->unk1A = 0;
    }
    if (tire->unk2 != 0 && found == 0) {
        collision->unk12 = 0xD;
    }
    tire->unk2 = found;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckForCurb);
#endif

#ifdef NON_MATCHING
void CheckSlickSpots(Car* car, u8 isPlayer)
{
    CarMotion* m;
    u8* flags;
    CarStats* st;
    s16 i;
    s16 n;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        flags = PLAYER_CAR(car)->flags;
        st = &PLAYER_CAR(car)->stats;
    } else {
        m = &AI_CAR(car)->motion;
        flags = AI_CAR(car)->flags;
        st = &AI_CAR(car)->stats;
    }
    if (flags[18]) {
        st->unk1C -= GetFieldsLastFrame();
        if (st->unk1C <= 0) {
            flags[13] = 0;
            flags[18] = 0;
        } else {
            flags[13] = 1;
        }
    } else {
        flags[13] = 0;
    }
    n = numSlickSpots;
    for (i = 0; i < n; i++) {
        if (m->pos.vx < slickSpots[i].x) {
            continue;
        }
        if (m->pos.vy < slickSpots[i].z) {
            continue;
        }
        if (slickSpots[i].x + slickSpots[i].w < m->pos.vx) {
            continue;
        }
        if (slickSpots[i].z + slickSpots[i].h < m->pos.vy) {
            continue;
        }
        flags[13] = 1;
        return;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckSlickSpots);
#endif

#ifdef NON_MATCHING
void CheckHealthStands(Car* car, u8 isPlayer)
{
    CarMotion* m;
    CarTire* tires;
    u8* flags;
    s8* sel;
    CarStats* st;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        tires = PLAYER_CAR(car)->tires;
        flags = PLAYER_CAR(car)->flags;
        sel = &PLAYER_CAR(car)->standId;
        st = &PLAYER_CAR(car)->stats;
    } else {
        m = &AI_CAR(car)->motion;
        tires = AI_CAR(car)->tires;
        flags = AI_CAR(car)->flags;
        sel = &AI_CAR(car)->unk04;
        st = &AI_CAR(car)->stats;
    }
    if (!isPlayer) {
        flags[22] = 0;
        tires[0].unk0E = 0;
        tires[1].unk0E = 0;
        tires[2].unk0E = 0;
        tires[3].unk0E = 0;
    }
    if (flags[22]) {
        if (health_stand_active(*sel) == 0) {
            st->unk16 -= GetFieldsLastFrame();
        } else {
            st->unk16 = 0x3C;
        }
        if ((s16)st->unk16 <= 0) {
            flags[22] = 0;
            tires[0].unk0E = 0;
            tires[1].unk0E = 0;
            tires[2].unk0E = 0;
            tires[3].unk0E = 0;
        }
        return;
    }
    if (st->unk40 <= 0) {
        return;
    }
    if (m->vel.vy > 0) {
        if (tires[2].unk0E != 0) {
            flags[22] = 1;
            *sel = tires[2].unk0E;
        } else if (tires[3].unk0E != 0) {
            flags[22] = 1;
            *sel = tires[3].unk0E;
        }
    } else {
        if (tires[0].unk0E != 0 && health_stand_active(tires[0].unk0E)) {
            flags[22] = 1;
            *sel = tires[0].unk0E;
        } else if (tires[1].unk0E != 0 && health_stand_active(tires[1].unk0E)) {
            flags[22] = 1;
            *sel = tires[1].unk0E;
        }
    }
    if (flags[22]) {
        st->healthCap = st->unk40 + st->unk50;
        st->healthRate = ((st->healthCap - st->unk50) * GetFieldsLastFrame()) / 180;
        if (st->healthRate <= 0) {
            st->healthRate = 1;
        }
        return;
    }
    if (!isPlayer) {
        return;
    }
    if (st->unk01 != 0) {
        return;
    }
    if (m->vel.vy != 0) {
        return;
    }
    switch (shellGetCurrentLevel()) {
    case 1:
        if (m->pos.vx >= -12464) {
            if (m->pos.vx < -12399) {
                if (m->pos.vy >= -4360) {
                    if (m->pos.vy < -4239) {
                        st->unk01 = 1;
                        st->unk40 = st->unk44;
                    }
                }
            }
        }
        break;
    case 2:
        if (m->pos.vx >= 16288) {
            if (m->pos.vx < 16337) {
                if (m->pos.vy >= -7600) {
                    if (m->pos.vy < -7439) {
                        st->unk01 = 1;
                        st->unk40 = st->unk44;
                    }
                }
            }
        }
        break;
    case 3:
        if (m->pos.vx >= 11840) {
            if (m->pos.vx < 12161) {
                if (m->pos.vy >= -4000) {
                    if (m->pos.vy < -3919) {
                        st->unk01 = 1;
                        st->unk40 = st->unk44;
                    }
                }
            }
        }
        break;
    case 4:
        if (m->pos.vx >= 22640) {
            if (m->pos.vx < 22673) {
                if (m->pos.vy >= 11632) {
                    if (m->pos.vy < 11793) {
                        st->unk01 = 1;
                        st->unk40 = st->unk44;
                    }
                }
            }
        }
        break;
    case 5:
        if (m->pos.vx >= 8272) {
            if (m->pos.vx < 8345) {
                if (m->pos.vy >= 10072) {
                    if (m->pos.vy < 10145) {
                        st->unk01 = 1;
                        st->unk40 = st->unk44;
                    }
                }
            }
        }
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckHealthStands);
#endif

#ifdef NON_MATCHING
void UpdateTirePositions(Car* car, u8 isPlayer)
{
    CarMotion* m;
    CarStats* st;
    CarTire* tires;
    u8* flags;
    CarBounce* bounce;
    AICar* ai;
    s32 uaIndex;
    s32 spd;
    s32 hi;
    s32 lo;
    s32 lim;
    s32 d0;
    s32 d1;
    s32 t;
    s32 k;
    s32 p;
    s32 q;
    s32 ang;
    s32 ang2;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        st = &PLAYER_CAR(car)->stats;
        tires = PLAYER_CAR(car)->tires;
        flags = PLAYER_CAR(car)->flags;
        uaIndex = PLAYER_CAR(car)->uaIndex;
        bounce = &PLAYER_CAR(car)->bounce;
        spd = 0;
        ai = 0;
    } else {
        m = &AI_CAR(car)->motion;
        st = &AI_CAR(car)->stats;
        tires = AI_CAR(car)->tires;
        flags = AI_CAR(car)->flags;
        bounce = &AI_CAR(car)->bounce;
        spd = AI_CAR(car)->unk38;
        ai = AI_CAR(car);
        uaIndex = AI_CAR(car)->uaIndex;
    }
    tires[0].unk40 = (s16)tires[0].unk24 + tires[0].unk16 + tires[0].unk18 + (s16)tires[0].unk1C
        + (s16)tires[0].unk1E + tires[0].unk20;
    if (tires[0].unk0 == 0) {
        tires[0].unk40 += tires[0].unk1A;
    }
    tires[1].unk40 = (s16)tires[1].unk24 + tires[1].unk16 + tires[1].unk18 + (s16)tires[1].unk1C
        + (s16)tires[1].unk1E + tires[1].unk20;
    if (tires[1].unk0 == 0) {
        tires[1].unk40 += tires[1].unk1A;
    }
    tires[2].unk40 = (s16)tires[2].unk24 + tires[2].unk16 + tires[2].unk18 + (s16)tires[2].unk1C
        + (s16)tires[2].unk1E + tires[2].unk20;
    if (tires[2].unk0 == 0) {
        tires[2].unk40 += tires[2].unk1A;
    }
    tires[3].unk40 = (s16)tires[3].unk24 + tires[3].unk16 + tires[3].unk18 + (s16)tires[3].unk1C
        + (s16)tires[3].unk1E + tires[3].unk20;
    if (tires[3].unk0 == 0) {
        tires[3].unk40 += tires[3].unk1A;
    }
    if (tires[0].unk40 >= tires[1].unk40) {
        hi = tires[0].unk40;
    } else {
        hi = tires[1].unk40;
    }
    if (tires[2].unk40 >= tires[3].unk40) {
        lo = tires[2].unk40;
    } else {
        lo = tires[3].unk40;
    }
    if (hi >= lo) {
        lo = hi;
    }
    lim = lo - 3 * st->unk78 / 4;
    if (tires[0].unk40 < lim) {
        tires[0].unk40 = lim;
    }
    if (tires[1].unk40 < lim) {
        tires[1].unk40 = lim;
    }
    if (tires[2].unk40 < lim) {
        tires[2].unk40 = lim;
    }
    if (tires[3].unk40 < lim) {
        tires[3].unk40 = lim;
    }
    if (flags[23] != 0) {
        UpdateRollingCar(car, isPlayer);
    } else if (flags[0] == 0) {
        m->rot.vy = ratan2(
            (tires[1].unk40 - tires[0].unk40 + tires[3].unk40 - tires[2].unk40) / 2, st->unk74);
        if (m->rot.vy >= 854) {
            m->rot.vy = 853;
        } else if (m->rot.vy < -853) {
            m->rot.vy = -853;
        }
        d0 = 0;
        d1 = 0;
        if (tires[0].unk0 != tires[1].unk0 && tires[2].unk0 != tires[3].unk0) {
            d0 = (s16)tires[0].unk14 / GetFieldsLastFrame();
            d1 = (s16)tires[1].unk14 / GetFieldsLastFrame();
        }
        if (d0 + st->unk5C < d1 && st->unkC0 < m->vel.vy) {
            flags[23] = 1;
            m->unk02 = m->rot2.vz;
            m->rotDelta.vy = 1;
            m->unk04 = m->rot2.vz + 1024;
        } else if (d1 + st->unk5C < d0 && st->unkA8 / 2 < m->vel.vy) {
            flags[23] = 1;
            m->unk02 = m->rot2.vz;
            m->rotDelta.vy = -1;
            m->unk04 = m->rot2.vz - 1024;
        }
    }
    if (flags[0] == 0 || flags[10] != 0) {
        m->rot.vx = -ratan2(
            (tires[0].unk40 - tires[2].unk40 + tires[1].unk40 - tires[3].unk40) / 2, st->unk78);
    } else {
        m->rot.vx = SmoothAngleValue(m->rot.vx, 0, 98);
    }
    if (m->rot.vx >= 513) {
        m->rot.vx = 512;
    } else if (m->rot.vx < -512) {
        m->rot.vx = -512;
    }
    if (flags[10] == 0) {
        m->rot.vx += bounce->unk30;
        m->rot.vy += bounce->unk34;
    }
    m->pos.vz = (tires[0].unk40 + tires[1].unk40 + tires[2].unk40 + tires[3].unk40) / 4;
    t = st->unk74;
    k = m->rot.vy;
    if (k < 0) {
        k = -k;
    }
    ang = k;
    t = t / 2 + 8;
    BoundAngle(&ang);
    if (ang >= 1024) {
        q = t * (ang - 1024);
        ang -= 1024;
        p = t - q / 1024;
        q = st->unk70 * ang / 1024;
    } else {
        p = t * ang / 1024;
        q = st->unk6C - st->unk6C * ang / 1024;
    }
    m->pos.vz += p + q;
    if (shellGetCurrentLevel() == 5 && st->unk40 > 0 && m->pos.vz < 160) {
        if (isPlayer == 0 && ai != 0) {
            if (spd >= 1601) {
                ang2 = 2048 - (GetPlayerInfo(ai->unk0E)->motion.rot.vz - ai->unk30);
                BoundAngle(&ang2);
                k = ang2;
                if (k < 0) {
                    k = -k;
                }
                if (k >= 513) {
                    spd = 8000;
                }
            }
        }
        if ((spd >= 6401 || uaIndex == 130) && ai != 0) {
            if (ai->unk16F != 0) {
                PutAICarBackOnRoof(ai);
            } else {
                ai->stats.unk40 -= 20;
                ai->unk16F = ai->stats.unk40 > 0;
            }
        } else {
            st->unk40 = -1;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", UpdateTirePositions);
#endif

#ifdef NON_MATCHING
void PutAICarBackOnRoof(AICar* car)
{
    s32 dx;
    s32 dz;
    s32 tp;

    if (GetPlayerInfo(car->unk0E)->unkC0 == 8) {
        tp = 7;
    } else {
        tp = 0x1F;
    }
    dx = car->motion.pos.vx - tPoints[tp].x;
    dz = car->motion.pos.vy - tPoints[tp].z;
    if (dx < 0) {
        dx = -dx;
    }
    if (dz < 0) {
        dz = -dz;
    }
    if (dx < 240 && dz < 240) {
        car->stats.triggerPt = tp;
        InitTriggerPoint(car);
        car->stats.unk38 = 1;
        car->unk16F = 0;
    } else {
        car->motion.pos.vx = SmoothValue(car->motion.pos.vx, tPoints[tp].x, 0x62);
        car->motion.pos.vy = SmoothValue(car->motion.pos.vy, tPoints[tp].z, 0x62);
        car->stats.unk38 = 0;
        car->motion.vel.vy = 0;
        car->motion.rotDelta.vz = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", PutAICarBackOnRoof);
#endif

#ifdef NON_MATCHING
void UpdateRollingCar(Car* car, u8 isPlayer)
{
    CarMotion* m;
    CarStats* st;
    CarTire* tires;
    u8* flags;
    s32 lim;
    s32 r;
    s32 d;
    s32 v;
    s32 w;

    if (isPlayer) {
        m = &PLAYER_CAR(car)->motion;
        st = &PLAYER_CAR(car)->stats;
        tires = PLAYER_CAR(car)->tires;
        flags = PLAYER_CAR(car)->flags;
    } else {
        m = &AI_CAR(car)->motion;
        st = &AI_CAR(car)->stats;
        tires = AI_CAR(car)->tires;
        flags = AI_CAR(car)->flags;
    }
    if (m->vel.vy < 32 * (2850 * GetFieldsLastFrame() / 100)) {
        lim = 32 * (2850 * GetFieldsLastFrame() / 100);
    } else {
        lim = m->vel.vy;
    }
    r = 91 * m->vel.vy * GetFieldsLastFrame() / lim;
    if (r < 0) {
        if (-45 * GetFieldsLastFrame() < r) {
            r = -45 * GetFieldsLastFrame();
        }
    } else if (r < 45 * GetFieldsLastFrame()) {
        r = 45 * GetFieldsLastFrame();
    }
    if (flags[11] == 0) {
        if (rand() & 1) {
            explodeCreateFragments(1, (VECTOR*)&m->pos);
        }
        if ((rand() & 1) == 0) {
            do_smoke(&m->pos);
        }
    }
    if (m->rotDelta.vy > 0) {
        v = __builtin_abs(rsin(m->rot.vy));
        d = r + v * r / 4096;
        m->rot.vy = m->rot.vy + d;
    } else {
        v = __builtin_abs(rsin(m->rot.vy));
        d = r + v * r / 4096;
        m->rot.vy = m->rot.vy - d;
    }
    v = m->vel.vy;
    if (v < 0) {
        v = -v;
    }
    if (v < 32 * (950 * GetFieldsLastFrame() / 100)) {
        v = m->rot.vy;
        w = 3 * d;
        if (w < 0) {
            w = -w;
        }
        if (v < 0) {
            v = -v;
        }
        if (v < w) {
            flags[23] = 0;
            m->rotDelta.vy = 0;
            m->rot.vy = 0;
            SetNoCarDrift(m, flags);
        }
    } else {
        w = rsin(m->rot.vy) * st->unk74 / 4096;
        tires[0].unk40 += w;
        tires[1].unk40 += w;
        tires[2].unk40 += w;
        tires[3].unk40 += w;
        if (m->vel.vy != 0) {
            if (m->vel.vy > 0) {
                m->vel.vy = m->vel.vy - st->unkBC / 3;
            } else {
                m->vel.vy = m->vel.vy + st->unkBC / 3;
            }
        }
        m->rot2.vz = (s16)m->unk02;
        flags[1] = 1;
        m->rot.vz = SmoothAngleValue(m->rot.vz, m->unk04, 95);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", UpdateRollingCar);
#endif

#ifdef NON_MATCHING
void InitTireInfo(CarTire* tires)
{
    tires[0].unk0 = 0;
    tires[0].unk1 = 0;
    tires[0].unk2 = 0;
    tires[0].catapulted = 0;
    tires[0].bombDamaged = 0;
    tires[1].unk0 = 0;
    tires[1].unk1 = 0;
    tires[1].unk2 = 0;
    tires[1].catapulted = 0;
    tires[1].bombDamaged = 0;
    tires[2].unk0 = 0;
    tires[2].unk1 = 0;
    tires[2].unk2 = 0;
    tires[2].catapulted = 0;
    tires[2].bombDamaged = 0;
    tires[3].unk0 = 0;
    tires[3].unk1 = 0;
    tires[3].unk2 = 0;
    tires[3].catapulted = 0;
    tires[3].bombDamaged = 0;
    tires[0].unk22 = 0;
    tires[1].unk22 = 0;
    tires[2].unk22 = 0;
    tires[3].unk22 = 0;
    tires[0].unk24 = 0;
    tires[1].unk24 = 0;
    tires[2].unk24 = 0;
    tires[3].unk24 = 0;
    tires[0].unk10 = 0;
    tires[1].unk10 = 0;
    tires[2].unk10 = 0;
    tires[3].unk10 = 0;
    tires[0].unk40 = 0;
    tires[0].unk16 = 0;
    tires[0].unk18 = 0;
    tires[0].unk1C = 0;
    tires[0].unk1E = 0;
    tires[0].unk20 = 0;
    tires[0].unk1A = 0;
    tires[0].unk26 = 2;
    tires[1].unk40 = 0;
    tires[1].unk16 = 0;
    tires[1].unk18 = 0;
    tires[1].unk1C = 0;
    tires[1].unk1E = 0;
    tires[1].unk20 = 0;
    tires[1].unk1A = 0;
    tires[1].unk26 = 2;
    tires[2].unk40 = 0;
    tires[2].unk16 = 0;
    tires[2].unk18 = 0;
    tires[2].unk1C = 0;
    tires[2].unk1E = 0;
    tires[2].unk20 = 0;
    tires[2].unk1A = 0;
    tires[2].unk26 = 2;
    tires[3].unk40 = 0;
    tires[3].unk16 = 0;
    tires[3].unk18 = 0;
    tires[3].unk1C = 0;
    tires[3].unk1E = 0;
    tires[3].unk20 = 0;
    tires[3].unk1A = 0;
    tires[3].unk26 = 2;
    tires[0].unk08 = 0;
    tires[0].unk0A = 0;
    tires[0].unk0E = 0;
    tires[0].unk3 = 0;
    tires[0].unk28 = 0;
    tires[0].unk12 = 0;
    tires[0].unk14 = 0;
    tires[0].unk40 = 0;
    tires[1].unk08 = 0;
    tires[1].unk0A = 0;
    tires[1].unk0E = 0;
    tires[1].unk3 = 0;
    tires[1].unk28 = 0;
    tires[1].unk12 = 0;
    tires[1].unk14 = 0;
    tires[1].unk40 = 0;
    tires[2].unk08 = 0;
    tires[2].unk0A = 0;
    tires[2].unk0E = 0;
    tires[2].unk3 = 0;
    tires[2].unk28 = 0;
    tires[2].unk12 = 0;
    tires[2].unk14 = 0;
    tires[2].unk40 = 0;
    tires[3].unk08 = 0;
    tires[3].unk0A = 0;
    tires[3].unk0E = 0;
    tires[3].unk3 = 0;
    tires[3].unk28 = 0;
    tires[3].unk12 = 0;
    tires[3].unk14 = 0;
    tires[3].unk40 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", InitTireInfo);
#endif

#ifdef NON_MATCHING
void InitTireGroup(CarTire* tires, s16 group)
{
    tires[0].unk10 = group;
    tires[1].unk10 = group;
    tires[2].unk10 = group;
    tires[3].unk10 = group;
    tires[0].unk40 = triggerPtGroups.pos[group].y;
    tires[1].unk40 = triggerPtGroups.pos[group].y;
    tires[2].unk40 = triggerPtGroups.pos[group].y;
    tires[3].unk40 = triggerPtGroups.pos[group].y;
    tires[0].unk22 = tires[0].unk40;
    tires[1].unk22 = tires[1].unk40;
    tires[2].unk22 = tires[2].unk40;
    tires[3].unk22 = tires[3].unk40;
    tires[0].unk24 = tires[0].unk40;
    tires[1].unk24 = tires[1].unk40;
    tires[2].unk24 = tires[2].unk40;
    tires[3].unk24 = tires[3].unk40;
    tires[0].unk0 = 1;
    tires[1].unk0 = 1;
    tires[2].unk0 = 1;
    tires[3].unk0 = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", InitTireGroup);
#endif

void UpdateCarOnSlickSpot(Car* car, u8 isPlayer)
{
    CarStats* st;
    CarMotion* m;
    s32 v;
    s32 lim;

    if (isPlayer) {
        st = &PLAYER_CAR(car)->stats;
        m = &PLAYER_CAR(car)->motion;
    } else {
        st = &AI_CAR(car)->stats;
        m = &AI_CAR(car)->motion;
    }
    v = m->vel.vy;
    lim = st->unkA4;
    v = __builtin_abs(v);
    if (lim < v) {
        SetFullCarDrift(m);
        return;
    }
    if (isPlayer) {
        BringBackDriftingCar(car, isPlayer);
    }
}

#ifdef NON_MATCHING
s32 GetMinSpeedNeeded(s16 group, s16 idx)
{
    s16 speed;
    s32 fields;

    speed = triggerPtGroups.minSpeed[group][idx];
    fields = GetFieldsLastFrame() * 19;
    return ((speed * fields) / 100) << 5;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetMinSpeedNeeded);
#endif

#ifdef NON_MATCHING
s16 GetNumBridges(void)
{
    return numBridges;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetNumBridges);
#endif

#ifdef NON_MATCHING
s16 MakeFakePotHole(void)
{
    numCheckPotHoles = numPotHoles + 1;
    return numPotHoles;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", MakeFakePotHole);
#endif

#ifdef NON_MATCHING
void UpdateNumPotHoles(void)
{
    if (numCheckPotHoles != numPotHoles) {
        potHoleTics += GetFieldsLastFrame();
        if (potHoleTics >= 101) {
            potHoleTics = 0;
            numCheckPotHoles = numPotHoles;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", UpdateNumPotHoles);
#endif

TriggerPtStartPts* GetTriggerPtStartPts(void)
{
    return &triggerPtStartPts;
}

#ifdef NON_MATCHING
void CalcDistFromRoadCenter(AICar* car, s32* out, u8 which)
{
    s32 a;
    u8 b;
    u8 c;
    s16 h1;
    s16 h2;
    s32 p1;
    s32 p2;
    s32 q1;
    s32 q2;
    s32 d0;
    s32 d1;

    if (which) {
        a = car->unk164;
        b = car->unk165;
        c = car->unk166;
        h1 = car->unk18E;
        h2 = car->unk190;
        p1 = car->unk198;
        p2 = car->unk19C;
        q1 = car->unk1D4;
        q2 = car->unk1D8;
    } else {
        a = car->unk167;
        b = car->unk168;
        c = car->unk169;
        h1 = car->unk192;
        h2 = car->unk194;
        p1 = car->unk1A0;
        p2 = car->unk1A4;
        q1 = car->unk1DC;
        q2 = car->unk1E0;
    }
    if (a) {
        if (h2 != 0) {
            out[0] = (car->motion.pos.vx - p1) - (h1 * (car->motion.pos.vy - p2)) / h2;
        } else {
            out[0] = 0;
        }
        if (h1 != 0) {
            out[1] = (car->motion.pos.vy - p2) - (h2 * (car->motion.pos.vx - p1)) / h1;
            d0 = out[0];
            d1 = out[1];
            if (d0 < 0) {
                d0 = -d0;
            }
            if (d1 < 0) {
                d1 = -d1;
            }
            if (d1 < d0) {
                out[0] = 0;
            } else {
                out[1] = 0;
            }
        } else {
            out[1] = 0;
        }
    } else if (b) {
        out[0] = car->motion.pos.vx - q1;
        out[1] = 0;
    } else {
        out[0] = 0;
        out[1] = car->motion.pos.vy - q2;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CalcDistFromRoadCenter);
#endif

#ifdef NON_MATCHING
void CheckMonsterSmash(Car* car, u8 isPlayer)
{
    u8* flags;
    CarMotion* m;
    CarTire* tires;
    CarStats* st;
    Cs* cs;
    u8* otherFlags;
    CarStats* otherStats;
    PlayerCar* player;
    AICar* ai;
    s32 best;
    s32 d;
    s32 dx;
    s32 dz;
    s32 v;
    VECTOR3 pos;
    u8 otherIsPlayer;
    s16 idx;

    if (isPlayer) {
        flags = PLAYER_CAR(car)->flags;
        m = &PLAYER_CAR(car)->motion;
        tires = PLAYER_CAR(car)->tires;
        st = &PLAYER_CAR(car)->stats;
    } else {
        flags = AI_CAR(car)->flags;
        m = &AI_CAR(car)->motion;
        tires = AI_CAR(car)->tires;
        st = &AI_CAR(car)->stats;
    }
    flags[27] = 0;
    best = 0x7FFF;
    cs = UAGetCs(st->unk30);
    otherFlags = 0;
    if (cs != 0) {
        dx = cs->pos.vx - tires[0].unk38;
        dz = cs->pos.vy - tires[0].unk3C;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d < st->unk2A) {
            flags[27] = 1;
            tires[0].unk20 = st->unk2C - st->unk2C * d / st->unk2A / 2;
            if (d < best) {
                best = d;
            }
        } else {
            tires[0].unk20 = 0;
        }
        dx = cs->pos.vx - tires[1].unk38;
        dz = cs->pos.vy - tires[1].unk3C;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d < st->unk2A) {
            flags[27] = 1;
            tires[1].unk20 = st->unk2C - st->unk2C * d / st->unk2A / 2;
            if (d < best) {
                best = d;
            }
        } else {
            tires[1].unk20 = 0;
        }
        dx = cs->pos.vx - tires[2].unk38;
        dz = cs->pos.vy - tires[2].unk3C;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d < st->unk2A) {
            flags[27] = 1;
            tires[2].unk20 = st->unk2C - st->unk2C * d / st->unk2A / 2;
            if (d < best) {
                best = d;
            }
        } else {
            tires[2].unk20 = 0;
        }
        dx = cs->pos.vx - tires[3].unk38;
        dz = cs->pos.vy - tires[3].unk3C;
        d = SquareRoot0(dx * dx + dz * dz);
        if (d < st->unk2A) {
            flags[27] = 1;
            tires[3].unk20 = st->unk2C - st->unk2C * d / st->unk2A / 2;
            if (d < best) {
                best = d;
            }
        } else {
            tires[3].unk20 = 0;
        }
    }
    if (cs != 0 && uaIsCarCS(cs, &otherIsPlayer, &idx)) {
        if (otherIsPlayer) {
            player = GetPlayerInfo(idx);
            otherFlags = player->flags;
            otherStats = &player->stats;
        } else {
            ai = GetAICarInfo(idx);
            otherFlags = ai->flags;
            otherStats = &ai->stats;
        }
    } else {
        otherFlags = 0;
        otherStats = 0;
    }
    if (flags[27] != 0 && best < st->unk2A / 2) {
        pos.vx = m->pos.vx + (rand() & 0x3F) - 32;
        pos.vy = m->pos.vy + (rand() & 0x3F) - 32;
        v = (rand() & 0x1F) + 32;
        pos.vz = m->pos.vz - v;
        if (pos.vz <= 0) {
            pos.vz = 1;
        }
        explodeCreateFragments(rand() & 3, (VECTOR*)&pos);
        switch (rand() & 0xF) {
        case 0:
            do_smoke(&pos);
            break;
        case 1:
            do_big_smoke(&pos);
            break;
        case 2:
            do_simple_explosion(&pos);
            break;
        case 3:
            do_steam(&pos);
            break;
        }
        if (otherFlags != 0 && otherStats->unk4C != 0 && (GetFrameCount() & 3) == 0
            && otherStats->unk40 > 0) {
            otherStats->unk40--;
        }
    }
    if (flags[27] == 0) {
        tires[0].unk20 = 0;
        tires[1].unk20 = 0;
        tires[2].unk20 = 0;
        tires[3].unk20 = 0;
        if (otherFlags != 0) {
            otherFlags[28] = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CheckMonsterSmash);
#endif

#ifdef NON_MATCHING
void InitTriggerPoint(AICar* car)
{
    s16 i;
    s16 next;

    GetNextTriggerPoint(
        car, tPoints[car->stats.triggerPt].link[rand() % tPoints[car->stats.triggerPt].f1A]);
    AICarInitTransition(car);
    CalcTurnStart(car, 0);
    UpdateCurrentTriggerPt(car);
    GetNextTriggerPoint(car, car->stats.triggerPt);
    AICarInitTransition(car);
    CalcTurnStart(car, 0);
    UpdateCurrentTriggerPt(car);
    next = 0;
    for (i = 0; i < tPoints[car->stats.triggerPt].f1A; i++) {
        if (tPoints[car->stats.triggerPt].link[i] == car->stats.unk68) {
            next = i + 1;
            if (next >= tPoints[car->stats.triggerPt].f1A) {
                next = 0;
            }
            break;
        }
    }
    car->stats.unk68 = tPoints[car->stats.triggerPt].link[next];
    GetNextTriggerPoint(car, car->stats.unk68);
    AICarInitTransition(car);
    CalcTurnStart(car, 0);
    UpdateCurrentTriggerPt(car);
    car->motion.pos.vx = tPoints[car->stats.triggerPt].x;
    car->motion.pos.vy = tPoints[car->stats.triggerPt].z;
    car->motion.pos.vz = triggerPtGroups.pos[tPoints[car->stats.triggerPt].type].y;
    car->motion.vel.vy = 0;
    car->flags[1] = 0;
    car->motion.rot.vz = car->unk1BC;
    InitTireGroup(car->tires, tPoints[car->stats.triggerPt].type);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", InitTriggerPoint);
#endif

#ifdef NON_MATCHING
void UpdateCurrentTriggerPt(AICar* car)
{
    s16 dx;
    s16 dz;
    u8 both;

    car->unk170 = car->unk172;
    car->unk172 = car->unk174;
    car->unk1BC = car->unk1C0;
    car->unk1F8 = car->unk1FC;
    car->unk18A = car->unk18C;
    car->unk198 = car->unk1A0;
    car->unk19C = car->unk1A4;
    car->unk1A0 = car->unk1A8;
    car->unk1A4 = car->unk1AC;
    if (tPoints[car->unk170].type == tPoints[car->unk172].type) {
        car->unk16E = 0;
    } else {
        car->unk16E = 1;
    }
    CalcTurnStart(car, 0);
    car->unk200 = car->unk1B0;
    car->unk18E = tPoints[car->unk172].x - tPoints[car->unk170].x;
    car->unk190 = tPoints[car->unk172].z - tPoints[car->unk170].z;
    dx = car->unk18E;
    if (dx < 0) {
        dx = -dx;
    }
    car->unk165 = dx < 40;
    dz = car->unk190;
    if (dz < 0) {
        dz = -dz;
    }
    car->unk166 = dz < 40;
    both = 0;
    if (car->unk165 == 0) {
        both = car->unk166 == 0;
    }
    car->unk164 = both;
    if (car->unk164) {
        car->unk1D4 = tPoints[car->unk170].x + car->unk18E / 2;
        car->unk1D8 = tPoints[car->unk170].z + car->unk190 / 2;
    } else if (car->unk165) {
        car->unk1D4 = tPoints[car->unk170].x;
    } else {
        car->unk1D8 = tPoints[car->unk170].z;
    }
    AICarInitSwerve(car);
    car->unk160 = 0;
    car->unk174 = car->unk170;
    car->unk17E = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", UpdateCurrentTriggerPt);
#endif

#ifdef NON_MATCHING
void CalcTurnStart(AICar* car, u8 which)
{
    s32 v;
    s32 s;
    s16 dx;
    s16 dz;
    u8 both;

    if (which) {
        v = car->unk1C4;
        if (v < 0) {
            v = -v;
        }
        if (v < 113) {
            car->unk1B0 = 160;
            car->stats.unk94 = car->stats.unk88;
        } else {
            v = car->unk1C4;
            if (v < 0) {
                v = -v;
            }
            if (v < 455) {
                car->unk1B0 = car->stats.unk24 / 2;
                car->stats.unk94 = 4 * car->stats.unk88 / 5;
            } else {
                v = car->unk1C4;
                if (v < 0) {
                    v = -v;
                }
                if (v < 773) {
                    car->unk1B0 = 7 * car->stats.unk24 / 10;
                    car->stats.unk94 = 3 * car->stats.unk88 / 4;
                } else {
                    v = car->unk1C4;
                    if (v < 0) {
                        v = -v;
                    }
                    if (v < 1285) {
                        car->unk1B0 = car->stats.unk24;
                        car->stats.unk94 = 2 * car->stats.unk88 / 3;
                    } else {
                        if (car->unk16C != 0) {
                            car->stats.unk94 = 2 * car->stats.unk88 / 3;
                        } else {
                            car->stats.unk94 = car->stats.unk88 / 2;
                        }
                        car->unk1B0 = 10 * car->stats.unk24 / 7;
                    }
                }
            }
        }
        if (car->unk18C < 81) {
            car->stats.unk94 = car->stats.unk94 / 2;
            car->unk1B0 = car->unk1B0 / 2;
        } else if (car->unk18C < 121) {
            car->stats.unk94 = 3 * car->stats.unk94 / 4;
            car->unk1B0 = 3 * car->unk1B0 / 4;
        }
        if (car->stats.unk94 < 20) {
            car->stats.unk94 = 20;
        }
        car->stats.unkB4
            = car->stats.unk94 * 19 * GetFieldsLastFrame() / 100 * 32 * car->stats.unkFC / 100;
        if (car->unk1B0 < 160) {
            car->unk1B0 = 160;
        }
        s = GetAICarStrength();
        if (s != 100) {
            if (s < 101) {
                if (s == 60) {
                    car->unk1B0 = 2 * car->unk1B0 / 3;
                }
            } else if (s == 120) {
                car->unk1B0 = 6 * car->unk1B0 / 5;
            }
        }
    } else {
        car->unk1B0 = 10 * car->stats.unk24 / 7;
        car->unk1B0 += car->unk1B0 / 2;
        car->stats.unk94 = 2 * car->stats.unk88 / 3;
        car->stats.unkB4
            = car->stats.unk94 * 19 * GetFieldsLastFrame() / 100 * 32 * car->stats.unkFC / 100;
    }
    car->unk192 = tPoints[car->unk174].x - tPoints[car->unk172].x;
    car->unk194 = tPoints[car->unk174].z - tPoints[car->unk172].z;
    dx = car->unk192;
    if (dx < 0) {
        dx = -dx;
    }
    car->unk168 = dx < 40;
    dz = car->unk194;
    if (dz < 0) {
        dz = -dz;
    }
    car->unk169 = dz < 40;
    both = 0;
    if (car->unk168 == 0) {
        both = car->unk169 == 0;
    }
    car->unk167 = both;
    if (car->unk167) {
        car->unk1DC = tPoints[car->unk172].x + car->unk192 / 2;
        car->unk1E0 = tPoints[car->unk172].z + car->unk194 / 2;
    } else if (car->unk168) {
        car->unk1DC = tPoints[car->unk172].x;
    } else {
        car->unk1E0 = tPoints[car->unk172].z;
    }
    car->unk1B4 = car->unk1B0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", CalcTurnStart);
#endif

#ifdef NON_MATCHING
void GetNextTriggerPoint(AICar* car, s16 pt)
{
    s32 r;
    s32 dx;
    s32 dz;
    s32 a;
    s16 out[4];

    if (pt != -1) {
        car->unk174 = pt;
        car->unk18C = 96;
    } else {
        if (car->unk01 == 2) {
            r = rand() % tPoints[car->unk172].f1A;
            if (tPoints[car->unk172].f1A >= 2) {
                while (tPoints[car->unk172].link[r] == car->unk170) {
                    r = rand() % tPoints[car->unk172].f1A;
                }
            }
            car->unk174 = tPoints[car->unk172].link[(s16)r];
            car->unk18C = tPoints[car->unk172].dist[(s16)r];
        } else {
            car->unk174 = GetClosestTriggerPointFromCurrPt(car, &tPoints[car->unk172], out);
            car->unk18C = out[0];
        }
    }
    dx = tPoints[car->unk174].x - tPoints[car->unk172].x;
    dz = tPoints[car->unk174].z - tPoints[car->unk172].z;
    car->unk1FC = SquareRoot0(dx * dx + dz * dz);
    if (dz == 0) {
        dz = 1;
    }
    car->unk1C0 = ratan2(dx, dz);
    BoundAngle(&car->unk1C0);
    if (car->unk16B != 0) {
        car->unk1A8 = tPoints[car->unk174].x + car->unk184;
        car->unk1AC = tPoints[car->unk174].z + car->unk184;
    } else if (car->unk16A != 0) {
        car->unk1A8 = tPoints[car->unk174].x - car->unk184;
        car->unk1AC = tPoints[car->unk174].z - car->unk184;
    } else {
        car->unk1A8 = tPoints[car->unk174].x;
        car->unk1AC = tPoints[car->unk174].z;
    }
    car->unk1C4 = car->unk1C0 - car->unk1BC;
    BoundAngle(&car->unk1C4);
    a = car->unk1C4;
    if (a < 0) {
        a = -a;
    }
    if (a >= 1593) {
        car->unk16C = 1;
    } else {
        car->unk16C = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetNextTriggerPoint);
#endif

#ifdef NON_MATCHING
s16 GetClosestTriggerPointFromCurrPt(AICar* car, TriggerPt* tp, s16* out)
{
    s16 la;
    s16 bestPt;
    s16 pt;
    s16 dist;
    s16 i;
    s32 best;
    s32 d;
    s32 lap[3];

    la = GetLookAheadPos(car, lap);
    bestPt = tp->link[0];
    *out = tp->dist[0];
    if (car->unk01 != 1) {
        best = 0x7FFFFFFF;
    } else {
        best = 0;
    }
    for (i = 0; i < tp->f1A; i++) {
        pt = tp->link[i];
        if (car->unk01 == 0 && la >= 0 && tPoints[pt].type != la) {
            continue;
        }
        d = SquareRoot0((lap[0] - tPoints[pt].x) * (lap[0] - tPoints[pt].x)
            + (lap[1] - tPoints[pt].z) * (lap[1] - tPoints[pt].z));
        dist = tp->dist[i];
        if (car->unk01 == 1) {
            if (best >= d) {
                continue;
            }
            if (pt == car->unk170 && car->unk17C >= 3) {
                continue;
            }
        } else {
            if (d >= best) {
                continue;
            }
            if (pt == car->unk170 && (dist < 121 || car->unk17C >= 3)) {
                continue;
            }
        }
        bestPt = pt;
        best = d;
        *out = dist;
    }
    if (bestPt == car->unk170) {
        car->unk17C++;
    } else {
        car->unk17C = 0;
    }
    return bestPt;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetClosestTriggerPointFromCurrPt);
#endif

#ifdef NON_MATCHING
s16 GetLookAheadPos(AICar* car, s32* out)
{
    PlayerCar* player;
    s32 grp;
    s32 type;
    s16 ret;
    s16 tp;
    s16 tgt;
    s16 link;
    s16 t;
    s16 i;
    s32 a;
    s32 b;
    VECTOR vin;
    VECTOR vout;

    player = GetPlayerInfo(car->unk0E);
    grp = player->unkC0;
    ret = tPoints[car->unk172].type;
    type = tPoints[car->unk172].type;
    tp = triggerPtGroups.a[type][grp];
    tgt = triggerPtGroups.b[type][grp];
    if (grp != type && triggerPtGroups.a[type][grp] >= 0) {
        car->unk4C = 1;
    } else {
        car->unk4C = 0;
    }
    if (car->unk4C != 0) {
        if (car->unk01 != 0) {
            tp = *(s16*)&player->flags[0x1E];
            ret = -1;
        } else if (tp == car->unk172) {
            link = tPoints[tp].link[0];
            for (i = car->unk01; i < tPoints[tp].f1A; i++) {
                t = tPoints[tp].link[i];
                if (tPoints[t].type == tgt) {
                    link = t;
                    ret = tgt;
                    if (GetFrameCount() & 1) {
                        break;
                    }
                }
            }
            tp = link;
        }
        out[0] = tPoints[tp].x;
        out[1] = tPoints[tp].z;
        out[2] = tPoints[tp].f08;
    } else {
        vin.vx = 0;
        vin.vy = car->unk34 / 2;
        vin.vz = 0;
        if (car->unk01 == 0) {
            if (player->motion.vel.vy != 0) {
                a = car->stats.unk10C * player->motion.vel.vy;
                if (player->motion.vel.vy >= player->stats.unkA8) {
                    b = player->motion.vel.vy;
                } else {
                    b = player->stats.unkA8;
                }
                vin.vy = a / b;
            } else {
                vin.vy = 240;
            }
        }
        mathMulTransVec(&player->motion.mat2, &vin, &vout);
        out[0] = player->motion.pos.vx + vout.vx;
        out[1] = player->motion.pos.vy + vout.vy;
        ret = -1;
        out[2] = player->motion.pos.vz + vout.vz;
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/car_update", GetLookAheadPos);
#endif
