#ifndef __TM1_MOVIE_H__
#define __TM1_MOVIE_H__

#include "common.h"
#include <libgpu.h>

typedef s32 (*MovieCallback)(s32 frames, s32 limit, u8 flip);

s32 moviePlaySingleTracLogoMovie(u_long* buf, char* name);
void movieInit(void);
s32 moviePlaySlideShow(char* fmt, s32 count, s32 secs, s32 allowAbort);
s32 moviecdPlayMovie(char* name, MovieCallback cb, u8 wait, u8 rgb24, s32 limit, s32 x0, s32 y0,
    s32 x1, s32 y1, s32 w, s32 h, DISPENV* disp, u8* pflip);

#endif // __TM1_MOVIE_H__
