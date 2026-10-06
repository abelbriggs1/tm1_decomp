#include "common.h"

#include "tm1/ua_sw.h"

static DbSwitch* hoverCopSwitch;
static DbSwitch* bazCopSwitch;
static DbSwitch* FallingBoxSwitch;
static DbSwitch* DropBoxSwitch;
static DbSwitch* BreakingWindowSwitch;
static s32 numDestroySwitches;
static s32 numDestroyGroups;

#ifdef NON_MATCHING
void uaswInitDbSwitch(DbSwitch* node, s32 type)
{
    s32 idx;

    switch (type) {
    case 600:
    case 601:
    case 610:
    case 611:
    case 612:
    case 613:
    case 770:
    case 902:
    case 1000:
    case 1001:
    case 1002:
    case 1009:
    case 1010:
    case 1017:
        if (node->kind == 9) {
            idx = numDestroySwitches;
            if (idx < 30) {
                destroySwitch[idx] = node;
                node->sub = idx;
                numDestroySwitches = idx + 1;
            }
        }
        break;
    case 1019:
        if (node->kind == 9) {
            BreakingWindowSwitch = node;
        }
        break;
    case 750:
        if (node->kind == 9) {
            bazCopSwitch = node;
        }
        break;
    case 752:
        if (node->kind == 9) {
            hoverCopSwitch = node;
        }
        break;
    case 920:
        if (node->kind == 9) {
            if (node->sub < 11) {
                healthstandSwitch[node->sub - 1] = node;
            }
        }
        break;
    case 700:
        if (node->kind == 9) {
            barricadeSwitch[node->sub - 1] = node;
        }
        break;
    case 1018:
        FallingBoxSwitch = node;
        break;
    case 1016:
        DropBoxSwitch = node;
        break;
    case 14:
    case 24:
    case 34:
    case 44:
    case 54:
    case 64:
    case 74:
    case 84:
    case 94:
    case 104:
    case 114:
    case 124:
    case 134:
        if (node->kind == 1) {
            if (node->shadowSub < 4) {
                if (node->shadowSub != 0) {
                    carHeadlightSwitch[type / 10][node->shadowSub - 1] = node;
                }
            }
        }
        break;
    case 930:
    case 931:
    default:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInitDbSwitch);
#endif

#ifdef NON_MATCHING
void uaswSetState(s32 id, s32 num, u32 state)
{
    switch (id) {
    case 600:
    case 601:
    case 610:
    case 611:
    case 612:
    case 613:
    case 770:
    case 902:
    case 1000:
    case 1001:
    case 1002:
    case 1009:
    case 1010:
    case 1017:
        uaswSetSwitch(destroySwitch[num], state);
        break;
    case 1019:
        uaswSetSwitch(BreakingWindowSwitch, state);
        break;
    case 750:
        uaswSetSwitch(bazCopSwitch, state);
        break;
    case 752:
        uaswSetSwitch(hoverCopSwitch, state);
        break;
    case 920:
        uaswSetSwitch(healthstandSwitch[num - 1], state);
        break;
    case 700:
        uaswSetSwitch(barricadeSwitch[num - 1], state);
        break;
    case 1018:
        uaswSetSwitch(FallingBoxSwitch, state);
        break;
    case 1016:
        uaswSetSwitch(DropBoxSwitch, state);
        break;
    case 930:
    case 931:
    default:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswSetState);
#endif

void uaswSetSwitch(DbSwitch* s, u32 state)
{
    if (s != 0) {
        if (state < s->numStates) {
            s->state = state;
        }
    }
}

#ifdef NON_MATCHING
void uaswInitCarSwitch(Cs* cs, s32 id)
{
    DbSwitch* p;
    DbSwitch* q;
    s32 kind;
    s32 idx;

    p = cs->epNode;
    if (p == 0) {
        return;
    }
    kind = p->kind;
    if (kind != 1) {
        return;
    }
    p = p->child;
    if (p == 0) {
        return;
    }
    if (p->kind != kind) {
        return;
    }
    q = p->child;
    if (q == 0) {
        return;
    }
    if (q->kind != 9) {
        return;
    }
    idx = id / 10;
    if (idx < 16) {
        carSwitch[idx] = q;
        q->state = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInitCarSwitch);
#endif

#ifdef NON_MATCHING
void uaswSetCarState(s32 id, u32 state, s8 unused)
{
    s32 idx;

    idx = id / 10;
    if (idx < 16) {
        if (carSwitch[idx] != 0) {
            if (state < carSwitch[idx]->numStates) {
                carSwitch[idx]->state = state;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswSetCarState);
#endif

#ifdef NON_MATCHING
void uaswInitCarTire(DbSwitch* n)
{
    s32 idx;
    s32 sub;

    if (n == 0) {
        return;
    }
    if (n->kind != 9) {
        return;
    }
    idx = n->id / 10;
    if (idx < 16) {
        sub = n->sub - 1;
        if (sub < 3) {
            carTireSwitch[idx][sub] = n;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInitCarTire);
#endif

#ifdef NON_MATCHING
void uaswToggleCarTireState(s32 idx, s32 dir)
{
    s32 j;
    DbSwitch* n;

    if (idx < 16) {
        for (j = 0; j < 3; j++) {
            n = carTireSwitch[idx][j];
            if (n != 0) {
                if (dir < 0) {
                    n->state--;
                    if (n->state >= n->numStates) {
                        n->state = n->numStates - 1;
                    }
                } else {
                    n->state++;
                    if (n->state >= n->numStates) {
                        n->state = 0;
                    }
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswToggleCarTireState);
#endif

#ifdef NON_MATCHING
void uaswInitCarShadow(DbSwitch* n)
{
    s32 idx;
    s32 sub;

    if (n == 0) {
        return;
    }
    if (n->kind != 1) {
        return;
    }
    idx = n->id / 10;
    if (idx < 16) {
        sub = n->shadowSub - 1;
        if (sub < 3) {
            carShadowSwitch[idx][sub] = n;
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInitCarShadow);
#endif

#ifdef NON_MATCHING
void uaswSetCarShadow(s32 id, s32 on)
{
    s32 idx;
    s32 j;
    DbSwitch* n;

    idx = id / 10;
    if (idx < 16) {
        for (j = 0; j < 3; j++) {
            n = carShadowSwitch[idx][j];
            if (n != 0) {
                if (on) {
                    n->numGroupsChildren = 1;
                } else {
                    n->numGroupsChildren = 0;
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswSetCarShadow);
#endif

#ifdef NON_MATCHING
void uaswInitDbEditNumChildren(DbSwitch* node, s32 type)
{
    s32 idx;

    switch (type) {
    case 400:
    case 401:
    case 402:
    case 403:
    case 404:
    case 405:
    case 406:
    case 410:
    case 411:
    case 412:
    case 413:
    case 414:
    case 420:
    case 421:
    case 422:
    case 423:
    case 430:
    case 431:
    case 450:
        if (node->kind == 4) {
            idx = node->num - 1;
            if (idx < 50) {
                gWeaponPickup[idx] = node;
            }
        }
        break;
    case 715:
    case 716:
    case 720:
    case 721:
    case 730:
    case 1003:
    case 1004:
    case 1005:
    case 1008:
    case 1015:
        if (node->kind == 1) {
            idx = numDestroyGroups;
            if (idx < 30) {
                destroyGroup[idx] = node;
                node->shadowSub = idx;
                numDestroyGroups = idx + 1;
            }
        }
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInitDbEditNumChildren);
#endif

#ifdef NON_MATCHING
void uaswSetNumChildren(s32 id, s32 num, s32 n)
{
    s32 idx;

    idx = num - 1;
    switch (id) {
    case 400:
    case 401:
    case 402:
    case 403:
    case 404:
    case 405:
    case 406:
    case 410:
    case 411:
    case 412:
    case 413:
    case 414:
    case 420:
    case 421:
    case 422:
    case 423:
    case 430:
    case 431:
    case 450:
        if (idx < 50) {
            uaswSetNumTransChildren(gWeaponPickup[idx], n);
        }
        break;
    case 715:
    case 716:
    case 720:
    case 721:
    case 730:
    case 1003:
    case 1004:
    case 1005:
    case 1008:
    case 1015:
        uaswSetNumGroupsChildren(destroyGroup[idx + 1], n);
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswSetNumChildren);
#endif

void uaswSetNumGroupsChildren(DbSwitch* s, s32 n)
{
    if (s != 0) {
        s->numGroupsChildren = n;
    }
}

void uaswSetNumTransChildren(DbSwitch* s, s32 n)
{
    if (s != 0) {
        s->numTransChildren = n;
    }
}

#ifdef NON_MATCHING
void uaswInit(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 15; i++) {
        carSwitch[i] = 0;
        for (j = 0; j < 3; j++) {
            carTireSwitch[i][j] = 0;
            carHeadlightSwitch[i][j] = 0;
            carShadowSwitch[i][j] = 0;
        }
    }
    for (i = 0; i < 50; i++) {
        gWeaponPickup[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        destroySwitch[i] = 0;
        destroyGroup[i] = 0;
    }
    hoverCopSwitch = 0;
    bazCopSwitch = 0;
    for (i = 0; i < 4; i++) {
        barricadeSwitch[i] = 0;
    }
    for (i = 0; i < 10; i++) {
        healthstandSwitch[i] = 0;
    }
    FallingBoxSwitch = 0;
    DropBoxSwitch = 0;
    numDestroySwitches = 0;
    numDestroyGroups = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswInit);
#endif

#ifdef NON_MATCHING
void uaswCarHeadlightsOnOff(u32 id, s32 mode)
{
    u32 idx;
    s32 j;

    idx = id / 10;
    for (j = 0; j < 3; j++) {
        if (idx < 15) {
            if (carHeadlightSwitch[idx][j] != 0) {
                if (mode == 1) {
                    carHeadlightSwitch[idx][j]->numGroupsChildren = 2;
                } else {
                    carHeadlightSwitch[idx][j]->numGroupsChildren = 0;
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/ua_sw", uaswCarHeadlightsOnOff);
#endif
