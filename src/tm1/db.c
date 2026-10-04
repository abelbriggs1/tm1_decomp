#include "common.h"

#include <stdio.h>

#include "tm1/cs.h"
#include "tm1/explode.h"
#include "tm1/hud.h"
#include "tm1/interactives.h"
#include "tm1/math.h"
#include "tm1/targets.h"
#include "tm1/ua.h"
#include "tm1/ua_dash.h"
#include "tm1/ua_effect.h"
#include "tm1/ua_sw.h"
#include "tm1/view.h"
#include "tm1/weapon.h"

#include "tm1/db.h"

#define DMD_MAGIC 0x50535844
#define DMD_VERSION 0x43

extern void exit(s32 code);
extern char* shellGetCurrentDatabaseFileName(void);
extern void screenInitCarOccupants(void* node);

s32 db3DEnvironmentTrap = 0;
char dbOlder[] = "older";
char dbNewer[] = "newer";

DbNode* gHierNodes[250];
s32 location[250][3];

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
INCLUDE_ASM("asm/nonmatchings/tm1/db", dbInit);
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
INCLUDE_ASM("asm/nonmatchings/tm1/db", dbScanForInteractiveStuff);
#endif

#ifdef NON_MATCHING
void dbProcessInteractives(s32 type, DbNode* node, s32* loc, s32 parent, s32* parentLoc)
{
    switch (type) {
    case 3: {
        Cs* cs;

        cs = dbInitCsForAModel(node);
        cs->unk00 = 0xFFE;
        cs->unk0C = 0;
        break;
    }
    case 1:
        if (db3DEnvironmentTrap != 0) {
            printf("Warning: found another node flagged as the 3D environment\n");
            break;
        }
        csSetEpNode(csGetWorldCs(), node);
        db3DEnvironmentTrap = 1;
        break;
    case 2: {
        Cs* cs;

        cs = csCreate();
        csSetEpNode(cs, node);
        viewSetSky(cs);
        cs->unk0C = 0;
        break;
    }
    case 0xA:
        UAAddCs(dbInitCsForAModel(node), 0xA);
        break;
    case 0x14:
        UAAddCs(dbInitCsForAModel(node), 0x14);
        break;
    case 0x1E:
        UAAddCs(dbInitCsForAModel(node), 0x1E);
        break;
    case 0x28:
        UAAddCs(dbInitCsForAModel(node), 0x28);
        break;
    case 0x32:
        UAAddCs(dbInitCsForAModel(node), 0x32);
        break;
    case 0x3C:
        UAAddCs(dbInitCsForAModel(node), 0x3C);
        break;
    case 0x46:
        UAAddCs(dbInitCsForAModel(node), 0x46);
        break;
    case 0x50:
        UAAddCs(dbInitCsForAModel(node), 0x50);
        break;
    case 0x5A:
        UAAddCs(dbInitCsForAModel(node), 0x5A);
        break;
    case 0x64:
        UAAddCs(dbInitCsForAModel(node), 0x64);
        break;
    case 0x6E:
        UAAddCs(dbInitCsForAModel(node), 0x6E);
        break;
    case 0x78:
        UAAddCs(dbInitCsForAModel(node), 0x78);
        break;
    case 0x82:
        UAAddCs(dbInitCsForAModel(node), 0x82);
        break;
    case 0xC:
    case 0x16:
    case 0x20:
    case 0x34:
    case 0x3E:
    case 0x48:
    case 0x52:
    case 0x5C:
    case 0x66:
    case 0x70:
    case 0x7A:
    case 0x84:
        uaswInitCarTire(&node->sw);
        break;
    case 0xD:
    case 0x17:
    case 0x21:
    case 0x2B:
    case 0x35:
    case 0x3F:
    case 0x49:
    case 0x53:
    case 0x5D:
    case 0x67:
    case 0x71:
    case 0x7B:
    case 0x85:
        uaswInitCarShadow(&node->sw);
        break;
    case 0x50A:
    case 0x50B:
    case 0x50C:
    case 0x50D:
        hudStoreArrowIcon(type, &node->gr);
        break;
    case 0x258:
    case 0x259:
    case 0x262:
    case 0x263:
    case 0x264:
    case 0x265:
    case 0x302:
    case 0x386:
    case 0x3A2:
    case 0x3A3:
    case 0x3E8:
    case 0x3E9:
    case 0x3EA:
    case 0x3F1:
    case 0x3F2:
    case 0x3F9:
        uaswInitDbSwitch(&node->sw, type);
        init_target(type, node->sw.sub, (VEC3*)loc);
        break;
    case 0x2CB:
    case 0x2CC:
    case 0x2D0:
    case 0x2D1:
    case 0x2DA:
    case 0x3EB:
    case 0x3EC:
    case 0x3ED:
    case 0x3F0:
    case 0x3F7:
        uaswInitDbEditNumChildren(&node->sw, type);
        init_target(type, node->sw.shadowSub, (VEC3*)loc);
        break;
    case 0x2EE:
    case 0x2F0:
        if (node->h.op == 1) {
            init_merc(type, node);
        }
        break;
    case 0x2EF:
    case 0x2F4:
    case 0x2F8:
    case 0x304:
    case 0x305:
    case 0x307:
    case 0x308:
    case 0x309:
    case 0x30A:
    case 0x30D:
    case 0x30E:
        init_pedestrian(type, node);
        break;
    case 0x2F1:
    case 0x2F2:
    case 0x2F3:
        setup_static_cop(&node->sw, (VEC3*)loc);
        break;
    case 0x398:
        set_health_stand(node->sw.sub, loc);
        goto doSwitch;
    case 0xC9:
    case 0xCA:
    case 0xCB:
    case 0xCC:
    case 0xCD:
    case 0xCE:
    case 0x133:
    case 0x137:
    case 0x138:
        bulAddCs(node, type);
        break;
    case 0xD2:
    case 0xD4:
    case 0xD5:
    case 0xD6:
        carAddWeapTex(&node->gr, type);
        break;
    case 0x4D8:
        UAdashInitDashboard((u_long*)node);
        break;
    case 0x4EC:
        UAdashInitDashboardIcons(&node->dash);
        break;
    case 0x2A:
        UAeffectInitMonsterTruckWheel(&node->wheel);
        break;
    case 0x12D:
    case 0x12E:
    case 0x134:
    case 0x135:
        bulMakeSpecialBullet(type, &node->gr);
        break;
    case 0x190:
    case 0x191:
    case 0x192:
    case 0x193:
    case 0x194:
    case 0x195:
    case 0x196:
    case 0x19A:
    case 0x19B:
    case 0x19C:
    case 0x19D:
    case 0x19E:
    case 0x1A4:
    case 0x1A5:
    case 0x1A6:
    case 0x1A7:
    case 0x1AE:
    case 0x1AF:
    case 0x1C2:
        uaswInitDbEditNumChildren(&node->sw, type);
        setPickup(loc, type, node->sw.num);
        break;
    case 0x32A:
        UAeffectInitTV(&node->eff);
        break;
    case 0x28A:
        explodeStoreFlameAnimation(node);
        break;
    case 0x28E:
        explodeStoreExplosionAnimation(node);
        break;
    case 0x28B:
        explodeStoreSmokeAnimation(node);
        break;
    case 0x290:
        explodeStoreBurnAnimation(node);
        break;
    case 0x28D:
        explodeStoreSparkAnimation(node);
        break;
    case 0x28C:
        explodeStoreContrailAnimation(node);
        break;
    case 0x28F:
        explodeStoreGburstAnimation(node);
        break;
    case 0x29E:
    case 0x29F:
    case 0x2A0:
    case 0x2A1:
        explodeLoadFragTexture(type, &node->gr);
        break;
    case 0xE:
    case 0x18:
    case 0x22:
    case 0x2C:
    case 0x36:
    case 0x40:
    case 0x4A:
    case 0x54:
    case 0x5E:
    case 0x68:
    case 0x72:
    case 0x7C:
    case 0x86:
    case 0x2BC:
    case 0x3F8:
    case 0x3FA:
    case 0x3FB:
    doSwitch:
        uaswInitDbSwitch(&node->sw, type);
        break;
    case 0x399:
        UAeffectInitHealthStandLightning(&node->eff);
        break;
    case 0x500:
        uadashInitSteeringWheel(&node->gr);
        break;
    case 0x514:
        screenInitCarOccupants(node);
        break;
    case 0x4DF:
        uadashInitRearViewMirror();
        break;
    default:
        break;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/db", dbProcessInteractives);
#endif

void dbReset3DEnvironmentTrap(void)
{
    db3DEnvironmentTrap = 0;
}

#ifndef NON_MATCHING
INCLUDE_ASM("asm/nonmatchings/tm1/db", jtbl_800F83B0);

INCLUDE_ASM("asm/nonmatchings/tm1/db", jtbl_800F83E0);
#endif
