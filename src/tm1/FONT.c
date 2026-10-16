#include "tm1/FONT.H"

#include "common.h"
#include <LIBGPU.H>
#include <LIBGS.H>

#include "tm1/FILEIO.H"
#include "tm1/GRUTILS.H"
#include "tm1/RT.H"

#define NUM_FONTS 7
#define NUM_FONTINFO_FIELDS 77
#define STANDARD_WIDTH(font) (gFontInfo[(font)][47].width)
#define STANDARD_HEIGHT(font) (gFontInfo[(font)][43].height)
#define GET_GLYPH_METRICS(font_id, glyph_index, out_width, out_height, out_y_offset)               \
    do {                                                                                           \
        *(out_width) = gFontInfo[(font_id)][(glyph_index)].width;                                  \
        *(out_height) = gFontInfo[(font_id)][(glyph_index)].height;                                \
        *(out_y_offset) = gFontInfo[(font_id)][(glyph_index)].yOffset;                             \
    } while (0)

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

typedef struct {
    char* name;
    s32 loaded;
    s32 tpage;
    s32 clut;
} FontTexture;

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
            width += STANDARD_WIDTH(font) + gFontSpacing[font];
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

void fontPrintCenteredXY(int font, u32 base_x, u32 y, char* string)
{
    int len = fontStringWidth(font, string);
    gFontCursorX = base_x - (len >> 1);
    gFontCursorY = y;
    fontPrint(font, string);
}

int fontPrint(int font, char* string)
{
    char cur_char;
    int useful_char;
    int num_printed;
    SPRT* char_template;

    DrawSync(0);
    DrawPrim(&gFontDrawMode[font]);

    cur_char = *string;
    num_printed = 0;

    if (cur_char != '\0') {
        while (cur_char != '\0') {
            switch (cur_char) {
            case '\n':
                gFontCursorX = 0;
                gFontCursorY += STANDARD_HEIGHT(font);
                break;
            case '\r':
                gFontCursorY += STANDARD_HEIGHT(font);
                break;
            case ' ':
                gFontCursorX += STANDARD_WIDTH(font) + gFontSpacing[font];
                break;
            case '\b':
                gFontCursorX += -1;
                break;
            default:
                useful_char = getFontInfoIndex(cur_char);
                DrawSync(0);
                char_template = &gCharacterTemplate[font];

                char_template->u0 = gFontInfo[font][useful_char].u + gFontVramCoords[font].x;
                char_template->v0 = gFontInfo[font][useful_char].v + gFontVramCoords[font].y;
                char_template->w = gFontInfo[font][useful_char].width;
                char_template->h = gFontInfo[font][useful_char].height;
                char_template->x0 = gFontCursorX;
                char_template->y0 = (gFontCursorY - gFontInfo[font][useful_char].height)
                    + gFontInfo[font][useful_char].yOffset;
                DrawPrim(char_template);

                gFontCursorX += gFontInfo[font][useful_char].width + gFontSpacing[font];
                break;
            }

            string = string + 1;
            cur_char = *string;
            num_printed += 1;
        }
    }

    DrawSync(0);
    return num_printed;
}

void fontSpritePrintXY(int font, u32 x, u32 y, u32* drawMode, u32* sprites, char* string, void* ot)
{
    gFontCursorX = x;
    gFontCursorY = y;
    fontSpritePrint(font, drawMode, sprites, string, ot);
}

void fontSpritePrintCenteredXY(
    int font, u32 base_x, u32 y, u32* drawMode, u32* sprites, char* string, void* ot)
{
    char cur_char;
    char* p;

    gFontCursorX = 0;
    gFontCursorY = 0;
    cur_char = *string;
    p = string;
    while (cur_char != '\0') {
        switch (cur_char) {
        case '\n':
            gFontCursorX = 0;
            gFontCursorY += STANDARD_HEIGHT(font);
            break;
        case '\r':
            gFontCursorY += STANDARD_HEIGHT(font);
            break;
        case ' ':
            gFontCursorX += STANDARD_WIDTH(font) + gFontSpacing[font];
            break;
        case '\b':
            gFontCursorX -= 1;
            break;
        default:
            gFontCursorX += gFontInfo[font][getFontInfoIndex(cur_char)].width + gFontSpacing[font];
            break;
        }
        p++;
        cur_char = *p;
    }
    gFontCursorX = base_x - ((s32)gFontCursorX >> 1);
    gFontCursorY = y;
    fontSpritePrint(font, drawMode, sprites, string, ot);
}

int fontSpritePrint(int font, u32* drawMode, u32* sprites, char* string, void* ot)
{
    char cur_char;
    int useful_char;
    int num_printed;
    SPRT* sprite;

    __builtin_memcpy(drawMode, &gFontDrawMode[font], sizeof(DR_MODE));
    cur_char = *string;
    num_printed = 0;
    if (cur_char != '\0') {
        while (cur_char != '\0') {
            sprite = &((SPRT*)sprites)[num_printed];
            switch (cur_char) {
            case '\n':
                gFontCursorX = 0;
                gFontCursorY += STANDARD_HEIGHT(font);
                break;
            case '\r':
                gFontCursorY += STANDARD_HEIGHT(font);
                break;
            case ' ':
                gFontCursorX += STANDARD_WIDTH(font) + gFontSpacing[font];
                break;
            case '\b':
                gFontCursorX -= 1;
                break;
            default:
                useful_char = getFontInfoIndex(cur_char);
                __builtin_memcpy(sprite, &gCharacterTemplate[font], sizeof(SPRT));
                sprite->u0 = gFontInfo[font][useful_char].u + gFontVramCoords[font].x;
                sprite->v0 = gFontInfo[font][useful_char].v + gFontVramCoords[font].y;
                sprite->w = gFontInfo[font][useful_char].width;
                sprite->h = gFontInfo[font][useful_char].height;
                sprite->x0 = gFontCursorX;
                sprite->y0 = (gFontCursorY - gFontInfo[font][useful_char].height)
                    + gFontInfo[font][useful_char].yOffset;
                AddPrim(ot, sprite);
                gFontCursorX += gFontInfo[font][useful_char].width + gFontSpacing[font];
                break;
            }
            string++;
            cur_char = *string;
            num_printed++;
        }
    }
    AddPrim(ot, drawMode);
    return num_printed;
}

void fontGetCursor(u32* x, u32* y)
{
    *x = gFontCursorX;
    *y = gFontCursorY;
}

void fontSetCursor(u32 x, u32 y)
{
    gFontCursorX = x;
    gFontCursorY = y;
}

void fontGetCharacterInfo(
    int font, char character, u32* width, u32* height, s32* yOffset, s32* spacing)
{
    *spacing = gFontSpacing[font];

    switch (character) {
    case '\n':
        *width = 0;
        *height = STANDARD_HEIGHT(font);
        *yOffset = 0;
        break;
    case '\r':
        *width = 0;
        *height = STANDARD_HEIGHT(font);
        *yOffset = 0;
        break;
    case '\b':
        *width = -1;
        *height = 0;
        *yOffset = 0;
        break;
    case ' ':
        *width = STANDARD_WIDTH(font);
        *height = 0;
        *yOffset = 0;
        break;
    default: {
        int index = getFontInfoIndex(character);
        GET_GLYPH_METRICS(font, index, width, height, yOffset);
        break;
    }
    }
}

void fontInit(void)
{
    s32 i;
    RECT rect;
    GsIMAGE image;
    FontTexture textures[2] = {
        { "UATIM\\UAFONT1.TIM", 0, -1, -1 },
        { "TIM\\CPFONT.TIM", 0, -1, -1 },
    };
    FontTexture* fonts[NUM_FONTS] = {
        &textures[0],
        &textures[0],
        &textures[1],
        &textures[1],
        &textures[0],
        &textures[0],
        &textures[0],
    };

    gFontCursorX = 0;
    gFontCursorY = 0;
    gFontInfo[0] = fontInfo1;
    gFontSpacing[0] = -1;
    gFontInfo[1] = fontInfo1;
    gFontSpacing[1] = -1;
    gFontInfo[4] = fontInfo3;
    gFontSpacing[4] = 3;
    gFontInfo[5] = fontInfo1;
    gFontSpacing[5] = 7;
    gFontInfo[6] = fontInfo2;
    gFontSpacing[6] = -1;
    gFontInfo[3] = fontInfo4;
    gFontSpacing[3] = 0;
    gFontInfo[2] = fontInfo4;
    gFontSpacing[2] = 0;

    for (i = 0; i < NUM_FONTS; i++) {
        if (!fonts[i]->loaded) {
            fileioLoadFileIntoRam(rtGetDoubleBufferArea(), fonts[i]->name);
            GsGetTimInfo((u_long*)(rtGetDoubleBufferArea() + 4), &image);
            rect.x = gFontVramCoords[i].x + 960;
            rect.y = gFontVramCoords[i].y + 256;
            rect.w = image.pw;
            rect.h = image.ph;
            LoadImage(&rect, image.pixel);
            fonts[i]->tpage = GetTPage(image.pmode, 0, rect.x, rect.y);
            if (image.clut != NULL) {
                fonts[i]->clut = Load4BitClut((u32*)image.clut, 960, 511 - i);
            }
            fonts[i]->loaded = 1;
        }
        setSprt(&gCharacterTemplate[i]);
        if (fonts[i]->clut != -1) {
            gCharacterTemplate[i].clut = fonts[i]->clut;
        } else {
            gCharacterTemplate[i].clut = 0;
        }
        fontSetDefaultColor(i);
        SetShadeTex(&gCharacterTemplate[i], 0);
        setUV0(&gCharacterTemplate[i], 0, 0);
        SetDrawMode(&gFontDrawMode[i], 0, 0, fonts[i]->tpage, NULL);
        DrawSync(0);
    }
}

void fontSetColor(int font, u8 red, u8 green, u8 blue)
{
    setRGB0(&gCharacterTemplate[font], red, green, blue);
}

void fontSetDefaultColor(int font)
{
    SPRT* sprite = &gCharacterTemplate[font];

    switch (font) {
    case 0:
        setRGB0(sprite, 0x5D, 0x1E, 0x17); /* #5D1E17 — dark brick red */
        break;
    case 1:
        setRGB0(sprite, 0x73, 0x3D, 0x1B); /* #733D1B — brown */
        break;
    case 3:
        setRGB0(sprite, 0x73, 0x19, 0x08); /* #731908 — dark reddish brown */
        break;
    case 2:
        setRGB0(sprite, 0x80, 0x80, 0x80); /* #808080 — gray */
        break;
    case 4:
        setRGB0(sprite, 0x64, 0x00, 0x1E); /* #64001E — dark burgundy */
        break;
    case 6:
        setRGB0(sprite, 0x73, 0x54, 0x23); /* #735423 — golden brown */
        break;
    case 5:
        setRGB0(sprite, 0x32, 0x32, 0x32); /* #323232 — dark gray */
        break;
    default:
        return;
    }
}
