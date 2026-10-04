#ifndef __TM1_HIER_SUB_H__
#define __TM1_HIER_SUB_H__

#include "common.h"

#include "tm1/rt.h"

typedef struct SubPacket {
    /* 0x00 */ u32 w0;
    /* 0x04 */ u32 w1;
    /* 0x08 */ u32 w2;
} SubPacket;

typedef struct SubPoly {
    /* 0x00 */ s32* flags;
    /* 0x04 */ s32 otIdx;
    /* 0x08 */ void* prim;
    /* 0x0C */ u16 code;
    /* 0x0E */ u16 twoSided;
    /* 0x10 */ s32* back;
} SubPoly; /* 0x14 */

void subPoly3(Db* db, SubPoly* f, s32 count);
void subLoadPoly3Words(s32 col, s32 row);
void subPoly4(Db* db, SubPoly* f, s32 count);
void subLoadPoly4Words(s32 col, s32 row);
void subAddPrim(Db* db, SubPoly* f, SubPacket** cursor, void** extra);

#endif /* __TM1_HIER_SUB_H__ */
