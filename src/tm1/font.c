#include "tm1/font.h"

#include "common.h"
#include <libgpu.h>

#define NUM_FONTS 7
#define NUM_FONTINFO_FIELDS 187

typedef struct {
    u8 unk1;
    u8 unk2;
    u8 width;
    u8 height;
    s16 unk5;
} FontInfoField;

typedef struct {
    u32 x;
    u32 y;
} Point;

// .data
extern Point gFontVramCoords[NUM_FONTS];
extern FontInfoField fontInfo1[NUM_FONTINFO_FIELDS];
extern FontInfoField fontInfo2[NUM_FONTINFO_FIELDS];
extern FontInfoField fontInfo3[10];
extern FontInfoField fontInfo4[NUM_FONTINFO_FIELDS];

// .bss
extern DR_MODE gFontDrawMode[NUM_FONTS];
extern SPRT gCharacterTemplate[NUM_FONTS];
extern int gFontSpacing[NUM_FONTS];
extern FontInfoField* gFontInfo[NUM_FONTS];

static u32 gFontCursorX = 0;
static u32 gFontCursorY = 0;

char fontMapUsefulCharacter(char value);

// TODO: If this isn't an inline, it must be a macro of some kind. This code is
// repeated often across the `font` TU.
// TODO: Rename this function once we understand what it's actually doing.
static inline int getFontInfoIndex(char c1)
{
    if (0x2F < fontMapUsefulCharacter(c1)) {
        if (fontMapUsefulCharacter(c1) < 0x7D) {
            return fontMapUsefulCharacter(c1) - 0x30;
        } else {
            return 0x2F;
        }
    } else {
        return 0x2F;
    }
}

void fontPrintXY(int font, u32 x, u32 y, char* string)
{
    gFontCursorX = x;
    gFontCursorY = y;
    fontPrint(font, string);
}

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

#ifdef NON_MATCHING
int fontStringWidth(int font, char* string)
{
    char cur_char;
    int width;

    cur_char = *string;
    width = 0;

    while (cur_char != '\0') {
        switch (cur_char) {
        case '\n':
            width = 0;
            break;
        case '\r':
            break;
        case ' ':
            width += gFontInfo[font][47].width + gFontSpacing[font];
            break;
        case '\b':
            width -= 1;
            break;
        default:
            width += gFontInfo[font][getFontInfoIndex(cur_char)].width + gFontSpacing[font];
            break;
        }
        string = string + 1;
        cur_char = *string;
    }

    return width;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontStringWidth);
#endif // NON_MATCHING

void fontPrintCenteredXY(int font, u32 base_x, u32 y, char* string)
{
    int len = fontStringWidth(font, string);
    gFontCursorX = base_x - (len >> 1);
    gFontCursorY = y;
    fontPrint(font, string);
}

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontPrint);
// int fontPrint(int font, char* string)
// {
//     char cur_char;
//     int useful_char;
//     int num_printed;
//     SPRT* char_template;
//     FontInfoField** font_info;

//     DrawSync(0);
//     DrawPrim(&gFontDrawMode[font]);

//     cur_char = *string;
//     num_printed = 0;

//     if (cur_char != '\0') {
//         while (cur_char != '\0') {
//             font_info = &gFontInfo[font];

//             switch (cur_char) {
//             case '\n':
//                 // Reset the cursor to the next line.
//                 gFontCursorY += (*font_info)[43].height;
//                 gFontCursorX = 0;
//                 break;
//             case '\r':
//                 gFontCursorY += (*font_info)[43].height;
//                 break;
//             case ' ':
//                 gFontCursorX += (*font_info)[47].width + gFontSpacing[font];
//                 break;
//             case '\b':
//                 gFontCursorX += -1;
//                 break;
//             default:
//                 useful_char = getFontInfoIndex(cur_char);
//                 DrawSync(0);
//                 char_template = &gCharacterTemplate[font];

//                 char_template->u0 = (*font_info)[useful_char].unk1 + gFontVramCoords[font].x;
//                 char_template->v0 = (*font_info)[useful_char].unk2 + gFontVramCoords[font].y;
//                 char_template->w = (*font_info)[useful_char].width;
//                 char_template->h = (*font_info)[useful_char].height;
//                 char_template->x0 = gFontCursorX;
//                 char_template->y0 = (gFontCursorY - (*font_info)[useful_char].height)
//                     + (*font_info)[useful_char].unk5;
//                 DrawPrim(char_template);

//                 gFontCursorX += (*font_info)[useful_char].width + gFontSpacing[font];
//                 break;
//             }

//             string = string + 1;
//             cur_char = *string;
//             num_printed += 1;
//         }
//     }

//     DrawSync(0);
//     return num_printed;
// }

void fontSpritePrintXY(int font, u32 x, u32 y, u32* unk4, u32* unk5, char* string, void* unk7)
{
    gFontCursorX = x;
    gFontCursorY = y;
    fontSpritePrint(font, unk4, unk5, string, unk7);
}

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSpritePrintCenteredXY);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSpritePrint);

#ifdef NON_MATCHING
void fontGetCursor(u32* x, u32* y)
{
    *x = gFontCursorX;
    *y = gFontCursorY;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontGetCursor);
#endif // NON_MATCHING

void fontSetCursor(u32 x, u32 y)
{
    gFontCursorX = x;
    gFontCursorY = y;
}

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontGetCharacterInfo);
// void fontGetCharacterInfo(
//     int font, char character, u32* width, u32* height, s32* out5, s32* spacing)
// {
//     int c2;

//     *spacing = gFontSpacing[font];

//     switch (character) {
//     case '\n':
//         *width = 0;
//         *height = gFontInfo[font][0x2B].height;
//         *out5 = 0;
//         break;
//     case '\r':
//         *width = 0;
//         *height = gFontInfo[font][0x2B].height;
//         *out5 = 0;
//         break;
//     case '\b':
//         *width = -1;
//         *height = 0;
//         *out5 = 0;
//         break;
//     case ' ':
//         *width = gFontInfo[font][0x2F].width;
//         *height = 0;
//         *out5 = 0;
//         break;
//     default:
//         c2 = getFontInfoIndex(character);
//         // TODO: Possible a fake match, regalloc is very slightly off without this.
//         // Permuter could not find any other reasonable solutions.
//         // This could be a macro of some kind, but it's hard to tell because there isn't
//         // anywhere else in the code that reads all 3 values into variables like this.
//         do {
//             *width = gFontInfo[font][c2].width;
//             *height = gFontInfo[font][c2].height;
//             *out5 = gFontInfo[font][c2].unk5;
//         } while (0);
//         break;
//     }
// }

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontInit);

#ifdef NON_MATCHING
void fontSetColor(int font, u8 red, u8 green, u8 blue)
{
    SPRT* fontData = &gCharacterTemplate[font];
    fontData->r0 = red;
    fontData->g0 = green;
    fontData->b0 = blue;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSetColor);
#endif // NON_MATCHING

INCLUDE_RODATA("asm/nonmatchings/tm1/font", D_800FA99C);

INCLUDE_RODATA("asm/nonmatchings/tm1/font", D_800FA9B0);

INCLUDE_RODATA("asm/nonmatchings/tm1/font", D_800FA9C0);

INCLUDE_ASM("asm/nonmatchings/tm1/font", fontSetDefaultColor);
// void fontSetDefaultColor(int font)
// {
//     switch (font) {
//     case 0:
//         gCharacterTemplate[font].r0 = 0x5D;
//         gCharacterTemplate[font].g0 = 0x1E;
//         gCharacterTemplate[font].b0 = 0x17;
//         break;
//     case 1:
//         gCharacterTemplate[font].r0 = 0x73;
//         gCharacterTemplate[font].g0 = 0x3D;
//         gCharacterTemplate[font].b0 = 0x1B;
//         break;
//     case 2:
//         gCharacterTemplate[font].r0 = 0x80;
//         gCharacterTemplate[font].g0 = 0x80;
//         gCharacterTemplate[font].b0 = 0x80;
//         break;
//     case 3:
//         gCharacterTemplate[font].r0 = 0x73;
//         gCharacterTemplate[font].g0 = 0x19;
//         gCharacterTemplate[font].b0 = 0x08;
//         break;
//     case 4:
//         gCharacterTemplate[font].r0 = 0x64;
//         gCharacterTemplate[font].g0 = 0x00;
//         gCharacterTemplate[font].b0 = 0x1E;
//         break;
//     case 5:
//         gCharacterTemplate[font].r0 = 0x32;
//         gCharacterTemplate[font].g0 = 0x32;
//         gCharacterTemplate[font].b0 = 0x32;
//         break;
//     case 6:
//         gCharacterTemplate[font].r0 = 0x73;
//         gCharacterTemplate[font].g0 = 0x54;
//         gCharacterTemplate[font].b0 = 0x23;
//         break;
//     default:
//         return;
//     }
// }
