#include "common.h"

#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/math.h"

#include "tm1/hd.h"
#include "tm1/hdp.h"

HdpEnt* fastHdpStack = (HdpEnt*)0x1F800000;

HdpEnt* hdpStack;
s32 hdp_stackIdx;

void hdpPush(void* node, s32* pos, s32 id0, s32 id1);
#ifdef NON_MATCHING
void hdpPush(void* node, s32* pos, s32 id0, s32 id1)
{
    HdpEnt* base;
    s32 i = hdp_stackIdx;

    do {
        if ((u32)i < 50) {
            base = fastHdpStack;
        } else {
            base = hdpStack;
        }
    } while (0);
    base[i].node = node;
    base[i].pos[0] = pos[0];
    base[i].pos[1] = pos[1];
    base[i].pos[2] = pos[2];
    base[i].id0 = id0;
    base[i].id1 = id1;
    if (hdp_stackIdx < 200) {
        hdp_stackIdx++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpPush);
#endif

s32 hdpPop(HdpEnt** out)
{
    HdpEnt* base;
    s32 i = hdp_stackIdx - 1;

    hdp_stackIdx = i;

    if (i >= 0) {
        if ((u32)i < 50) {
            base = fastHdpStack;
        } else {
            base = hdpStack;
        }
        base += i;
        *out = base;
    }
    return hdp_stackIdx;
}

#ifdef NON_MATCHING
void hdpSetNodeIds(s16* p0, s16* p1, HdpEnt* n, s16 id0, s16 id1)
{
    if (id0 == 0) {
        *p0 = n->id0;
        *p1 = n->id1;
    } else {
        *p0 = id0;
        *p1 = id1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpSetNodeIds);
#endif

#ifdef NON_MATCHING
s32 hdpIsPtInSphere(s32* p, s32 rad, s32* c)
{
    s32 d[3];
    s32 dist;

    if (rad <= 0xB504) {

        d[0] = (s32)((u32)p[0] - (u32)c[0]);
        d[1] = (s32)((u32)p[1] - (u32)c[1]);
        d[2] = (s32)((u32)p[2] - (u32)c[2]);
        dist = (s32)((u32)d[0] * (u32)d[0] + (u32)d[1] * (u32)d[1] + (u32)d[2] * (u32)d[2]);
        if (dist < 0 || dist >= (s32)((u32)rad * (u32)rad)) {
            return 0;
        }
        return 1;
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpIsPtInSphere);
#endif

#ifdef NON_MATCHING
void hdpTraverse(Cs* cs, s32* pt, HdCsHit* res)
{
    HdpEnt* e;
    s16 id0;
    s16 id1;
    void* node;
    s32* pos;
    s32 t0;
    s32 t1;
    u8 kind;

    node = cs->epNode;
    pos = pt;
    t0 = 0;
    t1 = 0;
    e = NULL;
    hdp_stackIdx = 0;
push:
    hdpPush(node, pos, t0, t1);
    while (res->depth == 0 && hdpPop(&e) >= 0) {
        kind = *(u8*)e->node;
        switch (kind) {
        case 0: {
            HdLeaf* n = (HdLeaf*)e->node;

            if (n->skip == 1) {
                break;
            }
            if (hdpIsPtInSphere(n->c, n->rad, e->pos) == 0) {
                break;
            }
            res->depth = hdpPntBox(&n->box, (VECTOR*)e->pos, &res->hit, 0);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->tag, n->tag2, e);
            break;
        }
        case 7: {
            HdBoxNode* n = (HdBoxNode*)e->node;

            res->depth = hdpPntBox(&n->box, (VECTOR*)e->pos, &res->hit, n->code == -2);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->tag, n->code, e);
            break;
        }
        case 8: {
            HdVol* n = (HdVol*)e->node;

            res->depth = hdpPntVolume(n, (VECTOR*)e->pos, res, n->code == -2);
            if (res->hit == 0) {
                break;
            }
            hdpSetHitTag(res, n->tag, n->tag2, e);
            break;
        }
        case 1: {
            HdGroup* n = (HdGroup*)e->node;
            s32 i;
            s32 cnt;

            if (n->flags & 1) {
                break;
            }
            if (hdpIsPtInSphere(n->c, n->rad, e->pos) == 0) {
                break;
            }
            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            cnt = n->cnt;
            i = 0;
            if (cnt != 0) {
                do {
                    hdpPush(n->child[i], e->pos, id0, id1);
                    i++;
                } while (i < cnt);
            }
            break;
        }
        case 2:
            hdpLod((HdLodNode*)e->node, e);
            break;
        case 4: {
            HdTrans* n = (HdTrans*)e->node;
            s32 i;

            if (n->flags & 1) {
                break;
            }
            e->pos[0] = e->pos[0] - n->ofs[0];
            e->pos[1] = e->pos[1] - n->ofs[1];
            e->pos[2] = e->pos[2] - n->ofs[2];
            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            for (i = 0; i < n->cnt; i++) {
                hdpPush(n->child[i], e->pos, id0, id1);
            }
            break;
        }
        case 5: {
            HdRot* n = (HdRot*)e->node;
            VECTOR v;
            s32 dead[2];
            s32 i;

            if (n->flags & 1) {
                break;
            }
            e->pos[0] = e->pos[0] - n->mat.t[0];
            e->pos[1] = e->pos[1] - n->mat.t[1];
            e->pos[2] = e->pos[2] - n->mat.t[2];
            v.vx = e->pos[0];
            v.vy = e->pos[1];
            v.vz = e->pos[2];
            mathMulVec(&n->mat, &v, (VECTOR*)e->pos);
            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            for (i = 0; i < n->cnt; i++) {
                hdpPush(n->child[i], e->pos, id0, id1);
            }
            break;
        }
        case 9: {
            HdSwitch9* n = (HdSwitch9*)e->node;

            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            t0 = id0;
            t1 = id1;
            node = n->child[n->sel];
            pos = e->pos;
            goto push;
        }
        case 3: {
            HdSwitch3* n = (HdSwitch3*)e->node;

            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            t0 = id0;
            t1 = id1;
            node = n->child[n->sel];
            pos = e->pos;
            goto push;
        }
        case 11: {
            HdCtrl* n = (HdCtrl*)e->node;

            if (n->flags & 1) {
                break;
            }
            hdpSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            hdpCtrlNode(e, id0, id1);
            break;
        }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpTraverse);
#endif

#ifdef NON_MATCHING
void hdpLod(HdLodNode* n, HdpEnt* e)
{
    s32 d[3];
    u32 dist;
    s32 i;
    s32 go;

    d[0] = n->c[0] - e->pos[0];
    d[1] = n->c[1] - e->pos[1];
    d[2] = n->c[2] - e->pos[2];
    d[0] = d[0] >> 3;
    d[1] = d[1] >> 3;
    d[2] = d[2] >> 3;
    dist = (d[0] * d[0]) + (d[1] * d[1]) + (d[2] * d[2]);
    go = 1;
    for (i = 0; i < n->cnt; i++) {
        if (go == 0) {
            break;
        }
        if (dist >= n->rec[i]->near && dist < n->rec[i]->far) {
            hdpPush(n->rec[i]->child, e->pos, e->id0, e->id1);
            go = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpLod);
#endif

#ifdef NON_MATCHING
s32 hdpPntBox(HdBox* b, VECTOR* pos, s32* hit, s32 mode)
{
    VECTOR p;
    VECTOR d;

    mathMulVec(&b->mat, pos, &p);
    d.vx = b->lo.vx + p.vx;
    if (d.vx > 0) {
        d.vy = b->lo.vy + p.vy;
        if (d.vy > 0) {
            d.vz = b->lo.vz + p.vz;
            if (d.vz > 0 && b->hi.vx - d.vx > 0 && b->hi.vy - d.vy > 0 && b->hi.vz - d.vz > 0) {
                *hit = 1;
            }
        }
    }
    if (*hit == 1 && mode != 0) {
        *hit = 0;
        return -2;
    }
    return *hit == 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpPntBox);
#endif

#ifdef NON_MATCHING
s32 hdpPntVolume(HdVol* n, VECTOR* pos, HdCsHit* res, s32 mode)
{
    VECTOR p;

    mathMulVec(&n->mat0, pos, &p);
    if (n->d0.vx + p.vx > 0 && n->d0.vy + p.vy > 0 && n->d0.vz + p.vz > 0) {
        if (n->nplane < 4) {
            res->hit = 1;
        } else {
            mathMulVec(&n->mat1, pos, &p);
            if (n->d1.vx + p.vx > 0
                && (n->nplane < 5
                    || (n->d1.vy + p.vy > 0 && (n->nplane < 6 || n->d1.vz + p.vz > 0)))) {
                res->hit = 1;
            }
        }
    }
    if (res->hit == 1 && mode != 0) {
        res->depth = -2;
        res->hit = 0;
        return res->depth;
    }
    return res->hit == 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpPntVolume);
#endif

#ifdef NON_MATCHING
void hdpCtrlNode(HdpEnt* e, s16 id0, s16 id1)
{
    HdCtrl* n;
    s32 i;
    s32 ok;

    n = e->node;
    i = 0;
    ok = 1;
    while (i < n->cnt) {
        if (hdpInCtrlBox(n->box[i], (VECTOR*)e->pos) != 0) {
            ok = 0;
        }
        i++;
        if (ok == 0) {
            break;
        }
    }
    if (ok != 0) {
        if (n->inNode != 0) {
            hdpPush(n->inNode, e->pos, id0, id1);
        }
    } else {
        if (n->outNode != 0) {
            hdpPush(n->outNode, e->pos, id0, id1);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpCtrlNode);
#endif

#ifdef NON_MATCHING
s32 hdpInCtrlBox(HdBox* b, VECTOR* pos)
{
    VECTOR p;
    VECTOR d;
    s32 r;

    mathMulVec(&b->mat, pos, &p);
    r = 0;
    d.vz = b->lo.vz + p.vz;
    if (d.vz >= 0) {
        d.vy = b->lo.vy + p.vy;
        if (d.vy >= 0) {
            d.vx = b->lo.vx + p.vx;
            if (d.vx >= 0 && b->hi.vx - d.vx >= 0 && b->hi.vy - d.vy >= 0) {
                r = b->hi.vz - d.vz >= 0;
            }
        }
    }
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpInCtrlBox);
#endif

#ifdef NON_MATCHING
void hdpSetHitTag(HdCsHit* r, s16 id0, s16 id1, HdpEnt* n)
{
    if (id0 == 0) {
        r->tag0 = n->id0;
        r->tag1 = n->id1;
    } else {
        r->tag0 = id0;
        r->tag1 = id1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", hdpSetHitTag);
#endif

#ifdef NON_MATCHING
HdCsHit* HdPntTest(s32 skip0, s32 skip1, VEC3* pt, s32* hitId)
{
    static HdCsHit tmpResult;
    Cs* cs;

    tmpResult.hit = 0;
    tmpResult.histIdx = -1;
    tmpResult.tag0 = -1;
    tmpResult.depth = 0;
    *hitId = 0;
    cs = csGetWorldCs();
    if (cs->epNode != 0) {
        hdpTraverse(cs, &pt->x, &tmpResult);
        if (tmpResult.hit == 1) {
            *hitId = cs->unkC0;
        }
    }
    if (*hitId == 0) {
        for (cs = csGetCsList(); cs != NULL && *hitId == 0; cs = cs->next) {
            if (cs->drawMode != 0 && cs->unkC0 != skip0 && cs->unkC0 != skip1 && cs->unk0C != 0
                && cs->unkC0 > 0 && cs->unkC0 < 80
                && hdpIsPtInSphere(&cs->pos.vx, ((HdCsInfo*)cs->epNode)->radius, &pt->x) != 0) {
                *hitId = cs->unkC0;
            }
        }
    }
    return &tmpResult;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hdp", HdPntTest);
#endif

void hdpInit(HdpEnt* stack)
{
    hdpStack = stack;
}
