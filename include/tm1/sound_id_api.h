#ifndef TM1_SOUND_ID_API_H
#define TM1_SOUND_ID_API_H

#include "common.h"

void soundPlayId(u32 id, u32 dist, s32 range, s32 channel, s32 a4, s32 a5);
void soundStopPlayId(u32 id, s32 channel, s32 a2, s32 unused);

#endif
