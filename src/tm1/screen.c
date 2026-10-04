#include "common.h"
#include <libetc.h>
#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <rand.h>
#include <stdio.h>
#include <strings.h>

#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/ctlpad.h"
#include "tm1/fileio.h"
#include "tm1/font.h"
#include "tm1/hier.h"
#include "tm1/light.h"
#include "tm1/long_vector.h"
#include "tm1/movie.h"
#include "tm1/rt.h"
#include "tm1/screen.h"
#include "tm1/shell.h"
#include "tm1/smooth.h"
#include "tm1/sound.h"
#include "tm1/ua.h"
#include "tm1/ua_dash.h"
#include "tm1/ua_sound.h"
#include "tm1/view.h"

#define TITLE_IMAGE ((u_long*)0x80041CE0)
#define OPTIONS_IMAGE ((u_long*)0x80068228)
#define VEHICLE_IMAGE ((u_long*)0x8008F328)
#define AUDIO_IMAGE ((u_long*)0x800B7B98)
#define TEXT_BUFFER ((char*)0x800BBB98)

typedef struct ScreenCinema {
    /* 0x0 */ char* file;
    /* 0x4 */ s32 frames;
    /* 0x8 */ s32 rgb24;
} ScreenCinema; /* 0xC */

typedef struct ScreenArrow {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ char* label;
} ScreenArrow; /* 0x8 */

typedef struct ScreenVehicleFace {
    /* 0x00 */ POLY_FT4 poly;
    /* 0x28 */ s32 x;
    /* 0x2C */ s32 y;
} ScreenVehicleFace; /* 0x30 */

extern void SetBlockFill(BLK_FILL* p);
extern void CarInitDeltas(CarStats* stats, s32 name);

extern s32 gTitleChoice;
extern s32 gOptionsChoice;
extern s32 gMainOptionsChoice;
extern s16 gFrameNum;
extern s32 ua_music_vol;
extern s32 ua_sfx_vol;
extern s32 gDifficulty;
extern s32 gCursorOn;
extern s32 gCursorCount;
extern RECT gCursorRect;

extern u8* gFaceObject;
extern s16 InLeft;
extern s32 gHeading;
extern s16 screen_whichOT;
extern u_long* screen_ot;
extern s32 whichLevel;
extern s16 x0;
extern s16 y0;
extern s16 angle;
extern char gPassword[6];
extern SVECTOR gEyeRot;
extern SVECTOR gDeltaRot;
extern s32 gStartLocation;

extern POLY_FT4* Face;

extern char* screenCarPictures[12];
extern ScreenArrow screenVehicleArrows[2];
extern ScreenArrow screenControlArrows[2];
extern ScreenCinema screenCinemas[4];
extern char* gTransitionLabels[14];
extern char* screenControlPadLabels[4];
extern char* screenVolumeLabels[15];
extern char* screenDifficultyLabels[3];
extern u16 screenNumCards[8];
extern s16 D_80170F44[7][8];
extern s16 D_80170FB4[7][8];
extern s16 D_80171024[8];
extern s16 D_80171034[8];
extern char* screenPasswords[15];

extern DRAWENV gScreenDrawEnv;
extern DISPENV gScreenDisplayEnv;
extern POLY_FT4 gCards[8];
extern DISPENV displays[2];
extern POLY_FT4 screen_sw;
extern u_long screen_OT[2][2];
extern u_long gCursorImageBuf[48];
extern LVECTOR gEyePoint;
extern LVECTOR gDeltaTrans;
extern DISPENV gCardsDisplayEnv[2];
extern DRAWENV gCardsDrawEnv[2];

#ifdef NON_MATCHING
s32 atoi(char* str)
{
    unsigned char* p;
    s32 val;

    p = (unsigned char*)str;
    val = 0;
    while (*p >= '0') {
        if (*p > '9') {
            break;
        }
        val = *p++ + (val * 10 - '0');
    }
    return val;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", atoi);
#endif

void screenSetEnv(void)
{
    PutDispEnv(&gScreenDisplayEnv);
    PutDrawEnv(&gScreenDrawEnv);
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB7A8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB904);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB910);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB91C);

#ifdef NON_MATCHING
s32 screenDisplayTitle(void)
{
    s32 ret = 1;
    char* choices[3] = { "1P CONTEST", "2P BATTLE", "OPTIONS" };
    DRAWENV env;
    s32 frames;
    s32 prev;
    s32 cur;
    u_long* image;

    image = TITLE_IMAGE;
    screenSetEnv();
    VSync(0);
    screenDisplayImage(1, image, 0, 0);
    cur = gTitleChoice;
    screenDisplayChoicesFromBottom(2, 3, -1, cur, choices, 0, 195, 20);
    while (ctlpadAnyKey(0)) {
        UpdCtlPad();
    }
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prev = gTitleChoice;
        if (ctlpadAnyUp(0)) {
            frames = 0;
            if (gTitleChoice == 0) {
                gTitleChoice = 2;
            } else {
                gTitleChoice = gTitleChoice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(0)) {
            frames = 0;
            if (gTitleChoice == 2) {
                gTitleChoice = 0;
            } else {
                gTitleChoice = gTitleChoice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnySelect(0)) {
            uasoundPlayMenuSelectFlush();
            switch (gTitleChoice) {
            case 0:
                ret = 7;
                goto done;
            case 1:
                ret = 8;
                goto done;
            case 2:
                ret = 0x1D;
                goto done;
            case 3:
                ret = 0x15;
                goto done;
            case 4:
                ret = 0xF;
                goto done;
            }
            goto done;
        } else if (frames >= 1201) {
            ret = 2;
            goto done;
        }
        if (gTitleChoice != prev) {
            screenDisplayImage(0, image, 0, 0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            cur = gTitleChoice;
            screenDisplayChoicesFromBottom(2, 3, prev, cur, choices, 0, 195, 20);
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
        while (ctlpadAnyKey(2)) {
            UpdCtlPad();
        }
    }
done:
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayTitle);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB928);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB934);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB948);

#ifdef NON_MATCHING
s32 screenBattleOptions(s32 twoPlayer)
{
    s32 ret = 1;
    char* names[3] = { "START", "OPTIONS", "EXIT" };
    DRAWENV env;
    s32 choice = 0;
    s32 pad;
    s32 frames;
    s32 prev;
    u_long* buf;
    s32 car0;
    s32 car1;

    buf = (u_long*)rtGetDoubleBufferArea();
    pad = (twoPlayer != 0) * 2;
    VSync(0);
    screenSetEnv();
    car0 = UAGetPlayerCar(0);
    if (twoPlayer) {
        car1 = UAGetPlayerCar(1);
        fileioLoadFileIntoRam(buf, "UASCREEN\\SCRNE2.TIM");
        screenLoadCardsForBattleOptions(twoPlayer, car0, car1);
    } else {
        fileioLoadFileIntoRam(buf, "UASCREEN\\SCRNE21P.TIM");
        screenLoadCardsForBattleOptions(twoPlayer, car0, -1);
    }
    screenDisplayImage(1, buf, 0, 0);
    screenDrawCardsForBattleOptions(twoPlayer);
    screenDisplayChoicesFromBottom(0, 3, -1, choice, names, 0, 210, 20);
    while (ctlpadAnyKey(pad)) {
        UpdCtlPad();
    }
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prev = choice;
        if (ctlpadAnyUp(pad)) {
            frames = 0;
            if (choice == 0) {
                choice = 2;
            } else {
                choice = choice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(pad)) {
            frames = 0;
            if (choice == 2) {
                choice = 0;
            } else {
                choice = choice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnySelect(pad)) {
            uasoundPlayMenuSelectFlush();
            switch (choice) {
            case 0:
                ret = 10;
                goto out;
            case 1:
                ret = 6;
                goto out;
            case 2:
                ret = 1;
                goto out;
            }
            goto out;
        } else if (frames >= 1201) {
            ret = 1;
            goto out;
        }
        if (choice != prev) {
            screenDisplayImage(0, buf, 0, 0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            screenDrawCardsForBattleOptions(twoPlayer);
            screenDisplayChoicesFromBottom(0, 3, -1, choice, names, 0, 210, 20);
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
        while (ctlpadAnyKey(pad)) {
            UpdCtlPad();
        }
    }
out:
    if (ret == 10) {
        while (ctlpadAnyKey(pad)) {
            UpdCtlPad();
        }
    }
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenBattleOptions);
#endif

#ifdef NON_MATCHING
void screenDisplayImage(s32 rightHalf, u_long* data, s32 x, s32 y)
{
    static GsIMAGE tim;
    static RECT theRect;
    s16 w;
    s16 h;

    DrawSync(0);
    GsGetTimInfo(data + 1, &tim);
    w = tim.pw;
    h = tim.ph;
    theRect.x = x;
    theRect.w = w;
    theRect.h = h;
    if (rightHalf != 0) {
        theRect.x = x + 320;
    }
    theRect.y = y;
    LoadImage(&theRect, tim.pixel);
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayImage);
#endif

#ifdef NON_MATCHING
void screenDisplayToDisplay(s32 toRight, s32 unused)
{
    static RECT src;
    s32 x;
    s32 y;

    if (toRight == 0) {
        x = 320;
        y = 0;
        src.x = 0;
        src.y = 0;
    } else {
        x = 0;
        y = 0;
        src.x = 320;
        src.y = 0;
    }
    src.w = 320;
    src.h = 240;
    MoveImage(&src, x, y);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayToDisplay);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB960);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB96C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB980);

#ifdef NON_MATCHING
s32 screenAudioOptions(s32 noLoad, s32 hasTracks)
{
    s32 choice = 0;
    s32 prev = -1;
    char* names[5] = { "MUSIC:", "SFX:", "PLAY TRACK:", "", "EXIT" };
    char* names2[5];
    s32 fonts[2];
    DRAWENV env;
    s32 pad;
    s32 frames;
    char* title;
    u_long* buf;
    s32 dir;
    s32 dir2;

    buf = AUDIO_IMAGE;
    title = "";
    fonts[0] = 0;
    fonts[1] = 1;
    pad = (rtIsSplitScreenOn() != 0) * 2;
    VSync(0);
    DrawSync(0);
    screenSetEnv();
    while (ctlpadAnyKey(pad)) {
        UpdCtlPad();
    }
    shellSetSoundtrackChoice(soundGetCurrentDATrack());
    if (hasTracks) {
        title = shellGetSoundtrackTitle();
    }
    names2[0] = screenGetMusicLabel();
    names2[1] = screenGetEffectsLabel();
    names2[2] = 0;
    names2[3] = 0;
    names2[4] = 0;
    if (noLoad == 0) {
        buf = (u_long*)rtGetDoubleBufferArea();
        fileioLoadFileIntoRam(buf, "UASCREEN\\SCRNFC1.TIM");
    }
    if (hasTracks == 0) {
        names[2] = "";
    }
    soundStartPlayDA(soundGetCurrentDATrack());
    screenDisplayImage(1, buf, 0, 0);
    screenDisplayChoicesFromBottom(0, 5, prev, choice, names, names2, 200, 20);
    fontPrintCenteredXY(*(choice == 2 ? &fonts[1] : &fonts[0]), 159, 175, title);
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prev = choice;
        if (ctlpadAnyUp(pad)) {
            frames = 0;
            if (choice == 0) {
                choice = 4;
            } else {
                choice = choice - 1;
            }
            if (hasTracks == 0) {
                if (choice == 3) {
                    choice = 1;
                }
            }
            if (choice == 3) {
                choice = 2;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(pad)) {
            frames = 0;
            if (choice == 4) {
                choice = 0;
            } else {
                choice = choice + 1;
            }
            if (hasTracks == 0) {
                if (choice == 2) {
                    choice = 4;
                }
            }
            if (choice == 3) {
                choice = 4;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyLeft(pad) || ctlpadAnyRight(pad)) {
            frames = 0;
            switch (choice) {
            case 0:
                if (ctlpadAnyLeft(pad)) {
                    dir = -1;
                } else {
                    dir = 1;
                }
                screenNextMusicChoice(dir);
                prev = -1;
                screenSetMusicVolume();
                screenNextEffectsChoice(-dir);
                screenSetEffectsVolume();
                names2[0] = screenGetMusicLabel();
                names2[1] = screenGetEffectsLabel();
                uasoundPlayMenuCycleFlush();
                break;
            case 1:
                if (ctlpadAnyLeft(pad)) {
                    dir = -1;
                } else {
                    dir = 1;
                }
                screenNextEffectsChoice(dir);
                prev = -1;
                screenSetEffectsVolume();
                screenNextMusicChoice(-dir);
                screenSetMusicVolume();
                names2[1] = screenGetEffectsLabel();
                names2[0] = screenGetMusicLabel();
                uasoundPlayMenuCycleFlush();
                break;
            case 2:
                if (hasTracks == 0) {
                    break;
                }
                if (ctlpadAnyLeft(pad)) {
                    dir2 = -1;
                } else {
                    dir2 = 1;
                }
                prev = -1;
                shellNextSoundtrackChoice(dir2);
                title = shellGetSoundtrackTitle();
                soundStartPlayDA(shellGetSoundtrackId());
                uasoundPlayMenuCycleFlush();
                break;
            }
        } else if (ctlpadAnySelect(pad)) {
            uasoundPlayMenuSelectFlush();
            if (choice == 4) {
                return 4;
            }
        } else if (frames >= 18001) {

            return;
        }
        if (prev != choice) {
            VSync(0);
            screenDisplayImage(0, buf, 0, 0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            screenDisplayChoicesFromBottom(0, 5, prev, choice, names, names2, 200, 20);
            fontPrintCenteredXY(*(choice == 2 ? &fonts[1] : &fonts[0]), 159, 175, title);
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
        while (ctlpadAnyKey(pad)) {
            UpdCtlPad();
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenAudioOptions);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB998);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9A8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9B4);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9C0);

#ifdef NON_MATCHING
s32 screenOptions(s32 twoPlayer)
{
    static s32 prevChoice;
    char* names[4];
    char* names2[4];
    s32 frames;
    s32 pad;
    s32 minChoice;
    s32 dir;
    s32 ret = 20;

    minChoice = 0;
    names[3] = "EXIT";
    pad = (twoPlayer != 0) * 2;
    names2[3] = 0;
    if (twoPlayer) {
        minChoice = 1;
        if (gOptionsChoice == 0) {
            gOptionsChoice = minChoice;
        }
        names[0] = "";
        names2[0] = "";
        names[2] = "STAGE:";
        names2[2] = shellGetTwoPlayerCurrentLevelName();
        names[1] = "SELECT CONTROLS";
        names2[1] = "";
    } else {
        names[0] = "DIFFICULTY:";
        names2[0] = screenGetDifficultyLabel();
        names[2] = "PASSWORD";
        names2[2] = "";
        names[1] = "CONTROLS:";
        names2[1] = screenGetControlPadLabel(PadGetConfig(0));
    }
    VSync(0);
    screenSetEnv();
    screenDisplayImage(1, OPTIONS_IMAGE, 0, 0);
    screenDisplayChoicesFromBottom(0, 4, -1, gOptionsChoice, names, names2, 180, 20);
    while (ctlpadAnyKey(pad)) {
        UpdCtlPad();
    }
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prevChoice = gOptionsChoice;
        if (ctlpadAnyUp(pad)) {
            frames = 0;
            if (gOptionsChoice == minChoice) {
                gOptionsChoice = 3;
            } else {
                gOptionsChoice = gOptionsChoice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(pad)) {
            frames = 0;
            if (gOptionsChoice == 3) {
                gOptionsChoice = minChoice;
            } else {
                gOptionsChoice = gOptionsChoice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyLeft(pad) || ctlpadAnyRight(pad)) {
            frames = 0;
            switch (gOptionsChoice) {
            case 0:
                uasoundPlayMenuCycleFlush();
                if (ctlpadAnyLeft(pad)) {
                    dir = -1;
                } else {
                    dir = 1;
                }
                uaSetDifficulty(screenNextDifficulty(dir));
                names2[0] = screenGetDifficultyLabel();
                prevChoice = -1;
                break;
            case 1:
                if (twoPlayer == 0) {
                    uasoundPlayMenuCycleFlush();
                    ret = 0x18;
                    return ret;
                }
                break;
            case 2:
                if (twoPlayer == 0) {
                    break;
                }
                if (ctlpadAnyLeft(pad)) {
                    dir = -1;
                } else {
                    dir = 1;
                }
                shellNextLevelChoice(dir);
                uasoundPlayMenuCycleFlush();
                ret = 0x17;
                return ret;
            }
        } else if (ctlpadAnySelect(pad)) {
            switch (gOptionsChoice) {
            case 0:
                uasoundPlayMenuCycleFlush();
                uaSetDifficulty(screenNextDifficulty(1));
                names2[0] = screenGetDifficultyLabel();
                prevChoice = -1;
                break;
            case 3:
                uasoundPlayMenuSelectFlush();
                gOptionsChoice = minChoice;
                prevChoice = -1;
                return ret;
            case 1:
                if (twoPlayer) {
                    uasoundPlayMenuSelectFlush();
                    ret = 0x18;
                    return ret;
                }
                uasoundPlayMenuCycleFlush();
                ret = 0x18;
                return ret;
            case 2:
                if (twoPlayer) {
                    uasoundPlayMenuCycleFlush();
                    ret = 0x17;
                    return ret;
                }
                uasoundPlayMenuSelectFlush();
                ret = 0x16;
                return ret;
            }
        } else if (frames >= 18001) {
            return ret;
        }
        if (prevChoice != gOptionsChoice) {
            VSync(0);
            screenDisplayImage(1, OPTIONS_IMAGE, 0, 0);
            screenDisplayChoicesFromBottom(0, 4, -1, gOptionsChoice, names, names2, 180, 20);
        }
        while (ctlpadAnyKey(pad)) {
            UpdCtlPad();
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenOptions);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9CC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9DC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9EC);

#ifdef NON_MATCHING
s32 screenMainOptions(void)
{
    static s32 prevChoice;
    s32 ret = 1;
    char* choices[4];
    s32 frames;

    choices[0] = "AUDIO CONTROLS";
    choices[1] = "CONTEST HISTORY";
    choices[2] = "STAFF ROLL";
    choices[3] = "EXIT";
    VSync(0);
    screenSetEnv();
    screenDisplayImage(1, OPTIONS_IMAGE, 0, 0);
    screenDisplayChoicesFromBottom(0, 4, -1, gMainOptionsChoice, choices, 0, 180, 20);
    while (ctlpadAnyKey(0)) {
        UpdCtlPad();
    }
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prevChoice = gMainOptionsChoice;
        if (ctlpadAnyUp(0)) {
            frames = 0;
            if (gMainOptionsChoice == 0) {
                gMainOptionsChoice = 3;
            } else {
                gMainOptionsChoice = gMainOptionsChoice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(0)) {
            frames = 0;
            if (gMainOptionsChoice == 3) {
                gMainOptionsChoice = 0;
            } else {
                gMainOptionsChoice = gMainOptionsChoice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnySelect(0)) {
            uasoundPlayMenuSelectFlush();
            switch (gMainOptionsChoice) {
            case 0:
                ret = 0x19;
                break;
            case 1:
                ret = 0x15;
                break;
            case 2:
                ret = 0xF;
                break;
            case 3:
                ret = 1;
                gMainOptionsChoice = 0;
                prevChoice = -1;
                VSync(0);
                screenDisplayImage(1, OPTIONS_IMAGE, 0, 0);
                break;
            }
            return ret;
        } else if (frames >= 18001) {
            return ret;
        }
        if (prevChoice != gMainOptionsChoice) {
            VSync(0);
            screenDisplayImage(1, OPTIONS_IMAGE, 0, 0);
            screenDisplayChoicesFromBottom(0, 4, -1, gMainOptionsChoice, choices, 0, 180, 20);
        }
        while (ctlpadAnyKey(0)) {
            UpdCtlPad();
        }
    }
done:
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenMainOptions);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FB9F8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA10);

#ifdef NON_MATCHING
void screenChooseBattleground(s32 split)
{
    static GsIMAGE tim;
    char buf[32];
    s32 fonts[2];
    DRAWENV env;
    char* levelName;
    s32 (*nextFn)(s32);
    char* (*nameFn)();
    s32 changed;
    s32 done;
    s32 left;
    s32 right;
    s32 frames;
    u_long* image;

    levelName = shellGetTwoPlayerCurrentLevelName();
    changed = 0;
    image = (u_long*)rtGetDoubleBufferArea();
    done = 0;
    right = 0;
    fonts[0] = 2;
    nextFn = shellNextLevelChoice;
    fonts[1] = 3;
    if (split) {
        nextFn = shellNextTwoPlayerLevelChoice;
    }
    nameFn = shellGetCurrentLevelName;
    if (split) {
        nameFn = shellGetTwoPlayerCurrentLevelName;
    }
    VSync(0);
    sprintf(buf, "UASCREEN\\SCRNE3%d.TIM", shellGetCurrentLevel() + 1);
    fileioLoadFileIntoRam(image, buf);
    screenDisplayImage(1, image, 0, 0);
    fontPrintXY(fonts[0], 64, 200, "<");
    fontPrintXY(fonts[0], 239, 200, ">");
    fontPrintCenteredXY(0, 159, 200, levelName);
    fontPrintCenteredXY(2, 159, 220, "Press SELECT To Select");
    while (ctlpadAnyKey(2)) {
        UpdCtlPad();
    }
    frames = 0;
    while (done == 0) {
        VSync(0);
        frames++;
        UpdCtlPad();
        left = ctlpadAnyLeft(2);
        if (left || (right = ctlpadAnyRight(2))) {
            changed = 1;
            uasoundPlayMenuCycleFlush();
            frames = 0;
            if (ctlpadAnyLeft(2)) {
                nextFn(-1);
            } else {
                nextFn(1);
            }
            levelName = nameFn();
            fontPrintXY(fonts[left], 64, 200, "<");
            fontPrintXY(fonts[right], 239, 200, ">");
            sprintf(buf, "UASCREEN\\SCRNE3%d.TIM", shellGetCurrentLevel() + 1);
            fileioLoadFileIntoRam(image, buf);
        } else if (ctlpadAnySelect(2)) {
            done = 1;
            uasoundPlayMenuSelectFlush();
            continue;
        } else if (frames >= 18001) {
            done = 1;
            continue;
        }
        while (ctlpadAnyKey(2)) {
            fontPrintXY(fonts[left], 64, 200, "<");
            fontPrintXY(fonts[right], 239, 200, ">");
            UpdCtlPad();
        }
        right = 0;
        if (changed) {
            VSync(0);
            screenDisplayImage(0, image, 0, 0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            changed = 0;
            fontPrintXY(fonts[0], 64, 200, "<");
            fontPrintXY(fonts[0], 239, 200, ">");
            fontPrintCenteredXY(0, 159, 200, levelName);
            fontPrintCenteredXY(2, 159, 220, "Press SELECT To Select");
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
    }
    DrawSync(0);
    GsGetTimInfo(image + 1, &tim);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenChooseBattleground);
#endif

s32 screenCharacterRowPosition(s32 row, s32 numRows, s32 center, s32 spacing)
{
    row = row + 1;
    return center - (numRows - row) * spacing;
}

#ifdef NON_MATCHING
void screenDisplayChoicesFromBottom(
    s32 mode, s32 count, s32 unused, s32 sel, char** names, char** names2, s32 a6, s32 a7)
{
    void (*pf)(s32, u32, u32, char*);
    s32 i;
    s32 y;
    s32 xa;
    s32 xb;

    if (mode == 2) {
        pf = fontPrintCenteredXY;
        xa = 159;
        xb = 159;
    } else {
        pf = fontPrintXY;
        xa = 30;
        xb = 160;
    }
    for (i = 0; i < count; i++) {
        y = screenCharacterRowPosition(i, count, a6, a7);
        pf(0, xa, y, names[i]);
        if (names2 != 0 && names2[i] != 0) {
            pf(0, xb, y, names2[i]);
        }
    }
    y = screenCharacterRowPosition(sel, count, a6, a7);
    pf(1, xa, y, names[sel]);
    if (names2 != 0 && names2[sel] != 0) {
        pf(1, xb, y, names2[sel]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayChoicesFromBottom);
#endif

#ifdef NON_MATCHING
void screenInitCarOccupants(u8* model)
{
    u8* prim;
    u8* verts;
    u8* uv;
    POLY_FT4* p;
    POLY_FT4* top;
    s32 i;

    if (model != 0 && model[0] == 0) {
        if (gFaceObject == 0) {
            gFaceObject = model;
        }
        top = (POLY_FT4*)uadashGetTopBlitAddr();
        prim = *(u8**)(model + 12);
        verts = *(u8**)(model + 4);
        Face = top;
        for (i = 0; i < 12; i++) {
            setlen(&Face[i], 9);
            setcode(&Face[i], 0x2c);
            Face[i].code |= 2;
            Face[i].r0 = 0xbf;
            Face[i].g0 = 0xbf;
            Face[i].b0 = 0xbf;
            uv = prim + 16;
            uv += (prim[15] >> 2) & 0x1c;
            Face[i].u0 = uv[0];
            Face[i].v0 = uv[5];
            Face[i].u1 = uv[4];
            Face[i].v1 = uv[9];
            Face[i].u2 = uv[8];
            Face[i].v2 = uv[13];
            Face[i].u3 = uv[12];
            Face[i].v3 = uv[1];
            p = (POLY_FT4*)(i * 40 + (s32)Face);
            p->x0 = *(s16*)(verts + *(s16*)(prim + 4) * 8) >> 8;
            p->y0 = *(s16*)(verts + *(s16*)(prim + 4) * 8 + 4) >> 8;
            p->x1 = *(s16*)(verts + *(s16*)(prim + 6) * 8) >> 8;
            p->y1 = *(s16*)(verts + *(s16*)(prim + 6) * 8 + 4) >> 8;
            p->x2 = *(s16*)(verts + *(s16*)(prim + 8) * 8) >> 8;
            p->y2 = *(s16*)(verts + *(s16*)(prim + 8) * 8 + 4) >> 8;
            p->x3 = *(s16*)(verts + *(s16*)(prim + 10) * 8) >> 8;
            p->y3 = *(s16*)(verts + *(s16*)(prim + 10) * 8 + 4) >> 8;
            p->clut = *(u16*)(uv + 2);
            p->tpage = *(u16*)(uv + 6);
            prim += prim[2] * 4;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenInitCarOccupants);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA28);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA3C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA50);

#ifdef NON_MATCHING
void screenLoadCarSelectionBackground(void)
{
    fileioLoadFileIntoRam(TITLE_IMAGE, "UASCREEN\\SCRNC.TIM");
    fileioLoadFileIntoRam(OPTIONS_IMAGE, "UASCREEN\\SCRNF.TIM");
    fileioLoadFileIntoRam(VEHICLE_IMAGE, "UASCREEN\\SCRND.TIM");
    fileioLoadFileIntoRam(AUDIO_IMAGE, "UASCREEN\\SCRNFC1.TIM");
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenLoadCarSelectionBackground);
#endif

void screenShowCarBio(s32 carName, s32 pad)
{
    char name[32];
    void* buf;

    buf = (u_long*)rtGetDoubleBufferArea();
    DrawSync(0);
    VSync(0);
    screenSetEnv();
    while (GetPadStatus((s16)pad) != 0) {
        UpdCtlPad();
    }
    sprintf(name, "UABIOS\\SCNDB%02dB.TIM", GetCarNumFromCarName(carName) + 1);
    fileioLoadFileIntoRam(buf, name);
    screenDisplayImage(0, buf, 0, 0);
    DrawSync(0);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    DrawSync(0);
    do {
        UpdCtlPad();
    } while (ctlpadAnySelect(pad) == 0);
    uasoundPlayMenuSelectFlush();
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA7C);

#ifdef NON_MATCHING
s32 screenHoldForTwoPlayerControl(s32* pa, s32* pb, u_long* image)
{
    u16 blink;
    s32 ret;
    s32 frame;
    char* msg;

    blink = 0;
    msg = "PRESS ANY BUTTON";
    ret = 1;
    shellPlayCarEngineRev();
    frame = 1;
    do {
        UpdCtlPad();
        if (GetPadStatus(2) & 0xffff0000) {
            ret = 0;
            *pa = 1;
            *pb = 1;
        } else if (GetPadStatus(2) & 0xffff) {
            ret = 0;
            *pa = 0;
            *pb = 0;
        }
        while (ctlpadAnyKey(2)) {
            UpdCtlPad();
        }
        if (*pa != 2) {
            break;
        }
        screenSetEnv();
        screenDisplayImage(0, image, 0, 0);
        if (blink & 1) {
            fontPrintCenteredXY(6, 159, 129, msg);
        }
        blink >>= 1;
        if ((blink & 0xffff) == 0) {
            blink = 0xff00;
        }
        VSync(0);
        screenDisplayToDisplay(0, 1);
    } while (frame++ < 1200);
    uasoundPlayMenuSelectFlush();
    soundForceAllSoundsOffNow();
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenHoldForTwoPlayerControl);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA90);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBA9C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBAA8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBAB4);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBAC0);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBACC);

#ifdef NON_MATCHING
s32 screenVehicleChoice(s32 players, u8 demo)
{
    s32 selected[2];
    LVECTOR translation = { 0, -7680, 4096 };
    SVECTOR rotation;
    s32 fonts[4];
    char* menuLabels[3] = { "SELECT", "CAR INFO", "EXIT" };
    char* playerLabels[2];
    s32 playerX[2];
    SPRT menuSprites[3][10];
    DR_MODE menuModes[3];
    SPRT playerSprites[2][10];
    DR_MODE playerModes[2];
    SPRT carNameSprites[2][20];
    DR_MODE carNameMode;
    SPRT arrowSprites[2];
    DR_MODE arrowModes[2];
    ScreenVehicleFace faces[2];
    Db* db;
    Db* cdb;
    POLY_FT4* p;
    s32 car;
    s32 menu;
    s32 remaining;
    s32 shrinkFrames;
    s32 done;
    s32 pad;
    s32 player;
    s32 lastCar;
    s32 left;
    s32 right;
    s32 released;
    s32 reset;
    s32 imageRight;
    s32 savedIsBg;
    s32 frames;
    s32 up;
    s32 i;
    s32 other;
    s32 lf, rf;
    u16 blink;

    selected[0] = selected[1] = -1;
    lastCar = -1;
    left = right = 0;
    fonts[0] = 0;
    fonts[1] = 1;
    fonts[2] = 2;
    fonts[3] = 3;
    blink = 0;
    playerLabels[0] = "Player 1";
    playerLabels[1] = "Player 2";
    playerX[0] = 42;
    playerX[1] = 277;
    car = menu = 0;
    remaining = players;
    shrinkFrames = done = 0;
    player = 0;
    pad = 2;
    while (GetPadStatus(2) != 0)
        UpdCtlPad();
    if (demo) {
        soundInterruptPlayDA();
        fileioLoadFileIntoRam(VEHICLE_IMAGE, "UASCREEN\\SCRND2.TIM");
        soundResumePlayDA();
    }
    soundGetReverbType();
    soundSetReverbOff();
    lightSetColor(0, 2, 60, 60, 128);
    screenSetVehicleHeading(120);
    hierSetLodScale(8);
    screenInitCarOccupants((u8*)gFaceObject);
    reset = 0;
    released = 1;
    faces[0].x = playerX[0];
    faces[0].y = 80;
    faces[1].x = playerX[1];
    faces[1].y = 80;
    db = rtGetDb();
    cdb = rtGetCdb();
    savedIsBg = db[0].draw.isbg;
    db[0].draw.isbg = 0;
    db[1].draw.isbg = 0;
    screenDisplayImage(0, VEHICLE_IMAGE, 0, 0);
    DrawSync(0);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    viewSetDeltaTrans(&translation, 0);
    rotation.vx = 182;
    rotation.vy = 0;
    rotation.vz = 0;
    rotation.pad = 0;
    viewSetDeltaRot(&rotation, 0);
    if (players == 2)
        done = screenHoldForTwoPlayerControl(&pad, &player, VEHICLE_IMAGE);
    else
        pad = 0;
    frames = 0;
    while (!done) {
        UpdCtlPad();
        if (cdb == db) {
            cdb = db + 1;
            imageRight = 1;
        } else {
            cdb = db;
            imageRight = 0;
        }
        cdb->unk8 = (s32)cdb->area;
        VSync(0);
        DrawSync(0);
        ClearOTagR(((u_long*)cdb->small), 4096);
        if (!demo) {
            for (i = 0; i < 3; ++i)
                fontSpritePrintXY(fonts[menu == i], 110, 40 + i * 20, &menuModes[i], menuSprites[i],
                    (u8*)menuLabels[i], ((u_long*)cdb->small) + 4095);
            if (!blink)
                blink = 0xFF00;
            if (players >= 2) {
                if (blink & 1)
                    fontSpritePrintCenteredXY(2, playerX[player], 30, &playerModes[player],
                        playerSprites[player], (u8*)playerLabels[player],
                        ((u_long*)cdb->small) + 4095);
                other = player ^ 1;
                fontSpritePrintCenteredXY(2, playerX[other], 30, &playerModes[other],
                    playerSprites[other], (u8*)playerLabels[other], ((u_long*)cdb->small) + 4095);
            } else if (blink & 1) {
                fontSpritePrintCenteredXY(2, playerX[0], 30, &playerModes[0], playerSprites[0],
                    (u8*)playerLabels[0], ((u_long*)cdb->small) + 4095);
            }
            blink >>= 1;
            lf = rf = 0;
            if ((left || right) && menu == 0) {
                lf = left;
                rf = right;
            }
            fontSpritePrintXY(fonts[2 + lf], screenVehicleArrows[0].x, screenVehicleArrows[0].y,
                &arrowModes[0], &arrowSprites[0], (u8*)screenVehicleArrows[0].label,
                ((u_long*)cdb->small));
            fontSpritePrintXY(fonts[2 + rf], screenVehicleArrows[1].x, screenVehicleArrows[1].y,
                &arrowModes[1], &arrowSprites[1], (u8*)screenVehicleArrows[1].label,
                ((u_long*)cdb->small));
        }
        fontSpritePrintCenteredXY(0, 159, 215, &carNameMode, carNameSprites[imageRight],
            (u8*)uaGetCarNameString(screenGetCarsEnum(car)), ((u_long*)cdb->small));
        other = player ^ 1;
        if (shrinkFrames) {
            if (selected[other] < 12) {
                Face[selected[other]].x0 = SmoothValue(Face[selected[other]].x0, 24, 50);
                Face[selected[other]].y0 = SmoothValue(Face[selected[other]].y0, 22, 50);
                Face[selected[other]].x1 = SmoothValue(Face[selected[other]].x1, 24, 50);
                Face[selected[other]].x2 = SmoothValue(Face[selected[other]].x2, -24, 50);
                Face[selected[other]].y2 = SmoothValue(Face[selected[other]].y2, 22, 50);
                Face[selected[other]].x3 = SmoothValue(Face[selected[other]].x3, -24, 50);
            }
            --shrinkFrames;
        }
        faces[player].poly = Face[car];
        p = &faces[player].poly;
        p->x0 += faces[player].x;
        p->y0 += faces[player].y;
        p->x1 += faces[player].x;
        p->y1 += faces[player].y;
        p->x2 += faces[player].x;
        p->y2 += faces[player].y;
        p->x3 += faces[player].x;
        p->y3 += faces[player].y;
        AddPrim(((u_long*)cdb->small) + 4095, p);
        other = player ^ 1;
        if (selected[other] != -1 && selected[other] < 12) {
            faces[other].poly = Face[selected[other]];
            p = &faces[other].poly;
            p->x0 += faces[other].x;
            p->y0 += faces[other].y;
            p->x1 += faces[other].x;
            p->y1 += faces[other].y;
            p->x2 += faces[other].x;
            p->y2 += faces[other].y;
            p->x3 += faces[other].x;
            p->y3 += faces[other].y;
            AddPrim(((u_long*)cdb->small) + 4095, p);
        }
        UpdCtlPad();
        if (released) {
            if (!demo) {
                frames = 0;
                if (!ctlpadAnyKey(pad))
                    released = 0;
                goto draw_vehicle;
            }
        } else if (!demo) {
            up = ctlpadAnyUp(pad);
            if (up || ctlpadAnyDown(pad)) {
                released = 1;
                if (up) {
                    if (menu-- == 0)
                        menu = 2;
                } else {
                    if (menu++ == 2)
                        menu = 0;
                }
                uasoundPlayMenuCycleFlush();
                goto draw_vehicle;
            }
            if (ctlpadAnyLeft(pad) && ctlpadAnyRight(pad)) {
                if (remaining == 2) {
                    remaining = 1;
                    selected[player] = 12;
                    pad ^= 1;
                    player ^= 1;
                }
                goto draw_vehicle;
            }
        }
        if (ctlpadAnySelect(pad)) {
            if (demo)
                break;
            switch (menu) {
            case 0:
                if (screenGetCarsEnum(car) != -1)
                    selected[player] = car;
                if (remaining == 1)
                    done = 1;
                else {
                    --remaining;
                    pad ^= 1;
                    player ^= 1;
                    car = car == 0 ? 1 : 0;
                }
                shrinkFrames = 8;
                uasoundPlayMenuSelectFlush();
                break;
            case 1:
                uasoundPlayMenuSelectFlush();
                screenShowCarBio(screenGetCarsEnum(car), pad);
                break;
            case 2:
                uasoundPlayMenuSelectFlush();
                done = 1;
                break;
            }
            released = 1;
            goto draw_vehicle;
        }
        if (!demo) {
            left = ctlpadAnyLeft(pad);

            if (left || (right = ctlpadAnyRight(pad))) {
                released = 1;
                if (menu == 2)
                    continue;
                reset = 1;
                if (left) {
                    if (--car < 0)
                        car = 11;
                    if (car == selected[player ^ 1])
                        --car;
                    if (car < 0)
                        car = 11;
                } else {
                    if (++car >= 12)
                        car = 0;
                    if (car == selected[player ^ 1])
                        ++car;
                    if (car >= 12)
                        car = 0;
                }
                uasoundPlayMenuCycleFlush();
            } else if (frames >= 18001) {
                selected[0] = selected[1] = -1;
                break;
            }
        }
    draw_vehicle:
        if (demo && frames >= 361) {
            frames = 0;
            screenSetVehicleHeading(120);
            ++car;
            reset = 1;
            if (car == 12)
                break;
        }
        if (reset) {
            screenResetFrameNum();
            viewSetDeltaTrans(&translation, 0);
            reset = 0;
        }
        screenSpinVehicle(cdb, screenGetCarsEnum(car), left, selected[player ^ 1]);
        if (lastCar != car) {
            if (lastCar != -1)
                soundForceAllSoundsOffNow();
            lastCar = car;
            uasoundPlayCarSignature(screenGetCarsEnum(car), 0, 0);
        }
        uasoundPlayCarEngineRev(screenGetCarsEnum(car), 0, 0, 30);
        soundProcessIds();
        PutDrawEnv(&cdb->draw);
        PutDispEnv(&cdb->disp);
        screenDisplayImage(imageRight, VEHICLE_IMAGE, 0, 0);
        ++frames;
        DrawOTag(((u_long*)cdb->small) + 4095);
    }
    if (frames >= 18001)
        selected[0] = selected[1] = -1;
    if (!demo) {
        if (menu == 2)
            selected[0] = -1;
        else {
            for (i = 0; i < players; ++i)
                if (selected[i] != -1)
                    UASetPlayerCar((s16)i, screenGetCarsEnum(selected[i]), 1);
        }
    } else {
        soundInterruptPlayDA();
        fileioLoadFileIntoRam(VEHICLE_IMAGE, "UASCREEN\\SCRND.TIM");
        soundResumePlayDA();
    }
    VSync(0);
    DrawSync(0);
    db[0].draw.isbg = savedIsBg;
    db[1].draw.isbg = savedIsBg;
    hierSetLodScale(3);
    return screenGetCarsEnum(selected[0]);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenVehicleChoice);
#endif

s32 screenGetCarsEnum(s32 which)
{
    switch (which) {
    case 0:
        return 10;
    case 1:
        return 20;
    case 2:
        return 30;
    case 3:
        return 50;
    case 4:
        return 100;
    case 5:
        return 60;
    case 6:
        return 90;
    case 7:
        return 70;
    case 8:
        return 80;
    case 9:
        return 110;
    case 10:
        return 40;
    case 11:
        return 120;
    case 12:
        return 130;
    }
    return -1;
}

void screenSetVehicleHeading(s32 heading)
{
    gHeading = (heading << 12) / 360;
}

void screenResetFrameNum(void)
{
    gFrameNum = 0;
    InLeft = 0;
}

#ifdef NON_MATCHING
void screenSpinVehicle(Db* db, s32 carName, s32 force, s32 exclude)
{
    CarStats e;
    Cs* cs;
    s32 spin;
    s16 carNum;
    s32 next;
    s32 x;

    spin = (rsin((gFrameNum << 14) / 360) * 30) >> 4;
    if (force != 0 || InLeft != 0) {
        InLeft = 1;
        spin = -spin;
    }
    carNum = GetCarNumFromCarName(carName);
    cs = UAGetCs(carName);
    if (cs == 0) {
        printf("Car %d\n", carName);
        return;
    }
    cs->pos.vx = (InLeft == 0 ? 7680 : -7680) - spin;
    cs->pos.vy = 0;
    CarInitDeltas(&e, carName);
    cs->pos.vz = e.unk6C * 32;
    cs->rot.vx = 0;
    cs->rot.vy = 0;
    cs->rot.vz = gHeading;
    RotMatrixYXZ(&cs->rot, &cs->mat);
    viewProc(0);
    hierProcOne(db, cs);
    if (gFrameNum < 22) {
        next = carNum;
        if (InLeft != 0) {
            next = next + 1;
            if (next >= 12) {
                next = 0;
            }
            if (next == exclude) {
                next = next + 1;
            }
            if (next >= 12) {
                next = 0;
            }
        } else {
            next = next - 1;
            if (next < 0) {
                next = 11;
            }
            if (next == exclude) {
                next = next - 1;
            }
            if (next < 0) {
                next = 11;
            }
        }
        carName = GetCarNameFromCarNum(next);
        cs = UAGetCs(carName);
        if (cs == 0) {
            return;
        }
        cs->pos.vx = -spin;
        cs->pos.vy = 0;
        CarInitDeltas(&e, carName);
        cs->pos.vz = e.unk6C * 32;
        cs->rot.vx = 0;
        cs->rot.vy = 0;
        cs->rot.vz = gHeading;
        RotMatrixYXZ(&cs->rot, &cs->mat);
        viewProc(0);
        hierProcOne(db, cs);
    }
    gHeading += 11;
    if (gFrameNum < 22) {
        gFrameNum = gFrameNum + 1;
    } else {
        InLeft = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenSpinVehicle);
#endif

#ifdef NON_MATCHING
void screenGrayedScreen(void)
{
    POLY_F4 poly;
    DRAWENV env;

    setPolyF4(&poly);
    SetSemiTrans(&poly, 1);
    setXY4(&poly, 0, 0, 320, 0, 0, 240, 320, 240);
    setRGB0(&poly, 0, 0, 0);
    SetDefDrawEnv(&env, 320, 0, 320, 240);
    env.tpage = GetTPage(0, 0, 0, 0);
    PutDrawEnv(&env);
    VSync(0);
    screenSetEnv();
    DrawPrim(&poly);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGrayedScreen);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBB94);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBBA8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBBBC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBBC8);

#ifdef NON_MATCHING
s32 screenTransitionLevel(s32 level, s32 advanced)
{
    char* meetText = "Meet Your Opponents";
    char lines[5][50];
    DRAWENV env;
    u_long* image;
    s32 pad;
    s32 count;
    s32 line;
    s32 previous;
    s32 x;
    s32 start;
    s32 limit;
    s32 phase;

    image = (u_long*)rtGetDoubleBufferArea();
    count = 5;
    pad = 2 * (rtIsSplitScreenOn() != 0);
    DrawSync(0);
    while (GetPadStatus((s16)pad) != 0)
        UpdCtlPad();
    screenGrayedScreen();
    screenVramToRam(320, 0, 320, 240, image);
    sprintf(lines[0], advanced ? "CONGRATULATIONS!" : "WELCOME!");
    sprintf(lines[1], "Prepare for...");
    sprintf(lines[2], "%s", gTransitionLabels[level]);
    sprintf(lines[3], "%s", gTransitionLabels[7 + level]);
    sprintf(lines[4], "%s", screenPasswordForLevel(level));
    if (level == 0)
        count = 3;
    for (line = 0; line < count; ++line) {
        x = (fontStringWidth(6, (u8*)lines[line]) >> 1) + 320;
        while (x >= 151) {
            screenRamToVram(0, 0, 320, 240, image);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);

            for (previous = line - 1; previous >= 0; --previous)
                fontPrintCenteredXY(6, 159, 75 + previous * 30, lines[previous]);
            fontPrintCenteredXY(line < 4 ? 6 : 5, x < 159 ? 159 : x, 75 + line * 30, lines[line]);
            x -= 20;
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
    }
    DrawSync(0);
    screenRamToVram(0, 0, 320, 240, image);
    SetDefDrawEnv(&env, 0, 0, 320, 240);
    PutDrawEnv(&env);
    if (count >= 4) {
        fontPrintCenteredXY(5, 159, 195, lines[4]);
        fontPrintCenteredXY(6, 159, 165, lines[3]);
    }
    fontPrintCenteredXY(6, 159, 135, lines[2]);
    fontPrintCenteredXY(6, 159, 105, lines[1]);
    fontPrintCenteredXY(6, 159, 75, lines[0]);
    DrawSync(0);
    PutDrawEnv(&gScreenDrawEnv);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    while (!shellCheckTransitionStatus())
        VSync(0);
    shellLoadCards();
    screenRamToVram(0, 0, 320, 240, image);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    DrawSync(0);
    shellDrawCards();
    shellSetTransitionState(1);
    shellCheckTransitionStatus();
    screenVramToRam(320, 0, 320, 240, image);
    meetText[18] = screenGetLevelNumCards(level) == 1 ? 0 : 's';
    start = -(fontStringWidth(6, (u8*)meetText) >> 1);
    limit = 159;
    DrawSync(0);
    for (phase = 0; phase < 2; ++phase) {
        x = start;
        while (x < limit) {
            DrawSync(0);
            shellCheckTransitionStatus();
            screenRamToVram(0, 0, 320, 240, image);
            DrawSync(0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            fontPrintCenteredXY(6, x, 220, meetText);
            DrawSync(0);
            x += 20;
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
        limit = 335 - start;
        start = x;
        while (!shellCheckTransitionStatus())
            VSync(0);
    }
    fontSetDefaultColor(6);
    return DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenTransitionLevel);
#endif

void screenLostGame(void)
{
    uasoundPlayCarSignature(10, 0, 0);
    soundProcessIds();
    screenGrayedScreen();
    fontPrintCenteredXY(6, 159, 119, "YOU LOSE!");
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBBE4);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBBFC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBC18);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBC34);

#ifdef NON_MATCHING
void screenBossCar(void)
{
    s32 pad;
    s32 i;

    i = 300;
    pad = 2 * (rtIsSplitScreenOn() != 0);
    screenGrayedScreen();
    fontPrintCenteredXY(6, 159, 70, "CONGRATULATIONS!");
    fontPrintCenteredXY(6, 159, 100, "You have made it to the");
    fontPrintCenteredXY(6, 159, 130, "FINAL round.  Prepare to");
    fontPrintCenteredXY(6, 159, 160, "battle MINION, last year's");
    fontPrintCenteredXY(6, 159, 190, "Twisted Metal winner!");
    while (GetPadStatus((s16)pad) != 0) {
        UpdCtlPad();
    }
    while (--i != -1) {
        VSync(0);
        UpdCtlPad();
        if (ctlpadAnySelect(pad)) {
            uasoundPlayMenuSelectFlush();
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenBossCar);
#endif

void screenInit(void)
{
    SetDefDrawEnv(&gScreenDrawEnv, 320, 0, 320, 240);
    PutDrawEnv(&gScreenDrawEnv);
    SetDefDispEnv(&gScreenDisplayEnv, 320, 0, 320, 240);
    PutDispEnv(&gScreenDisplayEnv);
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBC4C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBC64);

#ifdef NON_MATCHING
char* screenChooseControls(s32 mode, s32 pad, s32 unused)
{
    static GsIMAGE tim;
    char imageName[32];
    char playerName[16];
    DRAWENV env;
    s32 fonts[2];
    u_long* image;
    s32 config;
    s32 inputPad;
    s32 changed;
    s32 left;
    s32 right;
    s32 waitRelease;
    s32 idleFrames;
    s32 labelFrames;
    u32 blink;

    image = (u_long*)rtGetDoubleBufferArea();
    labelFrames = 0;
    inputPad = 0;
    fonts[0] = 2;
    fonts[1] = 3;
    PutDrawEnv(&gScreenDrawEnv);
    DrawSync(0);
    VSync(0);
    soundProcessIds();
    if (mode >= 2)
        inputPad = pad != 0;
    config = PadGetConfig((s16)pad);
    sprintf(imageName, "UASCREEN\\SCRNFB%d.TIM", config);
    fileioLoadFileIntoRam(image, imageName);
    screenDisplayImage(0, image, 0, 0);
    if (config != 0)
        fontPrintXY(fonts[0], screenControlArrows[0].x, screenControlArrows[0].y,
            screenControlArrows[0].label);
    changed = 0;
    if (config != 3)
        fontPrintXY(fonts[0], screenControlArrows[1].x, screenControlArrows[1].y,
            screenControlArrows[1].label);
    left = 0;
    right = 0;
    idleFrames = 0;
    waitRelease = 1;
    blink = 0xF0F0;
    sprintf(playerName, "Player %d", pad + 1);

    for (;;) {
        VSync(0);
        ++idleFrames;
        UpdCtlPad();
        if (waitRelease) {
            idleFrames = 0;
            if (config != 0)
                fontPrintXY(fonts[left], screenControlArrows[0].x, screenControlArrows[0].y,
                    screenControlArrows[0].label);
            if (config != 3)
                fontPrintXY(fonts[right], screenControlArrows[1].x, screenControlArrows[1].y,
                    screenControlArrows[1].label);
            if (!ctlpadAnyKey(inputPad))
                waitRelease = 0;
            if (!waitRelease) {
                left = 0;
                right = 0;
            }
        } else {
            if (ctlpadAnySelect(inputPad)) {
                uasoundPlayMenuSelectFlush();
                break;
            }
            left = ctlpadAnyLeft(inputPad);
            if (left || (right = ctlpadAnyRight(inputPad))) {
                waitRelease = 1;
                if (left) {
                    if (config > 0) {
                        --config;
                        changed = 1;
                    }
                } else if (config < 3) {
                    ++config;
                    changed = 1;
                }
                if (changed) {
                    uasoundPlayMenuCycleFlush();
                    fontPrintXY(fonts[left], screenControlArrows[0].x, screenControlArrows[0].y,
                        screenControlArrows[0].label);
                    fontPrintXY(fonts[right], screenControlArrows[1].x, screenControlArrows[1].y,
                        screenControlArrows[1].label);
                }
            } else if (idleFrames >= 18001) {
                break;
            }
        }
        if (changed) {
            VSync(0);
            sprintf(imageName, "UASCREEN\\SCRNFB%d.TIM", config);
            fileioLoadFileIntoRam(image, imageName);
            changed = 0;
        }
        DrawSync(0);
        screenSetEnv();
        screenDisplayImage(0, image, 0, 0);
        SetDefDrawEnv(&env, 0, 0, 320, 240);
        DrawSync(0);
        PutDrawEnv(&env);
        if (mode >= 2) {
            if (labelFrames++ < 60) {
                if (blink & 1)
                    fontPrintCenteredXY(0, 159, 40, playerName);
                if (blink == 0)
                    blink = 0xF0F0;
                blink >>= 1;
            } else {
                fontPrintCenteredXY(0, 159, 40, playerName);
            }
        }
        if (config != 0)
            fontPrintXY(fonts[left], screenControlArrows[0].x, screenControlArrows[0].y,
                screenControlArrows[0].label);
        if (config != 3)
            fontPrintXY(fonts[right], screenControlArrows[1].x, screenControlArrows[1].y,
                screenControlArrows[1].label);
        DrawSync(0);
        PutDrawEnv(&gScreenDrawEnv);
        VSync(0);
        screenDisplayToDisplay(0, 1);
    }
    DrawSync(0);
    GsGetTimInfo(image + 1, &tim);
    PadSetConfig(config, (s16)pad);
    return screenGetControlPadLabel(config);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenChooseControls);
#endif

#ifdef NON_MATCHING
char* screenGetControlPadLabel(s32 which)
{
    return screenControlPadLabels[which];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetControlPadLabel);
#endif

#ifdef NON_MATCHING
char* screenGetMusicLabel(void)
{
    return screenVolumeLabels[ua_music_vol];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetMusicLabel);
#endif

s32 screenNextMusicChoice(s32 delta)
{
    ua_music_vol += delta;
    if (ua_music_vol < 0) {
        ua_music_vol = 0;
    } else if (ua_music_vol == 15) {
        ua_music_vol = 14;
    }
    return ua_music_vol;
}

void screenSetMusicVolume(void)
{
    s32 vol;
    s32 choice;

    choice = ua_music_vol;
    do {
        if (choice != 14) {
            vol = choice * 7;
        } else {
            vol = 99;
        }
    } while (0);
    uasoundSetMusicVolume(vol);
}

s32 screenGetMusicVolume(void)
{
    s32 vol;

    if (ua_music_vol != 14) {
        vol = ua_music_vol * 7;
    } else {
        vol = 99;
    }
    return vol;
}

#ifdef NON_MATCHING
char* screenGetEffectsLabel(void)
{
    return screenVolumeLabels[ua_sfx_vol];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetEffectsLabel);
#endif

s32 screenNextEffectsChoice(s32 delta)
{
    ua_sfx_vol += delta;
    if (ua_sfx_vol < 0) {
        ua_sfx_vol = 0;
    } else if (ua_sfx_vol == 15) {
        ua_sfx_vol = 14;
    }
    return ua_sfx_vol;
}

void screenSetEffectsVolume(void)
{
    s32 vol;
    s32 choice;

    choice = ua_sfx_vol;
    do {
        if (choice != 14) {
            vol = choice * 7;
        } else {
            vol = 99;
        }
    } while (0);
    uasoundSetMusicVolume(99 - vol);
}

#ifdef NON_MATCHING
char* screenGetDifficultyLabel(void)
{
    return screenDifficultyLabels[gDifficulty];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetDifficultyLabel);
#endif

s32 screenNextDifficulty(s32 delta)
{
    gDifficulty += delta;
    if (gDifficulty < 0) {
        gDifficulty = 2;
    } else if (gDifficulty == 3) {
        gDifficulty = 0;
    }
    return gDifficulty;
}

#ifdef NON_MATCHING
void screenLostALife(s32 unused_lives)
{
    POLY_F4 prim;
    DRAWENV draw;
    s32 i;
    s32 n;
    s32 t;

    n = 30;
    screenSetEnv();
    i = 1;
    screenDumpScreenToRam(0, (u_long*)rtGetDoubleBufferArea());
    setPolyF4(&prim);
    SetSemiTrans(&prim, 1);
    setXY4(&prim, 0, 0, 320, 0, 0, 240, 320, 240);
    SetDefDrawEnv(&draw, 0, 0, 320, 240);
    draw.tpage = GetTPage(0, 0, 0, 0);
    do {
        UpdCtlPad();
        if (ctlpadAnySelect(0)) {
            uasoundPlayMenuSelectFlush();
            return;
        }
        screenLoadScreenFromRam(0, (u_long*)rtGetDoubleBufferArea());

        t = ((n - i) << 8) * 0xFF / n;
        i++;
        setRGB0(&prim, ~(t >> 8), 0, 0);
        PutDrawEnv(&draw);
        DrawPrim(&prim);
        VSync(0);
        DrawSync(0);
        screenDisplayToDisplay(0, 1);
    } while (i <= n);
    VSync(60);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenLostALife);
#endif

s32 screenMovieIsDoneTest(s32 frames, s32 limit, u8 flip)
{
    s32 result;

    result = 0;
    UpdCtlPad();
    if (ctlpadAnySelect(2 * (rtIsSplitScreenOn() != 0))) {
        uasoundPlayMenuSelectFlush();
        result = 1;
    }
    return result;
}

s32 screenMovieIsDoneTestWithTwoSecondWait(s32 frames, s32 limit, u8 flip)
{
    s32 result;

    result = 0;
    UpdCtlPad();
    if (ctlpadAnySelect(2 * (rtIsSplitScreenOn() != 0))) {
        if (frames < 31) {
            goto done;
        }
        uasoundPlayMenuSelectFlush();
        result = 1;
    }
done:
    return result;
}

#ifdef NON_MATCHING
s32 screenPlayCinema(s32 which)
{
    static DRAWENV draws[2];
    u8 done;
    s32 result;
    s32 w;
    s32 sw;
    s32 sh;
    MovieCallback cb;

    done = 1;
    w = 320;
    soundInterruptPlayDA();
    soundForceAllSoundsOffNow();
    sw = 320;
    sh = 240;
    cb = screenMovieIsDoneTest;
    if (which == 0) {
        cb = screenMovieIsDoneTestWithTwoSecondWait;
    }
    VSync(0);
    DrawSync(0);
    screenSetEnv();
    if (screenCinemas[which].rgb24 != 0) {
        w = 480;
        SetDefDispEnv(&displays[0], 0, 0, 320, sh);
        SetDefDispEnv(&displays[1], 480, 0, 320, sh);
        displays[0].isrgb24 = 0;
        displays[1].isrgb24 = 1;
        PutDispEnv(&displays[1]);
        SetDefDrawEnv(&draws[0], 0, 0, 480, 256);
        SetDefDrawEnv(&draws[1], 480, 0, 480, 256);
        draws[0].isbg = 1;
        draws[1].isbg = 1;
        PutDrawEnv(&draws[1]);
    } else {
        SetDefDispEnv(&displays[0], 0, 0, 320, sh);
        SetDefDispEnv(&displays[1], 320, 0, 320, sh);
    }
    result = moviecdPlayMovie(screenCinemas[which].file, cb, 0, screenCinemas[which].rgb24,
        screenCinemas[which].frames, 0, 0, w, 0, sw, sh, displays, &done);
    DrawSync(0);
    VSync(0);
    screenSetEnv();
    if (screenCinemas[which].rgb24 != 0) {
        screenClearScreen(0, 0, 320, 240);
        goto disp;
    }
    if (done == 0) {
        VSync(0);
    disp:
        screenDisplayToDisplay(0, 1);
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenPlayCinema);
#endif

void screenPlayAllMovies(void)
{
    u32 i;

    for (i = 0; i < 3; i++) {
        if (screenCinemas[i].file[0] != 0) {
            screenPlayCinema(i);
        }
    }
}

void screenDisplayLogos(void)
{
    screenPlayCinema(0);
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBD94);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBDA8);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBDBC);

#ifdef NON_MATCHING
void screenCheckMidGameOptions(void)
{
    static char* aborted = "BATTLE ABORTED";
    s32 choice = 0;
    char* pw = "";
    POLY_F4 poly;
    DRAWENV env;
    char buf[30] = "";
    char* names[5] = { "START", "QUIT", "AUDIO", "CONTROLLER OPTIONS", "" };
    s32 fonts[2];
    s32 pad;
    s32 prev;
    s32* lo;
    s32* pf;
    s32* pf2;
    s32* pf3;
    s32* pf4;
    char* paused;

    fonts[0] = 0;
    fonts[1] = 1;
    pad = (rtIsSplitScreenOn() != 0) * 2;
    paused = "PAUSED";
    if (GetCtlPad(1, pad) == 0) {
        return;
    }
    soundInterruptPlayDA();
    DrawSync(0);
    VSync(0);
    screenSetEnv();
    soundForceAllSoundsOffNow();
    screenDisplayToDisplay(1, 0);
    setPolyF4(&poly);
    SetSemiTrans(&poly, 1);
    setXY4(&poly, 0, 0, 320, 0, 0, 240, 320, 240);
    setRGB0(&poly, 0, 0, 0);
    SetDefDrawEnv(&env, 0, 0, 320, 240);
    env.tpage = GetTPage(0, 0, 0, 0);
    PutDrawEnv(&env);
    DrawPrim(&poly);
    DrawSync(0);
    VSync(0);
    screenSetEnv();
    screenDisplayToDisplay(0, 1);
    if (rtIsSplitScreenOn() == 0 && shellGetCurrentLevel() != 0) {
        pw = screenPasswordForLevel(shellGetCurrentLevel());
        sprintf(buf, "%s PASSWORD:", shellGetCurrentLevelName());
    }
    fontPrintCenteredXY(6, 159, 60, paused);
    fontPrintCenteredXY(fonts[0], 159, 90, buf);
    fontPrintCenteredXY(5, 159, 115, pw);
    lo = fonts;
    pf = (choice == 0) ? fonts + 1 : lo;
    fontPrintCenteredXY(*pf, 159, 140, names[0]);
    pf2 = (choice == 1) ? fonts + 1 : lo;
    fontPrintCenteredXY(*pf2, 159, 160, names[1]);
    pf3 = (choice == 2) ? fonts + 1 : lo;
    fontPrintCenteredXY(*pf3, 159, 180, names[2]);
    pf4 = (choice == 3) ? fonts + 1 : lo;
    fontPrintCenteredXY(*pf4, 159, 200, names[3]);
    while (ctlpadAnyKey(pad)) {
        UpdCtlPad();
    }
    for (;;) {
        VSync(0);
        prev = choice;
        UpdCtlPad();
        if (ctlpadAnyUp(pad)) {
            if (choice == 0) {
                choice = 3;
            } else {
                choice = choice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(pad)) {
            if (choice == 3) {
                choice = 0;
            } else {
                choice = choice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnySelect(pad)) {
            uasoundPlayMenuSelectFlush();
            switch (choice) {
            case 0:
                while (ctlpadAnyKey(pad)) {
                    UpdCtlPad();
                }
                soundResumePlayDA();
                screenDisplayToDisplay(0, 1);
                DrawSync(0);
                return;
            case 2:
                while (ctlpadAnyKey(pad)) {
                    UpdCtlPad();
                }
                screenAudioOptions(0, 0);
                soundResumePlayDA();
                while (ctlpadAnyKey(pad)) {
                    UpdCtlPad();
                }
                DrawSync(0);
                return;
            case 3:
                if (rtIsSplitScreenOn()) {
                    screenChooseControls(2, screenGetWhichPad(), 0);
                } else {
                    screenChooseControls(1, 0, 0);
                }
                soundResumePlayDA();
                while (ctlpadAnyKey(pad)) {
                    UpdCtlPad();
                }
                DrawSync(0);
                return;
            case 1:
                VSync(0);
                screenDisplayToDisplay(0, 1);
                fontPrintCenteredXY(6, 159, 115, aborted);
                rtReturnToShell(2, 0);
                while (ctlpadAnyKey(pad)) {
                    UpdCtlPad();
                }
                DrawSync(0);
                screenWaitForContinue(30, 30);
                return;
            }
        }
        if (prev != choice) {
            VSync(0);
            fontPrintCenteredXY(6, 159, 60, paused);
            fontPrintCenteredXY(fonts[0], 159, 90, buf);
            fontPrintCenteredXY(5, 159, 115, pw);
            lo = fonts;
            pf = (choice == 0) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf, 159, 140, names[0]);
            pf2 = (choice == 1) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf2, 159, 160, names[1]);
            pf3 = (choice == 2) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf3, 159, 180, names[2]);
            pf4 = (choice == 3) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf4, 159, 200, names[3]);
        }
        while (ctlpadAnyKey(pad)) {
            UpdCtlPad();
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCheckMidGameOptions);
#endif

#ifdef NON_MATCHING
void screenClearScreen(s32 x, s32 y, s32 w, s32 h)
{
    BLK_FILL fill;

    SetBlockFill(&fill);
    setRGB0(&fill, 0, 0, 0);
    setXY0(&fill, x, y);
    setWH(&fill, w, h);
    DrawPrim(&fill);
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenClearScreen);
#endif

#ifdef NON_MATCHING
void screenPrintScore(s16 who, s16 wins, s16 losses)
{
    char* names[2];
    char buf1[8];
    char buf2[8];
    char* label[2];
    s32 font[2];

    names[0] = uaGetCarNameString(*(s32*)((char*)GetPlayerInfo(0) + 148));
    names[1] = uaGetCarNameString(*(s32*)((char*)GetPlayerInfo(1) + 148));
    screenGrayedScreen();
    label[who] = "WINNER!";
    label[who ^ 1] = "LOSER";
    font[who] = 1;
    font[who ^ 1] = 0;
    fontPrintCenteredXY(font[0], 159, 40, label[0]);
    fontPrintXY(font[0], 20, 79, names[0]);
    sprintf(buf1, "%d", wins);
    sprintf(buf2, "%d", losses);
    fontPrintCenteredXY(2, 220, 64, "WINS");
    fontPrintCenteredXY(2, 220, 84, buf1);
    fontPrintCenteredXY(2, 270, 64, "LOSSES");
    fontPrintCenteredXY(2, 270, 84, buf2);
    fontPrintCenteredXY(font[1], 159, 154, label[1]);
    fontPrintXY(font[1], 20, 193, names[1]);
    sprintf(buf1, "%d", losses);
    sprintf(buf2, "%d", wins);
    fontPrintCenteredXY(2, 220, 178, "WINS");
    fontPrintCenteredXY(2, 220, 198, buf1);
    fontPrintCenteredXY(2, 270, 178, "LOSSES");
    fontPrintCenteredXY(2, 270, 198, buf2);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenPrintScore);
#endif

#ifdef NON_MATCHING
void screenInitCardSprites(s32 which, s16 count, s32* opponents)
{
    POLY_FT4* p;
    s16 i;
    s32 v;
    s32 vv;
    s32 tp;

    v = 0;
    tp = 10;
    whichLevel = which;
    SetDefDispEnv(&gCardsDisplayEnv[0], 320, 0, 320, 240);
    SetDefDrawEnv(&gCardsDrawEnv[0], 0, 0, 320, 240);
    gCardsDrawEnv[0].isbg = 1;
    gCardsDrawEnv[0].dtd = 0;
    SetDefDispEnv(&gCardsDisplayEnv[1], 0, 0, 320, 240);
    SetDefDrawEnv(&gCardsDrawEnv[1], 320, 0, 320, 240);
    gCardsDrawEnv[1].isbg = 1;
    gCardsDrawEnv[1].dtd = 0;
    for (i = 0; i < count; i++) {
        p = &gCards[i];
        setPolyFT4(p);
        SetSemiTrans(p, 0);
        p->r0 = 127;
        p->g0 = 127;
        p->b0 = 127;
        gCards[i].x0 = D_80171024[which];
        gCards[i].y0 = -D_80171034[which];
        gCards[i].x1 = D_80171024[which];
        gCards[i].y1 = D_80171034[which];
        gCards[i].x2 = -D_80171024[which];
        gCards[i].y2 = -D_80171034[which];
        gCards[i].x3 = -D_80171024[which];
        gCards[i].y3 = D_80171034[which];
        vv = v;
        if (i == 4) {
            tp++;
            v = 0;
            vv = 0;
        }
        gCards[i].v0 = v;
        gCards[i].v1 = vv + 63;
        gCards[i].v3 = vv + 63;
        gCards[i].u0 = 127;
        gCards[i].u1 = 127;
        gCards[i].u2 = 0;
        gCards[i].u3 = 0;
        gCards[i].v2 = vv;
        gCards[i].tpage = tp + 128;
        gCards[i].clut = ((i & 0x3ff) << 6) | 0x30;
        v += 64;
    }
    setPolyFT4(&screen_sw);
    SetSemiTrans(&screen_sw, 0);
    screen_sw.r0 = 127;
    screen_sw.g0 = 127;
    screen_sw.b0 = 127;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenInitCardSprites);
#endif

#ifdef NON_MATCHING
void screenResetCards(s32 which)
{
    x0 = 160;
    y0 = 320;
    angle = 0;
    if (which >= 0) {
        gCards[which].x0 = screen_sw.x0;
        gCards[which].y0 = screen_sw.y0;
        gCards[which].x1 = screen_sw.x1;
        gCards[which].y1 = screen_sw.y1;
        gCards[which].x2 = screen_sw.x2;
        gCards[which].y2 = screen_sw.y2;
        gCards[which].x3 = screen_sw.x3;
        gCards[which].y3 = screen_sw.y3;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenResetCards);
#endif

#ifdef NON_MATCHING
void screenDrawCard(s32 card, s32 instant, s32 skipMoving)
{
    MATRIX matrix;
    SVECTOR rotation;
    SVECTOR input;
    VECTOR output;
    long flag;
    s32 row;
    s32 col;
    s32 random;
    s32 delta;
    s16 i;
    u_long* image;

    if (card < 0)
        return;
    screen_whichOT ^= 1;
    screen_ot = screen_OT[screen_whichOT];
    DrawSync(0);
    ClearOTagR(screen_ot, 2);
    if (!skipMoving) {
        if (!instant) {
            x0 = SmoothValue(x0, D_80170F44[whichLevel][card], 85);
            y0 = SmoothValue(y0, D_80170FB4[whichLevel][card], 85);
            angle = SmoothValue(angle, 1080, 85);
        } else {
            x0 = D_80170F44[whichLevel][card];
            y0 = D_80170FB4[whichLevel][card];
            random = rand();
            delta = random & 6;
            if (random & 8)
                delta = -delta;
            angle = 360 + delta;
        }
        for (row = 0; row < 3; ++row) {
            for (col = 0; col < 3; ++col)
                matrix.m[row][col] = row == col ? 4096 : 0;
            matrix.t[row] = 0;
        }
        matrix.t[0] = x0;
        matrix.t[1] = y0;
        matrix.t[2] = 0;
        rotation.vx = 0;
        rotation.vy = 0;

        rotation.vz = ((s32)angle * 4096) / 360;
        rotation.pad = 0;
        RotMatrixYXZ(&rotation, &matrix);
        SetTransMatrix(&matrix);
        SetRotMatrix(&matrix);
        input.vz = 0;
        input.pad = 0;
        input.vx = gCards[card].x0;
        input.vy = gCards[card].y0;
        RotTrans(&input, &output, &flag);
        screen_sw.x0 = output.vx;
        screen_sw.y0 = output.vy;
        input.vx = gCards[card].x1;
        input.vy = gCards[card].y1;
        RotTrans(&input, &output, &flag);
        screen_sw.x1 = output.vx;
        screen_sw.y1 = output.vy;
        input.vx = gCards[card].x2;
        input.vy = gCards[card].y2;
        RotTrans(&input, &output, &flag);
        screen_sw.x2 = output.vx;
        screen_sw.y2 = output.vy;
        input.vx = gCards[card].x3;
        input.vy = gCards[card].y3;
        RotTrans(&input, &output, &flag);
        screen_sw.x3 = output.vx;
        screen_sw.y3 = output.vy;
        screen_sw.u0 = gCards[card].u0;
        screen_sw.u1 = gCards[card].u1;
        screen_sw.u2 = gCards[card].u2;
        screen_sw.u3 = gCards[card].u3;
        screen_sw.v0 = gCards[card].v0;
        screen_sw.v1 = gCards[card].v1;
        screen_sw.v2 = gCards[card].v2;
        screen_sw.v3 = gCards[card].v3;
        screen_sw.clut = gCards[card].clut;
        screen_sw.tpage = gCards[card].tpage;
        AddPrim(screen_ot, &screen_sw);
    }
    for (i = 0; i < card; ++i)
        AddPrim(screen_ot, &gCards[i]);
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&gCardsDrawEnv[screen_whichOT]);
    PutDispEnv(&gCardsDisplayEnv[screen_whichOT]);
    image = (u_long*)rtGetDoubleBufferArea();
    screenRamToVram(gCardsDrawEnv[screen_whichOT].clip.x, gCardsDrawEnv[screen_whichOT].clip.y, 320,
        240, image);
    DrawOTag(screen_ot + 1);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDrawCard);
#endif

#ifdef NON_MATCHING
void screenLoadCarPictures(s16 numCars, s32* carNames)
{
    RECT rect;
    GsIMAGE tim;
    s32 xoff;
    s32 yoff;
    u8* buf;
    POLY_FT4* card;
    s16 i;

    buf = rtGetSmallBufferArea();
    for (i = 0; i < numCars; i++) {
        fileioLoadFileIntoRam(buf, screenCarPictures[GetCarNumFromCarName(carNames[i])]);
        card = &gCards[i];
        yoff = (card->tpage << 4) & 0x100;
        xoff = (card->tpage << 6) & 0x3ff;
        GsGetTimInfo((u_long*)(buf + 4), &tim);
        rect.x = (card->u2 >> 1) + xoff;
        rect.y = card->v2 + yoff;
        rect.w = tim.pw;
        rect.h = tim.ph;
        LoadImage(&rect, tim.pixel);
        if ((tim.pmode >> 3) & 1) {
            rect.x = (card->clut & 0x3f) << 4;
            rect.y = card->clut >> 6;
            rect.w = tim.cw;
            rect.h = tim.ch;
            LoadImage(&rect, tim.clut);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenLoadCarPictures);
#endif

#ifdef NON_MATCHING
s16 screenGetNumCards(void)
{
    return screenNumCards[whichLevel];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetNumCards);
#endif

#ifdef NON_MATCHING
s16 screenGetLevelNumCards(s32 level)
{
    return screenNumCards[level];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenGetLevelNumCards);
#endif

#ifdef NON_MATCHING
void screenLoadCardsForBattleOptions(s32 twoPlayer, s32 car0, s32 car1)
{
    s32 cars[2];
    s32 count;
    s32 v;
    s32 tp;
    s32 i;
    s32 vv;
    POLY_FT4* p;

    count = 1;
    cars[0] = car0;
    cars[1] = car1;
    if (twoPlayer) {
        count = 2;
    }
    v = 0;
    tp = 11;
    p = gCards;
    for (i = 0; i < count; i++) {
        if (cars[i] != 0x82) {
            setPolyFT4(p);
            SetSemiTrans(p, 0);
            p->r0 = 127;
            p->g0 = 127;
            p->b0 = 127;
            gCards[i].x2 = -64;
            gCards[i].x3 = -64;
            gCards[i].y0 = -32;
            gCards[i].y1 = 32;
            gCards[i].y2 = -32;
            gCards[i].y3 = 32;
            gCards[i].x0 = 64;
            gCards[i].x1 = 64;
            vv = v;
            if (i == 4) {
                tp++;
                v = 0;
                vv = 0;
            }
            gCards[i].v0 = v;
            v += 64;
            gCards[i].v1 = vv + 63;
            gCards[i].v3 = vv + 63;
            gCards[i].tpage = tp + 144;
            gCards[i].u0 = 127;
            gCards[i].u1 = 127;
            gCards[i].u2 = 0;
            gCards[i].u3 = 0;
            gCards[i].v2 = vv;
            gCards[i].clut = ((i & 0x3ff) << 6) | 0x28;
        }
        p++;
    }
    setPolyFT4(&screen_sw);
    SetSemiTrans(&screen_sw, 0);
    screen_sw.r0 = 127;
    screen_sw.g0 = 127;
    screen_sw.b0 = 127;
    screenLoadCarPictures(count, cars);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenLoadCardsForBattleOptions);
#endif

#ifdef NON_MATCHING
void screenDrawCardsForBattleOptions(s32 twoPlayer)
{
    s32 x;

    DrawSync(0);
    x = 0;
    if (twoPlayer == 0) {
        x = 75;
    }
    x += 83;
    screen_sw.x0 = gCards[0].x0 + x;
    screen_sw.y0 = gCards[0].y0 + 97;
    screen_sw.x1 = gCards[0].x1 + x;
    screen_sw.y1 = gCards[0].y1 + 97;
    screen_sw.x2 = gCards[0].x2 + x;
    screen_sw.y2 = gCards[0].y2 + 97;
    screen_sw.x3 = gCards[0].x3 + x;
    screen_sw.y3 = gCards[0].y3 + 97;
    screen_sw.u0 = gCards[0].u0;
    screen_sw.u1 = gCards[0].u1;
    screen_sw.u2 = gCards[0].u2;
    screen_sw.u3 = gCards[0].u3;
    screen_sw.v0 = gCards[0].v0;
    screen_sw.v1 = gCards[0].v1;
    screen_sw.v2 = gCards[0].v2;
    screen_sw.v3 = gCards[0].v3;
    screen_sw.clut = gCards[0].clut;
    screen_sw.tpage = gCards[0].tpage;
    DrawPrim(&screen_sw);
    if (twoPlayer) {
        DrawSync(0);
        screen_sw.x0 = gCards[1].x0 + 234;
        screen_sw.y0 = gCards[1].y0 + 97;
        screen_sw.x1 = gCards[1].x1 + 234;
        screen_sw.y1 = gCards[1].y1 + 97;
        screen_sw.x2 = gCards[1].x2 + 234;
        screen_sw.y2 = gCards[1].y2 + 97;
        screen_sw.x3 = gCards[1].x3 + 234;
        screen_sw.y3 = gCards[1].y3 + 97;
        screen_sw.u0 = gCards[1].u0;
        screen_sw.u1 = gCards[1].u1;
        screen_sw.u2 = gCards[1].u2;
        screen_sw.u3 = gCards[1].u3;
        screen_sw.v0 = gCards[1].v0;
        screen_sw.v1 = gCards[1].v1;
        screen_sw.v2 = gCards[1].v2;
        screen_sw.v3 = gCards[1].v3;
        screen_sw.clut = gCards[1].clut;
        screen_sw.tpage = gCards[1].tpage;
        DrawPrim(&screen_sw);
    }
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDrawCardsForBattleOptions);
#endif

void screenDumpScreenToRam(s32 rightHalf, u_long* dest)
{
    RECT r;

    DrawSync(0);
    r.w = 320;
    r.h = 240;
    if (rightHalf != 0) {
        r.x = 320;
    } else {
        r.x = 0;
    }
    r.y = 0;
    StoreImage(&r, dest);
    DrawSync(0);
}

void screenLoadScreenFromRam(s32 rightHalf, u_long* src)
{
    RECT r;

    DrawSync(0);
    r.w = 320;
    r.h = 240;
    if (rightHalf != 0) {
        r.x = 320;
    } else {
        r.x = 0;
    }
    r.y = 0;
    LoadImage(&r, src);
    DrawSync(0);
}

void screenVramToScreen(s32 rightHalf, s32 slot)
{
    RECT r;
    s32 dx;

    DrawSync(0);
    r.w = 320;
    r.h = 240;
    dx = rightHalf != 0 ? 320 : 0;
    if (slot < 16) {
        r.x = slot << 6;
    } else {
        r.x = (slot - 16) << 6;
    }
    if (slot >= 16) {
        r.y = 256;
    } else {
        r.y = 0;
    }
    MoveImage(&r, dx, 0);
    DrawSync(0);
}

#ifdef NON_MATCHING
void screenVramToRam(s32 x, s32 y, s32 w, s32 h, u_long* dest)
{
    RECT r;
    u_long* buffer;

    do {
        buffer = dest;
    } while (0);
    DrawSync(0);
    r.w = w;
    r.h = h;
    r.x = x;
    r.y = y;
    do {
        StoreImage(&r, buffer);
    } while (0);
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenVramToRam);
#endif

#ifdef NON_MATCHING
void screenRamToVram(s32 x, s32 y, s32 w, s32 h, u_long* src)
{
    RECT r;
    u_long* buffer;

    do {
        buffer = src;
    } while (0);
    DrawSync(0);
    r.w = w;
    r.h = h;
    r.x = x;
    r.y = y;
    do {
        LoadImage(&r, buffer);
    } while (0);
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenRamToVram);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBDDC);

#ifdef NON_MATCHING
s32 screenEnterAccessCode(void)
{
    s32 w;
    s32 h;
    s32 adv;
    u_long* buf;
    char* enter;
    char* password;
    s32 ch;
    s32 pos;
    s32 changed;
    s32 newChar;

    ch = ' ';
    buf = (u_long*)rtGetDoubleBufferArea();
    screenSetEnv();
    fileioLoadFileIntoRam(buf, "UASCREEN\\SCRNFD.TIM");
    fontSetColor(0, 55, 88, 127);
    VSync(0);
    screenDisplayImage(1, buf, 0, 0);
    enter = "ENTER";
    fontPrintCenteredXY(0, 159, 113, enter);
    password = "PASSWORD";
    fontPrintCenteredXY(0, 159, 133, password);
    screenResetCursor();
    pos = 0;
fill:
    gPassword[pos] = ch;
    pos++;
    if (pos < 5) {
        goto fill;
    }
    changed = 0;
    newChar = 0;
    gPassword[5] = 0;
    fontGetCharacterInfo(5, '=', &w, &h, &h, &adv);
    pos = 0;
    adv += w;
    fontSetCursor(108, 183);
    UpdCtlPad();
    while (ctlpadAnyKey(0)) {
        VSync(0);
        UpdCtlPad();
        screenBlinkCursor(5);
    }
    for (;;) {
        VSync(0);
        screenBlinkCursor(5);
        UpdCtlPad();
        ctlpadAnyKey(0);
        if (GetCtlPad(11, 0)) {
            newChar = 1;
            uasoundPlayMenuSelectFlush();
            ch = '<';
            changed = 1;
        } else if (GetCtlPad(13, 0)) {
            newChar = 1;
            uasoundPlayMenuSelectFlush();
            ch = '=';
            changed = 1;
        } else if (GetCtlPad(14, 0)) {
            newChar = 1;
            uasoundPlayMenuSelectFlush();
            ch = '@';
            changed = 1;
        } else if (GetCtlPad(12, 0)) {
            newChar = 1;
            uasoundPlayMenuSelectFlush();
            ch = '>';
            changed = 1;
        } else if (ctlpadAnyLeft(0)) {
            uasoundPlayMenuCycleFlush();
            if (pos == 0) {
                pos = 4;
            } else {
                pos = pos - 1;
            }
            changed = 1;
        } else if (ctlpadAnyRight(0)) {
            uasoundPlayMenuCycleFlush();
            if (pos == 4) {
                pos = 0;
            } else {
                pos = pos + 1;
            }
            changed = 1;
        } else if (screenWaitForContinue(0, 0)) {
            break;
        } else if (ctlpadAnyKey(0)) {
            soundProcessIds();
        }
        if (changed) {
            if (newChar) {
                gPassword[pos] = ch;
                if (pos == 4) {
                    pos = 0;
                } else {
                    pos = pos + 1;
                }
                newChar = 0;
            }
            VSync(0);
            screenDisplayImage(1, buf, 0, 0);
            fontPrintCenteredXY(0, 159, 113, enter);
            fontPrintCenteredXY(0, 159, 133, password);
            fontPrintXY(5, 107, 183, gPassword);
            fontSetCursor(pos * adv + 108, 183);
            UpdCtlPad();
            while (ctlpadAnyKey(0)) {
                VSync(0);
                UpdCtlPad();
                screenBlinkCursor(5);
            }
            changed = 0;
        }
    }
    screenEraseCursor();
    fontSetDefaultColor(0);
    return screenAccessCodeFromPassword(gPassword);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenEnterAccessCode);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBDF0);

#ifdef NON_MATCHING
s32 screenWrongPassword(s32 unused_state)
{
    char* strs[2] = { "TRY AGAIN?", "EXIT" };
    s32 fonts[2];
    DRAWENV env;
    s32 ret;
    s32 choice;
    s32 frames;
    s32 prev;
    u_long* buf;
    char* wrong;
    s32* pf;
    s32* pf2;
    s32* lo;

    buf = (u_long*)rtGetDoubleBufferArea();
    fonts[0] = 0;
    fonts[1] = 1;
    screenSetEnv();
    VSync(0);
    screenDisplayImage(1, buf, 0, 0);
    fontSetColor(fonts[0], 55, 88, 127);
    fontSetColor(fonts[1], 100, 100, 100);
    wrong = "WRONG!";
    fontPrintCenteredXY(0, 159, 95, wrong);
    ret = 6;
    fontPrintCenteredXY(fonts[1], 159, 133, strs[0]);
    choice = 0;
    fontPrintCenteredXY(fonts[0], 159, 153, strs[1]);
    fontPrintXY(5, 107, 183, gPassword);
    while (ctlpadAnyKey(2)) {
        UpdCtlPad();
    }
    frames = 0;
    for (;;) {
        VSync(0);
        frames++;
        UpdCtlPad();
        prev = choice;
        if (ctlpadAnyUp(0)) {
            frames = 0;
            if (choice == 0) {
                choice = 1;
            } else {
                choice = choice - 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnyDown(0)) {
            frames = 0;
            if (choice == 1) {
                choice = 0;
            } else {
                choice = choice + 1;
            }
            uasoundPlayMenuCycleFlush();
        } else if (ctlpadAnySelect(0)) {
            uasoundPlayMenuSelectFlush();
            switch (choice) {
            case 0:
                ret = 22;
                goto out;
            case 1:
                ret = 6;
                goto out;
            }
            goto out;
        } else if (frames >= 1201) {
            goto out;
        }
        if (prev != choice) {
            screenDisplayImage(0, buf, 0, 0);
            SetDefDrawEnv(&env, 0, 0, 320, 240);
            PutDrawEnv(&env);
            fontPrintCenteredXY(0, 159, 95, wrong);
            lo = fonts;
            pf = (choice == 0) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf, 159, 133, strs[0]);
            pf2 = (choice == 1) ? fonts + 1 : lo;
            fontPrintCenteredXY(*pf2, 159, 153, strs[1]);
            fontPrintXY(5, 107, 183, gPassword);
            PutDrawEnv(&gScreenDrawEnv);
            VSync(0);
            screenDisplayToDisplay(0, 1);
        }
        while (ctlpadAnyKey(0)) {
            UpdCtlPad();
        }
    }
out:
    fontSetDefaultColor(fonts[0]);
    fontSetDefaultColor(fonts[1]);
    return ret;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenWrongPassword);
#endif

char* screenPasswordForLevel(s32 level)
{
    switch (level) {
    case 1:
        return screenPasswords[1];
    case 2:
        return screenPasswords[2];
    case 3:
        return screenPasswords[3];
    case 4:
        return screenPasswords[4];
    case 5:
        return screenPasswords[5];
    case 6:
        return screenPasswords[13];
    }
    return screenPasswords[0];
}

s32 screenAccessCodeFromPassword(char* password)
{
    u32 i;

    if (*password != 0) {
        for (i = 0; i < 15; i++) {
            if (strcmp(password, screenPasswords[i]) == 0) {
                return i;
            }
        }
    }
    return -1;
}

#ifdef NON_MATCHING
char* screenPasswordFromAccessCode(s32 code)
{
    return screenPasswords[code];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenPasswordFromAccessCode);
#endif

#ifdef NON_MATCHING
void screenBlinkCursor(s32 font)
{
    s32 x;
    s32 y;
    s32 cx;
    s32 cy;

    gCursorCount = (s32)((u32)gCursorCount + 1u);
    if (gCursorCount >= 20) {
        gCursorOn = gCursorOn == 0;
        if (gCursorOn != 0) {
            fontGetCursor(&x, &y);
            do {
                cx = x;
                cy = y;
            } while (0);
            gCursorRect.x = (s32)((u32)cx + 320u);
            gCursorRect.y = cy;
            StoreImage(&gCursorRect, gCursorImageBuf);
            fontPrint(font, "_");
            fontSetCursor(x, y);
        } else {
            screenEraseCursor();
        }
        gCursorCount = 0;
        DrawSync(0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenBlinkCursor);
#endif

#ifdef NON_MATCHING
void screenEraseCursor(void)
{
    LoadImage(&gCursorRect, gCursorImageBuf);
    DrawSync(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenEraseCursor);
#endif

void screenResetCursor(void)
{
    gCursorOn = 0;
    gCursorCount = 0;
}

#ifdef NON_MATCHING
s32 screenWaitForContinue(s32 frames, s32 limit)
{
    s32 i;
    s32 pad;

    i = 0;
    pad = 2 * (rtIsSplitScreenOn() != 0);
    do {
        do {
            if (limit > 0) {
                VSync(0);
            }
            i++;
        } while (i < frames);
        UpdCtlPad();
        if (ctlpadAnySelect(pad)) {
            return 1;
        }
    } while (limit < 0 || limit >= i);
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenWaitForContinue);
#endif

void SetDefaultEndingParameters(void)
{
    gEyePoint.vx = 40;
    gEyePoint.vy = 0;
    gEyePoint.vz = 48;
    gEyeRot.vx = -102;
    gEyeRot.vy = 0;
    gEyeRot.vz = -22;
    gDeltaRot.vx = 0;
    gDeltaRot.vy = 0;
    gDeltaRot.vz = 0;
    gDeltaTrans.vx = 0;
    gDeltaTrans.vy = 8;
    gDeltaTrans.vz = 0;
    gStartLocation = 240;
}

void SetOutlawEndingParameters(void)
{
    gEyePoint.vx = 0;
    gEyePoint.vy = -240;
    gEyePoint.vz = 128;
    gEyeRot.vx = 0;
    gEyeRot.vy = 0;
    gEyeRot.vz = 0;
    gDeltaRot.vx = 11;
    gDeltaRot.vy = 22;
    gDeltaRot.vz = 11;
    gDeltaTrans.vx = 0;
    gDeltaTrans.vy = 2;
    gDeltaTrans.vz = 0;
    gStartLocation = -1280;
}

void SetRoadKillEndingParameters(void)
{
    gEyePoint.vx = 0;
    gEyePoint.vy = 0;
    gEyePoint.vz = 112;
    gEyeRot.vx = 79;
    gEyeRot.vy = 0;
    gEyeRot.vz = -34;
    gDeltaRot.vx = 0;
    gDeltaRot.vy = 0;
    gDeltaRot.vz = 0;
    gDeltaTrans.vx = 0;
    gDeltaTrans.vy = 16;
    gDeltaTrans.vz = 0;
    gStartLocation = -480;
}

#ifdef NON_MATCHING
void screenCarEnding(s32 which)
{
    CarStats e;
    Cs* cs;
    Db* db;
    Db* cur;
    s32 side;
    s32 saved;

    cs = UAGetCs(which);
    if (cs != 0) {
        UASetPlayerCar(0, which, 1);
        uaInitDB(0);
        CarInitDeltas(&e, which);
        cs->pos.vx = gDeltaTrans.vx;
        cs->pos.vz = e.unk6C;
        cs->pos.vy = gStartLocation;
        db = rtGetDb();
        cur = rtGetCdb();
        saved = db[0].draw.isbg;
        db[0].draw.isbg = 1;
        db[1].draw.isbg = 1;
        screenVramToScreen(0, 22);
        DrawSync(0);
        VSync(0);
        screenDisplayToDisplay(0, 1);
        viewSetDeltaTrans(&gEyePoint, 0);
        viewSetDeltaRot(&gEyeRot, 0);
        while (cs->pos.vy < 4000) {
            if (cur == db) {
                cur = &db[1];
                side = 1;
            } else {
                cur = db;
                side = 0;
            }
            cur->unk8 = (s32)cur->area;
            VSync(0);
            DrawSync(0);
            ClearOTagR(((u_long*)cur->small), 0x1000);
            cs->rot.vx += gDeltaRot.vx;
            cs->rot.vy += gDeltaRot.vy;
            cs->rot.vz += gDeltaRot.vz;
            RotMatrixYXZ(&cs->rot, &cs->mat);
            cs->pos.vy += gDeltaTrans.vy;
            viewProc(0);
            hierProcOne(cur, cs);
            PutDrawEnv(&cur->draw);
            PutDispEnv(&cur->disp);

            screenDisplayImage(side, AUDIO_IMAGE, 0, 0);
            DrawOTag(((u_long*)cur->small) + 4095);
        }
        DrawSync(0);
        db[0].draw.isbg = saved;
        db[1].draw.isbg = saved;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCarEnding);
#endif

#ifdef NON_MATCHING
void screenCarEndingInit(ScreenCarEnding* e, s32 car, u_long* image)
{
    e->cs = UAGetCs(car);
    if (e->cs != 0) {
        e->imageRight = 0;
        e->image = image;
        UASetPlayerCar(0, car, 1);
        uaInitDB(0);
        CarInitDeltas(&e->stats, car);
        e->cs->pos.vx = gDeltaTrans.vx;
        e->cs->pos.vz = e->stats.unk6C;
        e->cs->pos.vy = gStartLocation;
        e->db = rtGetDb();
        e->cdb = rtGetCdb();
        e->savedIsBg = e->db->draw.isbg;
        e->db[0].draw.isbg = 1;
        e->db[1].draw.isbg = 1;
        viewSetDeltaTrans(&gEyePoint, 0);
        viewSetDeltaRot(&gEyeRot, 0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCarEndingInit);
#endif

#ifdef NON_MATCHING
void screenCarEndingReset(ScreenCarEnding* e)
{
    if (e->cdb == e->db) {
        e->cdb = e->db + 1;
        e->imageRight = 1;
    } else {
        e->cdb = e->db;
        e->imageRight = 0;
    }
    e->cdb->unk8 = (s32)e->cdb->area;
    VSync(0);
    DrawSync(0);
    ClearOTagR(((u_long*)e->cdb->small), 0x1000);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCarEndingReset);
#endif

#ifdef NON_MATCHING
void screenCarEndingDrawCar(ScreenCarEnding* e)
{
    e->cs->rot.vx += gDeltaRot.vx;
    e->cs->rot.vy += gDeltaRot.vy;
    e->cs->rot.vz += gDeltaRot.vz;
    RotMatrixYXZ(&e->cs->rot, &e->cs->mat);
    e->cs->pos.vy += gDeltaTrans.vy;
    viewProc(0);
    hierProcOne(e->cdb, e->cs);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCarEndingDrawCar);
#endif

#ifdef NON_MATCHING
void screenCarEndingDisplayCar(ScreenCarEnding* e)
{
    PutDrawEnv(&e->cdb->draw);
    PutDispEnv(&e->cdb->disp);
    if (e->image != 0) {
        screenDisplayImage(e->imageRight, e->image, 0, 0);
    }
    DrawOTag(((u_long*)e->cdb->small) + 4095);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenCarEndingDisplayCar);
#endif

void screenCarEndingCleanup(ScreenCarEnding* e)
{
    DrawSync(0);
    e->db[0].draw.isbg = e->savedIsBg;
    e->db[1].draw.isbg = e->savedIsBg;
}

void screenPlayAllCarEndings(void)
{
    s32 i;

    for (i = 0; i < 12; i++) {
        screenPlayCarEnding(GetCarNameFromCarNum(i));
    }
}

void screenShowWarhawkDemo(void)
{
    soundInterruptPlayDA();
    VSync(0);
    screenSetEnv();
    fileioLoadFileIntoRam(rtGetDoubleBufferArea(), "UASCREEN\\SCRNALSO.TIM");
    screenDisplayImage(0, (u_long*)rtGetDoubleBufferArea(), 0, 0);
    screenDisplayToDisplay(0, 1);
    screenWaitForContinue(120, 120);
    screenPlayCinema(3);
}

#ifdef NON_MATCHING
void screenScrollTextInit(
    ScrollText* st, char* textFile, char* textBuf, char* bgFile, u_long* imageBuf, s32 font)
{
    char* p;
    s16 pad;

    pad = 2 * (rtIsSplitScreenOn() != 0);
    while (GetPadStatus(pad) != 0) {
        UpdCtlPad();
    }
    st->image = imageBuf;
    st->text = textBuf;
    soundInterruptPlayDA();
    fileioLoadFileIntoRam(st->image, bgFile);
    fileioLoadFileIntoRam(st->text, textFile);
    soundResumePlayDA();
    st->mode = 0;
    p = st->text;
    st->numLines = atoi(p);
    st->stopY = st->numLines;
    while (*p != ' ') {
        p++;
    }
    p++;
    switch (*p) {
    case 'L':
    case 'l':
        st->mode = 0;
        break;
    case 'C':
    case 'c':
        st->mode = 2;
        break;
    }
    st->line = 0;
    while (st->line < st->numLines) {
        while (*p != '\n') {
            p++;
        }
        *p = 0;
        p++;
        if (strncmp(p, "@S", 2) == 0) {
            st->stopY = st->line - 7;
        } else {
            st->lines[st->line] = p;
            st->line++;
        }
    }
    while (*p != '\n') {
        p++;
    }
    *p = 0;
    st->font = font;
    fontGetCharacterInfo(st->font, 'M', &st->charWidth, &st->spacing, &st->unk41C, &st->unk420);
    st->bottom = st->numLines * st->spacing + (st->numLines - 1) * 10 + 241;
    st->stopY = st->stopY * st->spacing + (st->stopY - 1) * 10 + 241;
    SetDefDrawEnv(&st->draw, 0, 0, 320, 240);
    PutDrawEnv(&st->draw);
    DrawSync(0);
    VSync(0);
    st->y0 = st->spacing + 240;
    st->spacing = st->spacing + 10;
    st->speed = 3;
    st->tick = 0;
    st->delay = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenScrollTextInit);
#endif

void screenScrollTextPrintOneLine(ScrollText* st)
{
    s32 line;
    s32 font;
    s32 y;

    if (st->mode == 2) {
        do {
            line = st->line;
            font = st->font;
            y = st->y;
            fontPrintCenteredXY(font, 159, y, st->lines[line]);
        } while (0);
    } else {
        do {
            line = st->line;
            font = st->font;
            y = st->y;
            fontPrintXY(font, 15, y, st->lines[line]);
        } while (0);
    }
}

#ifdef NON_MATCHING
s32 screenScrollTextUpdate(ScrollText* st)
{
    if (st->speed < 6 && st->tick >= st->speed) {
        st->y0 = st->y0 - 1;
        st->bottom = st->bottom - 1;
        st->stopY = st->stopY - 1;
        st->tick = 0;
    }
    st->tick = st->tick + 1;
    return st->tick;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenScrollTextUpdate);
#endif

#ifdef NON_MATCHING
s32 screenScrollTextProcessButtonPress(ScrollText* st)
{
    s32 pad;
    s32 result;

    result = 0;
    pad = 2 * (rtIsSplitScreenOn() != 0);
    if (ctlpadAnyUp(pad) && st->delay == 0) {
        st->delay = 30;
        if (st->speed >= 2) {
            st->speed--;
        }
    } else if (ctlpadAnyDown(pad) && st->delay == 0) {
        st->delay = 30;
        if (st->speed < 6) {
            st->speed++;
        }
    } else if (screenWaitForContinue(0, 0)) {
        result = 1;
    } else if (st->delay > 0) {
        st->delay--;
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenScrollTextProcessButtonPress);
#endif

#ifdef NON_MATCHING
void screenScrollTextOneScreenFull(ScrollText* st)
{
    st->y = st->y0;
    st->line = 0;
    goto test;
    do {
        if (st->y >= 0 && st->spacing + 240 >= st->y) {
            screenScrollTextPrintOneLine(st);
        }
        st->line++;
        st->y += st->spacing;
    test:;
    } while (st->line < st->numLines);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenScrollTextOneScreenFull);
#endif

s32 screenScrollText(char* textFile, char* bgFile, s32 font)
{
    ScrollText st;
    s16 pad;
    void* small;

    pad = 2 * (rtIsSplitScreenOn() != 0);
    while (GetPadStatus(pad) != 0) {
        UpdCtlPad();
    }
    DrawSync(0);
    VSync(0);
    screenSetEnv();
    small = rtGetSmallBufferArea();
    screenScrollTextInit(&st, textFile, small, bgFile, (u_long*)rtGetDoubleBufferArea(), font);
    while (st.bottom > 0) {
        screenDisplayImage(0, st.image, 0, 0);
        screenScrollTextOneScreenFull(&st);
        screenScrollTextUpdate(&st);
        VSync(0);
        screenDisplayToDisplay(0, 1);
        if (screenScrollTextProcessButtonPress(&st)) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE14);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE2C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE40);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE54);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE68);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE7C);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBE94);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBEA4);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBEBC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBED4);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBEEC);

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBF04);

#ifdef NON_MATCHING
s32 screenPlayCarEnding(s32 carId)
{
    char storyName[32];
    ScrollText credits;
    ScreenCarEnding ending;
    s32 pad;
    s32 passwordChoice;
    char* password;
    char* background;
    s32 signatureCountdown;
    s32 track;
    s32 musicStarted;
    s32 interrupted;
    s32 done;

    musicStarted = 0;
    interrupted = 0;
    pad = 2 * (rtIsSplitScreenOn() != 0);
    while (GetPadStatus((s16)pad) != 0)
        UpdCtlPad();
    shellPlayCarEngineRev();
    DrawSync(0);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    screenSetEnv();
    screenGrayedScreen();
    fileioLoadFileIntoRam(TITLE_IMAGE, "UASCREEN\\SCRNCALY.TIM");
    VSync(0);
    screenDisplayImage(1, TITLE_IMAGE, 0, 0);
    fontPrintCenteredXY(6, 159, 70, "CONGRATULATIONS!");
    fontPrintCenteredXY(6, 159, 110, "You shall have a ");
    fontPrintCenteredXY(6, 159, 125, "special password");
    passwordChoice = rand() & 3;
    switch (passwordChoice) {
    case 0:
        fontPrintCenteredXY(6, 159, 175, "Infinite Weapons");
        password = screenPasswords[7];
        break;
    case 1:
        fontPrintCenteredXY(6, 159, 175, "Minion and Friends");
        password = screenPasswords[11];
        break;
    case 2:
        fontPrintCenteredXY(6, 159, 175, "Arena with 5 Opponents");
        password = screenPasswords[13];
        break;
    default:
        fontPrintCenteredXY(6, 159, 175, "Helicopter View");
        password = screenPasswords[14];
        break;
    }
    fontPrintCenteredXY(5, 159, 205, password);
    shellLoadCarsEndDatabase();
    while (ctlpadAnyKey(pad))
        UpdCtlPad();
    screenWaitForContinue(0, 120);
    screenWaitForContinue(0, 120);
    screenWaitForContinue(0, 120);
    screenWaitForContinue(0, 120);
    screenWaitForContinue(0, 120);
    shellStopCarEngineRev();
    sprintf(storyName, "UATEXT\\CAREND%02d.TXT", GetCarNumFromCarName(carId) + 1);

    if (carId == 50) {
        background = "UASCREEN\\SCRNCOP.TIM";
        signatureCountdown = 720;
        SetOutlawEndingParameters();
        track = 8;
    } else if (carId == 120) {
        background = "UASCREEN\\SCRNROAD.TIM";
        signatureCountdown = 60;
        SetRoadKillEndingParameters();
        track = 2;
    } else {
        background = "UASCREEN\\SCRNLARO.TIM";
        signatureCountdown = 30;
        SetDefaultEndingParameters();
        track = 1;
    }
    screenScrollTextInit(
        &credits, "UATEXT\\CREDITS.TXT", TEXT_BUFFER, background, OPTIONS_IMAGE, 6);
    screenCarEndingInit(&ending, carId, OPTIONS_IMAGE);
    done = 0;
    do {
        screenCarEndingReset(&ending);
        PutDrawEnv(&ending.cdb->draw);
        PutDispEnv(&ending.cdb->disp);
        screenDisplayImage(ending.imageRight, credits.image, 0, 0);
        if (!musicStarted) {
            musicStarted = 1;
            if (track != 1)
                soundStartPlayDA(track);
        }
        screenCarEndingDrawCar(&ending);
        screenCarEndingDisplayCar(&ending);
        if (signatureCountdown == 0) {
            shellPlayCarEngineRev();
            uasoundPlayCarSignature(carId, 0, 0);
            soundProcessIds();
        }
        --signatureCountdown;
        screenScrollTextOneScreenFull(&credits);
        screenScrollTextUpdate(&credits);
        if (screenScrollTextProcessButtonPress(&credits)) {
            interrupted = 1;
            done = 1;
        }
        if (credits.bottom == 0)
            done = 1;
    } while (!done);
    screenDisplayImage(0, credits.image, 0, 0);
    screenDisplayImage(1, credits.image, 0, 0);
    screenCarEndingCleanup(&ending);
    return interrupted;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenPlayCarEnding);
#endif

void screenDisplaySonyLegalScreen(void)
{
    VSync(0);
    screenSetEnv();
    fileioLoadFileIntoRam(rtGetDoubleBufferArea(), "UASCREEN\\SISAPROD.TIM");
    screenDisplayImage(1, (u_long*)rtGetDoubleBufferArea(), 0, 0);
    VSync(140);
}

void screenDisplayDeveloperScreen(void)
{
    VSync(0);
    screenSetEnv();
    fileioLoadFileIntoRam(rtGetDoubleBufferArea(), "UASCREEN\\DEVELOP.TIM");
    screenDisplayImage(1, (u_long*)rtGetDoubleBufferArea(), 0, 0);
    VSync(140);
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBF48);

#ifdef NON_MATCHING
void screenShowAccessCodes(void)
{
    s32 pad;

    pad = 2 * (rtIsSplitScreenOn() != 0);
    VSync(0);
    screenDisplayToDisplay(0, 1);
    screenSetEnv();
    screenGrayedScreen();
    if (shellIsPerfect()) {
        fontPrintCenteredXY(6, 159, 50, "PASSWORDS");
        fontPrintCenteredXY(6, 159, 175, "Infinite Weapons");
        fontPrintCenteredXY(5, 159, 205, screenPasswords[7]);
    } else {
        fontPrintCenteredXY(6, 159, 50, "PASSWORD");
    }
    fontPrintCenteredXY(6, 159, 110, "Helicopter View");
    fontPrintCenteredXY(5, 159, 140, screenPasswords[14]);
    while (ctlpadAnyKey(pad)) {
        UpdCtlPad();
    }
    screenWaitForContinue(60, 300);
    screenDisplayToDisplay(0, 1);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenShowAccessCodes);
#endif

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBF54);

#ifdef NON_MATCHING
s32 screenDisplayHistory(void)
{
    return screenScrollText("UATEXT\\HISTORY.TXT", "UASCREEN\\SCRNCALY.TIM", 6);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayHistory);
#endif

#ifdef NON_MATCHING
s32 screenDisplayCredits(void)
{
    return screenScrollText("UATEXT\\CREDITS.TXT", "UASCREEN\\SCRNROAD.TIM", 6);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenDisplayCredits);
#endif

void screenShowAccessCodeForLevel(s32 level, s32 x, s32 y)
{
    char buf[56];
    char* password;

    password = 0;
    switch (level) {
    case 1:
        password = screenPasswordFromAccessCode(1);
        break;
    case 2:
        password = screenPasswordFromAccessCode(2);
        break;
    case 3:
        password = screenPasswordFromAccessCode(3);
        break;
    case 4:
        password = screenPasswordFromAccessCode(4);
        break;
    case 5:
        password = screenPasswordFromAccessCode(5);
        break;
    case 6:
        password = screenPasswordFromAccessCode(13);
        break;
    }
    if (password != 0) {
        sprintf(buf, "%s PASSWORD", shellGetLevelName(level));
        fontPrintCenteredXY(6, x, y, buf);
        fontPrintCenteredXY(5, x, y + 35, password);
    }
}

INCLUDE_RODATA("asm/nonmatchings/tm1/screen", D_800FBF74);

#ifdef NON_MATCHING
void screenWonSpecialLevel(void)
{
    screenGrayedScreen();
    if (shellIsPerfect()) {
        fontPrintCenteredXY(6, 159, 102, "God Mode");
        fontPrintCenteredXY(5, 159, 153, screenPasswords[8]);
    }
    while (GetPadStatus(2 * (rtIsSplitScreenOn() != 0)) != 0) {
        UpdCtlPad();
    }
    if (screenWaitForContinue(60, 300) == 1) {
        uasoundPlayMenuSelectFlush();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/screen", screenWonSpecialLevel);
#endif

s32 screenGetWhichPad(void)
{
    UpdCtlPad();
    return (GetPadStatus(2) & 0xffff0000) != 0;
}

void screenDisplayWarningScreen(void)
{
    fileioLoadFileIntoRam(rtGetDoubleBufferArea(), "UASCREEN\\SCEAPRES.TIM");
    VSync(0);
    screenSetEnv();
    screenDisplayImage(1, (u_long*)rtGetDoubleBufferArea(), 0, 0);
}
