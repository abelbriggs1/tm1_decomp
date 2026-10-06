#include "common.h"

#include <libgpu.h>

#include "tm1/hier.h"
#include "tm1/hud.h"
#include "tm1/rt.h"
#include "tm1/ua.h"
#include "tm1/ua_effect.h"
#include "tm1/weapon.h"

// TODO: Give proper names.
s32 D_8018BF18 = 0; /* numPendingReturns */
s32 D_8018BF1C = 0; /* pendingShell */
s32 D_8018BF20 = 0; /* rearViewOn */
s32 D_8018BF24 = 0; /* rearViewEnabled */
s32 split_screen = 0;
s32 D_8018BF2C = 20; /* read only by rtMainLoop */
s32 D_8018BF30 = 0; /* fieldCounter */

static Db* cdb;
static Db* curSdb;
static Db* curRdb;

#ifdef NON_MATCHING
s32 rtMainLoop(void)
{
    Db* p;
    u32 t;
    u32 n;
    s32 r;

    rtInitTweek();
    t = GetCurTics();
    cdb = db;
    if (split_screen) {
        curSdb = sdb;
    }
    if (D_8018BF20) {
        curRdb = rdb;
    }
    PlayGame();
    while (rtTimeToReturnToShell() == 0) {
        GetCurTics();
        p = db;
        if (cdb == db) {
            p = &db[1];
        }
        p->unk8 = (s32)p->area;
        cdb = p;
        ClearOTagR((unsigned int*)p->small, 0x1000);
        UpdCtlPad();
        move_missiles();
        UAeffectUpdateSpecialEffects();
        viewProc(0);
        display_bullets(cdb, 0);
        update_bullets();
        animate_explosions();
        draw_explosions(cdb, 0);
        hudDisplayRadar((u32*)cdb->small);
        hierProc(cdb, 0);
        PlayGame();
        if (split_screen) {
            p = sdb;
            if (curSdb == sdb) {
                p = &sdb[1];
            }
            curSdb = p;
            ClearOTagR((unsigned int*)p->small, 0x1000);
            viewProc(1);
            curSdb->unk8 = cdb->unk8;
            display_bullets(curSdb, 1);
            draw_explosions(curSdb, 1);
            hierProc(curSdb, 1);
        } else if (D_8018BF20) {
            p = rdb;
            if (curRdb == rdb) {
                p = &rdb[1];
            }
            curRdb = p;
            ClearOTagR((unsigned int*)p->small, 0x1000);
            viewProc(1);
            curRdb->unk8 = cdb->unk8;
            display_bullets(curRdb, 1);
            draw_explosions(curRdb, 1);
            hierProc(curRdb, 1);
        }
        DrawSync(0);
        do {
            VSync(0);
            n = GetCurTics();
            r = FrameTimeToUpdateRate(n - t);
        } while (D_8018BF2C < r);
        rtUpdateFieldCounter(GetFieldsLastFrame());
        SetFrameStart();
        t = GetCurTics();
        PutDrawEnv(&cdb->draw);
        PutDispEnv(&cdb->disp);
        DrawOTag((unsigned int*)(cdb->small + 16380));
        UAdashDrawInstruments(0, &cdb->draw);
        if (split_screen) {
            PutDrawEnv(&curSdb->draw);
            DrawOTag((unsigned int*)(curSdb->small + 16380));
            UAdashDrawInstruments(1, &sdb[0].draw);
        } else if (D_8018BF20) {
            PutDrawEnv(&curRdb->draw);
            DrawOTag((unsigned int*)(curRdb->small + 16380));
        }
        screenCheckMidGameOptions();
    }
    return D_8018BF1C;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/rt", rtMainLoop);
#endif

void rtReturnToShell(s32 arg, s32 mode)
{
    if (D_8018BF18 < 4) {
        gReturnsPending[D_8018BF18].mode = arg;
        gReturnsPending[D_8018BF18].arg = mode;
        D_8018BF18 = D_8018BF18 + 1;
    }
}

void rtClearPendingReturns(void)
{
    s32 i;
    s32 n;
    s32 neg;

    do {
        n = D_8018BF18;
    } while (0);
    i = 0;
    do {
        neg = -1;
    } while (0);
    for (; i < 4; i++) {
        gReturnsPending[n].mode = 0;
        gReturnsPending[n].arg = neg;
    }
    D_8018BF18 = 0;
    D_8018BF1C = 0;
}

s32 rtTimeToReturnToShell(void)
{
    s32 i;
    s32 found;
    s32 t;
    s32 a;
    s16 f;

    if (D_8018BF18 == 0) {
        return 0;
    }
    found = -1;
    for (i = 0; i < D_8018BF18; i++) {
        f = GetFieldsLastFrame();
        do {
            a = gReturnsPending[i].arg;
        } while (0);

        t = a - f;

        gReturnsPending[i].arg = t;
        if (t < 0) {
            found = i;
        }
    }
    if (found == -1) {
        return 0;
    }
    D_8018BF1C = gReturnsPending[found].mode;
    for (i = found + 1; i < D_8018BF18; i++) {
        gReturnsPending[i - 1].mode = gReturnsPending[i].mode;
        gReturnsPending[i - 1].arg = gReturnsPending[i].arg;
    }
    D_8018BF18--;
    return 1;
}

void rtUpdateFieldCounter(s32 n)
{
    D_8018BF30 += n;
}

void rtResetFieldCounter(void)
{
    D_8018BF30 = 0;
}

s32 rtGetFieldCounter(void)
{
    return D_8018BF30;
}

Db* rtGetDb(void)
{
    return db;
}

s32 rtWhichDisplayBuffer(void)
{
    return cdb == db;
}

s32 rtWhichDrawBuffer(void)
{
    return cdb != db;
}

Db* rtGetCdb(void)
{
    return cdb;
}

void rtSetCdb(s32 which)
{
    cdb = db;
    if (which) {
        cdb = &db[1];
    }
}

s32 rtWhichBuffer(void)
{
    return cdb != db;
}

u8* rtGetDoubleBufferArea(void)
{
    return gDoubleBuffers;
}

u8* rtGetSmallBufferArea(void)
{
    return D_801E4C50;
}

void rtDoubleBufferInit(void)
{
    s32 n;

    n = 0x16B70;
    db[0].area = gDoubleBuffers;
    db[1].area = D_801CE090;
    db[0].small = D_801E4C50;
    db[1].small = D_801E8C50;
    db[0].areaEnd = gDoubleBuffers + n;
    db[1].areaEnd = D_801CE090 + n;
    cdb = db;
    sdb[0].small = D_801ECC50;
    sdb[1].small = D_801F0C50;
    sdb[0].area = gDoubleBuffers;
    sdb[1].area = D_801CE090;
    sdb[0].areaEnd = gDoubleBuffers + n;
    sdb[1].areaEnd = D_801CE090 + n;
    curSdb = sdb;
    rdb[0].area = gDoubleBuffers;
    rdb[1].area = D_801CE090;
    rdb[0].small = D_801ECC50;
    rdb[1].small = D_801F0C50;
    rdb[0].areaEnd = gDoubleBuffers + n;
    rdb[1].areaEnd = D_801CE090 + n;
    curRdb = rdb;
}

Db* rtGetSplitScreenDb(void)
{
    return sdb;
}

Db* rtGetRearViewDb(void)
{
    return rdb;
}

void rtSetRearView(s32 on)
{
    if (on == 1) {
        D_8018BF24 = on;
    } else {
        D_8018BF24 = 0;
    }
}

void rtToggleRearView(void)
{
    Db* p;
    s32 t;
    Db* cur;

    if (D_8018BF24 != 0) {
        do {
            t = D_8018BF20;
            cur = cdb;
        } while (0);
        p = rdb;
        D_8018BF20 = (t == 0);
        if (cur == db) {
            p = &rdb[1];
        }
        curRdb = p;
    }
}

s32 rtIsRearViewOn(void)
{
    return D_8018BF20;
}

s32 rtIsSplitScreenOn(void)
{
    return split_screen;
}

void rtSetSplitScreen(s32 on)
{
    split_screen = on;
}

void rtInitTweek(void)
{
    // Dev note: `tweak`s in SingleTrac/Incognito's projects seem to refer to
    // debug settings/display/toggles that change game variables.
}
