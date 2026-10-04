#include "common.h"

#include <libgte.h>
#include <rand.h>

#include "tm1/ai_car.h"
#include "tm1/ai_car_init.h"
#include "tm1/ai_car_update.h"
#include "tm1/bridges.h"
#include "tm1/car_init.h"
#include "tm1/car_update.h"
#include "tm1/ctlpad.h"
#include "tm1/curbs.h"
#include "tm1/explosion.h"
#include "tm1/hud.h"
#include "tm1/interactives.h"
#include "tm1/light.h"
#include "tm1/math.h"
#include "tm1/potholes.h"
#include "tm1/rt.h"
#include "tm1/slick_spots.h"
#include "tm1/smooth.h"
#include "tm1/sound.h"
#include "tm1/targets.h"
#include "tm1/timer.h"
#include "tm1/trigger_pts.h"
#include "tm1/ua_dash.h"
#include "tm1/ua_sound.h"
#include "tm1/ua_sw.h"
#include "tm1/view.h"
#include "tm1/wdcopy.h"

#include "tm1/ua.h"

static s16 numPlayers = 0;
static s16 numCs = 0;
static s16 numTargets = 0;
static u8 gStandMode = 0;
static s32 gVar33C = 1;
static s32 gVar340 = 0;
static s32 gVar344 = 1;
static s32 gVar348 = 0;
static s32 gVar34C = 0;
static s32 gVar350 = 0;
static s32 playerCamFollow = 0;
static s32 aiCamFollow = 0;
static s32 twoPlayerMode = 0;
static s32 beatThisLevel = 0;
static u8 gHeliMode = 0;
static s32 gVar368 = 0;
static s32 difficulty = 1;
static s32 gVar370 = 0;
static u8 gLevelFlag = 0;
static s32 gCurLevel = 1;

char* gCarNameStrings[13] = {
    "SWEET TOOTH",
    "YELLOW JACKET",
    "DARKSIDE",
    "OUTLAW",
    "THUMPER",
    "CRIMSON FURY",
    "PIT VIPER",
    "WARTHOG",
    "MR GRIMM",
    "SPECTRE",
    "HAMMERHEAD",
    "ROAD KILL",
    "MINION",
};

u16 Level2OpponentBitMask[12] = {
    0x0016,
    0x0034,
    0x0032,
    0x0016,
    0x0026,
    0x0016,
    0x0980,
    0x0940,
    0x08C0,
    0x01C0,
    0x01C0,
    0x01C0,
};

u16 Level3OpponentBitMask[12] = {
    0x0E06,
    0x0E05,
    0x0E03,
    0x01F0,
    0x01E8,
    0x01D8,
    0x01B8,
    0x0178,
    0x00F8,
    0x0C07,
    0x0A07,
    0x0607,
};

u16 Level4OpponentBitMask[12] = {
    0x0E32,
    0x01CD,
    0x01CB,
    0x01C7,
    0x0E23,
    0x0E13,
    0x018F,
    0x014F,
    0x00CF,
    0x0C33,
    0x0A33,
    0x0633,
};

u16 Level5OpponentBitMask[12] = {
    0x07EC,
    0x0F95,
    0x0F93,
    0x07E5,
    0x0F87,
    0x07CD,
    0x07AD,
    0x076D,
    0x06ED,
    0x05ED,
    0x03ED,
    0x0797,
};

u16 Level6OpponentBitMask[12] = {
    0x0038,
    0x0038,
    0x0038,
    0x0034,
    0x002C,
    0x001C,
    0x0E00,
    0x0E00,
    0x0E00,
    0x0C40,
    0x0A40,
    0x0640,
};

u16* OpponentBitMasks[6] = {
    NULL,
    Level2OpponentBitMask,
    Level3OpponentBitMask,
    Level4OpponentBitMask,
    Level5OpponentBitMask,
    Level6OpponentBitMask,
};

s32 CarNumToCarName[12] = {
    10,
    20,
    30,
    50,
    100,
    60,
    90,
    70,
    80,
    110,
    40,
    120,
};

static u8 gBattleMusicOn = 0;
static s32 gVar3A4 = 0;
static s32 gVar3A8 = 0;
static u16 gVrEnabled[2] = { 1, 1 };

extern s32 bgColor[3];

void UASetBattleMusicOn(void)
{
    gBattleMusicOn = 1;
}

#ifdef NON_MATCHING
void UAAddCs(Cs* cs, s32 name)
{
    Cs** pcs;
    s32* pname;
    s32* ptype;
    s16 n;

    if (numCs < 12) {
        pcs = &carCs[numCs];
        pname = &carName[numCs];
        *pcs = cs;
        *pname = name;
        csSetDrawMode(*pcs, 0);
        n = numCs;
        ptype = &carTypes[n];
        *ptype = 2;
        numCs = n + 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAAddCs);
#endif

void uaInit(void)
{
    s16 i;

    gVar340 = 0;
    gVar344 = 1;
    gVar348 = 0;
    gVar34C = 0;
    gStandMode = 0;
    uaInitWeapons(1);
    for (i = 0; i < 2; i++) {
        playerInfo[i].stats.unk4C = 1;
    }
    uaInitCars();
    for (i = 0; i < 2; i++) {
        PadSetConfig(1, i);
    }
    camera[0] = NULL;
    camera[1] = NULL;
    UASetNumPlayers(1);
    playerCar[0] = 20;
    srand(GetCurTics());
}

#ifdef NON_MATCHING
void uaInitWeapons(s32 on)
{
    s16 i;

    for (i = 0; i < 2; i++) {
        gInitPlayerWeapons[i] = on;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaInitWeapons);
#endif

#ifdef NON_MATCHING
void uaInitCars(void)
{
    s16 i;

    for (i = 0; i < numCs; i++) {
        carTypes[i] = 2;
        carIndexToPlayerOrAIIndex[i] = -1;
    }
    for (i = 0; i < numTargets; i++) {
        aiIndexToCarIndex[i] = -1;
    }
    numTargets = 0;
    numCs = 0;
    camera[0] = NULL;
    camera[1] = NULL;
    clear_targets();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaInitCars);
#endif

#ifdef NON_MATCHING
void uaInitDB(u32 level)
{
    s16 i;
    Cs* cs;
    s16 t;
    s32 n;

    hudInitRadar();
    if (rtIsSplitScreenOn()) {
        twoPlayerMode = 1;
    } else {
        twoPlayerMode = 0;
    }
    UASetNumPlayers(twoPlayerMode ? 2 : 1);
    i = 0;
    if (numPlayers > 0) {
        do {
            playerInfo[i].stats.colorId = 1;
            InitPlayerCar(i, playerCar[i], 1);
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswInitCarSwitch(carCs[i], carName[i]);
            t = i + 1;
            do {
            } while (0);
            n = numCs;
            i = t;
        } while (t < n);
    }
    InitAICarsInBattle();
    switch (level) {
    case 0:
    case 6:
    default:
        InitLevel1DBSpecifics();
        break;
    case 1:
        InitLevel2DBSpecifics();
        break;
    case 2:
        InitLevel3DBSpecifics();
        break;
    case 3:
        InitLevel4DBSpecifics();
        break;
    case 4:
        InitLevel5DBSpecifics();
        break;
    case 5:
        InitLevel6DBSpecifics();
        break;
    }
    if (!twoPlayerMode) {
        uaPickAICars(level);
    }
    i = 0;
    if (numTargets > 0) {
        do {
            cs = GetAICs3D(aiCarInfo[i].playerIdx);
            if (cs != NULL) {
                aiCarInfo[i].stats.colorId = ((DrawNode*)cs->epNode)->drawFlag;
            } else {
                aiCarInfo[i].stats.colorId = 1;
            }
            t = i + 1;
            do {
            } while (0);
            n = numTargets;
            i = t;
        } while (t < n);
    }
    srand(GetCurTics());
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaInitDB);
#endif

#ifdef NON_MATCHING
void uaInitView(Db* db, s32 entry, s32 which)
{
    if (camera[which] == NULL) {
        viewCreate((ViewDb*)db, entry, which);
        camera[which] = csCreate();
        viewSetParent(camera[which], which);
    }
    helicoptorCS = csCreate();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaInitView);
#endif

#ifdef NON_MATCHING
void InitLevel1DBSpecifics(void)
{
    s32* p;

    gCurLevel = 1;
    gLevelFlag = 0;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    p[3] = 128;
    p[4] = 75;
    p[5] = 75;
    p[6] = 60;
    p[7] = 128;
    p[8] = 60;
    bgColor[0] = 40;
    bgColor[1] = 40;
    bgColor[2] = 40;
    viewSetBgColor();
    InitLevel1TriggerPoints();
    InitLevel1Bridges();
    InitLevel1Curbs();
    InitLevel1PotHoles();
    InitLevel1SlickSpots();
    gVar348 = 1;
    gVar34C = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel1DBSpecifics);
#endif

#ifdef NON_MATCHING
void InitLevel2DBSpecifics(void)
{
    s32* p;

    gCurLevel = 2;
    gLevelFlag = 1;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    p[3] = 128;
    p[4] = 75;
    p[5] = 75;
    p[6] = 60;
    p[7] = 128;
    p[8] = 60;
    bgColor[0] = 75;
    bgColor[1] = 70;
    bgColor[2] = 55;
    viewSetBgColor();
    InitLevel2TriggerPoints();
    InitLevel2Bridges();
    InitLevel2Curbs();
    InitLevel2PotHoles();
    InitLevel2SlickSpots();
    gVar348 = 1;
    gVar34C = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel2DBSpecifics);
#endif

#ifdef NON_MATCHING
void InitLevel3DBSpecifics(void)
{
    s32* p;

    gCurLevel = 3;
    gLevelFlag = 1;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    p[3] = 128;
    p[4] = 75;
    p[5] = 75;
    p[6] = 60;
    p[7] = 128;
    p[8] = 60;
    bgColor[0] = 60;
    bgColor[1] = 45;
    bgColor[2] = 20;
    viewSetBgColor();
    InitLevel3TriggerPoints();
    InitLevel3Bridges();
    InitLevel3Curbs();
    InitLevel3PotHoles();
    InitLevel3SlickSpots();
    gVar348 = 1;
    gVar34C = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel3DBSpecifics);
#endif

#ifdef NON_MATCHING
void InitLevel4DBSpecifics(void)
{
    s32* p;

    gCurLevel = 4;
    gLevelFlag = 1;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    p[3] = 128;
    p[4] = 75;
    p[5] = 75;
    p[6] = 60;
    p[7] = 128;
    p[8] = 60;
    bgColor[0] = 75;
    bgColor[1] = 60;
    bgColor[2] = 30;
    viewSetBgColor();
    InitLevel4TriggerPoints();
    InitLevel4Bridges();
    InitLevel4Curbs();
    InitLevel4PotHoles();
    InitLevel4SlickSpots();
    gVar348 = 1;
    gVar34C = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel4DBSpecifics);
#endif

#ifdef NON_MATCHING
void InitLevel5DBSpecifics(void)
{
    s32* p;

    gCurLevel = 5;
    gLevelFlag = 1;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    bgColor[0] = 110;
    bgColor[1] = 110;
    bgColor[2] = 100;
    viewSetBgColor();
    InitLevel5TriggerPoints();
    InitLevel5Bridges();
    InitLevel5Curbs();
    InitLevel5PotHoles();
    InitLevel5SlickSpots();
    gVar348 = 0;
    gVar34C = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel5DBSpecifics);
#endif

#ifdef NON_MATCHING
void InitLevel6DBSpecifics(void)
{
    s32* p;

    gCurLevel = 6;
    gLevelFlag = 0;
    if (rtIsSplitScreenOn()) {
        uaInitView(rtGetDb(), 11, 0);
        uaInitView(rtGetSplitScreenDb(), 12, 1);
    } else {
        uaInitView(rtGetDb(), 6, 0);
        uaInitView(rtGetRearViewDb(), 13, 1);
    }
    p = lightGetAmbient();
    p[0] = 175;
    p[1] = 175;
    p[2] = 175;
    p = lightGetRot();
    p[0] = 30;
    p[1] = 0;
    p[2] = 170;
    p[3] = 15;
    p[4] = 0;
    p[5] = -60;
    p[6] = 20;
    p[7] = 0;
    p[8] = 60;
    p = lightGetColor();
    p[0] = 200;
    p[1] = 200;
    p[2] = 200;
    p[3] = 128;
    p[4] = 75;
    p[5] = 75;
    p[6] = 60;
    p[7] = 128;
    p[8] = 60;
    bgColor[0] = 0;
    bgColor[1] = 20;
    bgColor[2] = 0;
    viewSetBgColor();
    InitLevel6TriggerPoints();
    InitLevel6Bridges();
    InitLevel6Curbs();
    InitLevel6PotHoles();
    InitLevel6SlickSpots();
    gVar348 = 0;
    gVar34C = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitLevel6DBSpecifics);
#endif

void InitLevel(u32 level)
{
    s16 i;
    s16 t;
    s32 n;

    switch (level) {
    case 0:
    default:
        InitUALevel1();
        break;
    case 1:
        InitUALevel2();
        break;
    case 2:
        InitUALevel3();
        break;
    case 3:
        InitUALevel4();
        break;
    case 4:
        InitUALevel5();
        break;
    case 5:
        InitUALevel6();
        break;
    case 6:
        InitUALevel7();
        break;
    }
    uaInitWeapons(0);
    i = 0;
    if (numPlayers > 0) {
        do {
            if (playerInfo[i].flags[25] != 0) {
                uaswSetCarShadow(playerInfo[i].uaIndex, 1);
            } else {
                uaswSetCarShadow(playerInfo[i].uaIndex, 0);
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numTargets > 0) {
        do {
            if (aiCarInfo[i].flags[25] != 0) {
                uaswSetCarShadow(aiCarInfo[i].uaIndex, 1);
            } else {
                uaswSetCarShadow(aiCarInfo[i].uaIndex, 0);
            }
            t = i + 1;
            do {
            } while (0);
            n = numTargets;
            i = t;
        } while (t < n);
    }
    viewSetParent(camera[0], 0);
    if (camera[1] != NULL) {
        viewSetParent(camera[1], 1);
    }
    gVar33C = 1;
    skipLevel = 0;
    beatThisLevel = 0;
    srand(GetCurTics());
}

#ifdef NON_MATCHING
void InitUALevel1(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    AICarInit(&helicoptor, -1, 1);
    helicoptor.stats.triggerPt = GetClosestTriggerPt(&helicoptor, 0);
    InitTriggerPoint(&helicoptor);
    helicoptor.unk01 = 0;
    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
                if (playerInfo[i].unkF4 == 4) {
                    InitHelicoptorPosition();
                }
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel1);
#endif

#ifdef NON_MATCHING
void InitUALevel2(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                if (playerInfo[i].unkF8 == 4) {
                    playerInfo[i].unkF8 = 2;
                }
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel2);
#endif

#ifdef NON_MATCHING
void InitUALevel3(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                if (playerInfo[i].unkF8 == 4) {
                    playerInfo[i].unkF8 = 2;
                }
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel3);
#endif

#ifdef NON_MATCHING
void InitUALevel4(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                if (playerInfo[i].unkF8 == 4) {
                    playerInfo[i].unkF8 = 2;
                }
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel4);
#endif

#ifdef NON_MATCHING
void InitUALevel5(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                if (playerInfo[i].unkF8 == 4) {
                    playerInfo[i].unkF8 = 2;
                }
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel5);
#endif

#ifdef NON_MATCHING
void InitUALevel6(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    AICarInit(&helicoptor, -1, 1);
    helicoptor.stats.triggerPt = GetClosestTriggerPt(&helicoptor, 0);
    InitTriggerPoint(&helicoptor);
    helicoptor.unk01 = 0;
    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
                if (playerInfo[i].unkF4 == 4) {
                    InitHelicoptorPosition();
                }
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel6);
#endif

#ifdef NON_MATCHING
void InitUALevel7(void)
{
    s16 i;
    s16 t;
    s32 n;
    LightEnv* e;
    Cs* cs;

    AICarInit(&helicoptor, -1, 1);
    helicoptor.stats.triggerPt = GetClosestTriggerPt(&helicoptor, 0);
    InitTriggerPoint(&helicoptor);
    helicoptor.unk01 = 0;
    InitAICarsInBattle();
    i = 0;
    if (numPlayers > 0) {
        do {
            CarInit(&playerInfo[i], playerInfo[i].uaIndex, gInitPlayerWeapons[i]);
            if (rtIsSplitScreenOn()) {
                playerInfo[i].vrPos[1].vx = playerInfo[i].vrPos[5].vx;
                playerInfo[i].vrPos[1].vy = playerInfo[i].vrPos[5].vy;
                playerInfo[i].vrPos[1].vz = playerInfo[i].vrPos[5].vz;
                playerInfo[i].vrRot[1].vx = playerInfo[i].vrRot[5].vx;
                playerInfo[i].vrRot[1].vy = playerInfo[i].vrRot[5].vy;
                playerInfo[i].vrRot[1].vz = playerInfo[i].vrRot[5].vz;
            } else {
                SetCarVRMode(&playerInfo[i], playerInfo[i].unkF8);
                if (playerInfo[i].unkF4 == 4) {
                    InitHelicoptorPosition();
                }
            }
            t = i + 1;
            do {
            } while (0);
            n = numPlayers;
            i = t;
        } while (t < n);
    }
    i = 0;
    if (numCs > 0) {
        do {
            uaswCarHeadlightsOnOff(carName[i], gVar348);
            e = lightGetEnv(0);
            cs = carCs[i];
            do {
            } while (0);
            n = numCs;
            cs->env = e;
            t = i + 1;
            i = t;
        } while (t < n);
    }
    soundSetReverbOff();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitUALevel7);
#endif

void uaLeaveLevel(s32 level)
{
}

void UASetNumPlayers(s32 n)
{
    numPlayers = n;
}

#ifdef NON_MATCHING
void uaPickAICars(s16 level)
{
    s32 i;

    if ((u16)level < 7) {
        for (i = 0; i < gAICars; i++) {
            UASetAICar(numTargets++, gSelectedAICars[i]);
        }
        helicoptor.playerIdx = numTargets;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaPickAICars);
#endif

#ifdef NON_MATCHING
void UASetPlayerCar(s16 player, s32 car, s32 mode)
{
    s16 i;

    for (i = 0; i < numCs; i++) {
        if (carName[i] == car) {
            playerCar[player] = car;
            return;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UASetPlayerCar);
#endif

#ifdef NON_MATCHING
s16 UAGetPlayerCar(s16 player)
{
    s32 car = playerCar[player];
    return car;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAGetPlayerCar);
#endif

void UASetGodMode(s16 player, u8 on)
{
    playerInfo[player].stats.unk4C = (on == 0);
    gStandMode = on;
    if (on != 0) {
        return;
    }
    uaDriveAICars(1);
}

u8 uaDrivingAICars(void)
{
    return aiCarInfo[0].driving;
}

void uaDriveAICars(u8 on)
{
    s16 i;

    for (i = 0; i < 8; i++) {
        aiCarInfo[i].driving = on;
    }
}

void UASetInfiniteWeapons(s32 player, s32 on)
{
    playerInfo[(s16)player].weap.reset = (u8)on;
}

void UASetHelicoptorMode(s32 unused, u8 mode)
{
    if (gCurLevel == 1) {
        gHeliMode = mode;
    } else if (gCurLevel > 0) {
        if (gCurLevel < 8) {
            if (gCurLevel >= 6) {
                gHeliMode = mode;
            }
        }
    }
}

#ifdef NON_MATCHING
void InitPlayerCar(s16 slot, s32 car, u8 flag)
{
    s16 i;
    s16 idx;
    u8 found;
    s32 type;

    found = 0;
    idx = 0;
    for (i = 0; i < numCs; i++) {
        if (carName[i] == car) {
            found = 1;
            idx = i;
            break;
        }
    }
    if (found) {
        csSetDrawMode(carCs[idx], 0);
        type = carTypes[idx];
        SetCarCsInfo(slot, idx, car, 1);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitPlayerCar);
#endif

#ifdef NON_MATCHING
void SetCarCsInfo(s16 slot, s16 idx, s32 car, u8 found)
{
    VECTOR pos;
    VECTOR3 rot;
    s32 mode;

    playerCs[slot] = carCs[idx];
    playerIndexToCarIndex[slot] = idx;
    carIndexToPlayerOrAIIndex[idx] = slot;
    playerCar[slot] = car;
    playerInfo[slot].playerIdx = slot;
    if (found) {
        CarInit(&playerInfo[slot], carName[idx], gInitPlayerWeapons[slot]);
        SetCarVRMode(&playerInfo[slot], 2);
    } else {
        rot.vx = playerInfo[slot].motion.rot.vx;
        mode = playerInfo[slot].unkF4;
        rot.vy = playerInfo[slot].motion.rot.vy;
        rot.vz = playerInfo[slot].motion.rot.vz;
        pos.vx = playerInfo[slot].motion.pos.vx;
        pos.vy = playerInfo[slot].motion.pos.vy;
        pos.vz = playerInfo[slot].motion.pos.vz;
        CarInit(&playerInfo[slot], carName[idx], gInitPlayerWeapons[slot]);
        SetCarVRMode(&playerInfo[slot], mode);
        playerInfo[slot].motion.rot.vx = rot.vx;
        playerInfo[slot].motion.rot.vy = rot.vy;
        playerInfo[slot].motion.rot.vz = rot.vz;
        playerInfo[slot].motion.pos.vx = pos.vx;
        playerInfo[slot].motion.pos.vy = pos.vy;
        playerInfo[slot].motion.pos.vz = pos.vz;
    }
    carTypes[idx] = 0;
    carCs[idx]->unkC0 = slot + 1;
    playerInfo[slot].playerIdx = slot;
    csSetDrawMode(carCs[idx], 1);
    viewSetParent(camera[0], 0);
    if (camera[1] != NULL) {
        viewSetParent(camera[1], 1);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", SetCarCsInfo);
#endif

#ifdef NON_MATCHING
void UASetAICar(s16 slot, s32 car)
{
    s16 i;
    s16 idx;
    u8 found;
    s32 type;

    found = 0;
    idx = 0;
    for (i = 0; i < numCs; i++) {
        if (carName[i] == car) {
            found = 1;
            idx = i;
            break;
        }
    }
    if (found) {
        type = carTypes[idx];
        SetAICarCsInfo(slot, idx, car, found);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UASetAICar);
#endif

#ifdef NON_MATCHING
void SetAICarCsInfo(s16 slot, s16 idx, s32 car, u8 found)
{
    aiCarCs[slot] = carCs[idx];
    aiIndexToCarIndex[slot] = idx;
    carIndexToPlayerOrAIIndex[idx] = slot;
    aiCarInfo[slot].playerIdx = slot;
    AICarInit(&aiCarInfo[slot], carName[idx], 1);
    aiCarInfo[slot].playerIdx = slot;
    carTypes[idx] = 1;
    carCs[idx]->unkC0 = slot + 50;
    csSetDrawMode(carCs[idx], 1);
    AICarSetDefaultWeapons(&aiCarInfo[slot]);
    viewSetParent(camera[0], 0);
    if (camera[1] != NULL) {
        viewSetParent(camera[1], 1);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", SetAICarCsInfo);
#endif

#ifdef NON_MATCHING
void PlayGame(void)
{
    s32 d[4];
    s32 sky[4];
    s16 i;
    s16 t;
    s32 n;
    s16 best;
    s16 bestVal;

    playerBegTics = GetCurTics();
    for (i = 0; numPlayers > i; i++) {
        if (playerCs[i] == NULL) {
            break;
        }
        if (!CheckPlayerHealthStand(i)) {
            UAPlayerUpdate(playerCs[i], &playerInfo[i]);
        }
        if (i != 0) {
            if (playerInfo[i].stats.unk40 > 0) {
                d[0] = playerInfo[i].motion.pos.vx - playerInfo[0].motion.pos.vx;
                d[1] = playerInfo[i].motion.pos.vy - playerInfo[0].motion.pos.vy;
                d[2] = playerInfo[i].motion.pos.vz - playerInfo[0].motion.pos.vz;
                hudAddRadarSig(playerInfo[i].uaIndex, d, playerInfo[0].motion.rot.vz);
            }
        }
        UASetSoundFlags(playerIndexToCarIndex[i], &playerInfo[i], 1);
        UpdatePlayerDamageModel(i);
        if (playerCamFollow == i || twoPlayerMode) {
            UpdateCamera(i);
        }
        if (gVar350) {
            sky[0] = aiCarInfo[aiCamFollow].motion.pos.vx;
            sky[1] = aiCarInfo[aiCamFollow].motion.pos.vy;
        } else if (playerCamFollow == i || twoPlayerMode) {
            sky[0] = playerInfo[i].motion.pos.vx;
            sky[1] = playerInfo[i].motion.pos.vy;
        }
        sky[2] = 0;
        viewSetSkyPosition(sky, i);
    }
    playerEndTics = GetCurTics();
    aiBegTics = GetCurTics();
    if ((numTargets > 0 && GetNumAICarsInBattle() <= 0)
        || (numTargets >= 2 && GetNumAICarsInBattle() < 2)) {
        gVar370 += GetFieldsLastFrame();
        if (gVar370 > 180) {
            best = -1;
            bestVal = -1;
            for (i = 0; numTargets > i; i++) {
                if (aiCarInfo[i].stats.unk40 > 0) {
                    if (best < 0
                        || (aiCarInfo[i].unk01 != 0
                            && bestVal < aiCarInfo[i].route[aiCarInfo[i].unk02].prio)) {
                        best = i;
                        bestVal = aiCarInfo[i].route[aiCarInfo[i].unk02].prio;
                    }
                }
            }
            aiCarInfo[best].chosen = 1;
            aiCarInfo[best].unk01 = 0;
            aiCarInfo[best].routeVal = aiCarInfo[best].route[aiCarInfo[best].unk02].val;
            gVar370 = 0;
        }
    } else {
        gVar370 = 0;
        i = 0;
        if (numTargets > 0) {
            do {
                if (aiCarInfo[i].chosen) {
                    aiCarInfo[i].chosen = 0;
                }
                t = i + 1;
                do {
                } while (0);
                n = numTargets;
                i = t;
            } while (t < n);
        }
    }
    for (i = 0; numTargets > i; i++) {
        if (aiCarCs[i] == NULL) {
            break;
        }
        UAAIUpdate(aiCarCs[i], &aiCarInfo[i]);
        if (aiCarInfo[i].unk40) {
            UASetSoundFlags(aiIndexToCarIndex[i], &aiCarInfo[i], 0);
        }
        if (aiCarInfo[i].stats.unk40 > 0) {
            d[0] = aiCarInfo[i].motion.pos.vx - playerInfo[0].motion.pos.vx;
            d[1] = aiCarInfo[i].motion.pos.vy - playerInfo[0].motion.pos.vy;
            d[2] = aiCarInfo[i].motion.pos.vz - playerInfo[0].motion.pos.vz;
            hudAddRadarSig(aiCarInfo[i].uaIndex, d, playerInfo[0].motion.rot.vz);
        }
        UpdateAICarDamageModel(i);
    }
    if (!twoPlayerMode && !playerInfo[0].flags[0] && !beatThisLevel && !AnyAICarsAlive()) {
        beatThisLevel = 1;
        rtReturnToShell(3, 180);
    }
    UpdateNumAICarsInBattle();
    aiEndTics = GetCurTics();
    regeneratePickupWeapons();
    regen_healthstands();
    if (!twoPlayerMode) {
        switch (gCurLevel) {
        case 2:
        case 4:
        case 5:
            if (GetNumAICarsInBattle() == 0) {
                uasoundPlayHuntDA(gCurLevel);
            } else if (gBattleMusicOn) {
                uasoundPlayBattleDA(gCurLevel);
                gBattleMusicOn = 0;
            }
            break;
        }
    }
    if (gCurLevel >= 2) {
        move_targets();
    }
    soundProcessIds();
    UpdateNumPotHoles();
    UAStats();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", PlayGame);
#endif

u8 CheckPlayerHealthStand(s32 player)
{
    u8 ret;
    s32 h;

    ret = 0;
    if (playerInfo[player].flags[22] != 0) {
        if (health_stand_active(playerInfo[player].standId)) {
            ret = 1;
            uasoundPlayRecharge();
            animate_health_stand(playerInfo[player].standId);
            h = playerInfo[player].stats.unk40 + playerInfo[player].stats.healthRate;
            playerInfo[player].stats.unk40 = h;
            if (playerInfo[player].stats.unk44 < h) {
                playerInfo[player].stats.unk40 = playerInfo[player].stats.unk44;
                ret = 0;
            } else if (playerInfo[player].stats.healthCap < h) {
                playerInfo[player].stats.unk40 = playerInfo[player].stats.healthCap;
                ret = 0;
            }
            if (!ret) {
                deactivate_health_stand(playerInfo[player].standId);
            }
        }
    }
    return ret;
}

#ifdef NON_MATCHING
void UpdatePlayerDamageModel(s16 player)
{
    s32 pct;
    s32 mode;

    pct = (playerInfo[player].stats.unk40 * 100) / playerInfo[player].stats.unk44;
    if (playerInfo[player].stats.damageThreshold < pct) {
        if (playerInfo[player].stats.damageLevel != 0) {
            uaswSetCarState(playerInfo[player].uaIndex, 0, playerInfo[player].stats.unk05);
            uaswCarHeadlightsOnOff(playerInfo[player].uaIndex, gVar348);
            playerInfo[player].stats.damageLevel = 0;
            if (playerInfo[player].flags[22] == 0) {
                do_smoke(&playerInfo[player].motion.pos);
            }
        }
    } else if (pct >= 11) {
        if (playerInfo[player].stats.damageLevel != 1) {
            uaswSetCarState(playerInfo[player].uaIndex, 1, playerInfo[player].stats.unk05);
            playerInfo[player].stats.damageLevel = 1;
            uaswCarHeadlightsOnOff(playerInfo[player].uaIndex, gVar348);
            if (playerInfo[player].flags[22] == 0) {
                do_smoke(&playerInfo[player].motion.pos);
            }
        }
    } else {
        if (playerInfo[player].stats.damageLevel != 2) {
            uaswSetCarState(playerInfo[player].uaIndex, 2, playerInfo[player].stats.unk05);
            playerInfo[player].stats.damageLevel = 2;
            uaswCarHeadlightsOnOff(playerInfo[player].uaIndex, gVar348);
            if (playerInfo[player].flags[22] == 0) {
                do_smoke(&playerInfo[player].motion.pos);
            }
        }
        if (playerInfo[player].stats.unk40 <= 0) {
            if (playerInfo[player].flags[0] == 0) {
                uasoundStopCarSounds(playerInfo[player].uaIndex);
                playerInfo[player].flags[0] = 1;
                gInitPlayerWeapons[player] = 1;
                StartDeathSequence(&playerInfo[player], 1);
                if (twoPlayerMode != 0) {
                    gVar340 = 1;
                    if (player == 0) {
                        mode = 4;
                    } else {
                        mode = 3;
                    }
                } else {
                    mode = 1;
                }
                rtReturnToShell(mode, 180);
            }
        }
    }
    if (skipLevel != 0) {
        if (playerInfo[player].stats.unk4C == 0) {
            rtReturnToShell(3, 180);
            skipLevel = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UpdatePlayerDamageModel);
#endif

#ifdef NON_MATCHING
void UpdateAICarDamageModel(s16 ai)
{
    s32 pct;

    pct = (aiCarInfo[ai].stats.unk40 * 100) / aiCarInfo[ai].stats.unk44;
    if (aiCarInfo[ai].stats.damageThreshold < pct) {
        if (aiCarInfo[ai].stats.damageLevel != 0) {
            uaswSetCarState(aiCarInfo[ai].uaIndex, 0, aiCarInfo[ai].stats.unk05);
            aiCarInfo[ai].stats.damageLevel = 0;
            uaswCarHeadlightsOnOff(aiCarInfo[ai].uaIndex, gVar348);
        }
    } else if (pct >= 11) {
        if (aiCarInfo[ai].stats.damageLevel != 1) {
            uaswSetCarState(aiCarInfo[ai].uaIndex, 1, aiCarInfo[ai].stats.unk05);
            aiCarInfo[ai].stats.damageLevel = 1;
            uaswCarHeadlightsOnOff(aiCarInfo[ai].uaIndex, gVar348);
        }
    } else {
        if (aiCarInfo[ai].stats.damageLevel != 2) {
            uaswSetCarState(aiCarInfo[ai].uaIndex, 2, aiCarInfo[ai].stats.unk05);
            aiCarInfo[ai].stats.damageLevel = 2;
            uaswCarHeadlightsOnOff(aiCarInfo[ai].uaIndex, gVar348);
        }
        if (aiCarInfo[ai].stats.unk40 <= 0) {
            if (aiCarInfo[ai].flags[0] == 0) {
                aiCarInfo[ai].flags[0] = 1;
                StartDeathSequence(&aiCarInfo[ai], 0);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UpdateAICarDamageModel);
#endif

#ifdef NON_MATCHING
void StartDeathSequence(Car* car, u8 isPlayer)
{
    u8* flags;
    CarMotion* motion;
    CarStats* stats;
    VECTOR3 v;

    if (isPlayer) {
        PLAYER_CAR(car)->flags[21] = 1;
        motion = &PLAYER_CAR(car)->motion;
        stats = &PLAYER_CAR(car)->stats;
        if (beatThisLevel) {
            return;
        }
        flags = PLAYER_CAR(car)->flags;
    } else {
        motion = &AI_CAR(car)->motion;
        stats = &AI_CAR(car)->stats;
        flags = AI_CAR(car)->flags;
        if (AI_CAR(car)->unk2C > 0) {
            AI_CAR(car)->flags[21] = 0;
        } else {
            AI_CAR(car)->flags[21] = 1;
        }
    }
    v.vx = motion->pos.vx;
    v.vy = motion->pos.vy;
    v.vz = 0;
    do_smoke(&v);
    InitBombDamage(car, isPlayer, 1, 0);
    flags[20] = 1;
    stats->deathTimer = 120;
    flags[16] = 1;
    stats->unk18 = 300;
    stats->monster = 0;
    SetBounce(car, 0, 2, 0, isPlayer);
    do_bigger_explosion(&motion->pos);
    do_big_explosion(&motion->pos);
    flags[26] = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", StartDeathSequence);
#endif

#ifdef NON_MATCHING
u8 AnyAICarsAlive(void)
{
    s32 i;
    s32 pad[1];

    for (i = 0; i < numTargets; i++) {
        if (aiCarInfo[i].stats.unk40 > 0) {
            return 1;
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", AnyAICarsAlive);
#endif

#ifdef NON_MATCHING
void UpdateCamera(s16 idx)
{
    SVECTOR rot;
    AICar* h;
    Cs** tbl;

    if (gVar368) {
        viewSetDeltaTrans(&zeroTrans, idx);
        viewSetDeltaRot(&zeroRot, idx);
        return;
    }
    if (playerInfo[idx].unkF4 == 4) {
        h = &helicoptor;
        AICarHeliUpdate(h);
        helicoptor.motion.pos.vz
            = SmoothValue(helicoptor.motion.pos.vz, playerInfo[0].motion.pos.vz + 600, 95);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        UASetCameraPosition(0, &h->motion.pos, &rot);
        return;
    }
    if (playerInfo[idx].unkF4 == 1) {
        if (gVar350) {
            rot.vx = aiCarCs[aiCamFollow]->rot.vx;
            rot.vy = aiCarCs[aiCamFollow]->rot.vy;
            rot.vz = aiCarCs[aiCamFollow]->rot.vz;
        } else {
            rot.vx = playerCs[idx]->rot.vx;
            rot.vy = playerCs[idx]->rot.vy;
            rot.vz = playerCs[idx]->rot.vz;
        }
    } else {
        rot.vx = 0;
        rot.vy = 0;
        if (gVar350) {
            tbl = &aiCarCs[aiCamFollow];
        } else {
            tbl = &playerCs[idx];
        }
        rot.vz = (*tbl)->rot.vz;
    }
    if (gVar350) {
        UASetCameraPosition(0, &aiCarInfo[aiCamFollow].motion.pos, &rot);
        return;
    }
    UASetCameraPosition(idx, &playerInfo[idx].motion.pos, &rot);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UpdateCamera);
#endif

void UAStats(void)
{
}

#ifdef NON_MATCHING
void UAPlayerUpdate(Cs* cs, PlayerCar* car)
{
    CarUpdateControlPad(car);
    CheckVRMode(car);
    CarUpdate(car);
    cs->rot.vx = car->motion.rot.vx;
    cs->rot.vy = car->motion.rot.vy;
    cs->rot.vz = car->motion.rot.vz;
    cs->pos.vx = car->motion.pos.vx;
    cs->pos.vy = car->motion.pos.vy;
    cs->pos.vz = car->motion.pos.vz;
    wdCopy((s32*)&cs->mat, (s32*)&car->motion.mat2, 8);
    if (car->collision.unk16 >= 3) {
        car->motion.lastRot.vx = car->motion.rot.vx;
        car->motion.lastRot.vy = car->motion.rot.vy;
        car->motion.lastRot.vz = car->motion.rot.vz;
        car->motion.lastPos.vx = car->motion.pos.vx;
        car->motion.lastPos.vy = car->motion.pos.vy;
        car->motion.lastPos.vz = car->motion.pos.vz;
        car->motion.unkA8 = car->motion.rot2.vx;
        car->motion.unkAC = car->motion.rot2.vy;
        car->motion.unkB0 = car->motion.rot2.vz;
        wdCopy((s32*)&car->motion.lastMat, (s32*)&car->motion.mat2, 8);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAPlayerUpdate);
#endif

#ifdef NON_MATCHING
void UAAIUpdate(Cs* cs, AICar* car)
{
    AICarUpdate(car);
    cs->rot.vx = car->motion.rot.vx;
    cs->rot.vy = car->motion.rot.vy;
    cs->rot.vz = car->motion.rot.vz;
    cs->pos.vx = car->motion.pos.vx;
    cs->pos.vy = car->motion.pos.vy;
    cs->pos.vz = car->motion.pos.vz;
    wdCopy((s32*)&cs->mat, (s32*)&car->motion.mat2, 8);
    if (car->collision.unk16 >= 3) {
        car->motion.lastRot.vx = car->motion.rot.vx;
        car->motion.lastRot.vy = car->motion.rot.vy;
        car->motion.lastRot.vz = car->motion.rot.vz;
        car->motion.lastPos.vx = car->motion.pos.vx;
        car->motion.lastPos.vy = car->motion.pos.vy;
        car->motion.lastPos.vz = car->motion.pos.vz;
        car->motion.unkA8 = car->motion.rot2.vx;
        car->motion.unkAC = car->motion.rot2.vy;
        car->motion.unkB0 = car->motion.rot2.vz;
        wdCopy((s32*)&car->motion.lastMat, (s32*)&car->motion.mat2, 8);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAAIUpdate);
#endif

#ifdef NON_MATCHING
void UARemoveVehicleFromDrawList(s32 name)
{
    s32 i;

    for (i = 0; i < numCs; i++) {
        if (carName[i] == name) {
            carTypes[i] = 2;
            csSetDrawMode(carCs[i], 0);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UARemoveVehicleFromDrawList);
#endif

#ifdef NON_MATCHING
void CheckVRMode(PlayerCar* car)
{
    u16 id;
    s16 player;
    u32 oldMode;
    u32 mode;
    s32 ok;

    id = car->playerIdx;
    oldMode = car->unkF4;
    player = id;
    mode = oldMode;
    if (player != playerCamFollow) {
        return;
    }
    if (rtIsSplitScreenOn()) {
        return;
    }
    if (car->flags[11]) {
        return;
    }
    if (car->flags[0]) {
        return;
    }
    if (car->skid[18]) {
        if (gVrEnabled[player] != 0) {
            gVrEnabled[player] = 0;
            mode = oldMode + 1;
            if (gHeliMode && !gLevelFlag) {
                ok = mode < 5;
            } else {
                ok = mode < 4;
            }
            if (!ok) {
                mode = 1;
                if (car->skid[10] && car->weap.cur == 11) {
                    car->unk9A = 1;
                    car->stats.cheatTimer = 0;
                }
            }
            SetCarVRMode(car, mode);
        }
    } else if (car->skid[17]) {
        if (gVrEnabled[player] != 0) {
            gVrEnabled[player] = 0;
            mode = oldMode - 1;
            if (mode == 0) {
                if (gHeliMode && !gLevelFlag) {
                    mode = 4;
                    if (car->skid[10] && car->weap.cur == 11) {
                        car->unk9A = 0;
                        if (car->unk9B) {
                            gStandMode = 1;
                            car->unk9B = 0;
                        }
                    }
                } else {
                    mode = 3;
                    if (car->skid[10] && car->weap.cur == 11) {
                        car->unk9A = 0;
                        if (car->unk9B) {
                            gStandMode = 1;
                            car->unk9B = 0;
                        }
                    }
                }
            }
            SetCarVRMode(car, mode);
        }
    } else if (car->skid[20]) {
        if (gVrEnabled[player] != 0) {
            gVrEnabled[player] = 0;
            hudRadarToggle();
        }
    } else if (car->skid[23]) {
        if (gVrEnabled[player] != 0) {
            gVrEnabled[player] = 0;
            uadashToggleRemainingCarsList();
        }
    } else if (car->skid[19]) {
        if (gVrEnabled[player] != 0) {
            rtToggleRearView();
            gVrEnabled[player] = 0;
        }
    } else {
        gVrEnabled[player] = 1;
    }
    if (!gHeliMode) {
        if (ctlpadSpecial(1, id)) {
            gHeliMode = 1;
        }
    } else if (mode != oldMode) {
        if (mode == 4) {
            InitHelicoptorPosition();
            if (rtIsRearViewOn()) {
                rtToggleRearView();
            }
            rtSetRearView(0);
        } else {
            rtSetRearView(1);
        }
    }
    if (gCurLevel == 6) {
        if (playerInfo[0].unkC0 == 7) {
            gVar33C = 0;
        }
        if (!gVar33C) {
            if (playerInfo[0].unkC0 == 5) {
                gVar33C = 1;
                SetCarVRMode(&playerInfo[0], playerInfo[0].unkF8);
                if (playerInfo[0].unkF4 == 4) {
                    InitHelicoptorPosition();
                }
            } else if (playerInfo[0].unkF4 == 3 || playerInfo[0].unkF4 == 4) {
                SetCarVRMode(&playerInfo[0], 2);
                playerInfo[0].unkF8 = oldMode;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", CheckVRMode);
#endif

#ifdef NON_MATCHING
void InitHelicoptorPosition(void)
{
    VECTOR v;
    VECTOR out;
    s32 x;
    s32 y;
    s32 z;

    v.vx = playerInfo[0].vrPos[3].vx;
    v.vy = playerInfo[0].vrPos[3].vy;
    v.vz = playerInfo[0].vrPos[3].vz;
    mathMulTransVec(&playerInfo[0].motion.mat, &v, &out);
    x = playerInfo[0].motion.pos.vx + out.vx;
    y = playerInfo[0].motion.pos.vy + out.vy;
    z = playerInfo[0].motion.pos.vz + out.vz;
    helicoptor.unk01 = 0;
    helicoptor.heliFlag = 1;
    helicoptor.motion.pos.vx = x;
    helicoptor.motion.pos.vy = y;
    helicoptor.motion.pos.vz = z;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", InitHelicoptorPosition);
#endif

#ifdef NON_MATCHING
s16 GetClosestPlayer(VECTOR3* pos, s16 skip)
{
    s16 best;
    s16 i;
    s32 bestDist;
    s32 dist;
    s32 dx;
    s32 dy;
    s32 px;
    s32 py;
    s32 cx;
    s32 cy;

    best = 0;
    bestDist = 0x7FFF;
    for (i = 0; i < numPlayers; i++) {
        px = pos->vx;
        cx = playerInfo[i].motion.pos.vx;
        py = pos->vy;
        cy = playerInfo[i].motion.pos.vy;
        dx = px - cx;
        if (dx < 0) {
            dx = -dx;
        }
        dy = py - cy;
        if (dy < 0) {
            dy = -dy;
        }
        dist = dx + dy;
        if (dist < bestDist) {
            if (i != skip) {
                best = i;
                bestDist = dist;
            }
        }
    }
    return best;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetClosestPlayer);
#endif

#ifdef NON_MATCHING
s16 GetClosestAICar(VECTOR3* pos)
{
    s16 best;
    s16 i;
    s32 bestDist;
    s32 dist;
    s32 dx;
    s32 dy;
    s32 px;
    s32 py;
    s32 cx;
    s32 cy;

    best = 0;
    bestDist = 0x7FFF;
    for (i = 0; i < numTargets; i++) {
        if (aiCarInfo[i].stats.unk40 > 0) {
            px = pos->vx;
            cx = aiCarInfo[i].motion.pos.vx;
            py = pos->vy;
            cy = aiCarInfo[i].motion.pos.vy;
            dx = px - cx;
            if (dx < 0) {
                dx = -dx;
            }
            dy = py - cy;
            if (dy < 0) {
                dy = -dy;
            }
            dist = dx + dy;
            if (dist < bestDist) {
                best = i;
                bestDist = dist;
            }
        }
    }
    return best;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetClosestAICar);
#endif

#ifdef NON_MATCHING
void GetPlayerPosition(s16 player, VECTOR3* out)
{
    out->vx = playerInfo[player].motion.pos.vx;
    out->vy = playerInfo[player].motion.pos.vy;
    out->vz = playerInfo[player].motion.pos.vz;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetPlayerPosition);
#endif

#ifdef NON_MATCHING
void GetAICarPosition(s16 ai, VECTOR3* out)
{
    out->vx = aiCarInfo[ai].motion.pos.vx;
    out->vy = aiCarInfo[ai].motion.pos.vy;
    out->vz = aiCarInfo[ai].motion.pos.vz;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetAICarPosition);
#endif

#ifdef NON_MATCHING
void GetPlayerRot(s16 player, SVECTOR* out)
{
    out->vx = playerInfo[player].motion.rot.vx;
    out->vy = playerInfo[player].motion.rot.vy;
    out->vz = playerInfo[player].motion.rot.vz;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetPlayerRot);
#endif

#ifdef NON_MATCHING
Cs* GetPlayerCs3D(s16 player)
{
    Cs* cs = playerCs[player];
    return playerCs[player];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetPlayerCs3D);
#endif

#ifdef NON_MATCHING
Cs* GetAICs3D(s16 ai)
{
    if (numTargets > ai) {
        Cs* cs = aiCarCs[ai];
        return aiCarCs[ai];
    }
    return helicoptorCS;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetAICs3D);
#endif

s16 GetAITheCameraFollows(void)
{
    return aiCamFollow;
}

s32 GetPlayerSpeed(s16 player)
{
    s16 fields;
    s32 speed;

    fields = GetFieldsLastFrame();
    speed = playerInfo[player].motion.vel.vy;
    return ((speed / 32) * 100) / (fields * 19);
}

s32 GetAISpeed(s16 ai)
{
    s16 fields;
    s32 speed;

    fields = GetFieldsLastFrame();
    speed = aiCarInfo[ai].motion.vel.vy;
    return ((speed / 32) * 100) / (fields * 19);
}

s16 GetNumPlayers(void)
{
    return numPlayers;
}

PlayerCar* GetPlayerInfo(s16 player)
{
    return &playerInfo[player];
}

s16 GetNumAICars(void)
{
    return numTargets;
}

AICar* GetAICarInfo(s16 ai)
{
    return &aiCarInfo[ai];
}

#ifdef NON_MATCHING
Cs* UAGetCs(s32 name)
{
    s32 i;
    s32 pad[1];

    for (i = 0; i < numCs; i++) {
        if (carName[i] == name) {
            return carCs[i];
        }
    }
    if (name == -1) {
        return helicoptorCS;
    }
    return NULL;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAGetCs);
#endif

#ifdef NON_MATCHING
u8 uaIsCarCS(Cs* cs, u8* isPlayer, s16* index)
{
    s16 i;
    u8 found;

    found = 0;
    for (i = 0; i < numCs && !found; i++) {
        if (carCs[i] == cs) {
            if (carTypes[i] == 0) {
                *isPlayer = 1;
                found = 1;
                *index = carIndexToPlayerOrAIIndex[i];
            } else if (carTypes[i] == 1) {
                *isPlayer = 0;
                found = 1;
                *index = carIndexToPlayerOrAIIndex[i];
            }
        }
    }
    return found;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaIsCarCS);
#endif

#ifdef NON_MATCHING
s16 uaGetCarMatID(s16 idx, u8 isPlayer)
{
    u16* tbl;
    s16 k;

    if (isPlayer) {
        tbl = &playerIndexToCarIndex[idx];
    } else {
        tbl = (u16*)&aiIndexToCarIndex[idx];
    }
    k = *tbl;
    return carCs[k]->unkC0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaGetCarMatID);
#endif

#ifdef NON_MATCHING
s32 uaIsCarMatID(s16 matId, s8* isPlayer, u16* index)
{
    s16 i;
    u8 found;

    found = 0;
    for (i = 0; i < numCs && !found; i++) {
        if (carCs[i]->unkC0 == matId) {
            if (carTypes[i] == 0) {
                *isPlayer = 1;
                found = 1;
            } else if (carTypes[i] == 1) {
                *isPlayer = 0;
                found = 1;
            }
            *index = carIndexToPlayerOrAIIndex[i];
        }
    }
    return found;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaIsCarMatID);
#endif

#ifdef NON_MATCHING
void UASetCameraPosition(s16 which, VECTOR3* pos, SVECTOR* rot)
{
    SVECTOR sv;
    VECTOR v;
    VECTOR out;
    MATRIX mtx;
    s32 dx;
    s32 dy;
    s32 dz;

    if (gCurLevel == 6) {
        if (pos->vz < 6800) {
            if (playerInfo[which].unkF4 == 4) {
                camera[which]->rot.vx = rot->vx;
                camera[which]->rot.vy = rot->vy;
                camera[which]->rot.vz = rot->vz;
                camera[which]->pos.vx = pos->vx;
                camera[which]->pos.vy = pos->vy;
                if (camera[which]->pos.vz > 6800) {
                    camera[which]->pos.vz = pos->vz;
                }
            } else {
                if (playerInfo[which].unkF4 != 5) {
                    sv.vx = playerInfo[which].dRot.vx;
                    sv.vy = playerInfo[which].dRot.vy;
                    sv.vz = playerInfo[which].motion.rot.vz;
                    RotMatrixYXZ(&sv, &mtx);
                    v.vx = playerInfo[which].dTrans.vx;
                    v.vy = playerInfo[which].dTrans.vy;
                    v.vz = playerInfo[which].dTrans.vz;
                    mathMulTransVec(&mtx, &v, &out);
                    camera[which]->pos.vx = playerInfo[which].motion.pos.vx + out.vx;
                    camera[which]->pos.vy = playerInfo[which].motion.pos.vy + out.vy;
                    camera[which]->pos.vz = playerInfo[which].motion.pos.vz + out.vz;
                    SetCarVRMode(&playerInfo[which], 5);
                }
                dx = pos->vx - camera[which]->pos.vx;
                dy = pos->vy - camera[which]->pos.vy;
                dz = pos->vz - camera[which]->pos.vz;
                camera[which]->rot.vx = -ratan2(dz, SquareRoot0(dx * dx + dy * dy));
                camera[which]->rot.vy = 0;
                camera[which]->rot.vz = ratan2(dx, dy);
            }
        } else {
            camera[which]->pos.vx = pos->vx;
            camera[which]->pos.vy = pos->vy;
            camera[which]->pos.vz = pos->vz;
            camera[which]->rot.vx = rot->vx;
            camera[which]->rot.vy = rot->vy;
            camera[which]->rot.vz = rot->vz;
            if (playerInfo[which].unkF4 == 5) {
                SetCarVRMode(&playerInfo[which], 2);
            }
        }
    } else {
        camera[which]->pos.vx = pos->vx;
        camera[which]->pos.vy = pos->vy;
        camera[which]->pos.vz = pos->vz;
        camera[which]->rot.vx = rot->vx;
        camera[which]->rot.vy = rot->vy;
        camera[which]->rot.vz = rot->vz;
    }
    RotMatrixYXZ(&camera[which]->rot, &camera[which]->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UASetCameraPosition);
#endif

#ifdef NON_MATCHING
void UAGetCameraPosition(s16 idx, VECTOR3* pos, SVECTOR* rot)
{
    pos->vx = camera[idx]->pos.vx;
    pos->vy = camera[idx]->pos.vy;
    pos->vz = camera[idx]->pos.vz;
    rot->vx = camera[idx]->rot.vx;
    rot->vy = camera[idx]->rot.vy;
    rot->vz = camera[idx]->rot.vz;
    RotMatrixYXZ(&camera[idx]->rot, &camera[idx]->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAGetCameraPosition);
#endif

#ifdef NON_MATCHING
Cs* UAGetCameraCS(s16 idx)
{
    return camera[idx];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UAGetCameraCS);
#endif

u8 uaUsingLanes(void)
{
    return gLevelFlag;
}

u8 uaUsingGroupGroundHeight(void)
{
    return gVar34C;
}

void uaInitTweeking(void)
{
}

#ifdef NON_MATCHING
void uaTweekDifficultyFnc(void)
{
    s16 i;
    s32 strength;
    s16 t;
    s32 n;

    switch (difficulty) {
    default:
        strength = 100;
        break;
    case 0:
        strength = 60;
        break;
    case 1:
        strength = 100;
        break;
    case 2:
        strength = 120;
        break;
    }
    i = 0;
    if (numTargets > 0) {
        do {
            CarInitStrength(&aiCarInfo[i].stats, strength);
            t = i + 1;
            do {
            } while (0);
            n = numTargets;
            i = t;
        } while (t < n);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaTweekDifficultyFnc);
#endif

void uaSetDifficulty(s32 d)
{
    difficulty = d;
    uaTweekDifficultyFnc();
    carSetPowerupDelaysBySkillLevel(d);
}

s32 uaGetDifficulty(void)
{
    return difficulty;
}

s32 uaGetTwoPlayerMode(void)
{
    return twoPlayerMode;
}

#ifdef NON_MATCHING
void PadSetConfig(s32 cfg, s16 player)
{
    PlayerCar* info = GetPlayerInfo(player);
    padConfig[player] = cfg;
    uaPadInit(info, cfg);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", PadSetConfig);
#endif

#ifdef NON_MATCHING
s32 PadGetConfig(s16 player)
{
    GetPlayerInfo(player);
    return padConfig[player];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", PadGetConfig);
#endif

#ifdef NON_MATCHING
void uaPadInit(PlayerCar* car, s32 mode)
{
    do {
        car->skid[40] = 12;
    } while (0);
    switch (mode) {
    case 0:
        do {
            car->skid[22] = 0;
        } while (0);
        do {
            car->skid[24] = 5;
        } while (0);
        do {
            car->skid[26] = 6;
        } while (0);
        do {
            car->skid[28] = 8;
        } while (0);
        do {
            car->skid[27] = 7;
        } while (0);
        do {
            car->skid[38] = 2;
        } while (0);
        do {
            car->skid[39] = 1;
        } while (0);
        do {
            car->skid[29] = 12;
        } while (0);
        do {
            car->skid[34] = 13;
        } while (0);
        do {
            car->skid[35] = 14;
        } while (0);
        do {
            car->skid[25] = 4;
        } while (0);
        do {
            car->skid[33] = 11;
        } while (0);
        do {
            car->skid[36] = 9;
        } while (0);
        do {
            car->skid[37] = 3;
        } while (0);
        do {
            car->skid[31] = 10;
        } while (0);
        do {
            car->skid[32] = 10;
        } while (0);
        break;
    case 1:
    default:
        do {
            car->skid[22] = 1;
        } while (0);
        do {
            car->skid[24] = 5;
        } while (0);
        do {
            car->skid[26] = 6;
        } while (0);
        do {
            car->skid[28] = 8;
        } while (0);
        do {
            car->skid[27] = 7;
        } while (0);
        do {
            car->skid[38] = 2;
        } while (0);
        do {
            car->skid[39] = 1;
        } while (0);
        do {
            car->skid[29] = 13;
        } while (0);
        do {
            car->skid[25] = 14;
        } while (0);
        do {
            car->skid[30] = 0;
        } while (0);
        do {
            car->skid[34] = 10;
        } while (0);
        do {
            car->skid[35] = 4;
        } while (0);
        do {
            car->skid[36] = 3;
        } while (0);
        do {
            car->skid[37] = 9;
        } while (0);
        do {
            car->skid[33] = 11;
        } while (0);
        do {
            car->skid[31] = 12;
        } while (0);
        do {
            car->skid[32] = 12;
        } while (0);
        break;
    case 2:
        do {
            car->skid[22] = 2;
        } while (0);
        do {
            car->skid[24] = 5;
        } while (0);
        do {
            car->skid[26] = 6;
        } while (0);
        do {
            car->skid[28] = 8;
        } while (0);
        do {
            car->skid[27] = 7;
        } while (0);
        do {
            car->skid[38] = 2;
        } while (0);
        do {
            car->skid[39] = 1;
        } while (0);
        do {
            car->skid[29] = 12;
        } while (0);
        do {
            car->skid[34] = 14;
        } while (0);
        do {
            car->skid[35] = 13;
        } while (0);
        do {
            car->skid[25] = 4;
        } while (0);
        do {
            car->skid[33] = 11;
        } while (0);
        do {
            car->skid[36] = 9;
        } while (0);
        do {
            car->skid[37] = 3;
        } while (0);
        do {
            car->skid[31] = 10;
        } while (0);
        do {
            car->skid[32] = 10;
        } while (0);
        break;
    case 3:
        do {
            car->skid[22] = 3;
        } while (0);
        do {
            car->skid[24] = 5;
        } while (0);
        do {
            car->skid[26] = 6;
        } while (0);
        do {
            car->skid[28] = 8;
        } while (0);
        do {
            car->skid[27] = 7;
        } while (0);
        do {
            car->skid[38] = 2;
        } while (0);
        do {
            car->skid[39] = 1;
        } while (0);
        do {
            car->skid[29] = 10;
        } while (0);
        do {
            car->skid[25] = 9;
        } while (0);
        do {
            car->skid[30] = 0;
        } while (0);
        do {
            car->skid[34] = 13;
        } while (0);
        do {
            car->skid[35] = 12;
        } while (0);
        do {
            car->skid[36] = 11;
        } while (0);
        do {
            car->skid[37] = 14;
        } while (0);
        do {
            car->skid[33] = 3;
        } while (0);
        do {
            car->skid[31] = 4;
        } while (0);
        do {
            car->skid[32] = 4;
        } while (0);
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaPadInit);
#endif

#ifdef NON_MATCHING
s16 GetPlayerCarToCarIndex(s16 player)
{
    return playerIndexToCarIndex[player];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetPlayerCarToCarIndex);
#endif

#ifdef NON_MATCHING
s16 GetAICarToCarIndex(s16 ai)
{
    return aiIndexToCarIndex[ai];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetAICarToCarIndex);
#endif

s32 GetAICarStrength(void)
{
    s32 r;

    switch (difficulty) {
    case 0:
    default:
        r = 60;
        break;
    case 1:
        r = 100;
        break;
    case 2:
        r = 120;
        break;
    }
    return r;
}

#ifdef NON_MATCHING
void uaDontDrawCar(s16 idx, u8 isPlayer)
{
    Cs* cs;

    if (isPlayer) {
        cs = playerCs[idx];
    } else {
        cs = aiCarCs[idx];
    }
    ((DrawNode*)cs->epNode)->drawFlag = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaDontDrawCar);
#endif

#ifdef NON_MATCHING
void uaDrawCar(s16 idx, u8 isPlayer)
{
    Cs* cs;
    u16 v;

    if (isPlayer) {
        cs = playerCs[idx];
        v = playerInfo[idx].stats.colorId;
    } else {
        cs = aiCarCs[idx];
        v = aiCarInfo[idx].stats.colorId;
    }
    if (v != 0) {
        ((DrawNode*)cs->epNode)->drawFlag = v;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaDrawCar);
#endif

s32 uaGetCurrentCar(s32 isPlayer, s16 idx)
{
    s32 name;

    if (isPlayer) {
        name = GetPlayerInfo(idx)->uaIndex;
    } else {
        name = GetAICarInfo(idx)->uaIndex;
    }

    switch (name) {
    case 110:
        return 1;
    case 20:
        return 2;
    case 90:
        return 3;
    case 70:
        return 4;
    case 10:
        return 5;
    case 100:
        return 6;
    case 60:
        return 7;
    case 120:
        return 8;
    case 40:
        return 9;
    case 50:
        return 10;
    case 30:
        return 11;
    }
    return 0;
}

s16 GetPlayerTheCameraFollows(void)
{
    return playerCamFollow;
}

#ifdef NON_MATCHING
void UASetSoundFlags(s16 idx, Car* car, u8 isPlayer)
{
    u8* flags;
    CarMotion* motion;
    CarStats* stats;
    CarCollision* collision;
    s32 name;
    s32 range;
    s32 xpos;
    s32 rev;

    if (isPlayer) {
        flags = PLAYER_CAR(car)->flags;
        motion = &PLAYER_CAR(car)->motion;
        stats = &PLAYER_CAR(car)->stats;
        collision = &PLAYER_CAR(car)->collision;
        name = GetPlayerInfo((s16)PLAYER_CAR(car)->playerIdx)->uaIndex;
    } else {
        flags = AI_CAR(car)->flags;
        motion = &AI_CAR(car)->motion;
        stats = &AI_CAR(car)->stats;
        collision = &AI_CAR(car)->collision;
        name = GetAICarInfo((s16)AI_CAR(car)->playerIdx)->uaIndex;
    }
    soundSetRangeAndXPositionFromWorldLoc(&motion->pos.vx);
    if (twoPlayerMode) {
        range = 0;
        xpos = 0;
    } else {
        range = soundGetCalculatedSoundRange();
        xpos = soundGetCalculatedSoundXPosition();
    }
    if (flags[2]) {
        uasoundPlayCarSkid(name, range, xpos, 99);
    }
    if (flags[26]) {
        uasoundPlayCarExplode(name, range, xpos, 99);
        flags[26] = 0;
    }
    if (flags[13] != 0 && flags[4] != 0) {
        rev = 99;
    } else {
        s32 spd = motion->vel.vy;
        s32 den = stats->unkA8;
        s32 gear = flags[4];

        rev = (__builtin_abs(spd) * 49) / den;
        if (gear) {
            rev += 49;
        }
    }
    uasoundPlayCarEngineRev(name, range, xpos, rev);
    if (collision->unk12) {
        uasoundPlayCarCrash(name, range, xpos, collision->unk12);
    }
    if (flags[24]) {
        uasoundPlayCarSignature(name, range, xpos);
        flags[24] = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", UASetSoundFlags);
#endif

#ifdef NON_MATCHING
s16 GetNumAICarsLiving(void)
{
    s32 i;
    s16 n;
    s32 pad[1];

    n = 0;
    for (i = 0; i < numTargets; i++) {
        if (aiCarInfo[i].stats.unk40 > 0) {
            n++;
        }
    }
    return n;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetNumAICarsLiving);
#endif

s16 GetCarNumFromCarName(s32 name)
{
    s32 i;

    for (i = 0; i < 12; i++) {
        if (name == CarNumToCarName[i]) {
            break;
        }
    }
    return i;
}

#ifdef NON_MATCHING
s32 GetCarNameFromCarNum(s16 num)
{
    return CarNumToCarName[num];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", GetCarNameFromCarNum);
#endif

#ifdef NON_MATCHING
void uaSelectOpponents(s32 car, s16 level, s16* outCount, s32* outCars)
{
    s32 carNum;
    s32 remaining;
    s32 n;
    s16 bits;
    s16 me;
    s32 r;
    s32* p;

    bits = 0;
    n = 0;
    if ((u16)level < 7) {
        for (carNum = 0; carNum < 12; carNum++) {
            if (car == CarNumToCarName[carNum]) {
                break;
            }
        }
        gAICars = 0;
        remaining = 1;
        if (level == 6) {
            remaining = 5;
        }
        if (level == 0 || level == 6) {
            me = carNum;
            remaining--;
            while (remaining != -1) {
                if (gVar3A8 >= 11 || ((gVar3A4 >> me) & 1) != 0) {
                    gVar3A4 = 0;
                    gVar3A8 = 0;
                }
                do {
                    r = rand() % 12;
                } while (((gVar3A4 >> r) & 1) != 0 || r == me);
                gSelectedAICars[gAICars] = CarNumToCarName[r];
                gAICars++;
                gVar3A4 |= 1 << r;
                gVar3A8++;
                outCars[n] = CarNumToCarName[r];
                n++;
                remaining--;
            }
        } else {
            if (OpponentBitMasks[level] != NULL) {
                bits = OpponentBitMasks[level][(s16)carNum];
            }
            gAICars = 0;
            if (bits != 0) {
                p = CarNumToCarName;
                while (bits != 0) {
                    if (bits & 1) {
                        gSelectedAICars[gAICars] = *p;
                        gAICars++;
                        outCars[n] = *p;
                        n++;
                    }
                    bits >>= 1;
                    p++;
                }
            }
        }
        *outCount = n;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaSelectOpponents);
#endif

#ifdef NON_MATCHING
char* uaGetCarNameString(s32 name)
{
    return gCarNameStrings[GetCarNumFromCarName(name)];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaGetCarNameString);
#endif

u8 uaHasPlayerBeatThisLevel(void)
{
    return (u8)beatThisLevel;
}

u8 uaPlayerCheating(void)
{
    return gStandMode;
}

#ifdef NON_MATCHING
void uaInitBossCar(void)
{
    s32 z;

    numTargets = 4;
    UASetAICar(3, 130);
    beatThisLevel = 0;
    if (playerInfo[0].unkC0 < 6) {
        aiCarInfo[3].stats.triggerPt = 28;
        InitTriggerPoint(&aiCarInfo[3]);
        z = 8000;
    } else {
        aiCarInfo[3].stats.triggerPt = 4;
        InitTriggerPoint(&aiCarInfo[3]);
        z = 7760;
    }
    aiCarInfo[3].motion.pos.vz = z;
    playerInfo[0].motion.vel.vx = 0;
    playerInfo[0].motion.vel.vy = 0;
    playerInfo[0].motion.vel.vz = 0;
    playerInfo[0].motion.rotDelta.vx = 0;
    playerInfo[0].motion.rotDelta.vy = 0;
    playerInfo[0].motion.rotDelta.vz = 0;
    if (playerInfo[0].motion.pos.vz < 6400) {
        CarInit(&playerInfo[0], playerInfo[0].uaIndex, 0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua", uaInitBossCar);
#endif
