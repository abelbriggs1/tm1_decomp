#ifndef __TM1_UA_SOUND_H__
#define __TM1_UA_SOUND_H__

#include "common.h"
#include "tm1/SOUND_REQ.H"

// Special weapon sound IDs.
#define SOUND_SPECIAL_DUNE_BUGGY 0
#define SOUND_SPECIAL_ICE_CREAM_TRUCK 1
#define SOUND_SPECIAL_MAD_MAX 2
#define SOUND_SPECIAL_CORVETTE 3
#define SOUND_SPECIAL_HUMVEE 4
#define SOUND_SPECIAL_MONSTER_TRUCK 5
#define SOUND_SPECIAL_LAMBORGHINI 6
#define SOUND_SPECIAL_TAXI_CAB 7
#define SOUND_SPECIAL_POLICE_CAR 8
#define SOUND_SPECIAL_SEMI_TRUCK 9
#define SOUND_SPECIAL_LOW_RIDER 10
#define SOUND_SPECIAL_HARLEY 11
#define SOUND_SPECIAL_BOSS_CAR 12

void uasoundFireCarMachineGuns(s32 w, s32 a, s32 b);
void uasoundStopCarMachineGuns(s32 w);
void uasoundExplodeMachineGunBullet(s32 a, s32 b, s32 c);
void uasoundPlayCarEngineRev(s32 w, s32 a, s32 b, s32 v);
void uasoundPlayCarCrash(s32 w, s32 a, s32 b, s32 v);
void uasoundPlayCarExplode(s32 w, s32 a, s32 b, s32 v);
void uasoundPlayCarSkid(s32 w, s32 a, s32 b, s32 v);
void uasoundStopCarSounds(s32 w);
void uasoundPlayCarWeaponPickup(s32 w, s32 a, s32 b);
void uasoundPlayCarWeaponLaunchOrInflight(s32 w, s32 a, s32 b);
void uasoundStopCarWeapon(s32 w);
void uasoundPlayCarWeaponExplode(s32 w, s32 a, s32 b, s32 v);
void uasoundPlayCarSpecialWeaponPickup(s32 w, s32 a, s32 b);
void uasoundPlayCarSpecialWeaponLaunchOrInflight(s32 w, s32 a, s32 b);
void uasoundStopCarSpecialWeapon(s32 w);
void uasoundPlayCarSpecialWeaponExplode(s32 w, s32 a, s32 b, s32 v);
void uasoundPlayCarSignature(s32 w, s32 a, s32 b);
void uasoundStopCarSignature(s32 w);
void uasoundPlayTurboBoost(s32 a, s32 b);
void uasoundStopTurboBoost(void);
void uasoundPlayHoverCops(s32 a, s32 b);
void uasoundStopHoverCops(void);
void uasoundPlayRecharge(void);
void uasoundPlayBeep(void);
void uasoundPlayMenuCycleFlush(void);
void uasoundPlayMenuCycle(void);
void uasoundPlayMenuSelectFlush(void);
void uasoundPlayMenuSelect(void);
void uasoundPlayCardThrowFlush(void);
void uasoundPlayMaleScream(void);
void uasoundPlayGeneralExplode(s32 a, s32 b, s32 v);
void uasoundPlayScreenEngineRev(s32 a);
void uasoundStopScreenEngineRev(void);
void uasoundPlayHuntDA(s32 mode);
void uasoundPlayBattleDA(s32 mode);
void uasoundSetMusicVolume(s32 vol);

#endif // __TM1_UA_SOUND_H__
