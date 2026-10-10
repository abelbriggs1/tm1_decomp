#include "common.h"

#include <libgte.h>

#include "tm1/ctlpad.h"
#include "tm1/math.h"
#include "tm1/smooth.h"

#include "tm1/vehicle.h"

extern void* sdk_memcpy();

// clang-format off
static MATRIX gVehicleIdentity = IDENTITY_MATRIX;

#ifdef NON_MATCHING
void VehicleInit(Vehicle* veh, s16 padId)
{
    veh->padId = padId;
    if ((s16)veh->padId < 0) {
        veh->active = 0;
    } else {
        veh->active = 1;
    }
    InitVehiclePad(veh);
    veh->moveStep = 40;
    veh->moveMaxScale = 500;
    veh->turnStep = 33;
    veh->turnMaxScale = 50;
    RecomputeVehicleDeltas(veh);
    veh->restart = 0;
    veh->unk14 = 0;
    InitVehicleDynamics(veh);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", VehicleInit);
#endif

void InitVehicleDynamics(Vehicle* veh)
{
    veh->pos.vx = 0;
    veh->pos.vy = 0;
    veh->pos.vz = 0;
    veh->vel.vx = 0;
    veh->vel.vy = 0;
    veh->vel.vz = 0;
    veh->ang.vx = 0;
    veh->ang.vy = 0;
    veh->ang.vz = 0;
    veh->angVel.vx = 0;
    veh->angVel.vy = 0;
    veh->angVel.vz = 0;
    sdk_memcpy(&veh->mat, &gVehicleIdentity, sizeof(MATRIX));
}

#ifdef NON_MATCHING
void InitVehiclePad(Vehicle* veh)
{
    veh->ctl[0] = 5;
    veh->ctl[1] = 6;
    veh->ctl[3] = 8;
    veh->ctl[2] = 7;
    veh->ctl[4] = 3;
    veh->ctl[5] = 3;
    veh->ctl[7] = 14;
    veh->ctl[6] = 13;
    veh->ctl[8] = 11;
    veh->ctl[9] = 12;
    veh->ctl[11] = 9;
    veh->ctl[10] = 9;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", InitVehiclePad);
#endif

void InitVehicleTweeking(void)
{
}

void TermVehicle(Vehicle* veh)
{
}

#ifdef NON_MATCHING
void RecomputeVehicleDeltas(Vehicle* veh)
{
    veh->moveDelta = veh->moveStep * 6;
    veh->moveMax = veh->moveStep * veh->moveMaxScale;
    veh->turnMax = veh->turnStep * veh->turnMaxScale;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", RecomputeVehicleDeltas);
#endif

#ifdef NON_MATCHING
void VehicleUpdate(Vehicle* veh, s16 padIdx)
{
    if (GetCtlPad(veh->ctl[7], padIdx) && !GetCtlPad(veh->ctl[11], padIdx)) {
        veh->angVel.vz += veh->turnStep;
        if (veh->angVel.vz > veh->turnMax) {
            veh->angVel.vz = veh->turnMax;
        }
    } else if (GetCtlPad(veh->ctl[6], padIdx) && !GetCtlPad(veh->ctl[10], padIdx)) {
        veh->angVel.vz -= veh->turnStep;
        if (veh->angVel.vz < -veh->turnMax) {
            veh->angVel.vz = -veh->turnMax;
        }
    } else {
        veh->angVel.vz = 0;
    }

    if (GetCtlPad(veh->ctl[8], padIdx)) {
        veh->angVel.vx = veh->turnMax;
    } else if (GetCtlPad(veh->ctl[9], padIdx)) {
        veh->angVel.vx = -veh->turnMax;
    } else {
        veh->angVel.vx = 0;
    }

    if (GetCtlPad(veh->ctl[11], padIdx) && GetCtlPad(veh->ctl[7], padIdx)) {
        veh->angVel.vy = -veh->turnMax;
    } else if (GetCtlPad(veh->ctl[10], padIdx) && GetCtlPad(veh->ctl[6], padIdx)) {
        veh->angVel.vy = veh->turnMax;
    } else {
        veh->angVel.vy = 0;
    }

    if (GetCtlPad(veh->ctl[0], padIdx) && !GetCtlPad(veh->ctl[4], padIdx)) {
        veh->vel.vy += veh->moveStep;
        if (veh->vel.vy > veh->moveMax) {
            veh->vel.vy = veh->moveMax;
        }
    } else if (GetCtlPad(veh->ctl[1], padIdx) && !GetCtlPad(veh->ctl[5], padIdx)) {
        veh->vel.vy -= veh->moveStep;
        if (veh->vel.vy < -veh->moveMax) {
            veh->vel.vy = -veh->moveMax;
        }
    } else {
        veh->vel.vy = 0;
    }

    if (GetCtlPad(veh->ctl[3], padIdx)) {
        veh->vel.vx = veh->moveDelta;
    } else if (GetCtlPad(veh->ctl[2], padIdx)) {
        veh->vel.vx = -veh->moveDelta;
    } else {
        veh->vel.vx = 0;
    }

    if (GetCtlPad(veh->ctl[4], padIdx) && GetCtlPad(veh->ctl[0], padIdx)) {
        veh->vel.vz = veh->moveDelta;
    } else if (GetCtlPad(veh->ctl[5], padIdx) && GetCtlPad(veh->ctl[1], padIdx)) {
        veh->vel.vz = -veh->moveDelta;
    } else {
        veh->vel.vz = 0;
    }

    VehicleRotUpdate(veh);
    VehicleTransUpdate(veh);
    if (veh->restart) {
        TermVehicle(veh);
        VehicleInit(veh, veh->padId);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", VehicleUpdate);
#endif

#ifdef NON_MATCHING
void VehicleRotUpdate(Vehicle* veh)
{
    SVECTOR ang;

    veh->ang.vx += veh->angVel.vx / 64;
    veh->ang.vy += veh->angVel.vy / 64;
    veh->ang.vz += veh->angVel.vz / 64;
    BoundVector(&veh->ang.vx);
    ang.vx = veh->ang.vx;
    ang.vy = veh->ang.vy;
    ang.vz = veh->ang.vz;
    RotMatrixYXZ(&ang, &veh->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", VehicleRotUpdate);
#endif

void VehicleTransUpdate(Vehicle* veh)
{
    VECTOR3 step;
    VECTOR3 world;

    step.vx = veh->vel.vx / 16;
    step.vy = veh->vel.vy / 16;
    step.vz = veh->vel.vz / 16;
    mathMulTransVec(&veh->mat, (VECTOR*)&step, (VECTOR*)&world);
    veh->pos.vx += world.vx;
    veh->pos.vy += world.vy;
    veh->pos.vz += world.vz;
}
