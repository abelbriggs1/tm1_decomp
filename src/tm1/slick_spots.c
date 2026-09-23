#include "common.h"

#include "tm1/car_update.h"

#include "tm1/slick_spots.h"

void InitLevel1SlickSpots(void)
{
    numSlickSpots = 0;
}

void InitLevel2SlickSpots(void)
{
    numSlickSpots = 0;
}

void InitLevel3SlickSpots(void)
{
    numSlickSpots = 0;
}

void InitLevel4SlickSpots(void)
{
    numSlickSpots = 1;
    slickSpots[0].x = 0x16A8;
    slickSpots[0].z = -768;
    slickSpots[0].w = 0x3F8;
    slickSpots[0].h = 0x6F8;
}

void InitLevel5SlickSpots(void)
{
    numSlickSpots = 0;
}

void InitLevel6SlickSpots(void)
{
    numSlickSpots = 0;
}
