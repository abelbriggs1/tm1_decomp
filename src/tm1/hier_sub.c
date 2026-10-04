#include "common.h"
#include <libetc.h>
#include <libgpu.h>

#include "tm1/hier_sub.h"

#define POUT16 ((s16*)pOut)
#define POUT8 ((u8*)pOut)

extern s32* prim2D[3];
extern s32 pOut[5];

s32* p0;
s32* p1;
s32* p2;
s32* p3;

#ifdef NON_MATCHING
void subPoly3(Db* db, SubPoly* f, s32 count)
{
    u8* out;
    u8* prim;
    u8* q0;
    u8* q1;
    u8* q2;
    s32* flags;
    s32 i;
    s32 step;
    s32 half;
    s32 half2;
    s32 nWords;
    s32 sub;

    out = (u8*)db->unk8;
    for (i = 0; i < count; i++) {
        flags = f[i].flags;
        if ((flags[3] & 0x70000) == 0x30000) {
            half = ((u8*)flags)[14] >> 6;
            prim = (u8*)f[i].prim;
            flags = f[i].flags;
            step = half * 4;
            q0 = prim + 8;
            q1 = q0 + step;
            q2 = q1 + step;
            p0 = (s32*)q0;
            p1 = (s32*)q1;
            p2 = (s32*)q2;
            nWords = ((u8*)flags)[3] + 1;
            prim2D[0] = (s32*)out;
            prim2D[1] = (s32*)(out + nWords * 4);
            prim2D[2] = (s32*)((u8*)prim2D[1] + nWords * 4);
            if (db->areaEnd < (u8*)prim2D[2] + 160) {
                break;
            }
            half2 = half;
            POUT16[0] = (*(s16*)(prim + 8) + *(s16*)(q1 + 0)) >> 1;
            POUT16[1] = (*(s16*)(prim + 10) + *(s16*)(q1 + 2)) >> 1;
            POUT16[2] = (*(s16*)(q1 + 0) + *(s16*)(q2 + 0)) >> 1;
            POUT16[3] = (*(s16*)(q1 + 2) + *(s16*)(q2 + 2)) >> 1;
            POUT16[4] = (*(s16*)(q2 + 0) + *(s16*)(prim + 8)) >> 1;
            POUT16[5] = (*(s16*)(q2 + 2) + *(s16*)(prim + 10)) >> 1;
            subLoadPoly3Words(2, half);
            if ((f[i].flags[0] & 0x7f) != 0) {
                u8* r0;
                u8* r1;
                u8* r2;

                r0 = (u8*)p0;
                r1 = (u8*)p1;
                r2 = (u8*)p2;
                p0 = (s32*)(r0 + 4);
                p1 = (s32*)(r1 + 4);
                p2 = (s32*)(r2 + 4);
                POUT8[0] = (r0[4] + r1[4]) >> 1;
                POUT8[1] = (r0[5] + r1[5]) >> 1;
                POUT8[4] = (r1[4] + r2[4]) >> 1;
                POUT8[5] = (r1[5] + r2[5]) >> 1;
                POUT8[8] = (r2[4] + r0[4]) >> 1;
                POUT8[9] = (r2[5] + r0[5]) >> 1;
                POUT16[3] = *(u16*)(r0 + 6);
                POUT16[5] = *(u16*)(r1 + 6);
                subLoadPoly3Words(3, half);
            }
            prim = (u8*)f[i].prim;
            flags = f[i].flags;
            q0 = prim + 4;
            p0 = (s32*)q0;
            if (((u8*)flags)[1] >= 2 || (sub = (((u8*)flags)[14] >> 3) & 7) >= 2) {
                q1 = q0 + step;
                q2 = q1 + step;
                p1 = (s32*)q1;
                p2 = (s32*)q2;
                POUT8[0] = (prim[4] + q1[0]) >> 1;
                POUT8[1] = (prim[5] + q1[1]) >> 1;
                POUT8[2] = (prim[6] + q1[2]) >> 1;
                POUT8[4] = (q1[0] + q2[0]) >> 1;
                POUT8[5] = (q1[1] + q2[1]) >> 1;
                POUT8[6] = (q1[2] + q2[2]) >> 1;
                POUT8[7] = q1[3];
                POUT8[8] = (q2[0] + prim[4]) >> 1;
                POUT8[9] = (q2[1] + prim[5]) >> 1;
                POUT8[10] = (q2[2] + prim[6]) >> 1;
                subLoadPoly3Words(1, half2);
            } else {
                prim2D[0][1] = *(s32*)(prim + 4);
                prim2D[1][1] = *(s32*)(prim + 4);
                prim2D[2][1] = *(s32*)(prim + 4);
            }
            prim = (u8*)f[i].prim;
            prim[3] = nWords - 1;
            ((u8*)prim2D[0])[3] = nWords - 1;
            ((u8*)prim2D[1])[3] = nWords - 1;
            ((u8*)prim2D[2])[3] = nWords - 1;
            out = (u8*)prim2D[2] + nWords * 4;
            subAddPrim(db, &f[i], (SubPacket**)&out, (void**)prim2D);
        }
    }
    db->unk8 = (s32)out;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier_sub", subPoly3);
#endif

#ifdef NON_MATCHING
void subLoadPoly3Words(s32 col, s32 row)
{
    s32* q;

    q = prim2D[0] + col;
    *q = pOut[1];
    q += row;
    *q = *p1;
    q += row;
    *q = pOut[0];
    q = prim2D[1] + col;
    *q = pOut[1];
    q += row;
    *q = pOut[2];
    q += row;
    *q = *p2;
    q = prim2D[2] + col;
    *q = pOut[1];
    q += row;
    *q = pOut[2];
    q += row;
    *q = pOut[0];
    *p1 = pOut[2];
    *p2 = pOut[0];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier_sub", subLoadPoly3Words);
#endif

#ifdef NON_MATCHING
void subPoly4(Db* db, SubPoly* f, s32 count)
{
    u8* out;
    u8* prim;
    u8* q0;
    u8* q1;
    u8* q2;
    u8* q3;
    s32* flags;
    s32 i;
    s32 step;
    s32 half;
    s32 half2;
    s32 nWords;
    s32 mx0;
    s32 my0;
    s32 mx2;
    s32 my2;
    s32 sub;

    out = (u8*)db->unk8;
    for (i = 0; i < count; i++) {
        flags = f[i].flags;
        if ((flags[3] & 0x70000) == 0x40000) {
            half = ((u8*)flags)[14] >> 6;
            prim = (u8*)f[i].prim;
            flags = f[i].flags;
            step = half * 4;
            q0 = prim + 8;
            q1 = q0 + step;
            q2 = q1 + step;
            q3 = q2 + step;
            p0 = (s32*)q0;
            p1 = (s32*)q1;
            p2 = (s32*)q2;
            p3 = (s32*)q3;
            nWords = ((u8*)flags)[3] + 1;
            prim2D[0] = (s32*)out;
            prim2D[1] = (s32*)(out + nWords * 4);
            prim2D[2] = (s32*)((u8*)prim2D[1] + nWords * 4);
            if (db->areaEnd < (u8*)prim2D[2] + 160) {
                break;
            }
            half2 = half;
            mx0 = (*(s16*)(prim + 8) + *(s16*)(q2 + 0)) >> 1;
            POUT16[0] = mx0;
            my0 = (*(s16*)(prim + 10) + *(s16*)(q2 + 2)) >> 1;
            POUT16[1] = my0;
            POUT16[2] = (*(s16*)(q1 + 0) + *(s16*)(prim + 8)) >> 1;
            POUT16[3] = (*(s16*)(q1 + 2) + *(s16*)(prim + 10)) >> 1;
            mx2 = (*(s16*)(q1 + 0) + *(s16*)(q3 + 0)) >> 1;
            POUT16[4] = mx2;
            my2 = (*(s16*)(q1 + 2) + *(s16*)(q3 + 2)) >> 1;
            POUT16[5] = my2;
            POUT16[6] = (*(s16*)(q3 + 0) + *(s16*)(q2 + 0)) >> 1;
            POUT16[8] = (mx0 + mx2) >> 1;
            POUT16[9] = (my0 + my2) >> 1;
            POUT16[7] = (*(s16*)(q3 + 2) + *(s16*)(q2 + 2)) >> 1;
            subLoadPoly4Words(2, half);
            if ((f[i].flags[0] & 0x7f) != 0) {
                u8* r0;
                u8* r1;
                u8* r2;
                u8* r3;
                s32 u0;
                s32 v0;
                s32 u2;
                s32 v2;

                r0 = (u8*)p0;
                r1 = (u8*)p1;
                r2 = (u8*)p2;
                r3 = (u8*)p3;
                p0 = (s32*)(r0 + 4);
                p1 = (s32*)(r1 + 4);
                p2 = (s32*)(r2 + 4);
                p3 = (s32*)(r3 + 4);
                u0 = (r0[4] + r2[4]) >> 1;
                POUT8[0] = u0;
                v0 = (r0[5] + r2[5]) >> 1;
                POUT8[1] = v0;
                POUT8[4] = (r1[4] + r0[4]) >> 1;
                POUT8[5] = (r1[5] + r0[5]) >> 1;
                u2 = (r1[4] + r3[4]) >> 1;
                POUT8[8] = u2;
                v2 = (r1[5] + r3[5]) >> 1;
                POUT8[9] = v2;
                POUT8[12] = (r3[4] + r2[4]) >> 1;
                POUT8[16] = (u0 + u2) >> 1;
                POUT8[13] = (r3[5] + r2[5]) >> 1;
                POUT8[17] = (v0 + v2) >> 1;
                POUT16[1] = *(u16*)(r0 + 6);
                POUT16[9] = *(u16*)(r1 + 6);
                POUT16[3] = *(u16*)(r1 + 6);
                POUT16[5] = *(u16*)(r1 + 6);
                subLoadPoly4Words(3, half);
                *(u16*)((u8*)prim2D[1] + 14) = *(u16*)((u8*)p0 + 2);
                p1 = (s32*)((u8*)prim2D[1] + 12);
                p1 = (s32*)((u8*)prim2D[2] + 12);
                *(u16*)((u8*)prim2D[2] + 14) = *(u16*)((u8*)p0 + 2);
            }
            prim = (u8*)f[i].prim;
            flags = f[i].flags;
            q0 = prim + 4;
            p0 = (s32*)q0;
            if (((u8*)flags)[1] >= 2 || (sub = (((u8*)flags)[14] >> 3) & 7) >= 2) {
                s32 b0;
                s32 c0;
                s32 b2;
                s32 c2;
                s32 d0;
                s32 d2;

                q1 = q0 + step;
                q2 = q1 + step;
                q3 = q2 + step;
                p1 = (s32*)q1;
                p2 = (s32*)q2;
                p3 = (s32*)q3;
                b0 = (prim[4] + q2[0]) >> 1;
                POUT8[0] = b0;
                c0 = (prim[5] + q2[1]) >> 1;
                POUT8[1] = c0;
                d0 = (prim[6] + q2[2]) >> 1;
                POUT8[2] = d0;
                POUT8[3] = prim[7];
                POUT8[4] = (q1[0] + prim[4]) >> 1;
                POUT8[5] = (q1[1] + prim[5]) >> 1;
                POUT8[6] = (q1[2] + prim[6]) >> 1;
                POUT8[7] = prim[7];
                b2 = (q1[0] + q3[0]) >> 1;
                POUT8[8] = b2;
                c2 = (q1[1] + q3[1]) >> 1;
                POUT8[9] = c2;
                d2 = (q1[2] + q3[2]) >> 1;
                POUT8[10] = d2;
                POUT8[12] = (q3[0] + q2[0]) >> 1;
                POUT8[13] = (q3[1] + q2[1]) >> 1;
                POUT8[14] = (q3[2] + q2[2]) >> 1;
                POUT8[16] = (b0 + b2) >> 1;
                POUT8[17] = (c0 + c2) >> 1;
                POUT8[18] = (d0 + d2) >> 1;
                POUT8[19] = prim[7];
                subLoadPoly4Words(1, half2);
            } else {
                prim2D[0][1] = *(s32*)(prim + 4);
                prim2D[1][1] = *(s32*)(prim + 4);
                prim2D[2][1] = *(s32*)(prim + 4);
            }
            prim = (u8*)f[i].prim;
            prim[3] = nWords - 1;
            ((u8*)prim2D[0])[3] = nWords - 1;
            ((u8*)prim2D[1])[3] = nWords - 1;
            ((u8*)prim2D[2])[3] = nWords - 1;
            out = (u8*)prim2D[2] + nWords * 4;
            subAddPrim(db, &f[i], (SubPacket**)&out, (void**)prim2D);
        }
    }
    db->unk8 = (s32)out;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier_sub", subPoly4);
#endif

#ifdef NON_MATCHING
void subLoadPoly4Words(s32 col, s32 row)
{
    s32* q;

    q = prim2D[0] + col;
    *q = pOut[0];
    q += row;
    *q = pOut[4];
    q += row;
    *q = *p2;
    q += row;
    *q = pOut[3];
    q = prim2D[1] + col;
    *q = pOut[1];
    q += row;
    *q = *p1;
    q += row;
    *q = pOut[4];
    q += row;
    *q = pOut[2];
    q = prim2D[2] + col;
    *q = pOut[4];
    q += row;
    *q = pOut[2];
    q += row;
    *q = pOut[3];
    q += row;
    *q = *p3;
    *p1 = pOut[1];
    *p2 = pOut[0];
    *p3 = pOut[4];
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier_sub", subLoadPoly4Words);
#endif

#ifdef NON_MATCHING
void subAddPrim(Db* db, SubPoly* f, SubPacket** cursor, void** extra)
{
    if ((*f->flags & 0x80) != 0 || f->twoSided != 0) {
        (*cursor)->w0 = *getScratchAddr(0x18);
        (*cursor)->w1 = *getScratchAddr(0x19);
        (*cursor)->w2 = *getScratchAddr(0x1A);
        AddPrim((u32*)db->small + f->otIdx, *cursor);
        *cursor += 1;
        AddPrim((u32*)db->small + f->otIdx, f->prim);
        AddPrim((u32*)db->small + f->otIdx, extra[0]);
        AddPrim((u32*)db->small + f->otIdx, extra[1]);
        AddPrim((u32*)db->small + f->otIdx, extra[2]);
        if (f->twoSided != 0) {
            (*cursor)->w0 = *getScratchAddr(0x1B);
            (*cursor)->w1 = *getScratchAddr(0x1C);
            (*cursor)->w2 = *getScratchAddr(0x1D);
        } else {
            (*cursor)->w0 = *getScratchAddr(0x18);
            (*cursor)->w1 = *getScratchAddr(0x19);
            (*cursor)->w2 = *getScratchAddr(0x1A);
        }
        if ((*f->flags & 0x80) != 0) {
            (*cursor)->w2 = *f->back;
            ((u8*)*cursor)[4] = f->code;
        }
        AddPrim((u32*)db->small + f->otIdx, *cursor);
        *cursor += 1;
    } else {
        AddPrim((u32*)db->small + f->otIdx, f->prim);
        AddPrim((u32*)db->small + f->otIdx, extra[0]);
        AddPrim((u32*)db->small + f->otIdx, extra[1]);
        AddPrim((u32*)db->small + f->otIdx, extra[2]);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/hier_sub", subAddPrim);
#endif
