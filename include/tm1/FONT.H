#ifndef __TM1_FONT_H__
#define __TM1_FONT_H__

#include "common.h"

void fontPrintXY(int font, u32 x, u32 y, char* string);
int fontStringWidth(int font, char* string);
void fontPrintCenteredXY(int font, u32 base_x, u32 y, char* string);
int fontPrint(int font, char* string);
void fontSpritePrintXY(int font, u32 x, u32 y, u32* unk4, u32* unk5, char* string, void* unk7);
void fontSpritePrintCenteredXY(
    int font, u32 base_x, u32 y, u32* unk4, u32* unk5, char* string, void* unk7);
int fontSpritePrint(int font, u32* unk2, u32* unk3, char* string, void* unk5);
void fontGetCursor(u32* x, u32* y);
void fontSetCursor(u32 x, u32 y);
void fontGetCharacterInfo(
    int font, char character, u32* width, u32* height, s32* out5, s32* spacing);
void fontInit();
void fontSetColor(int font, u8 red, u8 green, u8 blue);
void fontSetDefaultColor(int font);

#endif // __TM1_FONT_H__
