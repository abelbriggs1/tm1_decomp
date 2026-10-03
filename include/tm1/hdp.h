#ifndef __TM1_HDP_H__
#define __TM1_HDP_H__

#include "common.h"
#include <libgte.h>

#include "tm1/hd.h"
#include "tm1/long_vector.h"

s32 hdpPntBox(HdBox* b, VECTOR* pos, HdCsHit* res, s32 mode);
s32 hdpPntVolume(HdVol* n, VECTOR* pos, HdCsHit* res, s32 mode);
HdCsHit* HdPntTest(s32 skip0, s32 skip1, VEC3* pt, s32* hitId);

#endif /* __TM1_HDP_H__ */
