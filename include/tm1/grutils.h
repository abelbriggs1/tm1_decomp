#ifndef __TM1_GRUTILS_H__
#define __TM1_GRUTILS_H__

#include "common.h"
#include <libgpu.h>

// TODO: Consolidate; these are probably SDK types

/* One vertex of a DMD object's vertex array (8 bytes). */
typedef struct {
    s16 x; /* +0 */
    s16 y; /* +2 */
    s16 z; /* +4 */
    s16 pad; /* +6 */
} GrVert;

/* The UV/CLUT/TPAGE block a polygon record carries at +16 (POLY_FT4 layout). */
typedef struct {
    u8 u0; /* +0 */
    u8 v0; /* +1 */
    u16 clut; /* +2 */
    u8 u1; /* +4 */
    u8 v1; /* +5 */
    u16 tpage; /* +6 */
    u8 u2; /* +8 */
    u8 v2; /* +9 */
    u16 pad2; /* +10 */
    u8 u3; /* +12 */
    u8 v3; /* +13 */
    u16 pad3; /* +14 */
} GrTexUV;

/* A parsed DMD-style object: a polygon list plus its vertex list. */
typedef struct GrObj {
    u8 kind; /* 0 = sprite object */
    u8 pad0[3];
    GrVert* verts; /* +4  vertex array */
    u8 pad1[4];
    u8* polys; /* +12 first polygon record */
    s32 count; /* +16 number of polygon records */
} GrObj;

/* Parsed DMD3D sprite. */
typedef struct GrSprite {
    u8 u0, v0, w, h;
    u16 tpage, clut;
} GrSprite;

/* 2D screen sprite record built by grutilsParse2DScreenSprite (16 bytes). */
typedef struct {
    s16 u0; /* +0 */
    s16 v0; /* +2 */
    s16 w; /* +4 */
    s16 h; /* +6 */
    s16 tpage; /* +8 */
    s16 clut; /* +10 */
    s16 x; /* +12 */
    s16 y; /* +14 */
} GrScreenSprite;

u16 Load4BitClut(u32* data, int x, int y);
u32 grutilsParse2DScreenSprite(GrObj* obj, GrScreenSprite* out, u32 max);
u32 grutilsParse3DSprite(GrObj* obj, GrSprite* out, u32 max);
s32 grutilsParse2DSprite(GrObj* obj, GrSprite* out, u32 max);
void grutilsDrawNumberUsingSprites(s32 num, SPRT* prim, GrSprite* digits, u32* ot);

#endif
