#include "common.h"

#include <libgpu.h>
#include <libgs.h>
#include <libgte.h>
#include <rand.h>
#include <stdio.h>

#include "tm1/ai_car.h"
#include "tm1/car.h"
#include "tm1/fileio.h"
#include "tm1/font.h"
#include "tm1/grutils.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/smooth.h"
#include "tm1/ua.h"
#include "tm1/ua_dash.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

extern void exit(s32 code);
extern void* sdk_memcpy();

extern char* gCockpitTmsNames[12];
extern char* gCarSignStrings[13];

extern s32 gDashboardOn;
extern s32 D_8018C2AC;
extern DashS3 gHarleySprX;
extern DashS3 gHarleySprY;
extern DashS3 gHarleySprW;

extern s32 gDisplayDashboard;
extern s32 gBulletHoles;
extern s32 gListRemainingCars;
extern s32 gMaxCarryCapacity;
extern s16 whichOT;
extern u32* ot;

extern Dashboard gDash[2];
extern POLY_F4 gSeperator;
extern DR_MODE gMaxCarryDrawMode;
extern SPRT gMaxCarrySprites[23];
extern u32 OT[2][2];
extern POLY_FT4 sw;
extern POLY_FT4 hb1;
extern POLY_FT4 hb2;
extern SPRT gListSprites[8][30];
extern DR_MODE gListDrawMode[8];
extern u_long gTopBlitBuffer[];

#ifdef NON_MATCHING
void UAdashDrawDashboard(DVECTOR* pos)
{
    if (gDashboardOn) {
        gDash[0].dst[0].x = gDash[0].src[0].x + pos->vx;
        gDash[0].dst[0].y = gDash[0].src[0].y + pos->vy;
        LoadImage(&gDash[0].dst[0], gDash[0].data[0]);
        gDash[0].dst[1].x = gDash[0].src[1].x + pos->vx;
        gDash[0].dst[1].y = gDash[0].src[1].y + pos->vy;
        LoadImage(&gDash[0].dst[1], gDash[0].data[1]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", UAdashDrawDashboard);
#endif

#ifdef NON_MATCHING
void UAdashHealthColor(s32 tier, POLY_G4* p)
{
    u8 r0, g0, b0, r1, g1, b1;

    switch (tier) {
    case 0:
        r0 = 0xFF;
        g0 = 0x00;
        b0 = 0x00;
        r1 = 0xFF;
        g1 = 0x00;
        b1 = 0x00;
        break;
    case 1:
        r0 = 0xFF;
        g0 = 0x2A;
        b0 = 0x00;
        r1 = 0xFF;
        g1 = 0x73;
        b1 = 0x00;
        break;
    case 2:
        r0 = 0xBA;
        g0 = 0x9B;
        b0 = 0x20;
        r1 = 0xDD;
        g1 = 0xE7;
        b1 = 0x32;
        break;
    default:
        r0 = 0x00;
        g0 = 0x70;
        b0 = 0x0A;
        r1 = 0x31;
        g1 = 0x94;
        b1 = 0x29;
        break;
    }
    p->r0 = r0;
    p->g0 = g0;
    p->b0 = b0;
    p->r2 = r0;
    p->g2 = g0;
    p->b2 = b0;
    p->r1 = r1;
    p->g1 = g1;
    p->b1 = b1;
    p->r3 = r1;
    p->g3 = g1;
    p->b3 = b1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", UAdashHealthColor);
#endif

s32 UAdashTierFromPercentageOfHealth(s32 pct, s32 setHoles)
{
    if (pct < 10) {
        if (setHoles) {
            gBulletHoles = 6;
        }
        return 0;
    }
    if (pct < 30) {
        if (setHoles) {
            gBulletHoles = 3;
        }
        return 1;
    }
    if (pct < 50) {
        if (setHoles) {
            gBulletHoles = 2;
        }
        return 2;
    }
    if (setHoles) {
        gBulletHoles = 0;
    }
    return 3;
}

void uadashMaxCarryCapacity(void)
{
    gMaxCarryCapacity = 30;
}

void uadashClearCarryCapacity(void)
{
    gMaxCarryCapacity = 0;
}

INCLUDE_RODATA("asm/nonmatchings/tm1/ua_dash", D_800FC1FC);

#ifdef NON_MATCHING
void UAdashDrawInstruments(s32 player, Db* cdb)
{
    static DR_MODE drawMode;
    static SPRT sprites[30];
    Car* pi;
    POLY_G4* g;
    u8 isPlayer;
    s32 i;
    s32 n;
    s32 w;
    s32 pct;
    s32 speed;
    s32 name;
    s32 hp;
    s32 hpMax;
    s32 tier;
    void* car;
    DR_MODE* mode;
    u32 wep;
    s32 x1v;
    s32 x3v;
    s32 wv;
    s16 sel;
    POLY_G4* g2;

    whichOT = whichOT ^ 1;
    ot = OT[whichOT];
    ClearOTagR((u_long*)ot, 2);
    pi = GetPlayerInfo(player);
    sel = 0;
    if (gDisplayDashboard != 0 && pi->uaIndex != 0x50) {
        UAdashDrawDashboard((DVECTOR*)cdb);
        uadashDrawSteeringWheel(ot);
    }
    if (rtIsSplitScreenOn() != 0) {
        sel = 2;
    }
    for (i = sel; i < gDash[player].lifeCnt; i++) {
        AddPrim(ot, &gDash[player].lifeSpr[i]);
    }
    n = (s16)pi->weap.ammo[11] / (s8)pi->weap.unk48;
    if (n >= 6) {
        n = 5;
    }
    for (i = 0; i < n; i++) {
        AddPrim(ot, &gDash[player].specSpr[i]);
    }
    AddPrim(ot, &gDash[player].lifeMode);
    if (pi->weap.gunOverheat != 0) {
        AddPrim(ot, &gDash[player].f2);
    }
    hp = pi->stats.unk40;
    hpMax = pi->stats.unk44;
    pct = (hp * 100) / hpMax;
    tier = UAdashTierFromPercentageOfHealth(pct, 1);
    w = (pct * 0x58) / 100;
    if (w > 0) {
        if (w < 0x59) {
            wv = w;
        } else {
            wv = 0x58;
        }
    } else {
        wv = 1;
    }
    w = wv;
    if (w != 0) {
        x1v = gDash[player].g0.x0;
        x3v = gDash[player].g0.x2;
        g = &gDash[player].g0;
        x1v += w;
        x3v += w;
        gDash[player].g0.x1 = x1v;
        gDash[player].g0.x3 = x3v;
        UAdashHealthColor((s8)tier, g);
        AddPrim(ot, g);
    }
    if (gMaxCarryCapacity != 0 && rtIsSplitScreenOn() == 0) {
        fontSpritePrintCenteredXY(2, 0x9F, 0x9B, (u32*)&gMaxCarryDrawMode, (u32*)gMaxCarrySprites,
            "Max Carrying Capacity", rtGetCdb()->small);
        gMaxCarryCapacity--;
    }
    if (rtIsSplitScreenOn() == 0) {
        speed = GetPlayerSpeed(player);
        if (speed < 0) {
            speed = -speed;
        }
        uadashDrawSpeed(
            speed, 0x69, gDash[player].needle, gDash[player].dig, &gDash[player].speedMode, ot);
    }
    wep = pi->weap.cur;
    if (D_8018C2AC != 0 && wep < 12) {
        n = (s16)pi->weap.ammo[wep];
        if (wep == 11) {
            n = n / (s8)pi->weap.unk48;
        }
        if (n > 0) {
            AddPrim(ot, &gDash[player].wepSpr[wep]);
            AddPrim(ot, &gDash[player].wepMode[wep]);
            grutilsDrawNumberUsingSprites(n, gDash[player].ammoDig, gDash[player].dig, ot);
            mode = &gDash[player].selMode;
            goto sel;
        }
    } else {
        mode = &gDash[player].selMode;
    sel:
        AddPrim(ot, mode);
    }
    n = (s16)pi->weap.unk46 * 10 + pi->stats.unkF8 / 18;
    if (n >= 100) {
        pi->weap.unk46 = 10;
        pi->stats.unkF8 = 0;
        n = 99;
    }
    grutilsDrawNumberUsingSprites(n, gDash[player].machDig, gDash[player].dig2, ot);
    AddPrim(ot, &gDash[player].machMode);
    i = shellLivesRemaining() - 1;
    for (; i != -1; i--) {
        AddPrim(ot, &gDash[player].lives[i]);
    }
    AddPrim(ot, &gDash[player].livesMode);
    if (rtIsSplitScreenOn() != 0) {
        AddPrim(ot, &gDash[player].f0);
        if (player == 0) {
            AddPrim(ot, &gSeperator);
        }
        mode = &gDash[player].statMode;
    } else {
        uadashListRemainingCars(ot);
        if (rtIsRearViewOn() != 0 && gDisplayDashboard == 0) {
            UAdashDrawRearViewMirror(ot);
        }
        car = GetClosestCarToPlayer(player, &isPlayer);
        if (car != NULL) {
            if (isPlayer != 0) {
                name = ((Car*)car)->uaIndex;
                hp = ((Car*)car)->stats.unk40;
                hpMax = ((Car*)car)->stats.unk44;
                tier = UAdashTierFromPercentageOfHealth((hp * 100) / hpMax, 0);
            } else {
                name = ((CarAlt*)car)->uaIndex;
                hp = ((CarAlt*)car)->stats.unk40;
                hpMax = ((CarAlt*)car)->stats.unk44;
                tier = ((CarAlt*)car)->unk02;
            }
            if (hp > 0) {
                fontSetColor(2, 0x24, 0x96, 0x24);
                fontSpritePrintCenteredXY(
                    2, 0x92, 0x21, (u32*)&drawMode, (u32*)sprites, uadashGetCarSign(name), ot);
                fontSetDefaultColor(2);
            }
            w = (hp * 0x56) / hpMax;
            if (w > 0) {
                if (w < 0x57) {
                    wv = w;
                } else {
                    wv = 0x56;
                }
            } else {
                wv = 1;
            }
            w = wv;
            if (w != 0) {
                x1v = gDash[player].g1.x0;
                x3v = gDash[player].g1.x2;
                g2 = &gDash[player].g1;
                x1v += w;
                x3v += w;
                gDash[player].g1.x1 = x1v;
                gDash[player].g1.x3 = x3v;
                UAdashHealthColor((s8)tier, g2);
                AddPrim(ot, g2);
            }
        }
        n = GetNumAICarsLiving();
        grutilsDrawNumberUsingSprites(n, &gDash[player].killSpr, gDash[player].dig, ot);
        grutilsDrawNumberUsingSprites(
            GetNumAICars() - n, &gDash[player].totSpr, gDash[player].dig, ot);
        AddPrim(ot, &gDash[player].numMode);
        if (gDisplayDashboard != 0) {
            if (pi->uaIndex != 0x50) {
                for (i = 0; i < gDash[player].count; i++) {
                    AddPrim(ot, &gDash[player].elems[i]);
                    AddPrim(ot, &gDash[player].dmode[i]);
                }
                for (i = gBulletHoles - 1; i != -1; i--) {
                    AddPrim(ot, &gDash[player].bullets[i]);
                }
                AddPrim(ot, &gDash[player].bulMode);
            } else {
                for (i = 0; i < 3; i++) {
                    AddPrim(ot, &gDash[player].harley[i]);
                    AddPrim(ot, &gDash[player].hmode[i]);
                }
                uadashDrawHarleyHandleBars(ot);
                for (i = 0; i < 2; i++) {
                    AddPrim(ot, &(&gDash[player].f0)[i]);
                }
            }
        } else {
            for (i = 0; i < 2; i++) {
                AddPrim(ot, &(&gDash[player].f0)[i]);
            }
        }
        mode = &gDash[player].statMode;
    }
    SetDrawMode(mode, 0, 0, 0, 0);
    AddPrim(ot, mode);
    DrawOTag((u_long*)(ot + 1));
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", UAdashDrawInstruments);
#endif

#ifdef NON_MATCHING
void UAdashInitDashboardIcons(DashNode* node)
{
    GrSprite buf[12];
    s16 bias;
    s16 which;
    GrSprite* g;
    SPRT* s;
    DR_MODE* m;
    s32 n;
    s32 i;
    s32 x;
    s32 y;

    bias = 0;
    which = 1;
    if (node != NULL && (node->op & 0xFF) == 1 && node->nChild >= 5) {
        if (rtIsSplitScreenOn() != 0) {
            bias = -120;
            which = 2;
        }
        for (which = which - 1; which != -1; which = which - 1) {
            n = grutilsParse2DSprite(node->child[0], buf, 12);
            gDash[which].wepCnt = n;
            for (i = 0; i < gDash[which].wepCnt; i++) {
                m = &gDash[which].wepMode[i];
                s = &gDash[which].wepSpr[i];
                g = &buf[i];
                setlen(s, 4);
                setcode(s, 0x64);
                s->clut = g->clut;
                s->u0 = g->u0;
                s->v0 = g->v0;
                s->w = g->w;
                s->h = g->h;
                s->r0 = 0x80;
                s->g0 = 0x80;
                s->b0 = 0x80;
                SetShadeTex(s, 0);
                s->x0 = 0xFF;
                s->y0 = bias + 210;
                SetSemiTrans(s, 1);
                SetDrawMode(m, 0, 0, g->tpage, 0);
            }

            grutilsParse2DSprite(node->child[6], buf, 1);
            for (i = 0; i < 3; i++) {
                s = &gDash[which].lives[i];
                setlen(s, 4);
                setcode(s, 0x64);
                s->clut = buf[0].clut;
                s->u0 = buf[0].u0;
                s->v0 = buf[0].v0;
                s->w = buf[0].w;
                s->h = buf[0].h;
                s->r0 = 0x80;
                s->g0 = 0x80;
                s->b0 = 0x80;
                SetShadeTex(s, 0);
                s->x0 = 0x135;
                s->y0 = bias + 189 + i * 5;
                SetSemiTrans(s, 1);
                SetDrawMode(&gDash[which].livesMode, 0, 0, buf[0].tpage, 0);
            }

            grutilsParse2DSprite(node->child[7], buf, 1);
            for (i = 0; i < 6; i++) {
                s = &gDash[which].bullets[i];
                setlen(s, 4);
                setcode(s, 0x64);
                s->clut = buf[0].clut;
                s->u0 = buf[0].u0;
                s->v0 = buf[0].v0;
                s->w = buf[0].w;
                s->h = buf[0].h;
                s->r0 = 0x80;
                s->g0 = 0x80;
                s->b0 = 0x80;
                SetShadeTex(s, 0);
                s->x0 = 0;
                s->y0 = 0;
                SetSemiTrans(s, 1);
                SetDrawMode(&gDash[which].bulMode, 0, 0, buf[0].tpage, 0);
            }

            grutilsParse2DSprite(node->child[1], buf, 1);
            SetDrawMode(&gDash[which].numMode, 0, 0, buf[0].tpage, 0);

            n = grutilsParse2DSprite(node->child[2], buf, 5) - 1;
            gDash[which].lifeCnt = n;
            for (i = 0; i < gDash[which].lifeCnt; i++) {
                s = &gDash[which].lifeSpr[i];
                g = &buf[i];
                setlen(s, 4);
                setcode(s, 0x64);
                s->clut = g->clut;
                s->u0 = g->u0;
                s->v0 = g->v0;
                s->w = g->w;
                s->h = g->h;
                s->r0 = 0x80;
                s->g0 = 0x80;
                s->b0 = 0x80;
                SetShadeTex(s, 0);
                SetSemiTrans(s, 0);
            }
            SetDrawMode(&gDash[which].lifeMode, 0, 0, buf[0].tpage, 0);
            gDash[which].lifeSpr[0].x0 = 0x66;
            gDash[which].lifeSpr[0].y0 = bias + 40;
            gDash[which].lifeSpr[1].x0 = 0x97;
            gDash[which].lifeSpr[1].y0 = bias + 37;
            gDash[which].lifeSpr[2].r0 = 0x24;
            gDash[which].lifeSpr[2].g0 = 0x78;
            gDash[which].lifeSpr[2].b0 = 0x24;
            gDash[which].lifeSpr[2].x0 = 0xD8;
            gDash[which].lifeSpr[2].y0 = bias + 202;
            gDash[which].lifeSpr[3].r0 = 0x24;
            gDash[which].lifeSpr[3].g0 = 0x78;
            gDash[which].lifeSpr[3].b0 = 0x24;
            gDash[which].lifeSpr[3].x0 = 0xD8;
            gDash[which].lifeSpr[3].y0 = bias + 215;

            g = &buf[gDash[which].lifeCnt];
            x = 0x106;
            for (i = 0; i < 5; i++) {
                s = &gDash[which].specSpr[i];
                setlen(s, 4);
                setcode(s, 0x64);
                s->clut = g->clut;
                s->u0 = g->u0;
                s->v0 = g->v0;
                s->w = g->w;
                s->h = g->h;
                s->r0 = 0x80;
                s->g0 = 0x80;
                s->b0 = 0x80;
                gDash[which].specSpr[i].x0 = x;
                gDash[which].specSpr[i].y0 = bias + 202;
                x += 9;
                SetShadeTex(s, 0);
                SetSemiTrans(s, 0);
            }

            grutilsParse2DSprite(node->child[3], gDash[which].dig, 10);
            SetDrawMode(&gDash[which].selMode, 0, 0, gDash[which].dig[0].tpage, 0);
            SetDrawMode(&gDash[which].speedMode, 0, 0, gDash[which].dig[0].tpage, 0);
            for (i = 0; i < 3; i++) {
                s = &gDash[which].ammoDig[i];
                setlen(s, 4);
                setcode(s, 0x64);
                gDash[which].ammoDig[i].clut = gDash[which].dig[0].clut;
                s->r0 = 0x24;
                s->g0 = 0x78;
                s->b0 = 0x24;
                SetShadeTex(s, 0);
                SetSemiTrans(s, 0);
                s->x0 = 0x122;
                s->y0 = bias + 212;
                s = &gDash[which].needle[i];
                setlen(s, 4);
                setcode(s, 0x64);
                gDash[which].needle[i].clut = gDash[which].dig[0].clut;
                s->r0 = 0xFF;
                s->g0 = 0x0F;
                s->b0 = 0x24;
                SetShadeTex(s, 0);
                SetSemiTrans(s, 0);
                s->x0 = 0x73;
                s->y0 = bias + 202;
            }

            s = &gDash[which].killSpr;
            setlen(s, 4);
            setcode(s, 0x64);
            gDash[which].killSpr.clut = gDash[which].dig[0].clut;
            s->r0 = 0x24;
            s->g0 = 0x78;
            s->b0 = 0x24;
            SetShadeTex(s, 0);
            s->x0 = 0x86;
            y = bias + 39;
            s->y0 = y;
            s = &gDash[which].totSpr;
            setlen(s, 4);
            setcode(s, 0x64);
            gDash[which].totSpr.clut = gDash[which].dig[0].clut;
            s->r0 = 0x78;
            s->g0 = 0x24;
            s->b0 = 0x24;
            SetShadeTex(s, 0);
            s->x0 = 0xB3;
            s->y0 = y;

            grutilsParse2DSprite(node->child[4], gDash[which].dig2, 10);
            SetDrawMode(&gDash[which].machMode, 0, 0, gDash[which].dig2[0].tpage, 0);
            for (i = 0; i < 2; i++) {
                s = &gDash[which].machDig[i];
                setlen(s, 4);
                setcode(s, 0x64);
                gDash[which].machDig[i].clut = gDash[which].dig2[0].clut;
                s->r0 = 0x24;
                s->g0 = 0x78;
                s->b0 = 0x24;
                SetShadeTex(s, 0);
                SetSemiTrans(s, 0);
                s->x0 = 0xF3;
                s->y0 = bias + 202;
            }
        }
        D_8018C2AC = 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", UAdashInitDashboardIcons);
#endif

#ifdef NON_MATCHING
void UAdashInitDashboard(u_long* tim)
{
    s32 y;

    y = 0;
    if (rtIsSplitScreenOn()) {
        SetPolyF4(&gSeperator);
        setRGB0(&gSeperator, 0, 0, 0);
        setSemiTrans(&gSeperator, 0);
        y = -120;
        setXY4(&gSeperator, 0, 0x74, 0x140, 0x74, 0, 0x7A, 0x140, 0x7A);
    }

    setPolyG4(&gDash[0].g0);
    SetSemiTrans(&gDash[0].g0, 1);
    setXY4(&gDash[0].g0, 0xD9, y + 191, 0x131, y + 191, 0xD9, y + 199, 0x131, y + 199);

    setPolyF4(&gDash[0].f0);
    SetSemiTrans(&gDash[0].f0, 1);
    setXY4(&gDash[0].f0, 0xD8, y + 190, 0x132, y + 190, 0xD8, y + 200, 0x132, y + 200);
    setRGB0(&gDash[0].f0, 0xA, 0xA, 0xA);

    setPolyF4(&gDash[0].f2);
    SetSemiTrans(&gDash[0].f2, 1);
    setXY4(&gDash[0].f2, 0xD8, y + 214, 0xFE, y + 214, 0xD8, y + 224, 0xFE, y + 224);
    setRGB0(&gDash[0].f2, 0xFF, 0, 0);

    if (rtIsSplitScreenOn()) {
        setPolyG4(&gDash[1].g0);
        SetSemiTrans(&gDash[1].g0, 1);
        setXY4(&gDash[1].g0, 0xD9, y + 191, 0x131, y + 191, 0xD9, y + 199, 0x131, y + 199);

        setPolyF4(&gDash[1].f0);
        SetSemiTrans(&gDash[1].f0, 1);
        setXY4(&gDash[1].f0, 0xD8, y + 190, 0x132, y + 190, 0xD8, y + 200, 0x132, y + 200);
        setRGB0(&gDash[1].f0, 0xA, 0xA, 0xA);

        setPolyF4(&gDash[1].f2);
        SetSemiTrans(&gDash[1].f2, 1);
        setXY4(&gDash[1].f2, 0xD8, y + 214, 0xFE, y + 214, 0xD8, y + 224, 0xFE, y + 224);
        setRGB0(&gDash[1].f2, 0xFF, 0, 0);
    } else {
        uadashInitCockpit(tim);
        uadashInitRearViewMirror();

        setPolyG4(&gDash[0].g1);
        SetSemiTrans(&gDash[0].g1, 1);
        setXY4(&gDash[0].g1, 0x67, y + 0x16, 0xBD, y + 0x16, 0x67, y + 0x23, 0xBD, y + 0x23);

        setPolyF4(&gDash[0].f1);
        SetSemiTrans(&gDash[0].f1, 1);
        setXY4(&gDash[0].f1, 0x66, y + 0x15, 0xBE, y + 0x15, 0x66, y + 0x24, 0xBE, y + 0x24);
        setRGB0(&gDash[0].f1, 0xA, 0xA, 0xA);
    }
    gDashboardOn = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", UAdashInitDashboard);
#endif

u_long* uadashGetTopBlitAddr(void)
{
    return gTopBlitBuffer;
}

#ifdef NON_MATCHING
void uadashInitCockpit(u_long* tim)
{
    DashCockpitBuf buf;
    GrScreenSprite* s;
    DashElem* e;
    s32 n;
    s32 cnt;
    s32 i;

    buf.pix[0] = gTopBlitBuffer;
    buf.pix[1] = (u_long*)rtGetDoubleBufferArea();
    n = grutilsParse2DScreenSprite((GrObj*)tim, buf.spr, 7);
    for (i = 0; i < 2; i++) {
        gDash[0].data[i] = buf.pix[i];
        s = &buf.spr[i * (n - 1)];
        gDash[0].src[i].x = s->x;
        gDash[0].src[i].y = s->y;
        gDash[0].src[i].w = s->w;
        gDash[0].src[i].h = s->h;
        sdk_memcpy(&gDash[0].dst[i], &gDash[0].src[i], 8);
        gDash[0].data[i] = buf.pix[i];
    }
    cnt = n - 2;
    if (cnt < 0) {
        cnt = 0;
    }
    gDash[0].count = cnt;
    for (i = 0; i < gDash[0].count; i++) {
        e = &gDash[0].elems[i];
        s = &buf.spr[i + 1];
        SetSprt(&e->sprt);
        e->sprt.r0 = 0x80;
        e->sprt.g0 = 0x80;
        e->sprt.b0 = 0x80;
        e->sprt.x0 = s->x;
        e->sprt.y0 = s->y;
        e->sprt.w = s->w;
        e->sprt.h = s->h;
        e->sprt.u0 = s->u0;
        e->sprt.v0 = s->v0;
        e->sprt.clut = s->clut;
        e->tpage = s->tpage;
        SetDrawMode(&gDash[0].dmode[i], 0, 0, s->tpage, 0);
    }
    uadashInitHarley();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitCockpit);
#endif

#ifdef NON_MATCHING
void uadashReloadDashboard(u_long* tim, s32 idx)
{
    RECT r;
    GsIMAGE im;
    s32 tbuf[2];
    s32* t;
    DashElem* e;

    t = tbuf;
    e = &gDash[0].elems[idx];
    t[0] = (e->tpage << 4) & 0x100;
    t[1] = (e->tpage << 6) & 0x3FF;
    GsGetTimInfo(tim + 1, &im);
    r.x = (e->sprt.u0 >> 1) + t[1];
    r.h = im.ph;
    r.w = im.pw;
    r.y = e->sprt.v0 + t[0];
    LoadImage(&r, im.pixel);
    if ((im.pmode >> 3) & 1) {
        t[1] = (e->sprt.clut & 0x3F) * 16;
        t[0] = e->sprt.clut >> 6;
        r.h = im.ch;
        r.x = t[1];
        r.w = im.cw;
        r.y = t[0];
        LoadImage(&r, im.clut);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashReloadDashboard);
#endif

#ifdef NON_MATCHING
void uadashReloadHarleyDashboard(u_long* tim, s32 idx)
{
    RECT r;
    GsIMAGE im;
    s32 tbuf[2];
    s32* t;
    DashElem* e;

    t = tbuf;
    e = &gDash[0].harley[idx];
    t[0] = (e->tpage << 4) & 0x100;
    t[1] = (e->tpage << 6) & 0x3FF;
    GsGetTimInfo(tim + 1, &im);
    r.x = (e->sprt.u0 >> 1) + t[1];
    r.h = im.ph;
    r.w = im.pw;
    r.y = e->sprt.v0 + t[0];
    LoadImage(&r, im.pixel);
    if ((im.pmode >> 3) & 1) {
        t[1] = (e->sprt.clut & 0x3F) * 16;
        t[0] = e->sprt.clut >> 6;
        r.h = im.ch;
        r.x = t[1];
        r.w = im.cw;
        r.y = t[0];
        LoadImage(&r, im.clut);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashReloadHarleyDashboard);
#endif

#ifdef NON_MATCHING
void uadashReloadSteeringWheel(u_long* tim)
{
    RECT r;
    GsIMAGE im;
    s32 tbuf[2];
    s32* t;
    POLY_FT4* p;
    s32 x;
    s32 y;

    p = &gDash[0].wheel;
    t = tbuf;
    t[0] = (p->tpage << 4) & 0x100;
    t[1] = (p->tpage << 6) & 0x3FF;
    GsGetTimInfo(tim + 1, &im);
    x = MIN(p->u0, p->u1);
    x = MIN(x, p->u2);
    x = MIN(x, p->u3);
    r.x = x + tbuf[1];
    y = MIN(p->v0, p->v1);
    y = MIN(y, p->v2);
    y = MIN(y, p->v3);
    r.y = y + tbuf[0];
    r.w = im.pw;
    r.h = im.ph;
    LoadImage(&r, im.pixel);
    t = tbuf;
    if ((im.pmode >> 3) & 1) {
        t[1] = (p->clut & 0x3F) * 16;
        t[0] = p->clut >> 6;
        r.h = im.ch;
        r.x = t[1];
        r.w = im.cw;
        r.y = t[0];
        LoadImage(&r, im.clut);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashReloadSteeringWheel);
#endif

void uadashLoadHarleyTms(u_long* tims)
{
    s32 count;
    s32 i;
    u32 len;
    s32 idx;

    idx = 0;
    count = tims[0];
    tims++;
    len = *tims;
    for (i = 0; i < count; i++) {
        tims++;
        if (i == 3) {
            uadashReloadSteeringWheel(tims);
        } else {
            uadashReloadHarleyDashboard(tims, idx++);
        }
        tims += len >> 2;
        len = *tims;
    }
}

#ifdef NON_MATCHING
void uadashLoadCockpit(s32 car)
{
    char name[24];
    GsIMAGE im0;
    GsIMAGE im1;
    s32 num;
    u_long* p;
    s32 count;
    u32 len;
    s32 i;
    s32 idx;

    num = GetCarNumFromCarName(car);
    p = (u_long*)rtGetDoubleBufferArea();
    idx = 0;
    if (car < 0) {
        return;
    }
    sprintf(name, "UADASH\\%s", gCockpitTmsNames[num]);
    fileioLoadFileIntoRam(p, name);
    if (*p != 0x50535854) {
        printf("Error: %s - Invalid TMS file.", name);
        exit(-1);
    }
    p++;
    if (*p != 0x43) {
        printf("Error: %s - Old version.\n", name);
        exit(-1);
    }
    p++;
    count = *p;
    p++;
    if (car == 0x50) {
        uadashLoadHarleyTms(p);
        return;
    }
    count = p[0];
    p++;
    len = *p;
    for (i = 0; i < count; i++) {
        p++;
        switch (i) {
        case 0:
            GsGetTimInfo(p + 1, &im0);
            sdk_memcpy(gDash[0].data[0], im0.pixel, im0.pw * (im0.ph * 2));
            break;
        case 6:
            GsGetTimInfo(p + 1, &im1);
            sdk_memcpy(gDash[0].data[1], im1.pixel, im1.pw * (im1.ph * 2));
            break;
        case 7:
            uadashReloadSteeringWheel(p);
            break;
        default:
            uadashReloadDashboard(p, idx++);
            break;
        }
        p += len >> 2;
        len = *p;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashLoadCockpit);
#endif

void UAdashSetDashboardDrawFlag(s32 flag)
{
    gDisplayDashboard = flag;
}

#ifdef NON_MATCHING
void uadashInitSteeringWheel(GrObj* obj)
{
    GrVert* vt;
    DashFace* f;
    GrTexUV* uv;

    if (obj != NULL && obj->kind == 0) {
        vt = obj->verts;
        f = (DashFace*)obj->polys;
        setPolyFT4(&gDash[0].wheel);
        uv = (GrTexUV*)((u8*)&f->uv + ((f->uvsel >> 2) & 0x1C));
        SetSemiTrans(&gDash[0].wheel, 1);
        setRGB0(&gDash[0].wheel, 0xFF, 0xFF, 0xFF);
        gDash[0].wheel.x0 = vt[f->vi0].x / 8 - 120;
        gDash[0].wheel.y0 = -(vt[f->vi0].y / 8) - 64;
        gDash[0].wheel.x1 = vt[f->vi1].x / 8 - 120;
        gDash[0].wheel.y1 = -(vt[f->vi1].y / 8) - 64;
        gDash[0].wheel.x2 = vt[f->vi2].x / 8 - 120;
        gDash[0].wheel.y2 = vt[f->vi2].y / 8 - 64;
        gDash[0].wheel.x3 = vt[f->vi3].x / 8 - 120;
        gDash[0].wheel.y3 = vt[f->vi3].y / 8 - 64;
        gDash[0].wheel.u0 = uv->u0;
        gDash[0].wheel.v0 = uv->v0;
        gDash[0].wheel.u1 = uv->u1;
        gDash[0].wheel.v1 = uv->v1;
        gDash[0].wheel.u2 = uv->u2;
        gDash[0].wheel.v2 = uv->v2;
        gDash[0].wheel.u3 = uv->u3;
        gDash[0].wheel.v3 = uv->v3;
        gDash[0].wheel.clut = uv->clut;
        gDash[0].wheel.tpage = uv->tpage;
        setPolyFT4(&sw);
        SetSemiTrans(&sw, 1);
        setRGB0(&sw, 0xFF, 0xFF, 0xFF);
        uadashInitHarleyHandleBars();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitSteeringWheel);
#endif

#ifdef NON_MATCHING
void uadashInitHarleyHandleBars(void)
{
    setPolyFT4(&gDash[0].hb[0]);
    SetSemiTrans(&gDash[0].hb[0], 1);
    setRGB0(&gDash[0].hb[0], 0xFF, 0xFF, 0xFF);
    gDash[0].hb[0].x0 = -0x0E;
    gDash[0].hb[0].y0 = 0x7E;
    gDash[0].hb[0].x1 = -0x72;
    gDash[0].hb[0].y1 = 0x7E;
    gDash[0].hb[0].x2 = -0x0E;
    gDash[0].hb[0].y2 = -0x82;
    gDash[0].hb[0].x3 = -0x72;
    gDash[0].hb[0].y3 = -0x82;
    gDash[0].hb[0].u0 = gDash[0].wheel.u1;
    gDash[0].hb[0].v0 = gDash[0].wheel.v1;
    gDash[0].hb[0].u1 = gDash[0].wheel.u3;
    gDash[0].hb[0].v1 = gDash[0].wheel.v3;
    gDash[0].hb[0].u2 = gDash[0].wheel.u0;
    gDash[0].hb[0].v2 = gDash[0].wheel.v0;
    gDash[0].hb[0].u3 = gDash[0].wheel.u2;
    gDash[0].hb[0].v3 = gDash[0].wheel.v2;
    gDash[0].hb[0].clut = gDash[0].wheel.clut;
    gDash[0].hb[0].tpage = gDash[0].wheel.tpage;
    setPolyFT4(&hb1);
    SetSemiTrans(&hb1, 1);
    setRGB0(&hb1, 0xFF, 0xFF, 0xFF);
    setPolyFT4(&gDash[0].hb[1]);
    SetSemiTrans(&gDash[0].hb[1], 1);
    setRGB0(&gDash[0].hb[1], 0xFF, 0xFF, 0xFF);
    gDash[0].hb[1].x0 = 0x72;
    gDash[0].hb[1].y0 = 0x7E;
    gDash[0].hb[1].x1 = 0x0E;
    gDash[0].hb[1].y1 = 0x7E;
    gDash[0].hb[1].x2 = 0x72;
    gDash[0].hb[1].y2 = -0x82;
    gDash[0].hb[1].x3 = 0x0E;
    gDash[0].hb[1].y3 = -0x82;
    gDash[0].hb[1].u0 = gDash[0].wheel.u3;
    gDash[0].hb[1].v0 = gDash[0].wheel.v3;
    gDash[0].hb[1].u1 = gDash[0].wheel.u1;
    gDash[0].hb[1].v1 = gDash[0].wheel.v1;
    gDash[0].hb[1].u2 = gDash[0].wheel.u2;
    gDash[0].hb[1].v2 = gDash[0].wheel.v2;
    gDash[0].hb[1].u3 = gDash[0].wheel.u0;
    gDash[0].hb[1].v3 = gDash[0].wheel.v0;
    gDash[0].hb[1].clut = gDash[0].wheel.clut;
    gDash[0].hb[1].tpage = gDash[0].wheel.tpage;
    setPolyFT4(&hb2);
    SetSemiTrans(&hb2, 1);
    setRGB0(&hb2, 0xFF, 0xFF, 0xFF);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitHarleyHandleBars);
#endif

#ifdef NON_MATCHING
void uadashDrawSteeringWheel(u32* ot)
{
    static s32 delta;
    MATRIX mtx;
    SVECTOR rot;
    SVECTOR in;
    VECTOR out;
    long flag;
    POLY_FT4* p;
    Car* pi;
    s32 rate;
    s32 i;
    s32 j;

    pi = GetPlayerInfo(0);
    p = &gDash[0].wheel;
    rate = 0x46;
    if (pi->skid[7] || pi->skid[6]) {
        rate = 0x28;
    }
    if (pi->skid[2]) {
        delta = SmoothValue(delta, -28, rate);
    } else if (pi->skid[3]) {
        delta = SmoothValue(delta, 28, rate);
    } else {
        delta = SmoothValue(delta, 0, rate);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mtx.m[i][j] = (i == j) << 12;
        }
        mtx.t[i] = 0;
    }
    mtx.t[2] = 0;
    rot.vx = 0;
    rot.vy = 0;
    mtx.t[0] = 0x7D;
    mtx.t[1] = 0xE6;
    rot.vz = (delta << 12) / 360;
    RotMatrixYXZ(&rot, &mtx);
    SetTransMatrix(&mtx);
    SetRotMatrix(&mtx);
    in.vx = p->x0;
    in.vy = p->y0;
    RotTrans(&in, &out, &flag);
    sw.x0 = out.vx;
    sw.y0 = out.vy;
    in.vx = p->x1;
    in.vy = p->y1;
    RotTrans(&in, &out, &flag);
    sw.x1 = out.vx;
    sw.y1 = out.vy;
    in.vx = p->x2;
    in.vy = p->y2;
    RotTrans(&in, &out, &flag);
    sw.x2 = out.vx;
    sw.y2 = out.vy;
    in.vx = p->x3;
    in.vy = p->y3;
    RotTrans(&in, &out, &flag);
    sw.x3 = out.vx;
    sw.y3 = out.vy;
    sw.u0 = p->u0;
    sw.u1 = p->u1;
    sw.u2 = p->u2;
    sw.u3 = p->u3;
    sw.v0 = p->v0;
    sw.v1 = p->v1;
    sw.v2 = p->v2;
    sw.v3 = p->v3;
    sw.clut = p->clut;
    sw.tpage = p->tpage;
    if (ot != NULL) {
        AddPrim(ot, &sw);
        return;
    }
    DrawPrim(&sw);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashDrawSteeringWheel);
#endif

#ifdef NON_MATCHING
void uadashDrawHarleyHandleBars(u32* ot)
{
    static s32 delta;
    MATRIX mtx;
    SVECTOR rot;
    SVECTOR in;
    VECTOR out;
    long flag;
    Car* pi;
    s32 rate;
    s32 i;
    s32 j;

    pi = GetPlayerInfo(0);
    rate = 0x46;
    if (pi->skid[7] || pi->skid[6]) {
        rate = 0x28;
    }
    if (pi->skid[2]) {
        delta = SmoothValue(delta, -28, rate);
    } else if (pi->skid[3]) {
        delta = SmoothValue(delta, 28, rate);
    } else {
        delta = SmoothValue(delta, 0, rate);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            mtx.m[i][j] = (i == j) << 12;
        }
        mtx.t[i] = 0;
    }
    mtx.t[2] = 0;
    rot.vx = 0;
    rot.vy = 0;
    mtx.t[0] = 0x9F;
    mtx.t[1] = 0xCA;
    rot.vz = (delta << 12) / 360;
    RotMatrixYXZ(&rot, &mtx);
    SetTransMatrix(&mtx);
    SetRotMatrix(&mtx);
    in.vx = gDash[0].hb[0].x0;
    in.vy = gDash[0].hb[0].y0;
    RotTrans(&in, &out, &flag);
    hb1.x0 = out.vx;
    hb1.y0 = out.vy;
    in.vx = gDash[0].hb[0].x1;
    in.vy = gDash[0].hb[0].y1;
    RotTrans(&in, &out, &flag);
    hb1.x1 = out.vx;
    hb1.y1 = out.vy;
    in.vx = gDash[0].hb[0].x2;
    in.vy = gDash[0].hb[0].y2;
    RotTrans(&in, &out, &flag);
    hb1.x2 = out.vx;
    hb1.y2 = out.vy;
    in.vx = gDash[0].hb[0].x3;
    in.vy = gDash[0].hb[0].y3;
    RotTrans(&in, &out, &flag);
    hb1.x3 = out.vx;
    hb1.y3 = out.vy;
    hb1.u0 = gDash[0].hb[0].u0;
    hb1.u1 = gDash[0].hb[0].u1;
    hb1.u2 = gDash[0].hb[0].u2;
    hb1.u3 = gDash[0].hb[0].u3;
    hb1.v0 = gDash[0].hb[0].v0;
    hb1.v1 = gDash[0].hb[0].v1;
    hb1.v2 = gDash[0].hb[0].v2;
    hb1.v3 = gDash[0].hb[0].v3;
    hb1.clut = gDash[0].hb[0].clut;
    hb1.tpage = gDash[0].hb[0].tpage;
    AddPrim(ot, &hb1);
    in.vx = gDash[0].hb[1].x0;
    in.vy = gDash[0].hb[1].y0;
    RotTrans(&in, &out, &flag);
    hb2.x0 = out.vx;
    hb2.y0 = out.vy;
    in.vx = gDash[0].hb[1].x1;
    in.vy = gDash[0].hb[1].y1;
    RotTrans(&in, &out, &flag);
    hb2.x1 = out.vx;
    hb2.y1 = out.vy;
    in.vx = gDash[0].hb[1].x2;
    in.vy = gDash[0].hb[1].y2;
    RotTrans(&in, &out, &flag);
    hb2.x2 = out.vx;
    hb2.y2 = out.vy;
    in.vx = gDash[0].hb[1].x3;
    in.vy = gDash[0].hb[1].y3;
    RotTrans(&in, &out, &flag);
    hb2.x3 = out.vx;
    hb2.y3 = out.vy;
    hb2.u0 = gDash[0].hb[1].u0;
    hb2.u1 = gDash[0].hb[1].u1;
    hb2.u2 = gDash[0].hb[1].u2;
    hb2.u3 = gDash[0].hb[1].u3;
    hb2.v0 = gDash[0].hb[1].v0;
    hb2.v1 = gDash[0].hb[1].v1;
    hb2.v2 = gDash[0].hb[1].v2;
    hb2.v3 = gDash[0].hb[1].v3;
    hb2.clut = gDash[0].hb[1].clut;
    hb2.tpage = gDash[0].hb[1].tpage;
    AddPrim(ot, &hb2);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashDrawHarleyHandleBars);
#endif

#ifdef NON_MATCHING
void uadashDrawSpeed(
    s32 speed, s32 unused1, SPRT* unused2, GrSprite* unused3, DR_MODE* unused4, u32* ot)
{
    static DR_MODE drawMode;
    static SPRT digitSprites[4];
    char str[4] = "   ";
    s32 i;
    s32 q;
    s32 r;

    for (i = 2; i != -1; i--) {
        q = speed / 10;
        r = speed % 10;
        speed = q;
        if (i == 2 || r != 0 || speed != 0) {
            str[i] = r + '0';
        }
    }
    fontSpritePrintXY(4, 0x66, 0xD6, (u32*)&drawMode, (u32*)digitSprites, str, ot);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashDrawSpeed);
#endif

#ifdef NON_MATCHING
void uadashInitHarley(void)
{
    DashS3 xs = gHarleySprX;
    DashS3 ys = gHarleySprY;
    DashS3 ws = gHarleySprW;
    DashS3 cluts;
    DashB3 us;
    DashB3 vs;
    DashElem* e;
    s32 i;

    {
        DashS3 t;
        t.v[0] = gDash[0].elems[0].sprt.clut;
        t.v[1] = gDash[0].elems[1].sprt.clut;
        t.v[2] = gDash[0].elems[2].sprt.clut;
        cluts = t;
    }
    {
        DashB3 t;
        t.v[0] = gDash[0].elems[3].sprt.u0;
        t.v[1] = gDash[0].elems[4].sprt.u0;
        t.v[2] = gDash[0].elems[4].sprt.u0;
        us = t;
    }
    {
        DashB3 t;
        t.v[0] = gDash[0].elems[3].sprt.v0;
        t.v[1] = gDash[0].elems[4].sprt.v0;
        t.v[2] = gDash[0].elems[4].sprt.v0;
        vs = t;
    }

    for (i = 0; i < 3; i++) {
        e = &gDash[0].harley[i];
        SetSprt(&e->sprt);
        e->sprt.r0 = 0x80;
        e->sprt.g0 = 0x80;
        e->sprt.b0 = 0x80;
        e->sprt.x0 = xs.v[i];
        e->sprt.y0 = ys.v[i];
        e->sprt.w = ws.v[i];
        e->sprt.h = 0x24;
        e->sprt.u0 = us.v[i];
        e->sprt.v0 = vs.v[i];
        e->sprt.clut = cluts.v[i];
    }

    gDash[0].harley[0].tpage = gDash[0].elems[3].tpage;
    SetDrawMode(&gDash[0].hmode[0], 0, 0, gDash[0].harley[0].tpage, 0);
    gDash[0].harley[1].tpage = gDash[0].elems[4].tpage;
    SetDrawMode(&gDash[0].hmode[1], 0, 0, gDash[0].harley[1].tpage, 0);
    gDash[0].harley[2].tpage = gDash[0].elems[4].tpage + 1;
    SetDrawMode(&gDash[0].hmode[2], 0, 0, gDash[0].harley[2].tpage, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitHarley);
#endif

#ifdef NON_MATCHING
char* uadashGetCarSign(s32 car)
{
    return gCarSignStrings[GetCarNumFromCarName(car)];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashGetCarSign);
#endif

#ifdef NON_MATCHING
void uadashInitRearViewMirror(void)
{
    POLY_F4* p;

    p = gDash[0].mirrors;
    setPolyF4(p);
    setXY4(p, 0xD1, 0x12, 0x13A, 0x12, 0xD1, 0x14, 0x13A, 0x14);
    setRGB0(p, 0, 0, 0);
    SetSemiTrans(p, 0);
    p++;
    setPolyF4(p);
    setXY4(p, 0xD1, 0x32, 0x13A, 0x32, 0xD1, 0x34, 0x13A, 0x34);
    setRGB0(p, 0, 0, 0);
    SetSemiTrans(p, 0);
    p++;
    setPolyF4(p);
    setXY4(p, 0xD1, 0x14, 0xD3, 0x14, 0xD1, 0x32, 0xD3, 0x32);
    setRGB0(p, 0, 0, 0);
    SetSemiTrans(p, 0);
    p++;
    setPolyF4(p);
    setXY4(p, 0x138, 0x14, 0x13A, 0x14, 0x138, 0x32, 0x13A, 0x32);
    setRGB0(p, 0, 0, 0);
    SetSemiTrans(p, 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitRearViewMirror);
#endif

#ifdef NON_MATCHING
void uadashInitBulletHoles(void)
{
    s32 hole[12];
    s32* base;
    s32* h;
    s32* q;
    s32* c;
    SPRT* p;
    s32 i;
    s32 j;

    base = hole;
    h = base;
    for (i = 0; i < 6; i++) {
        h[0] = rand() % 295;
        h[6] = rand() % 105;
        if (i > 0) {
            q = base;
            c = h;
            for (j = 0; j < i; j++) {
                if (__builtin_abs(q[0] - c[0]) >= 16 && __builtin_abs(q[6] - c[6]) >= 16) { }
                q++;
            }
        }
        p = &gDash[0].bullets[i];
        p->x0 = h[0] + 20;
        p->y0 = h[6] + 50;
        h++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", uadashInitBulletHoles);
#endif

void UAdashDrawRearViewMirror(u32* ot)
{
    AddPrim(ot, &gDash[0].mirrors[0]);
    AddPrim(ot, &gDash[0].mirrors[1]);
    AddPrim(ot, &gDash[0].mirrors[2]);
    AddPrim(ot, &gDash[0].mirrors[3]);
}

void uadashToggleRemainingCarsList(void)
{
    gListRemainingCars ^= 1;
}

void uadashListRemainingCars(u32* ot)
{
    s32 sprOfs;
    s32 y;
    s32 modeOfs;
    s32 i;
    CarAlt* car;

    if (gListRemainingCars) {
        fontSetColor(2, 0x24, 0x96, 0x24);
        i = 0;
        sprOfs = 0;
        y = 0x46;
        modeOfs = 0;
        while (i < GetNumAICars()) {
            car = GetAICarInfo(i);
            if (car != NULL && car->stats.unk40 > 0) {
                fontSpritePrintXY(2, 0xDC, y, (u32*)((u8*)gListDrawMode + modeOfs),
                    (u32*)((u8*)gListSprites + sprOfs), uadashGetCarSign(car->uaIndex), ot);
            }
            sprOfs += sizeof(gListSprites[0]);
            y += 12;
            modeOfs += sizeof(DR_MODE);
            i++;
        }
        fontSetDefaultColor(2);
    }
}

// TODO: This is `rodata` that requires data migration (string table),
// and using `INCLUDE_RODATA()` (as we normally would) results in `gas` reordering
// it against our will. Our `INCLUDE_ASM()` prevents reordering, so use that instead.
//
// Remove this once we've properly migrated the data sections.
INCLUDE_ASM("asm/nonmatchings/tm1/ua_dash", D_800FC300);
