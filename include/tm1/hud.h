#ifndef __TM1_HUD_H__
#define __TM1_HUD_H__

#include "common.h"

#include "tm1/grutils.h"

void hudResetRadar(void);
void hudDisplayRadar(u32* ot);
void hudInitRadar(void);
void hudAddRadarSig(s32 id, s32* pos, s32 ang);
void hudradarActivate(void);
void hudRadarToggle(void);
void hudStoreArrowIcon(s32 id, GrObj* obj);

#endif /* __TM1_HUD_H__ */
