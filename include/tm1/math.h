#ifndef __TM1_MATH_H__
#define __TM1_MATH_H__

#include "common.h"
#include <libgte.h>

void mathMulVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVec(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulVecLong(volatile MATRIX* lhs, VECTOR* rhs, volatile VECTOR* out);
void mathMulTransVecShort(volatile MATRIX* lhs, SVECTOR* rhs, volatile VECTOR* out);
void mathNormalizeVec(volatile VECTOR* in, volatile VECTOR* out);

#endif // __TM1_MATH_H__
