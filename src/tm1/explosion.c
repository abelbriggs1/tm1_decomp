#include "common.h"

#include <libgpu.h>
#include <libgte.h>
#include <rand.h>

#include "tm1/explode.h"
#include "tm1/grutils.h"
#include "tm1/rt.h"
#include "tm1/stars.h"
#include "tm1/view.h"
#include "tm1/weapon.h"

#include "tm1/explosion.h"

static Fragment fragment[MAX_FRAGMENTS];
static Explosion pyro[MAX_EXPLOSIONS];

static Star starz[100];
static LINE_F2 starprim[200];

static GrSprite fragmenta[4];
static GrSprite fragmentb[4];
static GrSprite fragmentc[4];
static GrSprite fragmentd[4];

static GrSprite AburstInfo[16];
static GrSprite SparkInfo[4];
static GrSprite BurnInfo[8];
static GrSprite FlareInfo[4];
static GrSprite SmokeInfo[16];
static GrSprite ContrailInfo[8];
static GrSprite PlasmaInfo[4];
static GrSprite FlameInfo[10];
static GrSprite GburstInfo[12];
static GrSprite SteamInfo[16];

extern ArmorIconInfo ArmorInfo;

Explosion* init_explosion(VECTOR3* pos, s32 a1, s32 a2, s32 a3, void* frames, u16 flag);
#ifdef NON_MATCHING
Explosion* init_explosion(VECTOR3* pos, s32 a1, s32 a2, s32 a3, void* frames, u16 flag)
{
    s32 i;
    Explosion* e;
    VECTOR3* v;

    i = find_free_explosion();
    if (i < 0) {
        return 0;
    }
    e = &pyro[i];
    e->pos.vx = pos->vx;
    e->pos.vy = pos->vy;
    e->pos.vz = pos->vz;
    pyro[i].unk14 = a2;
    pyro[i].frames = frames;
    pyro[i].life = 0;
    pyro[i].unk10 = a1;
    pyro[i].unk2A = flag;
    pyro[i].unk2C = 0;
    pyro[i].unk0C = 0;
    pyro[i].unk28 = 0;
    pyro[i].unk1C = (s32)((u32)a3 * (u32)a2);
    v = &pyro[i].vel;
    v->vx = 0;
    v->vy = 0;
    v->vz = 0;
    return e;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", init_explosion);
#endif

#ifdef NON_MATCHING
void animate_explosions(void)
{
    s32 i;

    explodeUpdateFragments();
    for (i = 0; i < 80; i++) {
        if (pyro[i].life < 0) {
            continue;
        }
        pyro[i].life = (s32)((u32)pyro[i].life + 1u);
        pyro[i].pos.vx = (s32)((u32)pyro[i].pos.vx + (u32)pyro[i].vel.vx);
        pyro[i].pos.vy = (s32)((u32)pyro[i].pos.vy + (u32)pyro[i].vel.vy);
        pyro[i].pos.vz = (s32)((u32)pyro[i].pos.vz + (u32)pyro[i].vel.vz);
        if (pyro[i].life < pyro[i].unk1C) {
            continue;
        }
        if (pyro[i].unk2C-- != 0) {
            pyro[i].life = 0;
        } else {
            pyro[i].life = -1;
        }
        if (pyro[i].unk2C != 1) {
            continue;
        }
        if (pyro[i].frames != (void*)SmokeInfo) {
            continue;
        }
        pyro[i].unk1C = (s32)((u32)pyro[i].unk1C + 8u);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", animate_explosions);
#endif

s32 find_free_explosion(void)
{
    s32 i;

    for (i = 0; i < 80; i++) {
        if (pyro[i].life < 0) {
            return i;
        }
    }
    return -1;
}

// TODO: Leaving non-matching commented out for now since we don't have the
// required functions/data declared yet.

// void draw_explosions(Db *cdb, s32 which);
// #ifdef NON_MATCHING
// void draw_explosions(Db *cdb, s32 which)
// {
//     s32 i;
//     s32 cx;
//     s32 cy;
//     s32 hfov;
//     s32 vfov;
//     EyeTrans *eye;
//     EyeMat *mat;
//     u32 *ot;
//     Explosion *e;
//     GrSprite *f;
//     POLY_FT4 *p;
//     s32 z;
//     s32 zz;
//     s32 otz;
//     s32 size;
//     s32 sx;
//     s32 sy;
//     s32 px;
//     s32 py;
//     s32 nx;
//     s32 ny;
//     s32 ang;
//     s32 dx;
//     s32 dy;
//     s32 x0;
//     s32 y0;
//     s32 x1;
//     s32 y1;
//     s32 d;
//     s32 t;
//     s32 v[3];
//     s32 r[3];

//     ot = (u32 *)cdb->small;
//     viewGetCenter(which, &cx, &cy);
//     hfov = viewGetCurrentHorzFOVH(which);
//     vfov = viewGetCurrentVertFOVH(which);
//     explodeDisplayFragments(cdb, which);
//     eye = viewGetEyeTrans(which);
//     mat = viewGetEyeMat(which);
//     for (i = 0; i < 80; i++) {
//         if (pyro[i].life < 0) {
//             continue;
//         }
//         e = &pyro[i];
//         if (which > 0 && pyro[i].life == 1 && pyro[i].unk2C == 0) {
//             continue;
//         }
//         v[0] = pyro[i].pos.vx + eye->x;
//         v[1] = pyro[i].pos.vy + eye->y;
//         v[2] = pyro[i].pos.vz + eye->z;
//         mathMulVec(mat, v, r);
//         z = r[2];
//         if (z < -32) {
//             continue;
//         }
//         if (z >= 32001) {
//             continue;
//         }
//         if (z < 8) {
//             z = 8;
//         }
//         size = ((GrSprite *)e->frames)->w * e->unk10 * hfov / z;
//         size = size >> 3;
//         if (size >= 513) {
//             size = 512;
//         }
//         if (size <= 0) {
//             size = 1;
//         }
//         sx = r[0] * hfov / z;
//         sy = r[1] * vfov / z;
//         zz = z;
//         otz = z >> 3;
//         px = cx + sx;
//         py = cy + sy;
//         if (e->unk2A != 0) {
//             otz -= 8;
//         }
//         if (otz < 0) {
//             otz = 0;
//         }
//         if (px >= -99 && px < 1000) {
//             if (py >= -99 && py < 1000) {
//                 p = (POLY_FT4 *)cdb->unk8;
//                 if ((u8 *)(cdb->unk8 + 160) < cdb->areaEnd) {
//                     cdb->unk8 += 160;
//                     setPolyFT4(p);
//                     setSemiTrans(p, 1);
//                     t = e->unk28;
//                     if (t != 0 && t == 1) {
//                         setShadeTex(p, 0);
//                         p->r0 = 0x40;
//                         p->g0 = 0x40;
//                         p->b0 = 0xFF;
//                     } else {
//                         setShadeTex(p, 1);
//                     }
//                     f = &((GrSprite *)e->frames)[pyro[i].life / pyro[i].unk14];
//                     p->u0 = f->u0;
//                     p->v0 = f->v0;
//                     p->u1 = f->u0 + f->w;
//                     p->v1 = f->v0;
//                     p->u2 = f->u0;
//                     p->v2 = f->v0 + f->h;
//                     p->u3 = f->u0 + f->w;
//                     p->v3 = f->v0 + f->h;
//                     p->tpage = f->tpage;
//                     p->clut = f->clut;
//                     if (e->unk0C == 1) {
//                         v[0] = pyro[i].pos.vx + eye->x;
//                         v[1] = pyro[i].pos.vy + eye->y;
//                         v[2] = pyro[i].pos.vz + eye->z + e->unk20;
//                         mathMulVec(mat, v, r);
//                         ny = cy + (r[1] * vfov / zz);
//                         nx = cx + (r[0] * hfov / zz);
//                         ang = ratan2(ny - py, nx - px) + 512;
//                         dx = (rcos(ang) - rsin(ang)) * size >> 12;
//                         dy = (rsin(ang) + rcos(ang)) * size >> 12;   /* retail calls rsin first
//                         here (call_check --self --nm, 2026-09-19) */ p->x0 = nx - dx; p->y0 = ny
//                         - dy; p->x1 = nx + dx; p->y1 = ny + dy; p->x2 = px - dx; p->y2 = py - dy;
//                         p->x3 = px + dx;
//                         p->y3 = py + dy;
//                     } else {
//                         x0 = px - size;
//                         p->x0 = x0;
//                         d = size * 2;
//                         x1 = x0 + d;
//                         y0 = py - size;
//                         p->y0 = y0;
//                         p->x1 = x1;
//                         p->y1 = y0;
//                         y1 = y0 + d;
//                         p->x2 = x0;
//                         p->y2 = y1;
//                         p->x3 = x1;
//                         p->y3 = y1;
//                     }
//                     AddPrim(ot + otz, p);
//                 }
//             }
//         }
//     }
//     D_8018C09C = 1 - D_8018C09C;
// }
// #else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", draw_explosions);
// #endif

void do_simple_spark(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x1C);
    init_explosion(pos, 5, 1, 4, SparkInfo, 1);
}

void do_simple_explosion(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x21);
    init_explosion(pos, 0x10, 2, 0x10, AburstInfo, 1);
}

Explosion* do_blue_explosion(VECTOR3* pos)
{
    s32 range;
    Explosion* e;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x42);
    e = init_explosion(pos, 0x10, 2, 0x10, AburstInfo, 1);
    if (e != 0) {
        pyro[e->idx].unk28 = 1;
        do {
        } while (0);
    }
    return e;
}

#ifdef NON_MATCHING
void do_flamethrower_burst(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 6, 1, 2, &D_8019C640[rand() & 3], 0);
    if (e != 0) {
        e->vel.vx = (rand() & 7) - 3;
        e->vel.vy = (rand() & 7) - 3;
        /* the two `(rand()&0xF)-5` are the SAME source text twice: retail funnels both
           arms into ONE `sw ...,56($s0)` via a `j`, which only the ternary produces. */
        e->vel.vz = (((rand() & 0xF) - 5) > 0) ? 0 : ((rand() & 0xF) - 5);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", do_flamethrower_burst);
#endif

void do_mini_explosion(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x42);
    init_explosion(pos, 4, 2, 0x10, AburstInfo, 1);
}

void do_big_explosion(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x4B);
    init_explosion(pos, 0x10, 2, 0x10, AburstInfo, 1);
}

void do_bigger_explosion(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x5D);
    init_explosion(pos, 0x48, 1, 0x10, AburstInfo, 1);
}

void do_mondo_explosion(VECTOR3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x63);
    init_explosion(pos, 0x60, 1, 0x10, AburstInfo, 1);
}

void do_missile_plume(VECTOR3* pos)
{
    init_explosion(pos, 0x20, 1, 0x10, SmokeInfo, 0);
}

void do_flames(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0xC, 1, 8, FlameInfo, 1);
    if (e != 0) {
        e->unk2C = 0xC;
        e->unk0C = 1;
        e->unk20 = 0x50;
    }
}

void do_groundburst(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0xC, 1, 8, GburstInfo, 1);
    if (e != 0) {
        e->unk0C = 1;
        e->unk20 = 0x64;
    }
}

void do_big_flames(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x30, 1, 8, FlameInfo, 1);
    if (e != 0) {
        e->unk2C = 0xC;
        e->unk0C = 1;
        e->unk20 = 0xA0;
    }
}

void do_afterburner(VECTOR3* pos, VECTOR3* vel)
{
    Explosion* e;

    e = init_explosion(pos, 0x10, 3, 8, BurnInfo, 0);
    if (e != 0) {
        e->vel.vx = vel->vx;
        e->vel.vy = vel->vy;
        e->vel.vz = vel->vz;
    }
}

void do_flare(VECTOR3* pos)
{
    init_explosion(pos, 4, 6, 4, FlareInfo, 0);
}

void do_burn(VECTOR3* pos)
{
    init_explosion(pos, 8, 1, 2, &BurnInfo[rand() % 7], 0);
}

void do_flash(VECTOR3* pos)
{
    init_explosion(pos, 8, 1, 3, &BurnInfo[rand() % 6], 1);
}

void do_mflash(VECTOR3* pos)
{
    init_explosion(pos, 8, 1, 2, AburstInfo, 0);
}

void do_gun_plume(VECTOR3* pos)
{
    init_explosion(pos, 8, 1, 0x10, SmokeInfo, 0);
}

void do_smoke(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x10, 2, 0xC, SmokeInfo, 0);
    if (e != 0) {
        e->vel.vz = 0xA;
        e->unk2C = 2;
    }
}

void do_steam(VECTOR3* pos)
{
    init_explosion(pos, 8, 2, 0xC, SteamInfo, 1);
}

void do_big_smoke(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x20, 2, 0xC, SmokeInfo, 0);
    if (e != 0) {
        e->vel.vz = 0xA;
        e->unk2C = 2;
    }
}

void do_puff(VECTOR3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 4, 1, 0x10, SmokeInfo, 1);
    if (e != 0) {
        e->vel.vz = 6;
    }
}

void clear_explosions(void)
{
    s32 i;

    for (i = 0; i < 80; i++) {
        pyro[i].life = -1;
        pyro[i].idx = i;
    }
    explodeInitFragments();
}

#ifdef NON_MATCHING
void create_stars(void)
{
    s32 i;
    s32 x;
    s32 y;
    s32 d;

    for (i = 0; i < 100; i++) {
        starz[i].r = rand() % 100 + 105;
        starz[i].g = rand() % 100 + 105;
        starz[i].b = rand() % 100 + 105;
        starz[i].pos.vy = rand() - 16000;
        starz[i].pos.vx = rand() - 16000;
        x = __builtin_abs(starz[i].pos.vx);
        y = __builtin_abs(starz[i].pos.vy);
        d = 24000 - y - x;
        if (d < 1000) {
            d = rand() % 3000 + 1000;
        }
        starz[i].pos.vz = d;
        starz[i].phase = rand() % 512;
    }
    for (i = 0; i < 200; i++) {
        SetLineF2(&starprim[i]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", create_stars);
#endif

#ifdef NON_MATCHING
void display_stars(u_long* ot, s32 view)
{
    VECTOR pos;
    s32 cx;
    s32 cy;
    long flag;
    s32 hfov;
    s32 vfov;
    s32 i;
    s32 ph;
    s32 x;
    s32 y;
    LINE_F2* p;

    viewGetCenter(view, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(view);
    vfov = viewGetCurrentVertFOVH(view);
    SetTransMatrix(&starMatrix);
    SetRotMatrix((MATRIX*)viewGetEyeMat(view));
    for (i = 0; i < 100; i++) {
        ph = starz[i].phase;
        starz[i].phase = (ph + 1) % 512;
        RotTrans(&starz[i].pos, &pos, &flag);
        if (pos.vz > 0) {
            p = &starprim[i * 2 + (starz[i].phase & 1)];
            if (starz[i].phase != 0) {
                p->r0 = starz[i].r;
                p->g0 = starz[i].g;
                p->b0 = starz[i].b;
            } else {
                p->r0 = 255;
                p->g0 = 255;
                p->b0 = 255;
            }
            x = cx + pos.vx * hfov / (pos.vz + 1000);
            y = cy + pos.vy * vfov / (pos.vz + 1000);
            if ((u32)x < 512 && (u32)y < 512) {
                p->x0 = x;
                p->y0 = y;
                p->x1 = x;
                p->y1 = y;
                AddPrim(&ot[4090], p);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", display_stars);
#endif

void explodeStoreFlameAnimation(void* data)
{
    GrSprite* anim = FlameInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 10);
    for (i = 0, p = &anim[0].tpage; i < 10; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreExplosionAnimation(void* data)
{
    GrSprite* anim = AburstInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 16);
    for (i = 0, p = &anim[0].tpage; i < 16; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreGburstAnimation(void* data)
{
    GrSprite* anim = GburstInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 12);
    for (i = 0, p = &anim[0].tpage; i < 12; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreSmokeAnimation(void* data)
{
    GrSprite* anim;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, SmokeInfo, 16);
    anim = SteamInfo;
    grutilsParse3DSprite(data, anim, 16);
    for (i = 0, p = &anim[0].tpage; i < 16; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreBurnAnimation(void* data)
{
    GrSprite* anim = BurnInfo;

    grutilsParse3DSprite(data, anim, 8);
    bulSetFireballGraphics(anim);
}

void explodeStoreSparkAnimation(void* data)
{
    GrSprite* anim = SparkInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 4);
    for (i = 0, p = &anim[0].tpage; i < 4; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreContrailAnimation(void* data)
{
    GrSprite* anim = ContrailInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 8);
    bulSetContrailGraphics(anim);
    for (i = 0, p = &anim[0].tpage; i < 8; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

void explodeStoreFlareAnimation(void* data)
{
    GrSprite* anim = FlareInfo;

    grutilsParse3DSprite(data, anim, 4);
    bulSetEyeWeaponGraphics(anim);
}

void explodeStorePlasmaAnimation(void* data)
{
    GrSprite* anim = PlasmaInfo;
    u16* p;
    s32 i;

    grutilsParse3DSprite(data, anim, 4);
    bulSetPlasmaGraphics(anim);
    for (i = 0, p = &anim[0].tpage; i < 4; i++) {
        *p = (*p & 0x1F) + 32;
        p += 4;
    }
}

// TODO: ArmorInfo is in sbss and this would imply the TU is compiled with `-G8`,
// but the access here isn't done via GP. Leaving commented out for now.
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", explodeStoreArmorIcon);
// void explodeStoreArmorIcon(void *data)
// {
//     grutilsParse3DSprite(data, (GrSprite *)&ArmorInfo, 1);
// #ifdef NON_MATCHING
//     ArmorInfo.u0 += 6;
//     ArmorInfo.v0 += 2;
// #else
//     ArmorInfo.unk0 += 6;
//     ArmorInfo.unk1 += 2;
// #endif
// }

void explodeLoadFragTexture(s32 id, GrObj* data)
{
    if (data->kind != 0) {
        printf("\nError Parsing Fragment Sprite %d\n", id);
    } else if (id == 0x29E) {
        grutilsParse3DSprite(data, fragmenta, 4);
    } else if (id == 0x29F) {
        grutilsParse3DSprite(data, fragmentb, 4);
    } else if (id == 0x2A0) {
        grutilsParse3DSprite(data, fragmentc, 4);
    } else if (id == 0x2A1) {
        grutilsParse3DSprite(data, fragmentd, 4);
    }
}

void explodeInitFragments(void)
{
    s32 off;

    // TODO: Improve readability (the pointer arithmetic shouldn't be needed)
    for (off = 19 * sizeof(Fragment); off >= 0; off -= sizeof(Fragment)) {
        ((Fragment*)((u8*)fragment + off))->life = 0;
    }
}
s32 findFreeFragment(void)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (fragment[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

#ifdef NON_MATCHING
void explodeCreateFragments(s32 count, VECTOR* pos)
{
    s32 i;
    s32 idx;
    s32 r;
    VECTOR3* p;
    VECTOR3* v;

    for (i = 0; i < count; i++) {
        idx = findFreeFragment();
        if (idx < 0) {
            break;
        }
        fragment[idx].frame = 0;
        fragment[idx].life = 40;
        p = &fragment[idx].pos;
        p->vx = pos->vx;
        p->vy = pos->vy;
        p->vz = pos->vz;
        v = &fragment[idx].vel;
        v->vx = (rand() & 0x3F) - 32;
        v->vy = (rand() & 0x3F) - 32;
        v->vz = (rand() & 0x1F) + 16;
        r = rand() & 3;
        if (r == 0) {
            fragment[idx].anim = fragmenta;
        } else if (r == 1) {
            fragment[idx].anim = fragmentb;
        } else if (r == 2) {
            fragment[idx].anim = fragmenta;
        } else {
            fragment[idx].anim = fragmentb;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", explodeCreateFragments);
#endif

#ifdef NON_MATCHING
void explodeUpdateFragments(void)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (fragment[i].life > 0) {
            s32 dz;

            fragment[i].pos.vz = (s32)((u32)fragment[i].pos.vz + (u32)fragment[i].vel.vz);
            fragment[i].pos.vx = (s32)((u32)fragment[i].pos.vx + (u32)fragment[i].vel.vx);
            fragment[i].pos.vy = (s32)((u32)fragment[i].pos.vy + (u32)fragment[i].vel.vy);
            dz = (s32)((u32)fragment[i].vel.vz - 4u);
            fragment[i].vel.vz = dz;
            if (fragment[i].pos.vz <= 0 && dz < 0) {
                if (dz >= -5) {
                    fragment[i].life = 0;
                } else {
                    fragment[i].vel.vz = (s32)(0u - (u32)dz) / 2;
                }
            }
            if (fragment[i].life > 0) {
                fragment[i].life--;
                fragment[i].frame = (fragment[i].frame + 1) & 3;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", explodeUpdateFragments);
#endif

#ifdef NON_MATCHING
void explodeDisplayFragments(Db* cdb, s32 which)
{
    s32 i;
    s32 frame;
    s32 cx;
    s32 cy;
    s32 hfov;
    s32 vfov;
    EyeTrans* eye;
    EyeMat* mat;
    u32* ot;
    s32 z;
    s32 sx;
    s32 sy;
    s32 size;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    s32 d;
    POLY_FT4* p;
    s32 v[3];
    s32 r[3];

    ot = (u32*)cdb->small;
    viewGetCenter(which, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(which);
    vfov = viewGetCurrentVertFOVH(which);
    eye = viewGetEyeTrans(which);
    mat = viewGetEyeMat(which);
    for (i = 0; i < 20; i++) {
        if (fragment[i].life > 0) {
            frame = fragment[i].frame;
            v[0] = fragment[i].pos.vx + eye->x;
            v[1] = fragment[i].pos.vy + eye->y;
            v[2] = fragment[i].pos.vz + eye->z;
            mathMulVec(mat, v, r);
            z = r[2];
            if (z >= -32) {
                if (z < 32001) {
                    if (z < 8) {
                        z = 8;
                    }
                    sx = r[0] * hfov / z;
                    sy = r[1] * vfov / z;
                    size = hfov * 10 / z;
                    z = z >> 3;
                    sx = cx + sx;
                    sy = cy + sy;
                    if (sx >= -99 && sx < 1000 && sy >= -99 && sy < 1000) {
                        p = (POLY_FT4*)cdb->unk8;
                        if ((u8*)p + 160 <= cdb->areaEnd) {
                            cdb->unk8 += 160;
                            setPolyFT4(p);
                            setShadeTex(p, 1);
                            p->u0 = fragment[i].anim[frame].u0;
                            p->v0 = fragment[i].anim[frame].v0;
                            p->u1 = fragment[i].anim[frame].u0 + fragment[i].anim[frame].w;
                            p->v1 = fragment[i].anim[frame].v0;
                            p->u2 = fragment[i].anim[frame].u0;
                            p->v2 = fragment[i].anim[frame].v0 + fragment[i].anim[frame].h;
                            p->u3 = fragment[i].anim[frame].u0 + fragment[i].anim[frame].w;
                            p->v3 = fragment[i].anim[frame].v0 + fragment[i].anim[frame].h;
                            p->tpage = fragment[i].anim[frame].tpage;
                            p->clut = fragment[i].anim[frame].clut;
                            y0 = sy - size;
                            x0 = sx - size;
                            d = size * 2;
                            x1 = x0 + d;
                            p->x0 = x0;
                            p->y0 = y0;
                            p->x1 = x1;
                            p->y1 = y0;
                            y1 = y0 + d;
                            p->x2 = x0;
                            p->y2 = y1;
                            p->x3 = x1;
                            p->y3 = y1;
                            AddPrim(ot + z, p);
                        }
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", explodeDisplayFragments);
#endif

ArmorIconInfo* explodeGetArmorIcon(void)
{
    return &ArmorInfo;
}

#ifdef NON_MATCHING
void explodeMakeScreenRed(void)
{
    Db* cdb;
    POLY_F4* p;
    DR_MODE* dm;

    cdb = rtGetCdb();
    p = (POLY_F4*)cdb->unk8;
    if ((u8*)p + 144 < cdb->areaEnd) {
        cdb->unk8 += 96;
        setPolyF4(p);
        setRGB0(p, 255, 0, 0);
        setSemiTrans(p, 1);
        setXY4(p, 0, 0, 320, 0, 0, 240, 320, 240);
        addPrim(cdb->small, p);
        dm = (DR_MODE*)cdb->unk8;
        cdb->unk8 += 48;
        SetDrawMode(dm, 0, 0, 0, NULL);
        addPrim(cdb->small, dm);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", explodeMakeScreenRed);
#endif
