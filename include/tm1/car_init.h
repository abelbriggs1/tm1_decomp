#ifndef __TM1_CAR_INIT_H__
#define __TM1_CAR_INIT_H__

#include "common.h"

#include "tm1/car.h"

void CarInit(Car* car, s32 uaIndex, u8 which);
void CarInitDynamics(Car* car);
void InitMotionFlags(u8* flags);
void InitWeapons(Car* car, CarWeap* weap, u8 which);
void InitPlayerPositions(Car* car, u8 isP1);
void InitHitPoints(Car* car, u8 isPlayer);
void CarInitDeltas(CarStats* stats, s32 uaIndex);
void InitIceCreamTruck(CarStats* stats);
void InitSemiTruck(CarStats* stats);
void InitTaxiCab(CarStats* stats);
void InitMonsterTruck(CarStats* stats);
void InitLamborghini(CarStats* stats);
void InitPoliceCar(CarStats* stats);
void InitDuneBuggy(CarStats* stats);
void InitHumvee(CarStats* stats);
void InitLowRider(CarStats* stats);
void InitMadMax(CarStats* stats);
void InitHarley(CarStats* stats);
void InitCorvette(CarStats* stats);
void InitBossCar(CarStats* stats);
void InitHeliCar(CarStats* stats);
void CarInitTweeking(void);
void GetCarName(void);
void CarInitVRModes(Car* car);
void RecomputeCarDeltas(CarStats* stats);
void CarInitStrength(CarStats* stats, s32 strength);

#endif /* __TM1_CAR_INIT_H__ */
