#ifndef __TM1_HD_H__
#define __TM1_HD_H__

#include "common.h"
#include <libgte.h>

#include "tm1/cs.h"

typedef struct HdBox {
    /* 0x00 */ MATRIX mat;
    /* 0x20 */ SVECTOR lo;
    /* 0x28 */ SVECTOR hi;
} HdBox; /* 0x30 */

typedef struct HdLeaf {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ u8 pad04[0x10];
    /* 0x14 */ u16 tag2;
    /* 0x16 */ u16 skip;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 c[3];
    /* 0x28 */ s32 rad;
    /* 0x2C */ HdBox box;
} HdLeaf;

typedef struct HdBoxNode {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ s32 code;
    /* 0x08 */ HdBox box;
} HdBoxNode;

typedef struct HdVol {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ u8 nplane;
    /* 0x05 */ s8 code;
    /* 0x06 */ u16 tag2;
    /* 0x08 */ MATRIX mat0;
    /* 0x28 */ MATRIX mat1;
    /* 0x48 */ SVECTOR d0;
    /* 0x50 */ SVECTOR d1;
} HdVol;

typedef struct HdGroup {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ s32 c[3];
    /* 0x10 */ s32 rad;
    /* 0x14 */ u8 cnt;
    /* 0x15 */ u8 flags;
    /* 0x16 */ u16 tag2;
    /* 0x18 */ void* child[1];
} HdGroup;

typedef struct HdTrans {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ s32 ofs[3];
    /* 0x10 */ u16 tag2;
    /* 0x12 */ u8 cnt;
    /* 0x13 */ u8 flags;
    /* 0x14 */ void* child[1];
} HdTrans;

typedef struct HdRot {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ MATRIX mat;
    /* 0x24 */ u16 tag2;
    /* 0x26 */ u8 cnt;
    /* 0x27 */ u8 flags;
    /* 0x28 */ void* child[1];
} HdRot;

typedef struct HdSwitch9 {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ u16 tag2;
    /* 0x06 */ u8 pad06;
    /* 0x07 */ u8 sel;
    /* 0x08 */ void* child[1];
} HdSwitch9;

typedef struct HdSwitch3 {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ u16 tag2;
    /* 0x06 */ u8 pad06[6];
    /* 0x0C */ u8 sel;
    /* 0x0D */ u8 pad0D[3];
    /* 0x10 */ void* child[1];
} HdSwitch3;

typedef struct HdCtrl {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 tag;
    /* 0x04 */ s32 flags;
    /* 0x08 */ u16 tag2;
    /* 0x0A */ u8 cnt;
    /* 0x0B */ u8 pad0B;
    /* 0x0C */ void* inNode;
    /* 0x10 */ void* outNode;
    /* 0x14 */ HdBox* box[1];
} HdCtrl;

typedef struct HdLodRec {
    /* 0x00 */ u32 far;
    /* 0x04 */ u32 near;
    /* 0x08 */ void* child;
} HdLodRec;

typedef struct HdLodNode {
    /* 0x00 */ s32 pad00;
    /* 0x04 */ s32 c[3];
    /* 0x10 */ s32 cnt;
    /* 0x14 */ HdLodRec* rec[1];
} HdLodNode;

typedef struct HdCsInfo {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01[15];
    /* 0x10 */ s32 radius;
} HdCsInfo;

typedef struct HdEnt {
    /* 0x00 */ void* node;
    /* 0x04 */ s32 v[3];
    /* 0x10 */ s32 matIdx;
    /* 0x14 */ u16 id0;
    /* 0x16 */ u16 id1;
} HdEnt; /* 0x18 */

typedef struct HdCsHit {
    /* 0x00 */ s32 hit;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s16 histIdx;
    /* 0x0A */ s16 tag0;
    /* 0x0C */ s16 tag1;
    /* 0x0E */ s16 unk0E;
    /* 0x10 */ s32 depth;
    /* 0x14 */ s32 normal[3];
    /* 0x20 */ Cs* obj;
} HdCsHit; /* 0x24 */

void hdPush(void* node, s32* v, s32 matIdx, s32 id0, s16 id1);
s32 hdPop(HdEnt** out);
void hdSetNodeIds(s16* out0, s16* out1, HdEnt* e, s16 id0, s16 id1);
s32 hdIsPtInSphere(VECTOR* pt, s32 radius, VECTOR* centre, s32 slack);
void hdTraverse(Cs* cs, s32* pt, HdCsHit* res, s32 slack);
void hdLod(HdLodNode* node, HdEnt* e);
s32 hdPntBox(HdBox* b, VECTOR* pos, HdCsHit* res, s32 matIdx);
s32 hdPntVolume(HdVol* n, VECTOR* pos, HdCsHit* res, s32 matIdx);
void hdCtrlNode(HdEnt* e, s16 id0, s16 id1);
s32 hdInCtrlBox(HdBox* box, s32* pt);
void hdSetHitTag(HdCsHit* hit, s16 id0, s16 id1, HdEnt* e);
void HdCsTest(Cs* cs, HdCsHit* hit, s32 doWorld, MATRIX* otherMat, s32* otherPos);
void hdInit(HdEnt* stack);

#endif /* __TM1_HD_H__ */
