#include "common.h"
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <stdio.h>
#include <strings.h>

#include "tm1/cs.h"
#include "tm1/ctlpad.h"
#include "tm1/db.h"
#include "tm1/explosion.h"
#include "tm1/fileio.h"
#include "tm1/font.h"
#include "tm1/hier.h"
#include "tm1/interactives.h"
#include "tm1/light.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/sound.h"
#include "tm1/targets.h"
#include "tm1/timer.h"
#include "tm1/ua.h"
#include "tm1/ua_dash.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sound.h"
#include "tm1/ua_sw.h"
#include "tm1/view.h"
#include "tm1/weapon.h"

#define LOAD_BUFFER ((void*)0x800188B8)
#define CORE_VAB_BUFFER ((void*)0x80010000)

extern void exit(s32 code);

extern void screenDisplayLogos(void);
extern void screenSetMusicVolume(void);
extern void screenSetEffectsVolume(void);
extern void screenPlayCinema(s32 which);
extern s32 screenGetMusicVolume(void);
extern s32 screenDisplayTitle(void);
extern s32 screenMainOptions(void);
extern s32 screenOptions(s32 split);
extern s32 screenVehicleChoice(s32 players, u8 demo);
extern s32 screenDisplayHistory(void);
extern void screenAudioOptions(s32 a, s32 b);
extern s32 screenGetWhichPad(void);
extern void screenChooseControls(s32 a, s32 b, s32 c);
extern void screenChooseBattleground(s32 split);
extern s32 screenBattleOptions(s32 split);
extern s32 screenEnterAccessCode(void);
extern s32 screenWrongPassword(s32 state);
extern void screenLostALife(s32 lives);
extern void screenPrintScore(s32 who, s16 a, s16 b);
extern s32 screenTransitionLevel(s32 level, s32 advanced);
extern s32 screenPlayCarEnding(s32 vehicle);
extern void screenLostGame(void);
extern void screenDisplayCredits(void);
extern void screenDisplayWarningScreen(void);
extern void screenDisplaySonyLegalScreen(void);
extern void screenDisplayDeveloperScreen(void);
extern void screenWaitForContinue(s32 a, s32 b);
extern void screenInit(void);
extern void screenClearScreen(s32 x, s32 y, s32 w, s32 h);
extern void screenSetEnv(void);
extern void screenLoadCarSelectionBackground(void);
extern void screenLoadCarPictures(s16 count, s32* opponents);
extern void screenInitCardSprites(s32 level, s16 count, s32* opponents);
extern void screenDrawCard(s32 index, s32 flip, s32 quick);
extern void screenResetCards(s32 index);
extern void screenBossCar(void);

extern s32 gInfiniteWeapons;
extern s32 gGodMode;
extern s32 opponents[13];

s16 gAccessCodeDigit = 0;
s32 gCurrentLevel = 0;
s32 gStartLevel = 0;
s32 gCarsDbState = 0;
s32 gAccessCode = 0;

char gRoofDb[8] = "ROOFx";
char gSuburbDb[8] = "SUBURBx";
char gParkDb[8] = "PARKx";
char gFwyDb[8] = "XFWYx";
char gWhDb[4] = "WHx";
char gArenaDb[8] = "XARENA1";
char g2pRoofDb[8] = "2PROOF";
char g2pParkDb[8] = "2PPARK";
char g2pFwyDb[8] = "2PFWY";
char g2pWhDb[8] = "2PWH";
char gFinalName[8] = "FINAL";
char gFreewayName[8] = "FREEWAY";
char gArenaName[8] = "ARENA";

s32 gShellFlag = 0;
char gDbFmt[8] = "%s%s.%s";
char gDbDir[8] = "UADMD\\";
char gDbExt[4] = "DMD";
char gTexExt[4] = "TMS";
char gArena5Name[8] = "ARENA 5";
char gCanalName[8] = "CANAL";
s32 gSoundtrackChoice = 0;
s16 gNumOpponents = 0;
char gCarsName[8] = "CARS";
char gCarsEndName[8] = "CARSEND";

static s32 gVehicleID;
static s32 gLivesRemaining;
static s32 gTimeStamp;
static s32 gGameState;
static s32 gPreviousGameState;
static s32 gFd;
static char* dbFilename;
static s32 gTransitionState;

char* dbNames[7] = {
    gArenaDb,
    gWhDb,
    gFwyDb,
    gParkDb,
    gSuburbDb,
    gRoofDb,
    gArenaDb,
};
char* twoPlayerDbNames[6] = {
    gArenaDb,
    g2pWhDb,
    g2pFwyDb,
    g2pParkDb,
    "2PCANALS",
    g2pRoofDb,
};
char* levelNames[6] = {
    gArenaName,
    "WAREHOUSE",
    gFreewayName,
    "CITY PARK",
    "CYBURBIA",
    gFinalName,
};
extern char* soundtrackTitles[7];

#ifdef NON_MATCHING
void main(void)
{
    s32 quit;
    s32 dbLoaded;
    s16 score0;
    s16 score1;
    s32 advanced;
    s32 st;
    s32 ok;
    s32 vid;
    s32 lvl;
    u32 r;
    char* twoPlayerDb;

    dbLoaded = 0;
    quit = 0;
    score0 = 0;
    score1 = 0;
    gLivesRemaining = 3;
    advanced = 0;
    shellInitMachine();
    shellInitGame();
    screenDisplayWarningScreen();
    screenWaitForContinue(120, 120);
    screenDisplaySonyLegalScreen();
    screenDisplayDeveloperScreen();
    shellSetGameState(0);
    do {
        switch (gGameState) {
        case 0:
            screenDisplayLogos();
            shellSetGameState(30);
            screenSetMusicVolume();
            screenSetEffectsVolume();
            break;
        case 30:
            ResetUpdateRate();
            rtSetSplitScreen(0);
            shellLoadCarsDatabase();
            screenPlayCinema(2);
            shellSetGameState(1);
            soundSetPreviousMusicVolume(screenGetMusicVolume());
            soundStartPlayDA(2);
            break;
        case 1:
            gAccessCodeDigit = 0;
            gCurrentLevel = 0;
            gStartLevel = 0;
            advanced = 0;
            ResetUpdateRate();
            rtSetSplitScreen(0);
            uaInitWeapons(1);
            shellStopCarEngineRev();
            gVehicleID = -1;
            if (soundGetCurrentDATrack() != 2) {
                soundInterruptPlayDA();
                soundStartPlayDA(2);
            }
            dbLoaded = 0;
            shellLoadCarsDatabase();
            soundSetPreviousMusicVolume(screenGetMusicVolume());
            soundResumePlayDA();
            shellSetGameState(screenDisplayTitle());
            gLivesRemaining = 3;
            break;
        case 2:
            switch (gShellFlag) {
            case 0:
                shellSetGameState(3);
                gShellFlag = gShellFlag + 1;
                break;
            case 1:
                shellSetGameState(21);
                gShellFlag = gShellFlag + 1;
                break;
            case 2:
                shellSetGameState(27);
                gShellFlag = 0;
                break;
            }
            break;
        case 29:
            shellSetGameState(screenMainOptions());
            break;
        case 6:
            shellPlayCarEngineRev();
            soundProcessIds();
            shellSetGameState(screenOptions(rtIsSplitScreenOn()));
            break;
        case 27:
            if (soundGetCurrentDATrack() != 2) {
                soundInterruptPlayDA();
                soundStartPlayDA(2);
            }
            soundResumePlayDA();
            screenVehicleChoice(1, 1);
            shellSetGameState(30);
            break;
        case 21:
            if (soundGetCurrentDATrack() != 2) {
                soundInterruptPlayDA();
                soundStartPlayDA(2);
            }
            soundResumePlayDA();
            if (screenDisplayHistory() == 0) {
                shellLoadCarsDatabase();
                screenVehicleChoice(1, 1);
            }
            if (gPreviousGameState == 2) {
                shellSetGameState(30);
            } else {
                shellSetGameState(gPreviousGameState);
            }
            break;
        case 25:
            screenAudioOptions(1, 1);
            shellSetGameState(29);
            break;
        case 24:
            shellPlayCarEngineRev();
            if (rtIsSplitScreenOn() != 0) {
                screenChooseControls(2, screenGetWhichPad(), 1);
            } else {
                screenChooseControls(1, 0, 1);
            }
            shellSetGameState(6);
            break;
        case 23:
            shellPlayCarEngineRev();
            screenChooseBattleground(rtIsSplitScreenOn());
            if (rtIsSplitScreenOn() == 0) {
                if (gCurrentLevel != 0) {
                    shellSetGameState(22);
                } else {
                    shellSetGameState(6);
                }
            } else {
                shellSetGameState(6);
            }
            break;
        case 20:
            shellPlayCarEngineRev();
            shellSetGameState(screenBattleOptions(rtIsSplitScreenOn()));
            break;
        case 22:
            shellPlayCarEngineRev();
            ok = shellProcessAccessCode(screenEnterAccessCode());
            st = 6;
            if (ok == 0) {
                st = screenWrongPassword(st);
            }
            shellSetGameState(st);
            break;
        case 19:
            gLivesRemaining = 1;
            uaInitWeapons(1);
            shellSetGameState(10);
            break;
        case 7:
            shellSetGodMode(0, 0);
            shellSetGodMode(1, 0);
            UASetHelicoptorMode(0, 0);
            UASetHelicoptorMode(1, 0);
            shellSetInfiniteWeapons(0, 0);
            shellSetInfiniteWeapons(1, 0);
            dbLoaded = 0;
            soundInterruptPlayDA();
            gVehicleID = screenVehicleChoice(1, 0);
            shellLeaveLevel(0);
            if (gVehicleID != -1) {
                shellSetGameState(20);
            } else {
                shellSetGameState(1);
            }
            break;
        case 8:
            shellSetGodMode(0, 0);
            shellSetGodMode(1, 0);
            UASetHelicoptorMode(0, 0);
            UASetHelicoptorMode(1, 0);
            shellSetInfiniteWeapons(0, 0);
            shellSetInfiniteWeapons(1, 0);
            score0 = 0;
            soundInterruptPlayDA();
            gLivesRemaining = 1;
            gVehicleID = screenVehicleChoice(2, 0);
            shellLeaveLevel(0);
            do {
                vid = gVehicleID;
            } while (0);
            score1 = 0;
            gCurrentLevel = 0;
            dbLoaded = 0;
            if (vid != -1) {
                rtSetSplitScreen(1);
                shellSetGameState(20);
            } else {
                shellSetGameState(1);
            }
            break;
        case 3:
        case 28:
            soundInterruptPlayDA();
            screenPlayCinema(1);
            if (gPreviousGameState == 2) {
                shellSetGameState(30);
            } else {
                shellSetGameState(gPreviousGameState);
            }
            soundResumePlayDA();
            break;
        case 10:
            soundInterruptPlayDA();
            if (dbLoaded == 0) {
                if (rtIsSplitScreenOn() == 0) {
                    shellSetAppropriateDBnames(gVehicleID);
                    shellBeginLevelTransition();
                    screenTransitionLevel(gCurrentLevel, advanced);
                    advanced = 0;
                } else {
                    twoPlayerDb = shellGetCurrentTwoPlayerDatabaseFileName();
                    fileioLoadFileIntoRam(LOAD_BUFFER, shellGetCurrentTwoPlayerTextureFileName());
                    gTimeStamp = shellTextureInit(LOAD_BUFFER);
                    fileioLoadFileIntoRam(LOAD_BUFFER, twoPlayerDb);
                }
                gCarsDbState = 0;
                dbLoaded = 1;
                shellInitNewDatabase(gCurrentLevel, gTimeStamp);
            }
            shellInitLevel(gCurrentLevel);
            shellSetGameState(11);
            break;
        case 12:
            screenBossCar();
            uaInitBossCar();
            shellSetGameState(11);
            break;
        case 11:
            soundResetSoundEngine();
            soundStartPlayDA(shellSoundForThisLevel());
            r = rtMainLoop();
            soundResetSoundEngine();
            rtClearPendingReturns();
            soundForceAllSoundsOffNow();
            shellPlayCarEngineRev();
            soundStartPlayDA(2);
            soundInterruptPlayDA();
            switch (r) {
            case 4:
                dbLoaded = 0;
                score1 = score1 + 1;
                screenPrintScore(1, score0, score1);
                shellSetGameState(19);
                uaInitWeapons(1);
                break;
            case 2:
                shellSetGameState(30);
                uaInitWeapons(1);
                break;
            case 1:
                gLivesRemaining = gLivesRemaining - 1;
                if (gLivesRemaining == 0) {
                    shellSetGameState(14);
                    uaInitWeapons(1);
                } else {
                    screenLostALife(gLivesRemaining);
                    shellSetGameState(10);
                }
                break;
            case 3:
            default:
                if (rtIsSplitScreenOn() != 0) {
                    dbLoaded = 0;
                    score0 = score0 + 1;
                    screenPrintScore(0, score0, score1);
                    shellSetGameState(19);
                } else if (gCurrentLevel == 6) {
                    shellLeaveLevel(6);
                    shellSetGameState(30);
                } else if (gCurrentLevel != 5) {
                    advanced = 1;
                    shellLeaveLevel(gCurrentLevel);
                    do {
                        lvl = gCurrentLevel;
                    } while (0);
                    dbLoaded = 0;
                    gCurrentLevel = lvl + 1;
                    shellSetGameState(10);
                } else if (gAccessCodeDigit == 0) {
                    shellSetGameState(12);
                    gAccessCodeDigit = 1;
                } else {
                    shellSetGameState(13);
                    gAccessCodeDigit = 0;
                }
                break;
            }
            break;
        case 26:
            dbLoaded = 0;
            shellPlayCarEngineRev();
            shellLeaveLevel(gCurrentLevel - 1);
            gCurrentLevel = 0;
            shellBeginLevelTransition();
            screenTransitionLevel(gCurrentLevel, 0);
            shellSetGameState(10);
            break;
        case 13:
            soundInterruptPlayDA();
            soundStartPlayDA(2);
            soundInterruptPlayDA();
            screenPlayCarEnding(gVehicleID);
            if (uaGetDifficulty() >= 2) {
                gCurrentLevel = 6;
                st = 10;
            } else {
                st = 30;
            }
            shellSetGameState(st);
            dbLoaded = 0;
            break;
        case 14:
            shellPlayCarEngineRev();
            soundInterruptPlayDA();
            screenLostGame();
            shellSetGameState(30);
            gCurrentLevel = 0;
            break;
        case 15:
            shellStopCarEngineRev();
            if (soundGetCurrentDATrack() != 2) {
                soundInterruptPlayDA();
                soundStartPlayDA(2);
            }
            screenDisplayCredits();
            shellSetGameState(29);
            break;
        case 16:
            shellLeaveGame();
            shellSetGameState(2);
            break;
        default:
            shellSetGameState(0);
            break;
        }
    } while (quit == 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", main);
#endif

void timIntoVRAM(u_long* tim)
{
    RECT rect;
    GsIMAGE info;

    GsGetTimInfo(tim + 1, &info);
    rect.x = info.px;
    rect.y = info.py;
    rect.w = info.pw;
    rect.h = info.ph;
    LoadImage(&rect, info.pixel);
    if ((info.pmode >> 3) & 1) {
        rect.x = info.cx;
        rect.y = info.cy;
        rect.w = info.cw;
        rect.h = info.ch;
        LoadImage(&rect, info.clut);
    }
}

#ifndef NON_MATCHING
static const char sErrNotTms[] = "Error: %s isn't a valid TMS file\n";
static const char sErrNotCurrent[] = "Error: %s isn't the current version\n";
static const char sErrFileVersion[] = "       (the file is version %d, current version is %d)\n";
#endif

#ifdef NON_MATCHING
s32 shellTextureInit(u_long* p)
{
    s32 unused[2];
    s32 ret;
    s32 count;
    u32 size;
    s32 i;

    if (p == NULL) {
        return 0;
    }
    if (*p++ != 0x50535854) {
        printf("Error: %s isn't a valid TMS file\n", shellGetCurrentTextureFileName());
        exit(-1);
    }
    if (*p != 0x43) {
        printf("Error: %s isn't the current version\n", shellGetCurrentTextureFileName());
        printf("       (the file is version %d, current version is %d)\n", *p, 0x43);
        exit(-1);
    }
    p++;
    ret = *p;
    p++;
    count = *p;
    p++;
    size = *p;
    i = 0;
    if (count > 0) {
        do {
            p++;
            timIntoVRAM(p);
            p += size / 4;
            size = *p;
            i++;
        } while (i < count);
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellTextureInit);
#endif

s32 shellGetGameState(void)
{
    return gGameState;
}

void shellSetGameState(s32 state)
{
    s32 prev = gGameState;
    gGameState = state;
    gPreviousGameState = prev;
}

#ifdef NON_MATCHING
void shellSetAppropriateDBnames(s32 vehicle)
{
    char** entry = &dbNames[gCurrentLevel];
    char* last;
    char* str;
    s32 len;
    s32 lvl;

    len = strlen(*entry);
    do {
        str = *entry;
    } while (0);
    do {
        lvl = gCurrentLevel;
    } while (0);
    last = str + (len - 1);
    switch (lvl) {
    case 0:
        break;
    case 1:
    case 5:
        switch (vehicle) {
        case 10:
        case 20:
        case 30:
        case 50:
        case 60:
        case 100:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case 2:
        switch (vehicle) {
        case 10:
        case 20:
        case 30:
        case 40:
        case 110:
        case 120:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case 3:
        switch (vehicle) {
        case 10:
        case 40:
        case 60:
        case 100:
        case 110:
        case 120:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case 4:
        switch (vehicle) {
        case 20:
        case 30:
        case 100:
        case 120:
            *last = '2';
            break;
        default:
            *last = '1';
            break;
        }
        break;
    case 6:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellSetAppropriateDBnames);
#endif

#ifdef NON_MATCHING
char* shellGetCurrentDatabaseFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, gDbDir, dbNames[gCurrentLevel], gDbExt);
    return buffer;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetCurrentDatabaseFileName);
#endif

#ifdef NON_MATCHING
char* shellGetCurrentTextureFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, gDbDir, dbNames[gCurrentLevel], gTexExt);
    return buffer;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetCurrentTextureFileName);
#endif

#ifdef NON_MATCHING
char* shellGetCurrentLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == 6) {
        return gArena5Name;
    }
    return levelNames[lvl];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetCurrentLevelName);
#endif

#ifdef NON_MATCHING
char* shellGetLevelName(s32 level)
{
    if (level == 6) {
        return gArena5Name;
    }
    return levelNames[level];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetLevelName);
#endif

#ifdef NON_MATCHING
char* shellGetNextLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == 5) {
        return levelNames[0];
    }
    return levelNames[lvl + 1];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetNextLevelName);
#endif

s32 shellGetCurrentLevel(void)
{
    return gCurrentLevel;
}

s32 shellNextLevelChoice(s32 delta)
{
    s32 lvl = gCurrentLevel + delta;
    gCurrentLevel = lvl;
    if (lvl < 0) {
        gCurrentLevel = 5;
    } else if (lvl == 6) {
        gCurrentLevel = 0;
    }
    return gCurrentLevel;
}

#ifndef NON_MATCHING
static const char sUa2PlayDir[] = "UA2PLAY\\";
#endif

#ifdef NON_MATCHING
char* shellGetCurrentTwoPlayerDatabaseFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, "UA2PLAY\\", twoPlayerDbNames[gCurrentLevel], gDbExt);
    return buffer;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetCurrentTwoPlayerDatabaseFileName);
#endif

#ifdef NON_MATCHING
char* shellGetCurrentTwoPlayerTextureFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, "UA2PLAY\\", twoPlayerDbNames[gCurrentLevel], gTexExt);
    return buffer;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetCurrentTwoPlayerTextureFileName);
#endif

#ifndef NON_MATCHING
static const char sRoofTop[] = "ROOF TOP";
#endif

#ifdef NON_MATCHING
char* shellGetTwoPlayerCurrentLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == 4) {
        return gCanalName;
    }
    if (lvl == 5) {
        return "ROOF TOP";
    }
    return levelNames[lvl];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetTwoPlayerCurrentLevelName);
#endif

s32 shellGetTwoPlayerCurrentLevel(void)
{
    return gCurrentLevel;
}

s32 shellNextTwoPlayerLevelChoice(s32 delta)
{
    s32 lvl = gCurrentLevel + delta;
    gCurrentLevel = lvl;
    if (lvl < 0) {
        gCurrentLevel = 5;
    } else if (lvl == 6) {
        gCurrentLevel = 0;
    }
    return gCurrentLevel;
}

void shellInitMachine(void)
{
    SetVideoMode(MODE_NTSC);
    SetDispMask(0);
    ResetCallback();
    shellInitGraphicsSystem();
    fileioInit();
    soundInit();
    soundSetMusicVolume(0);
    soundStartPlayDA(2);
    soundInterruptPlayDA();
    fontInit();
    screenInit();
    screenClearScreen(0, 0, 320, 240);
    screenClearScreen(320, 0, 320, 240);
    screenSetEnv();
    SetDispMask(1);
    rtDoubleBufferInit();
}

void shellInitGraphicsSystem(void)
{
    ResetGraph(0);
    SetGraphDebug(0);
    InitGeom();
    InitTimer();
    InitCtlPad();
}

void shellInitGame(void)
{
    fileioLoadFileIntoRam(CORE_VAB_BUFFER, "SND\\UACORE.VAB");
    soundLoadCoreVabIntoSPURam(CORE_VAB_BUFFER);
    hierInit();
    uaInit();
}

void shellInitNewDatabase(s32 level, s32 tmsVersion)
{
    lightInit();
    uaInitCars();
    UAeffectInit();
    uaswInit();
    csInit();
    viewInit();
    carInitPickupWeapons();
    init_health_stands();
    clear_targets();
    clear_mercs();
    clear_pedestrians();
    lightInit();
    dbReset3DEnvironmentTrap();
    effectResetEffects();
    dbInit(tmsVersion);
    init_bullets();
    clear_explosions();
    init_missiles();
    InitContrails();
    uaInitDB(level);
    if (level != 7 && rtIsSplitScreenOn() == 0) {
        uadashLoadCockpit(gVehicleID);
    }
}

void shellInitLevel(s32 level)
{
    InitLevel(level);
    uadashInitBulletHoles();
}

void shellLeaveLevel(s32 level)
{
    soundInterruptPlayDA();
    uaLeaveLevel(level);
}

void shellLeaveGame(void)
{
    soundShutdown();
}

s32 shellGetAccessCode(void)
{
    return gAccessCode;
}

void shellSetAccessCode(s32 code)
{
    gAccessCode = code;
}

char* soundtrackTitles[7] = {
    "''TWISTED THEME''",
    "''CIRCUS METALLICUS''",
    "''ASPHALT ASSAULT''",
    "''CYBURB SLIDE''",
    "''CYBURB HUNT''",
    "''STALK N' ROLL''",
    "''DROP DEAD''",
};

s32 shellSoundForThisLevel(void)
{
    s32 split;
    s32 id;

    switch (gCurrentLevel) {
    case 0:
        return 3;
    case 1:
        split = rtIsSplitScreenOn();
        id = 4;
        goto chk;
    case 3:
        return 4;
    case 4:
        split = rtIsSplitScreenOn();
        id = 6;
    chk:
        if (split != 0) {
            id = 7;
        }
        return id;
    case 5:
        return 8;
    case 2:
    default:
        return 2;
    }
}

s32 shellGetSoundtrackId(void)
{
    switch (gSoundtrackChoice) {
    case 0:
    default:
        return 2;
    case 1:
        return 3;
    case 2:
        return 5;
    case 3:
        return 7;
    case 4:
        return 6;
    case 5:
        return 4;
    case 6:
        return 8;
    }
}

#ifdef NON_MATCHING
char* shellGetSoundtrackTitle(void)
{
    return soundtrackTitles[gSoundtrackChoice];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellGetSoundtrackTitle);
#endif

void shellSetSoundtrackChoice(s32 id)
{
    switch (id) {
    case 2:
        gSoundtrackChoice = 0;
        break;
    case 3:
        gSoundtrackChoice = 1;
        break;
    case 5:
        gSoundtrackChoice = 2;
        break;
    case 7:
        gSoundtrackChoice = 3;
        break;
    case 6:
        gSoundtrackChoice = 4;
        break;
    case 4:
        gSoundtrackChoice = 5;
        break;
    case 8:
        gSoundtrackChoice = 6;
        break;
    }
}

s32 shellNextSoundtrackChoice(s32 delta)
{
    s32 choice = gSoundtrackChoice + delta;
    gSoundtrackChoice = choice;
    if (choice < 0) {
        gSoundtrackChoice = 6;
    } else if (choice == 7) {
        gSoundtrackChoice = 0;
    }
    return gSoundtrackChoice;
}

void shellTurnCarHeadlightsOff(void)
{
    uaswCarHeadlightsOnOff(14, 0);
    uaswCarHeadlightsOnOff(24, 0);
    uaswCarHeadlightsOnOff(34, 0);
    uaswCarHeadlightsOnOff(44, 0);
    uaswCarHeadlightsOnOff(54, 0);
    uaswCarHeadlightsOnOff(64, 0);
    uaswCarHeadlightsOnOff(74, 0);
    uaswCarHeadlightsOnOff(84, 0);
    uaswCarHeadlightsOnOff(94, 0);
    uaswCarHeadlightsOnOff(104, 0);
    uaswCarHeadlightsOnOff(114, 0);
    uaswCarHeadlightsOnOff(124, 0);
    uaswCarHeadlightsOnOff(134, 0);
}

#ifdef NON_MATCHING
void shellSelectCards(void)
{
    uaSelectOpponents(gVehicleID, gCurrentLevel, &gNumOpponents, opponents);
    screenInitCardSprites(gCurrentLevel, gNumOpponents, opponents);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellSelectCards);
#endif

#ifdef NON_MATCHING
void shellLoadCards(void)
{
    screenLoadCarPictures(gNumOpponents, opponents);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellLoadCards);
#endif

void shellDrawCards(void)
{
    s32 i = -1;
    s32 j = 0;

    while (i < gNumOpponents + 1) {
        if (gNumOpponents - 1 < i) {
            screenDrawCard(gNumOpponents, 0, 1);
            screenDrawCard(gNumOpponents, 0, 1);
            i++;
        } else if (j % 11 == 0) {
            screenDrawCard(i, 1, 0);
            j++;
            uasoundPlayCardThrowFlush();
            screenResetCards(i);
            i++;
        } else {
            screenDrawCard(i, 0, 0);
            j++;
        }
    }
    DrawSync(0);
}

s32 shellProcessAccessCode(s32 code)
{
    s32 ok;

    soundProcessIds();
    switch (code) {
    case 1:
        gCurrentLevel = 1;
        gStartLevel = 1;
        ok = 1;
        break;
    case 2:
        gCurrentLevel = 2;
        gStartLevel = 2;
        ok = 1;
        break;
    case 3:
        gCurrentLevel = 3;
        gStartLevel = 3;
        ok = 1;
        break;
    case 4:
        gCurrentLevel = 4;
        gStartLevel = 4;
        ok = 1;
        break;
    case 5:
        gCurrentLevel = 5;
        gStartLevel = 5;
        ok = 1;
        break;
    case 9:
        ok = 1;
        screenBossCar();
        break;
    case 11:
        ok = 1;
        gCurrentLevel = 5;
        gStartLevel = 5;
        gAccessCodeDigit = 0;
        rtReturnToShell(3, 0);
        break;
    case 13:
        gCurrentLevel = 6;
        gStartLevel = 6;
        ok = 1;
        break;
    case 8:
        shellSetGodMode(0, 1);
        ok = 1;
        break;
    case 14:
        UASetHelicoptorMode(0, 1);
        ok = 1;
        break;
    case 7:
        shellSetInfiniteWeapons(0, 1);
        ok = 1;
        break;
    default:
        ok = 0;
        break;
    }
    if (ok == 1) {
        uasoundPlayCarWeaponPickup(0x19C, 0, 0);
    } else {
        uasoundStopCarSignature(10);
        uasoundPlayCarSignature(10, 0, 0);
    }
    soundProcessIds();
    screenWaitForContinue(60, 60);
    return ok;
}

s32 shellLivesRemaining(void)
{
    return gLivesRemaining;
}

void shellSetLivesRemaining(s32 lives)
{
    if (lives >= 4) {
        lives = 3;
    }
    gLivesRemaining = lives;
}

#ifdef NON_MATCHING
void shellBeginLevelTransition(void)
{
    s32 size;
    char* tex;

    DrawSync(0);
    uaSelectOpponents(gVehicleID, gCurrentLevel, &gNumOpponents, opponents);
    screenInitCardSprites(gCurrentLevel, gNumOpponents, opponents);
    if (rtIsSplitScreenOn() == 0) {
        shellSetAppropriateDBnames(gVehicleID);
        dbFilename = shellGetCurrentDatabaseFileName();
        tex = shellGetCurrentTextureFileName();
    } else {
        dbFilename = shellGetCurrentTwoPlayerDatabaseFileName();
        tex = shellGetCurrentTwoPlayerTextureFileName();
    }
    gFd = fileioOpenFileAsync(tex, &size);
    fileioReadFileAsync(gFd, LOAD_BUFFER, size);
    gTransitionState = 5;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellBeginLevelTransition);
#endif

#ifdef NON_MATCHING
s32 shellCheckTransitionStatus(void)
{
    s32 size;

    if (fileioAsyncReadCompleted() == 0) {
        return 0;
    }
    switch ((u32)gTransitionState) {
    case 1:
        fileioCloseFile(gFd);
        gTimeStamp = shellTextureInit(LOAD_BUFFER);
        gTransitionState = 2;
        break;
    case 2:
        DrawSync(0);
        gFd = fileioOpenFileAsync(dbFilename, &size);
        fileioReadFileAsync(gFd, LOAD_BUFFER, size);
        gTransitionState = 0;
        break;
    case 5:
        break;
    case 0:
        fileioCloseFile(gFd);
        break;
    default:
        fileioCloseFile(gFd);
        break;
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellCheckTransitionStatus);
#endif

s32 shellGetTransitionState(void)
{
    return gTransitionState;
}

void shellSetTransitionState(s32 state)
{
    gTransitionState = state;
}

#ifdef NON_MATCHING
void shellLoadCarsDatabase(void)
{
    char buf[40];
    char* fmt;
    char* dir;
    char* name;
    s32 tmsVersion;

    if (gCarsDbState != 0) {
        return;
    }
    soundInterruptPlayDA();
    fmt = gDbFmt;
    dir = gDbDir;
    name = gCarsName;
    sprintf(buf, fmt, dir, name, gTexExt);
    fileioLoadFileIntoRam(LOAD_BUFFER, buf);
    tmsVersion = shellTextureInit(LOAD_BUFFER);
    sprintf(buf, fmt, dir, name, gDbExt);
    fileioLoadFileIntoRam(LOAD_BUFFER, buf);
    shellInitNewDatabase(7, tmsVersion);
    shellTurnCarHeadlightsOff();
    screenLoadCarSelectionBackground();
    gCarsDbState = 1;
    soundResumePlayDA();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellLoadCarsDatabase);
#endif

#ifdef NON_MATCHING
void shellLoadCarsEndDatabase(void)
{
    char buf[40];
    char* fmt = gDbFmt;
    char* dir = gDbDir;
    char* name = gCarsEndName;
    s32 tmsVersion;

    soundInterruptPlayDA();
    sprintf(buf, fmt, dir, name, gTexExt);
    fileioLoadFileIntoRam(LOAD_BUFFER, buf);
    tmsVersion = shellTextureInit(LOAD_BUFFER);
    sprintf(buf, fmt, dir, name, gDbExt);
    fileioLoadFileIntoRam(LOAD_BUFFER, buf);
    shellInitNewDatabase(7, tmsVersion);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/shell", shellLoadCarsEndDatabase);
#endif

s32 shellGetCurrentVehicle(void)
{
    return gVehicleID;
}

void shellPlayCarEngineRev(void)
{
    s32 car = gVehicleID;
    do {
        if (car != -1) {
            uasoundPlayCarEngineRev(car, 0, 0, 30);
        } else {
            uasoundPlayScreenEngineRev(30);
        }
    } while (0);
    soundProcessIds();
}

void shellStopCarEngineRev(void)
{
    s32 car = gVehicleID;
    do {
        if (car != -1) {
            uasoundStopCarSounds(car);
        } else {
            uasoundStopScreenEngineRev();
        }
    } while (0);
    soundProcessIds();
}

void shellSetGodMode(s16 player, s32 on)
{
    UASetGodMode(player, on);
    gGodMode = on;
}

s32 shellIsGodMode(void)
{
    return gGodMode;
}

void shellSetInfiniteWeapons(s32 player, s32 on)
{
    UASetInfiniteWeapons(player, on);
    gInfiniteWeapons = on;
}

s32 shellIsInfiniteWeapons(void)
{
    return gInfiniteWeapons;
}

s32 shellIsPerfect(void)
{
    s32 perfect = 0;
    if ((gInfiniteWeapons | gGodMode) == 0) {
        perfect = (gStartLevel == 0);
    }
    return perfect;
}
