#include "common.h"

#include "tm1/sound_id_api.h"

#include "tm1/ua_sound.h"

// This seems to be shared between `sound` and `ua_sound`.
SoundRequest GENERAL_HOVER_COPS_InflightBuffer;

static s32 huntDADelay = 0;
static s32 battleDADelay = 0;

#define CAR_CASES                                                                                  \
    case 10:                                                                                       \
    case 20:                                                                                       \
    case 30:                                                                                       \
    case 40:                                                                                       \
    case 50:                                                                                       \
    case 60:                                                                                       \
    case 70:                                                                                       \
    case 80:                                                                                       \
    case 90:                                                                                       \
    case 100:                                                                                      \
    case 110:                                                                                      \
    case 120:                                                                                      \
    case 130:

void uasoundFireCarMachineGuns(s32 w, s32 a, s32 b)
{
    s32 id;

    switch (w) {
    case 0:
        return;
    case 80:
        id = 0x61;
        break;
    case 110:
        id = 0x64;
        break;
    case 20:
        id = 0x5b;
        break;
    case 90:
        id = 0x62;
        break;
    case 70:
        id = 0x60;
        break;
    case 10:
        id = 0x5a;
        break;
    case 100:
        id = 0x63;
        break;
    case 60:
        id = 0x5f;
        break;
    case 120:
        id = 0x65;
        break;
    case 40:
        id = 0x5d;
        break;
    case 50:
        id = 0x5e;
        break;
    case 30:
        id = 0x5c;
        break;
    case 130:
        id = 0x66;
        break;
    case 752:
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
    case 80:
        id = 0x61;
        break;
    case 110:
        id = 0x64;
        break;
    case 20:
        id = 0x5b;
        break;
    case 90:
        id = 0x62;
        break;
    case 70:
        id = 0x60;
        break;
    case 10:
        id = 0x5a;
        break;
    case 100:
        id = 0x63;
        break;
    case 60:
        id = 0x5f;
        break;
    case 120:
        id = 0x65;
        break;
    case 40:
        id = 0x5d;
        break;
    case 50:
        id = 0x5e;
        break;
    case 30:
        id = 0x5c;
        break;
    case 130:
        id = 0x66;
        break;
    case 752:
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
    case -1:
        return;
    case 80:
        id = 0x54;
        break;
    case 110:
        id = 0x57;
        break;
    case 20:
        id = 0x4e;
        break;
    case 90:
        id = 0x55;
        break;
    case 70:
        id = 0x53;
        break;
    case 10:
        id = 0x4d;
        break;
    case 100:
        id = 0x56;
        break;
    case 60:
        id = 0x52;
        break;
    case 120:
        id = 0x58;
        break;
    case 40:
        id = 0x50;
        break;
    case 50:
        id = 0x51;
        break;
    case 30:
        id = 0x4f;
        break;
    case 130:
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
    case -1:
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
    case -1:
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
    case -1:
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
    case -1:
        return;
    case 80:
        skid = 0;
        rev = 0x54;
        crash = 2;
        gun = 0x61;
        break;
    case 110:
        skid = 0;
        rev = 0x57;
        crash = 2;
        gun = 0x64;
        break;
    case 20:
        skid = 0;
        rev = 0x4e;
        crash = 2;
        gun = 0x5b;
        break;
    case 90:
        skid = 0;
        rev = 0x55;
        crash = 2;
        gun = 0x62;
        break;
    case 70:
        skid = 0;
        rev = 0x53;
        crash = 2;
        gun = 0x60;
        break;
    case 10:
        skid = 0;
        rev = 0x4d;
        crash = 2;
        gun = 0x5a;
        break;
    case 100:
        skid = 0;
        rev = 0x56;
        crash = 2;
        gun = 0x63;
        break;
    case 60:
        skid = 0;
        rev = 0x52;
        crash = 2;
        gun = 0x5f;
        break;
    case 120:
        skid = 0;
        rev = 0x58;
        crash = 2;
        gun = 0x65;
        break;
    case 40:
        skid = 0;
        rev = 0x50;
        crash = 2;
        gun = 0x5d;
        break;
    case 50:
        skid = 0;
        rev = 0x51;
        crash = 2;
        gun = 0x5e;
        break;
    case 30:
        skid = 0;
        rev = 0x4f;
        crash = 2;
        gun = 0x5c;
        break;
    case 130:
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
    case 412:
        id = 0x5;
        break;
    case 413:
        id = 0x8;
        break;
    case 414:
        id = 0xb;
        break;
    case 411:
        id = 0xe;
        break;
    case 410:
        id = 0x11;
        break;
    case 401:
        id = 0x14;
        break;
    case 405:
        id = 0x17;
        break;
    case 402:
        id = 0x1a;
        break;
    case 404:
        id = 0x1d;
        break;
    case 406:
        id = 0x20;
        break;
    case 403:
        id = 0x23;
        break;
    case 430:
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
    case 8:
        id = 0x6;
        break;
    case 9:
        id = 0x9;
        break;
    case 10:
        id = 0xc;
        break;
    case 7:
        id = 0xf;
        chan = 4;
        extra = 0x3c0;
        break;
    case 6:
        id = 0x12;
        break;
    case 0:
        id = 0x15;
        break;
    case 5:
        id = 0x18;
        break;
    case 1:
        id = 0x1b;
        break;
    case 3:
        id = 0x1e;
        break;
    case 4:
        id = 0x21;
        break;
    case 2:
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
    case 8:
        id = 6;
        break;
    case 9:
        id = 9;
        break;
    case 10:
        id = 12;
        break;
    case 7:
        id = 15;
        chan = 4;
        break;
    case 6:
        id = 18;
        break;
    case 0:
        id = 21;
        break;
    case 5:
        id = 24;
        break;
    case 1:
        id = 27;
        break;
    case 3:
        id = 30;
        break;
    case 4:
        id = 33;
        break;
    case 2:
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
    case 8:
        stopId = 0x6;
        playId = 0x7;
        break;
    case 9:
        stopId = 0x9;
        playId = 0xa;
        break;
    case 10:
        stopId = 0xc;
        playId = 0xd;
        break;
    case 7:
        stopId = 0xf;
        playId = 0x10;
        chan = 4;
        break;
    case 6:
        stopId = 0x12;
        playId = 0x13;
        break;
    case 0:
        stopId = 0x15;
        playId = 0x19;
        break;
    case 5:
        stopId = 0x18;
        playId = 0x19;
        break;
    case 1:
        stopId = 0x1b;
        playId = 0x1c;
        break;
    case 3:
        stopId = 0x1e;
        playId = 0x1f;
        break;
    case 4:
        stopId = 0x21;
        playId = 0x22;
        break;
    case 2:
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
    case 0:
        id = 0x26;
        break;
    case 1:
        id = 0x29;
        break;
    case 2:
        id = 0x2c;
        break;
    case 3:
        id = 0x2f;
        break;
    case 4:
        id = 0x32;
        break;
    case 5:
        id = 0x35;
        break;
    case 6:
        id = 0x38;
        break;
    case 7:
        id = 0x3b;
        break;
    case 8:
        id = 0x3e;
        break;
    case 9:
        id = 0x41;
        break;
    case 10:
        id = 0x44;
        break;
    case 11:
        id = 0x47;
        break;
    case 12:
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
    case 0:
        id = 0x27;
        break;
    case 1:
        id = 0x2a;
        break;
    case 2:
        id = 0x2d;
        break;
    case 3:
        id = 0x30;
        break;
    case 4:
        id = 0x33;
        break;
    case 5:
        id = 0x36;
        break;
    case 6:
        id = 0x39;
        break;
    case 7:
        id = 0x3c;
        break;
    case 8:
        id = 0x3f;
        break;
    case 9:
        id = 0x42;
        break;
    case 10:
        id = 0x45;
        break;
    case 11:
        id = 0x48;
        break;
    case 12:
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
    case 0:
        id = 0x27;
        break;
    case 1:
        id = 0x2a;
        break;
    case 2:
        id = 0x2d;
        break;
    case 3:
        id = 0x30;
        break;
    case 4:
        id = 0x33;
        break;
    case 5:
        id = 0x36;
        break;
    case 6:
        id = 0x39;
        break;
    case 7:
        id = 0x3c;
        break;
    case 8:
        id = 0x3f;
        break;
    case 9:
        id = 0x42;
        break;
    case 10:
        id = 0x45;
        break;
    case 11:
        id = 0x48;
        break;
    case 12:
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
    case 0:
        stopId = 0x27;
        playId = 0x28;
        break;
    case 1:
        stopId = 0x2a;
        playId = 0x2b;
        break;
    case 2:
        stopId = 0x2d;
        playId = 0x2e;
        break;
    case 3:
        stopId = 0x30;
        playId = 0x31;
        break;
    case 4:
        stopId = 0x33;
        playId = 0x34;
        break;
    case 5:
        stopId = 0x36;
        playId = 0x37;
        break;
    case 6:
        stopId = 0x39;
        playId = 0x3a;
        break;
    case 7:
        stopId = 0x3c;
        playId = 0x3d;
        break;
    case 8:
        stopId = 0x3f;
        playId = 0x40;
        break;
    case 9:
        stopId = 0x42;
        playId = 0x43;
        break;
    case 10:
        stopId = 0x45;
        playId = 0x46;
        break;
    case 11:
        stopId = 0x48;
        playId = 0x49;
        break;
    case 12:
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
    case -1:
        return;
    case 80:
        id = 0x7b;
        break;
    case 110:
        id = 0x7e;
        break;
    case 20:
        id = 0x75;
        break;
    case 90:
        id = 0x7c;
        break;
    case 70:
        id = 0x7a;
        break;
    case 10:
        id = 0x74;
        break;
    case 100:
        id = 0x7d;
        break;
    case 60:
        id = 0x79;
        break;
    case 120:
        id = 0x7f;
        break;
    case 40:
        id = 0x77;
        break;
    case 50:
        id = 0x78;
        break;
    case 30:
        id = 0x76;
        break;
    case 130:
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
    case -1:
        return;
    case 80:
        id = 0x7b;
        break;
    case 110:
        id = 0x7e;
        break;
    case 20:
        id = 0x75;
        break;
    case 90:
        id = 0x7c;
        break;
    case 70:
        id = 0x7a;
        break;
    case 10:
        id = 0x74;
        break;
    case 100:
        id = 0x7d;
        break;
    case 60:
        id = 0x79;
        break;
    case 120:
        id = 0x7f;
        break;
    case 40:
        id = 0x77;
        break;
    case 50:
        id = 0x78;
        break;
    case 30:
        id = 0x76;
        break;
    case 130:
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
    case 2:
        track = 4;
        break;
    case 4:
        track = 4;
        break;
    case 5:
        track = 6;
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

    track = 7;
    switch (mode) {
    case 2:
        break;
    case 4:
        track = 5;
        break;
    case 5:
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
