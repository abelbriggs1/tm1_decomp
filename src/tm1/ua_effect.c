#include "common.h"

#include <libgpu.h>
#include <libgte.h>
#include <rand.h>

#include "tm1/cs.h"
#include "tm1/light.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sw.h"

s32 gNumTVs = 0;
s32 gTruckWheelAngle = 0;
s32 gHitLightFlash = 0;
s32 gLightningTick = 0;
s32 gLightningPeriod = 0;
s32 gLightningOn = 0;

u16 gTVsTPage;
u16 D_8018C5FA;
s32 gLightningUoffset;
s32 gLightningVoffset;

WheelNode* gTruckWheel[4];
s32 gCarSpeed[15];
HitLight gHitLight[6];
u8* gTVpolyUVs[4];

u8* gLightningPart[4] = { 0, 0, 0, 0 };

u8 gLightningUVs[21][4][2] = {
    { { 0, 0 }, { 0, 7 }, { 63, 0 }, { 63, 7 } },
    { { 0, 8 }, { 0, 15 }, { 63, 8 }, { 63, 15 } },
    { { 0, 16 }, { 0, 23 }, { 63, 16 }, { 63, 23 } },
    { { 0, 24 }, { 0, 31 }, { 63, 24 }, { 63, 31 } },
    { { 0, 32 }, { 0, 39 }, { 63, 32 }, { 63, 39 } },
    { { 0, 40 }, { 0, 47 }, { 63, 40 }, { 63, 47 } },
    { { 0, 48 }, { 0, 55 }, { 63, 48 }, { 63, 55 } },
    { { 63, 0 }, { 63, 7 }, { 0, 0 }, { 0, 7 } },
    { { 63, 8 }, { 63, 15 }, { 0, 8 }, { 0, 15 } },
    { { 63, 16 }, { 63, 23 }, { 0, 16 }, { 0, 23 } },
    { { 63, 24 }, { 63, 31 }, { 0, 24 }, { 0, 31 } },
    { { 63, 32 }, { 63, 39 }, { 0, 32 }, { 0, 39 } },
    { { 63, 40 }, { 63, 47 }, { 0, 40 }, { 0, 47 } },
    { { 63, 48 }, { 63, 55 }, { 0, 48 }, { 0, 55 } },
    { { 0, 7 }, { 0, 0 }, { 63, 7 }, { 63, 0 } },
    { { 0, 15 }, { 0, 8 }, { 63, 15 }, { 63, 8 } },
    { { 0, 23 }, { 0, 16 }, { 63, 23 }, { 63, 16 } },
    { { 0, 31 }, { 0, 24 }, { 63, 31 }, { 63, 24 } },
    { { 0, 39 }, { 0, 32 }, { 63, 39 }, { 63, 32 } },
    { { 0, 47 }, { 0, 40 }, { 63, 47 }, { 63, 40 } },
    { { 0, 55 }, { 0, 48 }, { 63, 55 }, { 63, 48 } },
};

s32 D_801713CC[16] = {
    -1782,
    882,
    -1782,
    -300,
    -1518,
    336,
    -1200,
    300,
    -1200,
    -887,
    -18,
    882,
    -608,
    -18,
    -608,
    -618,
};

s32 D_8017140C[12] = {
    -2082,
    -714,
    -984,
    1140,
    -1032,
    -1140,
    888,
    1140,
    1080,
    -1140,
    2082,
    442,
};

s32 D_8017143C[28] = {
    -580,
    -18,
    -580,
    -518,
    0,
    582,
    0,
    -300,
    372,
    300,
    600,
    48,
    276,
    -877,
    1200,
    582,
    1200,
    -18,
    -608,
    -18,
    1787,
    882,
    1787,
    -618,
    0,
    -494,
    1768,
    -494,
};

s32 D_801714AC[24] = {
    -282,
    52,
    144,
    2040,
    12,
    1200,
    600,
    600,
    1200,
    1765,
    1200,
    20,
    2036,
    -240,
    2400,
    1200,
    2756,
    2040,
    3000,
    12,
    3588,
    1788,
    3882,
    1422,
};

void UAeffectInit(void)
{
    s32 i;
    u8** p;

    i = 0;
    gTruckWheel[0] = 0;
    gTruckWheel[1] = 0;
    gTruckWheel[2] = 0;
    gTruckWheel[3] = 0;
    gNumTVs = 0;
    p = gTVpolyUVs;
loop:
    *p = 0;
    p++;
    i++;
    if (i < 4) {
        goto loop;
    }
    gLightningPart[0] = 0;
    gLightningPart[1] = 0;
    gLightningPart[2] = 0;
    gLightningPart[3] = 0;
}

#ifdef NON_MATCHING
void UAeffectUpdateMonsterTruckWheels(s32 speed)
{
    MATRIX m;
    s32 i;
    s32 j;
    s32 a;

    a = gTruckWheelAngle;
    a = a - speed / 7;
    gTruckWheelAngle = a;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            s32 t = (i == j);
            m.m[i][j] = t << 12;
        }
        m.t[i] = 0;
    }
    RotMatrixX(-gTruckWheelAngle, &m);
    for (i = 0; i < 4; i++) {
        if (gTruckWheel[i] != 0) {
            gTruckWheel[i]->rot[0] = m.m[0][0];
            gTruckWheel[i]->rot[1] = m.m[0][1];
            gTruckWheel[i]->rot[2] = m.m[0][2];
            gTruckWheel[i]->rot[3] = m.m[1][0];
            gTruckWheel[i]->rot[4] = m.m[1][1];
            gTruckWheel[i]->rot[5] = m.m[1][2];
            gTruckWheel[i]->rot[6] = m.m[2][0];
            gTruckWheel[i]->rot[7] = m.m[2][1];
            gTruckWheel[i]->rot[8] = m.m[2][2];
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectUpdateMonsterTruckWheels);
#endif

#ifdef NON_MATCHING
void UAeffectUpdateTVs(void)
{
    s32 buf;
    s32 i;

    buf = rtWhichDisplayBuffer();
    for (i = 0; i < gNumTVs; i++) {
        if (gTVpolyUVs[i] != 0) {
            ((UVWord*)gTVpolyUVs[i])->page = (&gTVsTPage)[buf];
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectUpdateTVs);
#endif

#ifdef NON_MATCHING
void UAeffectInitTV(EffNode* node)
{
    u8* uv;

    if (node == 0) {
        return;
    }
    if (node->type != 0) {
        return;
    }
    if (gNumTVs == 4) {
        return;
    }
    uv = node->prim;
    uv = uv + 16 + ((uv[15] >> 2) & 0x1c);
    uv[0] = 0x1c;
    uv[1] = 0x3a;
    uv[4] = 0xa2;
    uv[5] = 0x3a;
    uv[8] = 0x1c;
    uv[9] = 0x9d;
    uv[12] = 0xa2;
    uv[13] = 0x9d;
    gTVpolyUVs[gNumTVs] = uv + 4;
    gNumTVs = gNumTVs + 1;
    node->index = gNumTVs;
    if (gNumTVs == 1) {
        gTVsTPage = GetTPage(2, 0, 64, 0);
        D_8018C5FA = GetTPage(2, 0, 384, 0);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectInitTV);
#endif

#ifdef NON_MATCHING
void UAeffectInitMonsterTruckWheel(WheelNode* node)
{
    s32 id;

    if (node == 0) {
        return;
    }
    if (node->type != 5) {
        return;
    }
    id = node->wheelId;
    if (id >= 1 && id <= 4) {
        gTruckWheel[id - 1] = node;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectInitMonsterTruckWheel);
#endif

void UAeffectUpdateSpecialEffects(void)
{
    if (shellGetCurrentLevel() > 0) {
        UAeffectUpdateHealthStandLightning();
    }
    UAeffectUpdateTVs();
    UAeffectUpdateHitLights();
}

#ifdef NON_MATCHING
void UAeffectUpdateCarSpeed(s32 carId, s32 speed)
{
    if (carId == 40) {
        UAeffectUpdateMonsterTruckWheels(speed);
    } else {
        carId = carId / 10;
        if (carId < 15) {
            gCarSpeed[carId] = gCarSpeed[carId] + __builtin_abs(speed);
            if (gCarSpeed[carId] >= 321) {
                uaswToggleCarTireState(carId, speed);
                gCarSpeed[carId] = 0;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectUpdateCarSpeed);
#endif

void UAeffectActHitLights(Cs* node, s32 count, u8* unused_rgb)
{
    s32 i;
    s32 found;

    found = 0;
    i = 0;
    while (1) {
        if (i >= 6) {
            break;
        }
        if (gHitLight[i].node == node) {
            gHitLight[i].count = count;
            found = 1;
        }
        i++;
        if (found) {
            break;
        }
    }
    i = 0;
    if (!found) {
        while (1) {
            if (i >= 6) {
                break;
            }
            if (gHitLight[i].count == 0) {
                found = 1;
                gHitLight[i].count = count;
                gHitLight[i].node = node;
            }
            i++;
            if (found) {
                break;
            }
        }
    }
}

void UAeffectUpdateHitLights(void)
{
    Cs** nd;
    s32* cnt;
    s32 i;
    s32 c;

    gHitLightFlash = (gHitLightFlash != 1);
    i = 0;
    nd = &gHitLight[0].node;
    cnt = &gHitLight[0].count;
loop:
    c = *cnt;
    if (c > 0) {
        gHitLight[i].count = c - 1;
        c = *cnt;
        if (c != 0 && gHitLightFlash != 0) {
            (*nd)->env = lightGetEnv(1);
        } else {
            (*nd)->env = lightGetEnv(0);
        }
    }
    nd += 2;
    i++;
    cnt += 2;
    if (i < 6) {
        goto loop;
    }
}

void UAeffectBarricade(s32 a0, s32 a1, s32 state)
{
    if (state == 0) {
        state = 1;
    } else if (state == 1) {
        state = 2;
    } else {
        return;
    }
    uaswSetState(a0, a1, state);
}

void effectResetEffects(void)
{
    gNumTVs = 0;
}

void UAeffectSetFreezeLight(Cs* node)
{
    node->env = lightGetEnv(2);
}

void UAeffectClearFreezeLight(Cs* node)
{
    node->env = lightGetEnv(0);
}

#ifdef NON_MATCHING
void UAeffectInitHealthStandLightning(EffNode* node)
{
    u8* p;
    s32 i;

    if (node == 0) {
        return;
    }
    if (node->type != 0) {
        return;
    }
    if (node->kind != 4) {
        return;
    }
    p = node->prim;
    if (p == 0) {
        gLightningPart[3] = 0;
        gLightningPart[2] = 0;
        gLightningPart[1] = 0;
        gLightningPart[0] = 0;
        return;
    }
    for (i = 0; i < 4; i++) {
        u8* q;

        q = p + 16;
        gLightningPart[i] = q + ((p[15] >> 2) & 0x1c);
        p = p + p[2] * 4;
    }
    gLightningUoffset = gLightningPart[0][0];
    gLightningVoffset = gLightningPart[0][1];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectInitHealthStandLightning);
#endif

#ifdef NON_MATCHING
void UAeffectUpdateHealthStandLightning(void)
{
    u8* p;
    s32 i;
    s32 j;
    s32 r;

    if (gLightningPart[0] == 0) {
        return;
    }
    gLightningTick = gLightningTick + 1;
    if (gLightningTick >= gLightningPeriod) {
        gLightningTick = 0;
        gLightningOn = (gLightningOn == 0);
        gLightningPeriod = rand() / 2184 + 4;
        if (gLightningOn == 0) {
            gLightningPeriod = gLightningPeriod >> 1;
        }
    }
    if (gLightningOn != 0) {
        for (i = 0; i < 4; i++) {
            r = rand() / 1560;
            if (r >= 21) {
                r = 20;
            }
            p = gLightningPart[i];
            for (j = 0; j < 4; j++) {
                p[0] = gLightningUoffset + gLightningUVs[r][j][0];
                p[1] = gLightningVoffset - gLightningUVs[r][j][1];
                p += 4;
            }
        }
    } else {
        for (i = 0; i < 4; i++) {
            p = gLightningPart[i];
            for (j = 0; j < 4; j++) {
                p[0] = gLightningUoffset;
                p[1] = gLightningVoffset;
                p += 4;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_effect", UAeffectUpdateHealthStandLightning);
#endif
