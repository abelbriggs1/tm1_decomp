#ifndef __TM1_HDP_H__
#define __TM1_HDP_H__

#include "common.h"
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/hd.h"
#include "tm1/math.h"

typedef struct HdpEnt {
    /* 0x00 */ void* node;
    /* 0x04 */ s32 pos[3];
    /* 0x10 */ s16 id0;
    /* 0x12 */ s16 id1;
} HdpEnt; /* 0x14 */

void hdpPush(void* node, s32* pos, s32 id0, s32 id1);
s32 hdpPop(HdpEnt** out);
void hdpSetNodeIds(s16* out0, s16* out1, HdpEnt* e, s16 id0, s16 id1);
s32 hdpIsPtInSphere(s32* pt, s32 rad, s32* centre);
void hdpTraverse(Cs* cs, s32* pt, HdCsHit* res);
void hdpLod(HdLodNode* node, HdpEnt* e);
s32 hdpPntBox(HdBox* b, VECTOR* pos, s32* hit, s32 mode);
s32 hdpPntVolume(HdVol* n, VECTOR* pos, HdCsHit* res, s32 mode);
void hdpCtrlNode(HdpEnt* e, s16 id0, s16 id1);
s32 hdpInCtrlBox(HdBox* b, VECTOR* pos);
void hdpSetHitTag(HdCsHit* hit, s16 id0, s16 id1, HdpEnt* e);
HdCsHit* HdPntTest(s32 skip0, s32 skip1, VECTOR3* pt, s32* hitId);
void hdpInit(HdpEnt* stack);

#endif /* __TM1_HDP_H__ */
