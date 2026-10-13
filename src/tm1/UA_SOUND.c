#include "common.h"

#include "tm1/LEVELS.H"
#include "tm1/SOUND.H"
#include "tm1/SOUND_ID_API.H"
#include "tm1/UA.H"

#include "tm1/UA_SOUND.H"

#define HOVER_REQUEST GENERAL_HOVER_COPS_InflightBuffer

static SoundRequest GENERAL_HOVER_COPS_InflightBuffer;

static s32 huntDADelay = 0;
static s32 battleDADelay = 0;

#define CAR_CASES                                                                                  \
    case UA_ICE_CREAM_TRUCK:                                                                       \
    case UA_TAXI_CAB:                                                                              \
    case UA_SEMI_TRUCK:                                                                            \
    case UA_MONSTER_TRUCK:                                                                         \
    case UA_POLICE_CAR:                                                                            \
    case UA_LAMBORGHINI:                                                                           \
    case UA_HUMVEE:                                                                                \
    case UA_HARLEY:                                                                                \
    case UA_DUNE_BUGGY:                                                                            \
    case UA_LOW_RIDER:                                                                             \
    case UA_CORVETTE:                                                                              \
    case UA_MAD_MAX:                                                                               \
    case UA_BOSS_CAR:

void uasoundFireCarMachineGuns(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case 0:
        return;
    case UA_HARLEY:
        id = 0x61;
        break;
    case UA_CORVETTE:
        id = 0x64;
        break;
    case UA_TAXI_CAB:
        id = 0x5b;
        break;
    case UA_DUNE_BUGGY:
        id = 0x62;
        break;
    case UA_HUMVEE:
        id = 0x60;
        break;
    case UA_ICE_CREAM_TRUCK:
        id = 0x5a;
        break;
    case UA_LOW_RIDER:
        id = 0x63;
        break;
    case UA_LAMBORGHINI:
        id = 0x5f;
        break;
    case UA_MAD_MAX:
        id = 0x65;
        break;
    case UA_MONSTER_TRUCK:
        id = 0x5d;
        break;
    case UA_POLICE_CAR:
        id = 0x5e;
        break;
    case UA_SEMI_TRUCK:
        id = 0x5c;
        break;
    case UA_BOSS_CAR:
        id = 0x66;
        break;
    case UA_HOVER_COP:
        id = 0x84;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 0, 0, 0);
}

void uasoundStopCarMachineGuns(s32 w)
{
    s32 id;

    switch (w) {
    case 0:
        return;
    case UA_HARLEY:
        id = 0x61;
        break;
    case UA_CORVETTE:
        id = 0x64;
        break;
    case UA_TAXI_CAB:
        id = 0x5b;
        break;
    case UA_DUNE_BUGGY:
        id = 0x62;
        break;
    case UA_HUMVEE:
        id = 0x60;
        break;
    case UA_ICE_CREAM_TRUCK:
        id = 0x5a;
        break;
    case UA_LOW_RIDER:
        id = 0x63;
        break;
    case UA_LAMBORGHINI:
        id = 0x5f;
        break;
    case UA_MAD_MAX:
        id = 0x65;
        break;
    case UA_MONSTER_TRUCK:
        id = 0x5d;
        break;
    case UA_POLICE_CAR:
        id = 0x5e;
        break;
    case UA_SEMI_TRUCK:
        id = 0x5c;
        break;
    case UA_BOSS_CAR:
        id = 0x66;
        break;
    case UA_HOVER_COP:
        id = 0x84;
        break;
    default:
        return;
    }
    soundStopPlayId(id, 0, 0, 0);
}

void uasoundExplodeMachineGunBullet(s32 a, s32 b, s32 c)
{
    soundPlayId(4, a, b, 8, c, 0);
}

void uasoundPlayCarEngineRev(s32 w, s32 a, s32 b, s32 v)
{
    s32 id;

    switch (w) {
    case UA_NONE:
        return;
    case UA_HARLEY:
        id = 0x54;
        break;
    case UA_CORVETTE:
        id = 0x57;
        break;
    case UA_TAXI_CAB:
        id = 0x4e;
        break;
    case UA_DUNE_BUGGY:
        id = 0x55;
        break;
    case UA_HUMVEE:
        id = 0x53;
        break;
    case UA_ICE_CREAM_TRUCK:
        id = 0x4d;
        break;
    case UA_LOW_RIDER:
        id = 0x56;
        break;
    case UA_LAMBORGHINI:
        id = 0x52;
        break;
    case UA_MAD_MAX:
        id = 0x58;
        break;
    case UA_MONSTER_TRUCK:
        id = 0x50;
        break;
    case UA_POLICE_CAR:
        id = 0x51;
        break;
    case UA_SEMI_TRUCK:
        id = 0x4f;
        break;
    case UA_BOSS_CAR:
        id = 0x59;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 1, v, 0);
}

void uasoundPlayCarCrash(s32 w, s32 a, s32 b, s32 v)
{
    s32 id;

    if (v >= 51) {
        v = v + 20;
    }
    switch (w) {
    case UA_NONE:
        break;
        CAR_CASES
        id = 0x6a;
        if (v < 34) {
            v = v * 3;
        } else {
            if (v < 67) {
                id = 0x6b;
                v = (v * 50) / 66 + 49;
            } else {
                id = 0x6c;
                v = (v * 50) / 99 + 49;
            }
        }
        soundPlayId(id, a, b, 8, v, 0);
        break;
    }
}

void uasoundPlayCarExplode(s32 w, s32 a, s32 b, s32 v)
{
    s32 id;

    switch (w) {
    case UA_NONE:
        break;
        CAR_CASES
        id = 0x71;
        if (v < 34) {
            v = v * 3;
        } else {
            if (v < 67) {
                id = 0x72;
                v = (v * 50) / 66 + 49;
            } else {
                id = 0x73;
                a = 0;
                v = (v * 50) / 99 + 49;
            }
        }
        soundPlayId(id, a, b, 8, v, 0);
        break;
    }
}

void uasoundPlayCarSkid(s32 w, s32 a, s32 b, s32 v)
{
    s32 id;

    switch (w) {
    case UA_NONE:
        break;
        CAR_CASES
        id = 0x6d;
        if (v < 34) {
            v = v * 3;
        } else {
            if (v < 67) {
                id = 0x6e;
                v = (v * 50) / 66 + 49;
            } else {
                id = 0x6f;
                v = (v * 50) / 99 + 49;
            }
        }
        soundPlayId(id, a, b, 8, v, 0);
        break;
    }
}

void uasoundStopCarSounds(s32 w)
{
    s32 skid;
    s32 rev;
    s32 crash;
    s32 gun;

    switch (w) {
    case UA_NONE:
        return;
    case UA_HARLEY:
        skid = 0;
        rev = 0x54;
        crash = 2;
        gun = 0x61;
        break;
    case UA_CORVETTE:
        skid = 0;
        rev = 0x57;
        crash = 2;
        gun = 0x64;
        break;
    case UA_TAXI_CAB:
        skid = 0;
        rev = 0x4e;
        crash = 2;
        gun = 0x5b;
        break;
    case UA_DUNE_BUGGY:
        skid = 0;
        rev = 0x55;
        crash = 2;
        gun = 0x62;
        break;
    case UA_HUMVEE:
        skid = 0;
        rev = 0x53;
        crash = 2;
        gun = 0x60;
        break;
    case UA_ICE_CREAM_TRUCK:
        skid = 0;
        rev = 0x4d;
        crash = 2;
        gun = 0x5a;
        break;
    case UA_LOW_RIDER:
        skid = 0;
        rev = 0x56;
        crash = 2;
        gun = 0x63;
        break;
    case UA_LAMBORGHINI:
        skid = 0;
        rev = 0x52;
        crash = 2;
        gun = 0x5f;
        break;
    case UA_MAD_MAX:
        skid = 0;
        rev = 0x58;
        crash = 2;
        gun = 0x65;
        break;
    case UA_MONSTER_TRUCK:
        skid = 0;
        rev = 0x50;
        crash = 2;
        gun = 0x5d;
        break;
    case UA_POLICE_CAR:
        skid = 0;
        rev = 0x51;
        crash = 2;
        gun = 0x5e;
        break;
    case UA_SEMI_TRUCK:
        skid = 0;
        rev = 0x4f;
        crash = 2;
        gun = 0x5c;
        break;
    case UA_BOSS_CAR:
        skid = 0;
        rev = 0x59;
        crash = 2;
        gun = 0x66;
        break;
    default:
        return;
    }
    soundStopPlayId(skid, 8, 0, 0);
    soundStopPlayId(rev, 1, 0, 0);
    soundStopPlayId(crash, 7, 0, 0);
    soundStopPlayId(gun, 0, 0, 0);
}

void uasoundPlayCarWeaponPickup(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case UA_PICKUP_BOMB:
        id = 0x5;
        break;
    case UA_PICKUP_OIL:
        id = 0x8;
        break;
    case UA_PICKUP_SPIKE:
        id = 0xb;
        break;
    case UA_PICKUP_FLAMETHROWER:
        id = 0xe;
        break;
    case UA_PICKUP_CATAPULT:
        id = 0x11;
        break;
    case UA_PICKUP_FIRE_MISSILE:
        id = 0x14;
        break;
    case UA_PICKUP_REAR_MISSILE:
        id = 0x17;
        break;
    case UA_PICKUP_FREEZE_MISSILE:
        id = 0x1a;
        break;
    case UA_PICKUP_POWER_MISSILE:
        id = 0x1d;
        break;
    case UA_PICKUP_SINGING_MISSILE:
        id = 0x20;
        break;
    case UA_PICKUP_HOMING_MISSILE:
        id = 0x23;
        break;
    case UA_UNK_430:
        id = 0x81;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 7, 0, 0);
}

void uasoundPlayCarWeaponLaunchOrInflight(s32 w, s32 a, s32 b)
{
    s32 id;
    s32 chan;
    s32 extra;

    chan = 7;
    extra = -1;
    switch (w) {
    case WEAPON_BOMB:
        id = 0x6;
        break;
    case WEAPON_OIL:
        id = 0x9;
        break;
    case WEAPON_SPIKE:
        id = 0xc;
        break;
    case WEAPON_FLAMETHROWER:
        id = 0xf;
        chan = 4;
        extra = 0x3c0;
        break;
    case WEAPON_CATAPULT:
        id = 0x12;
        break;
    case WEAPON_FIRE_MISSILE:
        id = 0x15;
        break;
    case WEAPON_REAR_MISSILE:
        id = 0x18;
        break;
    case WEAPON_FREEZE_MISSILE:
        id = 0x1b;
        break;
    case WEAPON_POWER_MISSILE:
        id = 0x1e;
        break;
    case WEAPON_SINGING_MISSILE:
        id = 0x21;
        break;
    case WEAPON_HOMING_MISSILE:
        id = 0x24;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, chan, extra, 0);
}

void uasoundStopCarWeapon(s32 w)
{
    s32 id;
    s32 chan;

    chan = 7;
    switch (w) {
    case WEAPON_BOMB:
        id = 6;
        break;
    case WEAPON_OIL:
        id = 9;
        break;
    case WEAPON_SPIKE:
        id = 12;
        break;
    case WEAPON_FLAMETHROWER:
        id = 15;
        chan = 4;
        break;
    case WEAPON_CATAPULT:
        id = 18;
        break;
    case WEAPON_FIRE_MISSILE:
        id = 21;
        break;
    case WEAPON_REAR_MISSILE:
        id = 24;
        break;
    case WEAPON_FREEZE_MISSILE:
        id = 27;
        break;
    case WEAPON_POWER_MISSILE:
        id = 30;
        break;
    case WEAPON_SINGING_MISSILE:
        id = 33;
        break;
    case WEAPON_HOMING_MISSILE:
        id = 36;
        break;
    default:
        return;
    }
    soundStopPlayId(id, chan, 0, 0);
}

void uasoundPlayCarWeaponExplode(s32 w, s32 a, s32 b, s32 v)
{
    s32 stopId;
    s32 playId;
    s32 chan;

    chan = 7;
    switch (w) {
    case WEAPON_BOMB:
        stopId = 0x6;
        playId = 0x7;
        break;
    case WEAPON_OIL:
        stopId = 0x9;
        playId = 0xa;
        break;
    case WEAPON_SPIKE:
        stopId = 0xc;
        playId = 0xd;
        break;
    case WEAPON_FLAMETHROWER:
        stopId = 0xf;
        playId = 0x10;
        chan = 4;
        break;
    case WEAPON_CATAPULT:
        stopId = 0x12;
        playId = 0x13;
        break;
    case WEAPON_FIRE_MISSILE:
        stopId = 0x15;
        playId = 0x19;
        break;
    case WEAPON_REAR_MISSILE:
        stopId = 0x18;
        playId = 0x19;
        break;
    case WEAPON_FREEZE_MISSILE:
        stopId = 0x1b;
        playId = 0x1c;
        break;
    case WEAPON_POWER_MISSILE:
        stopId = 0x1e;
        playId = 0x1f;
        break;
    case WEAPON_SINGING_MISSILE:
        stopId = 0x21;
        playId = 0x22;
        break;
    case WEAPON_HOMING_MISSILE:
        stopId = 0x24;
        playId = 0x25;
        break;
    default:
        return;
    }
    soundStopPlayId(stopId, chan, 0, 0);
    soundPlayId(playId, a, b, 8, v, 0);
}

void uasoundPlayCarSpecialWeaponPickup(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case SOUND_SPECIAL_DUNE_BUGGY:
        id = 0x26;
        break;
    case SOUND_SPECIAL_ICE_CREAM_TRUCK:
        id = 0x29;
        break;
    case SOUND_SPECIAL_MAD_MAX:
        id = 0x2c;
        break;
    case SOUND_SPECIAL_CORVETTE:
        id = 0x2f;
        break;
    case SOUND_SPECIAL_HUMVEE:
        id = 0x32;
        break;
    case SOUND_SPECIAL_MONSTER_TRUCK:
        id = 0x35;
        break;
    case SOUND_SPECIAL_LAMBORGHINI:
        id = 0x38;
        break;
    case SOUND_SPECIAL_TAXI_CAB:
        id = 0x3b;
        break;
    case SOUND_SPECIAL_POLICE_CAR:
        id = 0x3e;
        break;
    case SOUND_SPECIAL_SEMI_TRUCK:
        id = 0x41;
        break;
    case SOUND_SPECIAL_LOW_RIDER:
        id = 0x44;
        break;
    case SOUND_SPECIAL_HARLEY:
        id = 0x47;
        break;
    case SOUND_SPECIAL_BOSS_CAR:
        id = 0x4a;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 7, 0, 0);
}

void uasoundPlayCarSpecialWeaponLaunchOrInflight(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case SOUND_SPECIAL_DUNE_BUGGY:
        id = 0x27;
        break;
    case SOUND_SPECIAL_ICE_CREAM_TRUCK:
        id = 0x2a;
        break;
    case SOUND_SPECIAL_MAD_MAX:
        id = 0x2d;
        break;
    case SOUND_SPECIAL_CORVETTE:
        id = 0x30;
        break;
    case SOUND_SPECIAL_HUMVEE:
        id = 0x33;
        break;
    case SOUND_SPECIAL_MONSTER_TRUCK:
        id = 0x36;
        break;
    case SOUND_SPECIAL_LAMBORGHINI:
        id = 0x39;
        break;
    case SOUND_SPECIAL_TAXI_CAB:
        id = 0x3c;
        break;
    case SOUND_SPECIAL_POLICE_CAR:
        id = 0x3f;
        break;
    case SOUND_SPECIAL_SEMI_TRUCK:
        id = 0x42;
        break;
    case SOUND_SPECIAL_LOW_RIDER:
        id = 0x45;
        break;
    case SOUND_SPECIAL_HARLEY:
        id = 0x48;
        break;
    case SOUND_SPECIAL_BOSS_CAR:
        id = 0x4b;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 7, 0, 0);
}

void uasoundStopCarSpecialWeapon(s32 w)
{
    s32 id;

    switch (w) {
    case SOUND_SPECIAL_DUNE_BUGGY:
        id = 0x27;
        break;
    case SOUND_SPECIAL_ICE_CREAM_TRUCK:
        id = 0x2a;
        break;
    case SOUND_SPECIAL_MAD_MAX:
        id = 0x2d;
        break;
    case SOUND_SPECIAL_CORVETTE:
        id = 0x30;
        break;
    case SOUND_SPECIAL_HUMVEE:
        id = 0x33;
        break;
    case SOUND_SPECIAL_MONSTER_TRUCK:
        id = 0x36;
        break;
    case SOUND_SPECIAL_LAMBORGHINI:
        id = 0x39;
        break;
    case SOUND_SPECIAL_TAXI_CAB:
        id = 0x3c;
        break;
    case SOUND_SPECIAL_POLICE_CAR:
        id = 0x3f;
        break;
    case SOUND_SPECIAL_SEMI_TRUCK:
        id = 0x42;
        break;
    case SOUND_SPECIAL_LOW_RIDER:
        id = 0x45;
        break;
    case SOUND_SPECIAL_HARLEY:
        id = 0x48;
        break;
    case SOUND_SPECIAL_BOSS_CAR:
        id = 0x4b;
        break;
    default:
        return;
    }
    soundStopPlayId(id, 7, 0, 0);
}

void uasoundPlayCarSpecialWeaponExplode(s32 w, s32 a, s32 b, s32 v)
{
    s32 stopId;
    s32 playId;

    switch (w) {
    case SOUND_SPECIAL_DUNE_BUGGY:
        stopId = 0x27;
        playId = 0x28;
        break;
    case SOUND_SPECIAL_ICE_CREAM_TRUCK:
        stopId = 0x2a;
        playId = 0x2b;
        break;
    case SOUND_SPECIAL_MAD_MAX:
        stopId = 0x2d;
        playId = 0x2e;
        break;
    case SOUND_SPECIAL_CORVETTE:
        stopId = 0x30;
        playId = 0x31;
        break;
    case SOUND_SPECIAL_HUMVEE:
        stopId = 0x33;
        playId = 0x34;
        break;
    case SOUND_SPECIAL_MONSTER_TRUCK:
        stopId = 0x36;
        playId = 0x37;
        break;
    case SOUND_SPECIAL_LAMBORGHINI:
        stopId = 0x39;
        playId = 0x3a;
        break;
    case SOUND_SPECIAL_TAXI_CAB:
        stopId = 0x3c;
        playId = 0x3d;
        break;
    case SOUND_SPECIAL_POLICE_CAR:
        stopId = 0x3f;
        playId = 0x40;
        break;
    case SOUND_SPECIAL_SEMI_TRUCK:
        stopId = 0x42;
        playId = 0x43;
        break;
    case SOUND_SPECIAL_LOW_RIDER:
        stopId = 0x45;
        playId = 0x46;
        break;
    case SOUND_SPECIAL_HARLEY:
        stopId = 0x48;
        playId = 0x49;
        break;
    case SOUND_SPECIAL_BOSS_CAR:
        stopId = 0x4b;
        playId = 0x4c;
        break;
    default:
        return;
    }
    soundStopPlayId(stopId, 0, 0, 0);
    soundPlayId(playId, a, b, 8, v, 0);
}

void uasoundPlayCarSignature(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case UA_NONE:
        return;
    case UA_HARLEY:
        id = 0x7b;
        break;
    case UA_CORVETTE:
        id = 0x7e;
        break;
    case UA_TAXI_CAB:
        id = 0x75;
        break;
    case UA_DUNE_BUGGY:
        id = 0x7c;
        break;
    case UA_HUMVEE:
        id = 0x7a;
        break;
    case UA_ICE_CREAM_TRUCK:
        id = 0x74;
        break;
    case UA_LOW_RIDER:
        id = 0x7d;
        break;
    case UA_LAMBORGHINI:
        id = 0x79;
        break;
    case UA_MAD_MAX:
        id = 0x7f;
        break;
    case UA_MONSTER_TRUCK:
        id = 0x77;
        break;
    case UA_POLICE_CAR:
        id = 0x78;
        break;
    case UA_SEMI_TRUCK:
        id = 0x76;
        break;
    case UA_BOSS_CAR:
        id = 0x80;
        break;
    default:
        return;
    }
    soundPlayId(id, a, b, 7, 0, 0);
}

void uasoundStopCarSignature(s32 w)
{
    s32 id;

    switch (w) {
    case UA_NONE:
        return;
    case UA_HARLEY:
        id = 0x7b;
        break;
    case UA_CORVETTE:
        id = 0x7e;
        break;
    case UA_TAXI_CAB:
        id = 0x75;
        break;
    case UA_DUNE_BUGGY:
        id = 0x7c;
        break;
    case UA_HUMVEE:
        id = 0x7a;
        break;
    case UA_ICE_CREAM_TRUCK:
        id = 0x74;
        break;
    case UA_LOW_RIDER:
        id = 0x7d;
        break;
    case UA_LAMBORGHINI:
        id = 0x79;
        break;
    case UA_MAD_MAX:
        id = 0x7f;
        break;
    case UA_MONSTER_TRUCK:
        id = 0x77;
        break;
    case UA_POLICE_CAR:
        id = 0x78;
        break;
    case UA_SEMI_TRUCK:
        id = 0x76;
        break;
    case UA_BOSS_CAR:
        id = 0x80;
        break;
    default:
        return;
    }
    soundStopPlayId(id, 7, 0, 0);
}

void uasoundPlayTurboBoost(s32 a, s32 b)
{
    soundPlayId(0x82, a, b, 0, 0, 0);
}

void uasoundStopTurboBoost(void)
{
    soundStopPlayId(0x82, 0, 0, 0);
}

void uasoundPlayHoverCops(s32 a, s32 b)
{
    addSoundRequest(0x83, a, b, 0, 0, 0, &HOVER_REQUEST);
}

void uasoundStopHoverCops(void)
{
    soundStopPlayId(0x83, 0, 0, 0);
}

void uasoundPlayRecharge(void)
{
    soundPlayId(0x70, 0, 0, 7, 0, 0);
}

void uasoundPlayBeep(void)
{
    soundPlayId(0x85, 0, 0, 7, 0, 0);
}

void uasoundPlayMenuCycleFlush(void)
{
    soundPlayId(0x86, 0, 0, 5, 0, 0);
    soundProcessIds();
}

void uasoundPlayMenuCycle(void)
{
    soundPlayId(0x86, 0, 0, 5, 0, 0);
}

void uasoundPlayMenuSelectFlush(void)
{
    soundPlayId(0x87, 0, 0, 5, 0, 0);
    soundProcessIds();
}

void uasoundPlayMenuSelect(void)
{
    soundPlayId(0x87, 0, 0, 5, 0, 0);
}

void uasoundPlayCardThrowFlush(void)
{
    soundPlayId(0x88, 0, 0, 5, 0, 0);
    soundProcessIds();
}

void uasoundPlayMaleScream(void)
{
    soundPlayId(0x89, 0, 0, 7, 0, 0);
}

void uasoundPlayGeneralExplode(s32 a, s32 b, s32 v)
{
    s32 id;

    if (v < 34) {
        id = 0x67;
        v = v * 3;
    } else {
        if (v < 67) {
            id = 0x68;
            v = (v * 50) / 66 + 49;
        } else {
            id = 0x69;
            a = 0;
            v = (v * 50) / 99 + 49;
        }
    }
    soundPlayId(id, a, b, 8, v, 0);
}

void uasoundPlayScreenEngineRev(s32 a)
{
    soundPlayId(0x52, 0, 0, 1, a, 0);
}

void uasoundStopScreenEngineRev(void)
{
    soundStopPlayId(0x52, 1, 0, 0);
}

void uasoundPlayHuntDA(s32 mode)
{
    s32 track;

    switch (mode) {
    case LEVEL_NUM(LEVEL2_WAREHOUSE):
        track = DA_TRACK_STALK_N_ROLL;
        break;
    case LEVEL_NUM(LEVEL4_CITY_PARK):
        track = DA_TRACK_STALK_N_ROLL;
        break;
    case LEVEL_NUM(LEVEL5_CYBURBIA):
        track = DA_TRACK_CYBURB_HUNT;
        break;
    default:
        return;
    }
    if (battleDADelay <= 0) {
        huntDADelay = huntDADelay - 1;
        if (huntDADelay <= 0) {
            huntDADelay = 30;
        }
        if (soundGetCurrentDATrack() != track) {
            soundStartPlayDA(track);
        }
    } else {
        battleDADelay = battleDADelay - 1;
    }
}

void uasoundPlayBattleDA(s32 mode)
{
    s32 track;

    track = DA_TRACK_CYBURB_SLIDE;
    switch (mode) {
    case LEVEL_NUM(LEVEL2_WAREHOUSE):
        break;
    case LEVEL_NUM(LEVEL4_CITY_PARK):
        track = DA_TRACK_ASPHALT_ASSAULT;
        break;
    case LEVEL_NUM(LEVEL5_CYBURBIA):
        break;
    default:
        return;
    }
    if (huntDADelay <= 0) {
        battleDADelay = battleDADelay - 1;
        if (battleDADelay <= 0) {
            battleDADelay = 30;
        }
        if (soundGetCurrentDATrack() != track) {
            soundStartPlayDA(track);
        }
    } else {
        huntDADelay = huntDADelay - 1;
    }
}

void uasoundSetMusicVolume(s32 vol)
{
    s32 t;

    if (vol >= 100) {
        vol = 99;
    }
    t = (vol >= 0) ? vol : 0;
    vol = t;
    if (vol == 99) {
        soundSetMusicVolume(127);
        soundSetSFXVolume(0);
    } else {
        if (vol == 0) {
            soundSetMusicVolume(0);
            soundSetSFXVolume(57);
        } else {
            soundSetMusicVolume((97 * vol) / 99 + 30);
            soundSetSFXVolume((47 * (99 - vol)) / 99 + 10);
        }
    }
}

void flushSoundRequestBuffer(SoundRequest* req)
{
    if (req->pending) {
        soundPlayId(req->id, req->dist, req->arg2, req->arg3, req->arg4, req->arg5);
        req->pending = 0;
        req->dist = SOUND_REQUEST_NO_DIST;
    }
}

void soundFlushRequestBuffers(void)
{
    flushSoundRequestBuffer(&HOVER_REQUEST);
}

void soundFlushRequestStopBuffers(void)
{
}

void initRequestBuffer(SoundRequest* req)
{
    req->dist = SOUND_REQUEST_NO_DIST;
    req->pending = 0;
    req->id = 0;
    req->arg2 = 0;
    req->arg3 = 0;
    req->arg4 = 0;
    req->arg5 = 0;
    req->unk1C = 0;
}

void soundInitRequestBuffers(void)
{
    initRequestBuffer(&HOVER_REQUEST);
}

void addSoundRequest(s32 id, s32 dist, s32 arg2, s32 arg3, s32 arg4, s32 arg5, SoundRequest* req)
{
    u8 better;

    better = req->pending ? ((u32)dist < (u32)req->dist) : 1;
    if (better) {
        req->id = id;
        req->dist = dist;
        req->arg2 = arg2;
        req->arg3 = arg3;
        req->arg4 = arg4;
        req->arg5 = arg5;
        req->pending = 1;
    }
}

void soundProcessSpecialSounds(void)
{
}

void soundShutDownSpecialSounds(void)
{
}
