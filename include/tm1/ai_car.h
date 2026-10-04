#ifndef __TM1_AI_CAR_H__
#define __TM1_AI_CAR_H__

#include "common.h"
#include "tm1/car.h"

void AICarUpdate(CarAlt* car);
void AICarUpdateHealthTier(CarAlt* car);
void AICarUpdateClosestPlayer(CarAlt* car);
void AICarUpdatePlayerRange(CarAlt* car);
void AICarUpdateCurrPtRange(CarAlt* car);
void AICarUpdateNextPtAngle(CarAlt* car);
void AICarUpdateDrivingProfile(CarAlt* car);
void AICarInitTransition(CarAlt* car);
void AICarUpdateTransition(CarAlt* car);
void NoBeadOnPlayer(CarAlt* car);
void GetNewPointToDriveTo(CarAlt* car);
void HelpLostCar(CarAlt* car);
void AIInitTurnFlags(CarAlt* car);
void AICarTurn(CarAlt* car);
u8 TimeToComeOutOfTurn(CarAlt* car, s32 diff);
void AICarDriveBetweenPts(CarAlt* car);
void UpdateTurnStart(CarAlt* car);
void AICarInitSwerve(CarAlt* car);
void AICarUpdateSwerve(CarAlt* car);
u8 GetBeadOnPlayer(CarAlt* car, s32 range);
s32 GetBufferZoneSpeed(CarAlt* car);
void AICarUpdateControlPad(CarAlt* car);
void AICarOutOfBattle(CarAlt* car);
void AICarInBattle(CarAlt* car);
void UpdateAICarsInBattleLaneDist(void);
void AICarUpdatePlayerDir(CarAlt* car);
void BringBackAICarInCollision(CarAlt* car);
void AICarHeliUpdate(CarAlt* car);
void UpdateNumAICarsInBattle(void);
s16 GetNumAICarsInBattle(void);
void StartLostCheck(CarAlt* car);
void* GetClosestCarToPlayer(s16 player, u8* isPlayer);
void InitControlPad(u8* pad);
void TermAICar(void);
void InitAICarsInBattle(void);

#endif // __TM1_AI_CAR_H__
