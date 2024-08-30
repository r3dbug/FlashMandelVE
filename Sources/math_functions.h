
#ifndef MATH_FUNCTIONS_H
#define MATH_FUNCTIONS_H

#include <math.h>
#include <exec/types.h>

extern double GlobalSIN, GlobalCOS;

/* General math functions */

double atan2C(double, double);
double atan2Creorg(double, double);
double Fast2ArcTan(double);
double Fast2ArcTan(double);
double atan2Capprox(double, double);

/* Multimandel Floating Point with periodicity checking */
UWORD IdenticalWithEpsilon(double, double);

#endif /* MATH_FUNCTIONS_H */
