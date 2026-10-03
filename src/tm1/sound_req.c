#include "common.h"

#include "tm1/sound_id_api.h"
#include "tm1/sound_req.h"
#include "tm1/ua_sound.h"

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

#ifdef NON_MATCHING
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
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound_req", initRequestBuffer);
#endif

void soundInitRequestBuffers(void)
{
    initRequestBuffer(&HOVER_REQUEST);
}

#ifdef NON_MATCHING
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
#else
INCLUDE_ASM("asm/nonmatchings/tm1/sound_req", addSoundRequest);
#endif

void soundProcessSpecialSounds(void)
{
}

void soundShutDownSpecialSounds(void)
{
}
