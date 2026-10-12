#ifndef __TM1_BRIDGES_H__
#define __TM1_BRIDGES_H__

#include "common.h"

typedef struct {
    s16 obj[4]; /* 0x00  object/segment ids; the Init loop presets all four to -1 */
    s16 unk08; /* 0x08 */
    s16 unk0A; /* 0x0A */
    s16 y0; /* 0x0C ramp height at the LOW end */
    s16 y1; /* 0x0E ramp height at the HIGH end */
    s16 extentX; /* 0x10 */
    s16 extentZ; /* 0x12 */
    s32 minX; /* 0x14 MIN corner X */
    s32 minZ; /* MIN corner Z  0x18 */
    s32 rampDir; /* 0x1C ramp axis/direction code 0..3 */
} Bridge; /* 0x20 */

extern Bridge bridges[];
extern s16 numBridges;
extern s16 numCheckBridges;

Bridge* GetBridgeDat(s16 which);
void InitLevel1Bridges(void);
void InitLevel2Bridges(void);
void InitLevel3Bridges(void);
void InitLevel4Bridges(void);
void InitLevel5Bridges(void);
void InitLevel6Bridges(void);

#endif // __TM1_BRIDGES_H__
