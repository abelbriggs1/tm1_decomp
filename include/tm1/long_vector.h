#ifndef TM1_LONG_VECTOR_H
#define TM1_LONG_VECTOR_H
#include "common.h"

/* Engine vector: three32-bit coordinates, not PsyQ's padded16-byte VECTOR. */
// TODO: Remove and combine with `VEC3`.
typedef struct {
    s32 vx, vy, vz;
} LVECTOR;

typedef struct VEC3 {
    s32 x;
    s32 y;
    s32 z;
} VEC3;
#endif
