#ifndef __TM1_DB_H__
#define __TM1_DB_H__

#include "common.h"
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/grutils.h"
#include "tm1/ua_dash.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sw.h"

union DbNode;

typedef struct DbNodeHdr {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
} DbNodeHdr;

typedef struct DbSubNode {
    /* 0x00 */ u8 pad00[8];
    /* 0x08 */ union DbNode* f8;
} DbSubNode;

typedef struct DbNode1 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[16];
    /* 0x14 */ u8 nChild;
    /* 0x15 */ u8 pad15[3];
    /* 0x18 */ union DbNode* child[1];
} DbNode1;

typedef struct DbNode2 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[12];
    /* 0x10 */ s32 nChild;
    /* 0x14 */ DbSubNode* child[1];
} DbNode2;

typedef struct DbNode3 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[6];
    /* 0x0A */ u8 fA;
    /* 0x0B */ u8 nChild;
    /* 0x0C */ u8 padC;
    /* 0x0D */ u8 fD;
    /* 0x0E */ u8 padE[2];
    /* 0x10 */ union DbNode* child[1];
} DbNode3;

typedef struct DbNode4 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ s32 t[3];
    /* 0x10 */ u8 pad10[2];
    /* 0x12 */ u8 nChild;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ union DbNode* child[1];
} DbNode4;

typedef struct DbNode5 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ MATRIX mat;
    /* 0x24 */ u8 pad24[2];
    /* 0x26 */ u8 nChild;
    /* 0x27 */ u8 pad27;
    /* 0x28 */ union DbNode* child[1];
} DbNode5;

typedef struct DbNode9 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[2];
    /* 0x06 */ u8 nChild;
    /* 0x07 */ u8 pad07;
    /* 0x08 */ union DbNode* child[1];
} DbNode9;

typedef struct DbNode11 {
    /* 0x00 */ u8 op;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ u16 kind;
    /* 0x04 */ u8 pad04[8];
    /* 0x0C */ union DbNode* childA;
    /* 0x10 */ union DbNode* childB;
} DbNode11;

/* A DMD hierarchy node; byte 0 is the opcode that selects the layout. */
typedef union DbNode {
    DbNodeHdr h;
    DbNode1 v1;
    DbNode2 v2;
    DbNode3 v3;
    DbNode4 v4;
    DbNode5 v5;
    DbNode9 v9;
    DbNode11 v11;
    DbSwitch sw;
    GrObj gr;
    EffNode eff;
    WheelNode wheel;
    DashNode dash;
} DbNode;

void dbInit(u32 tmsVersion);
Cs* dbInitCsForAModel(void* node);
void dbScanForInteractiveStuff(DbNode* group);
void dbProcessInteractives(s32 type, DbNode* node, s32* loc, s32 parent, s32* parentLoc);
void dbReset3DEnvironmentTrap(void);

#endif /* __TM1_DB_H__ */
