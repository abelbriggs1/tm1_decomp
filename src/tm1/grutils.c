#include "common.h"

#include "tm1/grutils.h"

u16 Load4BitClut(u32* data, int x, int y)
{
    RECT rect;

    rect.w = 16;
    rect.x = x;
    rect.y = y;
    rect.h = 1;
    LoadImage(&rect, (u_long*)data);
    DrawSync(0);
    return GetClut(x, y);
}

#ifdef NON_MATCHING
u32 grutilsParse2DScreenSprite(GrObj* obj, GrScreenSprite* out, u32 max)
{
    GrVert* verts;
    u8* poly;
    u8* base;
    GrTexUV* tex;
    int i;

    if (obj == 0 || obj->kind != 0) {
        return 0;
    }
    verts = obj->verts;
    poly = obj->polys;
    if (max >= (u32)obj->count) {
        max = obj->count;
    }
    for (i = 0; i < (int)max; i++) {
        base = poly + 16;
        tex = (GrTexUV*)(base + ((poly[15] >> 2) & 0x1c));
        out[i].x = verts[(s16) * (u16*)(poly + 4)].x / 8;
        out[i].y = -verts[(s16) * (u16*)(poly + 4)].y / 8;
        out[i].u0 = tex->u0;
        out[i].v0 = tex->v0;
        out[i].w = (verts[(s16) * (u16*)(poly + 6)].x - verts[(s16) * (u16*)(poly + 4)].x) / 8;
        out[i].h = (verts[(s16) * (u16*)(poly + 4)].y - verts[(s16) * (u16*)(poly + 8)].y) / 8;
        out[i].clut = tex->clut;
        out[i].tpage = tex->tpage;
        poly += 4 * poly[2];
    }
    return max;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/grutils", grutilsParse2DScreenSprite);
#endif

#ifdef NON_MATCHING
u32 grutilsParse3DSprite(GrObj* obj, GrSprite* out, u32 max)
{
    u8* poly;
    u8* base;
    GrTexUV* tex;
    int i;

    if (obj == 0 || obj->kind != 0) {
        return 0;
    }
    poly = obj->polys;
    if (max >= (u32)obj->count) {
        max = obj->count;
    }
    for (i = 0; i < (int)max; i++) {
        base = poly + 16;
        tex = (GrTexUV*)(base + ((poly[15] >> 2) & 0x1c));
        out[i].u0 = tex->u0;
        out[i].v0 = tex->v0;
        out[i].w = tex->u3 - tex->u2;
        out[i].h = tex->v3 - tex->v1;
        out[i].clut = tex->clut;
        out[i].tpage = tex->tpage;
        poly += 4 * poly[2];
    }
    return max;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/grutils", grutilsParse3DSprite);
#endif

#ifdef NON_MATCHING
s32 grutilsParse2DSprite(GrObj* obj, GrSprite* out, u32 max)
{
    s32 n;
    int i;

    n = grutilsParse3DSprite(obj, out, max);
    for (i = 0; i < n; i++) {
        out->w += 1;
        out->h += 1;
        out++;
    }
    return n;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/grutils", grutilsParse2DSprite);
#endif

#ifdef NON_MATCHING
void grutilsDrawNumberUsingSprites(s32 num, SPRT* prim, GrSprite* digits, u32* ot)
{
    s32 d;

    if (num < 10) {
        prim->u0 = digits[num].u0;
        prim->v0 = digits[num].v0;
        prim->w = digits[num].w;
        prim->h = digits[num].h;
        AddPrim(ot, prim);
    } else {
        if (num >= 100) {
            num = num % 100;
        }
        d = num / 10;
        prim->u0 = digits[d].u0;
        prim->v0 = digits[d].v0;
        prim->w = digits[d].w;
        prim->h = digits[d].h;
        d = num - d * 10;
        AddPrim(ot, prim);
        prim[1].x0 = prim->x0 + prim->w + 1;
        prim[1].u0 = digits[d].u0;
        prim[1].v0 = digits[d].v0;
        prim[1].w = digits[d].w;
        prim[1].h = digits[d].h;
        AddPrim(ot, &prim[1]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/grutils", grutilsDrawNumberUsingSprites);
#endif
