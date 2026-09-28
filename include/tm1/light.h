#ifndef __TM1_LIGHT_H__
#define __TM1_LIGHT_H__

#include "common.h"
#include <libgte.h>

typedef struct LightEnv {
    s32 ambient[3]; /* 0x00 */
    MATRIX color; /* 0x0C */
    MATRIX light; /* 0x2C */
} LightEnv; /* 0x4C */

void lightSetLight(s32 e, s32 j, s32 rx, s32 ry, s16 r, s16 g, s16 b);
void lightSetRot(s32 e, s32 j, s16 rx, s16 ry);
void lightSetColor(s32 e, s32 j, s16 r, s16 g, s16 b);
void lightSetAmbient(s32 e, s16 r, s16 g, s16 b);
void lightInit(void);
void lightSetRedFlashEnv(void);
void lightSetFreezeEnv(void);
void lightSetLightningEnv(void);
void lightSetWorldLights(s32 e);
s32* lightGetAmbient(void);
s32* lightGetRot(void);
s32* lightGetColor(void);
LightEnv* lightGetEnv(s32 e);

#endif // __TM1_LIGHT_H__
