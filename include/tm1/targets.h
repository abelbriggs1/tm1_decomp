#ifndef __TM1_TARGETS_H__
#define __TM1_TARGETS_H__

#include "common.h"

#include "tm1/cs.h"
#include "tm1/db.h"
#include "tm1/math.h"
#include "tm1/ua_sw.h"

typedef struct Target {
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 instance;
    /* 0x08 */ s32 kind;
    /* 0x0C */ u8 flag;
    /* 0x10 */ VECTOR3 pos;
} Target; /* 0x1C */

typedef struct HoverMerc {
    /* 0x00 */ s32 state;
    /* 0x04 */ Cs* obj;
    /* 0x08 */ DbSwitch* node;
    /* 0x0C */ s32 timer;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s32 unk18;
} HoverMerc; /* 0x1C */

typedef struct PedPath {
    /* 0x00 */ s32 x0;
    /* 0x04 */ s32 y0;
    /* 0x08 */ s32 x1;
    /* 0x0C */ s32 y1;
} PedPath; /* 0x10 */

typedef struct Pedestrian {
    /* 0x00 */ Cs* obj;
    /* 0x04 */ DbSwitch* node;
    /* 0x08 */ s32 id;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 pathIdx;
    /* 0x18 */ PedPath* path;
    /* 0x1C */ s16 dir;
    /* 0x1E */ s16 unk1E;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    /* 0x28 */ s16 unk28;
    /* 0x2A */ s16 unk2A;
} Pedestrian; /* 0x2C */

typedef struct StaticCop {
    /* 0x00 */ s32 state;
    /* 0x04 */ DbSwitch* node;
    /* 0x08 */ s16 hits;
    /* 0x0A */ s16 pad0A;
    /* 0x0C */ s32 kind;
    /* 0x10 */ VECTOR3 pos;
} StaticCop; /* 0x1C */

s32 find_free_target(void);
void clear_targets(void);
void init_target(s32 type, s32 instance, VECTOR3* pos);
Target* get_targets(void);
s32 target_takehit(s32 type, s32 instance, s32 damage);
void turnOnHoverCopSounds(s32* loc, u8 firing, u8 launching);
void move_mercs(void);
void merc_takehit(s32 which, s32 who);
void init_merc(s32 which, DbNode* ep);
VECTOR3* get_hcop_position(s32 num);
void clear_mercs(void);
void clear_pedestrians(void);
void choose_ped_path(Pedestrian* ped);
void init_pedestrian(s32 id, DbNode* ep);
void move_pedestrians(void);
void pedestrian_takehit(s32 id, s32 damage);
void ped_takehit(s32 num, s32 damage);
s32 check_ped_hits(VECTOR3* pos, s32 damage);
void init_static_cops(void);
void setup_static_cop(DbSwitch* node, VECTOR3* pos);
void static_cop_fire(s32 num, VECTOR3* tgt);
void move_static_cops(void);
void static_cop_takehit(s32 num);
void move_drop_box(void);
void move_breaking_window(void);
void drop_box_takehit(void);
void window_takehit(void);
void move_targets(void);

#endif /* __TM1_TARGETS_H__ */
