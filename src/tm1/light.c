#include "common.h"

#include "tm1/light.h"

static s32 ambient[3];
static s32 lightRot[3][3];
static s32 lightColor[3][3];
static LightEnv lightEnv[4];

#ifdef NON_MATCHING
void lightSetLight(s32 e, s32 j, s32 rx, s32 ry, s16 r, s16 g, s16 b)
{
    s32 ax;
    s32 t0;
    s32 t1;
    s32 t2;

    if (e < 5) {
        if (j < 3) {
            ax = (rx << 12) / 360;
            t0 = -(rsin(ax) * rcos((ry << 12) / 360)) / 4096;
            t1 = -(rcos(ax) * rcos((ry << 12) / 360)) / 4096;
            t2 = rsin((-ry << 12) / 360);
            lightEnv[e].light.m[j][0] = -t0;
            lightEnv[e].light.m[j][1] = -t1;
            lightEnv[e].light.m[j][2] = -t2;
            lightEnv[e].color.m[0][j] = r << 4;
            lightEnv[e].color.m[1][j] = g << 4;
            lightEnv[e].color.m[2][j] = b << 4;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/light", lightSetLight);
#endif

void lightSetRot(s32 e, s32 j, s16 rx, s16 ry)
{
    s32 ax;
    s32 t0;
    s32 t1;
    s32 t2;

    if (e < 5) {
        if (j < 3) {
            lightRot[j][0] = ry;
            lightRot[j][1] = 0;
            lightRot[j][2] = rx;
            ax = (rx << 12) / 360;
            t0 = -(rsin(ax) * rcos((ry << 12) / 360)) / 4096;
            t1 = -(rcos(ax) * rcos((ry << 12) / 360)) / 4096;
            t2 = rsin((-ry << 12) / 360);
            lightEnv[e].light.m[j][0] = -t0;
            lightEnv[e].light.m[j][1] = -t1;
            lightEnv[e].light.m[j][2] = -t2;
        }
    }
}

void lightSetColor(s32 e, s32 j, s16 r, s16 g, s16 b)
{
    if (e < 5) {
        if (j < 3) {
            lightColor[j][0] = r;
            lightColor[j][1] = g;
            lightColor[j][2] = b;
            lightEnv[e].color.m[0][j] = r << 4;
            lightEnv[e].color.m[1][j] = g << 4;
            lightEnv[e].color.m[2][j] = b << 4;
        }
    }
}

void lightSetAmbient(s32 e, s16 r, s16 g, s16 b)
{
    if (e < 5) {
        lightEnv[e].ambient[0] = r;
        lightEnv[e].ambient[1] = g;
        lightEnv[e].ambient[2] = b;
    }
}

void lightInit(void)
{
    ambient[0] = 0x20;
    ambient[1] = 0x20;
    ambient[2] = 0x20;
    lightRot[0][2] = 170;
    lightRot[0][1] = 0;
    lightRot[0][0] = 30;
    lightRot[1][2] = -60;
    lightRot[1][1] = 0;
    lightRot[1][0] = 15;
    lightRot[2][2] = 60;
    lightRot[2][1] = 0;
    lightRot[2][0] = 20;
    lightColor[0][0] = 200;
    lightColor[0][1] = 200;
    lightColor[0][2] = 200;
    lightColor[1][0] = 128;
    lightColor[1][1] = 75;
    lightColor[1][2] = 75;
    lightColor[2][0] = 60;
    lightColor[2][1] = 128;
    lightColor[2][2] = 60;
    lightSetWorldLights(0);
    lightSetRedFlashEnv();
    lightSetFreezeEnv();
    lightSetLightningEnv();
}

#ifdef NON_MATCHING
void lightSetRedFlashEnv(void)
{
    lightSetAmbient(1, ambient[0], ambient[1], ambient[2]);
    lightSetLight(1, 0, lightRot[0][2], lightRot[0][0], 255, 0, 0);
    lightSetLight(1, 1, lightRot[1][2], lightRot[1][0], 255, 0, 0);
    lightSetLight(1, 2, lightRot[2][2], lightRot[2][0], 255, 0, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/light", lightSetRedFlashEnv);
#endif

#ifdef NON_MATCHING
void lightSetFreezeEnv(void)
{
    lightSetAmbient(2, ambient[0], ambient[1], ambient[2]);
    lightSetLight(2, 0, lightRot[0][2], lightRot[0][0], 60, 140, 170);
    lightSetLight(2, 1, lightRot[1][2], lightRot[1][0], 60, 140, 170);
    lightSetLight(2, 2, lightRot[2][2], lightRot[2][0], 60, 140, 170);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/light", lightSetFreezeEnv);
#endif

#ifdef NON_MATCHING
void lightSetLightningEnv(void)
{
    lightSetAmbient(3, 256, 256, 256);
    lightSetLight(3, 0, lightRot[0][2], lightRot[0][0], 256, 256, 256);
    lightSetLight(3, 1, lightRot[1][2], lightRot[1][0], 0, 0, 0);
    lightSetLight(3, 2, lightRot[2][2], lightRot[2][0], 0, 0, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/light", lightSetLightningEnv);
#endif

#ifdef NON_MATCHING
void lightSetWorldLights(s32 e)
{
    lightSetAmbient(0, ambient[0], ambient[1], ambient[2]);
    lightSetLight(
        0, 0, lightRot[0][2], lightRot[0][0], lightColor[0][0], lightColor[0][1], lightColor[0][2]);
    lightSetLight(
        0, 1, lightRot[1][2], lightRot[1][0], lightColor[1][0], lightColor[1][1], lightColor[1][2]);
    lightSetLight(
        0, 2, lightRot[2][2], lightRot[2][0], lightColor[2][0], lightColor[2][1], lightColor[2][2]);
    SetColorMatrix(&lightEnv[0].color);
    SetLightMatrix(&lightEnv[0].light);
    SetBackColor(lightEnv[0].ambient[0], lightEnv[0].ambient[1], lightEnv[0].ambient[2]);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/light", lightSetWorldLights);
#endif

s32* lightGetAmbient(void)
{
    return ambient;
}

s32* lightGetRot(void)
{
    return &lightRot[0][0];
}

s32* lightGetColor(void)
{
    return &lightColor[0][0];
}

LightEnv* lightGetEnv(s32 e)
{
    if (e < 4) {
        return &lightEnv[e];
    }
    return &lightEnv[0];
}
