#include "common.h"

#include <libgte.h>
#include <rand.h>

#include "tm1/ai_car.h"
#include "tm1/car.h"
#include "tm1/car_init.h"
#include "tm1/car_update.h"
#include "tm1/shell.h"
#include "tm1/trigger_pts.h"
#include "tm1/ua.h"

#include "tm1/ai_car_init.h"

extern MATRIX D_80170D94;

#ifdef NON_MATCHING
void AICarInit(AICar* car, s32 uaIndex, u8 which)
{
    u8 found;
    s16 i;
    s16 start;
    TriggerPtStartPts* pts;

    found = 0;
    car->uaIndex = uaIndex;
    car->stats.unk00 = 0;
    AICarInitDynamics(car);
    if (which) {
        InitWeapons(car, &car->weap, 0);
        car->stats.unkF8 = 0;
    } else {
        car->weap.ammo[11] = 20;
    }
    car->unk01 = 2;
    InitAIFlags(&car->unk40);
    InitAITransDat((AITransDat*)&car->unk160);
    car->stats.triggerPt = 0;
    pts = GetTriggerPtStartPts();
    start = rand() % pts->n;
    for (i = start; i < pts->n && !found; i++) {
        if (pts->grp[i] == 0) {
            car->stats.triggerPt = pts->start[i];
            pts->grp[i] = 1;
            found = 1;
        }
    }
    for (i = 0; i < start && !found; i++) {
        if (pts->grp[i] == 0) {
            car->stats.triggerPt = pts->start[i];
            pts->grp[i] = 1;
            found = 1;
        }
    }
    if (!found) {
        for (i = pts->n; i < 15; i++) {
            if (pts->grp[i] == 0) {
                car->stats.triggerPt = pts->start[i];
                pts->grp[i] = 1;
                break;
            }
        }
    }
    InitTriggerPoint(car);
    InitControlPad(car->skid);
    SetBounce(car, 2, 2, 0, 0);
    AICarOutOfBattle(car);
    car->driving = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_init", AICarInit);
#endif

#ifdef NON_MATCHING
void AICarInitDynamics(AICar* car)
{
    car->motion.pos.vx = 0;
    car->motion.pos.vy = 0;
    car->motion.pos.vz = car->stats.unk6C;
    car->motion.vel.vx = 0;
    car->motion.vel.vy = 0;
    car->motion.vel.vz = 0;
    car->motion.rot.vx = 0;
    car->motion.rot.vy = 0;
    car->motion.rot.vz = 0;
    car->motion.rotDelta.vx = 0;
    car->motion.rotDelta.vy = 0;
    car->motion.rotDelta.vz = 0;
    car->motion.rot2.vx = 0;
    car->motion.rot2.vy = 0;
    car->motion.rot2.vz = 0;
    car->motion.rot2Delta.vx = 0;
    car->motion.rot2Delta.vy = 0;
    car->motion.rot2Delta.vz = 0;
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
    car->unk160 = 0;
    InitMotionFlags(car->flags);
    CarInitMotion(car, 0);
    car->stats.unk38 = 1;
    InitNoCollision(&car->collision);
    car->routeVal = 0;
    car->unk0A = 0;
    car->unk0C = 0;
    CarInitStrength(&car->stats, GetAICarStrength());
    CarInitDeltas(&car->stats, car->uaIndex);
    InitHitPoints(car, 0);
    InitTireInfo(car->tires);
    if (shellGetCurrentLevel() == 5) {
        InitTireGroup(car->tires, 8);
    } else {
        InitTireGroup(car->tires, 0);
    }
    car->unk18 = 0x140;
    if (car->uaIndex == -1) {
        car->unk18 = 0x320;
    }
    car->unk1C = 0xA0;
    car->unk20 = 0x258;
    car->unk24 = 0x3E8;
    car->unk28 = 0x7D0;
    AICarInitProfiles(car);
    car->stats.unk40 = car->stats.unk44;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_init", AICarInitDynamics);
#endif

#ifdef NON_MATCHING
void AICarInitProfiles(AICar* car)
{
    car->unk104 = car->stats.unk44 / 5;
    car->unk105 = (car->stats.unk44 * 49) / 100;
    car->unk106 = (car->stats.unk44 * 75) / 100;
    car->unk107 = car->stats.unk44;
    car->attack[0].unk02 = 0;
    car->attack[1].unk02 = 0;
    car->attack[2].unk02 = 90;
    car->attack[3].unk02 = 90;
    car->attack[0].unk04 = 50;
    car->attack[1].unk04 = 50;
    car->attack[2].unk04 = 50;
    car->attack[3].unk04 = 50;
    car->attack[0].unk06 = 50;
    car->attack[1].unk06 = 50;
    car->attack[2].unk06 = 50;
    car->attack[3].unk06 = 50;
    switch (car->uaIndex) {
    case 10:
        car->route[0].prio = 0;
        car->route[0].unk04 = 0;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 50;
        car->route[1].unk04 = 100;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 300;
        car->route[2].prio = 100;
        car->route[2].unk04 = 100;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 100;
        car->route[3].unk04 = 100;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 90;
        car->attack[1].unk00 = 90;
        car->attack[2].unk00 = 90;
        car->attack[3].unk00 = 90;
        break;
    case 20:
        car->route[0].prio = 0;
        car->route[0].unk04 = 100;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 10;
        car->route[1].unk04 = 80;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 300;
        car->route[2].prio = 60;
        car->route[2].unk04 = 85;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 80;
        car->route[3].unk04 = 95;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 30;
        car->attack[3].unk00 = 30;
        break;
    case 30:
        car->route[0].prio = 70;
        car->route[0].unk04 = 97;
        car->route[0].unk08 = 100;
        car->route[0].val = 180;
        car->route[0].unk10 = 120;
        car->route[0].unk14 = 60;
        car->route[1].prio = 80;
        car->route[1].unk04 = 90;
        car->route[1].unk08 = 100;
        car->route[1].val = 180;
        car->route[1].unk10 = 120;
        car->route[1].unk14 = 60;
        car->route[2].prio = 90;
        car->route[2].unk04 = 90;
        car->route[2].unk08 = 100;
        car->route[2].val = 180;
        car->route[2].unk10 = 120;
        car->route[2].unk14 = 60;
        car->route[3].prio = 95;
        car->route[3].unk04 = 95;
        car->route[3].unk08 = 100;
        car->route[3].val = 180;
        car->route[3].unk10 = 120;
        car->route[3].unk14 = 60;
        car->attack[0].unk00 = 90;
        car->attack[1].unk00 = 90;
        car->attack[2].unk00 = 90;
        car->attack[3].unk00 = 90;
        break;
    case 40:
        car->route[0].prio = 90;
        car->route[0].unk04 = 100;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 50;
        car->route[1].unk04 = 100;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 300;
        car->route[2].prio = 50;
        car->route[2].unk04 = 100;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 90;
        car->route[3].unk04 = 95;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 90;
        car->attack[1].unk00 = 90;
        car->attack[2].unk00 = 100;
        car->attack[3].unk00 = 100;
        break;
    case 60:
        car->route[0].prio = 10;
        car->route[0].unk04 = 98;
        car->route[0].unk08 = 100;
        car->route[0].val = 120;
        car->route[0].unk10 = 120;
        car->route[0].unk14 = 60;
        car->route[1].prio = 10;
        car->route[1].unk04 = 98;
        car->route[1].unk08 = 100;
        car->route[1].val = 120;
        car->route[1].unk10 = 120;
        car->route[1].unk14 = 60;
        car->route[2].prio = 10;
        car->route[2].unk04 = 98;
        car->route[2].unk08 = 100;
        car->route[2].val = 120;
        car->route[2].unk10 = 120;
        car->route[2].unk14 = 60;
        car->route[3].prio = 30;
        car->route[3].unk04 = 98;
        car->route[3].unk08 = 100;
        car->route[3].val = 120;
        car->route[3].unk10 = 120;
        car->route[3].unk14 = 60;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 0;
        car->attack[3].unk00 = 0;
        break;
    case 50:
        car->route[0].prio = 0;
        car->route[0].unk04 = 85;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 420;
        car->route[0].unk14 = 300;
        car->route[1].prio = 0;
        car->route[1].unk04 = 85;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 420;
        car->route[1].unk14 = 300;
        car->route[2].prio = 85;
        car->route[2].unk04 = 85;
        car->route[2].unk08 = 100;
        car->route[2].val = 600;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 85;
        car->route[3].unk04 = 85;
        car->route[3].unk08 = 100;
        car->route[3].val = 600;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 50;
        car->attack[3].unk00 = 50;
        break;
    case 70:
        car->route[0].prio = 80;
        car->route[0].unk04 = 98;
        car->route[0].unk08 = 100;
        car->route[0].val = 120;
        car->route[0].unk10 = 600;
        car->route[0].unk14 = 150;
        car->route[1].prio = 80;
        car->route[1].unk04 = 98;
        car->route[1].unk08 = 100;
        car->route[1].val = 120;
        car->route[1].unk10 = 600;
        car->route[1].unk14 = 150;
        car->route[2].prio = 80;
        car->route[2].unk04 = 98;
        car->route[2].unk08 = 100;
        car->route[2].val = 120;
        car->route[2].unk10 = 600;
        car->route[2].unk14 = 150;
        car->route[3].prio = 80;
        car->route[3].unk04 = 98;
        car->route[3].unk08 = 100;
        car->route[3].val = 120;
        car->route[3].unk10 = 600;
        car->route[3].unk14 = 150;
        car->attack[0].unk00 = 65;
        car->attack[1].unk00 = 65;
        car->attack[2].unk00 = 65;
        car->attack[3].unk00 = 65;
        break;
    case 80:
        car->route[0].prio = 0;
        car->route[0].unk04 = 0;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 600;
        car->route[1].prio = 0;
        car->route[1].unk04 = 0;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 600;
        car->route[2].prio = 0;
        car->route[2].unk04 = 0;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 600;
        car->route[3].prio = 0;
        car->route[3].unk04 = 0;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 600;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 0;
        car->attack[3].unk00 = 0;
        break;
    case 90:
        car->route[0].prio = 15;
        car->route[0].unk04 = 90;
        car->route[0].unk08 = 100;
        car->route[0].val = 600;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 90;
        car->route[1].unk04 = 95;
        car->route[1].unk08 = 100;
        car->route[1].val = 480;
        car->route[1].unk10 = 180;
        car->route[1].unk14 = 180;
        car->route[2].prio = 15;
        car->route[2].unk04 = 90;
        car->route[2].unk08 = 100;
        car->route[2].val = 600;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 15;
        car->route[3].unk04 = 90;
        car->route[3].unk08 = 100;
        car->route[3].val = 600;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 10;
        car->attack[3].unk00 = 10;
        break;
    case 100:
        car->route[0].prio = 0;
        car->route[0].unk04 = 0;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 100;
        car->route[0].unk14 = 100;
        car->route[1].prio = 0;
        car->route[1].unk04 = 0;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 120;
        car->route[2].prio = 0;
        car->route[2].unk04 = 0;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 120;
        car->route[3].prio = 65;
        car->route[3].unk04 = 65;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 120;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 5;
        car->attack[3].unk00 = 5;
        break;
    case 110:
        car->route[0].prio = 0;
        car->route[0].unk04 = 100;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 95;
        car->route[1].unk04 = 100;
        car->route[1].unk08 = 100;
        car->route[1].val = 600;
        car->route[1].unk10 = 600;
        car->route[1].unk14 = 300;
        car->route[2].prio = 95;
        car->route[2].unk04 = 100;
        car->route[2].unk08 = 100;
        car->route[2].val = 600;
        car->route[2].unk10 = 600;
        car->route[2].unk14 = 300;
        car->route[3].prio = 95;
        car->route[3].unk04 = 100;
        car->route[3].unk08 = 100;
        car->route[3].val = 600;
        car->route[3].unk10 = 600;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 5;
        car->attack[3].unk00 = 5;
        break;
    case 120:
        car->route[0].prio = 0;
        car->route[0].unk04 = 100;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 10;
        car->route[1].unk04 = 80;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 300;
        car->route[2].prio = 60;
        car->route[2].unk04 = 85;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 80;
        car->route[3].unk04 = 95;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 30;
        car->attack[3].unk00 = 50;
        break;
    case 130:
        car->route[0].prio = 30;
        car->route[0].unk04 = 90;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 300;
        car->route[0].unk14 = 300;
        car->route[1].prio = 60;
        car->route[1].unk04 = 80;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 300;
        car->route[1].unk14 = 300;
        car->route[2].prio = 90;
        car->route[2].unk04 = 95;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 300;
        car->route[2].unk14 = 300;
        car->route[3].prio = 90;
        car->route[3].unk04 = 90;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 300;
        car->route[3].unk14 = 300;
        car->attack[0].unk00 = 100;
        car->attack[1].unk00 = 100;
        car->attack[2].unk00 = 100;
        car->attack[3].unk00 = 100;
        break;
    default:
        car->route[0].prio = 100;
        car->route[0].unk04 = 100;
        car->route[0].unk08 = 100;
        car->route[0].val = 300;
        car->route[0].unk10 = 0;
        car->route[0].unk14 = 0;
        car->route[1].prio = 100;
        car->route[1].unk04 = 100;
        car->route[1].unk08 = 100;
        car->route[1].val = 300;
        car->route[1].unk10 = 0;
        car->route[1].unk14 = 0;
        car->route[2].prio = 100;
        car->route[2].unk04 = 100;
        car->route[2].unk08 = 100;
        car->route[2].val = 300;
        car->route[2].unk10 = 0;
        car->route[2].unk14 = 0;
        car->route[3].prio = 100;
        car->route[3].unk04 = 100;
        car->route[3].unk08 = 100;
        car->route[3].val = 300;
        car->route[3].unk10 = 0;
        car->route[3].unk14 = 0;
        car->attack[0].unk00 = 0;
        car->attack[1].unk00 = 0;
        car->attack[2].unk00 = 0;
        car->attack[3].unk00 = 0;
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_init", AICarInitProfiles);
#endif

#ifdef NON_MATCHING
void InitAIFlags(u8* flags)
{
    flags[0] = 0;
    flags[1] = 0;
    flags[2] = 0;
    flags[3] = 0;
    flags[4] = 0;
    flags[5] = 0;
    flags[6] = 0;
    flags[7] = 0;
    flags[8] = 0;
    flags[9] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_init", InitAIFlags);
#endif

#ifdef NON_MATCHING
void InitAITransDat(AITransDat* transDat)
{
    transDat->unk10[0] = 0;
    transDat->unk10[1] = 0;
    transDat->unk10[2] = 0;
    transDat->unk10[3] = 0;
    transDat->unk10[4] = 0;
    transDat->unk10[5] = 0;
    transDat->unk10[6] = 0;
    transDat->unk10[10] = 0;
    transDat->unk10[7] = 0;
    transDat->unk00[0] = 0;
    transDat->unk00[1] = 0;
    transDat->unk00[2] = 0;
    transDat->unk00[3] = 0;
    transDat->unk00[4] = 0;
    transDat->unk00[5] = 0;
    transDat->unk00[6] = 0;
    transDat->unk00[10] = 0;
    transDat->unk00[11] = 0;
    transDat->unk00[12] = 0;
    transDat->unk00[13] = 0;
    transDat->unk00[14] = 0;
    transDat->unk00[15] = 0;
    transDat->unk10[9] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ai_car_init", InitAITransDat);
#endif
