#ifndef __TM1_VEHICLE_H__
#define __TM1_VEHICLE_H__

#include "common.h"
#include "tm1/long_vector.h"
#include <libgte.h>

typedef struct Vehicle {
    /*0x00*/ u8 ctl[12];
    /*0x0C*/ s16 active;
    /*0x0E*/ u16 padId;
    /*0x10*/ s32 restart;
    /*0x14*/ s32 unk14;
    /*0x18*/ s32 moveStep;
    /*0x1C*/ s32 moveDelta;
    /*0x20*/ s32 moveMax;
    /*0x24*/ s32 turnStep;
    /*0x28*/ s32 turnMax;
    /*0x2C*/ s32 moveMaxScale;
    /*0x30*/ s32 turnMaxScale;
    /*0x34*/ VEC3 pos;
    /*0x40*/ VEC3 vel;
    /*0x4C*/ VEC3 ang;
    /*0x58*/ VEC3 angVel;
    /*0x64*/ MATRIX mat;
} Vehicle; /* 0x84 */

extern MATRIX gVehicleIdentity;

void VehicleInit(Vehicle* veh, s16 padId);
void InitVehicleDynamics(Vehicle* veh);
void InitVehiclePad(Vehicle* veh);
void InitVehicleTweeking(void);
void TermVehicle(Vehicle* veh);
void RecomputeVehicleDeltas(Vehicle* veh);
void VehicleUpdate(Vehicle* veh, s16 padIdx);
void VehicleRotUpdate(Vehicle* veh);
void VehicleTransUpdate(Vehicle* veh);

#endif // __TM1_VEHICLE_H__
