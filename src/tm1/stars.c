#include "common.h"

#include <libgpu.h>
#include <libgte.h>
#include <rand.h>

#include "tm1/view.h"

#include "tm1/stars.h"

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
INCLUDE_ASM("asm/nonmatchings/tm1/stars", create_stars);
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
INCLUDE_ASM("asm/nonmatchings/tm1/stars", display_stars);
#endif
