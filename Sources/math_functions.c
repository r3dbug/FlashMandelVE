
#include "math_functions.h"

/* General math functions */

double GlobalSIN, GlobalCOS;

double __asm arctanFPU(register __fp0 double);

/*
#define PI					3.14159265358979323846264338327
#define PIDIV2  	     	1.57079632679489661923132169163
#define PINEG   			-3.14159265358979323846264338327
#define PIDIV2NEG      		-1.57079632679489661923132169163
*/

double Fast2ArcTan(double x) {
  double a1  =  0.99997726f;
  double a3  = -0.33262347f;
  double a5  =  0.19354346f;
  double a7  = -0.11643287f;
  double a9  =  0.05265332f;
  double a11 = -0.01172120f;

  double x_sq = x*x;
  return
    x * (a1 + x_sq * (a3 + x_sq * (a5 + x_sq * (a7 + x_sq * (a9 + x_sq * a11)))));
}

double atan2Capprox(double x, double y)
{
	if (x>0) return Fast2ArcTan(y/x);
	else if ((x<0) && (y>0)) return Fast2ArcTan(y/x)+PI;
	else if ((x<0) && (y==0)) return PI;
	else if ((x<0) && (y<0)) return Fast2ArcTan(y/x)-PI;
	else if ((x==0) && (y>0)) return PI/2.0;
	else if ((x==0) && (y<0)) return -PI/2.0;
	else return 0.0;
}

double atan2C(double x, double y)
{
	if (x>0) return arctanFPU(y/x);
	else if ((x<0) && (y>0)) return arctanFPU(y/x)+PI;
	else if ((x<0) && (y==0)) return PI;
	else if ((x<0) && (y<0)) return  arctanFPU(y/x)-PI;
	else if ((x==0) && (y>0)) return PI/2.0;
	else if ((x==0) && (y<0)) return -PI/2.0;
	else return 0.0;
}

double atan2Creorg(double x, double y)
{
	if (x>0) return arctanFPU(y/x);
    if (x<0) 
    {
    	if (y>0) return arctanFPU(y/x)+PI;
        if (y<0) return arctanFPU(y/x)-PI;
        return PI;  /* y==0 */
    }
    
    /* x==0 */
    if (y>0) return PI/2.0;
    if (y<0) return -PI/2.0;
    
    return 0.0;
}

/* Multimandel doubleing Point with periodicity checking */
UWORD IdenticalWithEpsilon(double a, double b) 
{
	double d, epsilon=0.001; /* 0.0000001; */
	d=fabs(a-b);
	if (d<epsilon) return 1;
	else return 0;
}
