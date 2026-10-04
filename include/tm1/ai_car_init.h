#ifndef __TM1_AI_CAR_INIT_H__
#define __TM1_AI_CAR_INIT_H__

#include "common.h"

#include "tm1/car.h"

typedef struct AITransDat {
    /*0x00*/ u8 unk00[16];
    /*0x10*/ s16 unk10[11];
} AITransDat; /* 0x26 */

void AICarInit(AICar* car, s32 uaIndex, u8 which);
void AICarInitDynamics(AICar* car);
void AICarInitProfiles(AICar* car);
void InitAIFlags(u8* flags);
void InitAITransDat(AITransDat* transDat);

#endif /* __TM1_AI_CAR_INIT_H__ */
