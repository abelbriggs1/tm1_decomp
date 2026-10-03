#ifndef __TM1_TRIGGER_PTS_H__
#define __TM1_TRIGGER_PTS_H__

#include "common.h"

typedef struct TriggerPt {
    /*0x00*/ s32 x;
    /*0x04*/ s32 z;
    /*0x08*/ s16 f08;
    /*0x0A*/ s16 link[4];
    /*0x12*/ s16 dist[4];
    /*0x1A*/ s8 f1A;
    /*0x1B*/ s8 type;
} TriggerPt;

typedef struct GroupPos {
    /*0x00*/ s32 x;
    /*0x04*/ s32 z;
    /*0x08*/ s32 y;
} GroupPos;

typedef struct TriggerPtGroups {
    /*0x000*/ s16 count;
    /*0x002*/ s16 a[14][14];
    /*0x18A*/ s16 b[14][14];
    /*0x312*/ u16 minSpeed[14][14];
    /*0x49C*/ GroupPos pos[14];
    /*0x544*/ s32 w[14];
    /*0x57C*/ s32 d[14];
} TriggerPtGroups;

typedef struct TriggerPtStartPts {
    /*0x00*/ s8 n;
    /*0x01*/ s8 pad;
    /*0x02*/ u8 grp[16];
    /*0x12*/ s16 start[15];
} TriggerPtStartPts;

extern s16 numTriggerPoints;
extern TriggerPt tPoints[175];
extern TriggerPtGroups triggerPtGroups;
extern TriggerPtStartPts triggerPtStartPts;

void InitLevel1TriggerPoints(void);
void InitLevel2TriggerPoints(void);
void InitLevel3TriggerPoints(void);
void InitLevel4TriggerPoints(void);
void InitLevel5TriggerPoints(void);
void Init2ndPartOfLevel5TriggerPts(void);
void Init3rdPartOfLevel5TriggerPts(void);
void Init4thPartOfLevel5TriggerPts(void);
void InitLevel6TriggerPoints(void);
void Init2ndPartOfLevel6TriggerPts(void);

#endif // __TM1_TRIGGER_PTS_H__
