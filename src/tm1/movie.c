#include "common.h"

#include <libcd.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpress.h>
#include <libsnd.h>
#include <stdio.h>

#include "tm1/ctlpad.h"
#include "tm1/fileio.h"
#include "tm1/rt.h"
#include "tm1/screen.h"
#include "tm1/sound.h"
#include "tm1/timer.h"

#include "tm1/movie.h"

typedef struct StrEnv {
    u_long* buf[2]; /* 0x00 */
    s32 idx; /* 0x08 */
    u_long* img; /* 0x0C */
    RECT rect[2]; /* 0x10 */
    s32 flip; /* 0x20 */
    RECT slice; /* 0x24 */
    s32 done; /* 0x2C */
} StrEnv; /* 0x30 */

s32 lastFrame = 0;
s32 lastW = 0;
s32 lastH = 0;

s32 SliceSize;
s16 SliceX;
s16 SliceY;
s16 SliceW;
s16 SliceH;
u16 gSectorsPer;
u_long* mdec_bs;
u_long* mdec_rl;
u_long* mdec_image;
u8 gIsRGB24;
CdlLOC loc;
s32 Rewind_Switch;
u_long* sect_buff;

DISPENV Disp[2];
StrEnv dec;

extern s32 StCdIntrFlag;

extern void DecDCToutCallback(void (*func)());

void disp_mdec(u_long* src, s32* pflip);
void strSetDefDecEnv(StrEnv* env, s32 flip, s32 x0, s32 y0, s32 x1, s32 y1, s32 w, s32 h);
void strInit(CdlLOC* pos, void (*cb)());
void strCallback(void);
s32 strNextVlc(StrEnv* env, u32 limit);
u_long* strNext(StrEnv* env, u32 limit);
void strSync(StrEnv* env, s32 unused);
void strKickCD(CdlLOC* pos);

#ifdef NON_MATCHING
s32 moviePlaySingleTracLogoMovie(u_long* buf, char* name)
{
    s32 size;
    s32 flip;
    s32 fd;
    s32 pos;
    s32 nsec;
    s32 abort;

    flip = 0;
    fd = fileioOpenFile(name, &size);
    if (fd == -1) {
        return 0;
    }
    do {
        fileioReadFile(fd, buf, 0x800);
    } while (0);
    gSectorsPer = ((u16*)buf)[3];
    nsec = gSectorsPer;
    fileioReadFile(fd, buf + 512, (nsec - 1) << 11);
    SliceSize = 0x780;
    SliceW = 16;
    SliceH = 240;
    pos = gSectorsPer << 11;
    SetDefDispEnv(&Disp[0], 0, 0, 320, 240);
    SetDefDispEnv(&Disp[1], 320, 0, 320, 240);
    Disp[1].isrgb24 = 0;
    Disp[0].isrgb24 = 0;
    DecDCTReset(0);
    abort = 0;
    while (pos <= size) {
        disp_mdec(buf, &flip);
        UpdCtlPad();
        if ((GetCtlPad(2, 2) & 0xff) || (GetCtlPad(1, 2) & 0xff)) {
            abort = 1;
            break;
        }
        fileioReadFile(fd, buf, gSectorsPer << 11);
        pos = pos + (gSectorsPer << 11);
    }
    if (flip == 1) {
        screenDisplayToDisplay(0, 1);
    }
    fileioCloseFile(fd);
    return abort;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", moviePlaySingleTracLogoMovie);
#endif

#ifdef NON_MATCHING
void disp_mdec(u_long* src, s32* pflip)
{
    u_long* d;
    u_long* s;
    s32 i;
    s32 j;

    d = mdec_bs;
    for (i = 0; i < gSectorsPer; i++) {
        s = src + 8;
        for (j = 0; j < 504; j++) {
            *d++ = *s++;
        }
        src += 512;
    }
    DecDCTvlc(mdec_bs, mdec_rl);
    DecDCTin(mdec_rl, 0);
    SliceX = (*pflip != 0) ? 320 : 0;
    SliceY = 0;
    i = SliceX + 320;
    while (SliceX < i) {
        DecDCTout(mdec_image, SliceSize);
        DecDCToutSync(0);
        LoadImage((RECT*)&SliceX, mdec_image);
        SliceX = SliceX + SliceW;
    }
    VSync(0);
    PutDispEnv(&Disp[*pflip]);
    *pflip = (*pflip == 0);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", disp_mdec);
#endif

void movieInit(void)
{
    mdec_bs = (u_long*)rtGetDoubleBufferArea();
    mdec_rl = (u_long*)rtGetDoubleBufferArea() + 5040;
    mdec_image = (u_long*)rtGetDoubleBufferArea();
}

#ifdef NON_MATCHING
s32 moviePlaySlideShow(char* fmt, s32 count, s32 secs, s32 allowAbort)
{
    char name[48];
    s32 i;
    s32 t0;
    s32 first;
    s32 lim;

    soundInterruptPlayDA();
    first = ctlpadAnyKey(2) & 0xff;
    for (i = 0; i < count; i++) {
        t0 = GetCurTics();
        sprintf(name, fmt, i);
        fileioLoadFileIntoRam(rtGetDoubleBufferArea(), name);
        screenDisplayImage(1, (u_long*)rtGetDoubleBufferArea(), 0, 0);
        if ((GetCurTics() - t0) < (secs * 68644)) {
            do {
            } while (0);
            lim = secs;
            lim = lim * 68644;
            do {
                VSync(0);
                UpdCtlPad();
                if (ctlpadAnyKey(2) & 0xff) {
                    if (first == 0 && allowAbort != 0) {
                        return -1;
                    }
                } else {
                    first = 0;
                }
            } while ((GetCurTics() - t0) < lim);
        }
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", moviePlaySlideShow);
#endif

#ifdef NON_MATCHING
void strSetDefDecEnv(StrEnv* env, s32 flip, s32 x0, s32 y0, s32 x1, s32 y1, s32 w, s32 h)
{
    u_long* p;
    s32 sw;

    p = (u_long*)rtGetDoubleBufferArea() + 16384;
    env->buf[0] = p;
    p += 19200;
    env->buf[1] = p;
    p += 19200;
    env->idx = 0;
    env->img = p;
    env->flip = flip ^ 1;
    env->done = 0;
    env->rect[0].x = x0;
    env->rect[0].y = y0;
    env->rect[0].w = w;
    env->rect[0].h = h;
    env->rect[1].x = x1;
    env->rect[1].y = y1;
    env->rect[1].w = w;
    env->rect[1].h = h;
    sw = 16;
    if (gIsRGB24) {
        sw = 24;
    }
    env->slice.x = x0;
    env->slice.y = y0;
    env->slice.w = sw;
    env->slice.h = h;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strSetDefDecEnv);
#endif

#ifdef NON_MATCHING
void strInit(CdlLOC* pos, void (*cb)())
{
    DecDCTReset(0);
    Rewind_Switch = 0;
    DecDCToutCallback(cb);
    StSetRing(sect_buff, 32);
    StSetStream(gIsRGB24, 1, -1, 0, 0);
    strKickCD(pos);
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strInit);
#endif

// void strCallback(void)
// {
//     s32 f;
//
//     if (gIsRGB24 != 0 && StCdIntrFlag != 0) {
//         StCdInterrupt();
//         StCdIntrFlag = 0;
//     }
//     LoadImage(&dec.slice, dec.img);
//     dec.slice.x = dec.slice.x + dec.slice.w;
//     f = dec.flip;
//     if (dec.slice.x < dec.rect[f].x + dec.rect[f].w) {
//         DecDCTout(dec.img, (dec.slice.w * dec.slice.h) / 2);
//     } else {
//         dec.done = 1;
//         f = (f == 0);
//         dec.flip = f;
//         dec.slice.x = dec.rect[f].x;
//         dec.slice.y = dec.rect[f].y;
//     }
// }
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strCallback);

#ifdef NON_MATCHING
s32 strNextVlc(StrEnv* env, u32 limit)
{
    s32 n;
    u_long* p;

    for (n = 0x800000; n != 0; n--) {
        p = strNext(env, limit);
        if (p != 0) {
            env->idx = (env->idx == 0);
            DecDCTvlc(p, env->buf[env->idx]);
            StFreeRing(p);
            return 0;
        }
    }
    return -1;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strNextVlc);
#endif

#ifdef NON_MATCHING
u_long* strNext(StrEnv* env, u32 limit)
{
    u_long* addr;
    u_long* header;
    s32 n;
    s32 w;

    n = 0x800000;
    do {
        if (StGetNext(&addr, &header) == 0) {
            goto found;
        }
        n--;
    } while (n != 0);
    return 0;
found:
    lastFrame = header[2];
    if (header[2] >= limit) {
        Rewind_Switch = 1;
    }
    if (lastW != ((u16*)header)[8] || lastH != ((u16*)header)[9]) {
        lastW = ((u16*)header)[8];
        lastH = ((u16*)header)[9];
    }
    if (gIsRGB24) {
        w = (lastW * 3) / 2;
    } else {
        w = lastW;
    }
    env->rect[1].w = w;
    env->rect[0].w = env->rect[1].w;
    env->rect[1].h = lastH;
    env->rect[0].h = env->rect[1].h;
    env->slice.h = lastH;
    return addr;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strNext);
#endif

#ifdef NON_MATCHING
void strSync(StrEnv* env, s32 unused)
{
    s32 n;

    *(&n) = 0x800000;
    if (env->done == 0) {
        do {
            *(&n) = *(&n) - 1;
            if (*(&n) == 0) {
                printf("time out in decoding !\n");
                env->done = 1;
                env->flip = (env->flip == 0);
                env->slice.x = env->rect[env->flip].x;
                env->slice.y = env->rect[env->flip].y;
            }
        } while (env->done == 0);
    }
    env->done = 0;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", strSync);
#endif

void strKickCD(CdlLOC* pos)
{
    while (CdControl(0x15, (u_char*)pos, 0) == 0) { }
    while (CdRead2(0x1C0) == 0) { }
}

#ifdef NON_MATCHING
s32 moviecdPlayMovie(char* name, MovieCallback cb, u8 wait, u8 rgb24, s32 limit, s32 x0, s32 y0,
    s32 x1, s32 y1, s32 w, s32 h, DISPENV* disp, u8* pflip)
{
    CdlFILE pos;
    u8 waitFlag;
    s32 done;
    u8 quit;
    s32 mode;
    s32 flip;

    done = 0;
    quit = 0;
    mode = 2;
    gIsRGB24 = rgb24;
    waitFlag = wait;
    if (rgb24 != 0) {
        mode = 3;
    }
    if (CdSearchFile(&pos, name) == 0) {
        printf("In moviecdPlayMovie file %s not found\n", name);
        return 0;
    }
    sect_buff = (u_long*)rtGetDoubleBufferArea();
    SsSetSerialVol(0, soundGetMasterVolume(), soundGetMasterVolume());
    do {
        loc.minute = pos.pos.minute;
        loc.second = pos.pos.second;
        loc.sector = pos.pos.sector;
        strSetDefDecEnv(&dec, *pflip, x0, y0, x1, y1, w, h);
        strInit(&loc, strCallback);
        strNextVlc(&dec, limit);
        while (1) {
            if (done != 0) {
                break;
            }
            DecDCTin(dec.buf[dec.idx], mode);
            DecDCTout(dec.img, (dec.slice.w * dec.slice.h) / 2);
            strNextVlc(&dec, limit);
            strSync(&dec, 0);
            VSync(0);
            flip = (dec.flip == 0);
            *pflip = flip;
            if (rgb24 != 0) {
                disp[flip].isrgb24 = gIsRGB24;
            }
            PutDispEnv(&disp[flip]);
            SetDispMask(1);
            if (rgb24 == 0) {
                FntFlush(-1);
            }
            if (Rewind_Switch == 1) {
                break;
            }
            if (cb != 0) {
                done = cb(lastFrame, limit, flip);
            }
        }
        if (done != 0 || Rewind_Switch == 0 || waitFlag == 0) {
            quit = 1;
        }
    } while (quit == 0);
    SsSetSerialVol(0, 0, 0);
    DecDCToutCallback(0);
    CdDataCallback(0);
    CdReadyCallback(0);
    SsSetSerialVol(0, soundGetMusicVolume(), soundGetMusicVolume());
    CdControl(9, 0, 0);
    return done;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/movie", moviecdPlayMovie);
#endif
