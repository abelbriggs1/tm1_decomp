#ifndef TM1_FOV_DATA_H
#define TM1_FOV_DATA_H
#include "common.h"
#include <libgte.h>

// TODO: Determine where this should live.
typedef struct {
    s32 fovDegrees; /* +0  */
    s32 fovDegreesVert; /* +4  */
    s32 fovh; /* +8  */
    s32 vertFovh; /* +12 */
    s32 screenWidth; /* +16 */
    s32 screenHeight; /* +20 */
    s32 centerWidth; /* +24 */
    s32 centerHeight; /* +28 */
    s32 drawX; /* +32 */
    s32 drawY; /* +36 */
    s16 rotXX; /* +40 */
    s16 rotZY; /* +42 */
    s16 rotYZ; /* +44 */
    s16 pad46; /* +46 */
    SVECTOR rotA[3]; /* +48 */
    SVECTOR rotB[3]; /* +72 */
} FovDat; /* 96  */

typedef char TM1FovRecordSize[(sizeof(FovDat) == 96) ? 1 : -1];
#endif
