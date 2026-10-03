#include "common.h"

#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/math.h"
#include "tm1/timer.h"

#include "tm1/hd.h"
#include "tm1/hdp.h"

HdEnt* fastHdStack = (HdEnt*)0x1F800000;
s32* hdCsOtherPos = NULL;
MATRIX* hdCsOtherMat = NULL;
Cs* hdCsSelf = NULL;

HdEnt* hdStack;
s32 hd_stackIdx;
s32 matCnt;

void hdPush(void* node, s32* v, s32 matIdx, s32 id0, s16 id1)
{
    HdEnt* e;
    HdEnt* base;
    s32 idx;

    idx = hd_stackIdx;
    if ((u32)idx < 40) {
        do {
            base = fastHdStack;
        } while (0);
    } else {
        do {
            base = hdStack;
        } while (0);
    }
    e = (HdEnt*)(idx * (s32)sizeof(HdEnt) + (s32)base);
    e->node = node;
    e->v[0] = v[0];
    e->v[1] = v[1];
    e->v[2] = v[2];
    do {
        e->matIdx = matIdx;
        e->id0 = id0;
        e->id1 = id1;
    } while (0);
    if (hd_stackIdx < 200) {
        hd_stackIdx++;
    }
}

s32 hdPop(HdEnt** out)
{
    s32 i;
    HdEnt* b;

    i = hd_stackIdx - 1;
    hd_stackIdx = i;
    if (i >= 0) {
        if ((u32)i < 40) {
            b = fastHdStack;
        } else {
            b = hdStack;
        }
        b += i;
        *out = b;
    }
    return hd_stackIdx;
}

#ifdef NON_MATCHING
void hdSetNodeIds(s16* out0, s16* out1, HdEnt* e, s16 id0, s16 id1)
{
    if (id0 == 0) {
        *out0 = e->id0;
        *out1 = e->id1;
    } else {
        *out0 = id0;
        *out1 = id1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdSetNodeIds);
#endif

#ifdef NON_MATCHING
s32 hdIsPtInSphere(VECTOR* pt, s32 radius, VECTOR* centre, s32 slack)
{
    u32 d[3];
    u32 dist;

    if (radius <= 0xB504) {
        d[0] = (u32)pt->vx - (u32)centre->vx;
        d[1] = (u32)pt->vy - (u32)centre->vy;
        d[2] = (u32)pt->vz - (u32)centre->vz;
        dist = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
        if ((s32)dist < 0 || (s32)dist >= (s32)((u32)radius * (u32)radius + (u32)slack)) {
            return 0;
        }
        return 1;
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdIsPtInSphere);
#endif

#ifdef NON_MATCHING
void hdTraverse(Cs* cs, s32* pt, HdCsHit* res, s32 slack)
{
    HdEnt* e;
    s16 id0;
    s16 id1;
    void* node;
    s32* pos;
    s32 t0;
    s32 t1;
    s32 matIdx;
    s32 r;

    node = cs->epNode;
    pos = pt;
    t0 = 0;
    t1 = 0;
    e = NULL;
    hd_stackIdx = 0;
    matIdx = matCnt;
push:
    hdPush(node, pos, matIdx, t0, t1);
    while (res->depth == 0 && hdPop(&e) >= 0) {
        switch (*(u8*)e->node) {
        case 0: {
            HdLeaf* n = (HdLeaf*)e->node;

            if (n->skip == 1) {
                break;
            }
            if (hdIsPtInSphere((VECTOR*)n->c, n->rad, (VECTOR*)e->v, slack) == 0) {
                break;
            }
            res->depth = hdPntBox(&n->box, (VECTOR*)e->v, res, e->matIdx);
            if (res->hit == 0) {
                break;
            }
            hdSetHitTag(res, n->tag, n->tag2, e);
            break;
        }
        case 7: {
            HdBoxNode* n = (HdBoxNode*)e->node;

            if (n->code == -2) {
                r = hdpPntBox(&n->box, (VECTOR*)e->v, &res->hit, 1);
            } else {
                r = hdPntBox(&n->box, (VECTOR*)e->v, res, e->matIdx);
            }
            res->depth = r;
            if (res->hit == 0) {
                break;
            }
            hdSetHitTag(res, n->tag, n->code, e);
            break;
        }
        case 8: {
            HdVol* n = (HdVol*)e->node;

            if (n->code == -2) {
                r = hdpPntVolume(n, (VECTOR*)e->v, res, 1);
            } else {
                r = hdPntVolume(n, (VECTOR*)e->v, res, e->matIdx);
            }
            res->depth = r;
            if (res->hit == 0) {
                break;
            }
            hdSetHitTag(res, n->tag, n->tag2, e);
            break;
        }
        case 1: {
            HdGroup* n = (HdGroup*)e->node;
            s32 i;
            s32 cnt;

            if (n->flags & 1) {
                break;
            }
            if (!(n->flags & 4)) {
                if (hdIsPtInSphere((VECTOR*)n->c, n->rad, (VECTOR*)e->v, slack) == 0) {
                    break;
                }
            }
            cnt = n->cnt;
            i = 0;
            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            if (cnt != 0) {
                for (;;) {
                    hdPush(n->child[i], e->v, e->matIdx, id0, id1);
                    i++;
                    if (i >= cnt) {
                        break;
                    }
                }
            }
            break;
        }
        case 2:
            hdLod((HdLodNode*)e->node, e);
            break;
        case 4: {
            HdTrans* n = (HdTrans*)e->node;
            s32 i;

            if (n->flags & 1) {
                break;
            }
            e->v[0] = e->v[0] - n->ofs[0];
            e->v[1] = e->v[1] - n->ofs[1];
            e->v[2] = e->v[2] - n->ofs[2];
            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            for (i = 0; i < n->cnt; i++) {
                hdPush(n->child[i], e->v, e->matIdx, id0, id1);
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
            e->v[0] = e->v[0] - n->mat.t[0];
            e->v[1] = e->v[1] - n->mat.t[1];
            e->v[2] = e->v[2] - n->mat.t[2];
            v.vx = e->v[0];
            v.vy = e->v[1];
            v.vz = e->v[2];
            mathMulVec(&n->mat, &v, (VECTOR*)e->v);
            matPtrs[e->matIdx] = &n->mat;
            e->matIdx = e->matIdx + 1;
            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            for (i = 0; i < n->cnt; i++) {
                hdPush(n->child[i], e->v, e->matIdx, id0, id1);
            }
            break;
        }
        case 9: {
            HdSwitch9* n = (HdSwitch9*)e->node;

            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            t0 = id0;
            t1 = id1;
            node = n->child[n->sel];
            matIdx = e->matIdx;
            pos = e->v;
            goto push;
        }
        case 3: {
            HdSwitch3* n = (HdSwitch3*)e->node;

            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            t0 = id0;
            t1 = id1;
            node = n->child[n->sel];
            matIdx = e->matIdx;
            pos = e->v;
            goto push;
        }
        case 11: {
            HdCtrl* n = (HdCtrl*)e->node;

            if (n->flags & 1) {
                break;
            }
            hdSetNodeIds(&id0, &id1, e, n->tag, n->tag2);
            hdCtrlNode(e, id0, id1);
            break;
        }
        }
    }
    matCnt = e->matIdx;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdTraverse);
#endif

#ifdef NON_MATCHING
void hdLod(HdLodNode* node, HdEnt* e)
{
    s32 d[3];
    u32 dist;
    s32 i;
    s32 more;

    d[0] = node->c[0] - e->v[0];
    d[1] = node->c[1] - e->v[1];
    d[2] = node->c[2] - e->v[2];
    d[0] = d[0] >> 3;
    d[1] = d[1] >> 3;
    d[2] = d[2] >> 3;
    more = 1;
    i = 0;
    dist = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    for (i = 0; i < node->cnt; i++) {
        if (more == 0) {
            break;
        }
        if (dist >= node->rec[i]->near && dist < node->rec[i]->far) {
            hdPush(node->rec[i]->child, e->v, e->matIdx, e->id0, e->id1);
            more = 0;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdLod);
#endif

#ifdef NON_MATCHING
s32 hdPntBox(HdBox* b, VECTOR* pos, HdCsHit* res, s32 matIdx)
{
    VECTOR out;
    s32 d0[6];
    s32 d1[6];
    VECTOR q;
    VECTOR p;
    VECTOR tmp;
    VECTOR w;
    VECTOR n0;
    VECTOR n1;
    s32 fwd;
    s32 i;
    s32 j;
    s32 k;
    s32 first;
    s32 second;
    s32 cnt;

    fwd = GetFrameCount() & 1;
    i = 0;
    if (fwd == 0) {
        i = hdCsSelf->nhist - 1;
    }
    while (res->hit != 1 && i >= 0) {
        if (i >= hdCsSelf->nhist) {
            break;
        }
        mathMulTransVecShort(&hdCsSelf->mat, &hdCsSelf->hist[i], &p);
        worldToTp[i][0] = p.vx + hdCsSelf->pos.vx;
        worldToTp[i][1] = p.vy + hdCsSelf->pos.vy;
        worldToTp[i][2] = p.vz + hdCsSelf->pos.vz;
        for (j = 0; j < matIdx; j++) {
            tmp.vx = p.vx;
            tmp.vy = p.vy;
            tmp.vz = p.vz;
            mathMulVec(matPtrs[j], &tmp, &p);
        }
        p.vx = p.vx + pos->vx;
        p.vy = p.vy + pos->vy;
        p.vz = p.vz + pos->vz;
        mathMulVec(&b->mat, &p, &out);
        d0[0] = b->lo.vx + out.vx;
        if (d0[0] > 0) {
            d0[1] = b->lo.vy + out.vy;
            if (d0[1] > 0) {
                d0[2] = b->lo.vz + out.vz;
                if (d0[2] > 0) {
                    d0[3] = b->hi.vx - d0[0];
                    if (d0[3] > 0) {
                        d0[4] = b->hi.vy - d0[1];
                        if (d0[4] > 0) {
                            d0[5] = b->hi.vz - d0[2];
                            if (d0[5] > 0) {
                                res->histIdx = i;
                                res->hit = 1;
                            }
                        }
                    }
                }
            }
        }
        if (fwd != 0) {
            i++;
        } else {
            i--;
        }
    }
    if (res->hit != 1) {
        return 0;
    }
    k = res->histIdx;
    mathMulTransVecShort(hdCsOtherMat, &hdCsSelf->hist[k], &w);
    w.vx = w.vx + hdCsOtherPos[0];
    w.vy = w.vy + hdCsOtherPos[1];
    w.vz = w.vz + hdCsOtherPos[2];
    q.vx = w.vx - worldToTp[k][0];
    q.vy = w.vy - worldToTp[k][1];
    j = 0;
    q.vz = w.vz - worldToTp[k][2];
    for (j = 0; j < matIdx; j++) {
        tmp.vx = q.vx;
        tmp.vy = q.vy;
        tmp.vz = q.vz;
        mathMulVec(matPtrs[j], &tmp, &q);
    }
    q.vx = q.vx + p.vx;
    q.vy = q.vy + p.vy;
    q.vz = q.vz + p.vz;
    mathMulVec(&b->mat, &q, &out);
    d1[0] = b->lo.vx + out.vx;
    d1[1] = b->lo.vy + out.vy;
    d1[2] = b->lo.vz + out.vz;
    first = -1;
    second = 0;
    d1[3] = b->hi.vx - d1[0];
    i = 0;
    cnt = 0;
    d1[4] = b->hi.vy - d1[1];
    d1[5] = b->hi.vz - d1[2];
    for (i = 0; i < 6; i++) {
        if ((d0[i] & 0x80000000) != (d1[i] & 0x80000000)) {
            if (first >= 0) {
                second = i;
            } else {
                first = i;
            }
            if (i < 3) {
                res->normal[0] = b->mat.m[i][0];
                res->normal[1] = b->mat.m[i][1];
                res->normal[2] = b->mat.m[i][2];
            } else {
                res->normal[0] = -b->mat.m[i - 3][0];
                res->normal[1] = -b->mat.m[i - 3][1];
                res->normal[2] = -b->mat.m[i - 3][2];
            }
            cnt++;
        }
    }
    if (cnt < 2) {
        if (cnt != 0) {
            return d0[first];
        }
        res->unk04 = 1;
        return 1;
    }
    if (first < 3) {
        n0.vx = b->mat.m[first][0];
        n0.vy = b->mat.m[first][1];
        n0.vz = b->mat.m[first][2];
    } else {
        n0.vx = -b->mat.m[first - 3][0];
        n0.vy = -b->mat.m[first - 3][1];
        n0.vz = -b->mat.m[first - 3][2];
    }
    if (second < 3) {
        n1.vx = b->mat.m[second][0];
        n1.vy = b->mat.m[second][1];
        n1.vz = b->mat.m[second][2];
    } else {
        n1.vx = -b->mat.m[second - 3][0];
        n1.vy = -b->mat.m[second - 3][1];
        n1.vz = -b->mat.m[second - 3][2];
    }
    n0.vx = (n0.vx + n1.vx) >> 1;
    n0.vy = (n0.vy + n1.vy) >> 1;
    n0.vz = (n0.vz + n1.vz) >> 1;
    VectorNormal(&n0, (VECTOR*)&res->normal[0]);
    if (d0[first] < d0[second]) {
        return d0[second];
    }
    return d0[first];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdPntBox);
#endif

#ifdef NON_MATCHING
s32 hdPntVolume(HdVol* n, VECTOR* pos, HdCsHit* res, s32 matIdx)
{
    VECTOR o0;
    VECTOR o1;
    s32 da[6];
    s32 db[6];
    VECTOR q;
    VECTOR p;
    VECTOR tmp;
    VECTOR w;
    VECTOR n0;
    VECTOR n1;
    s32 fwd;
    s32 i;
    s32 j;
    s32 k;
    s32 code;
    s32 first;
    s32 second;
    s32 cnt;

    fwd = GetFrameCount() & 1;
    if (fwd != 0) {
        i = 0;
    } else {
        i = hdCsSelf->nhist - 1;
    }
    while (res->hit != 1 && i >= 0) {
        if (i >= hdCsSelf->nhist) {
            break;
        }
        mathMulTransVecShort(&hdCsSelf->mat, &hdCsSelf->hist[i], &p);
        worldToTp[i][0] = p.vx + hdCsSelf->pos.vx;
        worldToTp[i][1] = p.vy + hdCsSelf->pos.vy;
        worldToTp[i][2] = p.vz + hdCsSelf->pos.vz;
        for (j = 0; j < matIdx; j++) {
            tmp.vx = p.vx;
            tmp.vy = p.vy;
            tmp.vz = p.vz;
            mathMulVec(matPtrs[j], &tmp, &p);
        }
        p.vx = p.vx + pos->vx;
        p.vy = p.vy + pos->vy;
        p.vz = p.vz + pos->vz;
        mathMulVec(&n->mat0, &p, &o0);
        da[0] = n->d0.vx + o0.vx;
        if (da[0] > 0) {
            da[1] = n->d0.vy + o0.vy;
            if (da[1] > 0) {
                da[2] = n->d0.vz + o0.vz;
                if (da[2] > 0) {
                    if (n->nplane < 4) {
                        goto inside;
                    }
                    mathMulVec(&n->mat1, &p, &o1);
                    da[3] = n->d1.vx + o1.vx;
                    if (da[3] > 0) {
                        if (n->nplane < 5) {
                            goto inside;
                        }
                        da[4] = n->d1.vy + o1.vy;
                        if (da[4] > 0) {
                            if (n->nplane < 6) {
                                goto inside;
                            }
                            da[5] = n->d1.vz + o1.vz;
                            if (da[5] > 0) {
                            inside:
                                res->hit = 1;
                            }
                        }
                    }
                }
            }
        }
        if (res->hit == 1) {
            res->histIdx = i;
        }
        if (fwd != 0) {
            i++;
        } else {
            i--;
        }
    }
    if (res->hit == 0) {
        return 0;
    }
    code = n->code;
    if (code >= 0) {
        if (code < 3) {
            res->normal[0] = n->mat0.m[code][0];
            res->normal[1] = n->mat0.m[code][1];
            res->normal[2] = n->mat0.m[code][2];
        } else {
            res->normal[0] = n->mat1.m[code - 3][0];
            res->normal[1] = n->mat1.m[code - 3][1];
            res->normal[2] = n->mat1.m[code - 3][2];
        }
        return da[code];
    }
    k = res->histIdx;
    mathMulTransVecShort(hdCsOtherMat, &hdCsSelf->hist[k], &w);
    w.vx = w.vx + hdCsOtherPos[0];
    w.vy = w.vy + hdCsOtherPos[1];
    w.vz = w.vz + hdCsOtherPos[2];
    q.vx = w.vx - worldToTp[k][0];
    q.vy = w.vy - worldToTp[k][1];
    j = 0;
    q.vz = w.vz - worldToTp[k][2];
    for (j = 0; j < matIdx; j++) {
        tmp.vx = q.vx;
        tmp.vy = q.vy;
        tmp.vz = q.vz;
        mathMulVec(matPtrs[j], &tmp, &q);
    }
    q.vx = q.vx + p.vx;
    q.vy = q.vy + p.vy;
    q.vz = q.vz + p.vz;
    mathMulVec(&n->mat0, &q, &o0);
    db[0] = n->d0.vx + o0.vx;
    db[1] = n->d0.vy + o0.vy;
    db[2] = n->d0.vz + o0.vz;
    first = -1;
    if (n->nplane >= 4) {
        mathMulVec(&n->mat1, &q, &o1);
        db[3] = n->d1.vx + o1.vx;
        db[4] = n->d1.vy + o1.vy;
        db[5] = n->d1.vz + o1.vz;
    }
    second = 0;
    i = 0;
    cnt = 0;
    for (i = 0; i < n->nplane; i++) {
        if ((da[i] & 0x80000000) != (db[i] & 0x80000000)) {
            if (first >= 0) {
                second = i;
            } else {
                first = i;
            }
            if (i < 3) {
                res->normal[0] = n->mat0.m[i][0];
                res->normal[1] = n->mat0.m[i][1];
                res->normal[2] = n->mat0.m[i][2];
            } else {
                res->normal[0] = n->mat1.m[i - 3][0];
                res->normal[1] = n->mat1.m[i - 3][1];
                res->normal[2] = n->mat1.m[i - 3][2];
            }
            cnt++;
        }
    }
    if (cnt < 2) {
        if (cnt != 0) {
            return da[first];
        }
        res->unk04 = 1;
        return 1;
    }
    if (first < 3) {
        n0.vx = n->mat0.m[first][0];
        n0.vy = n->mat0.m[first][1];
        n0.vz = n->mat0.m[first][2];
    } else {
        n0.vx = n->mat1.m[first - 3][0];
        n0.vy = n->mat1.m[first - 3][1];
        n0.vz = n->mat1.m[first - 3][2];
    }
    if (second < 3) {
        n1.vx = n->mat0.m[second][0];
        n1.vy = n->mat0.m[second][1];
        n1.vz = n->mat0.m[second][2];
    } else {
        n1.vx = n->mat1.m[second - 3][0];
        n1.vy = n->mat1.m[second - 3][1];
        n1.vz = n->mat1.m[second - 3][2];
    }
    n0.vx = (n0.vx + n1.vx) >> 1;
    n0.vy = (n0.vy + n1.vy) >> 1;
    n0.vz = (n0.vz + n1.vz) >> 1;
    VectorNormal(&n0, (VECTOR*)&res->normal[0]);
    if (da[first] < da[second]) {
        return da[second];
    }
    return da[first];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdPntVolume);
#endif

#ifdef NON_MATCHING
void hdCtrlNode(HdEnt* e, s16 id0, s16 id1)
{
    HdCtrl* node;
    s32 i;
    s32 in;

    node = e->node;
    i = 0;
    in = 1;
    while (i < node->cnt) {
        if (hdInCtrlBox(node->box[i], e->v) != 0) {
            in = 0;
        }
        i++;
        if (in == 0) {
            break;
        }
    }
    if (in != 0) {
        if (node->inNode != 0) {
            hdPush(node->inNode, e->v, e->matIdx, id0, id1);
        }
    } else {
        if (node->outNode != 0) {
            hdPush(node->outNode, e->v, e->matIdx, id0, id1);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdCtrlNode);
#endif

#ifdef NON_MATCHING
s32 hdInCtrlBox(HdBox* box, s32* pt)
{
    VECTOR r;
    s32 q[3];
    s32 res;

    mathMulVec(&box->mat, (VECTOR*)pt, &r);
    res = 0;
    q[2] = box->lo.vz + r.vz;
    if (q[2] >= 0) {
        q[1] = box->lo.vy + r.vy;
        if (q[1] >= 0) {
            q[0] = box->lo.vx + r.vx;
            if (q[0] >= 0 && (box->hi.vx - q[0]) >= 0 && (box->hi.vy - q[1]) >= 0) {
                res = (box->hi.vz - q[2]) >= 0;
            }
        }
    }
    return res;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdInCtrlBox);
#endif

#ifdef NON_MATCHING
void hdSetHitTag(HdCsHit* hit, s16 id0, s16 id1, HdEnt* e)
{
    if (id0 == 0) {
        hit->tag0 = e->id0;
        hit->tag1 = e->id1;
    } else {
        hit->tag0 = id0;
        hit->tag1 = id1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", hdSetHitTag);
#endif

#ifdef NON_MATCHING
void HdCsTest(Cs* cs, HdCsHit* hit, s32 doWorld, MATRIX* otherMat, s32* otherPos)
{
    VECTOR delta;
    VECTOR local;
    VECTOR tmp;
    Cs* p;
    s32 rad;
    s32 i;
    MATRIX** m;
    HdCsInfo* info;

    hit->hit = 0;
    hit->unk04 = 0;
    hit->obj = 0;
    hit->histIdx = -1;
    hit->tag0 = -1;
    hit->depth = 0;
    hdCsOtherMat = otherMat;
    hdCsSelf = cs;
    hdCsOtherPos = otherPos;
    info = cs->epNode;
    if (info == 0 || info->kind != 1) {
        rad = 100;
    } else {
        rad = info->radius;
    }
    p = csGetWorldCs();
    rad = rad * rad;
    if (p->epNode != NULL && doWorld != 0) {
        matCnt = 0;
        hit->obj = p;
        hdTraverse(p, &cs->pos.vx, hit, rad);
        if (hit->depth == -2) {
            hit->depth = 0;
        }
    }
    p = csGetCsList();
    while (hit->depth == 0) {
        if (p == 0) {
            break;
        }
        matCnt = 0;
        if (p->drawMode != 0 && p != cs && p->unk0C != 0) {
            matPtrs[0] = &p->mat;
            delta.vx = cs->pos.vx - p->pos.vx;
            delta.vy = cs->pos.vy - p->pos.vy;
            matCnt = 1;
            delta.vz = cs->pos.vz - p->pos.vz;
            mathMulVec(&p->mat, &delta, &local);
            hdTraverse(p, (s32*)&local, hit, rad);
            if (hit->hit == 1) {
                hit->obj = p;
            }
        }
        p = p->next;
    }
    if (hit->hit == 1) {
        i = matCnt - 1;
        if (i >= 0) {
            m = &matPtrs[i];
            do {
                tmp.vx = hit->normal[0];
                tmp.vy = hit->normal[1];
                tmp.vz = hit->normal[2];
                mathMulTransVec(*m, &tmp, (VECTOR*)&hit->normal[0]);
                i--;
                m--;
            } while (i >= 0);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hd", HdCsTest);
#endif

void hdInit(HdEnt* stack)
{
    hdStack = stack;
}
