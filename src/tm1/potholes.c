#include "common.h"

#include "tm1/car_update.h"

#include "tm1/potholes.h"

// TODO: Fix up the loops in this file and remove `do while`
// barriers if possible.

PotHole* GetPotHoleDat(s16 index)
{
    return &potHoles[index];
}

void InitLevel1PotHoles()
{
    s16 i;
    s16 t;
    s32 n;

    numPotHoles = 4;
    numCheckPotHoles = 4;
    i = 0;
    do {
        potHoles[i].active = 0;
        t = i + 1;
        do {
        } while (0);
        n = numPotHoles;
        i = t;
    } while (t < n);

    potHoles[0].x = 0x3C8;
    potHoles[0].z = 0x6D8;
    potHoles[0].a = 0x50;
    potHoles[0].b = 0xA0;
    potHoles[0].c = 0x18;

    potHoles[1].x = 0x438;
    potHoles[1].z = 0xC08;
    potHoles[1].a = 0xA0;
    potHoles[1].b = 0xA0;
    potHoles[1].c = 0x18;

    potHoles[2].x = 0x910;
    potHoles[2].z = 0x370;
    potHoles[2].a = 0x78;
    potHoles[2].b = 0x80;
    potHoles[2].c = 0x18;

    potHoles[3].x = 0xED8;
    potHoles[3].z = 0x870;
    potHoles[3].a = 0xA0;
    potHoles[3].b = 0xB8;
    potHoles[3].c = 0x18;
}

void InitLevel2PotHoles()
{
    s16 i;
    s16 t;
    s32 n;

    numPotHoles = 3;
    numCheckPotHoles = 3;
    i = 0;
    do {
        potHoles[i].active = 0;
        t = i + 1;
        do {
        } while (0);
        n = numPotHoles;
        i = t;
    } while (t < n);

    potHoles[0].x = -13928;
    potHoles[0].z = -6728;
    potHoles[0].a = 0x38;
    potHoles[0].b = 0xF00;
    potHoles[0].c = 0x18;

    potHoles[1].x = -13928;
    potHoles[1].z = -6728;
    potHoles[1].a = 0xF00;
    potHoles[1].b = 0x38;
    potHoles[1].c = 0x18;

    potHoles[2].x = -10128;
    potHoles[2].z = -6728;
    potHoles[2].a = 0x38;
    potHoles[2].b = 0x600;
    potHoles[2].c = 0x18;
}

void InitLevel3PotHoles()
{
    s16 i;
    s16 t;
    s32 n;

    numPotHoles = 2;
    numCheckPotHoles = 2;
    i = 0;
    do {
        potHoles[i].active = 0;
        t = i + 1;
        do {
        } while (0);
        n = numPotHoles;
        i = t;
    } while (t < n);

    potHoles[0].x = -16944;
    potHoles[0].z = 0x720;
    potHoles[0].a = 0x240;
    potHoles[0].b = 0x180;
    potHoles[0].c = -16;

    potHoles[1].x = 0x3FF0;
    potHoles[1].z = 0x720;
    potHoles[1].a = 0x240;
    potHoles[1].b = 0x180;
    potHoles[1].c = -16;
}

void InitLevel4PotHoles()
{
    s16 i;
    s16 t;
    s32 n;

    numPotHoles = 1;
    numCheckPotHoles = 1;
    i = 0;
    do {
        potHoles[i].active = 0;
        t = i + 1;
        do {
        } while (0);
        n = numPotHoles;
        i = t;
    } while (t < n);

    potHoles[0].x = 0xC0;
    potHoles[0].z = 0xC0;
    potHoles[0].a = 0x1140;
    potHoles[0].b = 0x840;
    potHoles[0].c = 8;
    potHoles[0].active = 1;
}

void InitLevel5PotHoles()
{
    s16 i;
    s16 t;
    s32 n;

    numPotHoles = 2;
    numCheckPotHoles = 2;
    i = 0;
    do {
        potHoles[i].active = 0;
        t = i + 1;
        do {
        } while (0);
        n = numPotHoles;
        i = t;
    } while (t < n);

    potHoles[0].x = -2656;
    potHoles[0].z = 0x23A0;
    potHoles[0].a = 0x320;
    potHoles[0].b = 0xC0;
    potHoles[0].c = -16;

    potHoles[1].x = 0x77C0;
    potHoles[1].z = 0x23A0;
    potHoles[1].a = 0x320;
    potHoles[1].b = 0xC0;
    potHoles[1].c = -16;
}

void InitLevel6PotHoles()
{
    numPotHoles = 0;
    numCheckPotHoles = 0;
}
