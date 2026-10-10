#ifndef __TM1_MATH_H__
#define __TM1_MATH_H__

#include "common.h"
#include <libgte.h>

/* Engine vector: three 32-bit coordinates, not PsyQ's padded 16-byte VECTOR. */
typedef struct {
    s32 vx, vy, vz;
} VECTOR3;

// Constant for `1.0` in fixed point with 12 bits of mantissa (FXP16 - 1.3.12, FXP32 - 1.19.12)
#define FXP_ONE 0x1000

// clang-format off
#define IDENTITY_MATRIX { \
    {  \
        {FXP_ONE, 0, 0}, \
        {0, FXP_ONE, 0}, \
        {0, 0, FXP_ONE} \
    }, \
    {0, 0, 0} \
}

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulVecLong(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVecShort(volatile MATRIX* lhs, SVECTOR* rhs, volatile VECTOR* out);
void mathNormalizeVec(volatile VECTOR* in, volatile VECTOR* out);

#endif // __TM1_MATH_H__
