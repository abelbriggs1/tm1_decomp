#ifndef __TM1_UA_H__
#define __TM1_UA_H__

#include "common.h"
#include "tm1/CAR.H"
#include "tm1/CS.H"
#include "tm1/MATH.H"
#include "tm1/RT.H"
#include <LIBGTE.H>

// `ua` entity type indices.
// The helicopter camera car also uses `UA_NONE`, since it has no model.
#define UA_NONE (-1)
#define UA_3D_ENVIRONMENT 1
#define UA_SKY 2
#define UA_UNK_3 3

// Cars. Each car's tire, shadow, and headlight nodes use the car's index
// plus 2, 3, and 4 respectively.
#define UA_ICE_CREAM_TRUCK 10
#define UA_ICE_CREAM_TRUCK_TIRE 12
#define UA_ICE_CREAM_TRUCK_SHADOW 13
#define UA_ICE_CREAM_TRUCK_HEADLIGHTS 14
#define UA_TAXI_CAB 20
#define UA_TAXI_CAB_TIRE 22
#define UA_TAXI_CAB_SHADOW 23
#define UA_TAXI_CAB_HEADLIGHTS 24
#define UA_SEMI_TRUCK 30
#define UA_SEMI_TRUCK_TIRE 32
#define UA_SEMI_TRUCK_SHADOW 33
#define UA_SEMI_TRUCK_HEADLIGHTS 34
#define UA_MONSTER_TRUCK 40
#define UA_MONSTER_TRUCK_WHEEL 42
#define UA_MONSTER_TRUCK_SHADOW 43
#define UA_MONSTER_TRUCK_HEADLIGHTS 44
#define UA_POLICE_CAR 50
#define UA_POLICE_CAR_TIRE 52
#define UA_POLICE_CAR_SHADOW 53
#define UA_POLICE_CAR_HEADLIGHTS 54
#define UA_LAMBORGHINI 60
#define UA_LAMBORGHINI_TIRE 62
#define UA_LAMBORGHINI_SHADOW 63
#define UA_LAMBORGHINI_HEADLIGHTS 64
#define UA_HUMVEE 70
#define UA_HUMVEE_TIRE 72
#define UA_HUMVEE_SHADOW 73
#define UA_HUMVEE_HEADLIGHTS 74
#define UA_HARLEY 80
#define UA_HARLEY_TIRE 82
#define UA_HARLEY_SHADOW 83
#define UA_HARLEY_HEADLIGHTS 84
#define UA_DUNE_BUGGY 90
#define UA_DUNE_BUGGY_TIRE 92
#define UA_DUNE_BUGGY_SHADOW 93
#define UA_DUNE_BUGGY_HEADLIGHTS 94
#define UA_LOW_RIDER 100
#define UA_LOW_RIDER_TIRE 102
#define UA_LOW_RIDER_SHADOW 103
#define UA_LOW_RIDER_HEADLIGHTS 104
#define UA_CORVETTE 110
#define UA_CORVETTE_TIRE 112
#define UA_CORVETTE_SHADOW 113
#define UA_CORVETTE_HEADLIGHTS 114
#define UA_MAD_MAX 120
#define UA_MAD_MAX_TIRE 122
#define UA_MAD_MAX_SHADOW 123
#define UA_MAD_MAX_HEADLIGHTS 124
#define UA_BOSS_CAR 130
#define UA_BOSS_CAR_TIRE 132
#define UA_BOSS_CAR_SHADOW 133
#define UA_BOSS_CAR_HEADLIGHTS 134

// Weapon models and sprites.
#define UA_UNK_201 201
#define UA_UNK_202 202
#define UA_UNK_203 203
#define UA_UNK_204 204
#define UA_UNK_205 205
#define UA_UNK_206 206
#define UA_CATAPULT 210
#define UA_BOMB 212
#define UA_OIL 213
#define UA_SPIKE 214
#define UA_UNK_301 301
#define UA_UNK_302 302
#define UA_SWARM_MISSILE 307
#define UA_UNK_308 308
#define UA_UNK_309 309
#define UA_GHOST_MISSILE 311
#define UA_DEATHSPEAR 312

// Weapon pickups.
#define UA_UNK_400 400
#define UA_PICKUP_FIRE_MISSILE 401
#define UA_PICKUP_FREEZE_MISSILE 402
#define UA_PICKUP_HOMING_MISSILE 403
#define UA_PICKUP_POWER_MISSILE 404
#define UA_PICKUP_REAR_MISSILE 405
#define UA_PICKUP_SINGING_MISSILE 406
#define UA_PICKUP_CATAPULT 410
#define UA_PICKUP_FLAMETHROWER 411
#define UA_PICKUP_BOMB 412
#define UA_PICKUP_OIL 413
#define UA_PICKUP_SPIKE 414
#define UA_UNK_420 420
#define UA_UNK_421 421
#define UA_UNK_422 422
#define UA_UNK_423 423
#define UA_UNK_430 430
#define UA_UNK_431 431
#define UA_PICKUP_SPECIAL 450

// Destroyable targets.
#define UA_UNK_600 600
#define UA_UNK_601 601
#define UA_UNK_603 603
#define UA_UNK_610 610
#define UA_UNK_611 611
#define UA_UNK_612 612
#define UA_UNK_613 613

// Explosion effects.
#define UA_FLAME_ANIMATION 650
#define UA_SMOKE_ANIMATION 651
#define UA_CONTRAIL_ANIMATION 652
#define UA_SPARK_ANIMATION 653
#define UA_EXPLOSION_ANIMATION 654
#define UA_GBURST_ANIMATION 655
#define UA_BURN_ANIMATION 656
#define UA_FRAGMENT_A 670
#define UA_FRAGMENT_B 671
#define UA_FRAGMENT_C 672
#define UA_FRAGMENT_D 673

#define UA_BARRICADE 700
#define UA_UNK_702 702
#define UA_UNK_703 703
#define UA_UNK_705 705
#define UA_UNK_707 707
#define UA_UNK_708 708
#define UA_UNK_713 713
#define UA_UNK_715 715
#define UA_UNK_716 716
#define UA_UNK_720 720
#define UA_UNK_721 721
#define UA_UNK_730 730

// Hover cops.
#define UA_BAZ_COP 750
#define UA_HOVER_COP 752

// Static cops.
#define UA_UNK_753 753
#define UA_UNK_754 754
#define UA_UNK_755 755

// Pedestrians.
#define UA_UNK_751 751
#define UA_UNK_756 756
#define UA_UNK_760 760
#define UA_UNK_772 772
#define UA_UNK_773 773
#define UA_UNK_775 775
#define UA_UNK_776 776
#define UA_UNK_777 777
#define UA_UNK_778 778
#define UA_UNK_781 781
#define UA_UNK_782 782

#define UA_UNK_770 770
#define UA_TV 810
#define UA_UNK_902 902
#define UA_HEALTH_STAND 920
#define UA_HEALTH_STAND_LIGHTNING 921
#define UA_UNK_930 930
#define UA_UNK_931 931

#define UA_UNK_1000 1000
#define UA_UNK_1001 1001
#define UA_UNK_1002 1002
#define UA_UNK_1003 1003
#define UA_UNK_1004 1004
#define UA_UNK_1005 1005
#define UA_UNK_1008 1008
#define UA_UNK_1009 1009
#define UA_UNK_1010 1010
#define UA_UNK_1015 1015
#define UA_DROP_BOX 1016
#define UA_UNK_1017 1017
#define UA_FALLING_BOX 1018
#define UA_BREAKING_WINDOW 1019

// Cockpit and HUD.
#define UA_DASHBOARD 1240
#define UA_REAR_VIEW_MIRROR 1247
#define UA_DASHBOARD_ICONS 1260
#define UA_STEERING_WHEEL 1280
#define UA_ARROW_UP 1290
#define UA_ARROW_RIGHT 1291
#define UA_ARROW_DOWN 1292
#define UA_ARROW_LEFT 1293
#define UA_CAR_OCCUPANTS 1300

#define UA_IS_PICKUP(type) ((type) >= UA_PICKUP_FIRE_MISSILE && (type) <= UA_PICKUP_SPECIAL)
#define UA_IS_PERSON(type) ((type) >= UA_BAZ_COP && (type) <= UA_UNK_782)

// Maps a car's UA index to its car-relative slot/ID.
// (A car and its tire, shadow, and headlight nodes all share the same ID.)
#define UA_CAR_SLOT(type) ((type) / 10)

typedef struct DrawNode {
    /*0x00*/ u8 pad00[0x14];
    /*0x14*/ u8 drawFlag;
} DrawNode;

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
void StartDeathSequence(Car* car, u8 isPlayer);
u8 AnyAICarsAlive(void);
void UpdateCamera(s16 idx);
void UAStats(void);
void UAPlayerUpdate(Cs* cs, PlayerCar* car);
void UAAIUpdate(Cs* cs, AICar* car);
void UARemoveVehicleFromDrawList(s32 name);
void CheckVRMode(PlayerCar* car);
void InitHelicoptorPosition(void);
s16 GetClosestPlayer(VECTOR3* pos, s16 skip);
s16 GetClosestAICar(VECTOR3* pos);
void GetPlayerPosition(s16 player, VECTOR3* out);
void GetAICarPosition(s16 ai, VECTOR3* out);
void GetPlayerRot(s16 player, SVECTOR* out);
Cs* GetPlayerCs3D(s16 player);
Cs* GetAICs3D(s16 ai);
s16 GetAITheCameraFollows(void);
s32 GetPlayerSpeed(s16 player);
s32 GetAISpeed(s16 ai);
s16 GetNumPlayers(void);
PlayerCar* GetPlayerInfo(s16 player);
s16 GetNumAICars(void);
AICar* GetAICarInfo(s16 ai);
Cs* UAGetCs(s32 name);
u8 uaIsCarCS(Cs* cs, u8* isPlayer, s16* index);
s16 uaGetCarMatID(s16 idx, u8 isPlayer);
s32 uaIsCarMatID(s16 matId, s8* isPlayer, u16* index);
void UASetCameraPosition(s16 which, VECTOR3* pos, SVECTOR* rot);
void UAGetCameraPosition(s16 idx, VECTOR3* pos, SVECTOR* rot);
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
void uaPadInit(PlayerCar* car, s32 mode);
s16 GetPlayerCarToCarIndex(s16 player);
s16 GetAICarToCarIndex(s16 ai);
s32 GetAICarStrength(void);
void uaDontDrawCar(s16 idx, u8 isPlayer);
void uaDrawCar(s16 idx, u8 isPlayer);
s32 uaGetCurrentCar(s32 isPlayer, s16 idx);
s16 GetPlayerTheCameraFollows(void);
void UASetSoundFlags(s16 idx, Car* car, u8 isPlayer);
s16 GetNumAICarsLiving(void);
s16 GetCarNumFromCarName(s32 name);
s32 GetCarNameFromCarNum(s16 num);
void uaSelectOpponents(s32 car, s16 level, s16* outCount, s32* outCars);
char* uaGetCarNameString(s32 name);
u8 uaHasPlayerBeatThisLevel(void);
u8 uaPlayerCheating(void);
void uaInitBossCar(void);

#endif // __TM1_UA_H__
