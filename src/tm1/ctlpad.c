#include "common.h"

#include "sdk/libetc.h"

#include "tm1/ctlpad.h"

static CtlPadStatus gCtlPad = { 0 };

void InitCtlPad(void)
{
    PadInit(0);
}

void TermCtlPad(void)
{
    PadStop();
}

void UpdCtlPad(void)
{
    gCtlPad.all = PadRead(1);
}

u32 GetPadStatus(s16 which)
{
    if (which == 1) {
        return gCtlPad.half[0];
    }
    if (which == 0) {
        return gCtlPad.all & 0xFFFF0000;
    }
    return gCtlPad.all;
}

void SetPadStatus(s16 which, u32 value)
{
    if (which == 1) {
        gCtlPad.all = value | (gCtlPad.all & 0xFFFF0000);
        return;
    }
    if (which == 0) {
        gCtlPad.all = value | gCtlPad.half[0];
        return;
    }
    gCtlPad.all = value;
}

u8 GetCtlPad(s16 button, s16 which)
{
    s32 down;
    u32 v;
    s16 idx;

    down = 0;
    if (which == 2) {
        if (GetCtlPad(button, 1) || GetCtlPad(button, 0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        idx = button - 1;
        switch (idx) {
        case 1:
            down = (v >> 0xB) & 1;
            break;
        case 0:
            down = (v >> 8) & 1;
            break;
        case 2:
            down = (v >> 2) & 1;
            break;
        case 3:
            down = v & 1;
            break;
        case 4:
            down = (v >> 0xC) & 1;
            break;
        case 5:
            down = (v >> 0xE) & 1;
            break;
        case 6:
            down = (v >> 0xF) & 1;
            break;
        case 7:
            down = (v >> 0xD) & 1;
            break;
        case 8:
            down = (v >> 3) & 1;
            break;
        case 9:
            down = (v >> 1) & 1;
            break;
        case 10:
            down = (v >> 4) & 1;
            break;
        case 11:
            down = (v >> 6) & 1;
            break;
        case 12:
            down = (v >> 7) & 1;
            break;
        case 13:
            down = (v >> 5) & 1;
            break;
        }
    }
    return down;
}

s32 ctlpadSpecial(s32 button, s16 which)
{
    s32 match;
    u32 v;
    s16 idx;

    match = 0;
    if (which == 0) {
        v = gCtlPad.all;
    } else {
        v = gCtlPad.half[1];
    }
    idx = button - 1;
    switch (idx) {
    case 0:
        if (v == 0x180B) {
            match = 1;
        }
        break;
    case 1:
        if (v == 0x30C9) {
            match = 1;
        }
        break;
    case 2:
        if (v == 0xC036) {
            match = 1;
        }
        break;
    case 3:
        if (v == 0xC035) {
            match = 1;
        }
        break;
    case 4:
        if (v == 0x30CA) {
            match = 1;
        }
        break;
    case 5:
        if (v == 0x3000) {
            match = 1;
        }
        break;
    }
    return match;
}

u8 ctlpadAnyKey(s32 which)
{
    u32 v;

    if (which == 0) {
        v = gCtlPad.half[0];
    } else if (which == 1) {
        v = gCtlPad.half[1];
    } else {
        v = gCtlPad.all;
    }
    return v != 0;
}

u8 ctlpadAnyUp(s32 which)
{
    u32 down;
    u32 v;

    if (which == 2) {
        down = 0;
        if (ctlpadAnyUp(1) || ctlpadAnyUp(0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        down = v >> 0xC;
        down &= 1;
    }
    return down;
}

u8 ctlpadAnyDown(s32 which)
{
    u32 down;
    u32 v;

    if (which == 2) {
        down = 0;
        if (ctlpadAnyDown(1) || ctlpadAnyDown(0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        down = v >> 0xE;
        down &= 1;
    }
    return down;
}

u8 ctlpadAnyLeft(s32 which)
{
    u32 down;
    u32 v;

    if (which == 2) {
        down = 0;
        if (ctlpadAnyLeft(1) || ctlpadAnyLeft(0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        down = v >> 0xF;
        down &= 1;
    }
    return down;
}

u8 ctlpadAnyRight(s32 which)
{
    u32 down;
    u32 v;

    if (which == 2) {
        down = 0;
        if (ctlpadAnyRight(1) || ctlpadAnyRight(0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        down = v >> 0xD;
        down &= 1;
    }
    return down;
}

u8 ctlpadAnySelect(s32 which)
{
    u32 down;
    u32 v;

    if (which == 2) {
        down = 0;
        if (ctlpadAnySelect(1) || ctlpadAnySelect(0)) {
            down = 1;
        }
    } else {
        v = gCtlPad.all;
        if (which == 1) {
            v >>= 16;
        }
        down = v & 0x940;
        down = down != 0;
    }
    return down;
}

s32 GetJoyStick(void)
{
    return 0;
}
