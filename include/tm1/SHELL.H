#ifndef __TM1_SHELL_H__
#define __TM1_SHELL_H__

#include "common.h"

void timIntoVRAM(u_long* tim);
s32 shellTextureInit(u_long* p);
s32 shellGetGameState(void);
void shellSetGameState(s32 state);
void shellSetAppropriateDBnames(s32 vehicle);
char* shellGetCurrentDatabaseFileName(void);
char* shellGetCurrentTextureFileName(void);
char* shellGetCurrentLevelName(void);
char* shellGetLevelName(s32 level);
char* shellGetNextLevelName(void);
s32 shellGetCurrentLevel(void);
s32 shellNextLevelChoice(s32 delta);
char* shellGetCurrentTwoPlayerDatabaseFileName(void);
char* shellGetCurrentTwoPlayerTextureFileName(void);
char* shellGetTwoPlayerCurrentLevelName(void);
s32 shellGetTwoPlayerCurrentLevel(void);
s32 shellNextTwoPlayerLevelChoice(s32 delta);
void shellInitMachine(void);
void shellInitGraphicsSystem(void);
void shellInitGame(void);
void shellInitNewDatabase(s32 level, s32 tmsVersion);
void shellInitLevel(s32 level);
void shellLeaveLevel(s32 level);
void shellLeaveGame(void);
s32 shellGetAccessCode(void);
void shellSetAccessCode(s32 code);
s32 shellSoundForThisLevel(void);
s32 shellGetSoundtrackId(void);
char* shellGetSoundtrackTitle(void);
void shellSetSoundtrackChoice(s32 id);
s32 shellNextSoundtrackChoice(s32 delta);
void shellTurnCarHeadlightsOff(void);
void shellSelectCards(void);
void shellLoadCards(void);
void shellDrawCards(void);
s32 shellProcessAccessCode(s32 code);
s32 shellLivesRemaining(void);
void shellSetLivesRemaining(s32 lives);
void shellBeginLevelTransition(void);
s32 shellCheckTransitionStatus(void);
s32 shellGetTransitionState(void);
void shellSetTransitionState(s32 state);
void shellLoadCarsDatabase(void);
void shellLoadCarsEndDatabase(void);
s32 shellGetCurrentVehicle(void);
void shellPlayCarEngineRev(void);
void shellStopCarEngineRev(void);
void shellSetGodMode(s16 player, s32 on);
s32 shellIsGodMode(void);
void shellSetInfiniteWeapons(s32 player, s32 on);
s32 shellIsInfiniteWeapons(void);
s32 shellIsPerfect(void);

#endif /* __TM1_SHELL_H__ */
