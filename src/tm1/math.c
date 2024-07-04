#include "common.h"

#include "sdk/libgte.h"

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out)
{

    out->vx
        = ((rhs->vx * lhs->m[0][0]) + (rhs->vy * lhs->m[0][1]) + (rhs->vz * lhs->m[0][2])) / 0x1000;

    out->vy
        = ((rhs->vx * lhs->m[1][0]) + (rhs->vy * lhs->m[1][1]) + (rhs->vz * lhs->m[1][2])) / 0x1000;

    out->vz
        = ((rhs->vx * lhs->m[2][0]) + (rhs->vy * lhs->m[2][1]) + (rhs->vz * lhs->m[2][2])) / 0x1000;
}

INCLUDE_ASM("asm/nonmatchings/tm1/math", mathMulTransVec);

INCLUDE_ASM("asm/nonmatchings/tm1/math", mathMulVecLong);

INCLUDE_ASM("asm/nonmatchings/tm1/math", mathMulTransVecShort);

INCLUDE_ASM("asm/nonmatchings/tm1/math", mathNormalizeVec);
