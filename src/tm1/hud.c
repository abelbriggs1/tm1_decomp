#include "common.h"
#include <abs.h>
#include <libgpu.h>
#include <libgte.h>

#include "tm1/grutils.h"
#include "tm1/hud.h"
#include "tm1/rt.h"

typedef struct Radar {
    /* 0x000 */ POLY_F4 bg[2];
    /* 0x030 */ LINE_F2 sig[48];
    /* 0x330 */ LINE_F2 mark[2];
    /* 0x350 */ DR_MODE mode;
    /* 0x35C */ u8 sigCount0;
    /* 0x35D */ u8 sigCount1;
    /* 0x35E */ u8 sigCount2;
    /* 0x35F */ u8 sigCount3;
    /* 0x360 */ s32 sigTotal;
    /* 0x364 */ s32 range;
    /* 0x368 */ s32 scale;
    /* 0x36C */ s32 buffer;
} Radar; /* 0x370 */

extern Radar radar;
extern s32 radarBuffer[];

static GrSprite ArrowUpInfo;
static GrSprite ArrowDownInfo;
static GrSprite ArrowLeftInfo;
static GrSprite ArrowRightInfo;

u8 radarOn = 1;

#ifdef NON_MATCHING
void hudResetRadar(void)
{
    radar.sigTotal = 0;
    radar.sigCount2 = 0;
    radar.sigCount3 = 0;
    radar.sigCount1 = 0;
    radar.sigCount0 = 0;
    radarBuffer[0] = (s32)(1u - (u32)radarBuffer[0]);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hud", hudResetRadar);
#endif

#ifdef NON_MATCHING
void hudDisplayRadar(u32* ot)
{
    Db* cdb;
    SPRT* p;
    DR_MODE* m;
    s32 i;
    RECT tw;

    if (rtIsSplitScreenOn() == 0 && radarOn != 0) {
        cdb = rtGetCdb();
        if (radar.sigCount0 != 0) {
            p = (SPRT*)cdb->unk8;
            if ((u32)(cdb->unk8 + 128) < (u32)cdb->areaEnd) {
                m = (DR_MODE*)((u8*)p + 20);
                cdb->unk8 = cdb->unk8 + 128;
                setSprt(p);
                setRGB0(p, 0x80, 0x80, 0x80);
                SetDrawMode(m, 0, 0, 0, NULL);
                setShadeTex(p, 1);
                setXY0(p, 0x2F, 0x1A);
                setWH(p, ArrowUpInfo.w, ArrowUpInfo.h);
                SetSemiTrans(p, 1);
                setUV0(p, ArrowUpInfo.u0 + 1, ArrowUpInfo.v0);
                SetDrawMode(m, 0, 0, ArrowUpInfo.tpage, NULL);
                p->clut = ArrowUpInfo.clut;
                addPrim(ot, p);
                addPrim(ot, m);
            }
        }
        if (radar.sigCount1 != 0) {
            p = (SPRT*)cdb->unk8;
            if ((u32)(cdb->unk8 + 128) < (u32)cdb->areaEnd) {
                m = (DR_MODE*)((u8*)p + 20);
                cdb->unk8 = cdb->unk8 + 128;
                setSprt(p);
                setRGB0(p, 0x80, 0x80, 0x80);
                SetDrawMode(m, 0, 0, 0, NULL);
                setShadeTex(p, 1);
                setXY0(p, 0x2F, 0x4D);
                setWH(p, ArrowDownInfo.w, ArrowDownInfo.h);
                SetSemiTrans(p, 1);
                setUV0(p, ArrowDownInfo.u0 + 1, ArrowDownInfo.v0);
                SetDrawMode(m, 0, 0, ArrowDownInfo.tpage, NULL);
                p->clut = ArrowDownInfo.clut;
                addPrim(ot, p);
                addPrim(ot, m);
            }
        }
        if (radar.sigCount3 != 0) {
            p = (SPRT*)cdb->unk8;
            cdb->unk8 = cdb->unk8 + 128;
            if ((u32)(cdb->unk8 + 128) < (u32)cdb->areaEnd) {
                m = (DR_MODE*)((u8*)p + 20);
                setSprt(p);
                setRGB0(p, 0x80, 0x80, 0x80);
                SetDrawMode(m, 0, 0, 0, NULL);
                setShadeTex(p, 1);
                setXY0(p, 0xA, 0x31);
                setWH(p, ArrowLeftInfo.w, ArrowLeftInfo.h);
                SetSemiTrans(p, 1);
                setUV0(p, ArrowLeftInfo.u0, ArrowLeftInfo.v0);
                SetDrawMode(m, 0, 0, ArrowLeftInfo.tpage, NULL);
                p->clut = ArrowLeftInfo.clut;
                addPrim(ot, p);
                addPrim(ot, m);
            }
        }
        if (radar.sigCount2 != 0) {
            p = (SPRT*)cdb->unk8;
            if ((u32)(cdb->unk8 + 128) < (u32)cdb->areaEnd) {
                m = (DR_MODE*)((u8*)p + 20);
                cdb->unk8 = cdb->unk8 + 128;
                setSprt(p);
                setRGB0(p, 0x80, 0x80, 0x80);
                SetDrawMode(m, 0, 0, 0, NULL);
                setShadeTex(p, 1);
                setXY0(p, 0x59, 0x31);
                setWH(p, ArrowRightInfo.w, ArrowRightInfo.h);
                SetSemiTrans(p, 1);
                setUV0(p, ArrowRightInfo.u0 + 1, ArrowRightInfo.v0);
                SetDrawMode(m, 0, 0, ArrowRightInfo.tpage, NULL);
                p->clut = ArrowRightInfo.clut;
                addPrim(ot, p);
                addPrim(ot, m);
            }
        }
        i = 0;
        if (radar.sigTotal > 0) {
            do {
                AddPrim(ot, &radar.sig[radar.buffer + (i * 2)]);
                i++;
            } while (i < radar.sigTotal);
        }
        AddPrim(ot, &radar.mark[0]);
        AddPrim(ot, &radar.mark[1]);
        AddPrim(ot, &radar.bg[0]);
        AddPrim(ot, &radar.mode);
        hudResetRadar();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hud", hudDisplayRadar);
#endif

#ifdef NON_MATCHING
void hudInitRadar(void)
{
    s32 i;
    LINE_F2* p;

    setPolyF4(&radar.bg[0]);
    setPolyF4(&radar.bg[1]);
    SetSemiTrans(&radar.bg[0], 1);
    SetSemiTrans(&radar.bg[1], 1);
    setRGB0(&radar.bg[0], 0x34, 0x71, 0x11);
    setRGB0(&radar.bg[1], 0x34, 0x71, 0x11);
    setXY4(&radar.bg[0], 0x10, 0x20, 0x59, 0x20, 0x10, 0x4D, 0x59, 0x4D);
    setXY4(&radar.bg[1], 0x10, 0x20, 0x59, 0x20, 0x10, 0x4D, 0x59, 0x4D);
    p = &radar.sig[0];
    for (i = 0; i < 48; i++) {
        setLineF2(p);
        SetSemiTrans(p++, 0);
    }
    setLineF2(&radar.mark[0]);
    setLineF2(&radar.mark[1]);
    setRGB0(&radar.mark[0], 0, 0x30, 0);
    setRGB0(&radar.mark[1], 0, 0x30, 0);
    setXY2(&radar.mark[0], 0x33, 0x37, 0x37, 0x37);
    setXY2(&radar.mark[1], 0x35, 0x36, 0x35, 0x38);
    SetDrawMode(&radar.mode, 0, 0, 0x1C, 0);
    radar.range = 0x1F40;
    radar.scale = 0x14D555;
    radar.sigTotal = 0;
    radarBuffer[0] = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hud", hudInitRadar);
#endif

#ifdef NON_MATCHING
void hudAddRadarSig(s32 id, s32* pos, s32 ang)
{
    s32 x;
    s32 z;
    s32 sx;
    s32 sz;
    s32 cosine;
    s32 sine;
    s32 buf;
    s32 bx;
    s32 by;
    s32 n;
    LINE_F2* a;
    LINE_F2* b;

    if (id == 0x398 && radarBuffer[0] != 0) {
        return;
    }
    x = pos[0];
    z = pos[1];
    cosine = rcos(ang);
    sine = rsin(ang);
    sx = (s32)((u32)x * (u32)cosine - (u32)z * (u32)sine) / radar.scale;
    sine = rsin(ang);
    cosine = rcos(ang);
    sz = (s32)(0u - ((u32)x * (u32)sine + (u32)z * (u32)cosine)) / radar.scale;
    if ((u32)sx + 36u >= 72 || (u32)sz + 22u >= 44 || radar.sigTotal + 2 >= 25) {
        if (id != 0x398) {
            if ((u32)sx + 36u < 72) {
                if (sz > 0) {
                    radar.sigCount1 = 1;
                } else {
                    radar.sigCount0 = 1;
                }
            } else if (sx < 0 && ABS(sx) > ABS(sz)) {
                radar.sigCount3 = 1;
            } else if (sx > 0 && ABS(sx) > ABS(sz)) {
                radar.sigCount2 = 1;
            } else if (sz > 0) {
                radar.sigCount1 = 1;
            } else {
                radar.sigCount0 = 1;
            }
        }
    } else {
        buf = radarBuffer[0];
        bx = sx + 53;
        by = sz + 55;
        n = radar.sigTotal * 2;
        a = &radar.sig[buf + n];
        n += 2;
        b = &radar.sig[buf + n];
        if (id == 0x398) {
            setXY2(a, sx + 52, sz + 54, sx + 54, sz + 56);
            setXY2(b, sx + 52, sz + 56, sx + 54, sz + 54);
        } else {
            setXY2(a, bx, by, sx + 54, by);
            setXY2(b, bx, sz + 56, sx + 54, sz + 56);
        }
        switch (id) {
        case 0xA:
            setRGB0(a, 0xFF, 0x90, 0xBA);
            setRGB0(b, 0xFF, 0x90, 0xBA);
            break;
        case 0x14:
            setRGB0(a, 0xFF, 0xFF, 0);
            setRGB0(b, 0xFF, 0xFF, 0);
            break;
        case 0x28:
            setRGB0(a, 0, 0xE9, 0);
            setRGB0(b, 0, 0xE9, 0);
            break;
        case 0x32:
            setRGB0(a, 0, 0, 0xFF);
            setRGB0(b, 0, 0, 0xFF);
            break;
        case 0x3C:
            setRGB0(a, 0xFF, 0, 0);
            setRGB0(b, 0xFF, 0, 0);
            break;
        case 0x46:
            setRGB0(a, 0x82, 0xCA, 0x36);
            setRGB0(b, 0x82, 0xCA, 0x36);
            break;
        case 0x50:
            setRGB0(a, 0xFF, 0x66, 0);
            setRGB0(b, 0xFF, 0x66, 0);
            break;
        case 0x5A:
            setRGB0(a, 0xC7, 0x71, 0);
            setRGB0(b, 0xC7, 0x71, 0);
            break;
        case 0x64:
            setRGB0(a, 0xFF, 0, 0xFF);
            setRGB0(b, 0xFF, 0, 0xFF);
            break;
        case 0x6E:
            setRGB0(a, 0xFF, 0xFF, 0xFF);
            setRGB0(b, 0xFF, 0xFF, 0xFF);
            break;
        case 0x78:
            setRGB0(a, 0x9B, 0x9B, 0x32);
            setRGB0(b, 0x9B, 0x9B, 0x32);
            break;
        case 0x1E:
        case 0x82:
            setRGB0(a, 0, 0, 0);
            setRGB0(b, 0, 0, 0);
            break;
        case 0x398:
            setRGB0(a, 0xFF, 0xC8, 0xFF);
            setRGB0(b, 0xFF, 0xC8, 0xFF);
            break;
        default:
            setRGB0(a, 0xFF, 0xF0, 0x78);
            setRGB0(b, 0xFF, 0xF0, 0x78);
            break;
        }
        radar.sigTotal += 2;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hud", hudAddRadarSig);
#endif

void hudradarActivate(void)
{
    radarOn = 1;
}

void hudRadarToggle(void)
{
    radarOn = 1 - radarOn;
}

#ifdef NON_MATCHING
void hudStoreArrowIcon(s32 id, GrObj* obj)
{
    switch (id) {
    case 0x50A:
        grutilsParse2DSprite(obj, &ArrowUpInfo, 1);
        break;
    case 0x50C:
        grutilsParse2DSprite(obj, &ArrowDownInfo, 1);
        break;
    case 0x50D:
        grutilsParse2DSprite(obj, &ArrowLeftInfo, 1);
        break;
    case 0x50B:
        grutilsParse2DSprite(obj, &ArrowRightInfo, 1);
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hud", hudStoreArrowIcon);
#endif
