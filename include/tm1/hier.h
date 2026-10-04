#ifndef __TM1_HIER_H__
#define __TM1_HIER_H__

#include "common.h"
#include <libgpu.h>
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/hd.h"
#include "tm1/hier_sub.h"
#include "tm1/rt.h"

typedef struct HierEnt {
    /* 0x00 */ void* node;
    /* 0x04 */ s32 matIdx;
    /* 0x08 */ s32 pos[3];
    /* 0x14 */ s32 eoCnt;
} HierEnt; /* 0x18 */

/* A control node: pushes offNode if the eyepoint is inside any box, else onNode. */
typedef struct HierCtrl {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u8 pad08[2];
    /* 0x0A */ u8 nbox;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ void* onNode;
    /* 0x10 */ void* offNode;
    /* 0x14 */ HdBox* box[1];
} HierCtrl;

typedef struct HierLodRec {
    /* 0x00 */ u32 far;
    /* 0x04 */ u32 near;
    /* 0x08 */ void* node;
} HierLodRec;

typedef struct HierLod {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 ofs[3];
    /* 0x10 */ s32 nrec;
    /* 0x14 */ HierLodRec* rec[1];
} HierLod;

typedef struct HierAnim {
    /* 0x00 */ u8 pad00[6];
    /* 0x06 */ u16 rate;
    /* 0x08 */ u8 lo;
    /* 0x09 */ u8 hi;
    /* 0x0A */ u8 mode;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ u8 cur;
    /* 0x0D */ u8 dir;
    /* 0x0E */ u16 timer;
    /* 0x10 */ void* child[1];
} HierAnim;

typedef struct HierObj {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[0x17];
    /* 0x18 */ u32 flags;
    /* 0x1C */ s32 ofs[3];
    /* 0x28 */ s32 dist;
} HierObj;

typedef struct HierGroup {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ s32 ofs[3];
    /* 0x10 */ s32 dist;
    /* 0x14 */ u8 count;
    /* 0x15 */ u8 flags;
    /* 0x16 */ u8 pad16[2];
    /* 0x18 */ void* child[1];
} HierGroup;

typedef struct HierEo {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ s32 ofs[3];
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ u8 count;
    /* 0x13 */ u8 flags;
    /* 0x14 */ void* child[1];
} HierEo;

typedef struct HierRot {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[3];
    /* 0x04 */ MATRIX mat;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ u8 count;
    /* 0x27 */ u8 flags;
    /* 0x28 */ void* child[1];
} HierRot;

typedef struct HierSwitch {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 pad01[6];
    /* 0x07 */ u8 sel;
    /* 0x08 */ void* child[1];
} HierSwitch;

typedef struct MatInfo {
    /* 0x00 */ MATRIX* mat;
    /* 0x04 */ MATRIX* light;
    /* 0x08 */ MATRIX* fov0;
    /* 0x0C */ MATRIX* fov1;
} MatInfo; /* 0x10 */

typedef struct ObjRec {
    /* 0x00 */ void* node;
    /* 0x04 */ s32 pos[3];
    /* 0x10 */ s32 unk10;
    /* 0x14 */ Cs* cs;
    /* 0x18 */ s32 matIdx;
    /* 0x1C */ s32 eoCnt;
} ObjRec; /* 0x20 */

extern s32 worldEP;
extern HierEnt hierStack[150];
extern MatInfo matInfo[90];
extern SubPoly subPolyList[];
extern ObjRec objList[220];
extern MATRIX matList[];
extern MATRIX lightList[];
extern MATRIX fovNorms1[];
extern MATRIX fovNorms2[];

void hierPush(void* node, s32 mat, s32* pos, s32 eo);
s32 hierPop(HierEnt** out);
void hierTraverse(Cs* cs, void* node);
s32 hierIsInFov(VECTOR* ofs, s32 dist, VECTOR* pos, MATRIX* fov0, MATRIX* fov1);
void hierLod(HierLod* n, HierEnt* e);
void hierCtrlNode(HierEnt* unused, HierEnt* e);
s32 hierInBox(HdBox* b, VECTOR* pos);
void hierAnimate(HierAnim* a);
void hierAnimateBiDirect(HierAnim* a);
void hierSetWorldEP(s32 ep);
void hierProc(Db* db, s32 which);
void hierProcOne(Db* db, Cs* cs);
void hierInit(void);
void wordCopy(s32* dst, s32* src, s32 n);
void hierSetLodScale(s32 v);
void hierSetLodShift(s32 v);
void hierSetGroundFlag(s32 v);

#endif /* __TM1_HIER_H__ */
