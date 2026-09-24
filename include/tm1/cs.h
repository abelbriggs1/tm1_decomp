#ifndef __TM1_CS_H__
#define __TM1_CS_H__

#include "common.h"
#include "tm1/long_vector.h"
#include <libgte.h>

/* Coordinate system record -- 0x10C bytes (csCreate strides by 0x10C). */
typedef struct Cs {
    /* 0x000 */ s32 unk00;
    /* 0x004 */ void* env; /* lightGetEnv() result */
    /* 0x008 */ s32 drawMode;
    /* 0x00C */ s32 unk0C;
    /* 0x010 */ struct Cs* parent;
    /* 0x014 */ MATRIX mat;
    /* 0x034 */ MATRIX wmat;
    /* 0x054 */ SVECTOR rot;
    /* 0x05C */ LVECTOR pos;
    /* 0x068 */ LVECTOR wpos;
    /* 0x074 */ s32 unk74; /* collision mode-count bound */
    /* 0x078 */ u8 pad78[0xC0 - 0x78];
    /* 0x0C0 */ s32 unkC0;
    /* 0x0C4 */ MATRIX mat3;
    /* 0x0E4 */ MATRIX mat4;
    /* 0x104 */ void* epNode;
    /* 0x108 */ struct Cs* next;
} Cs;

extern Cs* csCreate(void);
extern void csInit(void);
extern Cs* csGetWorldCs(void);
extern Cs* csGetCsList(void);
extern void csAddToCsList(Cs* cs);
extern void csSetEpNode(Cs* cs, void* node);
extern void csSetNumInActiveDb(s32 n);
extern void csSetDrawMode(Cs* cs, s32 mode);
extern void csUpdMat(Cs* cs, MATRIX* out);

#endif /* __TM1_CS_H__ */
