#ifndef __TM1_SOUND_REQ_H__
#define __TM1_SOUND_REQ_H__

#include "common.h"

/* A pending "play this sound" request.  Only the nearest (smallest `dist`)
 * request survives until the buffer is flushed once per frame. */
typedef struct SoundRequest {
    /*0x00*/ u8 pending;
    /*0x04*/ s32 id;
    /*0x08*/ s32 dist;
    /*0x0C*/ s32 arg2;
    /*0x10*/ s32 arg3;
    /*0x14*/ s32 arg4;
    /*0x18*/ s32 arg5;
    /*0x1C*/ s32 unk1C;
} SoundRequest; /* 0x20 */

/* The "no request yet" distance sentinel written by initRequestBuffer and by
 * flushSoundRequestBuffer after the sound has been handed to soundPlayId. */
#define SOUND_REQUEST_NO_DIST 0xABF549

void flushSoundRequestBuffer(SoundRequest* req);
void soundFlushRequestBuffers(void);
void soundFlushRequestStopBuffers(void);
void initRequestBuffer(SoundRequest* req);
void soundInitRequestBuffers(void);
void addSoundRequest(s32 id, s32 dist, s32 arg2, s32 arg3, s32 arg4, s32 arg5, SoundRequest* req);
void soundProcessSpecialSounds(void);
void soundShutDownSpecialSounds(void);

#endif /* __TM1_SOUND_REQ_H__ */
