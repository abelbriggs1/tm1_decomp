#include "common.h"

#include <STDIO.H>

#include "tm1/CS.H"
#include "tm1/EXPLODE.H"
#include "tm1/HUD.H"
#include "tm1/INTERACT.H"
#include "tm1/MATH.H"
#include "tm1/SCREEN.H"
#include "tm1/SHELL.H"
#include "tm1/TARGETS.H"
#include "tm1/UA.H"
#include "tm1/UAEFFECT.H"
#include "tm1/UA_DASH.H"
#include "tm1/UA_SW.H"
#include "tm1/VIEW.H"
#include "tm1/WEAPON.H"

#include "tm1/DB.H"

#define DMD_MAGIC 0x50535844
#define DMD_VERSION 0x43

extern void exit(s32 code);

s32 db3DEnvironmentTrap = 0;
char dbOlder[] = "older";
char dbNewer[] = "newer";

static DbNode* gHierNodes[250];
static s32 location[250][3];

#ifdef NON_MATCHING
void dbInit(u32 tmsVersion)
{
    s32* p;
    u32 count;
    u32 i;

    p = (s32*)0x800188B8;
    if (*p != DMD_MAGIC) {
        printf("Error: %s isn't a valid DMD file\n", shellGetCurrentDatabaseFileName());
        exit(-1);
    }
    p = (s32*)0x800188BC;
    if (*p != DMD_VERSION) {
        printf("Error: %s isn't the current version\n", shellGetCurrentDatabaseFileName());
        printf("       (the file is version %d, current version is %d)\n", *p, DMD_VERSION);
        exit(-1);
    }
    p = (s32*)0x800188C0;
    if (tmsVersion != 0 && tmsVersion != (u32)*p) {
        char* rel;
        char* name;

        name = shellGetCurrentDatabaseFileName();
        rel = dbNewer;
        if ((u32)*p < tmsVersion) {
            rel = dbOlder;
        }
        printf("Error: %s is %s than its corresponding TMS file\n", name, rel);
    }
    p += 3;
    count = *p;
    p++;
    for (i = 0; i < count; i++) {
        if (*p == 0) {
            printf("Internal error with the DMD file: first group was null\n");
            return;
        }
        dbScanForInteractiveStuff((DbNode*)*p);
        p++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/DB", dbInit);
#endif

Cs* dbInitCsForAModel(void* node)
{
    Cs* cs;

    cs = csCreate();
    csAddToCsList(cs);
    csSetEpNode(cs, node);
    return cs;
}

#ifdef NON_MATCHING
void dbScanForInteractiveStuff(DbNode* group)
{
    s32 parentLoc[3];
    s32 preRot[3];
    s32 parent;
    s32 sp;
    s32 kind;
    s32* loc;
    DbNode* node;

    parent = 0;
    sp = 1;
    location[0][0] = 0;
    location[0][1] = 0;
    location[0][2] = 0;
    gHierNodes[0] = group;
    do {
        sp = sp - 1;
        node = gHierNodes[sp];
        if (node != NULL) {
            loc = location[sp];
            switch (node->h.op) {
            case 0:
                kind = node->h.kind;
                break;
            case 1: {
                s32 i, n;
                kind = node->h.kind;
                n = node->v1.nChild;
                for (i = 0; i < n; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v1.child[i];
                    sp++;
                }
                break;
            }
            case 2: {
                s32 i, n;
                kind = node->h.kind;
                n = node->v2.nChild;
                for (i = 0; i < n; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v2.child[i]->f8;
                    sp++;
                }
                break;
            }
            case 3: {
                s32 i, n;
                kind = node->h.kind;
                if (node->v3.fA == 1) {
                    node->v3.fD = 1;
                }
                n = node->v3.nChild;
                for (i = 0; i < n; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v3.child[i];
                    sp++;
                }
                break;
            }
            case 4: {
                s32 i, n;
                loc[0] = loc[0] + node->v4.t[0];
                loc[1] = loc[1] + node->v4.t[1];
                loc[2] = loc[2] + node->v4.t[2];
                parentLoc[0] = loc[0];
                parentLoc[1] = loc[1];
                parentLoc[2] = loc[2];
                kind = node->h.kind;
                n = node->v4.nChild;
                for (i = 0; i < n; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v4.child[i];
                    sp++;
                }
                break;
            }
            case 5: {
                s32 i, n;
                kind = node->h.kind;
                parentLoc[0] = loc[0];
                parentLoc[1] = loc[1];
                parentLoc[2] = loc[2];
                loc[0] = loc[0] + node->v5.mat.t[0];
                loc[1] = loc[1] + node->v5.mat.t[1];
                loc[2] = loc[2] + node->v5.mat.t[2];
                preRot[0] = loc[0];
                preRot[1] = loc[1];
                preRot[2] = loc[2];
                mathMulVec(&node->v5.mat, (VECTOR*)preRot, (VECTOR*)loc);
                n = node->v5.nChild;
                for (i = 0; i < n; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v5.child[i];
                    sp++;
                }
                break;
            }
            case 6:
            case 7:
            case 8:
                kind = 0;
                break;
            case 9: {
                s32 i;
                kind = node->h.kind;
                for (i = 0; i < node->v9.nChild; i++) {
                    location[sp][0] = loc[0];
                    location[sp][1] = loc[1];
                    location[sp][2] = loc[2];
                    gHierNodes[sp] = node->v9.child[i];
                    sp++;
                }
                break;
            }
            case 11:
                location[sp][0] = loc[0];
                location[sp][1] = loc[1];
                location[sp][2] = loc[2];
                gHierNodes[sp] = node->v11.childA;
                sp++;
                location[sp][0] = loc[0];
                location[sp][1] = loc[1];
                location[sp][2] = loc[2];
                gHierNodes[sp] = node->v11.childB;
                sp++;
                kind = node->h.kind;
                break;
            case 10:
            default:
                kind = 0;
                printf("Bad hierarchy opcode %d \n", node->h.op);
                break;
            }
            dbProcessInteractives(kind, node, loc, parent, parentLoc);
        }
    } while (sp > 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/DB", dbScanForInteractiveStuff);
#endif

#ifdef NON_MATCHING
void dbProcessInteractives(s32 type, DbNode* node, s32* loc, s32 parent, s32* parentLoc)
{
    switch (type) {
    case UA_UNK_3: {
        Cs* cs;

        cs = dbInitCsForAModel(node);
        cs->unk00 = 0xFFE;
        cs->unk0C = 0;
        break;
    }
    case UA_3D_ENVIRONMENT:
        if (db3DEnvironmentTrap != 0) {
            printf("Warning: found another node flagged as the 3D environment\n");
            break;
        }
        csSetEpNode(csGetWorldCs(), node);
        db3DEnvironmentTrap = 1;
        break;
    case UA_SKY: {
        Cs* cs;

        cs = csCreate();
        csSetEpNode(cs, node);
        viewSetSky(cs);
        cs->unk0C = 0;
        break;
    }
    case UA_ICE_CREAM_TRUCK:
        UAAddCs(dbInitCsForAModel(node), 0xA);
        break;
    case UA_TAXI_CAB:
        UAAddCs(dbInitCsForAModel(node), 0x14);
        break;
    case UA_SEMI_TRUCK:
        UAAddCs(dbInitCsForAModel(node), 0x1E);
        break;
    case UA_MONSTER_TRUCK:
        UAAddCs(dbInitCsForAModel(node), 0x28);
        break;
    case UA_POLICE_CAR:
        UAAddCs(dbInitCsForAModel(node), 0x32);
        break;
    case UA_LAMBORGHINI:
        UAAddCs(dbInitCsForAModel(node), 0x3C);
        break;
    case UA_HUMVEE:
        UAAddCs(dbInitCsForAModel(node), 0x46);
        break;
    case UA_HARLEY:
        UAAddCs(dbInitCsForAModel(node), 0x50);
        break;
    case UA_DUNE_BUGGY:
        UAAddCs(dbInitCsForAModel(node), 0x5A);
        break;
    case UA_LOW_RIDER:
        UAAddCs(dbInitCsForAModel(node), 0x64);
        break;
    case UA_CORVETTE:
        UAAddCs(dbInitCsForAModel(node), 0x6E);
        break;
    case UA_MAD_MAX:
        UAAddCs(dbInitCsForAModel(node), 0x78);
        break;
    case UA_BOSS_CAR:
        UAAddCs(dbInitCsForAModel(node), 0x82);
        break;
    case UA_ICE_CREAM_TRUCK_TIRE:
    case UA_TAXI_CAB_TIRE:
    case UA_SEMI_TRUCK_TIRE:
    case UA_POLICE_CAR_TIRE:
    case UA_LAMBORGHINI_TIRE:
    case UA_HUMVEE_TIRE:
    case UA_HARLEY_TIRE:
    case UA_DUNE_BUGGY_TIRE:
    case UA_LOW_RIDER_TIRE:
    case UA_CORVETTE_TIRE:
    case UA_MAD_MAX_TIRE:
    case UA_BOSS_CAR_TIRE:
        uaswInitCarTire(&node->sw);
        break;
    case UA_ICE_CREAM_TRUCK_SHADOW:
    case UA_TAXI_CAB_SHADOW:
    case UA_SEMI_TRUCK_SHADOW:
    case UA_MONSTER_TRUCK_SHADOW:
    case UA_POLICE_CAR_SHADOW:
    case UA_LAMBORGHINI_SHADOW:
    case UA_HUMVEE_SHADOW:
    case UA_HARLEY_SHADOW:
    case UA_DUNE_BUGGY_SHADOW:
    case UA_LOW_RIDER_SHADOW:
    case UA_CORVETTE_SHADOW:
    case UA_MAD_MAX_SHADOW:
    case UA_BOSS_CAR_SHADOW:
        uaswInitCarShadow(&node->sw);
        break;
    case UA_ARROW_UP:
    case UA_ARROW_RIGHT:
    case UA_ARROW_DOWN:
    case UA_ARROW_LEFT:
        hudStoreArrowIcon(type, &node->gr);
        break;
    case UA_UNK_600:
    case UA_UNK_601:
    case UA_UNK_610:
    case UA_UNK_611:
    case UA_UNK_612:
    case UA_UNK_613:
    case UA_UNK_770:
    case UA_UNK_902:
    case UA_UNK_930:
    case UA_UNK_931:
    case UA_UNK_1000:
    case UA_UNK_1001:
    case UA_UNK_1002:
    case UA_UNK_1009:
    case UA_UNK_1010:
    case UA_UNK_1017:
        uaswInitDbSwitch(&node->sw, type);
        init_target(type, node->sw.sub, (VECTOR3*)loc);
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
        uaswInitDbEditNumChildren(&node->sw, type);
        init_target(type, node->sw.shadowSub, (VECTOR3*)loc);
        break;
    case UA_BAZ_COP:
    case UA_HOVER_COP:
        if (node->h.op == 1) {
            init_merc(type, node);
        }
        break;
    case UA_UNK_751:
    case UA_UNK_756:
    case UA_UNK_760:
    case UA_UNK_772:
    case UA_UNK_773:
    case UA_UNK_775:
    case UA_UNK_776:
    case UA_UNK_777:
    case UA_UNK_778:
    case UA_UNK_781:
    case UA_UNK_782:
        init_pedestrian(type, node);
        break;
    case UA_UNK_753:
    case UA_UNK_754:
    case UA_UNK_755:
        setup_static_cop(&node->sw, (VECTOR3*)loc);
        break;
    case UA_HEALTH_STAND:
        set_health_stand(node->sw.sub, loc);
        goto doSwitch;
    case UA_UNK_201:
    case UA_UNK_202:
    case UA_UNK_203:
    case UA_UNK_204:
    case UA_UNK_205:
    case UA_UNK_206:
    case UA_SWARM_MISSILE:
    case UA_GHOST_MISSILE:
    case UA_DEATHSPEAR:
        bulAddCs(node, type);
        break;
    case UA_CATAPULT:
    case UA_BOMB:
    case UA_OIL:
    case UA_SPIKE:
        carAddWeapTex(&node->gr, type);
        break;
    case UA_DASHBOARD:
        UAdashInitDashboard((u_long*)node);
        break;
    case UA_DASHBOARD_ICONS:
        UAdashInitDashboardIcons(&node->dash);
        break;
    case UA_MONSTER_TRUCK_WHEEL:
        UAeffectInitMonsterTruckWheel(&node->wheel);
        break;
    case UA_UNK_301:
    case UA_UNK_302:
    case UA_UNK_308:
    case UA_UNK_309:
        bulMakeSpecialBullet(type, &node->gr);
        break;
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
        uaswInitDbEditNumChildren(&node->sw, type);
        setPickup(loc, type, node->sw.num);
        break;
    case UA_TV:
        UAeffectInitTV(&node->eff);
        break;
    case UA_FLAME_ANIMATION:
        explodeStoreFlameAnimation(node);
        break;
    case UA_EXPLOSION_ANIMATION:
        explodeStoreExplosionAnimation(node);
        break;
    case UA_SMOKE_ANIMATION:
        explodeStoreSmokeAnimation(node);
        break;
    case UA_BURN_ANIMATION:
        explodeStoreBurnAnimation(node);
        break;
    case UA_SPARK_ANIMATION:
        explodeStoreSparkAnimation(node);
        break;
    case UA_CONTRAIL_ANIMATION:
        explodeStoreContrailAnimation(node);
        break;
    case UA_GBURST_ANIMATION:
        explodeStoreGburstAnimation(node);
        break;
    case UA_FRAGMENT_A:
    case UA_FRAGMENT_B:
    case UA_FRAGMENT_C:
    case UA_FRAGMENT_D:
        explodeLoadFragTexture(type, &node->gr);
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
    case UA_BARRICADE:
    case UA_DROP_BOX:
    case UA_FALLING_BOX:
    case UA_BREAKING_WINDOW:
    doSwitch:
        uaswInitDbSwitch(&node->sw, type);
        break;
    case UA_HEALTH_STAND_LIGHTNING:
        UAeffectInitHealthStandLightning(&node->eff);
        break;
    case UA_STEERING_WHEEL:
        uadashInitSteeringWheel(&node->gr);
        break;
    case UA_CAR_OCCUPANTS:
        screenInitCarOccupants(node);
        break;
    case UA_REAR_VIEW_MIRROR:
        uadashInitRearViewMirror();
        break;
    default:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/DB", dbProcessInteractives);
#endif

void dbReset3DEnvironmentTrap(void)
{
    db3DEnvironmentTrap = 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/tm1/DB", jtbl_800F83B0);

INCLUDE_ASM("asm/nonmatchings/tm1/DB", jtbl_800F83E0);
#endif
