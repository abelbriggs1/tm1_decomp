#include "common.h"
#include "sdk/libgpu.h"

#define NUM_FONTS 7

extern SPRT gCharacterTemplate[NUM_FONTS];
extern int gFontSpacing[NUM_FONTS];
extern void* gFontInfo[NUM_FONTS];
extern u32 gFontCursorX;
extern u32 gFontCursorY;

INCLUDE_RODATA("asm/nonmatchings/tm1/font", D_800FA99C);

INCLUDE_RODATA("asm/nonmatchings/tm1/font", D_800FA9C0);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontPrintXY);

char fontMapUsefulCharacter(char value)
{
    if (value == '.') {
        return ';';
    }
    if (value == '!') {
        return '\\';
    }
    if (value == ',') {
        return '^';
    }
    if (value == '\'') {
        return '`';
    }
    if (value == '*') {
        return '{';
    }

    return value;
}

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontStringWidth);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontPrintCenteredXY);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontPrint);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSpritePrintXY);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSpritePrintCenteredXY);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSpritePrint);

// Uses gp-relative addressing
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontGetCursor);
// void fontGetCursor(u32* x, u32* y)
// {
//     *x = gFontCursorX;
//     *y = gFontCursorY;
// }

// Uses gp-relative addressing
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSetCursor);
// void fontSetCursor(u32 x, u32 y)
// {
//     gFontCursorX = x;
//     gFontCursorY = y;
// }

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontGetCharacterInfo);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontInit);

// Matches with -fforce-addr
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSetColor);
// void fontSetColor(int font, u8 red, u8 green, u8 blue)
// {
//     SPRT* fontData = &gCharacterTemplate[font];
//     fontData->r0 = red;
//     fontData->g0 = green;
//     fontData->b0 = blue;
// }

// Uses jump table in rodata
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSetDefaultColor);
