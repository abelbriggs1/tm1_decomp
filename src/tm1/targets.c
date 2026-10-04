#include "common.h"
#include <libgte.h>
#include <rand.h>
#include <stdio.h>

#include "tm1/cs.h"
#include "tm1/db.h"
#include "tm1/explosion.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/rt.h"
#include "tm1/shell.h"
#include "tm1/sound.h"
#include "tm1/targets.h"
#include "tm1/ua.h"
#include "tm1/ua_sound.h"
#include "tm1/ua_sw.h"
#include "tm1/weapon.h"

#define TGT_ABS(a) __builtin_abs(a)
#define TGT_MAX(a, b) ((a) < (b) ? (b) : (a))
#define TGT_LONGER(a, b) (TGT_ABS(a) < TGT_ABS(b) ? (b) : (a))
#define TGT_SHORTER(a, b) (TGT_ABS(a) < TGT_ABS(b) ? (a) : (b))
#define TGT_DIST2(a, b) (TGT_ABS(TGT_LONGER(a, b)) + (TGT_ABS(TGT_SHORTER(a, b)) >> 1))

extern s32 D_801713CC[16];
extern s32 D_8017140C[12];
extern s32 D_8017143C[28];
extern s32 D_801714AC[24];

extern Target target[50];
extern HoverMerc hovermerc[2];

Pedestrian pedestrian[6];
StaticCop scop[25];

s32 gTargetUnk1036 = 4;
s32 gTargetUnk1040 = 240;
s32 merc_made = 0;
s32 gTargetUnk1048 = 200;
s32 gTargetUnk1052 = 250;
s32 ped_made = 0;
s32 drop_box_state = 0;
s32 window_state = 0;
s32 gTargetUnk1068 = 0;

s32 D_8017150C[10] = {
    0x226,
    0x226,
    0x352,
    0x4FB,
    0x384,
    0x226,
    0x32,
    0x3E8,
    0x41A,
    0x401,
};

s32 D_80171534[5] = {
    0x3CA,
    0x384,
    0x3E8,
    0x366,
    0x3E8,
};

PedPath pedPathLevel1[3] = {
    { -1230, 845, -1230, 895 },
    { -655, -168, -605, -168 },
    { -1745, -72, -1795, -72 },
};

PedPath pedPathLevel2[5] = {
    { -2172, -420, -2172, -348 },
    { -2125, -545, -2040, -545 },
    { -2125, -833, -2040, -833 },
    { 2038, 691, 2123, 691 },
    { 2038, 547, 2123, 547 },
};

PedPath pedPathLevel3[9] = {
    { -373, -846, -373, -891 },
    { -52, 134, 28, 134 },
    { 229, 846, 229, 896 },
    { 996, -648, 996, -528 },
    { 1252, -431, 1147, -431 },
    { 546, -625, 646, -625 },
    { 860, 345, 860, 250 },
    { 1148, -121, 1253, -121 },
    { 1333, -604, 1333, -654 },
};

PedPath pedPathLevel3b[1] = {
    { 756, 96, 826, -57 },
};

PedPath pedPathLevel4[14] = {
    { 65, 810, 65, 985 },
    { 1090, 1258, 1090, 1142 },
    { 812, 668, 984, 668 },
    { 1240, 1430, 1240, 1629 },
    { 1890, 1170, 1890, 1270 },
    { 3510, 1445, 3510, 1555 },
    { 3510, 235, 3510, 365 },
    { 1105, 1440, 1354, 1440 },
    { 3230, 1120, 3230, 1250 },
    { 3230, 680, 3365, 680 },
    { 537, 982, 663, 982 },
    { 130, 542, 130, 658 },
    { 1000, 254, 1000, 346 },
    { 1550, 1238, 1950, 1238 },
};

PedPath pedPathLevel5[2] = {
    { 10, 10, 590, 10 },
    { 1510, 10, 1965, 10 },
};

s32 find_free_target(void)
{
    s32 i;

    for (i = 0; i < 50; i++) {
        if (target[i].kind < 0) {
            return i;
        }
    }
    return -1;
}

void clear_targets(void)
{
    s32 i;

    for (i = 0; i < 50; i++) {
        target[i].kind = -1;
    }
    clear_mercs();
    clear_pedestrians();
}

#ifdef NON_MATCHING
void init_target(s32 type, s32 instance, VEC3* pos)
{
    s32 i;
    s32 t;
    VEC3* d;

    i = find_free_target();
    if (i < 0) {
        printf("\nERROR! TARGET TYPE %d INSTANCE %d won't fit!\n", type, instance);
        return;
    }
    target[i].type = type;
    target[i].instance = instance;
    d = &target[i].pos;
    t = pos->x;
    d->x = t;
    t = pos->y;
    d->y = t;
    t = pos->z;
    d->z = t;
    target[i].kind = 1;
    target[i].flag = 0;
    switch (type) {
    case 0x258:
    case 0x25B:
    case 0x3F9:
        target[i].kind = 8;
        target[i].flag = 1;
        break;
    case 0x259:
        target[i].kind = 3;
        target[i].flag = 1;
        break;
    case 0x262:
        target[i].kind = 5;
        target[i].flag = 1;
        break;
    case 0x263:
        target[i].kind = 15;
        target[i].flag = 1;
        break;
    case 0x264:
        target[i].kind = 10;
        target[i].flag = 1;
        break;
    case 0x265:
        target[i].kind = 20;
        target[i].flag = 1;
        break;
    case 0x3F7:
        target[i].kind = 2;
        break;
    case 0x2BE:
        target[i].kind = 2;
        break;
    case 0x2CB:
        target[i].kind = 12;
        break;
    case 0x2CC:
        target[i].kind = 3;
        break;
    case 0x2D0:
        target[i].kind = 3;
        break;
    case 0x386:
        target[i].kind = 25;
        target[i].flag = 1;
        break;
    case 0x3E8:
        target[i].kind = 100;
        target[i].flag = 1;
        break;
    case 0x3EA:
    case 0x3F1:
    case 0x3F2:
        target[i].kind = 4;
        target[i].flag = 1;
        break;
    case 0x302:
    case 0x3E9:
        target[i].flag = 1;
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", init_target);
#endif

Target* get_targets(void)
{
    return target;
}

#ifdef NON_MATCHING
s32 target_takehit(s32 type, s32 instance, s32 damage)
{
    VEC3 p;
    VEC3* d;
    s32 i;
    s32 which;
    s32 x;
    s32 y;
    s32 z;

    which = -1;
    for (i = 0; i < 50; i++) {
        if (target[i].type == type && target[i].instance == instance && target[i].kind > 0) {
            which = i;
        }
    }
    if (which < 0) {
        return 0;
    }
    target[which].kind -= damage;
    if (target[which].kind > 0) {
        return 0;
    }
    d = &target[which].pos;
    do_smoke(d);
    do_puff(d);
    if (target[which].flag != 0) {
        uaswSetState(target[which].type, target[which].instance, 1);
    } else {
        uaswSetNumChildren(target[which].type, target[which].instance, 0);
    }
    switch (target[which].type) {
    case 611:
    case 613:
        x = target[which].pos.x;
        y = target[which].pos.y;
        z = target[which].pos.z;
        x -= 40;
        y += 40;
        z += 30;
        target[which].pos.x = x;
        target[which].pos.y = y;
        target[which].pos.z = z;
        do_bigger_explosion(&target[which].pos);
        do_simple_explosion(&target[which].pos);
        do_smoke(&target[which].pos);
        do_puff(&target[which].pos);
        x = target[which].pos.x;
        y = target[which].pos.y;
        z = target[which].pos.z;
        x += 40;
        y -= 40;
        z -= 30;
        target[which].pos.x = x;
        target[which].pos.y = y;
        target[which].pos.z = z;
        do_smoke(&target[which].pos);
        break;
    case 610:
    case 612:
        do_simple_explosion(&target[which].pos);
        do_flames(&target[which].pos);
        return 0;
    case 1001:
        soundSetRangeAndXPositionFromWorldLoc(&target[which].pos.x);
        uasoundPlayGeneralExplode(
            soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition(), 99);
        do_steam(&target[which].pos);
        window_takehit();
        break;
    case 600:
    case 601:
        do_simple_explosion(&target[which].pos);
        do_flames(&target[which].pos);
        break;
    case 1017:
        do_simple_explosion(&target[which].pos);
        do_flames(&target[which].pos);
        break;
    case 730:
        do_big_explosion(&target[which].pos);
        do_flames(&target[which].pos);
        carDropWeapon(9, 0, &target[which].pos.x, 0);
        break;
    case 1000:
        for (i = 0; i < 45; i++) {
            p.x = rand() % 2400 + 9600;
            p.y = rand() % 2400 + 4000;
            p.z = (rand() & 0x3FF) + 8000;
            do_big_smoke(&p);
            if (rand() & 1) {
                do_mondo_explosion(&p);
            }
        }
        break;
    case 720:
    case 770:
        uasoundPlayMaleScream();
        break;
    }
    return 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", target_takehit);
#endif

void turnOnHoverCopSounds(s32* loc, u8 firing, u8 launching)
{
    soundSetRangeAndXPositionFromWorldLoc(loc);
    uasoundPlayHoverCops(soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    if (firing) {
        uasoundFireCarMachineGuns(
            0x2F0, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    } else {
        uasoundStopCarMachineGuns(0x2F0);
    }
    if (launching) {
        uasoundPlayCarWeaponLaunchOrInflight(
            0, soundGetCalculatedSoundRange(), soundGetCalculatedSoundXPosition());
    }
}

// TODO: Using `INCLUDE_RODATA()` for these strings results in them being
// reordered against our will. `INCLUDE_ASM()` prevents reordering.
//
// Remove this once these strings are properly migrated.
INCLUDE_ASM("asm/nonmatchings/tm1/targets", D_800FC3C0);

INCLUDE_ASM("asm/nonmatchings/tm1/targets", D_800FC3E8);

INCLUDE_ASM("asm/nonmatchings/tm1/targets", D_800FC410);

INCLUDE_ASM("asm/nonmatchings/tm1/targets", D_800FC434);

#ifdef NON_MATCHING
void move_mercs(void)
{
    VECTOR pp;
    VEC3 aim;
    s32* table;
    s32 count;
    s32 last;
    s32 i;
    s32 j;
    s32 k;
    s32 best;
    s32 d;
    s32 r;
    s32 st;
    u8 firing;
    u8 launching;

    firing = 0;
    launching = 0;
    switch (shellGetCurrentLevel()) {
    case 1:
        table = D_801713CC;
        count = 8;
        i = 1;
        last = 2;
        break;
    case 2:
        table = D_8017140C;
        count = 6;
        i = 0;
        last = 1;
        break;
    case 3:
        table = D_8017143C;
        count = 14;
        i = 1;
        last = 2;
        break;
    case 4:
        table = D_801714AC;
        count = 12;
        i = 0;
        last = 1;
        break;
    case 5:
        table = D_8017150C;
        count = 5;
        i = 0;
        last = 2;
        break;
    default:
        return;
    }
    for (; i < last; i++) {
        st = hovermerc[i].unk14;
        if (st == 0) {
            hovermerc[i].timer--;
            if (hovermerc[i].timer <= 0) {
                hovermerc[i].unk14 = 1;
                GetPlayerPosition(0, (VEC3*)&pp);
                best = TGT_ABS(table[0] * 8 - pp.vx) + TGT_ABS(table[1] * 8 - pp.vy);
                k = 0;
                for (j = 1; j < count; j++) {
                    d = TGT_ABS(table[j * 2] * 8 - pp.vx) + TGT_ABS(table[j * 2 + 1] * 8 - pp.vy);
                    if (d < best && hovermerc[1 - i].unk16 != j) {
                        best = d;
                        k = j;
                    }
                }
                hovermerc[i].unk16 = k;
                hovermerc[i].obj->pos.vx = table[k * 2] * 8;
                hovermerc[i].obj->pos.vy = table[k * 2 + 1] * 8;
                hovermerc[i].obj->pos.vz = 400;
                hovermerc[i].unk18 = 0;
                if (shellGetCurrentLevel() == 5) {
                    hovermerc[i].obj->pos.vz += D_80171534[k] * 8;
                    hovermerc[i].unk18 = D_80171534[k] * 8;
                }
                hovermerc[i].timer = -1;
                hovermerc[i].node->state = 0;
                hovermerc[i].obj->drawMode = 1;
            }
        } else if (st == 1) {
            if (hovermerc[i].unk18 >= hovermerc[i].obj->pos.vz) {
                hovermerc[i].unk14 = 2;
                hovermerc[i].node->state = 1;
                hovermerc[i].timer = 16;
            } else {
                hovermerc[i].obj->pos.vz -= 4;
            }
        } else if (st == 2) {
            hovermerc[i].timer--;
            if (hovermerc[i].timer <= 0) {
                hovermerc[i].node->state = 2;
                hovermerc[i].unk14 = 3;
                r = rand() & 7;
                if (i != 0) {
                    hovermerc[i].timer = r + 10;
                } else {
                    hovermerc[i].timer = r + 18;
                }
                hovermerc[i].unk10 = 0;
            }
        } else if (st == 3) {
            hovermerc[i].timer--;
            if (hovermerc[i].timer > 0) {
                if (hovermerc[i].timer < 8) {
                    hovermerc[i].node->state = 2;
                }
                continue;
            }
            GetPlayerPosition(0, (VEC3*)&pp);
            if (hovermerc[i].state == 752) {
                if (hovermerc[i].unk10 < 5) {
                    hovermerc[i].obj->pos.vz += 48;
                    firing = 1;
                    aim.x = pp.vx - hovermerc[i].obj->pos.vx + rand() % gTargetUnk1048
                        - (gTargetUnk1048 >> 1);
                    aim.y = pp.vy - hovermerc[i].obj->pos.vy + rand() % gTargetUnk1048
                        - (gTargetUnk1048 >> 1);
                    aim.z = pp.vz - hovermerc[i].obj->pos.vz;
                    s_create_bullet(
                        -hovermerc[i].state, &aim, (VEC3*)&hovermerc[i].obj->pos, 2, 1, 28, 0);
                    hovermerc[i].node->state = 3;
                    hovermerc[i].timer = (rand() & 0x1F) + 10;
                    hovermerc[i].unk10++;
                    if (hovermerc[i].unk10 == 5) {
                        hovermerc[i].timer += 40 + (rand() & 0x1F);
                    }
                    do_flash((VEC3*)&hovermerc[i].obj->pos);
                    hovermerc[i].obj->pos.vz -= 48;
                    goto sound;
                }
            } else {
                k = hovermerc[i].unk10;
                if (shellGetCurrentLevel() == 5) {
                    if (k < 3) {
                        goto fire;
                    }
                } else if (k < 2) {
                    goto fire;
                }
            }
            hovermerc[i].node->state = 1;
            hovermerc[i].unk14 = 4;
            goto sound;
        fire:
            hovermerc[i].obj->pos.vz += 62;
            aim.x = 0;
            aim.y = 0;
            aim.z = ratan2(pp.vx - hovermerc[i].obj->pos.vx + rand() % gTargetUnk1048
                            - (gTargetUnk1048 >> 1),
                        pp.vy - hovermerc[i].obj->pos.vy)
                + rand() % gTargetUnk1048 - (gTargetUnk1048 >> 1);
            d = create_LOS_missile(-hovermerc[i].state, &aim, (VEC3*)&hovermerc[i].obj->pos, 2);
            if (d >= 0) {
                setContrailColor(d, 10, 50, 10);
            }
            do_flash((VEC3*)&hovermerc[i].obj->pos);
            do_puff((VEC3*)&hovermerc[i].obj->pos);
            hovermerc[i].obj->pos.vz -= 62;
            hovermerc[i].node->state = 3;
            launching = 1;
            hovermerc[i].timer = (rand() & 0x3F) + 30;
            hovermerc[i].unk10++;
            if (hovermerc[i].unk10 >= 2) {
                hovermerc[i].timer += 40 + (rand() & 0xF);
            }
        } else if (st == 4) {
            if (hovermerc[i].obj->pos.vz < hovermerc[i].unk18 + 400) {
                hovermerc[i].obj->pos.vz += 4;
            } else {
                hovermerc[i].unk14 = 0;
                hovermerc[i].timer = rand() % gTargetUnk1052 + (gTargetUnk1052 >> 1);
                hovermerc[i].node->state = 4;
                hovermerc[i].unk16 = -1;
            }
        }
    sound:
        turnOnHoverCopSounds(&hovermerc[i].obj->pos.vx, firing, launching);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", move_mercs);
#endif

#ifdef NON_MATCHING
void merc_takehit(s32 which, s32 who)
{
    s32 i;

    i = (which != 0x2EE);
    uasoundPlayMaleScream();
    hovermerc[i].node->state = 4;
    do_smoke((VEC3*)&hovermerc[i].obj->pos);
    do_burn((VEC3*)&hovermerc[i].obj->pos);
    do_steam((VEC3*)&hovermerc[i].obj->pos);
    do_simple_explosion((VEC3*)&hovermerc[i].obj->pos);
    hovermerc[i].obj->pos.vz = 0;
    do_flames((VEC3*)&hovermerc[i].obj->pos);
    hovermerc[i].unk14 = 0;
    hovermerc[i].timer = gTargetUnk1052 * 2 + rand() % gTargetUnk1052;
    hovermerc[i].obj->pos.vz = 1000;
    hovermerc[i].unk16 = -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", merc_takehit);
#endif

#ifdef NON_MATCHING
void init_merc(s32 which, DbNode* ep)
{
    s32 i;
    DbNode* n;
    DbSwitch* sw;

    i = (which != 0x2EE);
    hovermerc[i].state = which;
    hovermerc[i].timer = i * 200 + 200;
    hovermerc[i].unk10 = 0;
    hovermerc[i].obj = csCreate();
    if (hovermerc[i].obj == NULL) {
        printf("Unable to allocate CS for hovercop %ld\n", which);
        return;
    }
    n = ep->v1.child[0];
    if (n->h.op == 9) {
        sw = &n->sw;
    } else {
        sw = &n->v4.child[0]->sw;
    }
    if (sw->kind != 9) {
        printf("Unable to find switch for hovercop %ld\n", which);
        return;
    }
    csSetEpNode(hovermerc[i].obj, ep);
    csAddToCsList(hovermerc[i].obj);
    hovermerc[i].unk16 = -1;
    hovermerc[i].node = sw;
    hovermerc[i].unk14 = 0;
    merc_made++;
    hovermerc[i].obj->unkC0 = i + 100;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", init_merc);
#endif

VEC3* get_hcop_position(s32 num)
{
    if (num >= 0 && num < 2) {
        if (hovermerc[num].obj != NULL) {
            if (hovermerc[num].state > 0) {
                return (VEC3*)&hovermerc[num].obj->pos;
            }
        }
    }
    return NULL;
}

void clear_mercs(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        hovermerc[i].state = 0;
        hovermerc[i].obj = NULL;
    }
}

void clear_pedestrians(void)
{
    s32 i;

    for (i = 0; i < 6; i++) {
        pedestrian[i].obj = NULL;
    }
    ped_made = 0;
    init_static_cops();
}

#ifdef NON_MATCHING
void choose_ped_path(Pedestrian* ped)
{
    PedPath* table;
    s32 count;
    s32 n;
    s32 dx;
    s32 dy;

    switch (shellGetCurrentLevel()) {
    case 1:
        table = pedPathLevel1;
        count = 3;
        break;
    case 2:
        table = pedPathLevel2;
        count = 5;
        break;
    case 3:
        table = pedPathLevel3;
        count = 9;
        if (ped->id == 0x305) {
            table = pedPathLevel3b;
            count = 1;
        }
        break;
    case 4:
        table = pedPathLevel4;
        count = 14;
        break;
    case 5:
        table = pedPathLevel5;
        count = 2;
        break;
    default:
        return;
    }
    n = rand() % count;
    ped->pathIdx = n;
    ped->path = table;
    if (rand() & 1) {
        ped->dir = -1;
        ped->obj->pos.vx = table[n].x1 << 3;
        ped->obj->pos.vy = table[n].y1 << 3;
        ped->obj->pos.vz = 0;
        dx = table[n].x0 - table[n].x1;
        dy = table[n].y0 - table[n].y1;
    } else {
        ped->dir = 1;
        ped->obj->pos.vx = table[n].x0 << 3;
        ped->obj->pos.vy = table[n].y0 << 3;
        ped->obj->pos.vz = 0;
        dx = table[n].x1 - table[n].x0;
        dy = table[n].y1 - table[n].y0;
    }
    dx <<= 3;
    dy <<= 3;
    if (dx < 0) {
        ped->obj->rot.vz = 0xC00;
    }
    if (dx > 0) {
        ped->obj->rot.vz = 0x400;
    }
    if (dy < 0) {
        ped->obj->rot.vz = 0x800;
    }
    if (shellGetCurrentLevel() == 5) {
        ped->obj->pos.vz = 0x3CA;
    }
    RotMatrixYXZ(&ped->obj->rot, &ped->obj->mat);
    ped->obj->drawMode = 1;
    ped->unk0C = 0;
    ped->node->state = 1;
    ped->unk10 = 0;
    ped->unk24 = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", choose_ped_path);
#endif

#ifdef NON_MATCHING
void init_pedestrian(s32 id, DbNode* ep)
{
    DbNode* node;
    DbSwitch* n;

    if (ped_made < 6) {
        pedestrian[ped_made].obj = csCreate();
        if (pedestrian[ped_made].obj != NULL) {
            node = ep->v1.child[0];
            if (node->h.op == 9) {
                n = &node->sw;
            } else {
                n = &node->v4.child[0]->sw;
            }
            if (n->kind == 9) {
                pedestrian[ped_made].id = id;
                csSetEpNode(pedestrian[ped_made].obj, ep);
                csAddToCsList(pedestrian[ped_made].obj);
                pedestrian[ped_made].obj->unkC0 = ped_made + 150;
                pedestrian[ped_made].node = n;
                pedestrian[ped_made].unk10 = 0;
                choose_ped_path(&pedestrian[ped_made]);
                switch (id) {
                case 0x2F4:
                case 0x30A:
                    pedestrian[ped_made].unk26 = 3;
                    pedestrian[ped_made].unk28 = 4;
                    pedestrian[ped_made].unk2A = 5;
                    break;
                case 0x2EF:
                    pedestrian[ped_made].unk26 = 1;
                    pedestrian[ped_made].unk28 = 5;
                    pedestrian[ped_made].unk2A = 7;
                    break;
                case 0x2F8:
                    pedestrian[ped_made].unk26 = 5;
                    pedestrian[ped_made].unk28 = 5;
                    pedestrian[ped_made].unk2A = 5;
                    break;
                case 0x305:
                    pedestrian[ped_made].unk26 = 5;
                    pedestrian[ped_made].unk28 = 5;
                    pedestrian[ped_made].unk2A = 6;
                    break;
                case 0x30E:
                default:
                    pedestrian[ped_made].unk26 = 2;
                    pedestrian[ped_made].unk28 = 4;
                    pedestrian[ped_made].unk2A = 6;
                    break;
                }
                ped_made++;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", init_pedestrian);
#endif

#ifdef NON_MATCHING
void move_pedestrians(void)
{
    VECTOR pos;
    VECTOR v;
    VECTOR aim;
    s32 i;
    s32 p;
    s32 close;
    s32 ty;

    for (i = 0; i < ped_made; i++) {
        if (pedestrian[i].unk24 == 2) {
            v.vx = 0;
            v.vy = 24;
            v.vz = 0;
            mathMulTransVec(&pedestrian[i].obj->mat, &v, &pos);
            pedestrian[i].obj->pos.vx += pos.vx;
            pedestrian[i].obj->pos.vy += pos.vy;
            pedestrian[i].obj->pos.vz += 8;
            if (pedestrian[i].unk0C++ > gTargetUnk1036 * 2) {
                if (pedestrian[i].unk2A <= pedestrian[i].unk10) {
                    pedestrian[i].unk24 = 3;
                    pedestrian[i].unk20 = gTargetUnk1040;
                    pedestrian[i].obj->drawMode = 0;
                }
                if (pedestrian[i].unk2A > pedestrian[i].unk10) {
                    pedestrian[i].unk10++;
                }
                pedestrian[i].unk0C = 0;
                if (pedestrian[i].unk10 < pedestrian[i].node->numStates) {
                    pedestrian[i].node->state = pedestrian[i].unk10;
                }
            }
        }
        if (pedestrian[i].unk24 == 3) {
            pedestrian[i].unk20--;
            if (pedestrian[i].unk20 <= 0) {
                choose_ped_path(&pedestrian[i]);
            }
        }
        pedestrian[i].unk0C++;
        if (pedestrian[i].unk0C >= gTargetUnk1036) {
            p = 0;
            if (pedestrian[i].unk24 == 0) {
                for (; p < GetNumPlayers(); p++) {
                    GetPlayerPosition(p, (VEC3*)&pos);
                    if ((TGT_MAX(TGT_ABS(pos.vx - pedestrian[i].obj->pos.vx),
                             TGT_MAX(TGT_ABS(pos.vy - pedestrian[i].obj->pos.vy),
                                 TGT_ABS(pos.vz - pedestrian[i].obj->pos.vz)))
                            >> 3)
                        < 60) {
                        if (pedestrian[i].id != 0x2F8) {
                            pedestrian[i].unk24 = 1;
                            if (pedestrian[i].unk26 < pedestrian[i].unk28) {
                                pedestrian[i].unk10 = pedestrian[i].unk26 + 1;
                            }
                            pedestrian[i].unk0C = 0;
                            if (pedestrian[i].unk10 < pedestrian[i].node->numStates) {
                                pedestrian[i].node->state = pedestrian[i].unk10;
                            }
                        }
                    }
                }
            }
            if (pedestrian[i].unk24 == 0) {
                if (pedestrian[i].dir == 1) {
                    pos.vx = pedestrian[i].path[pedestrian[i].pathIdx].x1 * 8;
                    ty = pedestrian[i].path[pedestrian[i].pathIdx].y1 * 8;
                } else {
                    pos.vx = pedestrian[i].path[pedestrian[i].pathIdx].x0 * 8;
                    ty = pedestrian[i].path[pedestrian[i].pathIdx].y0 * 8;
                }
                pos.vy = ty;
                pos.vz = 0;
                pedestrian[i].unk0C = 0;
                pedestrian[i].unk10++;
                v.vx = pos.vx - pedestrian[i].obj->pos.vx;
                v.vy = pos.vy - pedestrian[i].obj->pos.vy;
                v.vz = 0;
                v.vx = v.vx * 8;
                v.vy = v.vy * 8;
                if (pedestrian[i].unk26 < pedestrian[i].unk10) {
                    pedestrian[i].unk10 = 0;
                }
                if (pedestrian[i].unk10 < pedestrian[i].node->numStates) {
                    pedestrian[i].node->state = pedestrian[i].unk10;
                }
                if (pedestrian[i].obj->pos.vx == pos.vx && pedestrian[i].obj->pos.vy == pos.vy) {
                    pedestrian[i].dir = -pedestrian[i].dir;
                    pedestrian[i].obj->rot.vz = (pedestrian[i].obj->rot.vz + 0x800) & 0xFFF;
                    RotMatrixYXZ(&pedestrian[i].obj->rot, &pedestrian[i].obj->mat);
                }
                if (TGT_ABS(pedestrian[i].obj->pos.vx - pos.vx) < 6) {
                    pedestrian[i].obj->pos.vx = pos.vx;
                } else if (pedestrian[i].obj->pos.vx < pos.vx) {
                    pedestrian[i].obj->pos.vx += 6;
                } else {
                    pedestrian[i].obj->pos.vx -= 6;
                }
                if (TGT_ABS(pedestrian[i].obj->pos.vy - pos.vy) < 6) {
                    pedestrian[i].obj->pos.vy = pos.vy;
                } else if (pedestrian[i].obj->pos.vy < pos.vy) {
                    pedestrian[i].obj->pos.vy += 6;
                } else {
                    pedestrian[i].obj->pos.vy -= 6;
                }
            } else if (pedestrian[i].unk24 == 1) {
                close = 0;
                for (p = 0; p < GetNumPlayers(); p++) {
                    GetPlayerPosition(p, (VEC3*)&pos);
                    if ((TGT_MAX(TGT_ABS(pos.vx - pedestrian[i].obj->pos.vx),
                             TGT_MAX(TGT_ABS(pos.vy - pedestrian[i].obj->pos.vy),
                                 TGT_ABS(pos.vz - pedestrian[i].obj->pos.vz)))
                            >> 3)
                        < 120) {
                        close = 1;
                    }
                }
                if (close != 0) {
                    pedestrian[i].unk0C = 1;
                    if (pedestrian[i].unk10 < pedestrian[i].unk28) {
                        pedestrian[i].unk10++;
                    }
                    if (pedestrian[i].unk10 < pedestrian[i].node->numStates) {
                        pedestrian[i].node->state = pedestrian[i].unk10;
                    }
                    if ((pedestrian[i].id == 0x2F4 || pedestrian[i].id == 0x2EF)
                        && !(rand() & 0xF)) {
                        pedestrian[i].obj->pos.vz += 30;
                        aim.vx = pos.vx - pedestrian[i].obj->pos.vx;
                        aim.vy = pos.vy - pedestrian[i].obj->pos.vy;
                        aim.vz = pos.vz - pedestrian[i].obj->pos.vz;
                        create_bullet(
                            -pedestrian[i].id, (VEC3*)&aim, (VEC3*)&pedestrian[i].obj->pos, 1);
                        do_mflash((VEC3*)&pedestrian[i].obj->pos);
                        pedestrian[i].obj->pos.vz -= 30;
                    }
                } else {
                    pedestrian[i].unk24 = 0;
                    pedestrian[i].unk10 = 0;
                    pedestrian[i].unk0C = 0;
                    pedestrian[i].node->state = 0;
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", move_pedestrians);
#endif

#ifdef NON_MATCHING
void pedestrian_takehit(s32 id, s32 damage)
{
    s32 i;
    s32 k;

    i = -1;
    if (id == 0x2F8) {
        return;
    }
    for (k = 0; k < ped_made; k++) {
        if (pedestrian[k].id == id) {
            i = k;
        }
    }
    if (i < 0) {
        return;
    }
    uasoundPlayMaleScream();
    pedestrian[i].unk24 = 2;
    if (pedestrian[i].unk2A > pedestrian[i].unk28) {
        pedestrian[i].unk10 = pedestrian[i].unk28 + 1;
    }
    pedestrian[i].unk0C = 0;
    if (pedestrian[i].unk10 < pedestrian[i].node->numStates) {
        pedestrian[i].node->state = pedestrian[i].unk10;
    }
    pedestrian[i].obj->pos.vz += 4;
    GetPlayerRot(GetClosestPlayer((VEC3*)&pedestrian[i].obj->pos, -1), &pedestrian[i].obj->rot);
    pedestrian[i].obj->rot.vx = 0;
    pedestrian[i].obj->rot.vy = 0;
    RotMatrixYXZ(&pedestrian[i].obj->rot, &pedestrian[i].obj->mat);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", pedestrian_takehit);
#endif

void ped_takehit(s32 num, s32 damage)
{
    pedestrian_takehit(pedestrian[num].id, damage);
}

#ifdef NON_MATCHING
s32 check_ped_hits(VEC3* pos, s32 damage)
{
    s32 k;

    for (k = 0; k < ped_made; k++) {
        if (pedestrian[k].id != 0x2F8) {
            if ((TGT_MAX(TGT_ABS(pos->x - pedestrian[k].obj->pos.vx),
                     TGT_MAX(TGT_ABS(pos->y - pedestrian[k].obj->pos.vy),
                         TGT_ABS(pos->z - pedestrian[k].obj->pos.vz)))
                    >> 3)
                < 7) {
                pedestrian_takehit(pedestrian[k].id, damage);
                return 1;
            }
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", check_ped_hits);
#endif

void init_static_cops(void)
{
    s32 i;

    for (i = 0; i < 25; i++) {
        scop[i].state = 0;
    }
    drop_box_state = 0;
    window_state = 0;
}

#ifdef NON_MATCHING
void setup_static_cop(DbSwitch* node, VEC3* pos)
{
    s32 i;
    VEC3* d;

    if (node->kind != 9) {
        printf("Error! Static Cop Not At SwNode!\n");
        return;
    }
    i = node->sub - 1;
    if (i < 0 || i >= 25) {
        printf("Error! Static Cop Exceeds Number Space\n");
        return;
    }
    scop[i].hits = 0;
    scop[i].node = node;
    scop[i].state = node->id;
    d = &scop[i].pos;
    d->x = pos->x;
    d->y = pos->y;
    d->z = pos->z;
    scop[i].kind = 6;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", setup_static_cop);
#endif

#ifdef NON_MATCHING
void static_cop_fire(s32 num, VEC3* tgt)
{
    VEC3 aim;
    s16 r;

    scop[num].hits = 3;
    scop[num].kind = 4;
    if (scop[num].state == 754 || scop[num].state == 755) {
        scop[num].pos.z += 52;
        aim.x = 0;
        aim.y = 0;
        aim.z = ratan2(tgt->x - scop[num].pos.x + rand() % gTargetUnk1048 - (gTargetUnk1048 >> 1),
                    tgt->y - scop[num].pos.y)
            + rand() % gTargetUnk1048 - (gTargetUnk1048 >> 1);
        r = create_LOS_missile(-scop[num].state, &aim, &scop[num].pos, 2);
        if (r >= 0) {
            setContrailColor(r, 10, 50, 10);
        }
        do_flash(&scop[num].pos);
        do_puff(&scop[num].pos);
        scop[num].pos.z -= 52;
        scop[num].kind = (rand() & 7) + 7;
    } else {
        scop[num].pos.z += 40;
        aim.x = tgt->x - scop[num].pos.x - 64 + (rand() & 0x7F);
        aim.y = tgt->y - scop[num].pos.y - 64 + (rand() & 0x7F);
        aim.z = tgt->z - scop[num].pos.z;
        create_bullet(-scop[num].state, &aim, &scop[num].pos, 1);
        do_flash(&scop[num].pos);
        scop[num].pos.z -= 40;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", static_cop_fire);
#endif

#ifdef NON_MATCHING
void move_static_cops(void)
{
    VECTOR pp;
    VEC3* sp;
    s32 i;

    GetPlayerPosition(0, (VEC3*)&pp);
    for (i = gTargetUnk1068; i < 25; i += 6) {
        if (scop[i].state <= 0) {
            continue;
        }
        sp = &scop[i].pos;
        switch (scop[i].hits) {
        case 4:
            scop[i].hits = 5;
            break;
        case 5:
            scop[i].hits = 6;
            scop[i].kind = 900;
            break;
        case 6:
            scop[i].kind--;
            if (scop[i].kind <= 0) {
                scop[i].hits = 0;
            }
            break;
        case 0:
            if (TGT_DIST2(pp.vx - sp->x, pp.vy - sp->y) < 1200) {
                scop[i].hits = 1;
            }
            break;
        case 1:
            if (TGT_DIST2(pp.vx - sp->x, pp.vy - sp->y) < 1200) {
                scop[i].hits = 2;
                scop[i].kind = 2;
            } else {
                scop[i].hits = 0;
            }
            break;
        case 2:
            scop[i].kind--;
            if (scop[i].kind <= 0) {
                static_cop_fire(i, (VEC3*)&pp);
            }
            break;
        case 3:
            scop[i].kind--;
            if (scop[i].kind < 3) {
                if (TGT_DIST2(pp.vx - sp->x, pp.vy - sp->y) < 1200) {
                    scop[i].hits = 2;
                } else {
                    scop[i].hits = 1;
                }
            }
            break;
        }
        if (scop[i].hits < scop[i].node->numStates) {
            scop[i].node->state = scop[i].hits;
        }
    }
    gTargetUnk1068++;
    if (gTargetUnk1068 >= 6) {
        gTargetUnk1068 = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", move_static_cops);
#endif

#ifdef NON_MATCHING
void static_cop_takehit(s32 num)
{
    DbSwitch* node;

    if (num < 26) {
        uasoundPlayMaleScream();
        num--;
        if (scop[num].state > 0) {
            if (scop[num].hits < 4) {
                node = scop[num].node;
                scop[num].hits = 4;
                node->state = 4;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/targets", static_cop_takehit);
#endif

void move_drop_box(void)
{
    if (drop_box_state > 0) {
        if (drop_box_state < 11) {
            uaswSetState(0x3FA, 0, drop_box_state - 1);
            drop_box_state++;
            if (drop_box_state == 10) {
                uaswSetState(0x3FA, 0, 9);
                drop_box_state = 0;
            }
        }
    }
}

void move_breaking_window(void)
{
    if (window_state > 0) {
        if (window_state < 11) {
            uaswSetState(0x3FB, 0, window_state - 1);
            window_state++;
            if (window_state == 10) {
                uaswSetState(0x3FB, 0, 9);
                window_state = 0;
            }
        }
    }
}

void drop_box_takehit(void)
{
    uaswSetState(0x3F8, 0, 1);
    drop_box_state = 1;
}

void window_takehit(void)
{
    window_state = 1;
}

void move_targets(void)
{
    if (shellGetCurrentLevel() == 5) {
        move_drop_box();
        move_breaking_window();
    }
    if (!rtIsSplitScreenOn()) {
        move_mercs();
        move_pedestrians();
        move_static_cops();
    }
}
