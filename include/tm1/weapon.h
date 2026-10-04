#ifndef __TM1_WEAPON_H__
#define __TM1_WEAPON_H__

#include "common.h"
#include <libgpu.h>
#include <libgte.h>

#include "tm1/cs.h"
#include "tm1/grutils.h"
#include "tm1/math.h"
#include "tm1/rt.h"

typedef struct Bullet {
    /* 0x00 */ VECTOR3 pos;
    /* 0x0C */ VECTOR3 prev;
    /* 0x18 */ VECTOR3 vel;
    /* 0x24 */ s32 owner;
    /* 0x28 */ s32 life;
    /* 0x2C */ s32 damage;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 kind;
    /* 0x35 */ u8 pad35[3];
    /* 0x38 */ s32 unk38;
} Bullet; /* 0x3C */

typedef struct ContrailPt {
    /* 0x00 */ u8 f00;
    /* 0x01 */ u8 f01;
    /* 0x02 */ u8 f02;
    /* 0x03 */ u8 f03;
    /* 0x04 */ s16 f04;
    /* 0x06 */ s16 f06;
    /* 0x08 */ s16 f08;
    /* 0x0A */ s16 f0A;
    /* 0x0C */ unsigned int b0 : 16;
    unsigned int b1 : 3;
    unsigned int b2 : 3;
    unsigned int b3 : 2;
    unsigned int b4 : 4;
    unsigned int b5 : 3;
    unsigned int b6 : 1;
    /* 0x10 */ u8 r;
    /* 0x11 */ u8 g;
    /* 0x12 */ u8 b;
    /* 0x13 */ u8 code;
    /* 0x14 */ u8 u0;
    /* 0x15 */ u8 v0;
    /* 0x16 */ u16 clut;
    /* 0x18 */ u8 u1;
    /* 0x19 */ u8 v1;
    /* 0x1A */ u16 tpage;
    /* 0x1C */ u8 u2;
    /* 0x1D */ u8 v2;
    /* 0x1E */ u16 pad1E;
    /* 0x20 */ u8 u3;
    /* 0x21 */ u8 v3;
    /* 0x22 */ u16 pad22;
} ContrailPt; /* 0x24 */

typedef struct TrailPt {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 pad;
} TrailPt; /* 0x08 */

typedef struct Missile {
    /* 0x000 */ u8 pad00[0x18];
    /* 0x018 */ VECTOR3 pos;
    /* 0x024 */ Cs* cs;
    /* 0x028 */ VECTOR3* target;
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 index;
    /* 0x031 */ u8 pad31[3];
    /* 0x034 */ s32 life;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ s32 unk3C;
    /* 0x040 */ s32 damage;
    /* 0x044 */ s32 unk44;
    /* 0x048 */ s32 unk48;
    /* 0x04C */ u8 unk4C;
    /* 0x04D */ u8 unk4D;
    /* 0x04E */ u8 pad4E[2];
    /* 0x050 */ s32 unk50;
    /* 0x054 */ u8 unk54;
    /* 0x055 */ u8 unk55;
    /* 0x056 */ s16 unk56;
    /* 0x058 */ TrailPt trail[21];
    /* 0x100 */ Cs* csB;
    /* 0x104 */ u8 f104;
    /* 0x105 */ u8 pad105;
    /* 0x106 */ s16 f106;
    /* 0x108 */ TrailPt* f108;
    /* 0x10C */ s32 f10C;
    /* 0x110 */ ContrailPt* f110;
    /* 0x114 */ s32 numPts;
    /* 0x118 */ s16 f118;
    /* 0x11A */ s16 f11A;
    /* 0x11C */ s32 f11C;
    /* 0x120 */ VECTOR3 f120;
    /* 0x12C */ s32 f12C;
    /* 0x130 */ u8 pad130[0x30];
    /* 0x160 */ ContrailPt pts[18];
} Missile; /* 0x3E8 */

extern Bullet blist[40];
extern GrSprite BulletSprites[32];
extern POLY_F4 fxSheet[2];
extern Missile mlist[10];

void bulSetOwnship(Cs* ownship);
void bulSetEyeWeaponGraphics(GrSprite* info);
void bulSetPlasmaGraphics(GrSprite* info);
void bulSetFireballGraphics(GrSprite* info);
void bulSetContrailGraphics(GrSprite* info);
void bulAddCs(void* node, s32 id);
void bulMakeSpecialBullet(s32 id, GrObj* obj);
s32 CAR_HD(VECTOR3* pt, s32 owner, s32 flag);
void init_bullets(void);
void destroy_all_bullets(void);
void create_bullet(s32 owner, VECTOR3* dir, VECTOR3* pos, s32 damage);
void s_create_bullet(
    s32 owner, VECTOR3* dir, VECTOR3* pos, s32 kind, s32 damage, s32 life, s32 arg6);
void move_bullets(void);
void display_bullets(Db* db, s32 which);
void kill_bullet(s32 i);
s32 find_free_bullet(s32 count);
void update_bullets(void);
void InitContrails(void);
void setContrailColor(u32 i, u8 r, u8 g, u8 b);
void startContrail(s32 i);
void animate_contrails(Missile* m);
void init_missiles(void);
void kill_missile(s16 i, s32 arg1);
void destroy_all_missiles(void);
s32 find_free_missile(s32 i);
void drop_contrail(Missile* m, s32 i);
void basic_missile_launch(Missile* m, VECTOR3* rot, VECTOR3* pos);
s16 create_FIRE_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_DEATHSPEAR_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_FREEZE_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_POWER_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_REAR_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_SINGING_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_GHOST_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_LOS_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_HOMING_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
s16 create_SWARM_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage);
void turn_heatseeker(Missile* m);
void move_missile(Missile* m);
Missile* get_missile(u32 i);
void move_missiles(void);
void bulBoltZap(VECTOR* p1, VECTOR* p2);
void SetFXSheet(s32 count, s32 r, s32 g, s32 b);
void DisplayFXSheet(u_long* ot);
s32 bulDispatchDamage(s32 id, s32 a, s32 b, s32 damage, VECTOR3* dir, s32 owner);

#endif /* __TM1_WEAPON_H__ */
