#include "common.h"

#include <LIBGTE.H>
#include <STDIO.H>

#include "tm1/CS.H"
#include "tm1/LIGHT.H"
#include "tm1/MATH.H"

#define CS_POOL_SIZE 90

extern void* sdk_memcpy();

// clang-format off
static MATRIX unitMatrix = IDENTITY_MATRIX;

static Cs* csList = NULL;

static Cs* world;
static s32 activeCsNum;
static s32 gCurrentCS;

static Cs csPool[CS_POOL_SIZE];

void csUpdMat(Cs* cs, MATRIX* out)
{
    MATRIX tm;
    VECTOR3 v;

    tm.t[0] = 0;
    tm.t[1] = 0;
    tm.t[2] = 0;
    v.vx = cs->parent->wpos.vx + cs->pos.vx;
    v.vy = cs->parent->wpos.vy + cs->pos.vy;
    v.vz = cs->parent->wpos.vz + cs->pos.vz;
    mathMulVec(&cs->mat, (VECTOR*)&v, (VECTOR*)&cs->wpos);
    TransposeMatrix(&cs->mat, &tm);
    SetRotMatrix(&cs->parent->wmat);
    SetTransMatrix(&cs->parent->wmat);
    MulRotMatrix0(&tm, &cs->wmat);
    mathMulVecLong(&cs->wmat, (VECTOR*)&cs->wpos, (VECTOR*)cs->wmat.t);
    SetRotMatrix(&cs->mat);
    SetTransMatrix(&cs->mat);
    MulRotMatrix0(&cs->parent->mat3, &cs->mat3);
    SetRotMatrix(&cs->mat);
    SetTransMatrix(&cs->mat);
    MulRotMatrix0(&cs->parent->mat4, &cs->mat4);
    TransposeMatrix(&cs->mat, &tm);
    SetRotMatrix(&cs->parent->env->light);
    SetTransMatrix(&cs->parent->env->light);
    MulRotMatrix0(&tm, out);
}

Cs* csGetCsList(void)
{
    return csList;
}

void csAddToCsList(Cs* cs)
{
    Cs* old = csList;
    csList = cs;
    cs->next = old;
}

#ifdef NON_MATCHING
Cs* csCreate(void)
{
    Cs* cs;

    if (gCurrentCS < CS_POOL_SIZE) {
        cs = &csPool[gCurrentCS];
        sdk_memcpy(&cs->mat, &unitMatrix, sizeof(MATRIX));
        sdk_memcpy(&cs->wmat, &unitMatrix, sizeof(MATRIX));
        cs->rot.vx = 0;
        cs->rot.vy = 0;
        cs->rot.vz = 0;
        cs->pos.vx = 0;
        cs->pos.vy = 0;
        cs->pos.vz = 0;
        cs->wpos.vx = 0;
        cs->wpos.vy = 0;
        cs->wpos.vz = 0;
        cs->mat.t[0] = 0;
        cs->mat.t[1] = 0;
        cs->mat.t[2] = 0;
        cs->wmat.t[0] = 0;
        cs->wmat.t[1] = 0;
        cs->wmat.t[2] = 0;
        cs->mat3.t[0] = 0;
        cs->mat3.t[1] = 0;
        cs->mat3.t[2] = 0;
        cs->mat4.t[0] = 0;
        cs->mat4.t[1] = 0;
        cs->mat4.t[2] = 0;
        cs->parent = world;
        cs->epNode = NULL;
        cs->unk00 = 0;
        cs->unk0C = 1;
        cs->unkC0 = 0;
        cs->drawMode = 1;
        cs->next = NULL;
        cs->env = lightGetEnv(0);
        gCurrentCS++;
        return cs;
    }
    printf("internal error: too many coordinate systems (%d)\n", gCurrentCS);
    return NULL;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/CS", csCreate);
#endif

void csSetEpNode(Cs* cs, void* node)
{
    cs->epNode = node;
}

void csSetNumInActiveDb(s32 n)
{
    activeCsNum = n;
}

void csInit(void)
{
    gCurrentCS = 0;
    world = csCreate();
    world->unkC0 = 8;
    csList = NULL;
}

Cs* csGetWorldCs(void)
{
    return world;
}

void csSetDrawMode(Cs* cs, s32 mode)
{
    cs->drawMode = mode & 0xFF;
}
