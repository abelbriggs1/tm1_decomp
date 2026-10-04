#ifndef __TM1_SCREEN_H__
#define __TM1_SCREEN_H__

#include "common.h"
#include <libgpu.h>

#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/rt.h"

typedef struct ScreenCarEnding {
    /* 0x000 */ CarStats stats;
    /* 0x11C */ s32 unk11C;
    /* 0x120 */ u_long* image;
    /* 0x124 */ s32 imageRight;
    /* 0x128 */ s32 savedIsBg;
    /* 0x12C */ Cs* cs;
    /* 0x130 */ Db* db;
    /* 0x134 */ Db* cdb;
} ScreenCarEnding; /* 0x138 */

typedef struct ScrollText {
    /* 0x000 */ char* lines[256];
    /* 0x400 */ s32 y;
    /* 0x404 */ s32 y0;
    /* 0x408 */ s32 bottom;
    /* 0x40C */ s32 line;
    /* 0x410 */ s32 numLines;
    /* 0x414 */ s32 charWidth;
    /* 0x418 */ s32 spacing;
    /* 0x41C */ s32 unk41C;
    /* 0x420 */ s32 unk420;
    /* 0x424 */ DRAWENV draw;
    /* 0x480 */ u_long* image;
    /* 0x484 */ char* text;
    /* 0x488 */ s32 speed;
    /* 0x48C */ s32 tick;
    /* 0x490 */ s32 delay;
    /* 0x494 */ s32 mode;
    /* 0x498 */ s32 font;
    /* 0x49C */ s32 stopY;
} ScrollText; /* 0x4A0 */

s32 atoi(char* str);
void screenSetEnv(void);
s32 screenDisplayTitle(void);
s32 screenBattleOptions(s32 twoPlayer);
void screenDisplayImage(s32 rightHalf, u_long* data, s32 x, s32 y);
void screenDisplayToDisplay(s32 toRight, s32 unused);
s32 screenAudioOptions(s32 noLoad, s32 hasTracks);
s32 screenOptions(s32 twoPlayer);
s32 screenMainOptions(void);
void screenChooseBattleground(s32 split);
s32 screenCharacterRowPosition(s32 row, s32 numRows, s32 center, s32 spacing);
void screenDisplayChoicesFromBottom(
    s32 mode, s32 count, s32 unused, s32 sel, char** names, char** names2, s32 bottom, s32 spacing);
void screenInitCarOccupants(u8* model);
void screenLoadCarSelectionBackground(void);
void screenShowCarBio(s32 carName, s32 pad);
s32 screenHoldForTwoPlayerControl(s32* pad, s32* player, u_long* image);
s32 screenVehicleChoice(s32 players, u8 demo);
s32 screenGetCarsEnum(s32 which);
void screenSetVehicleHeading(s32 heading);
void screenResetFrameNum(void);
void screenSpinVehicle(Db* db, s32 carName, s32 left, s32 exclude);
void screenGrayedScreen(void);
s32 screenTransitionLevel(s32 level, s32 advanced);
void screenLostGame(void);
void screenBossCar(void);
void screenInit(void);
char* screenChooseControls(s32 mode, s32 pad, s32 unused);
char* screenGetControlPadLabel(s32 which);
char* screenGetMusicLabel(void);
s32 screenNextMusicChoice(s32 delta);
void screenSetMusicVolume(void);
s32 screenGetMusicVolume(void);
char* screenGetEffectsLabel(void);
s32 screenNextEffectsChoice(s32 delta);
void screenSetEffectsVolume(void);
char* screenGetDifficultyLabel(void);
s32 screenNextDifficulty(s32 delta);
void screenLostALife(s32 lives);
s32 screenMovieIsDoneTest(s32 frames, s32 limit, u8 flip);
s32 screenMovieIsDoneTestWithTwoSecondWait(s32 frames, s32 limit, u8 flip);
s32 screenPlayCinema(s32 which);
void screenPlayAllMovies(void);
void screenDisplayLogos(void);
void screenCheckMidGameOptions(void);
void screenClearScreen(s32 x, s32 y, s32 w, s32 h);
void screenPrintScore(s16 who, s16 wins, s16 losses);
void screenInitCardSprites(s32 which, s16 count, s32* opponents);
void screenResetCards(s32 which);
void screenDrawCard(s32 card, s32 instant, s32 skipMoving);
void screenLoadCarPictures(s16 numCars, s32* carNames);
s16 screenGetNumCards(void);
s16 screenGetLevelNumCards(s32 level);
void screenLoadCardsForBattleOptions(s32 twoPlayer, s32 car0, s32 car1);
void screenDrawCardsForBattleOptions(s32 twoPlayer);
void screenDumpScreenToRam(s32 rightHalf, u_long* dest);
void screenLoadScreenFromRam(s32 rightHalf, u_long* src);
void screenVramToScreen(s32 rightHalf, s32 slot);
void screenVramToRam(s32 x, s32 y, s32 w, s32 h, u_long* dest);
void screenRamToVram(s32 x, s32 y, s32 w, s32 h, u_long* src);
s32 screenEnterAccessCode(void);
s32 screenWrongPassword(s32 state);
char* screenPasswordForLevel(s32 level);
s32 screenAccessCodeFromPassword(char* password);
char* screenPasswordFromAccessCode(s32 code);
void screenBlinkCursor(s32 font);
void screenEraseCursor(void);
void screenResetCursor(void);
s32 screenWaitForContinue(s32 frames, s32 limit);
void SetDefaultEndingParameters(void);
void SetOutlawEndingParameters(void);
void SetRoadKillEndingParameters(void);
void screenCarEnding(s32 car);
void screenCarEndingInit(ScreenCarEnding* e, s32 car, u_long* image);
void screenCarEndingReset(ScreenCarEnding* e);
void screenCarEndingDrawCar(ScreenCarEnding* e);
void screenCarEndingDisplayCar(ScreenCarEnding* e);
void screenCarEndingCleanup(ScreenCarEnding* e);
void screenPlayAllCarEndings(void);
void screenShowWarhawkDemo(void);
void screenScrollTextInit(
    ScrollText* st, char* textFile, char* textBuf, char* bgFile, u_long* imageBuf, s32 font);
void screenScrollTextPrintOneLine(ScrollText* st);
s32 screenScrollTextUpdate(ScrollText* st);
s32 screenScrollTextProcessButtonPress(ScrollText* st);
void screenScrollTextOneScreenFull(ScrollText* st);
s32 screenScrollText(char* textFile, char* bgFile, s32 font);
s32 screenPlayCarEnding(s32 car);
void screenDisplaySonyLegalScreen(void);
void screenDisplayDeveloperScreen(void);
void screenShowAccessCodes(void);
s32 screenDisplayHistory(void);
s32 screenDisplayCredits(void);
void screenShowAccessCodeForLevel(s32 level, s32 x, s32 y);
void screenWonSpecialLevel(void);
s32 screenGetWhichPad(void);
void screenDisplayWarningScreen(void);

#endif /* __TM1_SCREEN_H__ */
