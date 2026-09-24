/*
 * $PSLibId: Runtime Library Version 3.3$
 */
#ifndef _LIBMATH_H_
#define _LIBMATH_H_
/* 
  libmath.h
*/


double pow(double, double),exp(double);
double log(double), log10(double);
double floor(double), ceil(double);
double fmod(double,double),modf(double,double *);
double sin(double), cos(double), tan(double);
double asin(double), acos(double);
double atan(double), atan2(double, double);
double sinh(double), cosh(double), tanh(double);
double sqrt(double);
double hypot(double, double);
double ldexp(double, int), frexp(double, int *);

#define _ABS(x)		((x) < 0 ? -(x) : (x))
#define fabs(x)		_ABS(x)

extern int math_errno;
extern int math_err_point;

#endif /* _LIBMATH_H_ */
