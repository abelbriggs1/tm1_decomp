#ifndef __TM1_GPU_H__
#define __TM1_GPU_H__

#define GET_TPAGE_POSITION(tpage, x, y)                                                            \
    (*(y) = ((tpage) << 4) & 0x100, *(x) = ((tpage) << 6) & 0x3FF)
#define GET_CLUT_POSITION(clut, x, y) (*(x) = ((clut) & 0x3F) << 4, *(y) = (clut) >> 6)

#endif // __TM1_GPU_H__
