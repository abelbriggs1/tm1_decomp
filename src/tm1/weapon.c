#include "common.h"

#include <libgpu.h>
#include <libgte.h>
#include <rand.h>

#include "tm1/car.h"
#include "tm1/cs.h"
#include "tm1/explosion.h"
#include "tm1/grutils.h"
#include "tm1/hdp.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/sound.h"
#include "tm1/targets.h"
#include "tm1/ua.h"
#include "tm1/ua_sound.h"
#include "tm1/view.h"

#include "tm1/weapon.h"

#define ABS(a) __builtin_abs(a)
#define BIGGER(a, b) (ABS(a) < ABS(b) ? (b) : (a))

#define CONTRAIL_UV(p, q)                                                                          \
    (p)->u1 = contrailInfo[q].u0;                                                                  \
    (p)->v1 = contrailInfo[q].v0;                                                                  \
    (p)->u0 = contrailInfo[q].u0 + contrailInfo[q].w;                                              \
    (p)->v0 = contrailInfo[q].v0;                                                                  \
    (p)->u3 = contrailInfo[q].u0;                                                                  \
    (p)->v3 = contrailInfo[q].v0 + contrailInfo[q].h;                                              \
    (p)->u2 = contrailInfo[q].u0 + contrailInfo[q].w;                                              \
    (p)->v2 = contrailInfo[q].v0 + contrailInfo[q].h;                                              \
    (p)->clut = contrailInfo[q].clut;                                                              \
    (p)->tpage = contrailInfo[q].tpage;

#define CONTRAIL_PT(k)                                                                             \
    mathMulTransVec(&m->cs->mat, &in, &out);                                                       \
    m->trail[k].x = m->cs->pos.vx + out.vx - m->pos.vx;                                            \
    m->trail[k].y = m->cs->pos.vy + out.vy - m->pos.vy;                                            \
    m->trail[k].z = m->cs->pos.vz + out.vz - m->pos.vz;

static s32 bulletFlipFlop = 0;
void* fire_missile_node = 0;
void* homing_missile_node = 0;
void* swarm_missile_node = 0;
void* ghost_missile_node = 0;
void* deathspear_node = 0;
void* lava_bomb_node = 0;
void* RAMS_HEAD_MISSILE = 0;
void* TOWER_SPIKE_MISSILE = 0;
static s32 fxSheetCount = 0;

static Cs* jays_ownship;
static GrSprite* eyeWeaponInfo;
static GrSprite* contrailInfo;
static GrSprite* plasmaInfo;
static GrSprite* fireballInfo;

void bulSetOwnship(Cs* ownship)
{
    jays_ownship = ownship;
}

void bulSetEyeWeaponGraphics(GrSprite* info)
{
    eyeWeaponInfo = info;
}

void bulSetPlasmaGraphics(GrSprite* info)
{
    plasmaInfo = info;
}

void bulSetFireballGraphics(GrSprite* info)
{
    fireballInfo = info;
}

void bulSetContrailGraphics(GrSprite* info)
{
    contrailInfo = info;
}

void bulAddCs(void* node, s32 id)
{
    switch (id) {
    case 201:
    case 202:
    case 203:
    case 204:
    case 205:
    case 206:
        fire_missile_node = node;
        swarm_missile_node = node;
        break;
    case 311:
        ghost_missile_node = node;
        break;
    case 312:
        deathspear_node = node;
        break;
    case 307:
        swarm_missile_node = node;
        break;
    }
}

void bulMakeSpecialBullet(s32 id, GrObj* obj)
{
    switch (id) {
    case 0x12D:
        grutilsParse3DSprite(obj, &BulletSprites[0], 4);
        break;
    case 0x12E:
        grutilsParse3DSprite(obj, &BulletSprites[4], 4);
        break;
    case 0x135:
        grutilsParse3DSprite(obj, &BulletSprites[8], 2);
        grutilsParse3DSprite(obj, &BulletSprites[10], 2);
        break;
    case 0x134:
        grutilsParse3DSprite(obj, &BulletSprites[12], 4);
        break;
    }
}

#ifdef NON_MATCHING
s32 CAR_HD(VECTOR3* pt, s32 owner, s32 flag)
{
    s32 i;
    s32 np;
    s32 nai;
    Cs* cs;
    Target* t;
    VECTOR3* p;

    np = (s16)GetNumPlayers();
    i = 0;
    nai = (s16)GetNumAICars();
    for (i = 0; i < np; i++) {
        cs = GetPlayerCs3D(i);
        if (cs->unkC0 != owner) {
            if (ABS(BIGGER(pt->vx - cs->pos.vx, BIGGER(pt->vy - cs->pos.vy, pt->vz - cs->pos.vz)))
                >> 3 < flag + 6) {
                return cs->unkC0;
            }
        }
    }
    for (i = 0; i < nai; i++) {
        cs = GetAICs3D(i);
        if (cs->unkC0 != owner) {
            if (ABS(BIGGER(pt->vx - cs->pos.vx, BIGGER(pt->vy - cs->pos.vy, pt->vz - cs->pos.vz)))
                >> 3 < flag + 6) {
                return cs->unkC0;
            }
        }
    }
    t = get_targets();
    for (i = 0; i < 50; i++, t++) {
        if (t->kind > 0 && t->type != -owner) {
            if (ABS(BIGGER(pt->vx - t->pos.vx, BIGGER(pt->vy - t->pos.vy, pt->vz - t->pos.vz))) >> 3
                < flag + 5) {
                return i + 500;
            }
        }
    }
    if (check_ped_hits(pt, 0)) {
        return 8;
    }
    for (i = 0; i < 2; i++) {
        p = get_hcop_position(i);
        if (p != 0) {
            if (ABS(BIGGER(pt->vx - p->vx, BIGGER(pt->vy - p->vy, pt->vz - p->vz))) >> 3
                < flag + 4) {
                return i + 100;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", CAR_HD);
#endif

void init_bullets(void)
{
    s32 i;
    for (i = 39 * sizeof(Bullet); i >= 0; i -= sizeof(Bullet)) {
        ((Bullet*)((u8*)blist + i))->life = 0;
    }
}

void destroy_all_bullets(void)
{
    s32 i;

    for (i = 0; i < 40; i++) {
        if (blist[i].life > 0) {
            do_mini_explosion(&blist[i].pos);
        }
        blist[i].life = 0;
    }
}

#ifdef NON_MATCHING
void create_bullet(s32 owner, VECTOR3* dir, VECTOR3* pos, s32 damage)
{
    s32 i;
    s32 big;
    s32 mag;
    s32 life;
    VECTOR3 vel;
    Bullet* b;

    i = find_free_bullet(40);
    if (i >= 0) {
        b = &blist[i];
        big = BIGGER(dir->vx, BIGGER(dir->vy, dir->vz));
        mag = ABS(big);
        if (mag <= 0) {
            mag = 1;
        }
        if (owner == 1) {
            vel.vx = dir->vx * 250 / mag;
            vel.vy = dir->vy * 250 / mag;
            vel.vz = dir->vz * 250 / mag;
            life = 15;
        } else {
            vel.vx = dir->vx * 250 / mag;
            vel.vy = dir->vy * 250 / mag;
            vel.vz = dir->vz * 250 / mag;
            life = 15;
            if (owner >= 50) {
                life = 20;
            }
        }
        b->life = life;
        b->prev.vx = pos->vx;
        b->prev.vy = pos->vy;
        b->prev.vz = pos->vz;
        b->vel.vx = vel.vx;
        b->vel.vy = vel.vy;
        b->vel.vz = vel.vz;
        b->pos.vx = pos->vx;
        b->pos.vy = pos->vy;
        b->pos.vz = pos->vz;
        b->owner = owner;
        b->kind = 0;
        b->unk30 = 0;
        b->damage = damage;
        b->unk38 = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_bullet);
#endif

#ifdef NON_MATCHING
void s_create_bullet(
    s32 owner, VECTOR3* dir, VECTOR3* pos, s32 kind, s32 damage, s32 life, s32 arg6)
{
    s32 i;
    s32 big;
    s32 mag;
    VECTOR3 vel;
    Bullet* b;

    i = find_free_bullet(40);
    if (i >= 0) {
        b = &blist[i];
        big = BIGGER(dir->vx, BIGGER(dir->vy, dir->vz));
        mag = ABS(big);
        if (mag <= 0) {
            mag = 1;
        }
        if (kind == 4 || kind == 6) {
            mag = mag * 5 / 4;
        }
        if (kind >= 15 || kind == 5) {
            mag = mag * 2;
        }
        vel.vx = dir->vx * 250 / mag;
        vel.vy = dir->vy * 250 / mag;
        vel.vz = dir->vz * 250 / mag;
        b->prev.vx = pos->vx;
        b->prev.vy = pos->vy;
        b->prev.vz = pos->vz;
        b->vel.vx = vel.vx;
        b->vel.vy = vel.vy;
        b->vel.vz = vel.vz;
        b->pos.vx = pos->vx;
        b->pos.vy = pos->vy;
        b->pos.vz = pos->vz;
        b->life = life;
        b->owner = owner;
        b->kind = kind;
        b->unk30 = 0;
        b->damage = damage;
        b->unk38 = arg6;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", s_create_bullet);
#endif

#ifdef NON_MATCHING
void move_bullets(void)
{
    s32 i;
    s32 k;
    s32 range;
    u8 hitok;
    Bullet* b;
    HdCsHit* hit;
    VECTOR3 p;
    VECTOR3 q;

    hit = 0;
    for (i = 0; i < 40; i++) {
        if (blist[i].life > 0) {
            b = &blist[i];
            b->life--;
            b->prev.vx = b->pos.vx;
            b->prev.vy = b->pos.vy;
            b->prev.vz = b->pos.vz;
            b->pos.vx += b->vel.vx;
            b->pos.vy += b->vel.vy;
            b->pos.vz += b->vel.vz;
            p.vx = b->prev.vx;
            p.vy = b->prev.vy;
            p.vz = b->prev.vz;
            for (k = 3; k >= 0; k--) {
                q.vx = p.vx;
                q.vy = p.vy;
                q.vz = p.vz;
                p.vx = b->pos.vx - ((b->vel.vx * k) >> 2);
                p.vy = b->pos.vy - ((b->vel.vy * k) >> 2);
                p.vz = b->pos.vz - ((b->vel.vz * k) >> 2);
                b->unk30 = CAR_HD(&p, b->owner, (b->kind >= 15) * 4);
                if (b->unk30 == b->owner) {
                    b->unk30 = 0;
                }
                if (b->unk30 == 0 && p.vz <= 0) {
                    if (shellGetCurrentLevel() != 3 || b->vel.vz < 0) {
                        b->unk30 = 8;
                    }
                }
                if (k == 2 && b->unk30 == 0) {
                    hit = HdPntTest(b->owner, -1, &p, &b->unk30);
                    if (hit->tag0 == -b->owner || hit->tag0 == 0x3FD) {
                        hit->tag0 = 0;
                        b->unk30 = 0;
                    }
                }
                if (b->unk30 > 0) {
                    if (b->unk30 == 1) {
                        q.vx = p.vx;
                        q.vy = p.vy;
                        q.vz = p.vz;
                    }
                    do_simple_spark(&q);
                    if (rand() < 20000) {
                        do_puff(&q);
                    }
                    if (b->unk30 != 8) {
                        do_flash(&q);
                    }
                    if (b->kind >= 15) {
                        do_simple_explosion(&q);
                        q.vz = 0;
                        do_flames(&q);
                    }
                    b->life = 0;
                    soundSetRangeAndXPositionFromWorldLoc(&p.vx);
                    range = soundGetCalculatedSoundRange();
                    uasoundExplodeMachineGunBullet(range, soundGetCalculatedSoundXPosition(), 25);
                    hitok = bulDispatchDamage(
                        b->unk30, hit->tag0, hit->tag1, b->damage, &b->vel, b->owner);
                    k = -1;
                    if (b->kind == 3 && hitok) {
                        do_blue_explosion(&p);
                    }
                }
            }
            if (b->life <= 0 && b->kind == 7) {
                do_big_explosion(&b->pos);
                do_smoke(&b->pos);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", move_bullets);
#endif

#ifdef NON_MATCHING
void display_bullets(Db* db, s32 which)
{
    s32 i;
    s32 k;
    s32 d;
    s32 e;
    s32 next;
    s32 hfov;
    s32 z0;
    s32 z1;
    s32 sz;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    Bullet* b;
    GrSprite* sp;
    POLY_FT4* ft;
    LINE_G2* ln;
    POLY_G3* g3;
    VECTOR in;
    VECTOR r0;
    VECTOR r1;
    s32 cx;
    s32 cy;
    u_long* ot;
    s32 vfov;
    EyeMat* mat;
    EyeTrans* eye;

    sz = 1;
    ot = (u_long*)db->small;
    viewGetCenter(which, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(which);
    vfov = viewGetCurrentVertFOVH(which);
    eye = viewGetEyeTrans(which);
    ln = 0;
    ft = 0;
    g3 = 0;
    mat = viewGetEyeMat(which);
    if (fxSheetCount > 0 && which == 0) {
        DisplayFXSheet(ot);
    }
    displayDropWeapons(db, which);
    displayFlamethrowers();
    draw_tasers(db, which);
    for (i = 0; i < 40; i++) {
        if (blist[i].life <= 0) {
            continue;
        }
        b = &blist[i];
        in.vx = b->pos.vx + eye->x;
        in.vy = b->pos.vy + eye->y;
        in.vz = b->pos.vz + eye->z;
        mathMulVec(mat, &in, &r0);
        in.vx = b->prev.vx + eye->x;
        in.vy = b->prev.vy + eye->y;
        in.vz = b->prev.vz + eye->z;
        mathMulVec(mat, &in, &r1);
        z0 = r0.vz;
        z1 = r1.vz;
        if (z0 <= 0 || z1 <= 0) {
            continue;
        }
        if (b->kind & 1) {
            ft = (POLY_FT4*)db->unk8;
            next = db->unk8 + 160;
            if ((u8*)next >= db->areaEnd) {
                continue;
            }
            db->unk8 = next;
            setPolyFT4(ft);
            setSemiTrans(ft, 1);
        } else if (b->kind < 3) {
            ln = (LINE_G2*)db->unk8;
            next = db->unk8 + 80;
            if ((u8*)next >= db->areaEnd) {
                continue;
            }
            db->unk8 = next;
            setLineG2(ln);
        } else {
            g3 = (POLY_G3*)db->unk8;
            next = db->unk8 + 112;
            if ((u8*)next >= db->areaEnd) {
                continue;
            }
            db->unk8 = next;
            SetPolyG3(g3);
            setSemiTrans(g3, 0);
        }
        if (z0 >= 32001) {
            continue;
        }
        if (!(b->kind & 1)) {
            sz = (b->life & 0x1f) + 223;
            if (b->life <= 0) {
                break;
            }
            if (b->kind < 3) {
                if (b->kind == 0) {
                    ln->r0 = sz;
                    ln->g0 = sz;
                    ln->b0 = sz >> 2;
                } else if (b->kind == 2) {
                    ln->r0 = sz;
                    ln->g0 = sz;
                    ln->b0 = sz;
                } else {
                    ln->r0 = 0xFF;
                    ln->g0 = 0x96;
                    ln->b0 = 0x96;
                }
                sz -= 60;
                if (b->kind == 0) {
                    ln->r1 = sz;
                    ln->g1 = sz >> 1;
                    ln->b1 = 0;
                } else if (b->kind == 2) {
                    ln->r1 = sz >> 1;
                    ln->g1 = sz >> 1;
                    ln->b1 = sz;
                } else {
                    ln->r0 = 0xFF;
                    ln->g0 = 0x4B;
                    ln->b0 = 0x4B;
                }
            } else if (b->kind == 4) {
                g3->r0 = sz;
                g3->g0 = sz;
                g3->b0 = sz;
                g3->r1 = sz;
                g3->g1 = sz;
                g3->b1 = sz;
                g3->r2 = sz >> 1;
                g3->g2 = sz >> 1;
                g3->b2 = sz;
            } else {
                g3->r0 = sz;
                g3->g0 = sz >> 1;
                g3->b0 = sz >> 1;
                g3->r1 = sz;
                g3->g1 = sz >> 1;
                g3->b1 = sz >> 1;
                g3->r2 = sz >> 1;
                g3->g2 = 0;
                g3->b2 = 0;
            }
        } else if (b->kind == 1) {
            k = b->life & 3;
            ft->u0 = eyeWeaponInfo[k].u0;
            ft->v0 = eyeWeaponInfo[k].v0;
            ft->u1 = eyeWeaponInfo[k].u0 + eyeWeaponInfo[k].w;
            ft->v1 = eyeWeaponInfo[k].v0;
            ft->u2 = eyeWeaponInfo[k].u0;
            ft->v2 = eyeWeaponInfo[k].v0 + eyeWeaponInfo[k].h;
            ft->u3 = eyeWeaponInfo[k].u0 + eyeWeaponInfo[k].w;
            ft->v3 = eyeWeaponInfo[k].v0 + eyeWeaponInfo[k].h;
            ft->clut = eyeWeaponInfo[k].clut;
            ft->r0 = 0xAC;
            ft->g0 = 0xAC;
            ft->b0 = 0xAC;
            sz = (hfov << 7) / z0;
            ft->tpage = eyeWeaponInfo[k].tpage;
        } else if (b->kind == 3) {
            k = b->life & 3;
            sz = (b->damage >> 1) + 72;
            if (sz >= 256) {
                sz = 255;
            }
            ft->u0 = plasmaInfo[k].u0;
            ft->v0 = plasmaInfo[k].v0;
            ft->u1 = plasmaInfo[k].u0 + plasmaInfo[k].w;
            ft->v1 = plasmaInfo[k].v0;
            ft->u2 = plasmaInfo[k].u0;
            ft->v2 = plasmaInfo[k].v0 + plasmaInfo[k].h;
            ft->u3 = plasmaInfo[k].u0 + plasmaInfo[k].w;
            ft->v3 = plasmaInfo[k].v0 + plasmaInfo[k].h;
            ft->clut = plasmaInfo[k].clut;
            ft->tpage = plasmaInfo[k].tpage;
            switch (b->life & 3) {
            case 0:
                ft->r0 = sz;
                ft->g0 = sz;
                ft->b0 = sz;
                break;
            case 1:
                ft->r0 = sz;
                ft->g0 = sz >> 1;
                ft->b0 = sz >> 1;
                break;
            case 2:
                ft->r0 = sz >> 1;
                ft->g0 = sz;
                ft->b0 = sz >> 1;
                break;
            case 3:
                ft->r0 = sz >> 1;
                ft->g0 = sz >> 1;
                ft->b0 = sz;
                break;
            }
            if (b->damage < 310) {
                sz = (b->damage + 10) * hfov / z0;
            } else {
                sz = 320 * hfov / z0;
            }
            b->damage--;
            k = b->life & 3;
            ft->clut = plasmaInfo[k].clut;
            ft->tpage = plasmaInfo[k].tpage;
            if (b->damage <= 0) {
                b->life = 0;
            }
        } else if (b->kind == 5 || b->kind == 7) {
            k = b->life & 3;
            ft->u0 = BulletSprites[k].u0;
            ft->v0 = BulletSprites[k].v0;
            ft->u1 = BulletSprites[k].u0 + BulletSprites[k].w;
            ft->v1 = BulletSprites[k].v0;
            ft->u2 = BulletSprites[k].u0;
            ft->v2 = BulletSprites[k].v0 + BulletSprites[k].h;
            ft->u3 = BulletSprites[k].u0 + BulletSprites[k].w;
            ft->v3 = BulletSprites[k].v0 + BulletSprites[k].h;
            ft->clut = BulletSprites[k].clut;
            if (b->kind == 5) {
                ft->r0 = 0x96;
                ft->g0 = 0xFF;
                ft->b0 = 0x14;
            } else {
                ft->r0 = 0xFF;
                ft->g0 = 0x14;
                ft->b0 = 0x14;
            }
            sz = (hfov << 6) / z0;
            ft->tpage = BulletSprites[k].tpage;
        } else if (b->kind == 9) {
            k = rand() & 7;
            ft->u0 = fireballInfo[k].u0;
            ft->v0 = fireballInfo[k].v0;
            ft->u1 = fireballInfo[k].u0 + fireballInfo[k].w;
            ft->v1 = fireballInfo[k].v0;
            ft->u2 = fireballInfo[k].u0;
            ft->v2 = fireballInfo[k].v0 + fireballInfo[k].h;
            ft->u3 = fireballInfo[k].u0 + fireballInfo[k].w;
            ft->v3 = fireballInfo[k].v0 + fireballInfo[k].h;
            ft->clut = fireballInfo[k].clut;
            sz = (hfov << 8) / z0;
            ft->tpage = fireballInfo[k].tpage;
        } else if (b->kind >= 15 && (b->kind & 1)) {
            sp = &BulletSprites[((b->kind - 15) >> 1) * 4 + ((b->life >> 1) & 3)];
            ft->u0 = sp->u0;
            ft->v0 = sp->v0;
            ft->u1 = sp->u0 + sp->w;
            ft->v1 = sp->v0;
            ft->u2 = sp->u0;
            ft->v2 = sp->v0 + sp->h;
            ft->u3 = sp->u0 + sp->w;
            ft->v3 = sp->v0 + sp->h;
            ft->tpage = sp->tpage;
            ft->clut = sp->clut;
            if (b->kind != 17) {
                ft->r0 = 0x80;
                ft->g0 = 0x80;
                ft->b0 = 0x80;
            } else {
                ft->r0 = 0xFF;
                ft->g0 = 0xDC;
                ft->b0 = 0xDC;
            }
            sz = (hfov << 5) / z0;
        }
        x0 = r0.vx * hfov / z0 + cx;
        y0 = r0.vy * vfov / z0 + cy;
        x1 = r1.vx * hfov / z1 + cx;
        y1 = r1.vy * vfov / z1 + cy;
        if (!(b->kind & 1) && -(cx * 2) < x0 && x0 < cx * 4 && -(cy * 2) < y0 && y0 < cy * 4
            && -(cx * 2) < x1 && x1 < cx * 4 && -(cy * 2) < y1 && y1 < cy * 4) {
            if (b->kind < 3) {
                ln->x0 = x0;
                ln->y0 = y0;
                ln->x1 = x1;
                ln->y1 = y1;
                AddPrim(ot + (z0 >> 3), ln);
            } else {
                g3->x2 = x1;
                g3->y2 = y1;
                d = (y0 - y1) / 12 + 1;
                g3->x0 = x0 - d;
                g3->x1 = x0 + d;
                e = (x0 - x1) / 12;
                g3->y0 = y0 + e;
                g3->y1 = y0 - e;
                AddPrim(ot + (z0 >> 3), g3);
            }
        } else if ((b->kind & 1) && -(cx * 2) < x0 && x0 < cx * 4 && -(cy * 2) < y0
            && y0 < cy * 4) {
            ft->x0 = x0 - sz;
            ft->y0 = y0 - sz;
            ft->x1 = (x0 - sz) + sz * 2;
            ft->y1 = y0 - sz;
            ft->x2 = x0 - sz;
            ft->y2 = (y0 - sz) + sz * 2;
            ft->x3 = (x0 - sz) + sz * 2;
            ft->y3 = (y0 - sz) + sz * 2;
            AddPrim(ot + (z0 >> 3), ft);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", display_bullets);
#endif

void kill_bullet(s32 i)
{
    blist[i].life = 0;
}

s32 find_free_bullet(s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        if (blist[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

#ifdef NON_MATCHING
void update_bullets(void)
{
    bulletFlipFlop = 1 - bulletFlipFlop;
    move_bullets();
    jays_ownship = GetPlayerCs3D(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", update_bullets);
#endif

#ifdef NON_MATCHING
void InitContrails(void)
{
    s32 i;
    s32 j;
    TrailPt* tp;
    VECTOR3* vp;
    s32* fp;

    for (i = 0; i < 10; i++) {
        for (j = 0; j < 18; j++) {
            mlist[i].pts[j].b0 = 0;
            mlist[i].pts[j].f00 = 4;
            mlist[i].pts[j].f08 = 2;
            mlist[i].pts[j].f0A = 3;
            mlist[i].pts[j].code = 0x2E;
            mlist[i].pts[j].f01 = 1;
            mlist[i].pts[j].f02 = 9;
            mlist[i].pts[j].f03 = 9;
            mlist[i].pts[j].f04 = 0;
            mlist[i].pts[j].f06 = 1;
            mlist[i].pts[j].r = 0x80;
            mlist[i].pts[j].g = 0x80;
            mlist[i].pts[j].b = 0x80;
            mlist[i].pts[j].b1 = 4;
            mlist[i].pts[j].b2 = 0;
            mlist[i].pts[j].b3 = 2;
            mlist[i].pts[j].b4 = 0;
            mlist[i].pts[j].b5 = 1;
            mlist[i].pts[j].b6 = 0;
        }
    }
    for (i = 0; i < 10; i++) {
        tp = mlist[i].trail;
        mlist[i].f110 = mlist[i].pts;
        mlist[i].numPts = 18;
        mlist[i].f11A = 1;
        mlist[i].f104 = 0;
        mlist[i].f106 = 0;
        mlist[i].f108 = tp;
        mlist[i].f10C = 0;
        fp = &mlist[i].f11C;
        *fp = 0x2000;
        mlist[i].f118 = 0;
        vp = &mlist[i].f120;
        vp->vx = 0;
        vp->vy = 0;
        vp->vz = 0;
        mlist[i].f12C = 0x7FFF;
        mlist[i].csB = csCreate();
        mlist[i].csB->epNode = &mlist[i].f104;
        mlist[i].csB->drawMode = 0;
        mlist[i].csB->pos.vx = 0;
        mlist[i].csB->pos.vy = 0;
        mlist[i].csB->pos.vz = 0;
        csAddToCsList(mlist[i].csB);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", InitContrails);
#endif

void setContrailColor(u32 i, u8 r, u8 g, u8 b)
{
    s32 j;
    ContrailPt* p;

    if (i < 10) {
        p = mlist[i].pts;
        for (j = 0; j < 18; j++) {
            p[j].r = r;
            p[j].g = g;
            p[j].b = b;
        }
    }
}

#ifdef NON_MATCHING
void startContrail(s32 i)
{
    Missile* m;
    s32 j;
    s32 k;
    VECTOR back;
    VECTOR out;

    m = &mlist[i];
    m->numPts = m->unk54 * 3;
    for (j = 0; j < m->unk54; j++) {
        m->pts[j * 3 + 0].f08 = 0;
        m->pts[j * 3 + 0].f0A = 0;
        m->pts[j * 3 + 0].f04 = 1;
        m->pts[j * 3 + 0].f06 = 1;
        m->pts[j * 3 + 1].f08 = 1;
        m->pts[j * 3 + 1].f0A = 1;
        m->pts[j * 3 + 1].f04 = 2;
        m->pts[j * 3 + 1].f06 = 2;
        m->pts[j * 3 + 2].f08 = 2;
        m->pts[j * 3 + 2].f0A = 2;
        m->pts[j * 3 + 2].f04 = 0;
        m->pts[j * 3 + 2].f06 = 0;
    }
    back.vx = 0;
    back.vy = -200;
    back.vz = 0;
    mathMulTransVec(&m->cs->mat, &back, &out);
    m->csB->pos.vx = m->cs->pos.vx;
    m->csB->pos.vy = m->cs->pos.vy;
    m->csB->pos.vz = m->cs->pos.vz;
    for (j = 0; j < m->unk54 + 1; j++) {
        k = j * 3;
        m->trail[k].x = out.vx;
        m->trail[k].y = out.vy;
        m->trail[k].z = out.vz;
        k++;
        m->trail[k].x = out.vx;
        m->trail[k].y = out.vy;
        m->trail[k].z = out.vz;
        k++;
        m->trail[k].x = out.vx;
        m->trail[k].y = out.vy;
        m->trail[k].z = out.vz;
    }
    m->csB->drawMode = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", startContrail);
#endif

#ifdef NON_MATCHING
void animate_contrails(Missile* m)
{
    s32 head;
    s32 j;
    s32 q;
    s32 k;

    head = m->unk4C;
    for (j = 0; j < m->unk54; j++) {
        q = (j * 8 + (m->unk54 >> 1)) / m->unk54;
        k = head * 3;
        CONTRAIL_UV(&m->pts[k], q)
        k++;
        CONTRAIL_UV(&m->pts[k], q)
        k++;
        CONTRAIL_UV(&m->pts[k], q)
        head--;
        if (head < 0) {
            head = m->unk54 - 1;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", animate_contrails);
#endif

#ifdef NON_MATCHING
void init_missiles(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        mlist[i].cs = csCreate();
        csAddToCsList(mlist[i].cs);
        mlist[i].cs->unkC0 = 10;
        mlist[i].life = -1;
        mlist[i].cs->drawMode = 0;
        mlist[i].index = i;
        mlist[i].life = -15;
        mlist[i].cs->unk0C = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", init_missiles);
#endif

#ifdef NON_MATCHING
void kill_missile(s16 i, s32 arg1)
{
    mlist[i].life = -1;
    mlist[i].cs->drawMode = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", kill_missile);
#endif

void destroy_all_missiles(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        kill_missile(i, 0);
    }
}

s32 find_free_missile(s32 i)
{
    for (; i < 10; i++) {
        if (mlist[i].life <= 0) {
            return i;
        }
    }
    return -1;
}

#ifdef NON_MATCHING
void drop_contrail(Missile* m, s32 i)
{
    VECTOR in;
    VECTOR out;
    s32 h;
    s32 a;
    s32 b;

    m->unk55--;
    if (m->unk55 == 0) {
        a = m->unk4D * 3;
        m->unk4C++;
        if (m->unk4C >= m->unk54) {
            m->unk4C = 0;
        }
        h = m->unk4C * 3;
        m->pts[h].f0A = a + 1;
        m->pts[h].f06 = a;
        m->pts[h + 1].f0A = a + 2;
        m->pts[h + 1].f06 = a + 1;
        m->pts[h + 2].f0A = a;
        m->pts[h + 2].f06 = a + 2;
        m->unk4D++;
        if (m->unk4D >= 7) {
            m->unk4D = 0;
        }
        b = m->unk4D * 3;
        m->pts[h].f08 = b + 1;
        m->pts[h].f04 = b;
        m->pts[h + 1].f08 = b + 2;
        m->pts[h + 1].f04 = b + 1;
        m->pts[h + 2].f08 = b;
        m->pts[h + 2].f04 = b + 2;
        in.vx = 0;
        in.vy = 0;
        in.vz = 8;
        CONTRAIL_PT(b)
        in.vx = 8;
        in.vy = 0;
        in.vz = -8;
        CONTRAIL_PT(b + 1)
        in.vx = -8;
        in.vy = 0;
        in.vz = -8;
        CONTRAIL_PT(b + 2)
        m->unk55 = 2;
        animate_contrails(m);
    } else {
        in.vx = 0;
        in.vy = 0;
        in.vz = 8;
        b = m->unk4D * 3;
        CONTRAIL_PT(b)
        in.vx = 8;
        in.vy = 0;
        in.vz = -8;
        CONTRAIL_PT(b + 1)
        in.vx = -8;
        in.vy = 0;
        in.vz = -8;
        CONTRAIL_PT(b + 2)
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", drop_contrail);
#endif

#ifdef NON_MATCHING
void basic_missile_launch(Missile* m, VECTOR3* rot, VECTOR3* pos)
{
    Cs* cs = m->cs;
    s32 index;

    m->unk55 = 1;
    m->unk4C = 0;
    m->unk4D = 1;
    m->target = 0;
    m->unk54 = 6;
    cs->rot.vx = rot->vx;
    cs->rot.vy = rot->vy;
    cs->rot.vz = rot->vz;
    cs->pos.vx = pos->vx;
    cs->pos.vy = pos->vy;
    cs->pos.vz = pos->vz;
    m->pos.vx = pos->vx;
    m->pos.vy = pos->vy;
    m->pos.vz = pos->vz;
    if (cs->epNode == 0) {
        cs->drawMode = 0;
    } else {
        cs->drawMode = 1;
    }
    RotMatrixYXZ(&cs->rot, &cs->mat);
    index = m->index;
    setContrailColor(index, 0x80, 0x80, 0x80);
    m->unk56 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", basic_missile_launch);
#endif

s16 create_FIRE_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s16 i;
    s32 launch;

    do {
        launch = damage;
    } while (0);
    i = create_HOMING_missile(owner, target, rot, pos, launch);

    if (i >= 0) {
        mlist[i].unk3C = 10;
        setContrailColor(i, 0x80, 0x80, 0x80);
        mlist[i].unk48 = 0;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(0);
        uasoundPlayCarWeaponLaunchOrInflight(
            0, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}

#ifdef NON_MATCHING
s16 create_DEATHSPEAR_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s32 i;
    i = create_HOMING_missile(owner, target, rot, pos, damage);

    if (i >= 0) {
        if (deathspear_node != 0) {
            mlist[i].cs->epNode = deathspear_node;
        }
        mlist[i].unk3C = 10;
        mlist[i].unk54 = 1;
        setContrailColor(i, 0x32, 0, 0);
        mlist[i].csB->drawMode = 0;
        mlist[i].unk48 = 11;
        do_steam(pos);
    }
    return i;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_DEATHSPEAR_missile);
#endif

s16 create_FREEZE_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s16 i = create_LOS_missile(owner, rot, pos, damage);

    if (i >= 0) {
        setContrailColor(i, 0x20, 0x20, 0x9F);
        mlist[i].unk48 = 1;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(1);
        uasoundPlayCarWeaponLaunchOrInflight(
            1, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}

s16 create_POWER_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s16 i = create_LOS_missile(owner, rot, pos, damage);

    if (i >= 0) {
        setContrailColor(i, 0xFA, 0, 0);
        mlist[i].unk48 = 3;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(3);
        uasoundPlayCarWeaponLaunchOrInflight(
            3, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}

#ifdef NON_MATCHING
s16 create_REAR_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    VECTOR3 back;
    s16 i;

    back.vx = rot->vx;
    back.vy = rot->vy;
    back.vz = (rot->vz + 2048) & 0xFFF;
    i = create_HOMING_missile(owner, target, &back, pos, damage);
    if (i >= 0) {
        mlist[i].unk3C = 10;
        setContrailColor(i, 0x80, 0x80, 0x80);
        mlist[i].unk48 = 5;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(5);
        uasoundPlayCarWeaponLaunchOrInflight(
            5, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_REAR_missile);
#endif

s16 create_SINGING_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s16 i = create_LOS_missile(owner, rot, pos, damage);

    if (i >= 0) {
        setContrailColor(i, 0x10, 0x80, 0x10);
        mlist[i].unk48 = 4;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(4);
        uasoundPlayCarWeaponLaunchOrInflight(
            4, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}

s16 create_GHOST_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s32 i;
    s32 launch;

    do {
        launch = damage;
    } while (0);
    i = create_SWARM_missile(owner, target, rot, pos, launch);

    if (i >= 0) {
        setContrailColor(i, 0xCD, 0xCD, 0xFF);
        if (ghost_missile_node != 0) {
            mlist[i].cs->epNode = ghost_missile_node;
        }
        mlist[i].unk3C = 100;
        mlist[i].unk38 = 75;
        mlist[i].unk2C = 3;
        mlist[i].life = 80;
        mlist[i].unk48 = 4;
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        uasoundStopCarWeapon(4);
        uasoundPlayCarWeaponLaunchOrInflight(
            4, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
    return i;
}

#ifdef NON_MATCHING
s16 create_LOS_missile(s32 owner, VECTOR3* rot, VECTOR3* pos, s32 damage)
{

    s32 i;
    Missile* m;

    i = find_free_missile(0);
    if (i < 0) {
        return -1;
    }
    {
        m = &mlist[i];
        m->cs->epNode = fire_missile_node;
        basic_missile_launch(m, rot, pos);
        m->unk2C = 0;
        m->life = 30;
        m->unk38 = 150;
        m->damage = damage;
        m->target = 0;
        m->unk50 = owner;
        m->unk48 = 0;
        startContrail(i);
        animate_contrails(m);
        drop_contrail(m, i);
        return i;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_LOS_missile);
#endif

#ifdef NON_MATCHING
s16 create_HOMING_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s32 i;
    Missile* m;
    Cs* cs;

    i = find_free_missile(0);
    if (i < 0) {
        return -1;
    } else {
        m = &mlist[i];
        cs = m->cs;
        basic_missile_launch(m, rot, pos);
        m->unk2C = 1;
        m->life = 30;
        m->unk38 = 150;
        m->target = target;
        m->damage = damage;
        m->unk50 = owner;
        m->unk3C = 50;
        cs->epNode = fire_missile_node;
        setContrailColor(i, 0x40, 0, 0x40);
        startContrail(i);
        animate_contrails(m);
        drop_contrail(m, i);
        if (cs->epNode != 0) {
            cs->drawMode = 1;
        } else {
            cs->drawMode = 0;
        }
        soundSetRangeAndXPositionFromWorldLoc(&pos->vx);
        m->unk48 = 2;
        uasoundStopCarWeapon(2);
        uasoundPlayCarWeaponLaunchOrInflight(
            2, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
        return i;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_HOMING_missile);
#endif

#ifdef NON_MATCHING
s16 create_SWARM_missile(s32 owner, VECTOR3* target, VECTOR3* rot, VECTOR3* pos, s32 damage)
{
    s32 i;
    Missile* m;
    Cs* cs;

    i = find_free_missile(0);
    if (i < 0) {
        return -1;
    } else {
        m = &mlist[i];
        cs = m->cs;
        basic_missile_launch(m, rot, pos);
        m->unk2C = 2;
        m->life = 30;
        m->unk38 = 150;
        m->target = target;
        m->unk3C = 0x23;
        m->damage = damage;
        m->unk50 = owner;
        cs->epNode = swarm_missile_node;
        startContrail(i);
        drop_contrail(m, i);
        if (cs->epNode != 0) {
            cs->drawMode = 1;
        } else {
            cs->drawMode = 0;
        }
        return i;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", create_SWARM_missile);
#endif

#ifdef NON_MATCHING
void turn_heatseeker(Missile* m)
{
    VECTOR3* tgt;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 amin;
    s32 amax;
    s32 dist;
    s32 yaw;
    s32 pitch;
    s32 cur;
    s32 d;

    tgt = m->target;
    dx = tgt->vx - m->cs->pos.vx;
    dy = tgt->vy - m->cs->pos.vy;
    dz = tgt->vz - m->cs->pos.vz;
    yaw = (ratan2(dx, dy) + 0x1000) % 0x1000;

    amin = ABS(dx);
    amax = ABS(dy);
    if (amax < amin) {
        amin = amax;
    }
    amin = amin >> 1;
    if (ABS(dx) < amax) {
        dist = amax + amin;
    } else {
        dist = ABS(dx) + amin;
    }
    pitch = (0x1000 - ratan2(dz, dist)) % 0x1000;
    if (pitch > 0x400 && pitch < 0xC00) {
        pitch = (0x1400 - pitch) % 0x1000;
    }

    cur = m->cs->rot.vz;
    if (cur - yaw >= 0x801) {
        yaw += 0x1000;
    }
    if (yaw - cur >= 0x801) {
        yaw -= 0x1000;
    }
    d = cur - yaw;
    if (d < 0) {
        d = -d;
    }
    if (m->unk3C >= d) {
        m->cs->rot.vz = yaw;
    } else if (cur < yaw) {
        m->cs->rot.vz = m->cs->rot.vz + m->unk3C;
    } else {
        m->cs->rot.vz = m->cs->rot.vz - m->unk3C;
    }
    if (m->unk2C == 2 || m->unk2C == 3) {
        m->cs->rot.vz = m->cs->rot.vz + ((rand() % (m->unk3C * 4)) - (m->unk3C * 2));
    }
    if (m->cs->rot.vz >= 0x1000) {
        m->cs->rot.vz = m->cs->rot.vz - 0x1000;
    }
    if (m->cs->rot.vz < 0) {
        m->cs->rot.vz = m->cs->rot.vz + 0x1000;
    }

    cur = m->cs->rot.vx;
    if (cur - pitch >= 0x801) {
        pitch += 0x1000;
    }
    if (pitch - cur >= 0x801) {
        pitch -= 0x1000;
    }
    d = cur - pitch;
    if (d < 0) {
        d = -d;
    }
    if (m->unk3C >= d) {
        m->cs->rot.vx = pitch;
    } else if (cur < pitch) {
        m->cs->rot.vx = m->cs->rot.vx + m->unk3C;
    } else {
        m->cs->rot.vx = m->cs->rot.vx - m->unk3C;
    }
    if (m->unk2C == 2 || m->unk2C == 3) {
        if (m->unk2C == 2) {
            m->cs->rot.vx = m->cs->rot.vx + ((rand() % (m->unk3C * 2)) - m->unk3C);
        } else {
            m->cs->rot.vx = m->cs->rot.vx + ((rand() % (m->unk3C * 4)) - (m->unk3C * 2));
        }
    }
    if (m->cs->rot.vx >= 0x1000) {
        m->cs->rot.vx = m->cs->rot.vx - 0x1000;
    }
    if (m->cs->rot.vx < 0) {
        m->cs->rot.vx = m->cs->rot.vx + 0x1000;
    }
    RotMatrixYXZ(&m->cs->rot, &m->cs->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", turn_heatseeker);
#endif

#ifdef NON_MATCHING
void move_missile(Missile* m)
{
    HdCsHit* hit;
    s32 exploded;
    u8 idx;
    s32 i;
    VECTOR fwd;
    VECTOR d;
    VECTOR3 prev;
    VECTOR3 pt;

    hit = 0;
    exploded = 0;
    idx = m->index;
    if (m->target != 0) {
        turn_heatseeker(m);
    }
    if (m->life <= 0) {
        m->life--;
    } else {
        fwd.vz = 0;
        fwd.vx = 0;
        fwd.vy = m->unk38;
        if (m->unk38 < 150) {
            m->unk38 = m->unk38 + 5;
        } else {
            m->unk38 = 150;
        }
        mathMulTransVec(&m->cs->mat, &fwd, &d);
        pt.vx = m->cs->pos.vx;
        pt.vy = m->cs->pos.vy;
        pt.vz = m->cs->pos.vz;
        m->cs->pos.vx += d.vx;
        m->cs->pos.vy += d.vy;
        m->cs->pos.vz += d.vz;
        m->life = m->life - 1;
        soundSetRangeAndXPositionFromWorldLoc(&m->cs->pos.vx);
        if (m->unk56 >= 16) {
            m->cs->pos.vz -= 16;
            m->unk56 -= 16;
        } else if (m->unk56 > 0) {
            m->cs->pos.vz -= m->unk56;
            m->unk56 = 0;
        }
        uasoundPlayCarWeaponLaunchOrInflight(
            m->unk48, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
        for (i = 1; i >= 0; i--) {
            prev.vx = pt.vx;
            prev.vy = pt.vy;
            prev.vz = pt.vz;
            pt.vx = m->cs->pos.vx - ((d.vx * i) >> 1);
            pt.vy = m->cs->pos.vy - ((d.vy * i) >> 1);
            pt.vz = m->cs->pos.vz - ((d.vz * i) >> 1);
            m->unk44 = CAR_HD(&pt, m->unk50, 3);
            if (m->unk44 == m->unk50) {
                m->unk44 = 0;
            }
            if (m->unk2C == 3) {
                if (m->unk44 >= 200) {
                    m->unk44 = 0;
                } else if (m->unk44 < 0) {
                    m->unk44 = 0;
                }
            }
            if ((i & 1) && m->unk2C != 3 && m->unk44 == 0) {
                hit = HdPntTest(m->unk50, m->cs->unkC0, &pt, &m->unk44);
                if (m->unk44 == 8 && hit->tag0 == -m->unk50) {
                    m->unk44 = 0;
                }
                if (hit->tag0 == 1021) {
                    m->unk44 = 0;
                }
            }
            if (m->unk44 > 0) {
                pt.vz += 24;
                if (m->unk44 != 8) {
                    if (m->damage >= 0) {
                        do_simple_explosion(&pt);
                    } else {
                        do_blue_explosion(&pt);
                    }
                    if (m->unk48 == 11) {
                        do_steam(&pt);
                    } else {
                        do_smoke(&pt);
                    }
                } else {
                    if (m->damage < 0) {
                        do_blue_explosion(&pt);
                    } else {
                        do_mini_explosion(&pt);
                    }
                }
                kill_missile(idx, 1);
                exploded = 1;
                i = -1;
                bulDispatchDamage(
                    m->unk44, hit->tag0, hit->tag1, m->damage, (VECTOR3*)&d, m->unk50);
            }
        }
        if (m->unk2C != 3 && m->cs->pos.vz < 0) {
            if (shellGetCurrentLevel() != 3) {
                m->life = 0;
            } else if (m->cs->pos.vx < 6800) {
                m->life = 0;
            } else if (m->cs->pos.vx >= 8201) {
                m->life = 0;
            }
        }
        if (m->life == 0) {
            if (m->cs->pos.vz <= 0) {
                m->cs->pos.vz = 0;
            }
            exploded = 0;
            do_mini_explosion((VECTOR3*)&m->cs->pos);
            kill_missile(idx, 0);
        }
    }
    if (m->cs->epNode != RAMS_HEAD_MISSILE && m->cs->epNode != TOWER_SPIKE_MISSILE) {
        drop_contrail(m, idx);
    }
    if (exploded) {
        soundSetRangeAndXPositionFromWorldLoc(&m->cs->pos.vx);
        uasoundPlayCarWeaponExplode(
            m->unk48, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition(), 99);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", move_missile);
#endif

Missile* get_missile(u32 i)
{
    if (i >= 10) {
        return 0;
    }
    return &mlist[i];
}

void move_missiles(void)
{
    s32 i;

    for (i = 0; i < 10; i++) {
        if (mlist[i].life == 0) {
            kill_missile(i, 0);
        }
        if (mlist[i].life >= -11) {
            move_missile(&mlist[i]);
        } else {
            mlist[i].csB->drawMode = 0;
        }
    }
}

#ifdef NON_MATCHING
void bulBoltZap(VECTOR* p1, VECTOR* p2)
{
    Db* cdb;
    s32 i;
    s32 hfov;
    s32 vfov;
    EyeTrans* eye;
    EyeMat* mat;
    LINE_G2* line;
    VECTOR v;
    VECTOR3 pts[16];
    s32 scr[16][3];
    VECTOR delta;
    s32 cx;
    s32 cy;
    u32* ot;

    cdb = rtGetCdb();
    ot = (u32*)cdb->small;
    viewGetCenter(0, &cx, &cy);
    hfov = viewGetCurrentHorzFOVH(0);
    vfov = viewGetCurrentVertFOVH(0);
    eye = viewGetEyeTrans(0);
    mat = viewGetEyeMat(0);

    delta.vx = p2->vx - p1->vx;
    delta.vy = p2->vy - p1->vy;
    delta.vz = p2->vz - p1->vz;

    pts[0].vx = p1->vx;
    pts[0].vy = p1->vy;
    pts[0].vz = p1->vz + 30;
    pts[15].vx = p2->vx;
    pts[15].vy = p2->vy;
    pts[15].vz = p2->vz;

    for (i = 1; i < 15; i++) {
        pts[i].vx = p1->vx + delta.vx * i / 16;
        pts[i].vy = p1->vy + delta.vy * i / 16;
        pts[i].vz = delta.vz * i / 16 + 30 + p1->vz;
        pts[i].vz += (rand() & 0xff) - 128;
        pts[i].vy += (rand() & 0x3f) - 32;
        pts[i].vx += (rand() & 0x3f) - 32;
    }

    for (i = 0; i < 16; i++) {
        v.vx = pts[i].vx + eye->x;
        v.vy = pts[i].vy + eye->y;
        v.vz = pts[i].vz + eye->z;
        mathMulVec(mat, &v, scr[i]);
        if (scr[i][2] > 0) {
            scr[i][0] = cx + scr[i][0] * hfov / scr[i][2];
            scr[i][1] = cy + scr[i][1] * vfov / scr[i][2];
            scr[i][2] = scr[i][2] / 4;
            if (scr[i][0] < -4000 || scr[i][0] > 4000) {
                scr[i][2] = 0;
            }
            if (scr[i][1] < -4000 || scr[i][1] > 4000) {
                scr[i][2] = 0;
            }
        }
    }

    if ((rand() & 7) == 0) {
        do_simple_spark(&pts[rand() & 0xf]);
    }

    for (i = 0; i < 15; i++) {
        if (scr[i][2] > 0 && scr[i + 1][2] > 0 && scr[i][2] < 4091 && scr[i + 1][2] < 4091) {
            line = (LINE_G2*)cdb->unk8;
            if ((u8*)line < cdb->areaEnd) {
                setLineG2(line);
                cdb->unk8 += 80;
                setRGB0(line, -6 - i * 12, -6 - i * 12, 255);
                setRGB1(line, -6 - (i + 1) * 12, -6 - (i + 1) * 12, 255);
                line->x0 = scr[i][0];
                line->y0 = scr[i][1];
                line->x1 = scr[i + 1][0];
                line->y1 = scr[i + 1][1];
                AddPrim(ot + scr[i + 1][2], line);
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", bulBoltZap);
#endif

#ifdef NON_MATCHING
void SetFXSheet(s32 count, s32 r, s32 g, s32 b)
{
    setPolyF4(&fxSheet[0]);
    setPolyF4(&fxSheet[1]);
    setRGB0(&fxSheet[0], r, g, b);
    setRGB0(&fxSheet[1], r, g, b);
    setXY4(&fxSheet[0], 0, 0, 320, 0, 0, 240, 320, 240);
    setXY4(&fxSheet[1], 0, 0, 320, 0, 0, 240, 320, 240);
    SetSemiTrans(&fxSheet[0], 1);
    fxSheetCount = count;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", SetFXSheet);
#endif

#ifdef NON_MATCHING
void DisplayFXSheet(u_long* ot)
{
    s32 r0;
    s32 g0;
    s32 b0;
    s32 r;
    s32 g;
    s32 b;

    AddPrim(ot, &fxSheet[bulletFlipFlop]);
    fxSheetCount--;
    if (fxSheetCount < 8) {
        r0 = fxSheet[0].r0;
        g0 = fxSheet[0].g0;
        b0 = fxSheet[0].b0;
        r = (r0 * 3 + 128) >> 2;
        g = (g0 * 3 + 128) >> 2;
        b = (b0 * 3 + 128) >> 2;
        fxSheet[0].r0 = r;
        fxSheet[0].g0 = g;
        fxSheet[0].b0 = b;
        fxSheet[1].r0 = r;
        fxSheet[1].g0 = g;
        fxSheet[1].b0 = b;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", DisplayFXSheet);
#endif

#ifdef NON_MATCHING
s32 bulDispatchDamage(s32 id, s32 a, s32 b, s32 damage, VECTOR3* dir, s32 owner)
{
    Target* t;

    if (id >= 500) {
        t = get_targets();
        a = t[id - 500].type;
        b = t[id - 500].instance;
        id = 8;
    }
    if (id > 0 && id != 8) {
        if (id == 100) {
            a = 750;
            merc_takehit(a, damage);
            return 1;
        }
        if (id == 101) {
            a = 752;
            merc_takehit(a, damage);
            return 1;
        }
        if (id >= 150) {
            ped_takehit(id - 150, (s32)dir);
            return 1;
        }
        carTakeHit((s16)id, damage, (s32)dir, 0);
        return 1;
    }
    switch (a) {
    case 0x258:
    case 0x259:
    case 0x25B:
    case 0x262:
    case 0x263:
    case 0x264:
    case 0x265:
    case 0x2BE:
    case 0x2BF:
    case 0x2C1:
    case 0x2C3:
    case 0x2C4:
    case 0x2C9:
    case 0x2CB:
    case 0x2CC:
    case 0x2D0:
    case 0x2D1:
    case 0x2DA:
    case 0x302:
    case 0x386:
    case 0x3E8:
    case 0x3E9:
    case 0x3EA:
    case 0x3EB:
    case 0x3EC:
    case 0x3ED:
    case 0x3F0:
    case 0x3F1:
    case 0x3F2:
    case 0x3F7:
    case 0x3F9:
        target_takehit(a, b, damage);
        return 1;
    case 0x2EE:
    case 0x2F0:
        merc_takehit(a, damage);
        return 1;
    case 0x2F1:
    case 0x2F2:
    case 0x2F3:
        static_cop_takehit(b);
        return 1;
    case 0x3F8:
        drop_box_takehit();
        return 1;
    case 0x2F8:
    case 0x309:
    case 0x30A:
    case 0x30D:
    case 0x30E:
        pedestrian_takehit(a, (s32)dir);
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/weapon", bulDispatchDamage);
#endif
