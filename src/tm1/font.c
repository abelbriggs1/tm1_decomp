#include "tm1/font.h"

#include "common.h"
#include <libgpu.h>

#define NUM_FONTS 7
#define NUM_FONTINFO_FIELDS 77

typedef struct {
    u8 u;
    u8 v;
    u8 width;
    u8 height;
    s16 yOffset;
} FontInfoField;

typedef struct {
    u32 x;
    u32 y;
} Point;

static Point gFontVramCoords[NUM_FONTS] = {
    { 0, 32 },
    { 0, 32 },
    { 0, 160 },
    { 0, 160 },
    { 0, 32 },
    { 0, 32 },
    { 0, 32 },
};
static FontInfoField fontInfo1[NUM_FONTINFO_FIELDS] = {
    { 2, 2, 14, 12, 0 }, /* '0' */
    { 16, 14, 7, 12, 0 }, /* '1' */
    { 21, 2, 14, 12, 0 }, /* '2' */
    { 34, 2, 14, 12, 0 }, /* '3' */
    { 47, 2, 13, 12, 0 }, /* '4' */
    { 59, 2, 14, 12, 0 }, /* '5' */
    { 72, 2, 14, 12, 0 }, /* '6' */
    { 85, 2, 12, 12, 0 }, /* '7' */
    { 96, 2, 14, 12, 0 }, /* '8' */
    { 109, 2, 14, 12, 0 }, /* '9' */
    { 41, 40, 6, 10, 0 }, /* ':' */
    { 42, 44, 6, 6, 0 }, /* '.' */
    { 5, 95, 15, 15, 0 }, /* '<' */
    { 23, 95, 15, 15, 0 }, /* '=' */
    { 58, 95, 15, 15, 0 }, /* '>' */
    { 54, 38, 10, 12, 0 }, /* '?' */
    { 41, 95, 15, 15, 0 }, /* '@' */
    { 2, 14, 14, 12, 0 }, /* 'A' */
    { 140, 2, 14, 12, 0 }, /* 'B' */
    { 28, 14, 14, 12, 0 }, /* 'C' */
    { 41, 14, 14, 12, 0 }, /* 'D' */
    { 135, 40, 13, 12, 0 }, /* 'E' */
    { 66, 14, 13, 12, 0 }, /* 'F' */
    { 78, 14, 14, 12, 0 }, /* 'G' */
    { 91, 14, 14, 12, 0 }, /* 'H' */
    { 104, 14, 6, 12, 0 }, /* 'I' */
    { 109, 14, 13, 12, 0 }, /* 'J' */
    { 121, 14, 13, 12, 0 }, /* 'K' */
    { 124, 55, 13, 12, 0 }, /* 'L' */
    { 2, 26, 15, 12, 0 }, /* 'M' */
    { 137, 55, 14, 12, 0 }, /* 'N' */
    { 29, 26, 14, 12, 0 }, /* 'O' */
    { 42, 26, 14, 12, 0 }, /* 'P' */
    { 55, 26, 14, 13, 1 }, /* 'Q' */
    { 68, 26, 14, 12, 0 }, /* 'R' */
    { 81, 26, 14, 12, 0 }, /* 'S' */
    { 94, 26, 14, 12, 0 }, /* 'T' */
    { 126, 2, 14, 12, 0 }, /* 'U' */
    { 120, 26, 14, 12, 0 }, /* 'V' */
    { 133, 26, 18, 12, 0 }, /* 'W' */
    { 2, 38, 14, 12, 0 }, /* 'X' */
    { 120, 40, 14, 12, 0 }, /* 'Y' */
    { 28, 38, 14, 12, 0 }, /* 'Z' */
    { 79, 38, 7, 16, 2 }, /* '[' */
    { 48, 38, 6, 12, 0 }, /* '!' */
    { 85, 38, 7, 16, 2 }, /* ']' */
    { 64, 44, 6, 8, 1 }, /* ',' */
    { 42, 49, 14, 6, 5 }, /* '_' */
    { 71, 38, 6, 8, -5 }, /* '\'' */
    { 1, 55, 13, 12, 0 }, /* 'a' */
    { 13, 55, 12, 12, 0 }, /* 'b' */
    { 24, 55, 12, 12, 0 }, /* 'c' */
    { 35, 55, 12, 12, 0 }, /* 'd' */
    { 37, 79, 11, 12, 0 }, /* 'e' */
    { 56, 55, 11, 12, 0 }, /* 'f' */
    { 66, 55, 12, 12, 0 }, /* 'g' */
    { 77, 55, 12, 12, 0 }, /* 'h' */
    { 88, 55, 6, 12, 0 }, /* 'i' */
    { 93, 55, 11, 12, 0 }, /* 'j' */
    { 103, 55, 11, 12, 0 }, /* 'k' */
    { 113, 55, 11, 12, 0 }, /* 'l' */
    { 2, 67, 13, 12, 0 }, /* 'm' */
    { 49, 79, 12, 12, 0 }, /* 'n' */
    { 25, 67, 12, 12, 0 }, /* 'o' */
    { 36, 67, 12, 12, 0 }, /* 'p' */
    { 64, 79, 12, 13, 1 }, /* 'q' */
    { 58, 67, 12, 12, 0 }, /* 'r' */
    { 69, 67, 12, 12, 0 }, /* 's' */
    { 80, 67, 12, 12, 0 }, /* 't' */
    { 91, 67, 12, 12, 0 }, /* 'u' */
    { 102, 67, 12, 12, 0 }, /* 'v' */
    { 113, 67, 16, 12, 0 }, /* 'w' */
    { 2, 79, 12, 12, 0 }, /* 'x' */
    { 13, 79, 12, 12, 0 }, /* 'y' */
    { 24, 79, 11, 12, 0 }, /* 'z' */
    { 80, 92, 8, 20, 2 }, /* '*' */
    { 88, 92, 8, 20, 2 }, /* '|' */
};
static FontInfoField fontInfo2[NUM_FONTINFO_FIELDS] = {
    { 210, 58, 14, 19, 0 }, /* '0' */
    { 224, 58, 9, 19, 0 }, /* '1' */
    { 236, 58, 14, 19, 0 }, /* '2' */
    { 153, 77, 14, 19, 0 }, /* '3' */
    { 166, 77, 14, 19, 0 }, /* '4' */
    { 181, 77, 14, 19, 0 }, /* '5' */
    { 194, 77, 14, 19, 0 }, /* '6' */
    { 207, 77, 14, 19, 0 }, /* '7' */
    { 220, 77, 14, 19, 0 }, /* '8' */
    { 233, 77, 14, 19, 0 }, /* '9' */
    { 41, 40, 6, 10, 0 }, /* ':' */
    { 42, 44, 6, 6, 0 }, /* '.' */
    { 101, 82, 13, 8, 0 }, /* '<' */
    { 138, 98, 12, 3, -12 }, /* '=' */
    { 58, 95, 15, 15, 0 }, /* '>' */
    { 138, 77, 14, 19, 0 }, /* '?' */
    { 115, 79, 12, 12, 0 }, /* '@' */
    { 153, 1, 14, 19, 0 }, /* 'A' */
    { 166, 1, 14, 19, 0 }, /* 'B' */
    { 179, 1, 14, 19, 0 }, /* 'C' */
    { 192, 1, 14, 19, 0 }, /* 'D' */
    { 205, 1, 13, 19, 0 }, /* 'E' */
    { 217, 1, 13, 19, 0 }, /* 'F' */
    { 229, 1, 14, 19, 0 }, /* 'G' */
    { 152, 21, 14, 19, 0 }, /* 'H' */
    { 166, 21, 7, 19, 0 }, /* 'I' */
    { 173, 21, 14, 19, 0 }, /* 'J' */
    { 187, 21, 16, 19, 0 }, /* 'K' */
    { 202, 21, 13, 19, 0 }, /* 'L' */
    { 214, 21, 21, 19, 0 }, /* 'M' */
    { 234, 21, 16, 19, 0 }, /* 'N' */
    { 150, 39, 14, 19, 0 }, /* 'O' */
    { 163, 39, 14, 19, 0 }, /* 'P' */
    { 176, 39, 14, 19, 0 }, /* 'Q' */
    { 189, 39, 14, 19, 0 }, /* 'R' */
    { 202, 39, 13, 19, 0 }, /* 'S' */
    { 214, 39, 13, 19, 0 }, /* 'T' */
    { 226, 39, 14, 19, 0 }, /* 'U' */
    { 239, 39, 14, 19, 0 }, /* 'V' */
    { 151, 58, 21, 19, 0 }, /* 'W' */
    { 171, 58, 14, 19, 0 }, /* 'X' */
    { 184, 58, 13, 19, 0 }, /* 'Y' */
    { 197, 58, 14, 19, 0 }, /* 'Z' */
    { 79, 38, 7, 16, 2 }, /* '[' */
    { 247, 77, 7, 19, 0 }, /* '!' */
    { 85, 38, 7, 16, 2 }, /* ']' */
    { 64, 44, 6, 8, 1 }, /* ',' */
    { 92, 42, 12, 6, -3 }, /* '_' */
    { 71, 38, 6, 8, -11 }, /* '\'' */
    { 1, 55, 13, 12, 0 }, /* 'a' */
    { 13, 55, 12, 12, 0 }, /* 'b' */
    { 24, 55, 12, 12, 0 }, /* 'c' */
    { 35, 55, 12, 12, 0 }, /* 'd' */
    { 37, 79, 11, 12, 0 }, /* 'e' */
    { 56, 55, 11, 12, 0 }, /* 'f' */
    { 66, 55, 12, 12, 0 }, /* 'g' */
    { 77, 55, 12, 12, 0 }, /* 'h' */
    { 88, 55, 6, 12, 0 }, /* 'i' */
    { 93, 55, 11, 12, 0 }, /* 'j' */
    { 103, 55, 11, 12, 0 }, /* 'k' */
    { 113, 55, 11, 12, 0 }, /* 'l' */
    { 2, 67, 13, 12, 0 }, /* 'm' */
    { 49, 79, 12, 12, 0 }, /* 'n' */
    { 25, 67, 12, 12, 0 }, /* 'o' */
    { 36, 67, 12, 12, 0 }, /* 'p' */
    { 64, 79, 12, 13, 1 }, /* 'q' */
    { 58, 67, 12, 12, 0 }, /* 'r' */
    { 69, 67, 12, 12, 0 }, /* 's' */
    { 80, 67, 12, 12, 0 }, /* 't' */
    { 91, 67, 12, 12, 0 }, /* 'u' */
    { 102, 67, 12, 12, 0 }, /* 'v' */
    { 113, 67, 16, 12, 0 }, /* 'w' */
    { 2, 79, 12, 12, 0 }, /* 'x' */
    { 13, 79, 12, 12, 0 }, /* 'y' */
    { 24, 79, 11, 12, 0 }, /* 'z' */
    { 81, 93, 8, 18, 1 }, /* '*' */
    { 75, 93, 8, 18, 1 }, /* '|' */
};
static FontInfoField fontInfo3[10] = {
    { 136, 111, 9, 14, 0 }, /* '0' */
    { 148, 111, 9, 14, 0 }, /* '1' */
    { 160, 111, 9, 14, 0 }, /* '2' */
    { 172, 111, 9, 14, 0 }, /* '3' */
    { 184, 111, 9, 14, 0 }, /* '4' */
    { 196, 111, 9, 14, 0 }, /* '5' */
    { 208, 111, 9, 14, 0 }, /* '6' */
    { 220, 111, 9, 14, 0 }, /* '7' */
    { 232, 111, 9, 14, 0 }, /* '8' */
    { 244, 111, 9, 14, 0 }, /* '9' */
};
static FontInfoField fontInfo4[NUM_FONTINFO_FIELDS] = {
    { 0, 0, 7, 9, 0 }, /* '0' */
    { 9, 0, 4, 9, 0 }, /* '1' */
    { 16, 0, 7, 9, 0 }, /* '2' */
    { 24, 0, 7, 9, 0 }, /* '3' */
    { 32, 0, 7, 9, 0 }, /* '4' */
    { 40, 0, 6, 9, 0 }, /* '5' */
    { 48, 0, 7, 9, 0 }, /* '6' */
    { 56, 0, 7, 9, 0 }, /* '7' */
    { 64, 0, 7, 9, 0 }, /* '8' */
    { 72, 0, 7, 9, 0 }, /* '9' */
    { 81, 2, 3, 7, 0 }, /* ':' */
    { 81, 6, 3, 3, 0 }, /* '.' */
    { 90, 38, 18, 18, 3 }, /* '<' */
    { 47, 45, 6, 3, -2 }, /* '=' */
    { 109, 38, 18, 18, 3 }, /* '>' */
    { 109, 0, 6, 9, 0 }, /* '?' */
    { 58, 42, 18, 9, 0 }, /* '@' */
    { 0, 14, 9, 9, 0 }, /* 'A' */
    { 10, 14, 7, 9, 0 }, /* 'B' */
    { 19, 14, 7, 9, 0 }, /* 'C' */
    { 27, 14, 8, 9, 0 }, /* 'D' */
    { 36, 14, 6, 9, 0 }, /* 'E' */
    { 44, 14, 6, 9, 0 }, /* 'F' */
    { 52, 14, 8, 9, 0 }, /* 'G' */
    { 62, 14, 8, 9, 0 }, /* 'H' */
    { 71, 14, 3, 9, 0 }, /* 'I' */
    { 75, 14, 5, 9, 0 }, /* 'J' */
    { 82, 14, 8, 9, 0 }, /* 'K' */
    { 91, 14, 6, 9, 0 }, /* 'L' */
    { 99, 14, 11, 9, 0 }, /* 'M' */
    { 0, 28, 7, 9, 0 }, /* 'N' */
    { 8, 28, 9, 9, 0 }, /* 'O' */
    { 18, 28, 7, 9, 0 }, /* 'P' */
    { 27, 28, 9, 11, 2 }, /* 'Q' */
    { 37, 28, 7, 9, 0 }, /* 'R' */
    { 46, 28, 6, 9, 0 }, /* 'S' */
    { 53, 28, 7, 9, 0 }, /* 'T' */
    { 61, 28, 8, 9, 0 }, /* 'U' */
    { 70, 28, 9, 9, 0 }, /* 'V' */
    { 80, 28, 12, 9, 0 }, /* 'W' */
    { 93, 28, 8, 9, 0 }, /* 'X' */
    { 102, 28, 9, 9, 0 }, /* 'Y' */
    { 112, 28, 7, 9, 0 }, /* 'Z' */
    { 0, 41, 5, 11, 1 }, /* '[' */
    { 37, 41, 3, 10, 0 }, /* '!' */
    { 11, 41, 5, 11, 1 }, /* ']' */
    { 85, 6, 3, 5, 2 }, /* ',' */
    { 22, 50, 7, 2, 1 }, /* '_' */
    { 85, 6, 3, 5, -6 }, /* '\'' */
    { 0, 58, 7, 7, 0 }, /* 'a' */
    { 8, 56, 7, 9, 0 }, /* 'b' */
    { 16, 58, 6, 7, 0 }, /* 'c' */
    { 24, 56, 7, 9, 0 }, /* 'd' */
    { 32, 58, 7, 7, 0 }, /* 'e' */
    { 40, 56, 6, 9, 0 }, /* 'f' */
    { 47, 58, 7, 9, 2 }, /* 'g' */
    { 55, 56, 7, 9, 0 }, /* 'h' */
    { 63, 56, 3, 9, 0 }, /* 'i' */
    { 66, 56, 5, 11, 2 }, /* 'j' */
    { 72, 56, 7, 9, 0 }, /* 'k' */
    { 80, 56, 3, 9, 0 }, /* 'l' */
    { 84, 58, 11, 7, 0 }, /* 'm' */
    { 0, 72, 7, 7, 0 }, /* 'n' */
    { 8, 72, 7, 7, 0 }, /* 'o' */
    { 17, 72, 7, 9, 2 }, /* 'p' */
    { 25, 72, 7, 9, 2 }, /* 'q' */
    { 33, 72, 5, 7, 0 }, /* 'r' */
    { 40, 72, 5, 7, 0 }, /* 's' */
    { 47, 71, 5, 8, 0 }, /* 't' */
    { 54, 72, 7, 7, 0 }, /* 'u' */
    { 62, 72, 7, 7, 0 }, /* 'v' */
    { 71, 72, 11, 7, 0 }, /* 'w' */
    { 83, 72, 8, 7, 0 }, /* 'x' */
    { 92, 72, 8, 9, 2 }, /* 'y' */
    { 101, 72, 6, 7, 0 }, /* 'z' */
    { 0, 0, 0, 0, 0 }, /* '*' */
    { 0, 0, 0, 0, 0 }, /* '|' */
};

static DR_MODE gFontDrawMode[NUM_FONTS];
static SPRT gCharacterTemplate[NUM_FONTS];
static int gFontSpacing[NUM_FONTS];
static FontInfoField* gFontInfo[NUM_FONTS];

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

//                 char_template->u0 = (*font_info)[useful_char].u + gFontVramCoords[font].x;
//                 char_template->v0 = (*font_info)[useful_char].v + gFontVramCoords[font].y;
//                 char_template->w = (*font_info)[useful_char].width;
//                 char_template->h = (*font_info)[useful_char].height;
//                 char_template->x0 = gFontCursorX;
//                 char_template->y0 = (gFontCursorY - (*font_info)[useful_char].height)
//                     + (*font_info)[useful_char].yOffset;
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
//             *out5 = gFontInfo[font][c2].yOffset;
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
