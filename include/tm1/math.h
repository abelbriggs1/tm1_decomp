#ifndef __TM1_MATH_H__
#define __TM1_MATH_H__

#include "common.h"
#include <libgte.h>

/* Engine vector: three 32-bit coordinates, not PsyQ's padded 16-byte VECTOR. */
typedef struct {
    s32 vx, vy, vz;
} VECTOR3;

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulVecLong(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVecShort(volatile MATRIX* lhs, SVECTOR* rhs, volatile VECTOR* out);
void mathNormalizeVec(volatile VECTOR* in, volatile VECTOR* out);

#endif // __TM1_MATH_H__
