#ifndef __TM1_MATH_H__
#define __TM1_MATH_H__

#include "common.h"
#include <LIBGTE.H>
#include <RAND.H>

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

/* Modulo helper macros. */

// Take the integer `x % val`, assuming that `val` is a power of 2.
#define MOD_POW2(x, val) ((x) & ((val) - 1))

/* `rand()` helper macros. */

// Generate a random integer in the interval `[0, val)`.
#define RANDINT(val) (rand() % (val))

// Generate a random integer in the interval `[0, val)`, assuming that
// `val` is a power of 2.
#define RANDINT_POW2(val) MOD_POW2(rand(), val)

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulVecLong(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVecShort(volatile MATRIX* lhs, SVECTOR* rhs, volatile VECTOR* out);
void mathNormalizeVec(volatile VECTOR* in, volatile VECTOR* out);

#endif // __TM1_MATH_H__
