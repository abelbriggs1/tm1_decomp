#ifndef TM1_LONG_VECTOR_H
#define TM1_LONG_VECTOR_H
#include "common.h"

/* Engine vector: three32-bit coordinates, not PsyQ's padded16-byte VECTOR. */
// TODO: Remove and combine with `VEC3`.
typedef struct {
    s32 vx, vy, vz;
} LVECTOR;
#endif
