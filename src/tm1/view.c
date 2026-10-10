#include "common.h"

#include <libgpu.h>
#include <stdio.h>

#include "tm1/math.h"
#include "tm1/rt.h"

#include "tm1/view.h"

// COMMON
extern s32 bgColor[3];

static ViewNode* sky[2] = { NULL, NULL };
static s32 numViews = 0;
static s32 gFov = 6;

// clang-format off
static MATRIX fovRotMat = {
    {
        {4096, 0, 0},
        {0, 0, -4096},
        {0, 4096, 0}
    },
    {0, 0, 0},
};

// clang-format off
static FovDat fovDat[14] = {
    /* @0x80170270 */
    {
        40, 32, 439, 439,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3848, 1400, 0, 0},
            {3848, 1400, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1078, -3951, 0},
            {0, 1078, 3951, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x801702D0 */
    {
        45, 35, 386, 386,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3784, 1567, 0, 0},
            {3784, 1567, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1215, -3911, 0},
            {0, 1215, 3911, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170330 */
    {
        50, 39, 343, 343,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3712, 1731, 0, 0},
            {3712, 1731, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1352, -3866, 0},
            {0, 1352, 3866, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170390 */
    {
        55, 43, 307, 307,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3633, 1891, 0, 0},
            {3633, 1891, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1489, -3815, 0},
            {0, 1489, 3815, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x801703F0 */
    {
        60, 47, 277, 277,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3547, 2048, 0, 0},
            {3547, 2048, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1627, -3758, 0},
            {0, 1627, 3758, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170450 */
    {
        65, 51, 251, 251,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3454, 2200, 0, 0},
            {3454, 2200, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1765, -3695, 0},
            {0, 1765, 3695, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x801704B0 */
    {
        70, 55, 228, 228,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3355, 2349, 0, 0},
            {3355, 2349, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1904, -3626, 0},
            {0, 1904, 3626, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170510 */
    {
        75, 60, 208, 208,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3249, 2493, 0, 0},
            {3249, 2493, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 2043, -3550, 0},
            {0, 2043, 3550, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170570 */
    {
        80, 64, 190, 190,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3137, 2632, 0, 0},
            {3137, 2632, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 2181, -3466, 0},
            {0, 2181, 3466, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x801705D0 */
    {
        85, 69, 174, 174,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3019, 2767, 0, 0},
            {3019, 2767, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 2319, -3375, 0},
            {0, 2319, 3375, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170630 */
    {
        90, 74, 160, 160,
        320, 240, 320, 240,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-2896, 2896, 0, 0},
            {2896, 2896, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 2457, -3276, 0},
            {0, 2457, 3276, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170690 */
    {
        70, 55, 228, 228,
        320, 240, 320, 120,
        0, 0,
        4096, 3690, 3690, 0,
        {
            {-3454, 2200, 0, 0},
            {3454, 2200, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1765, -3695, 0},
            {0, 1765, 3695, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x801706F0 */
    {
        70, 55, 228, 228,
        320, 240, 320, 120,
        0, 120,
        4096, 3690, 3690, 0,
        {
            {-3454, 2200, 0, 0},
            {3454, 2200, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1765, -3695, 0},
            {0, 1765, 3695, 0},
            {0, -4095, 0, 0}
        },
    },
    /* @0x80170750 */
    {
        70, 55, 91, 91,
        320, 240, 101, 30,
        211, 20,
        -4096, 3690, 3690, 0,
        {
            {-3454, 2200, 0, 0},
            {3454, 2200, 0, 0},
            {0, 4095, 0, 0}
        },
        {
            {0, 1765, -3695, 0},
            {0, 1765, 3695, 0},
            {0, -4095, 0, 0}
        },
    },
};

// TODO: Cannot be matched yet due to `maspsx` `.sbss` misalignment.
extern ViewNode* ownship;
extern ViewNode* view[2];
extern ViewDb* curViewDb[2];
extern s32 curFovEntry[2];

extern MATRIX swap;
extern EyeTrans worldSpaceTrans[2];
extern EyeMat worldSpaceMat[2];

extern void* sdk_memcpy();

#ifdef NON_MATCHING
s32 viewGetCurrentFOVH(s32 which)
{
    return fovDat[curFovEntry[which]].fovh;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentFOVH);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentHorzFOVH(s32 which)
{
    return fovDat[curFovEntry[which]].fovh;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentHorzFOVH);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentVertFOVH(s32 which)
{
    return fovDat[curFovEntry[which]].vertFovh;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentVertFOVH);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentFOVDegrees(s32 which)
{
    return fovDat[curFovEntry[which]].fovDegrees;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentFOVDegrees);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentFOVDegreesHorz(s32 which)
{
    return fovDat[curFovEntry[which]].fovDegrees;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentFOVDegreesHorz);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentFOVDegreesVert(s32 which)
{
    return fovDat[curFovEntry[which]].fovDegreesVert;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentFOVDegreesVert);
#endif

#ifdef NON_MATCHING
void viewGetScreenSize(s32* w, s32* h)
{
    *w = fovDat[curFovEntry[0]].screenWidth;
    *h = fovDat[curFovEntry[0]].screenHeight;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetScreenSize);
#endif

#ifdef NON_MATCHING
void viewToggleView()
{
    s32 i;

    for (i = 0; i < numViews; i++) {
        curFovEntry[i]++;
        if (curFovEntry[i] > 10) {
            curFovEntry[i] = 0;
        }
        viewSetFovDat(curFovEntry[i], i);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewToggleView);
#endif

#ifdef NON_MATCHING
void viewInit()
{
    s32 i;

    numViews = 0;
    rtSetRearView(0);
    for (i = 0; i < 2; i++) {
        view[i] = 0;
        curViewDb[i] = 0;
        curFovEntry[i] = 6;
        sky[i] = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewInit);
#endif

#ifdef NON_MATCHING
void viewCreate(ViewDb* db, s32 entry, s32 which)
{
    s32 w;
    s32 h;
    s32 x;
    s32 y;
    s32 sw;
    s32 sh;

    if (which >= 2) {
        printf("Trying to create a view greater than MAX \n");
        return;
    }
    numViews++;
    if (numViews >= 3) {
        printf("Viewport already created, must destory all views fisrt \n");
        return;
    }
    curFovEntry[which] = entry;
    curViewDb[which] = db;
    w = fovDat[entry].centerWidth;
    h = fovDat[entry].centerHeight;
    x = fovDat[entry].drawX;
    y = fovDat[entry].drawY;
    sw = fovDat[entry].screenWidth;
    sh = fovDat[entry].screenHeight;
    SetDefDrawEnv(&db->draw0, x, y, w, h);
    SetDefDrawEnv(&db->draw1, x + sw, y, w, h);
    SetDefDispEnv(&db->disp0, sw, 0, sw, sh);
    SetDefDispEnv(&db->disp1, 0, 0, sw, sh);
    SetGeomScreen(fovDat[entry].fovh);
    SetGeomOffset(w / 2, h / 2);
    db->draw0.isbg = 1;
    db->draw1.isbg = 1;
    db->draw0.dtd = 0;
    db->draw1.dtd = 0;
    bgColor[0] = 0x14;
    bgColor[1] = 0x14;
    bgColor[2] = 0x14;
    db->draw0.r0 = bgColor[0];
    db->draw0.g0 = bgColor[1];
    db->draw0.b0 = bgColor[2];
    db->draw1.r0 = bgColor[0];
    db->draw1.g0 = bgColor[1];
    db->draw1.b0 = bgColor[2];
    ownship = csCreate();
    view[which] = csCreate();
    view[which]->parent = ownship;
    viewSetFovDat(entry, which);
    if (entry == 0xD) {
        rtSetRearView(1);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewCreate);
#endif

#ifdef NON_MATCHING
void viewSetFovDat(s32 entry, s32 which)
{
    s32 f;
    s32 w;
    s32 h;

    f = fovDat[entry].fovh;
    w = fovDat[entry].screenWidth;
    h = fovDat[entry].screenHeight;
    SetGeomScreen(f);
    SetGeomOffset(w / 2, h / 2);
    fovRotMat.m[0][0] = fovDat[entry].rotXX;
    fovRotMat.m[1][2] = -fovDat[entry].rotYZ;
    fovRotMat.m[2][1] = fovDat[entry].rotZY;
    swap = fovRotMat;
    view[which]->mat3.m[0][0] = fovDat[entry].rotA[0].vx;
    view[which]->mat3.m[1][0] = fovDat[entry].rotA[0].vy;
    view[which]->mat3.m[2][0] = fovDat[entry].rotA[0].vz;
    view[which]->mat3.m[0][1] = fovDat[entry].rotA[1].vx;
    view[which]->mat3.m[1][1] = fovDat[entry].rotA[1].vy;
    view[which]->mat3.m[2][1] = fovDat[entry].rotA[1].vz;
    view[which]->mat3.m[0][2] = fovDat[entry].rotA[2].vx;
    view[which]->mat3.m[1][2] = fovDat[entry].rotA[2].vy;
    view[which]->mat3.m[2][2] = fovDat[entry].rotA[2].vz;
    view[which]->mat3.t[0] = 0;
    view[which]->mat3.t[1] = 0;
    view[which]->mat3.t[2] = 0;
    view[which]->mat4.m[0][0] = fovDat[entry].rotB[0].vx;
    view[which]->mat4.m[1][0] = fovDat[entry].rotB[0].vy;
    view[which]->mat4.m[2][0] = fovDat[entry].rotB[0].vz;
    view[which]->mat4.m[0][1] = fovDat[entry].rotB[1].vx;
    view[which]->mat4.m[1][1] = fovDat[entry].rotB[1].vy;
    view[which]->mat4.m[2][1] = fovDat[entry].rotB[1].vz;
    view[which]->mat4.m[0][2] = fovDat[entry].rotB[2].vx;
    view[which]->mat4.m[1][2] = fovDat[entry].rotB[2].vy;
    view[which]->mat4.m[2][2] = fovDat[entry].rotB[2].vz;
    view[which]->mat4.t[0] = 0;
    view[which]->mat4.t[1] = 0;
    view[which]->mat4.t[2] = 0;
    gFov = fovDat[entry].fovDegrees;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetFovDat);
#endif

s32 viewGetFov()
{
    return gFov;
}

#ifdef NON_MATCHING
void viewSetFov(s32 fov)
{
    s32 i;
    s32 entry;

    gFov = fov;
    entry = (fov - 40) / 5;
    for (i = 0; i < numViews; i++) {
        viewSetFovDat(entry, i);
        curFovEntry[i] = entry;
    }
    gFov = fovDat[entry].fovDegrees;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetFov);
#endif

void viewUpdateFov()
{
    viewSetFov(gFov);
}

void viewTweekInit()
{
}

#ifdef NON_MATCHING
void viewProc(s32 which)
{
    MATRIX m0;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    s32 out[3];
    s32 negTrans[3];
    ViewNode* node;
    Cs* cs;
    s32* fe;

    fe = &curFovEntry[which];
    if (*fe == 13) {
        wordCp((s32*)&view[1]->rot, (s32*)&view[0]->rot, 3);
        wordCp((s32*)&view[1]->pos, (s32*)&view[0]->pos, 3);
        wordCp((s32*)&view[1]->parent->rot, (s32*)&view[0]->parent->rot, 3);
        wordCp((s32*)&view[1]->parent->pos, (s32*)&view[0]->parent->pos, 3);
        view[1]->rot.vz += 2048;
        view[1]->rot.vx += -56;
    }
    node = view[which];
    cs = csGetWorldCs();
    RotMatrixYXZ(&node->rot, &node->mat);
    RotMatrixYXZ(&node->parent->rot, &node->parent->mat);
    swap.m[0][0] = fovDat[*fe].rotXX;
    swap.m[1][2] = -fovDat[*fe].rotYZ;
    swap.m[2][1] = fovDat[*fe].rotZY;
    SetRotMatrix(&swap);
    SetTransMatrix(&swap);
    MulRotMatrix0(&node->mat, &m0);
    SetRotMatrix(&m0);
    SetTransMatrix(&m0);
    MulRotMatrix0(&node->parent->mat, &cs->wmat);
    negTrans[0] = -node->pos.vx;
    negTrans[1] = -node->pos.vy;
    negTrans[2] = -node->pos.vz;
    mathMulTransVec(&node->parent->mat, negTrans, out);
    cs->wpos.vx = out[0] - node->parent->pos.vx;
    cs->wpos.vy = out[1] - node->parent->pos.vy;
    cs->wpos.vz = out[2] - node->parent->pos.vz;
    mathMulVecLong(&cs->wmat, &cs->wpos, cs->wmat.t);
    wordCp((s32*)&worldSpaceTrans[which], (s32*)&cs->wpos, 3);
    wordCp((s32*)&worldSpaceMat[which], (s32*)&cs->wmat, 8);
    m0.t[0] = 0;
    m0.t[1] = 0;
    m0.t[2] = 0;
    m1.t[0] = 0;
    m1.t[1] = 0;
    m1.t[2] = 0;
    m2.t[0] = 0;
    m2.t[1] = 0;
    m2.t[2] = 0;
    m3.t[0] = 0;
    m3.t[1] = 0;
    m3.t[2] = 0;
    TransposeMatrix(&node->mat, &m0);
    TransposeMatrix(&node->parent->mat, &m1);
    SetRotMatrix(&m0);
    SetTransMatrix(&m0);
    MulRotMatrix0(&node->mat3, &m2);
    SetRotMatrix(&m0);
    SetTransMatrix(&m0);
    MulRotMatrix0(&node->mat4, &m3);
    SetRotMatrix(&m1);
    SetTransMatrix(&m1);
    MulRotMatrix0(&m2, &cs->mat3);
    SetRotMatrix(&m1);
    SetTransMatrix(&m1);
    MulRotMatrix0(&m3, &cs->mat4);
    SetGeomScreen(fovDat[*fe].fovh);
    SetGeomOffset(fovDat[*fe].centerWidth / 2, fovDat[*fe].centerHeight / 2);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewProc);
#endif

#ifdef NON_MATCHING
s32 viewGetCurrentFovSet(s32 which)
{
    return curFovEntry[which];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCurrentFovSet);
#endif

#ifdef NON_MATCHING
void viewSetParent(void* node, s32 which)
{
    if (node != 0) {
        if (view[which] != 0) {
            view[which]->parent = node;
        }
        return;
    }
    if (view[which] != 0) {
        view[which]->parent = ownship;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetParent);
#endif

#ifdef NON_MATCHING
void viewSetSky(ViewNode* node)
{
    sky[0] = node;
    node->unk00 = 0xFFE;
    if (rtIsSplitScreenOn()) {
        sky[1] = csCreate();
        sky[1]->unk00 = 0xFFE;
        csSetEpNode(sky[1], node->epNode);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetSky);
#endif

#ifdef NON_MATCHING
ViewNode* viewGetSky(s32 which)
{
    return sky[which];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetSky);
#endif

#ifdef NON_MATCHING
void viewSetSkyPosition(s32* pos, s32 which)
{
    if (sky[which] != 0) {
        sky[which]->pos.vx = pos[0];
        sky[which]->pos.vy = pos[1];
        sky[which]->pos.vz = pos[2];
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetSkyPosition);
#endif

#ifdef NON_MATCHING
void viewGetDeltaRot(void* dst, s32 which)
{
    sdk_memcpy(dst, &view[which]->rot, 8);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetDeltaRot);
#endif

#ifdef NON_MATCHING
void viewGetDeltaTrans(void* dst, s32 which)
{
    sdk_memcpy(dst, &view[which]->pos, 12);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetDeltaTrans);
#endif

#ifdef NON_MATCHING
void viewSetDeltaRot(void* src, s32 which)
{
    if (view[which] != 0) {
        sdk_memcpy(&view[which]->rot, src, 8);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetDeltaRot);
#endif

#ifdef NON_MATCHING
void viewSetDeltaTrans(void* src, s32 which)
{
    if (view[which] != 0) {
        sdk_memcpy(&view[which]->pos, src, 12);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetDeltaTrans);
#endif

#ifdef NON_MATCHING
void* viewGetOwnship(s32 which)
{
    return view[which]->parent;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetOwnship);
#endif

#ifdef NON_MATCHING
void viewSetBgColor()
{
    s32 i;
    ViewDb* db;

    for (i = 0; i < numViews; i++) {
        db = curViewDb[i];
        db->draw0.r0 = bgColor[0];
        db->draw0.g0 = bgColor[1];
        db->draw0.b0 = bgColor[2];
        db->draw1.r0 = bgColor[0];
        db->draw1.g0 = bgColor[1];
        db->draw1.b0 = bgColor[2];
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewSetBgColor);
#endif

#ifdef NON_MATCHING
void viewGetCenter(s32 which, s32* cx, s32* cy)
{
    *cx = fovDat[curFovEntry[which]].centerWidth / 2;
    *cy = fovDat[curFovEntry[which]].centerHeight / 2;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/view", viewGetCenter);
#endif

EyeTrans* viewGetEyeTrans(s32 which)
{
    return &worldSpaceTrans[which];
}

EyeMat* viewGetEyeMat(s32 which)
{
    return &worldSpaceMat[which];
}

void wordCp(s32* dst, s32* src, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++) {
        *dst = *src;
        src++;
        dst++;
    }
}
