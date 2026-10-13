#include "common.h"
#include <LIBETC.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBGTE.H>
#include <STDIO.H>
#include <STRINGS.H>

#include "tm1/ACCESS.H"
#include "tm1/CS.H"
#include "tm1/CTLPAD.H"
#include "tm1/DB.H"
#include "tm1/DIFFICUL.H"
#include "tm1/EXPLODE.H"
#include "tm1/FILEIO.H"
#include "tm1/FONT.H"
#include "tm1/HIER.H"
#include "tm1/INTERACT.H"
#include "tm1/LEVELS.H"
#include "tm1/LIGHT.H"
#include "tm1/RT.H"
#include "tm1/SCREEN.H"
#include "tm1/SHELL.H"
#include "tm1/SOUND.H"
#include "tm1/STATE.H"
#include "tm1/TARGETS.H"
#include "tm1/TIMER.H"
#include "tm1/TRANSIT.H"
#include "tm1/UA.H"
#include "tm1/UAEFFECT.H"
#include "tm1/UA_DASH.H"
#include "tm1/UA_SOUND.H"
#include "tm1/UA_SW.H"
#include "tm1/VIEW.H"
#include "tm1/WEAPON.H"

#define LOAD_BUFFER ((void*)0x800188B8)
#define CORE_VAB_BUFFER ((void*)0x80010000)

extern void exit(s32 code);

s16 gAccessCodeDigit = 0;
s32 gCurrentLevel = LEVEL1_ARENA;
s32 gStartLevel = LEVEL1_ARENA;
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

static s32 opponents[13];

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
    shellSetGameState(GAME_STATE_LOGOS);
    do {
        switch (gGameState) {
        case GAME_STATE_LOGOS:
            screenDisplayLogos();
            shellSetGameState(GAME_STATE_TITLE_MOVIE);
            screenSetMusicVolume();
            screenSetEffectsVolume();
            break;
        case GAME_STATE_TITLE_MOVIE:
            ResetUpdateRate();
            rtSetSplitScreen(0);
            shellLoadCarsDatabase();
            screenPlayCinema(2);
            shellSetGameState(GAME_STATE_TITLE);
            soundSetPreviousMusicVolume(screenGetMusicVolume());
            soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            break;
        case GAME_STATE_TITLE:
            gAccessCodeDigit = 0;
            gCurrentLevel = LEVEL1_ARENA;
            gStartLevel = LEVEL1_ARENA;
            advanced = 0;
            ResetUpdateRate();
            rtSetSplitScreen(0);
            uaInitWeapons(1);
            shellStopCarEngineRev();
            gVehicleID = -1;
            if (soundGetCurrentDATrack() != DA_TRACK_TWISTED_THEME) {
                soundInterruptPlayDA();
                soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            }
            dbLoaded = 0;
            shellLoadCarsDatabase();
            soundSetPreviousMusicVolume(screenGetMusicVolume());
            soundResumePlayDA();
            shellSetGameState(screenDisplayTitle());
            gLivesRemaining = 3;
            break;
        case GAME_STATE_ATTRACT:
            switch (gShellFlag) {
            case 0:
                shellSetGameState(GAME_STATE_DEMO_MOVIE);
                gShellFlag = gShellFlag + 1;
                break;
            case 1:
                shellSetGameState(GAME_STATE_HISTORY);
                gShellFlag = gShellFlag + 1;
                break;
            case 2:
                shellSetGameState(GAME_STATE_VEHICLE_DEMO);
                gShellFlag = 0;
                break;
            }
            break;
        case GAME_STATE_MAIN_OPTIONS:
            shellSetGameState(screenMainOptions());
            break;
        case GAME_STATE_OPTIONS:
            shellPlayCarEngineRev();
            soundProcessIds();
            shellSetGameState(screenOptions(rtIsSplitScreenOn()));
            break;
        case GAME_STATE_VEHICLE_DEMO:
            if (soundGetCurrentDATrack() != DA_TRACK_TWISTED_THEME) {
                soundInterruptPlayDA();
                soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            }
            soundResumePlayDA();
            screenVehicleChoice(1, 1);
            shellSetGameState(GAME_STATE_TITLE_MOVIE);
            break;
        case GAME_STATE_HISTORY:
            if (soundGetCurrentDATrack() != DA_TRACK_TWISTED_THEME) {
                soundInterruptPlayDA();
                soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            }
            soundResumePlayDA();
            if (screenDisplayHistory() == 0) {
                shellLoadCarsDatabase();
                screenVehicleChoice(1, 1);
            }
            if (gPreviousGameState == GAME_STATE_ATTRACT) {
                shellSetGameState(GAME_STATE_TITLE_MOVIE);
            } else {
                shellSetGameState(gPreviousGameState);
            }
            break;
        case GAME_STATE_AUDIO_OPTIONS:
            screenAudioOptions(1, 1);
            shellSetGameState(GAME_STATE_MAIN_OPTIONS);
            break;
        case GAME_STATE_CHOOSE_CONTROLS:
            shellPlayCarEngineRev();
            if (rtIsSplitScreenOn() != 0) {
                screenChooseControls(2, screenGetWhichPad(), 1);
            } else {
                screenChooseControls(1, 0, 1);
            }
            shellSetGameState(GAME_STATE_OPTIONS);
            break;
        case GAME_STATE_CHOOSE_BATTLEGROUND:
            shellPlayCarEngineRev();
            screenChooseBattleground(rtIsSplitScreenOn());
            if (rtIsSplitScreenOn() == 0) {
                if (gCurrentLevel != LEVEL1_ARENA) {
                    shellSetGameState(GAME_STATE_ACCESS_CODE);
                } else {
                    shellSetGameState(GAME_STATE_OPTIONS);
                }
            } else {
                shellSetGameState(GAME_STATE_OPTIONS);
            }
            break;
        case GAME_STATE_BATTLE_OPTIONS:
            shellPlayCarEngineRev();
            shellSetGameState(screenBattleOptions(rtIsSplitScreenOn()));
            break;
        case GAME_STATE_ACCESS_CODE:
            shellPlayCarEngineRev();
            ok = shellProcessAccessCode(screenEnterAccessCode());
            st = GAME_STATE_OPTIONS;
            if (ok == 0) {
                st = screenWrongPassword(st);
            }
            shellSetGameState(st);
            break;
        case GAME_STATE_2P_NEXT_ROUND:
            gLivesRemaining = 1;
            uaInitWeapons(1);
            shellSetGameState(GAME_STATE_LOAD_LEVEL);
            break;
        case GAME_STATE_1P_VEHICLE_SELECT:
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
                shellSetGameState(GAME_STATE_BATTLE_OPTIONS);
            } else {
                shellSetGameState(GAME_STATE_TITLE);
            }
            break;
        case GAME_STATE_2P_VEHICLE_SELECT:
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
            gCurrentLevel = LEVEL1_ARENA;
            dbLoaded = 0;
            if (vid != -1) {
                rtSetSplitScreen(1);
                shellSetGameState(GAME_STATE_BATTLE_OPTIONS);
            } else {
                shellSetGameState(GAME_STATE_TITLE);
            }
            break;
        case GAME_STATE_DEMO_MOVIE:
        case GAME_STATE_UNK_28:
            soundInterruptPlayDA();
            screenPlayCinema(1);
            if (gPreviousGameState == GAME_STATE_ATTRACT) {
                shellSetGameState(GAME_STATE_TITLE_MOVIE);
            } else {
                shellSetGameState(gPreviousGameState);
            }
            soundResumePlayDA();
            break;
        case GAME_STATE_LOAD_LEVEL:
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
            shellSetGameState(GAME_STATE_PLAY_LEVEL);
            break;
        case GAME_STATE_BOSS_CAR:
            screenBossCar();
            uaInitBossCar();
            shellSetGameState(GAME_STATE_PLAY_LEVEL);
            break;
        case GAME_STATE_PLAY_LEVEL:
            soundResetSoundEngine();
            soundStartPlayDA(shellSoundForThisLevel());
            r = rtMainLoop();
            soundResetSoundEngine();
            rtClearPendingReturns();
            soundForceAllSoundsOffNow();
            shellPlayCarEngineRev();
            soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            soundInterruptPlayDA();
            switch (r) {
            case RT_RETURN_P2_WON:
                dbLoaded = 0;
                score1 = score1 + 1;
                screenPrintScore(1, score0, score1);
                shellSetGameState(GAME_STATE_2P_NEXT_ROUND);
                uaInitWeapons(1);
                break;
            case RT_RETURN_ABORTED:
                shellSetGameState(GAME_STATE_TITLE_MOVIE);
                uaInitWeapons(1);
                break;
            case RT_RETURN_PLAYER_DIED:
                gLivesRemaining = gLivesRemaining - 1;
                if (gLivesRemaining == 0) {
                    shellSetGameState(GAME_STATE_LOST_GAME);
                    uaInitWeapons(1);
                } else {
                    screenLostALife(gLivesRemaining);
                    shellSetGameState(GAME_STATE_LOAD_LEVEL);
                }
                break;
            case RT_RETURN_WON:
            default:
                if (rtIsSplitScreenOn() != 0) {
                    dbLoaded = 0;
                    score0 = score0 + 1;
                    screenPrintScore(0, score0, score1);
                    shellSetGameState(GAME_STATE_2P_NEXT_ROUND);
                } else if (gCurrentLevel == LEVEL7_ARENA_5) {
                    shellLeaveLevel(LEVEL7_ARENA_5);
                    shellSetGameState(GAME_STATE_TITLE_MOVIE);
                } else if (gCurrentLevel != LEVEL6_ROOFTOPS) {
                    advanced = 1;
                    shellLeaveLevel(gCurrentLevel);
                    do {
                        lvl = gCurrentLevel;
                    } while (0);
                    dbLoaded = 0;
                    gCurrentLevel = lvl + 1;
                    shellSetGameState(GAME_STATE_LOAD_LEVEL);
                } else if (gAccessCodeDigit == 0) {
                    shellSetGameState(GAME_STATE_BOSS_CAR);
                    gAccessCodeDigit = 1;
                } else {
                    shellSetGameState(GAME_STATE_CAR_ENDING);
                    gAccessCodeDigit = 0;
                }
                break;
            }
            break;
        case GAME_STATE_RESTART_CONTEST:
            dbLoaded = 0;
            shellPlayCarEngineRev();
            shellLeaveLevel(gCurrentLevel - 1);
            gCurrentLevel = LEVEL1_ARENA;
            shellBeginLevelTransition();
            screenTransitionLevel(gCurrentLevel, 0);
            shellSetGameState(GAME_STATE_LOAD_LEVEL);
            break;
        case GAME_STATE_CAR_ENDING:
            soundInterruptPlayDA();
            soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            soundInterruptPlayDA();
            screenPlayCarEnding(gVehicleID);
            if (uaGetDifficulty() >= DIFFICULTY_HARD) {
                gCurrentLevel = LEVEL7_ARENA_5;
                st = GAME_STATE_LOAD_LEVEL;
            } else {
                st = GAME_STATE_TITLE_MOVIE;
            }
            shellSetGameState(st);
            dbLoaded = 0;
            break;
        case GAME_STATE_LOST_GAME:
            shellPlayCarEngineRev();
            soundInterruptPlayDA();
            screenLostGame();
            shellSetGameState(GAME_STATE_TITLE_MOVIE);
            gCurrentLevel = LEVEL1_ARENA;
            break;
        case GAME_STATE_CREDITS:
            shellStopCarEngineRev();
            if (soundGetCurrentDATrack() != DA_TRACK_TWISTED_THEME) {
                soundInterruptPlayDA();
                soundStartPlayDA(DA_TRACK_TWISTED_THEME);
            }
            screenDisplayCredits();
            shellSetGameState(GAME_STATE_MAIN_OPTIONS);
            break;
        case GAME_STATE_LEAVE_GAME:
            shellLeaveGame();
            shellSetGameState(GAME_STATE_ATTRACT);
            break;
        default:
            shellSetGameState(GAME_STATE_LOGOS);
            break;
        }
    } while (quit == 0);
}

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
INCLUDE_ASM("asm/nonmatchings/tm1/SHELL", shellTextureInit);
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
    case LEVEL1_ARENA:
        break;
    case LEVEL2_WAREHOUSE:
    case LEVEL6_ROOFTOPS:
        switch (vehicle) {
        case UA_ICE_CREAM_TRUCK:
        case UA_TAXI_CAB:
        case UA_SEMI_TRUCK:
        case UA_POLICE_CAR:
        case UA_LAMBORGHINI:
        case UA_LOW_RIDER:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case LEVEL3_FREEWAY:
        switch (vehicle) {
        case UA_ICE_CREAM_TRUCK:
        case UA_TAXI_CAB:
        case UA_SEMI_TRUCK:
        case UA_MONSTER_TRUCK:
        case UA_CORVETTE:
        case UA_MAD_MAX:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case LEVEL4_CITY_PARK:
        switch (vehicle) {
        case UA_ICE_CREAM_TRUCK:
        case UA_MONSTER_TRUCK:
        case UA_LAMBORGHINI:
        case UA_LOW_RIDER:
        case UA_CORVETTE:
        case UA_MAD_MAX:
            *last = '1';
            break;
        default:
            *last = '2';
            break;
        }
        break;
    case LEVEL5_CYBURBIA:
        switch (vehicle) {
        case UA_TAXI_CAB:
        case UA_SEMI_TRUCK:
        case UA_LOW_RIDER:
        case UA_MAD_MAX:
            *last = '2';
            break;
        default:
            *last = '1';
            break;
        }
        break;
    case LEVEL7_ARENA_5:
        break;
    }
}

char* shellGetCurrentDatabaseFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, gDbDir, dbNames[gCurrentLevel], gDbExt);
    return buffer;
}

char* shellGetCurrentTextureFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, gDbDir, dbNames[gCurrentLevel], gTexExt);
    return buffer;
}

char* shellGetCurrentLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == LEVEL7_ARENA_5) {
        return gArena5Name;
    }
    return levelNames[lvl];
}

char* shellGetLevelName(s32 level)
{
    if (level == LEVEL7_ARENA_5) {
        return gArena5Name;
    }
    return levelNames[level];
}

char* shellGetNextLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == LEVEL6_ROOFTOPS) {
        return levelNames[LEVEL1_ARENA];
    }
    return levelNames[lvl + 1];
}

s32 shellGetCurrentLevel(void)
{
    return gCurrentLevel;
}

s32 shellNextLevelChoice(s32 delta)
{
    s32 lvl = gCurrentLevel + delta;
    gCurrentLevel = lvl;
    if (lvl < 0) {
        gCurrentLevel = LEVEL6_ROOFTOPS;
    } else if (lvl == LEVEL6_ROOFTOPS + 1) {
        gCurrentLevel = LEVEL1_ARENA;
    }
    return gCurrentLevel;
}

char* shellGetCurrentTwoPlayerDatabaseFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, "UA2PLAY\\", twoPlayerDbNames[gCurrentLevel], gDbExt);
    return buffer;
}

char* shellGetCurrentTwoPlayerTextureFileName(void)
{
    static char buffer[24];

    sprintf(buffer, gDbFmt, "UA2PLAY\\", twoPlayerDbNames[gCurrentLevel], gTexExt);
    return buffer;
}

char* shellGetTwoPlayerCurrentLevelName(void)
{
    s32 lvl = gCurrentLevel;

    if (lvl == LEVEL5_CYBURBIA) {
        return gCanalName;
    }
    if (lvl == LEVEL6_ROOFTOPS) {
        return "ROOF TOP";
    }
    return levelNames[lvl];
}

s32 shellGetTwoPlayerCurrentLevel(void)
{
    return gCurrentLevel;
}

s32 shellNextTwoPlayerLevelChoice(s32 delta)
{
    s32 lvl = gCurrentLevel + delta;
    gCurrentLevel = lvl;
    if (lvl < 0) {
        gCurrentLevel = LEVEL6_ROOFTOPS;
    } else if (lvl == LEVEL6_ROOFTOPS + 1) {
        gCurrentLevel = LEVEL1_ARENA;
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
    soundStartPlayDA(DA_TRACK_TWISTED_THEME);
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
    case LEVEL1_ARENA:
        return DA_TRACK_CIRCUS_METALLICUS;
    case LEVEL2_WAREHOUSE:
        split = rtIsSplitScreenOn();
        id = DA_TRACK_STALK_N_ROLL;
        goto chk;
    case LEVEL4_CITY_PARK:
        return DA_TRACK_STALK_N_ROLL;
    case LEVEL5_CYBURBIA:
        split = rtIsSplitScreenOn();
        id = DA_TRACK_CYBURB_HUNT;
    chk:
        if (split != 0) {
            id = DA_TRACK_CYBURB_SLIDE;
        }
        return id;
    case LEVEL6_ROOFTOPS:
        return DA_TRACK_DROP_DEAD;
    case LEVEL3_FREEWAY:
    default:
        return DA_TRACK_TWISTED_THEME;
    }
}

s32 shellGetSoundtrackId(void)
{
    switch (gSoundtrackChoice) {
    case 0:
    default:
        return DA_TRACK_TWISTED_THEME;
    case 1:
        return DA_TRACK_CIRCUS_METALLICUS;
    case 2:
        return DA_TRACK_ASPHALT_ASSAULT;
    case 3:
        return DA_TRACK_CYBURB_SLIDE;
    case 4:
        return DA_TRACK_CYBURB_HUNT;
    case 5:
        return DA_TRACK_STALK_N_ROLL;
    case 6:
        return DA_TRACK_DROP_DEAD;
    }
}

char* shellGetSoundtrackTitle(void)
{
    return soundtrackTitles[gSoundtrackChoice];
}

void shellSetSoundtrackChoice(s32 id)
{
    switch (id) {
    case DA_TRACK_TWISTED_THEME:
        gSoundtrackChoice = 0;
        break;
    case DA_TRACK_CIRCUS_METALLICUS:
        gSoundtrackChoice = 1;
        break;
    case DA_TRACK_ASPHALT_ASSAULT:
        gSoundtrackChoice = 2;
        break;
    case DA_TRACK_CYBURB_SLIDE:
        gSoundtrackChoice = 3;
        break;
    case DA_TRACK_CYBURB_HUNT:
        gSoundtrackChoice = 4;
        break;
    case DA_TRACK_STALK_N_ROLL:
        gSoundtrackChoice = 5;
        break;
    case DA_TRACK_DROP_DEAD:
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

inline void shellSelectCards(void)
{
    uaSelectOpponents(gVehicleID, gCurrentLevel, &gNumOpponents, opponents);
    screenInitCardSprites(gCurrentLevel, gNumOpponents, opponents);
}

void shellLoadCards(void)
{
    screenLoadCarPictures(gNumOpponents, opponents);
}

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
    case ACCESS_CODE_WAREHOUSE:
        gCurrentLevel = LEVEL2_WAREHOUSE;
        gStartLevel = LEVEL2_WAREHOUSE;
        ok = 1;
        break;
    case ACCESS_CODE_FREEWAY:
        gCurrentLevel = LEVEL3_FREEWAY;
        gStartLevel = LEVEL3_FREEWAY;
        ok = 1;
        break;
    case ACCESS_CODE_CITY_PARK:
        gCurrentLevel = LEVEL4_CITY_PARK;
        gStartLevel = LEVEL4_CITY_PARK;
        ok = 1;
        break;
    case ACCESS_CODE_CYBURBIA:
        gCurrentLevel = LEVEL5_CYBURBIA;
        gStartLevel = LEVEL5_CYBURBIA;
        ok = 1;
        break;
    case ACCESS_CODE_ROOFTOPS:
        gCurrentLevel = LEVEL6_ROOFTOPS;
        gStartLevel = LEVEL6_ROOFTOPS;
        ok = 1;
        break;
    case ACCESS_CODE_BOSS_SCREEN:
        ok = 1;
        screenBossCar();
        break;
    case ACCESS_CODE_MINION:
        ok = 1;
        gCurrentLevel = LEVEL6_ROOFTOPS;
        gStartLevel = LEVEL6_ROOFTOPS;
        gAccessCodeDigit = 0;
        rtReturnToShell(RT_RETURN_WON, 0);
        break;
    case ACCESS_CODE_ARENA_5:
        gCurrentLevel = LEVEL7_ARENA_5;
        gStartLevel = LEVEL7_ARENA_5;
        ok = 1;
        break;
    case ACCESS_CODE_GOD_MODE:
        shellSetGodMode(0, 1);
        ok = 1;
        break;
    case ACCESS_CODE_HELICOPTER_VIEW:
        UASetHelicoptorMode(0, 1);
        ok = 1;
        break;
    case ACCESS_CODE_INFINITE_WEAPONS:
        shellSetInfiniteWeapons(0, 1);
        ok = 1;
        break;
    default:
        ok = 0;
        break;
    }
    if (ok == 1) {
        uasoundPlayCarWeaponPickup(UA_PICKUP_BOMB, 0, 0);
    } else {
        uasoundStopCarSignature(UA_ICE_CREAM_TRUCK);
        uasoundPlayCarSignature(UA_ICE_CREAM_TRUCK, 0, 0);
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

void shellBeginLevelTransition(void)
{
    s32 size;
    char* tex;

    DrawSync(0);
    shellSelectCards();
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
    gTransitionState = TRANSITION_READING_TEXTURES;
}

s32 shellCheckTransitionStatus(void)
{
    s32 size;

    if (fileioAsyncReadCompleted() == 0) {
        return 0;
    }
    switch ((u32)gTransitionState) {
    case TRANSITION_INIT_TEXTURES:
        fileioCloseFile(gFd);
        gTimeStamp = shellTextureInit(LOAD_BUFFER);
        gTransitionState = TRANSITION_START_DATABASE;
        break;
    case TRANSITION_START_DATABASE:
        DrawSync(0);
        gFd = fileioOpenFileAsync(dbFilename, &size);
        fileioReadFileAsync(gFd, LOAD_BUFFER, size);
        gTransitionState = TRANSITION_READING_DATABASE;
        break;
    case TRANSITION_READING_TEXTURES:
        break;
    case TRANSITION_READING_DATABASE:
        fileioCloseFile(gFd);
        break;
    default:
        fileioCloseFile(gFd);
        break;
    }
    return 1;
}

s32 shellGetTransitionState(void)
{
    return gTransitionState;
}

void shellSetTransitionState(s32 state)
{
    gTransitionState = state;
}

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
        perfect = (gStartLevel == LEVEL1_ARENA);
    }
    return perfect;
}
