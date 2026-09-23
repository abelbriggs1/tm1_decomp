#ifndef __TM1_SOUND_H__
#define __TM1_SOUND_H__

#include "common.h"
#include "tm1/sound_id_api.h"

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} SndVec16;

/* One playing tone = one element of gTonePool (0x84 bytes, 35 entries). */
typedef struct Tone {
    /*0x00*/ u8 vabId;
    /*0x01*/ u8 prog;
    /*0x02*/ u8 soundIndex;
    /*0x03*/ u8 tone;
    /*0x04*/ u8 voice;
    /*0x05*/ u8 unk05;
    /*0x06*/ u8 priority;
    /*0x07*/ u8 unk07;
    /*0x08*/ u8 unk08;
    /*0x09*/ u8 unk09;
    /*0x0A*/ u8 note;
    /*0x0B*/ u8 fine;
    /*0x0C*/ u8 unk0C[0x1C];
    /*0x28*/ u32 poolIndex;
    /*0x2C*/ struct Tone* next;
    /*0x30*/ struct Tone* prev;
    /*0x34*/ struct Tone* nextToPlay;
    /*0x38*/ struct Tone* prevToPlay;
    /*0x3C*/ s32 inUse;
    /*0x40*/ u32 mode;
    /*0x44*/ s32 unk44;
    /*0x48*/ s32 unk48;
    /*0x4C*/ s32 unk4C;
    /*0x50*/ u8 unk50;
    /*0x51*/ u8 unk51[3];
    /*0x54*/ s32 state;
    /*0x58*/ u8 dirty;
    /*0x59*/ u8 unk59;
    /*0x5A*/ u8 unk5A;
    /*0x5B*/ u8 unk5B;
    /*0x5C*/ s32 unk5C;
    /*0x60*/ s32 basePitch;
    /*0x64*/ s32 unk64;
    /*0x68*/ s32 pitchRange;
    /*0x6C*/ u8 volL;
    /*0x6D*/ u8 volR;
    /*0x6E*/ u8 unk6E;
    /*0x6F*/ u8 unk6F;
    /*0x70*/ s32 noise;
    /*0x74*/ u32 range;
    /*0x78*/ s32 unk78;
    /*0x7C*/ s32 pitchDelta;
    /*0x80*/ u8 doppler;
} Tone;

/* One logical sound id = one element of gSingleSounds (0x1C bytes, 139 entries).
   +4 is where soundProcessIds points SsUtGetProgAtr, so +4/+5 are the PsyQ ProgAtr's
   `tones`/`mvol` bytes. */
typedef struct SingleSound {
    /*0x00*/ s32 id;
    /*0x04*/ u8 numTones;
    /*0x05*/ u8 mvol;
    /*0x06*/ u8 unk06[0x0E];
    /*0x14*/ s32 loaded;
    /*0x18*/ Tone* tones;
} SingleSound;

typedef struct ProgVabRec {
    /*0x0*/ u16 vabId;
    /*0x2*/ u16 prog;
    /*0x4*/ u8 doppler;
    /*0x5*/ u8 pad;
} ProgVabRec;

/* One queued play/stop request (0x1C bytes, 46 entries). */
typedef struct SoundIdRequest {
    /*0x00*/ s32 id;
    /*0x04*/ s32 dist;
    /*0x08*/ s32 negRange;
    /*0x0C*/ s32 arg3;
    /*0x10*/ s32 arg4;
    /*0x14*/ s32 arg5;
    /*0x18*/ s32 kind;
} SoundIdRequest;

typedef struct {
    s16 masterVolume; /* +0  */
    s16 pad2; /* +2  */
    s32 sfxVolume; /* +4  */
    s32 musicVolume; /* +8  */
    s32 reverbVolume; /* +12 */
    s32 reverbType; /* +16 */
    s32 volumeRange; /* +20 */
    s32 volumeRangeSquared; /* +24 */
    s32 dopplerDelta; /* +28 */
    s32 dopplerEffectiveRange; /* +32 */
    s32 dopplerEffectiveRangeSquared; /* +36 */
    s16 activeVoiceCount; /* +40 */
    s16 totalVoiceCount; /* +42 */
    s32 numProgVabProgs; /* +44 */
} SoundSystemAttributes; /* 0x30 -- the whole .data this TU owns per the yaml */

typedef struct SndMatrix {
    u16 m[9];
} SndMatrix;

void soundMulVec(SndMatrix* m, s32* v, s32* out);
void soundSetRangeAndXPositionFromWorldLoc(s32* loc);
s32 getOpenVoice(void);
void silenceUnusedVoices(void);
void soundOff(Tone* t);
void dopplerAdjustTonePitch(Tone* t, u8* noteOut, u8* fineOut);
void soundOn(Tone* t);
void soundOffOn(Tone* t);
void soundPlaySound(Tone* t);
void soundRangeSortInstances(Tone** list, s32 lo, s32 hi);
void soundPrioritySortInstances(Tone** list, s32 lo, s32 hi);
void soundSortInstances(Tone** list, s32 lo, s32 hi);
void soundIdInit(void);
void soundPlayAllRequests(s32 count, Tone** list);
void soundForceAllSoundsOffNow(void);
void soundLoadCoreVabIntoSPURam(u8* body);
void soundLoadLevelVablIntoSPURam(u8* body);
void soundInit(void);
void soundInitRequestBuffers(void);
void soundShutDownSpecialSounds(void);
void addToneToTonesToPlayList(Tone* t);
void deleteToneFromTonesToPlayList(Tone* t);
void addToneToSingleSound(Tone* t, SingleSound* ss);
void deleteToneFromSingleSound(SingleSound* ss, Tone* t);
void turnOffAndDeleteASingleSoundsTones(SingleSound* ss);
Tone* mallocToneInstance(void);
void freeToneInstance(Tone* t);
void adjustTonePitchByPercent(Tone* t, s32 percent);
s32 soundGetCurrentDATrack();
s32 soundGetCalculatedSoundRange();
s32 soundGetCalculatedSoundXPosition();
void soundResetSoundEngine();
void releaseUsedVoice(s32 voice);
void soundForceAllSoundsOffNow();
u8 vabIdIsValid(s32 vabId);
void soundInitLevelPersistantSounds(s32 which);
void soundCloseLevelPersistantSounds(s32 which);
void soundInitLevelSwapSounds(s32 which);
void soundCloseLevelSwapSounds(s32 which);
void soundCloseGamePersistantSounds();
void soundUnloadLevelVabFromSPURam();
void soundShutdown();
void soundStopPlayDA();
s32 soundGetMasterDopplerDelta();
s32 soundGetMasterDopplerEffectiveRange();
void soundSetMasterDopplerDelta(s32 delta);
void soundSetMasterDopplerEffectiveRange(s32 range);
s32 soundGetMasterDopplerEffectiveRangeSquared();
void soundSetReverbVolume(s32 vol);
s32 soundGetReverbVolume();
void soundSetReverbType(s32 type);
s32 soundGetReverbType();
void soundSetReverbOn(s32 type);
void soundSetReverbOff();
s32 soundGetMasterVolume();
void soundSetMasterVolume(s32 vol);
void soundSetSFXVolume(s32 vol);
s32 soundGetSFXVolume();
void soundSetMusicVolume(s32 vol);
s32 soundGetMusicVolume();
void soundSetVolumeRange(s32 range);
s32 soundGetVolumeRangeSquared();
s32 soundGetVolumeRange();
void soundIncActiveVoiceCount();
void soundDecActiveVoiceCount();
void soundIncTotalVoiceCount();
void soundDecTotalVoiceCount();
s32 soundGetActiveVoiceCount();
s32 soundGetTotalVoiceCount();
void soundResetActiveVoiceCount();
void soundResetTotalVoiceCount();
s32 soundGetMusicVolumePreviousToInterrupt();
void soundSetPreviousMusicVolume(s32 vol);
s32 cdGetPos();
s32 cdGetRepTime();
u8 filterIds(s32 id);
s16* getProgVabProgs();
void swapInstances(Tone** list, s32 a, s32 b);
void soundProcessIds(void);

#endif
