#ifndef __TM1_UA_H__
#define __TM1_UA_H__

#include "common.h"
#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/long_vector.h"
#include "tm1/rt.h"
#include <libgte.h>

typedef struct DrawNode {
    /*0x00*/ u8 pad00[0x14];
    /*0x14*/ u8 drawFlag;
} DrawNode;

extern VEC3 zeroTrans;
extern s32 carTypes[13];
extern s16 carIndexToPlayerOrAIIndex[14];
extern s32 carName[13];
extern Cs* carCs[13];
extern Car playerInfo[2];
extern Cs* aiCarCs[8];
extern CarAlt aiCarInfo[8];
extern s16 aiIndexToCarIndex[8];
extern CarAlt helicoptor;
extern s32 gSelectedAICars[8];

extern u8 gInitPlayerWeapons[4];
extern u8 skipLevel;
extern SVECTOR zeroRot;
extern Cs* playerCs[2];
extern u16 playerIndexToCarIndex[2];
extern s32 padConfig[2];
extern s32 playerCar[2];
extern s32 aiBegTics;
extern s32 aiEndTics;
extern Cs* camera[2];
extern Cs* helicoptorCS;
extern s32 playerBegTics;
extern s32 playerEndTics;
extern s32 gAICars;

void UASetBattleMusicOn(void);
void UAAddCs(Cs* cs, s32 name);
void uaInit(void);
void uaInitWeapons(s32 on);
void uaInitCars(void);
void uaInitDB(u32 level);
void uaInitView(Db* db, s32 entry, s32 which);
void InitLevel1DBSpecifics(void);
void InitLevel2DBSpecifics(void);
void InitLevel3DBSpecifics(void);
void InitLevel4DBSpecifics(void);
void InitLevel5DBSpecifics(void);
void InitLevel6DBSpecifics(void);
void InitLevel(u32 level);
void InitUALevel1(void);
void InitUALevel2(void);
void InitUALevel3(void);
void InitUALevel4(void);
void InitUALevel5(void);
void InitUALevel6(void);
void InitUALevel7(void);
void uaLeaveLevel(s32 level);
void UASetNumPlayers(s32 n);
void uaPickAICars(s16 level);
void UASetPlayerCar(s16 player, s32 car, s32 mode);
s16 UAGetPlayerCar(s16 player);
void UASetGodMode(s16 player, u8 on);
u8 uaDrivingAICars(void);
void uaDriveAICars(u8 on);
void UASetInfiniteWeapons(s32 player, s32 on);
void UASetHelicoptorMode(s32 unused, u8 mode);
void InitPlayerCar(s16 slot, s32 car, u8 flag);
void SetCarCsInfo(s16 slot, s16 idx, s32 car, u8 found);
void UASetAICar(s16 slot, s32 car);
void SetAICarCsInfo(s16 slot, s16 idx, s32 car, u8 found);
void PlayGame(void);
u8 CheckPlayerHealthStand(s32 player);
void UpdatePlayerDamageModel(s16 player);
void UpdateAICarDamageModel(s16 ai);
void StartDeathSequence(void* car, u8 which);
u8 AnyAICarsAlive(void);
void UpdateCamera(s16 idx);
void UAStats(void);
void UAPlayerUpdate(Cs* cs, Car* car);
void UAAIUpdate(Cs* cs, CarAlt* car);
void UARemoveVehicleFromDrawList(s32 name);
void CheckVRMode(Car* car);
void InitHelicoptorPosition(void);
s16 GetClosestPlayer(VEC3* pos, s16 skip);
s16 GetClosestAICar(VEC3* pos);
void GetPlayerPosition(s16 player, VEC3* out);
void GetAICarPosition(s16 ai, VEC3* out);
void GetPlayerRot(s16 player, SVECTOR* out);
Cs* GetPlayerCs3D(s16 player);
Cs* GetAICs3D(s16 ai);
s16 GetAITheCameraFollows(void);
s32 GetPlayerSpeed(s16 player);
s32 GetAISpeed(s16 ai);
s16 GetNumPlayers(void);
Car* GetPlayerInfo(s16 player);
s16 GetNumAICars(void);
CarAlt* GetAICarInfo(s16 ai);
Cs* UAGetCs(s32 name);
u8 uaIsCarCS(Cs* cs, u8* isPlayer, s16* index);
s16 uaGetCarMatID(s16 idx, u8 isPlayer);
s32 uaIsCarMatID(s16 matId, s8* isPlayer, u16* index);
void UASetCameraPosition(s16 which, VEC3* pos, SVECTOR* rot);
void UAGetCameraPosition(s16 idx, VEC3* pos, SVECTOR* rot);
Cs* UAGetCameraCS(s16 idx);
u8 uaUsingLanes(void);
u8 uaUsingGroupGroundHeight(void);
void uaInitTweeking(void);
void uaTweekDifficultyFnc(void);
void uaSetDifficulty(s32 d);
s32 uaGetDifficulty(void);
s32 uaGetTwoPlayerMode(void);
void PadSetConfig(s32 cfg, s16 player);
s32 PadGetConfig(s16 player);
void uaPadInit(Car* car, s32 mode);
s16 GetPlayerCarToCarIndex(s16 player);
s16 GetAICarToCarIndex(s16 ai);
s32 GetAICarStrength(void);
void uaDontDrawCar(s16 idx, u8 isPlayer);
void uaDrawCar(s16 idx, u8 isPlayer);
s32 uaGetCurrentCar(s32 isPlayer, s16 idx);
s16 GetPlayerTheCameraFollows(void);
void UASetSoundFlags(s16 idx, void* car, u8 isPlayer);
s16 GetNumAICarsLiving(void);
s16 GetCarNumFromCarName(s32 name);
s32 GetCarNameFromCarNum(s16 num);
void uaSelectOpponents(s32 car, s16 level, s16* outCount, s32* outCars);
char* uaGetCarNameString(s32 name);
u8 uaHasPlayerBeatThisLevel(void);
u8 uaPlayerCheating(void);
void uaInitBossCar(void);

#endif // __TM1_UA_H__
