
#include "math_functions.h"

/* General math functions */

double GlobalSIN, GlobalCOS;

double __asm arctanFPU(register __fp0 double);

double atan2C(double x, double y)
{
	if (x>0) return arctanFPU(y/x);
	else if ((x<0) && (y>0)) return arctanFPU(y/x)+PI;
	else if ((x<0) && (y==0)) return PI;
	else if ((x<0) && (y<0)) return arctanFPU(y/x)-PI;
	else if ((x==0) && (y>0)) return PI/2.0;
	else if ((x==0) && (y<0)) return -PI/2.0;
	else return 0.0;
}

/* Multimandel Floating Point with periodicity checking */
UWORD IdenticalWithEpsilon(double a, double b) 
{
double d, epsilon=0.001; /* 0.0000001; */
	d=fabs(a-b);
	if (d<epsilon) return 1;
	else return 0;
}
