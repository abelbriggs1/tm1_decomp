#ifndef __TM1_STARS_H__
#define __TM1_STARS_H__

#include "common.h"
#include <libgpu.h>
#include <libgte.h>

typedef struct Star {
    /*0x00*/ SVECTOR pos;
    /*0x08*/ u8 r;
    /*0x09*/ u8 g;
    /*0x0A*/ u8 b;
    /*0x0B*/ u8 phase;
} Star;

void create_stars(void);
void display_stars(u_long* ot, s32 view);

#endif // __TM1_STARS_H__
