#include "common.h"

#include "tm1/explosion.h"
#include "tm1/grutils.h"
#include "tm1/rt.h"
#include "tm1/view.h"

#include "tm1/explode.h"

extern AnimFrame fragmenta[4];
extern AnimFrame fragmentb[4];
extern AnimFrame fragmentc[4];
extern AnimFrame fragmentd[4];

extern Fragment fragment[20];

extern ArmorIconInfo ArmorInfo;

void explodeStoreFlameAnimation(void* data)
{
    AnimFrame* anim = FlameInfo;
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
    AnimFrame* anim = AburstInfo;
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
    AnimFrame* anim = GburstInfo;
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
    AnimFrame* anim;
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
    AnimFrame* anim = BurnInfo;

    grutilsParse3DSprite(data, anim, 8);
    bulSetFireballGraphics(anim);
}

void explodeStoreSparkAnimation(void* data)
{
    AnimFrame* anim = SparkInfo;
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
    AnimFrame* anim = ContrailInfo;
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
    AnimFrame* anim = FlareInfo;

    grutilsParse3DSprite(data, anim, 4);
    bulSetEyeWeaponGraphics(anim);
}

void explodeStorePlasmaAnimation(void* data)
{
    AnimFrame* anim = PlasmaInfo;
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
INCLUDE_ASM("asm/nonmatchings/tm1/explode", explodeStoreArmorIcon);
// void explodeStoreArmorIcon(void *data)
// {
//     grutilsParse3DSprite(data, (AnimFrame *)&ArmorInfo, 1);
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
    FragVec* p;
    FragVec* v;

    for (i = 0; i < count; i++) {
        idx = findFreeFragment();
        if (idx < 0) {
            break;
        }
        fragment[idx].frame = 0;
        fragment[idx].life = 40;
        p = &fragment[idx].pos;
        p->x = pos->vx;
        p->y = pos->vy;
        p->z = pos->vz;
        v = &fragment[idx].vel;
        v->x = (rand() & 0x3F) - 32;
        v->y = (rand() & 0x3F) - 32;
        v->z = (rand() & 0x1F) + 16;
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
INCLUDE_ASM("asm/nonmatchings/tm1/explode", explodeCreateFragments);
#endif

#ifdef NON_MATCHING
void explodeUpdateFragments(void)
{
    s32 i;

    for (i = 0; i < 20; i++) {
        if (fragment[i].life > 0) {
            s32 dz;

            fragment[i].pos.z = (s32)((u32)fragment[i].pos.z + (u32)fragment[i].vel.z);
            fragment[i].pos.x = (s32)((u32)fragment[i].pos.x + (u32)fragment[i].vel.x);
            fragment[i].pos.y = (s32)((u32)fragment[i].pos.y + (u32)fragment[i].vel.y);
            dz = (s32)((u32)fragment[i].vel.z - 4u);
            fragment[i].vel.z = dz;
            if (fragment[i].pos.z <= 0 && dz < 0) {
                if (dz >= -5) {
                    fragment[i].life = 0;
                } else {
                    fragment[i].vel.z = (s32)(0u - (u32)dz) / 2;
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
INCLUDE_ASM("asm/nonmatchings/tm1/explode", explodeUpdateFragments);
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
            v[0] = fragment[i].pos.x + eye->x;
            v[1] = fragment[i].pos.y + eye->y;
            v[2] = fragment[i].pos.z + eye->z;
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
INCLUDE_ASM("asm/nonmatchings/tm1/explode", explodeDisplayFragments);
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
INCLUDE_ASM("asm/nonmatchings/tm1/explode", explodeMakeScreenRed);
#endif
