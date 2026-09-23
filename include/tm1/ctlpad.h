#ifndef __TM1_CTLPAD_H__
#define __TM1_CTLPAD_H__

#include "common.h"

/* Both controllers packed into one word: PadRead()'s return value.
   half[0] = pad 2 (low 16 bits), half[1] = pad 1 (high 16 bits). */
typedef union CtlPadStatus {
    u32 all;
    u16 half[2];
} CtlPadStatus;

void InitCtlPad(void);
void TermCtlPad(void);
void UpdCtlPad(void);
u32 GetPadStatus(s16 which);
void SetPadStatus(s16 which, u32 value);
u8 GetCtlPad(s16 button, s16 which);
s32 ctlpadSpecial(s32 button, s16 which);
u8 ctlpadAnyKey(s32 which);
u8 ctlpadAnyUp(s32 which);
u8 ctlpadAnyDown(s32 which);
u8 ctlpadAnyLeft(s32 which);
u8 ctlpadAnyRight(s32 which);
u8 ctlpadAnySelect(s32 which);
s32 GetJoyStick(void);

#endif
