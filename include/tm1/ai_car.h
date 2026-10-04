#ifndef __TM1_AI_CAR_H__
#define __TM1_AI_CAR_H__

#include "common.h"
#include "tm1/car.h"

void AICarUpdate(AICar* car);
void AICarUpdateHealthTier(AICar* car);
void AICarUpdateClosestPlayer(AICar* car);
void AICarUpdatePlayerRange(AICar* car);
void AICarUpdateCurrPtRange(AICar* car);
void AICarUpdateNextPtAngle(AICar* car);
void AICarUpdateDrivingProfile(AICar* car);
void AICarInitTransition(AICar* car);
void AICarUpdateTransition(AICar* car);
void NoBeadOnPlayer(AICar* car);
void GetNewPointToDriveTo(AICar* car);
void HelpLostCar(AICar* car);
void AIInitTurnFlags(AICar* car);
void AICarTurn(AICar* car);
u8 TimeToComeOutOfTurn(AICar* car, s32 diff);
void AICarDriveBetweenPts(AICar* car);
void UpdateTurnStart(AICar* car);
void AICarInitSwerve(AICar* car);
void AICarUpdateSwerve(AICar* car);
u8 GetBeadOnPlayer(AICar* car, s32 range);
s32 GetBufferZoneSpeed(AICar* car);
void AICarUpdateControlPad(AICar* car);
void AICarOutOfBattle(AICar* car);
void AICarInBattle(AICar* car);
void UpdateAICarsInBattleLaneDist(void);
void AICarUpdatePlayerDir(AICar* car);
void BringBackAICarInCollision(AICar* car);
void AICarHeliUpdate(AICar* car);
void UpdateNumAICarsInBattle(void);
s16 GetNumAICarsInBattle(void);
void StartLostCheck(AICar* car);
Car* GetClosestCarToPlayer(s16 player, u8* isPlayer);
void InitControlPad(u8* pad);
void TermAICar(void);
void InitAICarsInBattle(void);

#endif // __TM1_AI_CAR_H__
