#include "common.h"

#include <libgte.h>

#include "tm1/ctlpad.h"
#include "tm1/math.h"
#include "tm1/smooth.h"

#include "tm1/vehicle.h"

extern void* sdk_memcpy();

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
    veh->pos.x = 0;
    veh->pos.y = 0;
    veh->pos.z = 0;
    veh->vel.x = 0;
    veh->vel.y = 0;
    veh->vel.z = 0;
    veh->ang.x = 0;
    veh->ang.y = 0;
    veh->ang.z = 0;
    veh->angVel.x = 0;
    veh->angVel.y = 0;
    veh->angVel.z = 0;
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
        veh->angVel.z += veh->turnStep;
        if (veh->angVel.z > veh->turnMax) {
            veh->angVel.z = veh->turnMax;
        }
    } else if (GetCtlPad(veh->ctl[6], padIdx) && !GetCtlPad(veh->ctl[10], padIdx)) {
        veh->angVel.z -= veh->turnStep;
        if (veh->angVel.z < -veh->turnMax) {
            veh->angVel.z = -veh->turnMax;
        }
    } else {
        veh->angVel.z = 0;
    }

    if (GetCtlPad(veh->ctl[8], padIdx)) {
        veh->angVel.x = veh->turnMax;
    } else if (GetCtlPad(veh->ctl[9], padIdx)) {
        veh->angVel.x = -veh->turnMax;
    } else {
        veh->angVel.x = 0;
    }

    if (GetCtlPad(veh->ctl[11], padIdx) && GetCtlPad(veh->ctl[7], padIdx)) {
        veh->angVel.y = -veh->turnMax;
    } else if (GetCtlPad(veh->ctl[10], padIdx) && GetCtlPad(veh->ctl[6], padIdx)) {
        veh->angVel.y = veh->turnMax;
    } else {
        veh->angVel.y = 0;
    }

    if (GetCtlPad(veh->ctl[0], padIdx) && !GetCtlPad(veh->ctl[4], padIdx)) {
        veh->vel.y += veh->moveStep;
        if (veh->vel.y > veh->moveMax) {
            veh->vel.y = veh->moveMax;
        }
    } else if (GetCtlPad(veh->ctl[1], padIdx) && !GetCtlPad(veh->ctl[5], padIdx)) {
        veh->vel.y -= veh->moveStep;
        if (veh->vel.y < -veh->moveMax) {
            veh->vel.y = -veh->moveMax;
        }
    } else {
        veh->vel.y = 0;
    }

    if (GetCtlPad(veh->ctl[3], padIdx)) {
        veh->vel.x = veh->moveDelta;
    } else if (GetCtlPad(veh->ctl[2], padIdx)) {
        veh->vel.x = -veh->moveDelta;
    } else {
        veh->vel.x = 0;
    }

    if (GetCtlPad(veh->ctl[4], padIdx) && GetCtlPad(veh->ctl[0], padIdx)) {
        veh->vel.z = veh->moveDelta;
    } else if (GetCtlPad(veh->ctl[5], padIdx) && GetCtlPad(veh->ctl[1], padIdx)) {
        veh->vel.z = -veh->moveDelta;
    } else {
        veh->vel.z = 0;
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

    veh->ang.x += veh->angVel.x / 64;
    veh->ang.y += veh->angVel.y / 64;
    veh->ang.z += veh->angVel.z / 64;
    BoundVector(&veh->ang.x);
    ang.vx = veh->ang.x;
    ang.vy = veh->ang.y;
    ang.vz = veh->ang.z;
    RotMatrixYXZ(&ang, &veh->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/vehicle", VehicleRotUpdate);
#endif

void VehicleTransUpdate(Vehicle* veh)
{
    VEC3 step;
    VEC3 world;

    step.x = veh->vel.x / 16;
    step.y = veh->vel.y / 16;
    step.z = veh->vel.z / 16;
    mathMulTransVec(&veh->mat, (VECTOR*)&step, (VECTOR*)&world);
    veh->pos.x += world.x;
    veh->pos.y += world.y;
    veh->pos.z += world.z;
}
