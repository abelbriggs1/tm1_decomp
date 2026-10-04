#include "common.h"
#include <libcd.h>
#include <libetc.h>
#include <libspu.h>

#include "tm1/cs.h"

#include "tm1/sound.h"

// Found in the PSX scratchpad.
#define gVoiceList ((Tone**)0x1F800000)

extern SoundSystemAttributes systemAttributes;
// SoundSystemAttributes systemAttributes = {
//     0x7F, 0,
//     0x38,
//     0x7F,
//     0x38,
//     0,
//     0xD1D,
//     0xABF549,
//     0x6E,
//     0x8A7,
//     0x4ADCF1,
//     0, 0,
//     2,
// };

s32 gReverbOn = 0;
s32 gCurrentDATrack = 1;
s32 gDAState = 0;
s32 gPreviousMusicVolume = 0;
s32 gToneMinFree = 0;
s32 gToneMaxUsed = 0x22;
s32 gToneNumFree = 0x23;
s32 soundIdRequestIdx = -1;
Tone* tonesToPlayHead = 0;
Tone* tonesToPlayTail = 0;

static s32 gSoundRange;
static Cs* gSoundWorldCs;
static s32 StartPos;
static s32 EndPos;
static s32 CurPos;
static s32 RepTime;
static s32 cdInterruptLoc;

// TODO: Variable in `scommon`, doesn't get linked/positioned correctly.
// Many differences in codegen caused by this variable.
extern s16 soundVabIds[2];

extern s16 D_8016FEFC[];

// extern CdlLOC toc[100];
extern s32 gSoundEyeSpaceLoc[3];
extern s32 gSoundEyeToSoundLoc[3];
extern u8 soundVoiceTable[24];
extern SingleSound gSingleSounds[139];
extern Tone gTonePool[35];
extern SoundIdRequest gRequestQueue[45];
extern Tone* gFinishingTonesList[28];

extern u8 soundVabHeaders[0x40];
extern SpuVoiceAttr spuVoiceAttr;

void cbready(s32 intr, u8* result);
void cbvsync(void);
void cdplay(s32 mode);

s32 soundGetCurrentDATrack()
{
    return gCurrentDATrack;
}

s32 soundGetCalculatedSoundRange()
{
    return gSoundRange;
}

s32 soundGetCalculatedSoundXPosition()
{
    return gSoundEyeSpaceLoc[0];
}

#ifdef NON_MATCHING
void soundMulVec(SndMatrix* m, s32* v, s32* out)
{
    s32 t[3];

    t[0] = (v[0] < 3357) ? v[0] : 3357;
    t[1] = (v[1] < 3357) ? v[1] : 3357;
    t[2] = (v[2] < 3357) ? v[2] : 3357;
    out[0] = ((t[0] * (s16)m->m[0]) / 8192) + ((t[1] * (s16)m->m[1]) / 8192)
        + ((t[2] * (s16)m->m[2]) / 8192);
    out[1] = ((t[0] * (s16)m->m[3]) / 8192) + ((t[1] * (s16)m->m[4]) / 8192)
        + ((t[2] * (s16)m->m[5]) / 8192);
    out[2] = ((t[0] * (s16)m->m[6]) / 8192) + ((t[1] * (s16)m->m[7]) / 8192)
        + ((t[2] * (s16)m->m[8]) / 8192);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundMulVec);
#endif

void soundResetSoundEngine()
{
    soundForceAllSoundsOffNow();
}

#ifdef NON_MATCHING
void soundSetRangeAndXPositionFromWorldLoc(s32* loc)
{
    Cs* cs;
    s32 range;
    s32 r;

    cs = csGetWorldCs();
    gSoundEyeToSoundLoc[0] = loc[0] + cs->wpos.vx;
    gSoundEyeToSoundLoc[1] = loc[1] + cs->wpos.vy;
    gSoundWorldCs = cs;
    gSoundEyeToSoundLoc[2] = loc[2] + cs->wpos.vz;
    soundMulVec((SndMatrix*)&csGetWorldCs()->wmat, gSoundEyeToSoundLoc, gSoundEyeSpaceLoc);
    gSoundRange = gSoundEyeSpaceLoc[0] * gSoundEyeSpaceLoc[0];
    gSoundRange = gSoundRange + gSoundEyeSpaceLoc[1] * gSoundEyeSpaceLoc[1];
    gSoundRange = gSoundRange + gSoundEyeSpaceLoc[2] * gSoundEyeSpaceLoc[2];
    range = gSoundRange;
    if (range < 0) {
        range = 0;
        gSoundRange = range;
    }
    if (range < 0x800000) {
        r = range;
    } else {
        r = 0x7FFFFF;
    }
    gSoundRange = r;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundSetRangeAndXPositionFromWorldLoc);
#endif

s32 getOpenVoice(void)
{
    s32 i;
    s32 voice;

    voice = -1;
    i = 0;
    while (i < 24) {
        if (soundVoiceTable[i] != 0) {
            i++;
            continue;
        }
        if (SpuGetKeyStatus(1 << i) != 0) {
            i++;
            continue;
        }
        voice = i;
        soundVoiceTable[i] = 1;
        i++;
        break;
    }
    return voice;
}

void releaseUsedVoice(s32 voice)
{
    soundVoiceTable[voice] = 0;
}

#ifdef NON_MATCHING
void silenceUnusedVoices(void)
{
    s32 i;

    for (i = 0; i < 24; i++) {
        if (soundVoiceTable[i] == 0) {
            SsUtSetVVol(i, 0, 0);
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", silenceUnusedVoices);
#endif

#ifdef NON_MATCHING
void soundOff(Tone* t)
{
    s32 noise;

    if (t->state == 1 || t->state == 3) {
        noise = t->noise;
        if (noise == 1) {
            SpuSetNoiseVoice(0, noise << t->voice);
        }
        SsUtKeyOffV(t->voice);
        releaseUsedVoice(t->voice);
        t->state = 0;
        soundDecActiveVoiceCount();
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundOff);
#endif

#ifdef NON_MATCHING
void dopplerAdjustTonePitch(Tone* t, u8* noteOut, u8* fineOut)
{
    s32 pitch;

    pitch = (t->note << 7) + t->pitchDelta;
    *noteOut = pitch / 128;
    *fineOut = pitch - (*noteOut << 7);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", dopplerAdjustTonePitch);
#endif

#ifdef NON_MATCHING
void soundOn(Tone* t)
{
    s32 voice;
    u8 v;
    s32 note;

    if (t->state == 0 || t->state == 2) {
        voice = getOpenVoice();
        if (voice != -1) {
            spuVoiceAttr.voice = 1u << ((u32)voice & 31);
            spuVoiceAttr.mask = 3;
            spuVoiceAttr.volumex.left = 0;
            spuVoiceAttr.volumex.right = 0;
            SpuSetVoiceAttr(&spuVoiceAttr);
            v = 0x7F;
            if (t->note < 0x7F) {
                v = t->note;
            }
            t->note = v;
            v = 0x7F;
            if (t->fine < 0x7F) {
                v = t->fine;
            }
            t->fine = v;
            t->voice = SsUtKeyOnV(voice, t->vabId, t->prog, t->tone, t->note, t->fine, 0, 0);
            SsUtFlush();
            spuVoiceAttr.mask = 0x60;
            spuVoiceAttr.voice = 1u << (t->voice & 31);
            note = t->note << 8;
            spuVoiceAttr.note = note;
            spuVoiceAttr.sample_note = 0x3C00;
            spuVoiceAttr.note = t->fine | note;
            SpuSetVoiceAttr(&spuVoiceAttr);
            if (t->noise == 1) {
                SpuSetNoiseVoice(1, 1u << (t->voice & 31));
            }
            t->state = 1;
            soundIncActiveVoiceCount();
        }
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundOn);
#endif

void soundOffOn(Tone* t)
{
    if (t->state == 1) {
        soundOff(t);
    }
    soundOn(t);
}

#ifdef NON_MATCHING
void soundPlaySound(Tone* t)
{
    u8 note;
    u8 fine;
    SpuVoiceAttr* attr;
    u32 mode;
    u8 v;
    u8 w;
    u8 x;

    mode = t->mode;
    if (mode != 0) {
        if (mode < 3) {
            soundOn(t);
        }
    } else if (t->state == 1) {
        soundOffOn(t);
    } else {
        soundOn(t);
    }
    if (t->dirty != 0 && t->state == 1) {
        if (t->doppler != 0 && t->pitchDelta != 0) {
            dopplerAdjustTonePitch(t, &note, &fine);
        } else {
            note = t->note;
            fine = t->fine;
        }
        v = note;
        if (v >= 128) {
            v = 0x7F;
        }
        note = v;
        w = fine;
        if (w >= 128) {
            w = 0x7F;
        }
        fine = w;
        attr = &spuVoiceAttr;
        spuVoiceAttr.mask = 0x60;
        spuVoiceAttr.note = w | (v << 8);
        spuVoiceAttr.sample_note = 0x3C00;
        spuVoiceAttr.voice = 1u << (t->voice & 31);
        SpuSetVoiceAttr(attr);
        t->dirty = 0;
    }
    x = 0;
    if (t->volL != 0) {
        x = t->volL;
    }
    t->volL = x;
    x = 0x7F;
    if (t->volL < 0x7F) {
        x = t->volL;
    }
    t->volL = x;
    x = 0;
    if (t->volR != 0) {
        x = t->volR;
    }
    t->volR = x;
    x = 0x7F;
    if (t->volR < 0x7F) {
        x = t->volR;
    }
    t->volR = x;
    SsUtSetVVol(t->voice, t->volL, t->volR);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundPlaySound);
#endif

#ifdef NON_MATCHING
void soundPlayAllRequests(s32 count, Tone** list)
{
    s32 i;

    for (i = 0; i < count; i++) {
        soundPlaySound(list[i]);
    }
    silenceUnusedVoices();
    for (i = 0; i < count; i++) {
        list[i]->unk6E = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundPlayAllRequests);
#endif

#ifdef NON_MATCHING
void swapInstances(Tone** list, s32 a, s32 b)
{
    Tone* tmp;

    do {
        tmp = list[a];
    } while (0);
    list[a] = list[b];
    list[b] = tmp;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", swapInstances);
#endif

void soundRangeSortInstances(Tone** list, s32 lo, s32 hi)
{
    s32 i;
    s32 j;

    if (lo < hi) {
        swapInstances(list, lo, (lo + hi) / 2);
        i = lo;
        for (j = i + 1; j <= hi; j++) {
            if (list[j]->range <= list[lo]->range) {
                i++;
                swapInstances(list, i, j);
            }
        }
        swapInstances(list, lo, i);
        soundRangeSortInstances(list, lo, i - 1);
        soundRangeSortInstances(list, i + 1, hi);
    }
}

void soundPrioritySortInstances(Tone** list, s32 lo, s32 hi)
{
    s32 i;
    s32 j;

    if (lo < hi) {
        swapInstances(list, lo, (lo + hi) / 2);
        i = lo;
        for (j = i + 1; j <= hi; j++) {
            if (list[j]->priority <= list[lo]->priority) {
                i++;
                swapInstances(list, i, j);
            }
        }
        swapInstances(list, lo, i);
        soundPrioritySortInstances(list, lo, i - 1);
        soundPrioritySortInstances(list, i + 1, hi);
    }
}

void soundSortInstances(Tone** list, s32 lo, s32 hi)
{
    soundRangeSortInstances(list, lo, hi);
    soundPrioritySortInstances(list, lo, hi);
}

void soundLoadCoreVabIntoSPURam(u8* body)
{
    s32 tries;
    s16 id;

    tries = 20;
    id = SsVabOpen(body, soundVabHeaders);
    soundVabIds[0] = id;
    if (id == -1) {
        while (1) {
            if (tries <= 0) {
                break;
            }
            id = SsVabOpen(body, soundVabHeaders);
            soundVabIds[0] = id;
            tries--;
            if (id != -1) {
                break;
            }
        }
    }
    SsVabTransCompleted(1);
}

#ifdef NON_MATCHING
void soundLoadLevelVablIntoSPURam(u8* body)
{
    s32 tries;
    s16 id;

    tries = 20;
    id = SsVabOpen(body, &soundVabHeaders[0x20]);
    soundVabIds[1] = id;
    if (id == -1) {
        while (1) {
            if (tries <= 0) {
                break;
            }
            id = SsVabOpen(body, soundVabHeaders);
            soundVabIds[1] = id;
            tries--;
            if (id != -1) {
                break;
            }
        }
    }
    SsVabTransCompleted(1);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundLoadLevelVablIntoSPURam);
#endif

u8 vabIdIsValid(s32 vabId)
{
    return vabId != -1;
}

#ifdef NON_MATCHING
void soundUnloadLevelVabFromSPURam()
{
    if (vabIdIsValid(soundVabIds[1])) {
        SsVabClose(soundVabIds[1]);
        soundVabIds[1] = -1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundUnloadLevelVabFromSPURam);
#endif

void soundInitLevelPersistantSounds(s32 which)
{
    if (which == 0) {
        soundInitLevelSwapSounds(0);
    }
}

void soundCloseLevelPersistantSounds(s32 which)
{
    if (which == 0) {
        soundCloseLevelSwapSounds(0);
    }
}

void soundInitLevelSwapSounds(s32 which)
{
}

void soundCloseGamePersistantSounds()
{
    SsVabClose(soundVabIds[0]);
}

void soundCloseLevelSwapSounds(s32 which)
{
}

#ifdef NON_MATCHING
void soundInit(void)
{
    s16* p;
    s16 val;
    u32 i;

    SsInit();
    soundIdInit();
    SsSetReservedVoice(24);
    SpuSetReverb(0);
    SsSetTickMode(1);
    SsStart();
    SsSetMVol(systemAttributes.masterVolume, systemAttributes.masterVolume);
    SsSetStereo();
    soundInitRequestBuffers();
    i = 0;
    val = -1;
    p = soundVabIds;
    do {
        *p = val;
        i++;
        p++;
    } while (i < 2);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundInit);
#endif

void soundShutdown()
{
    soundStopPlayDA();
    soundCloseGamePersistantSounds();
    SsEnd();
    SsQuit();
}

void soundSetMasterVolume(s32 vol)
{
    s32 sfx;
    s32 music;
    s32 reverb;

    vol = (vol > 127) ? 127 : vol;
    if (vol < 0) {
        vol = 0;
    }
    sfx = systemAttributes.sfxVolume * 127 / systemAttributes.masterVolume;
    music = systemAttributes.musicVolume * 127 / systemAttributes.masterVolume;
    reverb = systemAttributes.reverbVolume * 127 / systemAttributes.masterVolume;
    systemAttributes.masterVolume = vol;
    systemAttributes.sfxVolume = systemAttributes.masterVolume * sfx / 127;
    systemAttributes.musicVolume = systemAttributes.masterVolume * music / 127;
    SsSetSerialVol(0, (s16)soundGetMusicVolume(), (s16)soundGetMusicVolume());
    systemAttributes.reverbVolume = systemAttributes.masterVolume * reverb / 127;
}

s32 soundGetMasterDopplerDelta()
{
    return systemAttributes.dopplerDelta;
}

s32 soundGetMasterDopplerEffectiveRange()
{
    return systemAttributes.dopplerEffectiveRange;
}

void soundSetMasterDopplerDelta(s32 delta)
{
    systemAttributes.dopplerDelta = delta;
}

void soundSetMasterDopplerEffectiveRange(s32 range)
{
    systemAttributes.dopplerEffectiveRange = range;
    systemAttributes.dopplerEffectiveRangeSquared = range * range;
}

s32 soundGetMasterDopplerEffectiveRangeSquared()
{
    return systemAttributes.dopplerEffectiveRangeSquared;
}

void soundSetReverbVolume(s32 vol)
{
    s32 v;

    if (systemAttributes.sfxVolume < vol) {
        vol = systemAttributes.sfxVolume;
    }
    if (vol < 0) {
        v = 0;
    } else {
        v = vol;
    }
    vol = v;
    systemAttributes.reverbVolume = vol;
}

s32 soundGetReverbVolume()
{
    return systemAttributes.reverbVolume;
}

void soundSetReverbType(s32 type)
{
    systemAttributes.reverbType = type;
}

s32 soundGetReverbType()
{
    return systemAttributes.reverbType;
}

void soundSetReverbOn(s32 type)
{
    if (type == 0) {
        soundSetReverbOff();
        return;
    }
    SsUtSetReverbType((s16)type);
    systemAttributes.reverbType = type;
    SsUtReverbOn();
    SsSetRVol((s16)systemAttributes.reverbVolume, (s16)systemAttributes.reverbVolume);
    gReverbOn = 1;
}

void soundSetReverbOff()
{
    SsUtReverbOff();
    gReverbOn = 0;
    systemAttributes.reverbType = 0;
}

s32 soundGetMasterVolume()
{
    return systemAttributes.masterVolume;
}

void soundSetSFXVolume(s32 vol)
{
    s32 v;

    if (systemAttributes.masterVolume < vol) {
        vol = systemAttributes.masterVolume;
    }
    if (vol < 0) {
        v = 0;
    } else {
        v = vol;
    }
    vol = v;
    systemAttributes.sfxVolume = vol;
}

s32 soundGetSFXVolume()
{
    return systemAttributes.sfxVolume;
}

void soundSetMusicVolume(s32 vol)
{
    s32 v;

    if (systemAttributes.masterVolume < vol) {
        vol = systemAttributes.masterVolume;
    }
    if (vol < 0) {
        v = 0;
    } else {
        v = vol;
    }
    vol = v;
    systemAttributes.musicVolume = vol;
    SsSetSerialVol(0, (s16)vol, (s16)vol);
}

s32 soundGetMusicVolume()
{
    return systemAttributes.musicVolume;
}

void soundSetVolumeRange(s32 range)
{
    if (range < 0 || range >= 3358) {
        range = 3357;
    }
    systemAttributes.volumeRange = range;
    systemAttributes.volumeRangeSquared = range * range;
}

s32 soundGetVolumeRangeSquared()
{
    return systemAttributes.volumeRangeSquared;
}

s32 soundGetVolumeRange()
{
    return systemAttributes.volumeRange;
}

void soundIncActiveVoiceCount()
{
    systemAttributes.activeVoiceCount++;
}

void soundDecActiveVoiceCount()
{
    systemAttributes.activeVoiceCount--;
}

void soundIncTotalVoiceCount()
{
    systemAttributes.totalVoiceCount++;
}

void soundDecTotalVoiceCount()
{
    systemAttributes.totalVoiceCount--;
}

s32 soundGetActiveVoiceCount()
{
    return systemAttributes.activeVoiceCount;
}

s32 soundGetTotalVoiceCount()
{
    return systemAttributes.totalVoiceCount;
}

void soundResetActiveVoiceCount()
{
    systemAttributes.activeVoiceCount = 0;
}

void soundResetTotalVoiceCount()
{
    systemAttributes.totalVoiceCount = 0;
}

s32 soundGetMusicVolumePreviousToInterrupt()
{
    return gPreviousMusicVolume;
}

void soundSetPreviousMusicVolume(s32 vol)
{
    gPreviousMusicVolume = vol;
}

// TODO: Requires BSS matching.
#ifdef NON_MATCHING
void soundStartPlayDA(s32 track)
{
    static CdlLOC toc[100];
    u8 changed;
    u32 st;
    s32 mv;
    s32 t;
    s32 v;
    s32 pos;
    s32 pm;
    s32 mv2;
    s32 t2;
    s32 v2;

    changed = 0;
    if (gDAState != 4) {
        pm = systemAttributes.musicVolume;
        mv2 = systemAttributes.masterVolume;
        t2 = mv2 & (mv2 >> 15);
        if (t2 < 0) {
            v2 = 0;
        } else {
            v2 = t2;
        }
        gPreviousMusicVolume = pm;
        t2 = v2;
        systemAttributes.musicVolume = t2;
        SsSetSerialVol(0, t2, t2);
    }
    if (gCurrentDATrack != track) {
        changed = 1;
    }
    st = gDAState;
    gCurrentDATrack = track;
    if (st < 2 || changed) {
        CdGetToc(toc);
        pos = CdPosToInt(&toc[track]) + 150;
        soundCDRepeat(pos, CdPosToInt(&toc[track + 1]) - 150);
        gDAState = 1;
        CdIntToPos(CurPos, (CdlLOC*)&cdInterruptLoc);
    } else if (st == 4) {
        pos = CdPosToInt(&toc[track]) + 150;
        CurPos = pos;
        StartPos = pos;
        EndPos = CdPosToInt(&toc[track + 1]);
        soundResumePlayDA();
    }
    t = gPreviousMusicVolume;
    if (systemAttributes.masterVolume < t) {
        t = systemAttributes.masterVolume;
    }
    if (t < 0) {
        v = 0;
    } else {
        v = t;
    }
    t = v;
    systemAttributes.musicVolume = t;
    SsSetSerialVol(0, t, t);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundStartPlayDA);
#endif

#ifdef NON_MATCHING
void soundInterruptPlayDA(void)
{
    s32 mv;
    s32 t;
    s32 v;
    s32 pm;

    if (gDAState == 1) {
        CdReadyCallback(0);
        pm = systemAttributes.musicVolume;
        mv = systemAttributes.masterVolume;
        t = mv & (mv >> 15);
        if (t < 0) {
            v = 0;
        } else {
            v = t;
        }
        gPreviousMusicVolume = pm;
        t = v;
        systemAttributes.musicVolume = t;
        SsSetSerialVol(0, t, t);
        CdControl(9, 0, 0);
        CdIntToPos(CurPos, (CdlLOC*)&cdInterruptLoc);
        gDAState = 4;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundInterruptPlayDA);
#endif

#ifdef NON_MATCHING
void soundResumePlayDA(void)
{
    s8 mode;
    s32 t;
    s32 v;

    if (gDAState == 4) {
        mode = 5;
        CdControl(0xE, (u8*)&mode, 0);
        CdReadyCallback(cbready);
        CdControl(3, (u8*)&cdInterruptLoc, 0);
        t = gPreviousMusicVolume;
        if (systemAttributes.masterVolume < t) {
            t = systemAttributes.masterVolume;
        }
        if (t < 0) {
            v = 0;
        } else {
            v = t;
        }
        t = v;
        systemAttributes.musicVolume = t;
        SsSetSerialVol(0, t, t);
        gDAState = 1;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundResumePlayDA);
#endif

void soundStopPlayDA(void)
{
    s32 mv;
    s32 t;
    s32 v;

    if (gDAState == 1 || gDAState == 4) {
        mv = systemAttributes.masterVolume;
        t = mv & (mv >> 15);
        if (t < 0) {
            v = 0;
        } else {
            v = t;
        }
        t = v;
        systemAttributes.musicVolume = t;
        SsSetSerialVol(0, t, t);
        CdControlB(8, 0, 0);
        gDAState = 0;
        gCurrentDATrack = 1;
    }
}

#ifdef NON_MATCHING
void soundCDRepeat(s32 start, s32 end)
{
    u8 mode;

    EndPos = end;
    StartPos = start;
    CurPos = start;
    RepTime = 0;
    mode = 5;
    CdControlB(0xE, (u8*)&mode, 0);
    CdReadyCallback(cbready);
    cdplay(3);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, systemAttributes.musicVolume, systemAttributes.musicVolume);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundCDRepeat);
#endif

#ifdef NON_MATCHING
void soundXARepeat(s32 start, s32 end)
{
    u8 mode;

    EndPos = end;
    StartPos = start;
    CurPos = start;
    RepTime = 0;
    mode = 0xC0;
    CdControlB(0xE, (u8*)&mode, 0);
    VSyncCallback(cbvsync);
    cdplay(6);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, systemAttributes.musicVolume, systemAttributes.musicVolume);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundXARepeat);
#endif

s32 cdGetPos()
{
    return CurPos;
}

s32 cdGetRepTime()
{
    return RepTime;
}

#ifdef NON_MATCHING
void cbready(s32 intr, u8* result)
{
    CdlLOC loc;
    s32 mv;
    s32 t;
    s32 v;

    if (intr == 1) {
        if (!(result[4] & 0x80)) {
            loc.minute = result[3];
            loc.sector = 0;
            loc.second = result[4];
            CurPos = CdPosToInt(&loc);
        }
        if (CurPos > EndPos || CurPos < StartPos) {
            mv = systemAttributes.masterVolume;
            t = mv & (mv >> 15);
            if (t < 0) {
                v = 0;
            } else {
                v = t;
            }
            systemAttributes.musicVolume = v;
            SsSetSerialVol(0, v, v);
            cdplay(3);
            t = gPreviousMusicVolume;
            if (systemAttributes.masterVolume < t) {
                t = systemAttributes.masterVolume;
            }
            if (t < 0) {
                v = 0;
            } else {
                v = t;
            }
            t = v;
            systemAttributes.musicVolume = t;
            SsSetSerialVol(0, t, t);
        }
    } else {
        cdplay(3);
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", cbready);
#endif

void cbvsync(void)
{
    u8 result[8];
    s32 st;
    s32 pos;

    if (VSync(-1) % 15 == 0) {
        st = CdSync(1, result);
        if (st == 5) {
            cdplay(6);
            return;
        }
        if (st != 2) {
            return;
        }
        if (CurPos > EndPos || CurPos < StartPos) {
            cdplay(6);
            return;
        }
        pos = CdPosToInt((CdlLOC*)&result[5]);
        if (pos > 0) {
            CurPos = pos;
        }
        CdControlF(0x11, 0);
    }
}

void cdplay(s32 mode)
{
    CdlLOC loc;

    CdIntToPos(StartPos, &loc);
    CdControl(mode, (u8*)&loc, 0);
    CurPos = StartPos;
    RepTime = RepTime + 1;
}

#ifdef NON_MATCHING
void soundProcessIds(void)
{
    s32 i;
    ProgVabRec* progs;
    s32 startIdx;
    s32 numFinishing;
    s32 numVoices;
    SingleSound* ss;
    Tone* t;
    Tone* nxt;
    Tone** vp;
    s32 j;
    s32 k;
    s32 id;
    s32 pct;
    s32 pan;
    s32 scale;
    s32 avail;
    s32 dd;
    u32 d;
    u32 vol;
    u32 volL;
    u32 volR;
    s16 scaleL;
    s16 scaleR;
    u8 create;
    u8 useScale;
    u8 freed;
    u32 tmp;

    startIdx = 0;
    numFinishing = 0;
    soundProcessSpecialSounds();
    scale = 99;
    soundFlushRequestBuffers();
    soundFlushRequestStopBuffers();
    progs = (ProgVabRec*)getProgVabProgs();
    soundResetTotalVoiceCount();
    numVoices = 0;
    if (soundIdRequestIdx != -1) {
        for (i = 0; i < soundIdRequestIdx; i++) {
            id = gRequestQueue[i].id;
            ss = &gSingleSounds[id];
            useScale = 0;
            if (ss->loaded == 0) {
                SsUtGetProgAtr(progs[id].vabId, progs[id].prog, &ss->numTones);
                ss->id = id;
                ss->loaded = 1;
            }
            if (gRequestQueue[i].kind == 3) {
                turnOffAndDeleteASingleSoundsTones(ss);
            } else if (gRequestQueue[i].kind == 2) {
                create = 1;
                if (ss->tones != NULL) {
                    t = ss->tones;
                    switch (gRequestQueue[i].arg3) {
                    case 1:
                        while (t != NULL) {
                            adjustTonePitchByPercent(t, gRequestQueue[i].arg4);
                            t->unk6E = 1;
                            t = t->next;
                        }
                        create = 0;
                        break;
                    case 2:
                        while (t != NULL) {
                            if (gRequestQueue[i].arg5 == 0) {
                                adjustTonePitchByPercent(t, 99);
                            } else {
                                adjustTonePitchByPercent(
                                    t, ((s32(*)(s32))gRequestQueue[i].arg5)(gRequestQueue[i].arg4));
                            }
                            t->unk6E = 1;
                            t = t->next;
                        }
                        create = 0;
                        break;
                    case 4:
                    case 5:
                        turnOffAndDeleteASingleSoundsTones(ss);
                        create = 1;
                        break;
                    case 6:
                        turnOffAndDeleteASingleSoundsTones(ss);
                        create = 1;
                        scale = gRequestQueue[i].arg4;
                        useScale = 1;
                        break;
                    case 0:
                    case 7:
                        while (t != NULL) {
                            t->unk6E = 1;
                            t = t->next;
                        }
                        create = 0;
                        break;
                    case 3:
                    case 8:
                        scale = gRequestQueue[i].arg4;
                        useScale = 1;
                        while (t != NULL) {
                            t->unk6E = 1;
                            t = t->next;
                        }
                        create = 0;
                        break;
                    }
                }
                if (create) {
                    for (j = 0; j < ss->numTones; j++) {
                        t = mallocToneInstance();
                        if (t == NULL) {
                            break;
                        }
                        t->vabId = progs[ss->id].vabId;
                        t->prog = progs[ss->id].prog;
                        t->doppler = progs[ss->id].doppler;
                        t->tone = j;
                        t->soundIndex = ss->id;
                        SsUtGetVagAtr(t->vabId, t->prog, t->tone, &t->priority);
                        t->unk5C = 0x1E00;
                        t->note = 120 - t->note;
                        t->unk64 = (t->unk0C[1] + (t->note - 60)) << 7;
                        t->basePitch = (t->unk0C[0] + (t->note - 60)) << 7;
                        t->pitchRange = t->unk64 - t->basePitch;
                        addToneToSingleSound(t, ss);
                        addToneToTonesToPlayList(t);
                        t->unk6E = 1;
                        switch (gRequestQueue[i].arg3) {
                        case 1:
                            adjustTonePitchByPercent(t, gRequestQueue[i].arg4);
                            t->mode = 2;
                            break;
                        case 2:
                            if (gRequestQueue[i].arg5 == 0) {
                                adjustTonePitchByPercent(t, 99);
                            } else {
                                adjustTonePitchByPercent(
                                    t, ((s32(*)(s32))gRequestQueue[i].arg5)(gRequestQueue[i].arg4));
                            }
                            t->mode = 2;
                            break;
                        case 0:
                            t->mode = 2;
                            break;
                        case 4:
                            t->mode = 2;
                            t->unk50 = 1;
                            t->unk4C = gRequestQueue[i].arg4;
                            break;
                        case 8:
                            scale = gRequestQueue[i].arg4;
                            useScale = 1;
                            t->mode = 1;
                            break;
                        case 7:
                            t->mode = 1;
                            break;
                        case 5:
                            t->mode = 0;
                            break;
                        case 3:
                            useScale = 1;
                            t->mode = 2;
                            scale = gRequestQueue[i].arg4;
                            break;
                        default:
                            useScale = 1;
                            t->mode = 0;
                            scale = gRequestQueue[i].arg4;
                            break;
                        }
                    }
                }
                d = 0;
                if (gRequestQueue[i].dist != 0) {
                    if ((u32)gRequestQueue[i].dist >= (u32)soundGetVolumeRangeSquared()) {
                        d = soundGetVolumeRangeSquared();
                    } else {
                        d = gRequestQueue[i].dist;
                    }
                }
                pan = (gRequestQueue[i].negRange < -160) ? -160 : gRequestQueue[i].negRange;
                if (pan > 160) {
                    pan = 160;
                }
                vol = (soundGetVolumeRangeSquared() - d) * soundGetSFXVolume();
                vol = vol / soundGetVolumeRangeSquared();
                volR = vol;
                volL = volR;
                if (pan > 0) {
                    tmp = ((s16)vol + (s16)((pan * soundGetSFXVolume()) / 160)) > 0
                        ? vol + ((pan * soundGetSFXVolume()) / 160)
                        : 0;
                    volL = tmp;
                } else if (pan < 0) {
                    tmp = ((s16)vol - (s16)((pan * soundGetSFXVolume()) / 160)) > 0
                        ? vol - ((pan * soundGetSFXVolume()) / 160)
                        : 0;
                    volR = tmp;
                }
                scaleL = (ss->mvol * (s16)volL) / 99;
                scaleR = (ss->mvol * (s16)volR) / 99;
                t = ss->tones;
                if (t != NULL) {
                    do {
                        t->volL = (t->unk08 * scaleL) / 99;
                        t->volR = (t->unk08 * scaleR) / 99;
                        if (useScale) {
                            t->volL = (t->volL * scale) / 99;
                            t->volR = (t->volR * scale) / 99;
                        }
                        t->unk78 = t->range;
                        t->range = gRequestQueue[i].dist;
                        if (t->doppler != 0) {
                            if ((u32)soundGetMasterDopplerEffectiveRangeSquared() >= t->range) {
                                t->pitchDelta = (s32)(t->unk78 - t->range)
                                    / (GetFieldsLastFrame() * soundGetMasterDopplerDelta());
                                if (t->pitchDelta > 2400) {
                                    t->pitchDelta = 0;
                                }
                                if (t->pitchDelta != 0) {
                                    dd = t->pitchDelta;
                                    if (dd < 0) {
                                        dd = -dd;
                                    }
                                    if (dd < 1200) {
                                        t->dirty = 1;
                                    }
                                }
                                t->pitchDelta = (t->pitchDelta > -1200) ? t->pitchDelta : -1200;
                                t->pitchDelta = (t->pitchDelta <= 1200) ? t->pitchDelta : 1200;
                            } else {
                                t->pitchDelta = 0;
                            }
                        }
                        t = t->next;
                    } while (t != NULL);
                }
            }
        }
    }
    t = tonesToPlayHead;
    if (t != NULL) {
        vp = &gVoiceList[numVoices];
        do {
            freed = 0;
            if (t->unk6E == 0) {
                if (t->state == 1) {
                    if (SpuGetKeyStatus(1 << t->voice) == 0) {
                        soundOff(t);
                        freed = 1;
                        deleteToneFromSingleSound(&gSingleSounds[t->soundIndex], t);
                        nxt = t->nextToPlay;
                        goto killTone;
                    }
                    if (t->unk50 != 0) {
                        t->unk4C = t->unk4C - 1;
                        if (t->unk4C <= 0) {
                            soundOff(t);
                            freed = 1;
                            deleteToneFromSingleSound(&gSingleSounds[t->soundIndex], t);
                            nxt = t->nextToPlay;
                            goto killTone;
                        }
                        t->unk6E = 1;
                    } else if (t->mode == 0 || t->mode == 1 || t->mode == 2) {
                        if (numFinishing + 1 < 29) {
                            gFinishingTonesList[numFinishing] = t;
                            numFinishing = numFinishing + 1;
                            t = t->nextToPlay;
                            soundIncTotalVoiceCount();
                            freed = 1;
                        } else {
                            nxt = t->nextToPlay;
                            soundOff(t);
                            freed = 1;
                            deleteToneFromSingleSound(&gSingleSounds[t->soundIndex], t);
                        killTone:
                            deleteToneFromTonesToPlayList(t);
                            freeToneInstance(t);
                            t = nxt;
                        }
                    }
                }
            }
            if (!freed) {
                if (t->unk6E == 2) {
                    t->unk6E = 1;
                }
                if (t->unk6E == 1) {
                    *vp = t;
                    vp++;
                    numVoices = numVoices + 1;
                    soundIncTotalVoiceCount();
                }
                t = t->nextToPlay;
            }
        } while (t != NULL);
        soundSortInstances(gVoiceList, 0, numVoices - 1);
        if (numVoices >= 24) {
            for (k = 24; k < numVoices; k++) {
                if (gVoiceList[k]->state == 1) {
                    gVoiceList[k]->unk6E = 2;
                    soundOff(gVoiceList[k]);
                } else {
                    soundOff(gVoiceList[k]);
                    deleteToneFromSingleSound(
                        &gSingleSounds[gVoiceList[k]->soundIndex], gVoiceList[k]);
                    deleteToneFromTonesToPlayList(gVoiceList[k]);
                    freeToneInstance(gVoiceList[k]);
                }
            }
            numVoices = 24;
        } else {
            avail = 24 - numVoices;
            if (numFinishing < avail) {
                avail = numFinishing;
            }
            startIdx = avail;
        }
        if (numFinishing != 0) {
            for (k = startIdx; k < numFinishing; k++) {
                soundOff(gFinishingTonesList[k]);
                deleteToneFromSingleSound(
                    &gSingleSounds[gFinishingTonesList[k]->soundIndex], gFinishingTonesList[k]);
                deleteToneFromTonesToPlayList(gFinishingTonesList[k]);
                freeToneInstance(gFinishingTonesList[k]);
            }
        }
        soundPlayAllRequests(numVoices, gVoiceList);
    }
    soundIdRequestIdx = -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundProcessIds);
#endif

#ifdef NON_MATCHING
void soundForceAllSoundsOffNow(void)
{
    Tone* t;
    Tone* nxt;

    soundShutDownSpecialSounds();
    if (tonesToPlayHead != NULL) {
        t = tonesToPlayHead;
        do {
            soundOff(t);
            deleteToneFromSingleSound(&gSingleSounds[t->soundIndex], t);
            nxt = t->nextToPlay;
            deleteToneFromTonesToPlayList(t);
            freeToneInstance(t);
            t = nxt;
        } while (t != NULL);
        tonesToPlayHead = NULL;
    }
    SsUtAllKeyOff(0);
    SsUtFlush();
    soundIdRequestIdx = -1;
    soundInitRequestBuffers();
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", soundForceAllSoundsOffNow);
#endif

u8 filterIds(s32 id)
{
    return 1;
}

void soundPlayId(u32 id, u32 dist, s32 range, s32 a3, s32 a4, s32 a5)
{
    s32 i;
    s32 fifth;
    s32 sixth;

    do {
        fifth = a4;
        sixth = a5;
    } while (0);

    if (id < 139 && dist < soundGetVolumeRangeSquared() && filterIds(id)) {
        if (soundIdRequestIdx == -1) {
            soundIdRequestIdx = 0;
        }
        i = soundIdRequestIdx;
        gRequestQueue[i].negRange = (s32)(0u - (u32)range);
        gRequestQueue[i].id = id;
        gRequestQueue[i].dist = dist;
        gRequestQueue[i].arg3 = a3;
        gRequestQueue[i].arg4 = fifth;
        gRequestQueue[i].arg5 = sixth;
        gRequestQueue[i].kind = 2;
        soundIdRequestIdx = i + 1;
        if (soundIdRequestIdx >= 46) {
            soundIdRequestIdx = i;
        }
    }
}

void soundStopPlayId(u32 id, s32 a1, s32 a2, s32 unused)
{
    s32 i;

    (void)unused; /* Retail accepts the callers' fourth word but never reads it. */
    if (id < 139 && filterIds(id)) {
        if (soundIdRequestIdx == -1) {
            soundIdRequestIdx = 0;
        }
        i = soundIdRequestIdx;
        gRequestQueue[i].id = id;
        gRequestQueue[i].arg3 = a1;
        gRequestQueue[i].arg4 = a2;
        gRequestQueue[i].kind = 3;
        soundIdRequestIdx = i + 1;
        if (soundIdRequestIdx >= 46) {
            soundIdRequestIdx = i;
        }
    }
}

#ifdef NON_MATCHING
void adjustTonePitchByPercent(Tone* t, s32 percent)
{
    u32 scaled;
    s32 pitch;

    if (percent < 0) {
        percent = 0;
    }
    if (percent >= 100) {
        percent = 99;
    }
    scaled = percent * t->pitchRange;
    pitch = t->basePitch + (scaled / 99);
    t->note = pitch / 128;
    t->fine = pitch - (t->note << 7);
    t->dirty = 1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", adjustTonePitchByPercent);
#endif

#ifdef NON_MATCHING
void addToneToSingleSound(Tone* t, SingleSound* ss)
{
    Tone* p;

    if (ss->tones != NULL) {
        p = ss->tones;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
        t->prev = p;
    } else {
        ss->tones = t;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", addToneToSingleSound);
#endif

#ifdef NON_MATCHING
void deleteToneFromSingleSound(SingleSound* ss, Tone* t)
{
    if (t->prev == NULL && t->next == NULL) {
        ss->tones = NULL;
        return;
    }
    if (t->prev == NULL) {
        ss->tones = t->next;
        t->next->prev = NULL;
        t->next = NULL;
        return;
    }
    if (t->next == NULL) {
        t->prev->next = NULL;
        t->prev = NULL;
        return;
    }
    t->prev->next = t->next;
    t->next->prev = t->prev;
    t->prev = NULL;
    t->next = NULL;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", deleteToneFromSingleSound);
#endif

void addToneToTonesToPlayList(Tone* t)
{
    Tone* tail;

    if (tonesToPlayHead == NULL) {
        tonesToPlayHead = t;
        tonesToPlayTail = t;
        return;
    }
    tail = tonesToPlayTail;
    tonesToPlayTail = t;
    tail->nextToPlay = t;
    t->prevToPlay = tail;
}

#ifdef NON_MATCHING
void deleteToneFromTonesToPlayList(Tone* t)
{
    if (t->prevToPlay == NULL && t->nextToPlay == NULL) {
        tonesToPlayHead = NULL;
        tonesToPlayTail = NULL;
        return;
    }
    if (t->prevToPlay == NULL) {
        tonesToPlayHead = t->nextToPlay;
        t->nextToPlay->prevToPlay = NULL;
        t->nextToPlay = NULL;
        return;
    }
    if (t->nextToPlay == NULL) {
        tonesToPlayTail = t->prevToPlay;
        t->prevToPlay->nextToPlay = NULL;
        t->prevToPlay = NULL;
        return;
    }
    t->prevToPlay->nextToPlay = t->nextToPlay;
    t->nextToPlay->prevToPlay = t->prevToPlay;
    t->prevToPlay = NULL;
    t->nextToPlay = NULL;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", deleteToneFromTonesToPlayList);
#endif

void turnOffAndDeleteASingleSoundsTones(SingleSound* ss)
{
    Tone* t;

    t = ss->tones;
    while (t != NULL) {
        soundOff(t);
        deleteToneFromSingleSound(ss, t);
        deleteToneFromTonesToPlayList(t);
        freeToneInstance(t);
        t = ss->tones;
    }
    ss->tones = NULL;
}

void soundIdInit(void)
{
    /* BIOS memset(dst,fill,count): first call has count0; second fills only
     * the first0x30 bytes with0x0C. Do not turn these into whole-pool clears. */
    sdk_memset(gSingleSounds, 0xF34, 0);
    sdk_memset(gTonePool, 0x120C, 0x30);
}

#ifdef NON_MATCHING
Tone* mallocToneInstance(void)
{
    Tone* result;
    s32 i;
    s32 j;
    s32 n;

    result = NULL;
    if (gToneNumFree != 0) {
        i = gToneMinFree;
        n = gToneMaxUsed;
        while (i <= n) {
            if (gTonePool[i].inUse == 0) {
                result = &gTonePool[i];
                gToneNumFree--;
                j = i + 1;
                if (gToneMinFree >= j) {
                    j = gToneMinFree;
                }
                gToneMinFree = j;
                result->poolIndex = i;
                result->next = NULL;
                result->prev = NULL;
                result->nextToPlay = NULL;
                result->prevToPlay = NULL;
                result->inUse = 1;
                result->state = 0;
                result->noise = 0;
                result->unk50 = 0;
                result->pitchDelta = 0;
                result->doppler = 0;
                break;
            }
            n = gToneMaxUsed;
            i++;
        }
    }
    return result;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", mallocToneInstance);
#endif

#ifdef NON_MATCHING
void freeToneInstance(Tone* t)
{
    u32 lo;
    u32 hi;

    if (t != NULL) {
        t->inUse = 0;
        if (gToneMinFree < t->poolIndex) {
            lo = gToneMinFree;
        } else {
            lo = t->poolIndex;
        }
        hi = gToneMaxUsed;
        gToneMinFree = lo;
        if (hi < t->poolIndex) {
            hi = t->poolIndex;
        }
        gToneMaxUsed = hi;
        gToneNumFree++;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound", freeToneInstance);
#endif

s16* getProgVabProgs()
{
    return D_8016FEFC;
}
