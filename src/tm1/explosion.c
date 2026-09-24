#include "common.h"

#include "tm1/explosion.h"

Explosion* init_explosion(VEC3* pos, s32 a1, s32 a2, s32 a3, void* frames, u16 flag);
#ifdef NON_MATCHING
Explosion* init_explosion(VEC3* pos, s32 a1, s32 a2, s32 a3, void* frames, u16 flag)
{
    s32 i;
    Explosion* e;
    VEC3* v;

    i = find_free_explosion();
    if (i < 0) {
        return 0;
    }
    e = &pyro[i];
    e->pos.x = pos->x;
    e->pos.y = pos->y;
    e->pos.z = pos->z;
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
    v->x = 0;
    v->y = 0;
    v->z = 0;
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
        pyro[i].pos.x = (s32)((u32)pyro[i].pos.x + (u32)pyro[i].vel.x);
        pyro[i].pos.y = (s32)((u32)pyro[i].pos.y + (u32)pyro[i].vel.y);
        pyro[i].pos.z = (s32)((u32)pyro[i].pos.z + (u32)pyro[i].vel.z);
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
//         v[0] = pyro[i].pos.x + eye->x;
//         v[1] = pyro[i].pos.y + eye->y;
//         v[2] = pyro[i].pos.z + eye->z;
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
//                         v[0] = pyro[i].pos.x + eye->x;
//                         v[1] = pyro[i].pos.y + eye->y;
//                         v[2] = pyro[i].pos.z + eye->z + e->unk20;
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

void do_simple_spark(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x1C);
    init_explosion(pos, 5, 1, 4, SparkInfo, 1);
}

void do_simple_explosion(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x21);
    init_explosion(pos, 0x10, 2, 0x10, AburstInfo, 1);
}

Explosion* do_blue_explosion(VEC3* pos)
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
void do_flamethrower_burst(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 6, 1, 2, &D_8019C640[rand() & 3], 0);
    if (e != 0) {
        e->vel.x = (rand() & 7) - 3;
        e->vel.y = (rand() & 7) - 3;
        /* the two `(rand()&0xF)-5` are the SAME source text twice: retail funnels both
           arms into ONE `sw ...,56($s0)` via a `j`, which only the ternary produces. */
        e->vel.z = (((rand() & 0xF) - 5) > 0) ? 0 : ((rand() & 0xF) - 5);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/explosion", do_flamethrower_burst);
#endif

void do_mini_explosion(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x42);
    init_explosion(pos, 4, 2, 0x10, AburstInfo, 1);
}

void do_big_explosion(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x4B);
    init_explosion(pos, 0x10, 2, 0x10, AburstInfo, 1);
}

void do_bigger_explosion(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x5D);
    init_explosion(pos, 0x48, 1, 0x10, AburstInfo, 1);
}

void do_mondo_explosion(VEC3* pos)
{
    s32 range;

    soundSetRangeAndXPositionFromWorldLoc(pos);
    range = soundGetCalculatedSoundRange();
    uasoundPlayGeneralExplode(range, soundGetCalculatedSoundXPosition(), 0x63);
    init_explosion(pos, 0x60, 1, 0x10, AburstInfo, 1);
}

void do_missile_plume(VEC3* pos)
{
    init_explosion(pos, 0x20, 1, 0x10, SmokeInfo, 0);
}

void do_flames(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0xC, 1, 8, FlameInfo, 1);
    if (e != 0) {
        e->unk2C = 0xC;
        e->unk0C = 1;
        e->unk20 = 0x50;
    }
}

void do_groundburst(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0xC, 1, 8, GburstInfo, 1);
    if (e != 0) {
        e->unk0C = 1;
        e->unk20 = 0x64;
    }
}

void do_big_flames(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x30, 1, 8, FlameInfo, 1);
    if (e != 0) {
        e->unk2C = 0xC;
        e->unk0C = 1;
        e->unk20 = 0xA0;
    }
}

void do_afterburner(VEC3* pos, VEC3* vel)
{
    Explosion* e;

    e = init_explosion(pos, 0x10, 3, 8, BurnInfo, 0);
    if (e != 0) {
        e->vel.x = vel->x;
        e->vel.y = vel->y;
        e->vel.z = vel->z;
    }
}

void do_flare(VEC3* pos)
{
    init_explosion(pos, 4, 6, 4, FlareInfo, 0);
}

void do_burn(VEC3* pos)
{
    init_explosion(pos, 8, 1, 2, &BurnInfo[rand() % 7], 0);
}

void do_flash(VEC3* pos)
{
    init_explosion(pos, 8, 1, 3, &BurnInfo[rand() % 6], 1);
}

void do_mflash(VEC3* pos)
{
    init_explosion(pos, 8, 1, 2, AburstInfo, 0);
}

void do_gun_plume(VEC3* pos)
{
    init_explosion(pos, 8, 1, 0x10, SmokeInfo, 0);
}

void do_smoke(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x10, 2, 0xC, SmokeInfo, 0);
    if (e != 0) {
        e->vel.z = 0xA;
        e->unk2C = 2;
    }
}

void do_steam(VEC3* pos)
{
    init_explosion(pos, 8, 2, 0xC, SteamInfo, 1);
}

void do_big_smoke(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 0x20, 2, 0xC, SmokeInfo, 0);
    if (e != 0) {
        e->vel.z = 0xA;
        e->unk2C = 2;
    }
}

void do_puff(VEC3* pos)
{
    Explosion* e;

    e = init_explosion(pos, 4, 1, 0x10, SmokeInfo, 1);
    if (e != 0) {
        e->vel.z = 6;
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
