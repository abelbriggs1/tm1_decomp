#include "common.h"

#include "tm1/curbs.h"

// The definitions almost certainly live in the AI or car code
// (`numCurbs` would be on an invalid 0x2 `.sdata` boundary if
// defined here).
// Leaving them declared here for now.
typedef struct {
    s16 w; /* 0x00 */
    s16 h; /* 0x02 */
    s16 t; /* 0x04 */
    s16 pad; /* 0x06 */
    s32 x; /* 0x08 */
    s32 z; /* 0x0C */
} Curb; /* 0x10 */
extern Curb curbs[];
extern s16 numCurbs;

void InitLevel1Curbs(void)
{
    numCurbs = 0;
}

void InitLevel2Curbs(void)
{
    numCurbs = 8;
    curbs[0].x = -14112;
    curbs[0].z = -6912;
    curbs[0].w = 4224;
    curbs[0].h = 4224;
    curbs[0].t = 8;

    curbs[1].x = -14112;
    curbs[1].z = -2112;
    curbs[1].w = 4224;
    curbs[1].h = 4224;
    curbs[1].t = 8;

    curbs[2].x = -14112;
    curbs[2].z = 2688;
    curbs[2].w = 1824;
    curbs[2].h = 4224;
    curbs[2].t = 8;

    curbs[3].x = -11984;
    curbs[3].z = 2688;
    curbs[3].w = 4800;
    curbs[3].h = 4224;
    curbs[3].t = 8;

    curbs[4].x = -6912;
    curbs[4].z = 2688;
    curbs[4].w = 1824;
    curbs[4].h = 4224;
    curbs[4].t = 8;

    curbs[5].x = -9312;
    curbs[5].z = 288;
    curbs[5].w = 4224;
    curbs[5].h = 1824;
    curbs[5].t = 8;

    curbs[6].x = -9312;
    curbs[6].z = -4800;
    curbs[6].w = 4224;
    curbs[6].h = 4800;
    curbs[6].t = 8;

    curbs[7].x = -9312;
    curbs[7].z = -6912;
    curbs[7].w = 4224;
    curbs[7].h = 1824;
    curbs[7].t = 8;
}

void InitLevel3Curbs(void)
{
    numCurbs = 0;
}

void InitLevel4Curbs(void)
{
    numCurbs = 16;
    curbs[0].x = -4512;
    curbs[0].z = -6912;
    curbs[0].w = 4224;
    curbs[0].h = 1824;
    curbs[0].t = 8;

    curbs[1].x = -4512;
    curbs[1].z = -4800;
    curbs[1].w = 4224;
    curbs[1].h = 2400;
    curbs[1].t = 8;

    curbs[2].x = -4512;
    curbs[2].z = -2112;
    curbs[2].w = 4224;
    curbs[2].h = 1824;
    curbs[2].t = 8;

    curbs[3].x = -4512;
    curbs[3].z = 0;
    curbs[3].w = 4224;
    curbs[3].h = 4800;
    curbs[3].t = 8;

    curbs[4].x = -4512;
    curbs[4].z = 5088;
    curbs[4].w = 4224;
    curbs[4].h = 1824;
    curbs[4].t = 8;

    curbs[5].x = 288;
    curbs[5].z = 2688;
    curbs[5].w = 4224;
    curbs[5].h = 4224;
    curbs[5].t = 8;

    curbs[6].x = 288;
    curbs[6].z = -2112;
    curbs[6].w = 4224;
    curbs[6].h = 4224;
    curbs[6].t = 8;

    curbs[7].x = 288;
    curbs[7].z = -6912;
    curbs[7].w = 4224;
    curbs[7].h = 4224;
    curbs[7].t = 8;

    curbs[8].x = 5088;
    curbs[8].z = -6912;
    curbs[8].w = 4224;
    curbs[8].h = 4224;
    curbs[8].t = 8;

    curbs[9].x = 5088;
    curbs[9].z = -2112;
    curbs[9].w = 4224;
    curbs[9].h = 4224;
    curbs[9].t = 8;

    curbs[10].x = 5088;
    curbs[10].z = 2688;
    curbs[10].w = 4224;
    curbs[10].h = 4224;
    curbs[10].t = 8;

    curbs[11].x = 9888;
    curbs[11].z = 5088;
    curbs[11].w = 4224;
    curbs[11].h = 1824;
    curbs[11].t = 8;

    curbs[12].x = 9888;
    curbs[12].z = 0;
    curbs[12].w = 4224;
    curbs[12].h = 4800;
    curbs[12].t = 8;

    curbs[13].x = 9888;
    curbs[13].z = -2112;
    curbs[13].w = 4224;
    curbs[13].h = 1824;
    curbs[13].t = 8;

    curbs[14].x = 9888;
    curbs[14].z = -4800;
    curbs[14].w = 4224;
    curbs[14].h = 2400;
    curbs[14].t = 8;

    curbs[15].x = 9888;
    curbs[15].z = -6912;
    curbs[15].w = 4224;
    curbs[15].h = 1824;
    curbs[15].t = 8;
}

void InitLevel5Curbs(void)
{
    numCurbs = 25;
    curbs[0].x = 192;
    curbs[0].z = 192;
    curbs[0].w = 2112;
    curbs[0].h = 4416;
    curbs[0].t = 8;

    curbs[1].x = 2496;
    curbs[1].z = 192;
    curbs[1].w = 2112;
    curbs[1].h = 4416;
    curbs[1].t = 8;

    curbs[2].x = 192;
    curbs[2].z = 4992;
    curbs[2].w = 4416;
    curbs[2].h = 2112;
    curbs[2].t = 8;

    curbs[3].x = 192;
    curbs[3].z = 7296;
    curbs[3].w = 4416;
    curbs[3].h = 2112;
    curbs[3].t = 8;

    curbs[4].x = 192;
    curbs[4].z = 9792;
    curbs[4].w = 2112;
    curbs[4].h = 4416;
    curbs[4].t = 8;

    curbs[5].x = 2496;
    curbs[5].z = 9792;
    curbs[5].w = 2112;
    curbs[5].h = 4416;
    curbs[5].t = 8;

    curbs[6].x = 4992;
    curbs[6].z = 192;
    curbs[6].w = 4416;
    curbs[6].h = 2112;
    curbs[6].t = 8;

    curbs[7].x = 4992;
    curbs[7].z = 2496;
    curbs[7].w = 4416;
    curbs[7].h = 2112;
    curbs[7].t = 8;

    curbs[8].x = 4992;
    curbs[8].z = 4992;
    curbs[8].w = 2112;
    curbs[8].h = 4416;
    curbs[8].t = 8;

    curbs[9].x = 7296;
    curbs[9].z = 4992;
    curbs[9].w = 2112;
    curbs[9].h = 4416;
    curbs[9].t = 8;

    curbs[10].x = 4992;
    curbs[10].z = 9792;
    curbs[10].w = 4416;
    curbs[10].h = 2112;
    curbs[10].t = 8;

    curbs[11].x = 4992;
    curbs[11].z = 12096;
    curbs[11].w = 4416;
    curbs[11].h = 2112;
    curbs[11].t = 8;

    curbs[12].x = 9792;
    curbs[12].z = 9792;
    curbs[12].w = 2112;
    curbs[12].h = 4416;
    curbs[12].t = 8;

    curbs[13].x = 12096;
    curbs[13].z = 9792;
    curbs[13].w = 2112;
    curbs[13].h = 4416;
    curbs[13].t = 8;

    curbs[14].x = 14592;
    curbs[14].z = 9792;
    curbs[14].w = 4416;
    curbs[14].h = 2112;
    curbs[14].t = 8;

    curbs[15].x = 14592;
    curbs[15].z = 12096;
    curbs[15].w = 4416;
    curbs[15].h = 2112;
    curbs[15].t = 8;

    curbs[16].x = 19392;
    curbs[16].z = 9792;
    curbs[16].w = 2112;
    curbs[16].h = 4416;
    curbs[16].t = 8;

    curbs[17].x = 21696;
    curbs[17].z = 9792;
    curbs[17].w = 2112;
    curbs[17].h = 4416;
    curbs[17].t = 8;

    curbs[18].x = 24192;
    curbs[18].z = 9792;
    curbs[18].w = 4416;
    curbs[18].h = 2112;
    curbs[18].t = 8;

    curbs[19].x = 24192;
    curbs[19].z = 12096;
    curbs[19].w = 4416;
    curbs[19].h = 2112;
    curbs[19].t = 8;

    curbs[20].x = 24192;
    curbs[20].z = 4992;
    curbs[20].w = 2112;
    curbs[20].h = 4416;
    curbs[20].t = 8;

    curbs[21].x = 26496;
    curbs[21].z = 4992;
    curbs[21].w = 2112;
    curbs[21].h = 4416;
    curbs[21].t = 8;

    curbs[22].x = 24192;
    curbs[22].z = 192;
    curbs[22].w = 4416;
    curbs[22].h = 2112;
    curbs[22].t = 8;

    curbs[23].x = 24192;
    curbs[23].z = 2496;
    curbs[23].w = 4416;
    curbs[23].h = 2112;
    curbs[23].t = 8;
}

void InitLevel6Curbs(void)
{
    numCurbs = 0;
}
