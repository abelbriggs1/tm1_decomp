#include "common.h"

#include "sdk/libgte.h"

// Constant for `1.0` in fixed point with 12 bits of mantissa (FXP16 - 1.3.12, FXP32 - 1.19.12)
#define FXP_ONE 0x1000

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[0][1]) + (rhs->vz * lhs->m[0][2]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[1][0]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[1][2]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[2][0]) + (rhs->vy * lhs->m[2][1]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

void mathMulTransVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[1][0]) + (rhs->vz * lhs->m[2][0]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[0][1]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[2][1]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[0][2]) + (rhs->vy * lhs->m[1][2]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

// Identical to `mathMulVec`.
void mathMulVecLong(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[0][1]) + (rhs->vz * lhs->m[0][2]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[1][0]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[1][2]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[2][0]) + (rhs->vy * lhs->m[2][1]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}

#ifdef NON_MATCHING
void mathMulTransVecShort(volatile MATRIX* lhs, SVECTOR* rhs, volatile VECTOR* out)
{
    out->vx = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[1][0]) + (rhs->vz * lhs->m[2][0]))
        / FXP_ONE;
    out->vy = ((rhs->vx * lhs->m[0][1]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[2][1]))
        / FXP_ONE;
    out->vz = ((rhs->vx * lhs->m[0][2]) + (rhs->vy * lhs->m[1][2]) + (rhs->vz * lhs->m[2][2]))
        / FXP_ONE;
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/math", mathMulTransVecShort);
#endif // NON_MATCHING

#ifdef NON_MATCHING
void mathNormalizeVec(volatile VECTOR* in, volatile VECTOR* out)
{
    s32 norm = SquareRoot0((in->vx * in->vx) + (in->vy * in->vy) + (in->vz * in->vz));
    if (norm > 0) {
        out->vx = (in->vx * FXP_ONE) / norm;
        out->vy = (in->vy * FXP_ONE) / norm;
        out->vz = (in->vz * FXP_ONE) / norm;
    } else {
        out->vx = 0;
        out->vy = 0;
        out->vz = 0;
    }
}
#else
INCLUDE_ASM("asm/nonmatchings/tm1/math", mathNormalizeVec);
#endif // NON_MATCHING
