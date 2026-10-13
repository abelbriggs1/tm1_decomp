#include "common.h"

#include "tm1/UA.H"
#include "tm1/UA_SW.H"

static DbSwitch* hoverCopSwitch;
static DbSwitch* bazCopSwitch;
static DbSwitch* FallingBoxSwitch;
static DbSwitch* DropBoxSwitch;
static DbSwitch* BreakingWindowSwitch;
static s32 numDestroySwitches;
static s32 numDestroyGroups;

void uaswInitDbSwitch(DbSwitch* node, s32 type)
{
    s32 idx;

    switch (type) {
    case UA_UNK_600:
    case UA_UNK_601:
    case UA_UNK_610:
    case UA_UNK_611:
    case UA_UNK_612:
    case UA_UNK_613:
    case UA_UNK_770:
    case UA_UNK_902:
    case UA_UNK_1000:
    case UA_UNK_1001:
    case UA_UNK_1002:
    case UA_UNK_1009:
    case UA_UNK_1010:
    case UA_UNK_1017:
        if (node->kind == 9) {
            idx = numDestroySwitches;
            if (idx < 30) {
                destroySwitch[idx] = node;
                node->sub = idx;
                numDestroySwitches = idx + 1;
            }
        }
        break;
    case UA_BREAKING_WINDOW:
        if (node->kind == 9) {
            BreakingWindowSwitch = node;
        }
        break;
    case UA_BAZ_COP:
        if (node->kind == 9) {
            bazCopSwitch = node;
        }
        break;
    case UA_HOVER_COP:
        if (node->kind == 9) {
            hoverCopSwitch = node;
        }
        break;
    case UA_HEALTH_STAND:
        if (node->kind == 9) {
            if (node->sub < 11) {
                healthstandSwitch[node->sub - 1] = node;
            }
        }
        break;
    case UA_BARRICADE:
        if (node->kind == 9) {
            barricadeSwitch[node->sub - 1] = node;
        }
        break;
    case UA_FALLING_BOX:
        FallingBoxSwitch = node;
        break;
    case UA_DROP_BOX:
        DropBoxSwitch = node;
        break;
    case UA_ICE_CREAM_TRUCK_HEADLIGHTS:
    case UA_TAXI_CAB_HEADLIGHTS:
    case UA_SEMI_TRUCK_HEADLIGHTS:
    case UA_MONSTER_TRUCK_HEADLIGHTS:
    case UA_POLICE_CAR_HEADLIGHTS:
    case UA_LAMBORGHINI_HEADLIGHTS:
    case UA_HUMVEE_HEADLIGHTS:
    case UA_HARLEY_HEADLIGHTS:
    case UA_DUNE_BUGGY_HEADLIGHTS:
    case UA_LOW_RIDER_HEADLIGHTS:
    case UA_CORVETTE_HEADLIGHTS:
    case UA_MAD_MAX_HEADLIGHTS:
    case UA_BOSS_CAR_HEADLIGHTS:
        if (node->kind == 1) {
            if (node->shadowSub < 4) {
                if (node->shadowSub != 0) {
                    carHeadlightSwitch[UA_CAR_SLOT(type)][node->shadowSub - 1] = node;
                }
            }
        }
        break;
    case UA_UNK_930:
    case UA_UNK_931:
    default:
        break;
    }
}

void uaswSetState(s32 id, s32 num, u32 state)
{
    switch (id) {
    case UA_UNK_600:
    case UA_UNK_601:
    case UA_UNK_610:
    case UA_UNK_611:
    case UA_UNK_612:
    case UA_UNK_613:
    case UA_UNK_770:
    case UA_UNK_902:
    case UA_UNK_1000:
    case UA_UNK_1001:
    case UA_UNK_1002:
    case UA_UNK_1009:
    case UA_UNK_1010:
    case UA_UNK_1017:
        uaswSetSwitch(destroySwitch[num], state);
        break;
    case UA_BREAKING_WINDOW:
        uaswSetSwitch(BreakingWindowSwitch, state);
        break;
    case UA_BAZ_COP:
        uaswSetSwitch(bazCopSwitch, state);
        break;
    case UA_HOVER_COP:
        uaswSetSwitch(hoverCopSwitch, state);
        break;
    case UA_HEALTH_STAND:
        uaswSetSwitch(healthstandSwitch[num - 1], state);
        break;
    case UA_BARRICADE:
        uaswSetSwitch(barricadeSwitch[num - 1], state);
        break;
    case UA_FALLING_BOX:
        uaswSetSwitch(FallingBoxSwitch, state);
        break;
    case UA_DROP_BOX:
        uaswSetSwitch(DropBoxSwitch, state);
        break;
    case UA_UNK_930:
    case UA_UNK_931:
    default:
        break;
    }
}

void uaswSetSwitch(DbSwitch* s, u32 state)
{
    if (s != 0) {
        if (state < s->numStates) {
            s->state = state;
        }
    }
}

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
    idx = UA_CAR_SLOT(id);
    if (idx < 16) {
        carSwitch[idx] = q;
        q->state = 0;
    }
}

void uaswSetCarState(s32 id, u32 state, s8 unused)
{
    s32 idx;

    idx = UA_CAR_SLOT(id);
    if (idx < 16) {
        if (carSwitch[idx] != 0) {
            if (state < carSwitch[idx]->numStates) {
                carSwitch[idx]->state = state;
            }
        }
    }
}

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
    idx = UA_CAR_SLOT(n->id);
    if (idx < 16) {
        sub = n->sub - 1;
        if (sub < 3) {
            carTireSwitch[idx][sub] = n;
        }
    }
}

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
    idx = UA_CAR_SLOT(n->id);
    if (idx < 16) {
        sub = n->shadowSub - 1;
        if (sub < 3) {
            carShadowSwitch[idx][sub] = n;
        }
    }
}

void uaswSetCarShadow(s32 id, s32 on)
{
    s32 idx;
    s32 j;
    DbSwitch* n;

    idx = UA_CAR_SLOT(id);
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

void uaswInitDbEditNumChildren(DbSwitch* node, s32 type)
{
    s32 idx;

    switch (type) {
    case UA_UNK_400:
    case UA_PICKUP_FIRE_MISSILE:
    case UA_PICKUP_FREEZE_MISSILE:
    case UA_PICKUP_HOMING_MISSILE:
    case UA_PICKUP_POWER_MISSILE:
    case UA_PICKUP_REAR_MISSILE:
    case UA_PICKUP_SINGING_MISSILE:
    case UA_PICKUP_CATAPULT:
    case UA_PICKUP_FLAMETHROWER:
    case UA_PICKUP_BOMB:
    case UA_PICKUP_OIL:
    case UA_PICKUP_SPIKE:
    case UA_UNK_420:
    case UA_UNK_421:
    case UA_UNK_422:
    case UA_UNK_423:
    case UA_UNK_430:
    case UA_UNK_431:
    case UA_PICKUP_SPECIAL:
        if (node->kind == 4) {
            idx = node->num - 1;
            if (idx < 50) {
                gWeaponPickup[idx] = node;
            }
        }
        break;
    case UA_UNK_715:
    case UA_UNK_716:
    case UA_UNK_720:
    case UA_UNK_721:
    case UA_UNK_730:
    case UA_UNK_1003:
    case UA_UNK_1004:
    case UA_UNK_1005:
    case UA_UNK_1008:
    case UA_UNK_1015:
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

void uaswSetNumChildren(s32 id, s32 num, s32 n)
{
    s32 idx;

    idx = num - 1;
    switch (id) {
    case UA_UNK_400:
    case UA_PICKUP_FIRE_MISSILE:
    case UA_PICKUP_FREEZE_MISSILE:
    case UA_PICKUP_HOMING_MISSILE:
    case UA_PICKUP_POWER_MISSILE:
    case UA_PICKUP_REAR_MISSILE:
    case UA_PICKUP_SINGING_MISSILE:
    case UA_PICKUP_CATAPULT:
    case UA_PICKUP_FLAMETHROWER:
    case UA_PICKUP_BOMB:
    case UA_PICKUP_OIL:
    case UA_PICKUP_SPIKE:
    case UA_UNK_420:
    case UA_UNK_421:
    case UA_UNK_422:
    case UA_UNK_423:
    case UA_UNK_430:
    case UA_UNK_431:
    case UA_PICKUP_SPECIAL:
        if (idx < 50) {
            uaswSetNumTransChildren(gWeaponPickup[idx], n);
        }
        break;
    case UA_UNK_715:
    case UA_UNK_716:
    case UA_UNK_720:
    case UA_UNK_721:
    case UA_UNK_730:
    case UA_UNK_1003:
    case UA_UNK_1004:
    case UA_UNK_1005:
    case UA_UNK_1008:
    case UA_UNK_1015:
        uaswSetNumGroupsChildren(destroyGroup[idx + 1], n);
        break;
    }
}

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

void uaswCarHeadlightsOnOff(u32 id, s32 mode)
{
    u32 idx;
    s32 j;

    idx = UA_CAR_SLOT(id);
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
