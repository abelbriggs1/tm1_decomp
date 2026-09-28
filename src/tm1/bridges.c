#include "common.h"

#include "tm1/bridges.h"

s16 numBridges = 0;
s16 numCheckBridges = 0;

Bridge* GetBridgeDat(s16 which)
{
    return &bridges[which];
}

void InitLevel1Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 4;
    numCheckBridges = 4;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 0;
    bridges[0].minX = 0x270;
    bridges[0].minZ = 0x8B8;
    bridges[0].y0 = 0;
    bridges[0].y1 = 0x28;
    bridges[0].extentX = 0xA0;
    bridges[0].extentZ = 0xA8;
    bridges[0].obj[0] = 1;

    bridges[1].rampDir = 1;
    bridges[1].minX = 0x270;
    bridges[1].minZ = 0x960;
    bridges[1].y0 = 0x28;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0xA0;
    bridges[1].extentZ = 0xB0;
    bridges[1].obj[0] = 0;

    bridges[2].rampDir = 0;
    bridges[2].minX = 0x7D0;
    bridges[2].minZ = 0x910;
    bridges[2].y0 = 0;
    bridges[2].y1 = 0x50;
    bridges[2].extentX = 0xA0;
    bridges[2].extentZ = 0x140;

    bridges[3].rampDir = 3;
    bridges[3].minX = 0x960;
    bridges[3].minZ = 0x640;
    bridges[3].y0 = 0x50;
    bridges[3].y1 = 0;
    bridges[3].extentX = 0x140;
    bridges[3].extentZ = 0xA0;
}

void InitLevel2Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 0xC;
    numCheckBridges = 0xA;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 0;
    bridges[0].minX = -9600;
    bridges[0].minZ = -1516;
    bridges[0].y0 = 0;
    bridges[0].y1 = 0x30;
    bridges[0].extentX = 0x60;
    bridges[0].extentZ = 0xB0;
    bridges[0].obj[0] = 1;
    bridges[0].unk0A = 0x1E;

    bridges[1].rampDir = 1;
    bridges[1].minX = -9600;
    bridges[1].minZ = -1340;
    bridges[1].y0 = 0x30;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0x60;
    bridges[1].extentZ = 0xA8;
    bridges[1].obj[0] = 0;
    bridges[1].unk0A = 0x1E;

    bridges[2].rampDir = 0;
    bridges[2].minX = -9600;
    bridges[2].minZ = 0x194;
    bridges[2].y0 = 0;
    bridges[2].y1 = 0x30;
    bridges[2].extentX = 0x60;
    bridges[2].extentZ = 0xB0;
    bridges[2].obj[0] = 3;
    bridges[2].unk0A = 0x1E;

    bridges[3].rampDir = 1;
    bridges[3].minX = -9600;
    bridges[3].minZ = 0x244;
    bridges[3].y0 = 0x30;
    bridges[3].y1 = 0;
    bridges[3].extentX = 0x60;
    bridges[3].extentZ = 0xA8;
    bridges[3].obj[0] = 2;
    bridges[3].unk0A = 0x1E;

    bridges[4].rampDir = 2;
    bridges[4].minX = -8332;
    bridges[4].minZ = 0x8A0;
    bridges[4].y0 = 0;
    bridges[4].y1 = 0x30;
    bridges[4].extentX = 0xB0;
    bridges[4].extentZ = 0x60;
    bridges[4].obj[0] = 5;
    bridges[4].unk0A = 0x1E;

    bridges[5].rampDir = 3;
    bridges[5].minX = -8156;
    bridges[5].minZ = 0x8A0;
    bridges[5].y0 = 0x30;
    bridges[5].y1 = 0;
    bridges[5].extentX = 0xA8;
    bridges[5].extentZ = 0x60;
    bridges[5].obj[0] = 4;
    bridges[5].unk0A = 0x1E;

    bridges[6].rampDir = 0;
    bridges[6].minX = -12456;
    bridges[6].minZ = 0x1530;
    bridges[6].y0 = 8;
    bridges[6].y1 = 0x20;
    bridges[6].extentX = 0x90;
    bridges[6].extentZ = 0x48;
    bridges[6].obj[0] = 0xA;

    bridges[7].rampDir = 1;
    bridges[7].minX = -12456;
    bridges[7].minZ = 0x1608;
    bridges[7].y0 = 0x20;
    bridges[7].y1 = 8;
    bridges[7].extentX = 0x90;
    bridges[7].extentZ = 0x48;
    bridges[7].obj[0] = 0xA;

    bridges[8].rampDir = 2;
    bridges[8].minX = -8304;
    bridges[8].minZ = -5256;
    bridges[8].y0 = 8;
    bridges[8].y1 = 0x20;
    bridges[8].extentX = 0x48;
    bridges[8].extentZ = 0x90;
    bridges[8].obj[0] = 0xB;

    bridges[9].rampDir = 3;
    bridges[9].minX = -8088;
    bridges[9].minZ = -5256;
    bridges[9].y0 = 0x20;
    bridges[9].y1 = 8;
    bridges[9].extentX = 0x48;
    bridges[9].extentZ = 0x90;
    bridges[9].obj[0] = 0xB;

    bridges[10].rampDir = 0;
    bridges[10].minX = -12456;
    bridges[10].minZ = 0x1578;
    bridges[10].y0 = 0x20;
    bridges[10].y1 = 0x20;
    bridges[10].extentX = 0x90;
    bridges[10].extentZ = 0x90;
    bridges[10].obj[0] = 6;
    bridges[10].obj[1] = 7;
    bridges[10].unk08 = 1;

    bridges[11].rampDir = 2;
    bridges[11].minX = -8232;
    bridges[11].minZ = -5256;
    bridges[11].y0 = 0x20;
    bridges[11].y1 = 0x20;
    bridges[11].extentX = 0x90;
    bridges[11].extentZ = 0x90;
    bridges[11].obj[0] = 8;
    bridges[11].obj[1] = 9;
    bridges[11].unk08 = 2;
}

void InitLevel3Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 0x12;
    numCheckBridges = 0xC;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 0;
    bridges[0].minX = -17280;
    bridges[0].minZ = 0x228;
    bridges[0].y0 = 0;
    bridges[0].y1 = 0x188;
    bridges[0].extentX = 0x4E0;
    bridges[0].extentZ = 0x438;
    bridges[0].obj[0] = 0x10;
    bridges[0].unk0A = 0x1E;

    bridges[1].rampDir = 1;
    bridges[1].minX = -17280;
    bridges[1].minZ = 0x960;
    bridges[1].y0 = 0x188;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0x4E0;
    bridges[1].extentZ = 0x438;
    bridges[1].obj[0] = 0x10;
    bridges[1].unk0A = 0x1E;

    bridges[2].rampDir = 0;
    bridges[2].minX = 0x3EB0;
    bridges[2].minZ = 0x1F0;
    bridges[2].y0 = 0;
    bridges[2].y1 = 0xC0;
    bridges[2].extentX = 0x4E0;
    bridges[2].extentZ = 0x470;
    bridges[2].obj[0] = 0x11;
    bridges[2].unk0A = 0x1E;

    bridges[3].rampDir = 1;
    bridges[3].minX = 0x3EB0;
    bridges[3].minZ = 0x960;
    bridges[3].y0 = 0xC0;
    bridges[3].y1 = 0;
    bridges[3].extentX = 0x4E0;
    bridges[3].extentZ = 0x470;
    bridges[3].obj[0] = 0x11;
    bridges[3].unk0A = 0x1E;

    bridges[4].rampDir = 2;
    bridges[4].minX = 0x1230;
    bridges[4].minZ = 0x2000;
    bridges[4].y0 = 0;
    bridges[4].y1 = 0x18;
    bridges[4].extentX = 0x90;
    bridges[4].extentZ = 0x90;
    bridges[4].obj[0] = 0xC;

    bridges[5].rampDir = 3;
    bridges[5].minX = 0x1308;
    bridges[5].minZ = 0x2000;
    bridges[5].y0 = 0x18;
    bridges[5].y1 = 0;
    bridges[5].extentX = 0x90;
    bridges[5].extentZ = 0x90;
    bridges[5].obj[0] = 0xC;

    bridges[6].rampDir = 2;
    bridges[6].minX = -4944;
    bridges[6].minZ = 0x2000;
    bridges[6].y0 = 0;
    bridges[6].y1 = 0x18;
    bridges[6].extentX = 0x90;
    bridges[6].extentZ = 0x90;
    bridges[6].obj[0] = 0xD;

    bridges[7].rampDir = 3;
    bridges[7].minX = -4728;
    bridges[7].minZ = 0x2000;
    bridges[7].y0 = 0x18;
    bridges[7].y1 = 0;
    bridges[7].extentX = 0x90;
    bridges[7].extentZ = 0x90;
    bridges[7].obj[0] = 0xD;

    bridges[8].rampDir = 2;
    bridges[8].minX = 0x1230;
    bridges[8].minZ = -8336;
    bridges[8].y0 = 0;
    bridges[8].y1 = 0x18;
    bridges[8].extentX = 0x90;
    bridges[8].extentZ = 0x90;
    bridges[8].obj[0] = 0xE;

    bridges[9].rampDir = 3;
    bridges[9].minX = 0x1308;
    bridges[9].minZ = -8336;
    bridges[9].y0 = 0x18;
    bridges[9].y1 = 0;
    bridges[9].extentX = 0x90;
    bridges[9].extentZ = 0x90;
    bridges[9].obj[0] = 0xE;

    bridges[10].rampDir = 2;
    bridges[10].minX = -4944;
    bridges[10].minZ = -8336;
    bridges[10].y0 = 0;
    bridges[10].y1 = 0x18;
    bridges[10].extentX = 0x90;
    bridges[10].extentZ = 0x90;
    bridges[10].obj[0] = 0xF;

    bridges[11].rampDir = 3;
    bridges[11].minX = -4728;
    bridges[11].minZ = -8336;
    bridges[11].y0 = 0x18;
    bridges[11].y1 = 0;
    bridges[11].extentX = 0x90;
    bridges[11].extentZ = 0x90;
    bridges[11].obj[0] = 0xF;

    bridges[12].rampDir = 2;
    bridges[12].minX = 0x1278;
    bridges[12].minZ = 0x2000;
    bridges[12].y0 = 0x18;
    bridges[12].y1 = 0x18;
    bridges[12].extentX = 0x90;
    bridges[12].extentZ = 0x90;
    bridges[12].obj[0] = 4;
    bridges[12].obj[1] = 5;
    bridges[12].unk08 = 2;

    bridges[13].rampDir = 2;
    bridges[13].minX = -4872;
    bridges[13].minZ = 0x2000;
    bridges[13].y0 = 0x18;
    bridges[13].y1 = 0x18;
    bridges[13].extentX = 0x90;
    bridges[13].extentZ = 0x90;
    bridges[13].obj[0] = 6;
    bridges[13].obj[1] = 7;
    bridges[13].unk08 = 1;

    bridges[14].rampDir = 2;
    bridges[14].minX = 0x1278;
    bridges[14].minZ = -8336;
    bridges[14].y0 = 0x18;
    bridges[14].y1 = 0x18;
    bridges[14].extentX = 0x90;
    bridges[14].extentZ = 0x90;
    bridges[14].obj[0] = 8;
    bridges[14].obj[1] = 9;
    bridges[14].unk08 = 4;

    bridges[15].rampDir = 2;
    bridges[15].minX = -4872;
    bridges[15].minZ = -8336;
    bridges[15].y0 = 0x18;
    bridges[15].y1 = 0x18;
    bridges[15].extentX = 0x90;
    bridges[15].extentZ = 0x90;
    bridges[15].obj[0] = 0xA;
    bridges[15].obj[1] = 0xB;
    bridges[15].unk08 = 3;

    bridges[16].rampDir = 0;
    bridges[16].minX = -17280;
    bridges[16].minZ = 0x660;
    bridges[16].y0 = 0x188;
    bridges[16].y1 = 0x188;
    bridges[16].extentX = 0x4E0;
    bridges[16].extentZ = 0x300;
    bridges[16].obj[0] = 1;
    bridges[16].obj[1] = 0;

    bridges[17].rampDir = 0;
    bridges[17].minX = 0x3EB0;
    bridges[17].minZ = 0x660;
    bridges[17].y0 = 0xC0;
    bridges[17].y1 = 0xC0;
    bridges[17].extentX = 0x4E0;
    bridges[17].extentZ = 0x300;
    bridges[17].obj[0] = 3;
    bridges[17].obj[1] = 2;
}

#ifdef NON_MATCHING
void InitLevel4Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 0x36;
    numCheckBridges = 0x2A;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 2;
    bridges[0].minX = 0x1620;
    bridges[0].minZ = -2688;
    bridges[0].y0 = 0;
    bridges[0].y1 = 0x110;
    bridges[0].extentX = 0x320;
    bridges[0].extentZ = 0x240;
    bridges[0].obj[0] = 2;

    bridges[1].rampDir = 3;
    bridges[1].minX = 0x1F00;
    bridges[1].minZ = -2688;
    bridges[1].y0 = 0x110;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0x320;
    bridges[1].extentZ = 0x240;
    bridges[1].obj[0] = 2;

    bridges[2].rampDir = 2;
    bridges[2].minX = 0x1940;
    bridges[2].minZ = -2688;
    bridges[2].y0 = 0x110;
    bridges[2].y1 = 0x110;
    bridges[2].extentX = 0x5C0;
    bridges[2].extentZ = 0x240;
    bridges[2].obj[0] = 0;
    bridges[2].obj[1] = 1;

    bridges[3].rampDir = 2;
    bridges[3].minX = 0x1620;
    bridges[3].minZ = 0x840;
    bridges[3].y0 = 0;
    bridges[3].y1 = 0x110;
    bridges[3].extentX = 0x320;
    bridges[3].extentZ = 0x240;
    bridges[3].obj[0] = 5;

    bridges[4].rampDir = 3;
    bridges[4].minX = 0x1F00;
    bridges[4].minZ = 0x840;
    bridges[4].y0 = 0x110;
    bridges[4].y1 = 0;
    bridges[4].extentX = 0x320;
    bridges[4].extentZ = 0x240;
    bridges[4].obj[0] = 5;

    bridges[5].rampDir = 2;
    bridges[5].minX = 0x1940;
    bridges[5].minZ = 0x840;
    bridges[5].y0 = 0x110;
    bridges[5].y1 = 0x110;
    bridges[5].extentX = 0x5C0;
    bridges[5].extentZ = 0x240;
    bridges[5].obj[0] = 3;
    bridges[5].obj[1] = 4;

    bridges[6].rampDir = 2;
    bridges[6].minX = 0x1C20;
    bridges[6].minZ = -7488;
    bridges[6].y0 = -80;
    bridges[6].y1 = -80;
    bridges[6].extentX = 0x320;
    bridges[6].extentZ = 0x300;
    bridges[6].obj[0] = 0x2D;
    bridges[6].unk0A = 0;

    bridges[7].rampDir = 3;
    bridges[7].minX = 0x1A90;
    bridges[7].minZ = -5800;
    bridges[7].y0 = 8;
    bridges[7].y1 = -80;
    bridges[7].extentX = 0xA0;
    bridges[7].extentZ = 0x898;
    bridges[7].obj[0] = 0x2A;
    bridges[7].obj[1] = 0x2D;
    bridges[7].obj[2] = 0x2E;
    bridges[7].unk0A = 0x28;

    bridges[8].rampDir = 3;
    bridges[8].minX = 0x1D10;
    bridges[8].minZ = -5800;
    bridges[8].y0 = -80;
    bridges[8].y1 = 8;
    bridges[8].extentX = 0xA0;
    bridges[8].extentZ = 0x898;
    bridges[8].obj[0] = 0x2A;
    bridges[8].obj[1] = 0x2D;
    bridges[8].obj[2] = 0x2E;
    bridges[8].unk0A = 0x28;

    bridges[9].rampDir = 3;
    bridges[9].minX = 0x1B58;
    bridges[9].minZ = -2880;
    bridges[9].y0 = -80;
    bridges[9].y1 = -80;
    bridges[9].extentX = 0x320;
    bridges[9].extentZ = 0x3C0;
    bridges[9].obj[0] = 0x2E;
    bridges[9].obj[1] = 0x2F;
    bridges[9].unk0A = 0;

    bridges[10].rampDir = 2;
    bridges[10].minX = 0x1CE8;
    bridges[10].minZ = -1000;
    bridges[10].y0 = 8;
    bridges[10].y1 = -80;
    bridges[10].extentX = 0xA0;
    bridges[10].extentZ = 0x898;
    bridges[10].obj[0] = 0x2B;
    bridges[10].obj[1] = 0x2F;
    bridges[10].obj[2] = 0x30;
    bridges[10].unk0A = 0x28;

    bridges[11].rampDir = 3;
    bridges[11].minX = 0x1F68;
    bridges[11].minZ = -1000;
    bridges[11].y0 = -80;
    bridges[11].y1 = 8;
    bridges[11].extentX = 0xA0;
    bridges[11].extentZ = 0x898;
    bridges[11].obj[0] = 0x2B;
    bridges[11].obj[1] = 0x2F;
    bridges[11].obj[2] = 0x30;
    bridges[11].unk0A = 0x28;

    bridges[12].rampDir = 2;
    bridges[12].minX = 0x1C20;
    bridges[12].minZ = 0x780;
    bridges[12].y0 = -80;
    bridges[12].y1 = -80;
    bridges[12].extentX = 0x320;
    bridges[12].extentZ = 0x3C0;
    bridges[12].obj[0] = 0x30;
    bridges[12].obj[1] = 0x31;
    bridges[12].unk0A = 0;

    bridges[13].rampDir = 2;
    bridges[13].minX = 0x1A90;
    bridges[13].minZ = 0xED8;
    bridges[13].y0 = 8;
    bridges[13].y1 = -80;
    bridges[13].extentX = 0xA0;
    bridges[13].extentZ = 0x898;
    bridges[13].obj[0] = 0x2C;
    bridges[13].obj[1] = 0x31;
    bridges[13].obj[2] = 0x32;
    bridges[13].unk0A = 0x28;

    bridges[14].rampDir = 3;
    bridges[14].minX = 0x1D10;
    bridges[14].minZ = 0xED8;
    bridges[14].y0 = -80;
    bridges[14].y1 = 8;
    bridges[14].extentX = 0xA0;
    bridges[14].extentZ = 0x898;
    bridges[14].obj[0] = 0x2C;
    bridges[14].obj[1] = 0x31;
    bridges[14].obj[2] = 0x32;
    bridges[14].unk0A = 0x28;

    bridges[15].rampDir = 2;
    bridges[15].minX = 0x1B58;
    bridges[15].minZ = 0x1A40;
    bridges[15].y0 = -80;
    bridges[15].y1 = -80;
    bridges[15].extentX = 0x320;
    bridges[15].extentZ = 0x300;
    bridges[15].obj[0] = 0x32;
    bridges[15].unk0A = 0;

    bridges[16].rampDir = 2;
    bridges[16].minX = -1200;
    bridges[16].minZ = -456;
    bridges[16].y0 = 8;
    bridges[16].y1 = 0x20;
    bridges[16].extentX = 0x48;
    bridges[16].extentZ = 0x90;
    bridges[16].obj[0] = 0x33;

    bridges[17].rampDir = 3;
    bridges[17].minX = -984;
    bridges[17].minZ = -456;
    bridges[17].y0 = 0x20;
    bridges[17].y1 = 8;
    bridges[17].extentX = 0x48;
    bridges[17].extentZ = 0x90;
    bridges[17].obj[0] = 0x33;

    bridges[18].rampDir = 0;
    bridges[18].minX = 0x10F8;
    bridges[18].minZ = 0xE70;
    bridges[18].y0 = 8;
    bridges[18].y1 = 0x20;
    bridges[18].extentX = 0x90;
    bridges[18].extentZ = 0x48;
    bridges[18].obj[0] = 0x34;

    bridges[19].rampDir = 1;
    bridges[19].minX = 0x10F8;
    bridges[19].minZ = 0xF48;
    bridges[19].y0 = 0x20;
    bridges[19].y1 = 8;
    bridges[19].extentX = 0x90;
    bridges[19].extentZ = 0x48;
    bridges[19].obj[0] = 0x34;

    bridges[20].rampDir = 2;
    bridges[20].minX = 0x3210;
    bridges[20].minZ = -5256;
    bridges[20].y0 = 8;
    bridges[20].y1 = 0x20;
    bridges[20].extentX = 0x48;
    bridges[20].extentZ = 0x90;
    bridges[20].obj[0] = 0x35;

    bridges[21].rampDir = 3;
    bridges[21].minX = 0x32E8;
    bridges[21].minZ = -5256;
    bridges[21].y0 = 0x20;
    bridges[21].y1 = 8;
    bridges[21].extentX = 0x48;
    bridges[21].extentZ = 0x90;
    bridges[21].obj[0] = 0x35;

    bridges[22].rampDir = 0;
    bridges[22].minX = -9600;
    bridges[22].minZ = -1516;
    bridges[22].y0 = 0;
    bridges[22].y1 = 0x30;
    bridges[22].extentX = 0x60;
    bridges[22].extentZ = 0xB0;
    bridges[22].obj[0] = 0x17;
    bridges[22].unk0A = 0x1E;

    bridges[23].rampDir = 1;
    bridges[23].minX = -9600;
    bridges[23].minZ = -1340;
    bridges[23].y0 = 0x30;
    bridges[23].y1 = 0;
    bridges[23].extentX = 0x60;
    bridges[23].extentZ = 0xA8;
    bridges[23].obj[0] = 0x16;
    bridges[23].unk0A = 0x1E;

    bridges[24].rampDir = 0;
    bridges[24].minX = -9600;
    bridges[24].minZ = 0x194;
    bridges[24].y0 = 0;
    bridges[24].y1 = 0x30;
    bridges[24].extentX = 0x60;
    bridges[24].extentZ = 0xB0;
    bridges[24].obj[0] = 0x19;
    bridges[24].unk0A = 0x1E;

    bridges[25].rampDir = 1;
    bridges[25].minX = -9600;
    bridges[25].minZ = 0x244;
    bridges[25].y0 = 0x30;
    bridges[25].y1 = 0;
    bridges[25].extentX = 0x60;
    bridges[25].extentZ = 0xA8;
    bridges[25].obj[0] = 0x18;
    bridges[25].unk0A = 0x1E;

    bridges[26].rampDir = 2;
    bridges[26].minX = -3340;
    bridges[26].minZ = -288;
    bridges[26].y0 = 0;
    bridges[26].y1 = 0x30;
    bridges[26].extentX = 0xB0;
    bridges[26].extentZ = 0x60;
    bridges[26].obj[0] = 0x1B;
    bridges[26].unk0A = 0x1E;

    bridges[27].rampDir = 3;
    bridges[27].minX = -3164;
    bridges[27].minZ = -288;
    bridges[27].y0 = 0x30;
    bridges[27].y1 = 0;
    bridges[27].extentX = 0xA8;
    bridges[27].extentZ = 0x60;
    bridges[27].obj[0] = 0x1A;
    bridges[27].unk0A = 0x1E;

    bridges[28].rampDir = 2;
    bridges[28].minX = -2188;
    bridges[28].minZ = -192;
    bridges[28].y0 = 0;
    bridges[28].y1 = 0x30;
    bridges[28].extentX = 0xB0;
    bridges[28].extentZ = 0x60;
    bridges[28].obj[0] = 0x1D;
    bridges[28].unk0A = 0x1E;

    bridges[29].rampDir = 3;
    bridges[29].minX = -2012;
    bridges[29].minZ = -192;
    bridges[29].y0 = 0x30;
    bridges[29].y1 = 0;
    bridges[29].extentX = 0xA8;
    bridges[29].extentZ = 0x60;
    bridges[29].obj[0] = 0x1C;
    bridges[29].unk0A = 0x1E;

    bridges[30].rampDir = 0;
    bridges[30].minX = 0;
    bridges[30].minZ = -3628;
    bridges[30].y0 = 0;
    bridges[30].y1 = 0x30;
    bridges[30].extentX = 0x60;
    bridges[30].extentZ = 0xB0;
    bridges[30].obj[0] = 0x1F;
    bridges[30].unk0A = 0x1E;

    bridges[31].rampDir = 1;
    bridges[31].minX = 0;
    bridges[31].minZ = -3452;
    bridges[31].y0 = 0x30;
    bridges[31].y1 = 0;
    bridges[31].extentX = 0x60;
    bridges[31].extentZ = 0xA8;
    bridges[31].obj[0] = 0x1E;
    bridges[31].unk0A = 0x1E;

    bridges[32].rampDir = 2;
    bridges[32].minX = 0x374;
    bridges[32].minZ = -2400;
    bridges[32].y0 = 0;
    bridges[32].y1 = 0x30;
    bridges[32].extentX = 0xB0;
    bridges[32].extentZ = 0x60;
    bridges[32].obj[0] = 0x21;
    bridges[32].unk0A = 0x1E;

    bridges[33].rampDir = 3;
    bridges[33].minX = 0x424;
    bridges[33].minZ = -2400;
    bridges[33].y0 = 0x30;
    bridges[33].y1 = 0;
    bridges[33].extentX = 0xA8;
    bridges[33].extentZ = 0x60;
    bridges[33].obj[0] = 0x20;
    bridges[33].unk0A = 0x1E;

    bridges[34].rampDir = 2;
    bridges[34].minX = 0x4F4;
    bridges[34].minZ = 0x960;
    bridges[34].y0 = 0;
    bridges[34].y1 = 0x30;
    bridges[34].extentX = 0xB0;
    bridges[34].extentZ = 0x60;
    bridges[34].obj[0] = 0x23;
    bridges[34].unk0A = 0x1E;

    bridges[35].rampDir = 3;
    bridges[35].minX = 0x5A4;
    bridges[35].minZ = 0x960;
    bridges[35].y0 = 0x30;
    bridges[35].y1 = 0;
    bridges[35].extentX = 0xA8;
    bridges[35].extentZ = 0x60;
    bridges[35].obj[0] = 0x22;
    bridges[35].unk0A = 0x1E;

    bridges[36].rampDir = 2;
    bridges[36].minX = -8332;
    bridges[36].minZ = 0x8A0;
    bridges[36].y0 = 0;
    bridges[36].y1 = 0x30;
    bridges[36].extentX = 0xB0;
    bridges[36].extentZ = 0x60;
    bridges[36].obj[0] = 0x25;
    bridges[36].unk0A = 0x1E;

    bridges[37].rampDir = 3;
    bridges[37].minX = -8156;
    bridges[37].minZ = 0x8A0;
    bridges[37].y0 = 0x30;
    bridges[37].y1 = 0;
    bridges[37].extentX = 0xA8;
    bridges[37].extentZ = 0x60;
    bridges[37].obj[0] = 0x24;
    bridges[37].unk0A = 0x1E;

    bridges[38].rampDir = 2;
    bridges[38].minX = 0x1AB8;
    bridges[38].minZ = -7200;
    bridges[38].y0 = 0;
    bridges[38].y1 = 0x58;
    bridges[38].extentX = 0x140;
    bridges[38].extentZ = 0xA0;
    bridges[38].obj[0] = 0x27;
    bridges[38].unk0A = 0x1E;

    bridges[39].rampDir = 3;
    bridges[39].minX = 0x1F68;
    bridges[39].minZ = -7200;
    bridges[39].y0 = 0x58;
    bridges[39].y1 = 0;
    bridges[39].extentX = 0x140;
    bridges[39].extentZ = 0xA0;
    bridges[39].obj[0] = 0x26;
    bridges[39].unk0A = 0x1E;

    bridges[40].rampDir = 2;
    bridges[40].minX = 0x19F0;
    bridges[40].minZ = 0x1B80;
    bridges[40].y0 = 0;
    bridges[40].y1 = 0x58;
    bridges[40].extentX = 0x140;
    bridges[40].extentZ = 0xA0;
    bridges[40].obj[0] = 0x29;
    bridges[40].unk0A = 0x1E;

    bridges[41].rampDir = 3;
    bridges[41].minX = 0x1EA0;
    bridges[41].minZ = 0x1B80;
    bridges[41].y0 = 0x58;
    bridges[41].y1 = 0;
    bridges[41].extentX = 0x140;
    bridges[41].extentZ = 0xA0;
    bridges[41].obj[0] = 0x28;
    bridges[41].unk0A = 0x1E;

    bridges[42].rampDir = 2;
    bridges[42].minX = 0x1B30;
    bridges[42].minZ = -5800;
    bridges[42].y0 = -80;
    bridges[42].y1 = -80;
    bridges[42].extentX = 0x1E0;
    bridges[42].extentZ = 0x898;
    bridges[42].obj[0] = 7;
    bridges[42].obj[1] = 8;
    bridges[42].obj[2] = 0x2D;
    bridges[42].obj[3] = 0x2E;

    bridges[43].rampDir = 2;
    bridges[43].minX = 0x1D88;
    bridges[43].minZ = -1000;
    bridges[43].y0 = -80;
    bridges[43].y1 = -80;
    bridges[43].extentX = 0x1E0;
    bridges[43].extentZ = 0x898;
    bridges[43].obj[0] = 0xA;
    bridges[43].obj[1] = 0xB;
    bridges[43].obj[2] = 0x2F;
    bridges[43].obj[3] = 0x30;

    bridges[44].rampDir = 2;
    bridges[44].minX = 0x1B30;
    bridges[44].minZ = 0xED8;
    bridges[44].y0 = -80;
    bridges[44].y1 = -80;
    bridges[44].extentX = 0x1E0;
    bridges[44].extentZ = 0x898;
    bridges[44].obj[0] = 0xD;
    bridges[44].obj[1] = 0xE;
    bridges[44].obj[2] = 0x31;
    bridges[44].obj[3] = 0x32;

    bridges[45].rampDir = 2;
    bridges[45].minX = 0x1A90;
    bridges[45].minZ = -6720;
    bridges[45].y0 = -80;
    bridges[45].y1 = -80;
    bridges[45].extentX = 0x4B0;
    bridges[45].extentZ = 0x398;
    bridges[45].obj[0] = 6;
    bridges[45].obj[1] = 0x2A;
    bridges[45].unk0A = 0;

    bridges[46].rampDir = 2;
    bridges[46].minX = 0x1A90;
    bridges[46].minZ = -3600;
    bridges[46].y0 = -80;
    bridges[46].y1 = -80;
    bridges[46].extentX = 0x3E8;
    bridges[46].extentZ = 0x370;
    bridges[46].obj[0] = 9;
    bridges[46].obj[1] = 0x2A;
    bridges[46].unk0A = 0;

    bridges[47].rampDir = 2;
    bridges[47].minX = 0x1B58;
    bridges[47].minZ = -1920;
    bridges[47].y0 = -80;
    bridges[47].y1 = -80;
    bridges[47].extentX = 0x4B0;
    bridges[47].extentZ = 0x398;
    bridges[47].obj[0] = 9;
    bridges[47].obj[1] = 0x2B;
    bridges[47].unk0A = 0;

    bridges[48].rampDir = 2;
    bridges[48].minX = 0x1C20;
    bridges[48].minZ = 0x4B0;
    bridges[48].y0 = -80;
    bridges[48].y1 = -80;
    bridges[48].extentX = 0x3E8;
    bridges[48].extentZ = 0x2D0;
    bridges[48].obj[0] = 0xC;
    bridges[48].obj[1] = 0x2B;
    bridges[48].unk0A = 0;

    bridges[49].rampDir = 2;
    bridges[49].minX = 0x1A90;
    bridges[49].minZ = 0xB40;
    bridges[49].y0 = -80;
    bridges[49].y1 = -80;
    bridges[49].extentX = 0x4B0;
    bridges[49].extentZ = 0x398;
    bridges[49].obj[0] = 0xC;
    bridges[49].obj[1] = 0x2C;
    bridges[49].unk0A = 0;

    bridges[50].rampDir = 2;
    bridges[50].minX = 0x1A90;
    bridges[50].minZ = 0x1770;
    bridges[50].y0 = -80;
    bridges[50].y1 = -80;
    bridges[50].extentX = 0x3E8;
    bridges[50].extentZ = 0x4B0;
    bridges[50].obj[0] = 0xF;
    bridges[50].obj[1] = 0x2C;
    bridges[50].unk0A = 0;

    bridges[51].rampDir = 2;
    bridges[51].minX = -1128;
    bridges[51].minZ = -456;
    bridges[51].y0 = 0x20;
    bridges[51].y1 = 0x20;
    bridges[51].extentX = 0x90;
    bridges[51].extentZ = 0x90;
    bridges[51].obj[0] = 0x10;
    bridges[51].obj[1] = 0x11;
    bridges[51].unk08 = 1;

    bridges[52].rampDir = 0;
    bridges[52].minX = 0x10F8;
    bridges[52].minZ = 0xEB8;
    bridges[52].y0 = 0x20;
    bridges[52].y1 = 0x20;
    bridges[52].extentX = 0x90;
    bridges[52].extentZ = 0x90;
    bridges[52].obj[0] = 0x12;
    bridges[52].obj[1] = 0x13;
    bridges[52].unk08 = 2;

    bridges[53].rampDir = 2;
    bridges[53].minX = 0x3258;
    bridges[53].minZ = -5256;
    bridges[53].y0 = 0x20;
    bridges[53].y1 = 0x20;
    bridges[53].extentX = 0x90;
    bridges[53].extentZ = 0x90;
    bridges[53].obj[0] = 0x14;
    bridges[53].obj[1] = 0x15;
    bridges[53].unk08 = 3;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/bridges", InitLevel4Bridges);
#endif

#ifdef NON_MATCHING
void InitLevel5Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 0x3A;
    numCheckBridges = 0x38;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 1;
    bridges[0].minX = 0x3780;
    bridges[0].minZ = 0x22E0;
    bridges[0].y0 = 0x90;
    bridges[0].y1 = 0;
    bridges[0].extentX = 0x180;
    bridges[0].extentZ = 0x180;
    bridges[0].obj[0] = 0x29;

    bridges[1].rampDir = 1;
    bridges[1].minX = 0x4A40;
    bridges[1].minZ = 0x22E0;
    bridges[1].y0 = 0x90;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0x180;
    bridges[1].extentZ = 0x180;
    bridges[1].obj[0] = 0x2A;

    bridges[2].rampDir = 0;
    bridges[2].minX = 0x4A40;
    bridges[2].minZ = 0x120;
    bridges[2].y0 = 0;
    bridges[2].y1 = 0x90;
    bridges[2].extentX = 0x180;
    bridges[2].extentZ = 0x180;
    bridges[2].obj[0] = 0x2B;

    bridges[3].rampDir = 3;
    bridges[3].minX = 0x5B20;
    bridges[3].minZ = 0x1200;
    bridges[3].y0 = 0x90;
    bridges[3].y1 = 0;
    bridges[3].extentX = 0x180;
    bridges[3].extentZ = 0x180;
    bridges[3].obj[0] = 4;

    bridges[4].rampDir = 2;
    bridges[4].minX = 0x5A50;
    bridges[4].minZ = 0x1200;
    bridges[4].y0 = 0x78;
    bridges[4].y1 = 0x90;
    bridges[4].extentX = 0xD0;
    bridges[4].extentZ = 0x180;
    bridges[4].obj[0] = 3;
    bridges[4].obj[1] = 0x10;
    bridges[4].obj[2] = 0x11;
    bridges[4].obj[3] = 0x12;

    bridges[5].rampDir = 0;
    bridges[5].minX = 0x3780;
    bridges[5].minZ = 0x120;
    bridges[5].y0 = 0;
    bridges[5].y1 = 0x90;
    bridges[5].extentX = 0x180;
    bridges[5].extentZ = 0x180;
    bridges[5].obj[0] = 6;

    bridges[6].rampDir = 1;
    bridges[6].minX = 0x3780;
    bridges[6].minZ = 0x2A0;
    bridges[6].y0 = 0x90;
    bridges[6].y1 = 0x78;
    bridges[6].extentX = 0x180;
    bridges[6].extentZ = 0xD0;
    bridges[6].obj[0] = 5;
    bridges[6].obj[1] = 0x16;
    bridges[6].obj[2] = 0x17;
    bridges[6].obj[3] = 0x2E;

    bridges[7].rampDir = 2;
    bridges[7].minX = 0x26A0;
    bridges[7].minZ = 0x1200;
    bridges[7].y0 = 0;
    bridges[7].y1 = 0x90;
    bridges[7].extentX = 0x180;
    bridges[7].extentZ = 0x180;
    bridges[7].obj[0] = 8;

    bridges[8].rampDir = 3;
    bridges[8].minX = 0x2820;
    bridges[8].minZ = 0x1200;
    bridges[8].y0 = 0x90;
    bridges[8].y1 = 0x78;
    bridges[8].extentX = 0xD0;
    bridges[8].extentZ = 0x180;
    bridges[8].obj[0] = 7;
    bridges[8].obj[1] = 0xD;
    bridges[8].obj[2] = 0xE;
    bridges[8].obj[3] = 0xF;

    bridges[9].rampDir = 1;
    bridges[9].minX = 0x30C0;
    bridges[9].minZ = 0x1B28;
    bridges[9].y0 = 0x110;
    bridges[9].y1 = 0;
    bridges[9].extentX = 0x21C0;
    bridges[9].extentZ = 0x1D8;

    bridges[10].rampDir = 2;
    bridges[10].minX = 0x2E00;
    bridges[10].minZ = 0xB40;
    bridges[10].y0 = 0;
    bridges[10].y1 = 0x110;
    bridges[10].extentX = 0x1D8;
    bridges[10].extentZ = 0xF00;

    bridges[11].rampDir = 0;
    bridges[11].minX = 0x30C0;
    bridges[11].minZ = 0x880;
    bridges[11].y0 = 0;
    bridges[11].y1 = 0x110;
    bridges[11].extentX = 0x21C0;
    bridges[11].extentZ = 0x1D8;

    bridges[12].rampDir = 3;
    bridges[12].minX = 0x5368;
    bridges[12].minZ = 0xB40;
    bridges[12].y0 = 0x110;
    bridges[12].y1 = 0;
    bridges[12].extentX = 0x1D8;
    bridges[12].extentZ = 0xF00;

    bridges[13].rampDir = 3;
    bridges[13].minX = 0x27E8;
    bridges[13].minZ = 0x1380;
    bridges[13].y0 = 0x110;
    bridges[13].y1 = 0x78;
    bridges[13].extentX = 0x108;
    bridges[13].extentZ = 0xAE0;
    bridges[13].obj[0] = 8;
    bridges[13].obj[1] = 0xF;

    bridges[14].rampDir = 3;
    bridges[14].minX = 0x27E8;
    bridges[14].minZ = 0x720;
    bridges[14].y0 = 0x110;
    bridges[14].y1 = 0x78;
    bridges[14].extentX = 0x108;
    bridges[14].extentZ = 0xAE0;
    bridges[14].obj[0] = 8;
    bridges[14].obj[1] = 0xF;

    bridges[15].rampDir = 3;
    bridges[15].minX = 0x28F0;
    bridges[15].minZ = 0x720;
    bridges[15].y0 = 0x78;
    bridges[15].y1 = 0;
    bridges[15].extentX = 0xD0;
    bridges[15].extentZ = 0x1740;
    bridges[15].obj[0] = 8;
    bridges[15].obj[1] = 0xD;
    bridges[15].obj[2] = 0xE;

    bridges[16].rampDir = 3;
    bridges[16].minX = 0x5A50;
    bridges[16].minZ = 0x1380;
    bridges[16].y0 = 0x78;
    bridges[16].y1 = 0x110;
    bridges[16].extentX = 0x108;
    bridges[16].extentZ = 0xAE0;
    bridges[16].obj[0] = 4;
    bridges[16].obj[1] = 0x12;

    bridges[17].rampDir = 3;
    bridges[17].minX = 0x5A50;
    bridges[17].minZ = 0x720;
    bridges[17].y0 = 0x78;
    bridges[17].y1 = 0x110;
    bridges[17].extentX = 0x108;
    bridges[17].extentZ = 0xAE0;
    bridges[17].obj[0] = 4;
    bridges[17].obj[1] = 0x12;

    bridges[18].rampDir = 3;
    bridges[18].minX = 0x5980;
    bridges[18].minZ = 0x720;
    bridges[18].y0 = 0;
    bridges[18].y1 = 0x78;
    bridges[18].extentX = 0xD0;
    bridges[18].extentZ = 0x1740;
    bridges[18].obj[0] = 4;
    bridges[18].obj[1] = 0x10;
    bridges[18].obj[2] = 0x11;

    bridges[19].rampDir = 0;
    bridges[19].minX = 0x3900;
    bridges[19].minZ = 0x2210;
    bridges[19].y0 = 0x78;
    bridges[19].y1 = 0x110;
    bridges[19].extentX = 0x1140;
    bridges[19].extentZ = 0x108;
    bridges[19].obj[0] = 0x14;
    bridges[19].obj[1] = 0x15;
    bridges[19].obj[2] = 0x29;
    bridges[19].obj[3] = 0x2A;

    bridges[20].rampDir = 0;
    bridges[20].minX = 0x2CA0;
    bridges[20].minZ = 0x2140;
    bridges[20].y0 = 0;
    bridges[20].y1 = 0x78;
    bridges[20].extentX = 0x1440;
    bridges[20].extentZ = 0xD0;
    bridges[20].obj[0] = 0x13;
    bridges[20].obj[1] = 0x29;
    bridges[20].obj[2] = 0x2C;

    bridges[21].rampDir = 0;
    bridges[21].minX = 0x44A0;
    bridges[21].minZ = 0x2140;
    bridges[21].y0 = 0;
    bridges[21].y1 = 0x78;
    bridges[21].extentX = 0x1200;
    bridges[21].extentZ = 0xD0;
    bridges[21].obj[0] = 0x13;
    bridges[21].obj[1] = 0x2A;
    bridges[21].obj[2] = 0x2D;

    bridges[22].rampDir = 1;
    bridges[22].minX = 0x3900;
    bridges[22].minZ = 0x268;
    bridges[22].y0 = 0x110;
    bridges[22].y1 = 0x78;
    bridges[22].extentX = 0x1140;
    bridges[22].extentZ = 0x108;
    bridges[22].obj[0] = 6;
    bridges[22].obj[1] = 0x17;
    bridges[22].obj[2] = 0x18;
    bridges[22].obj[3] = 0x2B;

    bridges[23].rampDir = 1;
    bridges[23].minX = 0x2CA0;
    bridges[23].minZ = 0x370;
    bridges[23].y0 = 0x78;
    bridges[23].y1 = 0;
    bridges[23].extentX = 0x1200;
    bridges[23].extentZ = 0xD0;
    bridges[23].obj[0] = 6;
    bridges[23].obj[1] = 0x16;
    bridges[23].obj[2] = 0x2E;

    bridges[24].rampDir = 1;
    bridges[24].minX = 0x4260;
    bridges[24].minZ = 0x370;
    bridges[24].y0 = 0x78;
    bridges[24].y1 = 0;
    bridges[24].extentX = 0x1440;
    bridges[24].extentZ = 0xD0;
    bridges[24].obj[0] = 0x16;
    bridges[24].obj[1] = 0x2B;
    bridges[24].obj[2] = 0x2F;

    bridges[25].rampDir = 0;
    bridges[25].minX = 0x23C8;
    bridges[25].minZ = 0xC20;
    bridges[25].y0 = 8;
    bridges[25].y1 = 0x20;
    bridges[25].extentX = 0x90;
    bridges[25].extentZ = 0x48;
    bridges[25].obj[0] = 0x21;

    bridges[26].rampDir = 1;
    bridges[26].minX = 0x23C8;
    bridges[26].minZ = 0xCF8;
    bridges[26].y0 = 0x20;
    bridges[26].y1 = 8;
    bridges[26].extentX = 0x90;
    bridges[26].extentZ = 0x48;
    bridges[26].obj[0] = 0x21;

    bridges[27].rampDir = 2;
    bridges[27].minX = 0x250;
    bridges[27].minZ = 0x2690;
    bridges[27].y0 = 8;
    bridges[27].y1 = 0x20;
    bridges[27].extentX = 0x48;
    bridges[27].extentZ = 0x90;
    bridges[27].obj[0] = 0x22;

    bridges[28].rampDir = 3;
    bridges[28].minX = 0x328;
    bridges[28].minZ = 0x2690;
    bridges[28].y0 = 0x20;
    bridges[28].y1 = 8;
    bridges[28].extentX = 0x48;
    bridges[28].extentZ = 0x90;
    bridges[28].obj[0] = 0x22;

    bridges[29].rampDir = 2;
    bridges[29].minX = 0x6080;
    bridges[29].minZ = 0x120;
    bridges[29].y0 = 8;
    bridges[29].y1 = 0x20;
    bridges[29].extentX = 0x48;
    bridges[29].extentZ = 0x90;
    bridges[29].obj[0] = 0x23;

    bridges[30].rampDir = 3;
    bridges[30].minX = 0x6158;
    bridges[30].minZ = 0x120;
    bridges[30].y0 = 0x20;
    bridges[30].y1 = 8;
    bridges[30].extentX = 0x48;
    bridges[30].extentZ = 0x90;
    bridges[30].obj[0] = 0x23;

    bridges[30].rampDir = 2;
    bridges[30].minX = 0x5848;
    bridges[30].minZ = 0x34F8;
    bridges[30].y0 = 8;
    bridges[30].y1 = 0x20;
    bridges[30].extentX = 0x48;
    bridges[30].extentZ = 0x90;
    bridges[30].obj[0] = 0x24;

    bridges[32].rampDir = 3;
    bridges[32].minX = 0x5920;
    bridges[32].minZ = 0x34F8;
    bridges[32].y0 = 0x20;
    bridges[32].y1 = 8;
    bridges[32].extentX = 0x48;
    bridges[32].extentZ = 0x90;
    bridges[32].obj[0] = 0x24;

    bridges[33].rampDir = 0;
    bridges[33].minX = 0x23C8;
    bridges[33].minZ = 0xC68;
    bridges[33].y0 = 0x20;
    bridges[33].y1 = 0x20;
    bridges[33].extentX = 0x90;
    bridges[33].extentZ = 0x90;
    bridges[33].obj[0] = 0x1C;
    bridges[33].obj[1] = 0x1D;
    bridges[33].unk08 = 1;

    bridges[34].rampDir = 2;
    bridges[34].minX = 0x298;
    bridges[34].minZ = 0x2690;
    bridges[34].y0 = 0x20;
    bridges[34].y1 = 0x20;
    bridges[34].extentX = 0x90;
    bridges[34].extentZ = 0x90;
    bridges[34].obj[0] = 0x1E;
    bridges[34].obj[1] = 0x1F;
    bridges[34].unk08 = 2;

    bridges[35].rampDir = 2;
    bridges[35].minX = 0x60C8;
    bridges[35].minZ = 0x120;
    bridges[35].y0 = 0x20;
    bridges[35].y1 = 0x20;
    bridges[35].extentX = 0x90;
    bridges[35].extentZ = 0x90;
    bridges[35].obj[0] = 0x20;
    bridges[35].obj[1] = 0x21;
    bridges[35].unk08 = 3;

    bridges[36].rampDir = 2;
    bridges[36].minX = 0x5890;
    bridges[36].minZ = 0x34F8;
    bridges[36].y0 = 0x20;
    bridges[36].y1 = 0x20;
    bridges[36].extentX = 0x90;
    bridges[36].extentZ = 0x90;
    bridges[36].obj[0] = 0x22;
    bridges[36].obj[1] = 0x23;
    bridges[36].unk08 = 4;

    bridges[37].rampDir = 0;
    bridges[37].minX = -2656;
    bridges[37].minZ = 0x1E10;
    bridges[37].y0 = 0;
    bridges[37].y1 = 0xC0;
    bridges[37].extentX = 0x320;
    bridges[37].extentZ = 0x470;
    bridges[37].obj[0] = 0x38;

    bridges[38].rampDir = 1;
    bridges[38].minX = -2656;
    bridges[38].minZ = 0x2580;
    bridges[38].y0 = 0xC0;
    bridges[38].y1 = 0;
    bridges[38].extentX = 0x320;
    bridges[38].extentZ = 0x470;
    bridges[38].obj[0] = 0x38;

    bridges[39].rampDir = 0;
    bridges[39].minX = 0x77C0;
    bridges[39].minZ = 0x1E10;
    bridges[39].y0 = 0;
    bridges[39].y1 = 0xC0;
    bridges[39].extentX = 0x320;
    bridges[39].extentZ = 0x470;
    bridges[39].obj[0] = 0x39;

    bridges[40].rampDir = 1;
    bridges[40].minX = 0x77C0;
    bridges[40].minZ = 0x2580;
    bridges[40].y0 = 0xC0;
    bridges[40].y1 = 0;
    bridges[40].extentX = 0x320;
    bridges[40].extentZ = 0x470;
    bridges[40].obj[0] = 0x39;

    bridges[41].rampDir = 0;
    bridges[41].minX = 0x3780;
    bridges[41].minZ = 0x2210;
    bridges[41].y0 = 0x78;
    bridges[41].y1 = 0x90;
    bridges[41].extentX = 0x180;
    bridges[41].extentZ = 0xD0;
    bridges[41].obj[0] = 0;
    bridges[41].obj[1] = 0x13;
    bridges[41].obj[2] = 0x14;
    bridges[41].obj[3] = 0x2C;

    bridges[42].rampDir = 0;
    bridges[42].minX = 0x4A40;
    bridges[42].minZ = 0x2210;
    bridges[42].y0 = 0x78;
    bridges[42].y1 = 0x90;
    bridges[42].extentX = 0x180;
    bridges[42].extentZ = 0xD0;
    bridges[42].obj[0] = 1;
    bridges[42].obj[1] = 0x13;
    bridges[42].obj[2] = 0x15;
    bridges[42].obj[3] = 0x2D;

    bridges[43].rampDir = 1;
    bridges[43].minX = 0x4A40;
    bridges[43].minZ = 0x2A0;
    bridges[43].y0 = 0x90;
    bridges[43].y1 = 0x78;
    bridges[43].extentX = 0x180;
    bridges[43].extentZ = 0xD0;
    bridges[43].obj[0] = 2;
    bridges[43].obj[1] = 0x16;
    bridges[43].obj[2] = 0x18;
    bridges[43].obj[3] = 0x2F;

    bridges[44].rampDir = 0;
    bridges[44].minX = 0x2CA0;
    bridges[44].minZ = 0x2210;
    bridges[44].y0 = 0x78;
    bridges[44].y1 = 0x110;
    bridges[44].extentX = 0xAE0;
    bridges[44].extentZ = 0x108;
    bridges[44].obj[0] = 0x14;
    bridges[44].obj[1] = 0x29;

    bridges[45].rampDir = 0;
    bridges[45].minX = 0x4BC0;
    bridges[45].minZ = 0x2210;
    bridges[45].y0 = 0x78;
    bridges[45].y1 = 0x110;
    bridges[45].extentX = 0xAE0;
    bridges[45].extentZ = 0x108;
    bridges[45].obj[0] = 0x15;
    bridges[45].obj[1] = 0x2A;

    bridges[46].rampDir = 1;
    bridges[46].minX = 0x2CA0;
    bridges[46].minZ = 0x268;
    bridges[46].y0 = 0x110;
    bridges[46].y1 = 0x78;
    bridges[46].extentX = 0xAE0;
    bridges[46].extentZ = 0x108;
    bridges[46].obj[0] = 6;
    bridges[46].obj[1] = 0x17;

    bridges[47].rampDir = 1;
    bridges[47].minX = 0x4BC0;
    bridges[47].minZ = 0x268;
    bridges[47].y0 = 0x110;
    bridges[47].y1 = 0x78;
    bridges[47].extentX = 0xAE0;
    bridges[47].extentZ = 0x108;
    bridges[47].obj[0] = 0x18;
    bridges[47].obj[1] = 0x2B;

    bridges[48].rampDir = 0;
    bridges[48].minX = 0x2820;
    bridges[48].minZ = 0x1E60;
    bridges[48].y0 = 0xF0;
    bridges[48].y1 = 0xF0;
    bridges[48].extentX = 0x1A0;
    bridges[48].extentZ = 0x480;
    bridges[48].obj[0] = 0xD;
    bridges[48].obj[1] = 0xF;
    bridges[48].obj[2] = 0x31;

    bridges[49].rampDir = 3;
    bridges[49].minX = 0x2820;
    bridges[49].minZ = 0x2140;
    bridges[49].y0 = 0xF0;
    bridges[49].y1 = 0xF0;
    bridges[49].extentX = 0x480;
    bridges[49].extentZ = 0x1A0;
    bridges[49].obj[0] = 0x14;
    bridges[49].obj[1] = 0x2C;
    bridges[49].obj[2] = 0x30;

    bridges[50].rampDir = 0;
    bridges[50].minX = 0x5980;
    bridges[50].minZ = 0x1E60;
    bridges[50].y0 = 0xF0;
    bridges[50].y1 = 0xF0;
    bridges[50].extentX = 0x1A0;
    bridges[50].extentZ = 0x480;
    bridges[50].obj[0] = 0x10;
    bridges[50].obj[1] = 0x12;
    bridges[50].obj[2] = 0x33;

    bridges[51].rampDir = 3;
    bridges[51].minX = 0x56A0;
    bridges[51].minZ = 0x2140;
    bridges[51].y0 = 0xF0;
    bridges[51].y1 = 0xF0;
    bridges[51].extentX = 0x480;
    bridges[51].extentZ = 0x1A0;
    bridges[51].obj[0] = 0x15;
    bridges[51].obj[1] = 0x2D;
    bridges[51].obj[2] = 0x32;

    bridges[52].rampDir = 0;
    bridges[52].minX = 0x2820;
    bridges[52].minZ = 0x2A0;
    bridges[52].y0 = 0xF0;
    bridges[52].y1 = 0xF0;
    bridges[52].extentX = 0x1A0;
    bridges[52].extentZ = 0x480;
    bridges[52].obj[0] = 0xE;
    bridges[52].obj[1] = 0xF;
    bridges[52].obj[2] = 0x35;

    bridges[53].rampDir = 3;
    bridges[53].minX = 0x2820;
    bridges[53].minZ = 0x2A0;
    bridges[53].y0 = 0xF0;
    bridges[53].y1 = 0xF0;
    bridges[53].extentX = 0x480;
    bridges[53].extentZ = 0x1A0;
    bridges[53].obj[0] = 0x17;
    bridges[53].obj[1] = 0x2E;
    bridges[53].obj[2] = 0x34;

    bridges[54].rampDir = 0;
    bridges[54].minX = 0x5980;
    bridges[54].minZ = 0x2A0;
    bridges[54].y0 = 0xF0;
    bridges[54].y1 = 0xF0;
    bridges[54].extentX = 0x1A0;
    bridges[54].extentZ = 0x480;
    bridges[54].obj[0] = 0x11;
    bridges[54].obj[1] = 0x12;
    bridges[54].obj[2] = 0x37;

    bridges[55].rampDir = 3;
    bridges[55].minX = 0x56A0;
    bridges[55].minZ = 0x2A0;
    bridges[55].y0 = 0xF0;
    bridges[55].y1 = 0xF0;
    bridges[55].extentX = 0x480;
    bridges[55].extentZ = 0x1A0;
    bridges[55].obj[0] = 0x18;
    bridges[55].obj[1] = 0x2F;
    bridges[55].obj[2] = 0x36;

    bridges[56].rampDir = 0;
    bridges[56].minX = -2656;
    bridges[56].minZ = 0x2280;
    bridges[56].y0 = 0xC0;
    bridges[56].y1 = 0xC0;
    bridges[56].extentX = 0x320;
    bridges[56].extentZ = 0x300;
    bridges[56].obj[0] = 0x25;
    bridges[56].obj[1] = 0x26;

    bridges[57].rampDir = 0;
    bridges[57].minX = 0x77C0;
    bridges[57].minZ = 0x2280;
    bridges[57].y0 = 0xC0;
    bridges[57].y1 = 0xC0;
    bridges[57].extentX = 0x320;
    bridges[57].extentZ = 0x300;
    bridges[57].obj[0] = 0x27;
    bridges[57].obj[1] = 0x28;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/bridges", InitLevel5Bridges);
#endif

void InitLevel6Bridges(void)
{
    s16 i;
    s16 j;

    numBridges = 0xB;
    numCheckBridges = 0xA;

    for (i = 0; i <= numBridges; i++) {
        bridges[i].unk08 = 0;
        bridges[i].unk0A = 10;
        for (j = 0; j < 4; j++) {
            bridges[i].obj[j] = -1;
        }
    }

    bridges[0].rampDir = 3;
    bridges[0].minX = 0x6E1;
    bridges[0].minZ = 0x2080;
    bridges[0].y0 = 0x1E0;
    bridges[0].y1 = 0;
    bridges[0].extentX = 0x960;
    bridges[0].extentZ = 0x190;

    bridges[1].rampDir = 1;
    bridges[1].minX = 0x1DB0;
    bridges[1].minZ = 0x1900;
    bridges[1].y0 = 0x320;
    bridges[1].y1 = 0;
    bridges[1].extentX = 0x190;
    bridges[1].extentZ = 0xC80;

    bridges[2].rampDir = 2;
    bridges[2].minX = 0x2A30;
    bridges[2].minZ = 0x7D0;
    bridges[2].y0 = 0;
    bridges[2].y1 = 0xF0;
    bridges[2].extentX = 0x4B0;
    bridges[2].extentZ = 0x190;

    bridges[3].rampDir = 0;
    bridges[3].minX = 0x3840;
    bridges[3].minZ = 0x4AF;
    bridges[3].y0 = 0;
    bridges[3].y1 = 0xF0;
    bridges[3].extentX = 0x190;
    bridges[3].extentZ = 0x4B0;

    bridges[4].rampDir = 2;
    bridges[4].minX = 0x29A0;
    bridges[4].minZ = 0x1080;
    bridges[4].y0 = 0;
    bridges[4].y1 = 0x18;
    bridges[4].extentX = 0x48;
    bridges[4].extentZ = 0x90;
    bridges[4].obj[0] = 0xA;

    bridges[5].rampDir = 3;
    bridges[5].minX = 0x2A78;
    bridges[5].minZ = 0x1080;
    bridges[5].y0 = 0x18;
    bridges[5].y1 = 0;
    bridges[5].extentX = 0x48;
    bridges[5].extentZ = 0x90;
    bridges[5].obj[0] = 0xA;

    bridges[6].rampDir = 3;
    bridges[6].minX = 0x1638;
    bridges[6].minZ = 0xB18;
    bridges[6].y0 = 0x28;
    bridges[6].y1 = 0;
    bridges[6].extentX = 0xD0;
    bridges[6].extentZ = 0xA0;
    bridges[6].obj[0] = 7;

    bridges[7].rampDir = 2;
    bridges[7].minX = 0x1608;
    bridges[7].minZ = 0xB18;
    bridges[7].y0 = 0;
    bridges[7].y1 = 0x28;
    bridges[7].extentX = 0x30;
    bridges[7].extentZ = 0xA0;
    bridges[7].obj[0] = 6;

    bridges[8].rampDir = 3;
    bridges[8].minX = 0x1660;
    bridges[8].minZ = 0x1E0;
    bridges[8].y0 = 0x30;
    bridges[8].y1 = 0;
    bridges[8].extentX = 0xD0;
    bridges[8].extentZ = 0xA0;
    bridges[8].obj[0] = 9;

    bridges[9].rampDir = 2;
    bridges[9].minX = 0x1630;
    bridges[9].minZ = 0x1E0;
    bridges[9].y0 = 0;
    bridges[9].y1 = 0x30;
    bridges[9].extentX = 0x30;
    bridges[9].extentZ = 0xA0;
    bridges[9].obj[0] = 8;

    bridges[10].rampDir = 2;
    bridges[10].minX = 0x29E8;
    bridges[10].minZ = 0x1080;
    bridges[10].y0 = 0x18;
    bridges[10].y1 = 0x18;
    bridges[10].extentX = 0x90;
    bridges[10].extentZ = 0x90;
    bridges[10].obj[0] = 4;
    bridges[10].obj[1] = 5;
    bridges[10].unk08 = 1;
}
