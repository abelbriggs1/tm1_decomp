#ifndef __TM1_VIEW_H__
#define __TM1_VIEW_H__

#include "common.h"
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/fov_data.h"

typedef Cs ViewNode;

// TODO: Use the proper PSYQ types if they fit here.
typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} ViewRect; /* 8 */

typedef struct {
    ViewRect clip; /* +0  */
    s16 ofs[2]; /* +8  */
    ViewRect tw; /* +12 */
    u16 tpage; /* +20 */
    u8 dtd; /* +22 */
    u8 dfe; /* +23 */
    u8 isbg; /* +24 */
    u8 r0; /* +25 */
    u8 g0; /* +26 */
    u8 b0; /* +27 */
    u8 dr_env[64]; /* +28 */
} ViewDrawEnv; /* 92 */

typedef struct {
    ViewRect disp; /* +0  */
    ViewRect screen; /* +8  */
    u8 isinter; /* +16 */
    u8 isrgb24; /* +17 */
    u8 pad0; /* +18 */
    u8 pad1; /* +19 */
} ViewDispEnv; /* 20 */

typedef struct ViewDb {
    u8 pad0[16]; /* +0   */
    ViewDrawEnv draw0; /* +16  */
    ViewDispEnv disp0; /* +108 */
    u8 pad128[16]; /* +128 */
    ViewDrawEnv draw1; /* +144 */
    ViewDispEnv disp1; /* +236 */
} ViewDb; /* 256 */

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} EyeTrans; /* 12 */

typedef struct {
    s16 m[9];
    s16 pad;
    s32 t[3];
} EyeMat; /* 32 */

s32 viewGetCurrentFOVH(s32 which);
s32 viewGetCurrentHorzFOVH(s32 which);
s32 viewGetCurrentVertFOVH(s32 which);
s32 viewGetCurrentFOVDegrees(s32 which);
s32 viewGetCurrentFOVDegreesHorz(s32 which);
s32 viewGetCurrentFOVDegreesVert(s32 which);
void viewGetScreenSize(s32* w, s32* h);
void viewCreate(ViewDb* db, s32 entry, s32 which);
void viewToggleView();
void viewInit();
void viewSetFovDat(s32 entry, s32 which);
s32 viewGetFov();
void viewSetFov(s32 fov);
void viewUpdateFov();
void viewTweekInit();
void viewProc(s32 which);
s32 viewGetCurrentFovSet(s32 which);
void viewSetParent(void* node, s32 which);
void viewSetSky(ViewNode* node);
ViewNode* viewGetSky(s32 which);
void viewSetSkyPosition(s32* pos, s32 which);
void viewGetDeltaRot(void* dst, s32 which);
void viewGetDeltaTrans(void* dst, s32 which);
void viewSetDeltaRot(void* src, s32 which);
void viewSetDeltaTrans(void* src, s32 which);
void* viewGetOwnship(s32 which);
void viewSetBgColor();
void viewGetCenter(s32 which, s32* cx, s32* cy);
EyeTrans* viewGetEyeTrans(s32 which);
EyeMat* viewGetEyeMat(s32 which);
void wordCp(s32* dst, s32* src, s32 n);

#endif
