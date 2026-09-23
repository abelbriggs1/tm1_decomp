#ifndef __TM1_CAR_UPDATE_H__
#define __TM1_CAR_UPDATE_H__

#include "common.h"

typedef struct {
    s16 w; /* 0x00 */
    s16 h; /* 0x02 */
    s16 t; /* 0x04 */
    s16 pad; /* 0x06 */
    s32 x; /* 0x08 */
    s32 z; /* 0x0C */
} Curb; /* 0x10 */

extern Curb curbs[];
extern s16 numCurbs;

typedef struct {
    u8 active; /* +0  */
    u8 pad1;
    s16 a; /* +2  */
    s16 b; /* +4  */
    s16 c; /* +6  */
    s32 x; /* +8  */
    s32 z; /* +12 */
} PotHole; /* 16  */

extern PotHole potHoles[];
extern s16 numPotHoles;
extern s16 numCheckPotHoles;

typedef struct SlickSpot {
    s16 w; /* +0 */
    s16 h; /* +2 */
    s32 x; /* +4 */
    s32 z; /* +8 */
} SlickSpot;

extern SlickSpot slickSpots[];
extern s16 numSlickSpots;

#endif // __TM1_CAR_UPDATE_H__
