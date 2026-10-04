#include "common.h"

#include <libgpu.h>
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/hd.h"
#include "tm1/hdp.h"
#include "tm1/math.h"
#include "tm1/view.h"

#include "tm1/hier.h"

#define SPAD_DRMODE0 ((DR_MODE*)0x1F800060)
#define SPAD_DRMODE1 ((DR_MODE*)0x1F80006C)
#define sdSubPolyCount (*(s32*)0x1F800038)
#define sdSubPolyPtr (*(s32**)0x1F80004C)
#define sdFogEnable (*(s32*)0x1F800088)

static s32 fovDepth = 1200;
static s32 fovDepthCfg = 1200;
HierEnt* fastStack = (HierEnt*)0x1F800000;
static s32 lodScale = 3;

static s32 stackIdx;
static s32 objCnt;
static s32 matIdx;
static s32 eoCnt;
static s32 rotCnt;
static s32 transCnt;
static s32 groupCnt;
static s32 lodCnt;
static s32 switchCnt;
static s32 polyCnt;
static s32 lodShiftValue;
static s32 groundFlag;

extern void geomProc(Db* db, ObjRec* objs, s32 nobj, MATRIX* mats, MATRIX* lights);
extern void subPoly3(Db* db, s32* list, s32 n);
extern void subPoly4(Db* db, s32* list, s32 n);

#ifdef NON_MATCHING
void hierPush(void* node, s32 mat, s32* pos, s32 eo)
{
    s32 i = stackIdx;

    if ((u32)i < 40) {
        fastStack[i].node = node;
        fastStack[i].matIdx = mat;
        fastStack[i].eoCnt = eo;
        fastStack[i].pos[0] = pos[0];
        fastStack[i].pos[1] = pos[1];
        fastStack[i].pos[2] = pos[2];
    } else {
        hierStack[i].node = node;
        hierStack[i].matIdx = mat;
        hierStack[i].eoCnt = eo;
        hierStack[i].pos[0] = pos[0];
        hierStack[i].pos[1] = pos[1];
        hierStack[i].pos[2] = pos[2];
    }
    if (stackIdx < 150) {
        stackIdx = stackIdx + 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierPush);
#endif

#ifdef NON_MATCHING
s32 hierPop(HierEnt** out)
{
    s32 i = --stackIdx;

    if (i >= 0) {
        if ((u32)i < 40) {
            *out = &fastStack[i];
        } else {
            *out = &hierStack[i];
        }
    }
    return stackIdx;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierPop);
#endif

#ifdef NON_MATCHING
void hierTraverse(Cs* cs, void* node)
{
    HierEnt* e;
    HierObj* obj;
    HierGroup* grp;
    HierEo* eo;
    HierRot* rot;
    HierSwitch* sw;
    HierAnim* an;
    s32 ok;

    e = 0;
    wordCopy((s32*)&matList[matIdx], (s32*)&cs->wmat, 8);
    stackIdx = 0;
    matInfo[matIdx].mat = &cs->wmat;
    matInfo[matIdx].light = &lightList[matIdx];
    matInfo[matIdx].fov0 = &cs->mat3;
    matInfo[matIdx].fov1 = &cs->mat4;
    hierPush(node, matIdx, (s32*)&cs->wpos, eoCnt);
    while (hierPop(&e) >= 0) {
        switch (*(u8*)e->node) {
        case 0: {
            s32 no;

            obj = (HierObj*)e->node;
            ok = 1;
            if (!(obj->flags & 0x2000)) {
                ok = hierIsInFov((VECTOR*)obj->ofs, obj->dist, (VECTOR*)e->pos,
                    matInfo[e->matIdx].fov0, matInfo[e->matIdx].fov1);
            }
            if (ok == 1) {
                no = objCnt;
                if (no < 220) {
                    objList[no].node = obj;
                    objList[no].matIdx = e->matIdx;
                    objList[no].cs = cs;
                    objList[no].eoCnt = e->eoCnt;
                    objList[no].pos[0] = e->pos[0];
                    objList[no].pos[1] = e->pos[1];
                    objList[no].pos[2] = e->pos[2];
                    objCnt = no + 1;
                }
            }
        } break;
        case 1:
            grp = (HierGroup*)e->node;
            if (!(grp->flags & 2)) {
                if (!(grp->flags & 4)) {
                    ok = hierIsInFov((VECTOR*)grp->ofs, grp->dist, (VECTOR*)e->pos,
                        matInfo[e->matIdx].fov0, matInfo[e->matIdx].fov1);
                } else {
                    ok = 1;
                }
                if (ok == 1) {
                    s32 i;
                    s32 n;

                    n = grp->count;
                    i = 0;
                    if (n != 0) {
                        do {
                            hierPush(grp->child[i], e->matIdx, e->pos, e->eoCnt);
                            i++;
                        } while (i < n);
                    }
                }
            }
            break;
        case 2:
            hierLod((HierLod*)e->node, e);
            break;
        case 4: {
            s32 i;

            eo = (HierEo*)e->node;
            if (!(eo->flags & 2)) {
                e->pos[0] = e->pos[0] + eo->ofs[0];
                e->pos[1] = e->pos[1] + eo->ofs[1];
                e->pos[2] = e->pos[2] + eo->ofs[2];
                eoCnt = eoCnt + 1;
                i = 0;
                while (i < eo->count) {
                    hierPush(eo->child[i], e->matIdx, e->pos, eoCnt);
                    i++;
                }
            }
        } break;
        case 5: {
            MATRIX m;
            VECTOR v;
            RECT unused;
            s32 i;

            rot = (HierRot*)e->node;
            if (!(rot->flags & 2)) {
                e->pos[0] = e->pos[0] + rot->mat.t[0];
                e->pos[1] = e->pos[1] + rot->mat.t[1];
                e->pos[2] = e->pos[2] + rot->mat.t[2];
                v.vx = e->pos[0];
                v.vy = e->pos[1];
                v.vz = e->pos[2];
                mathMulVec(&rot->mat, &v, (VECTOR*)e->pos);
                if (matIdx < 70) {
                    matIdx = matIdx + 1;
                    TransposeMatrix(&rot->mat, &m);
                    SetRotMatrix(matInfo[e->matIdx].mat);
                    MulRotMatrix0(&m, &matList[matIdx]);
                    matInfo[matIdx].mat = &matList[matIdx];
                    TransposeMatrix(&rot->mat, &m);
                    SetRotMatrix(matInfo[e->matIdx].light);
                    MulRotMatrix0(&m, &lightList[matIdx]);
                    matInfo[matIdx].light = &lightList[matIdx];
                    SetRotMatrix(&rot->mat);
                    SetTransMatrix(&rot->mat);
                    MulRotMatrix0(matInfo[e->matIdx].fov0, &fovNorms1[matIdx]);
                    matInfo[matIdx].fov0 = &fovNorms1[matIdx];
                    SetRotMatrix(&rot->mat);
                    SetTransMatrix(&rot->mat);
                    MulRotMatrix0(matInfo[e->matIdx].fov1, &fovNorms2[matIdx]);
                    matInfo[matIdx].fov1 = &fovNorms2[matIdx];
                }
                eoCnt = eoCnt + 1;
                i = 0;
                while (i < rot->count) {
                    hierPush(rot->child[i], matIdx, e->pos, eoCnt);
                    i++;
                }
            }
        } break;
        case 9:
            sw = (HierSwitch*)e->node;
            hierPush(sw->child[sw->sel], e->matIdx, e->pos, e->eoCnt);
            break;
        case 3:
            an = (HierAnim*)e->node;
            if (an->mode == 1) {
                hierAnimateBiDirect(an);
            } else {
                hierAnimate(an);
            }
            hierPush(an->child[an->cur], e->matIdx, e->pos, e->eoCnt);
            break;
        case 11:
            hierCtrlNode((HierEnt*)e->node, e);
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierTraverse);
#endif

#ifdef NON_MATCHING
s32 hierIsInFov(VECTOR* ofs, s32 dist, VECTOR* pos, MATRIX* fov0, MATRIX* fov1)
{
    VECTOR out0;
    VECTOR out1;
    VECTOR v;
    s32 lim;

    v.vx = pos->vx + ofs->vx;
    v.vy = pos->vy + ofs->vy;
    v.vz = pos->vz + ofs->vz;
    mathMulTransVec(fov0, &v, &out0);
    lim = -dist;
    if (out0.vx < lim || out0.vy < lim || out0.vz < lim) {
        return 0;
    }
    mathMulTransVec(fov1, &v, &out1);
    if (out1.vx < lim || out1.vy < lim) {
        return 0;
    }
    return out1.vz >= (-(fovDepth << 3) - dist);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierIsInFov);
#endif

#ifdef NON_MATCHING
void hierLod(HierLod* n, HierEnt* e)
{
    s32 d[3];
    s32 dist;
    s32 i;
    s32 go;

    d[0] = e->pos[0] + n->ofs[0];
    d[1] = e->pos[1] + n->ofs[1];
    d[2] = e->pos[2] + n->ofs[2];
    d[0] = d[0] >> lodScale;
    d[1] = d[1] >> lodScale;
    d[2] = d[2] >> lodScale;
    dist = (d[0] * d[0]) + (d[1] * d[1]) + (d[2] * d[2]);
    go = 1;
    i = 0;
    if (lodShiftValue > 0) {
        dist = dist << lodShiftValue;
    } else {
        dist = dist >> -lodShiftValue;
    }
    while (i < n->nrec) {
        if (go == 0) {
            break;
        }
        if (dist >= n->rec[i]->near && dist < n->rec[i]->far) {
            hierPush(n->rec[i]->node, e->matIdx, e->pos, e->eoCnt);
            go = 0;
        }
        i++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierLod);
#endif

#ifdef NON_MATCHING
void hierCtrlNode(HierEnt* unused, HierEnt* e)
{
    HierCtrl* n;
    s32 i;
    s32 ok;

    n = (HierCtrl*)e->node;
    i = 0;
    ok = 1;
    if (!(n->flags & 2)) {
        while (1) {
            if (i >= n->nbox) {
                break;
            }
            if (hierInBox(n->box[i], (VECTOR*)e->pos) != 0) {
                ok = 0;
            }
            i++;
            if (ok == 0) {
                break;
            }
        }
        if (ok != 0) {
            if (n->onNode != 0) {
                hierPush(n->onNode, e->matIdx, e->pos, e->eoCnt);
            }
        } else {
            if (n->offNode != 0) {
                hierPush(n->offNode, e->matIdx, e->pos, e->eoCnt);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierCtrlNode);
#endif

#ifdef NON_MATCHING
s32 hierInBox(HdBox* b, VECTOR* pos)
{
    VECTOR p;
    VECTOR d;
    VECTOR v;
    s32 r;

    v.vx = -pos->vx;
    v.vy = -pos->vy;
    v.vz = -pos->vz;
    mathMulVec(&b->mat, &v, &p);
    r = 0;
    d.vz = b->lo.vz + p.vz;
    if (d.vz <= 0) {
        goto out;
    }
    d.vy = b->lo.vy + p.vy;
    if (d.vy <= 0) {
        goto out;
    }
    d.vx = b->lo.vx + p.vx;
    if (d.vx <= 0) {
        goto out;
    }
    if ((b->hi.vx - d.vx) <= 0) {
        goto out;
    }
    if ((b->hi.vy - d.vy) <= 0) {
        goto out;
    }
    r = b->hi.vz - d.vz;
    r = r > 0;
out:
    return r;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierInBox);
#endif

#ifdef NON_MATCHING
void hierAnimate(HierAnim* a)
{
    a->timer++;
    if (a->timer > a->rate) {
        a->cur++;
        if (a->cur > a->hi) {
            a->cur = a->lo;
        }
        a->timer = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierAnimate);
#endif

#ifdef NON_MATCHING
void hierAnimateBiDirect(HierAnim* a)
{
    a->timer++;
    if (a->timer > a->rate) {
        a->cur = a->cur + a->dir;
        if (a->cur == a->hi) {
            a->dir = 255;
        } else if (a->cur == a->lo) {
            a->dir = 1;
        }
        a->timer = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierAnimateBiDirect);
#endif

void hierSetWorldEP(s32 ep)
{
    worldEP = ep;
}

#ifdef NON_MATCHING
void hierProc(Db* db, s32 which)
{
    Cs* cs;

    objCnt = 0;
    matIdx = 0;
    rotCnt = 0;
    transCnt = 0;
    groupCnt = 0;
    lodCnt = 0;
    switchCnt = 0;
    polyCnt = 0;
    eoCnt = 0;
    fovDepth = (viewGetCurrentFovSet(which) == 13) ? 100 : fovDepthCfg;
    cs = csGetWorldCs();
    if (cs->epNode != 0) {
        wordCopy((s32*)&lightList[matIdx], (s32*)&cs->env->light, 8);
        hierTraverse(cs, cs->epNode);
    }
    cs = viewGetSky(which);
    if (cs != 0) {
        if (matIdx < 70) {
            matIdx = matIdx + 1;
        }
        csUpdMat(cs, &lightList[matIdx]);
        eoCnt = eoCnt + 1;
        hierTraverse(cs, cs->epNode);
    }
    fovDepth = (viewGetCurrentFovSet(which) == 13) ? 1000 : fovDepthCfg;
    cs = csGetCsList();
    if (cs != 0) {
        do {
            if (cs->drawMode != 0) {
                if (matIdx < 70) {
                    matIdx = matIdx + 1;
                }
                csUpdMat(cs, &lightList[matIdx]);
                eoCnt = eoCnt + 1;
                hierTraverse(cs, cs->epNode);
            }
            cs = cs->next;
        } while (cs != 0);
    }
    SetDrawMode(SPAD_DRMODE0, db->draw.dfe, db->draw.dtd, db->draw.tpage, &db->draw.tw);
    SetDrawMode(SPAD_DRMODE1, db->draw.dfe, 1, db->draw.tpage, &db->draw.tw);
    sdSubPolyPtr = subPolyList;
    if (viewGetCurrentFovSet(which) == 13) {
        sdFogEnable = 1;
    } else {
        sdFogEnable = 0;
    }
    geomProc(db, objList, objCnt, matList, lightList);
    subPoly3(db, subPolyList, sdSubPolyCount);
    subPoly4(db, subPolyList, sdSubPolyCount);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierProc);
#endif

#ifdef NON_MATCHING
void hierProcOne(Db* db, Cs* cs)
{
    objCnt = 0;
    matIdx = 0;
    eoCnt = 0;
    fovDepth = fovDepthCfg;
    csUpdMat(cs, lightList);
    hierTraverse(cs, cs->epNode);
    matIdx = matIdx + 1;
    SetDrawMode(SPAD_DRMODE0, db->draw.dfe, db->draw.dtd, db->draw.tpage, &db->draw.tw);
    SetDrawMode(SPAD_DRMODE1, db->draw.dfe, 1, db->draw.tpage, &db->draw.tw);
    sdSubPolyPtr = subPolyList;
    sdFogEnable = 0;
    geomProc(db, objList, objCnt, matList, lightList);
    subPoly3(db, subPolyList, sdSubPolyCount);
    subPoly4(db, subPolyList, sdSubPolyCount);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier", hierProcOne);
#endif

void hierInit(void)
{
    hdInit((HdEnt*)hierStack);
    hdpInit((HdpEnt*)hierStack);
}

void wordCopy(s32* dst, s32* src, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++) {
        *dst++ = *src++;
    }
}

void hierSetLodScale(s32 v)
{
    lodScale = v;
}

void hierSetLodShift(s32 v)
{
    lodShiftValue = v;
}

void hierSetGroundFlag(s32 v)
{
    groundFlag = v;
}
