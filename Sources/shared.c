
#include "shared.h"
#include "enum.h"

UWORD InterruptDrawing(struct Window*, const LONG,const LONG,const LONG,const LONG);
BOOL   CheckBox      (struct RastPort *,const LONG,const LONG,const LONG,const LONG);

/* General function pointers */
/*
ULONG (*MultiMandelInCGeneric)(ULONG, double, double);
ULONG (*MultiJuliaInCGeneric)(ULONG,double,double);
ULONG (*BurningShipInCGeneric)(ULONG,double,double);
ULONG (*FractalInCGeneric)(ULONG,double,double);
ULONG (*FractalInCGenericStoreIterations)(ULONG,double,double);
void (*PutPixelWithColorMode)(ULONG, ULONG, ULONG);
*/
/* Common global variables (from boundary) */
/*UWORD* Data; */
/*extern UBYTE *Data;
extern UBYTE *Done;
extern ULONG *Queue;
extern ULONG DataSize;
extern ULONG DoneSize;
extern ULONG QueueSize;
extern ULONG QueueHead, QueueTail;
extern ULONG p;
extern UWORD flag;
extern struct Window* TempWin;
extern ULONG resx, resy;
extern UBYTE* screenbuffer;

extern enum { Loaded=1, Queued=2 };

*/

void FreeBuddha(void) 
{
	if (BuddhaCounter) FreeVec(BuddhaCounter);  /* Buddha orbit counters (1920x1080 ULONG) */
	/*if (CompressedBuddhacounter)=FreeVec(CompressedBuddhaCounter);*/
  	if (re) FreeVec(re);						  /* Array with orbits for periodicity checking */
  	if (im) FreeVec(im);
}

BOOL AllocBuddha(void)
{
	BuddhaCounter=AllocVec(sizeof(ULONG)*MAX_NEBULA_WIDTH*MAX_NEBULA_HEIGHT*3, MEMF_FAST); 		/* allocate enough memory for FullHD: 1920x1080 ULONG */
	/*  CompressedBuddhaCounter=AllocVec(Sizeof(ULONG)*MAX_NEBULA_WIDTH*MAX_NEBULA_HEIGHT*4, MEMF_FAST); /* allocate 33% more space */
  	re = AllocVec(sizeof(double) * MAXITERPERIODICITY, MEMF_FAST);
  	im = AllocVec(sizeof(double) * MAXITERPERIODICITY, MEMF_FAST);
  	SHOW_MAXCOUNTERS = FALSE;

	if ((!BuddhaCounter) || (!re) || (!im))
    {
        FreeBuddha();
        return FALSE;
    } else return TRUE;
}


LONG lround(double v)
{
	if (v>=0) return (LONG)(v+0.5);
	else return (LONG)(v-0.5);
}

double RenormalizedIterationCount(ULONG i, double fx, double fy, double p)
{
	/* i = iterations 
	   fx = final x (re)
	   fy = final y (im)
	   p = power (Mandelbrot = 2, Multibrot = >2)
	*/
	
	double log_zn;
	double mu, nu;
	
	log_zn = log(sqrt(fx * fx + fy * fy)) / p;
	nu 	   = log(log_zn / log(p)) / log(p);	
				
	mu = (double)i + 1 - nu;
	return mu;
}

/* PutPixel function depending on color mode */

/* Classic functions with palette for CLUT and RGB */
void PutPixelWithColorModeCLUT(ULONG x, ULONG y, ULONG Color)
{
	if (Color)
    {
		Color *= CURRENT_MAX_COLORS,

		Color /= ITERATIONS,

		Color += RESERVED_PENS;
    }
    PutPixel(x,y,Color);	
}

/* A special function for RGB is not necessary: 
 * the already defined PutPixel(x,y,Color) can be used.
 */

/* New functions for smooth colors */

/* Renormalized Iteration Count (RIC) */
void PutPixelWithColorModeRGBSmoothRIC(ULONG x, ULONG y, ULONG Color)
{
	double log_zn;
	double mu, nu;
	double fraction;
	ULONG color1, color2;
	
	if (Color)
	{
		log_zn = log(sqrt(FinalXP1 * FinalXP1 + FinalYP1 * FinalYP1)) / GlobalP;
		nu 	   = log(log_zn / log(GlobalP)) / log(GlobalP);	
				
		mu = (double)(MAX_ITERATIONS-Color) + 2 - nu;
	
		color1 = (ULONG)mu;  /* floor(x) */
		color2 = color1+1;   /* floor(x) */ 
			
		fraction = mu - (double)((int)mu);
								
		Color = InterpolateCLUT(color1, color2, fraction);
	}
	
	PutPixelARGB(x,y,Color);
}

/* Cyclic Sinus Coloring (FlashMandelNG / FMNG) */
void PutPixelWithColorModeRGBSmoothFMNG(ULONG x, ULONG y, ULONG Color)
{
	UBYTE red, green, blue;
	
	if (Color)
	{
		red	  = (UBYTE) lround((sin(0.016f * (double)Color + 0.20f) * 127.5f + 127.5f));
		green = (UBYTE) lround((sin(0.013f * (double)Color + 0.15f) * 127.5f + 127.5f));
		blue  = (UBYTE) lround((sin(0.010f * (double)Color + 0.10f) * 127.5f + 127.5f));
				
		Color = (red << 16) | (green << 8) | (blue);
	}

	PutPixelARGB(x,y,Color);
	
}

/******************************/
/* Mandel and Julia functions */
/******************************/

ULONG MandelInC(ULONG i, double cx, double cy)
{
	double zx, zy, xtemp;
	zx=cx;
	zy=cy;
	while ((i) && (zx*zx+zy*zy<=4)) {
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx * zy + cy;
		zx = xtemp + cx;
		i--;
	}	
	return i;
}

ULONG MandelInCStoreIterations(ULONG i, double cx, double cy)
{
	double zx, zy, xtemp;
	zx=cx;
	zy=cy;
	while ((i) && (zx*zx+zy*zy<=4)) {
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx * zy + cy;
		zx = xtemp + cx;
		re[i]=zx;
		im[i]=zy;
		i--;
	}	
	return i;
}

ULONG JuliaInC(ULONG i, double zx, double zy, double cx, double cy) {
double xtemp;
	while ((i) && (zx*zx+zy*zy<=4)) {
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx * zy + cy;
		zx = xtemp + cx;
		i--;
	}	
	return i;		
}

ULONG MultiMandelInC(ULONG i, double zx, double zy)
{
ULONG p;
double xtemp, ax, ay, ox, oy, cx, cy;
	cx = zx;
	cy = zy;
	ax = 0;
	ay = 0;
	
	while ((i) && (ax*ax+ay*ay<=4))
	{
/* this is the working Multibrot formula */
/*	
		xtemp=atan((double)p);
		xtemp = pow((ax*ax+ay*ay),(p/2.0)) * cos(p*atan2(ay,ax)) + cx;
		ay= pow((ax*ax+ay*ay),(p/2.0)) * sin(p*atan2(ay,ax)) + cy;
		ax = xtemp;
*/
		ox=ax;
		oy=ay;
		p=MultiPower-1;
		
		while (p) 
		{
			xtemp=ax*ox - ay*oy;
			ay = ax*oy + ay*ox; 
			ax = xtemp;
			p--;	
		}
		ax += cx;
		ay += cy;

		i--;
	}
	
	/* for smooth coloring */
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

ULONG MultiMandelInCStoreIterations(ULONG i, double zx, double zy)
{
ULONG p;
double xtemp, ax, ay, ox, oy, cx, cy;
	cx = zx;
	cy = zy;
	ax = 0;
	ay = 0;
	
	while ((i) && (ax*ax+ay*ay<=4))
	{
		ox=ax;
		oy=ay;
		p=MultiPower-1;
		
		while (p) 
		{
			xtemp=ax*ox - ay*oy;
			ay = ax*oy + ay*ox; 
			ax = xtemp;
			p--;	
		}
		ax += cx;
		ay += cy;

		re[i]=ax;
		im[i]=ay;

		i--;
	}
	
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

ULONG MultiJuliaInC(ULONG i, double zx, double zy)
{
ULONG p;
double xtemp, ax, ay, ox, oy, cx, cy;
	cx = JKRE;
	cy = JKIM;
	ax = zx;
	ay = zy;
	
	while ((i) && (ax*ax+ay*ay<=4))
	{
		ox=ax;
		oy=ay;
		p=MultiPower-1;
		while (p) 
		{
			xtemp=ax*ox - ay*oy;
			ay = ax*oy + ay*ox;
			ax = xtemp;
			p--;	
		}
		ax += cx;
		ay += cy;

		i--;
	}
	
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

/* functions when MultiPowerFloat is a float */

ULONG MultiJuliaFloatingPowerInC(ULONG i, double zx, double zy)
{
double p,p2;
double ax, ay, cx, cy, powvar, ax2ay2, patan2ayax;
	cx = JKRE;
	cy = JKIM;
	ax = zx;
	ay = zy;
	p = MultiPowerFloat;
	p2 = p/2.0;

	while ((i) && ((ax2ay2=ax*ax+ay*ay)<=4))
	{
		/* slightly optimized with some assembly */
		/* patan2ayax=p*atan2(ay,ax); */
		/* patan2ayax=p*atan2(ax,ay); */
		patan2ayax=p*atan2C(ax,ay);
	
		/* powvar = pow((ax2ay2),p2); */
		powvar = powFPU(ax2ay2,p2);
		ax = powvar * cos(patan2ayax) + cx;
		ay = powvar * sin(patan2ayax) + cy;

		i--;
	}
	
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

ULONG MultiMandelFloatingPowerInC(ULONG i, double zx, double zy)
{
double p, p2;
double ax, ay, cx, cy, ax2ay2, patan2ayax, powvar;
LONG tortoise, hare;
UBYTE isperiodic;

	cx = zx;
	cy = zy;
	ax = 0;
	ay = 0;
	p = MultiPowerFloat;
	p2 = p/2.0;
	
	re[i]=ax;
	im[i]=ay;
	
	tortoise=i;
	hare=i-1;
	isperiodic=0;
	
	while ((i) && (!isperiodic) && ((ax2ay2=ax*ax+ay*ay)<=4))
	{
		/* patan2ayax=p*atan2(ay,ax); */
		patan2ayax=p*atan2C(ax,ay);
		/* patan2ayax=p*atan2FPU(ax,ay); */
	
		/* powvar = pow((ax2ay2),p2); */
		powvar = powFPU(ax2ay2,p2);
/*
		// not faster than single calls to sin / cos
		sincosFPU(patan2ayax,patan2ayax);		
		ax = powvar * GlobalCOS + cx;
		ay = powvar * GlobalSIN + cy;
*/
		
		ax = powvar * cos(patan2ayax) + cx;
		ay = powvar * sin(patan2ayax) + cy;

/*
		patan2ayax=p*atan2andpowFPU(ax,ay,ax2ay2,p2);
		ax = GlobalPowerVar * cos(patan2ayax) + cx;
		ay = GlobalPowerVar * sin(patan2ayax) + cy;
*/
		/*i--;*/
		
		re[i]=ax;
		im[i]=ay;
		
		i--;
		
		if (i==hare) 
		{
			if (
				IdenticalWithEpsilon(re[hare],re[tortoise])
				&&
				IdenticalWithEpsilon(im[hare],im[tortoise])
			   ) 
			{
				/*printf("periodicity detected: tortoise[%d]: (%f,%f) <==> hare[%d]: (%f,%f)\n", tortoise, re[tortoise], im[tortoise], hare, re[hare], im[hare]);
				*/
				isperiodic=1;
				if (PeriodicityColoring) i=hare; /* i=0;  // tortoise-hare; */
				else i=0; /* early bailout */
			}
			else 
			{
				hare-=2;
				tortoise-=1;
			}	
		}

	}
	
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

ULONG MultiMandelFloatingPowerInCStoreIterations(ULONG i, double zx, double zy)
{
double p, p2;
double ax, ay, cx, cy, ax2ay2, patan2ayax, powvar;
LONG tortoise, hare;
UBYTE isperiodic;

	cx = zx;
	cy = zy;
	ax = 0;
	ay = 0;
	p = MultiPowerFloat;
	p2 = p/2.0;
	
	re[i]=ax;
	im[i]=ay;
	
	tortoise=i;
	hare=i-1;
	isperiodic=0;
	
	/* disable periodicity checking for orbits display */
	while ((i) && /*(!isperiodic) &&*/ ((ax2ay2=ax*ax+ay*ay)<=4))
	{
		/* patan2ayax=p*atan2(ay,ax); */
		patan2ayax=p*atan2C(ax,ay);
		/* patan2ayax=p*atan2FPU(ax,ay); */
	
		/* powvar = pow((ax2ay2),p2); */
		powvar = powFPU(ax2ay2,p2);
/*
		// not faster than single calls to sin / cos
		sincosFPU(patan2ayax,patan2ayax);		
		ax = powvar * GlobalCOS + cx;
		ay = powvar * GlobalSIN + cy;
*/
		
		ax = powvar * cos(patan2ayax) + cx;
		ay = powvar * sin(patan2ayax) + cy;

/*
		patan2ayax=p*atan2andpowFPU(ax,ay,ax2ay2,p2);
		ax = GlobalPowerVar * cos(patan2ayax) + cx;
		ay = GlobalPowerVar * sin(patan2ayax) + cy;
*/
		/*i--;*/
		
		re[i]=ax;
		im[i]=ay;
		
		i--;
		
/*		
		if (i==hare) 
		{
			if (
				IdenticalWithEpsilon(re[hare],re[tortoise])
				&&
				IdenticalWithEpsilon(im[hare],im[tortoise])
			   ) 
			{
				/*printf("periodicity detected: tortoise[%d]: (%f,%f) <==> hare[%d]: (%f,%f)\n", tortoise, re[tortoise], im[tortoise], hare, re[hare], im[hare]);
				*/
/*				isperiodic=1;
				if (PeriodicityColoring) i=hare; /* i=0;  // tortoise-hare; */
/*				else i=0; /* early bailout */
/*			}
			else 
			{
				hare-=2;
				tortoise-=1;
			}	
		}
*/
	}
	
	FinalXP1 = ax;
	FinalYP1 = ay;
	
	return i;
}

/**********************************************************************************************/
/********************************** BRUTE FORCE ***********************************************/
/**********************************************************************************************/

/* (1) Brute force */
/* (1a) Mandelbrot */
/* (1b) Julia */
/* (1c) Multibrot */

ULONG DrawMultiMandelFractalBrute (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , /*Dummy = NULL,*/ StartMicro, EndMicro;
double cx,cy;
WORD x, y;
double power;

 /*printf("DrawMultiMandelFractalBrute()\n");*/
 
/* printf("Calculating MultiMandel Brute with Power: %d\n", MultiPower); */

 CurrentTime (&StartSec,&StartMicro);

 power = (FractalType==MULTIFLOAT) ? MultiPowerFloat : MultiPower;
 
 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = (FractalType == MULTIINT) ? MultiPower : MultiPowerFloat; 

 /* ShowData("BRUTE (MANDEL)", a1,b1,a2,b2); */

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
			Color=MultiMandelInCGeneric(ITERATIONS,cx,cy);
			/* Color=MultiMandelInC(ITERATIONS,cx,cy); */
/*
if (Color)
{
     	Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

   	   	Color += RESERVED_PENS;
}
*/			PutPixelWithColorMode(x,y,Color);

		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&EndMicro);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);

/*
 printf("StartMicro: %ul EndMicro: %ul\n",StartMicro,EndMicro);
 return (EndMicro-StartMicro);
*/
}

/* (1d) MultiJulia */

ULONG DrawMultiJuliaFractalBrute (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , /*Dummy = NULL,*/ StartMicro, EndMicro;
double cx,cy;
WORD x, y;
double power;

 CurrentTime (&StartSec,&StartMicro);

 power = (FractalType==MULTIFLOAT) ? MultiPowerFloat : MultiPower;
 
 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = (FractalType == MULTIINT) ? MultiPower : MultiPowerFloat; 
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
			Color=MultiJuliaInCGeneric(ITERATIONS,cx,cy);

			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&EndMicro);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);

/*
 printf("StartMicro: %ul EndMicro: %ul\n",StartMicro,EndMicro);
 return (EndMicro-StartMicro);
*/
}

/* 1e) burning ship */

ULONG BurningShipInC(ULONG i, double x, double y)
{
double xn, yn, cx, cy, xtemp;

	xn=0;
	yn=0;
	
	cx = x;
	cy = -y;   /* flip it around, because otherwise ship will be upside down ... */
	
	while ((i) && (xn*xn + yn*yn < 4))
	{
		xtemp = xn*xn - yn*yn + cx;
		yn = fabs(2.0*xn*yn) + cy;
		xn = xtemp;
		i--; 
	}
	return i;
}

ULONG BurningShipInCStoreIterations(ULONG i, double x, double y)
{
    double xn, yn, cx, cy, xtemp;
	
	xn=0;
	yn=0;
	
	cx = x;
	cy = -y;   /* flip it around, because otherwise ship will be upside down ... */
	
	while ((i) && (xn*xn + yn*yn < 4))
	{
		xtemp = xn*xn - yn*yn + cx;
		yn = fabs(2.0*xn*yn) + cy;
		xn = xtemp;
		
		re[i] = xn;
		im[i] = yn;
		
		i--; 
	}
	
	FinalXP1 = xn;
	FinalYP1 = yn;
	
	return i;
}

ULONG BurningShipInCJulia(ULONG i, double x, double y)
{
double xn, yn, cx, cy, xtemp;

	xn=x;
	yn=-y;
	
	i-=1;
	
	cx = JKRE;
	cy = -JKIM;   /* flip it around, because otherwise ship will be upside down ... */
	
	while ((i) && (xn*xn + yn*yn < 4))
	{
		xtemp = xn*xn - yn*yn + cx;
		yn = fabs(2.0*xn*yn) + cy;
		xn = xtemp;
		i--; 
	}
	
	FinalXP1 = xn;
	FinalYP1 = yn;
	
	return i;
}

ULONG DrawBurningShipFractalBrute (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
double cx,cy;
WORD x, y;

 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
			Color=BurningShipInCGeneric(ITERATIONS,cx,cy);
			
			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);

}

/* Tricorn */

ULONG TricornInC(ULONG i, double x, double y)
{
double xn, yn, cx, cy, xtemp;

	xn=0;
	yn=0;
	
	cx = x;
	cy = y;   
	
	while ((i) && (xn*xn + yn*yn < 4))
	{
		xtemp = xn*xn - yn*yn + cx;
		yn = -2.0*xn*yn + cy;
		xn = xtemp;
		i--; 
	}
	
	FinalXP1 = xn;
	FinalYP1 = yn;
	
	return i;
}

ULONG DrawTricornFractalBrute (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
double cx,cy;
WORD x, y;

 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
			Color=TricornInC(ITERATIONS,cx,cy);
			
			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);

}

/**********************************************************************************************/
/********************************** TILING ****************************************************/
/**********************************************************************************************/

/* (2) Tiling - for both Mandelbrot and Julia*/
/* These funtions are separated via function pointers H_LINE, V_LINE */

BOOL RectangleDraw (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
LONG helpx,helpy;

  if ((helpx = (a2 - a1)) < MINLIMIT) return FALSE;

  if ((helpy = (b2 - b1)) < MINLIMIT) return FALSE;

  if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
  
  if (CheckBox (Win->RPort,a1,b1,a2,b2))
  {
/* 
	 SetAPen (Win->RPort,ReadPixel (Win->RPort,a1,b1));

     RectFill (Win->RPort,a1+1L,b1+1L,a2-1L,b2-1L);
*/	 

	 RectColorFill(Win->RPort,a1+1L,b1+1L,a2-1L,b2-1L,GetPixelPen(a1,b1));
	 
     return FALSE;
  }

  if ((helpx < (MINLIMIT * 2L)) || (helpy < (MINLIMIT * 2L)))
  {
     for (helpx = (a1 + 1L); helpx < a2; helpx++)
     {
         (*V_LINE) (Win->RPort,b1+1L,b2-1L,helpx);
     }

     return FALSE;
  }

  if (helpx >= helpy)
  {
     helpx = (a1 + a2) >> 1L;

     (*V_LINE) (Win->RPort,b1+1L,b2-1L,helpx);

     if (RectangleDraw (Win,a1,b1,helpx,b2)) return TRUE;

     if (RectangleDraw (Win,helpx,b1,a2,b2)) return TRUE;
  }

  else
  {
     helpy = (b1 + b2) >> 1L;

     (*H_LINE) (Win->RPort,a1+1L,a2-a1-1,helpy);

     if (RectangleDraw (Win,a1,b1,a2,helpy)) return TRUE;

     if (RectangleDraw (Win,a1,helpy,a2,b2)) return TRUE;
  }

  return FALSE;
}

ULONG DrawFractalTiling (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;

 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 /*ShowData*/ /*printf("shared.c: TILING", a1,b1,a2,b2); */

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

 (*H_LINE) (Win->RPort,a1,a2-a1+1,b1);

 (*V_LINE) (Win->RPort,b1+1,b2-1,a2);

 (*H_LINE) (Win->RPort,a1,a2-a1+1,b2);

 (*V_LINE) (Win->RPort,b1+1,b2-1,a1);

 RectangleDraw (Win,a1,b1,a2,b2);

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

/* Multibrot => needs separate functions */

/* (2aa) Horizontal */

void MMHLine (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x;

REGISTER UBYTE *TmpArray;

REGISTER ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = MultiMandelInCGeneric (MAX_ITERATIONS+1,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }

 WritePixelLine/* 8 */ (Rp, a1, y, width, PixelLine, &TempRP);
}

/* (2ab) Vertical */

void MMVLine (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 for (y = b2; y >= b1; y--)
 {
     Color = MultiMandelInCGeneric (MAX_ITERATIONS+1,CRE,CIM);
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,y,Color);
	 
     CIM += INCREMIMAG;
 }
}

/* Multibrot Julia Horizontal */

void MJHLine (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x = width;

UBYTE *TmpArray = PixelLine;

ULONG Color;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = MultiJuliaInCGeneric (MAX_ITERATIONS+1,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     
     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }
 WritePixelLine/*8*/ (Rp, a1, y, width, PixelLine, &TempRP);
}

/* (3bb) Vertical */

void MJVLine (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);

 for (y = b2; y >= b1; y--)
 {
     Color = MultiJuliaInCGeneric (MAX_ITERATIONS+1,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
	 PutPixel(x,y,Color);
	 
     CIM += INCREMIMAG;
 }
}


/* Burning Ship => separate functions */

/* (4aa) Horizontal */

void BSHLine (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x;

REGISTER UBYTE *TmpArray;

REGISTER ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = BurningShipInCGeneric (MAX_ITERATIONS+1,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }

 WritePixelLine/*8*/ (Rp, a1, y, width, PixelLine, &TempRP);
}

/* (4ab) Vertical */

void BSVLine (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 for (y = b2; y >= b1; y--)
 {
     Color = BurningShipInCGeneric (MAX_ITERATIONS+1,CRE,CIM);
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
	 PutPixel(x,y,Color);     
	 /*
     SetAPen (Rp,Color);

     WritePixel (Rp,x,y);
	 */
     CIM += INCREMIMAG;
 }
}


/**********************************************************************************************/
/********************************** BOUNDARY TRACE ********************************************/
/**********************************************************************************************/

/* (3) Boundary trace */

/* Memory allocation */
int AllocateBoundary(void) {
    DataSize=sizeof(UWORD)*resx*resy;  /* reserve words for 0xffff iterations */
    DoneSize=sizeof(UBYTE)*resx*resy;
    QueueSize=sizeof(ULONG)*((resx*resy)*4);
    Data=AllocMem(DataSize, MEMF_PUBLIC | MEMF_CLEAR); 
    Done=AllocMem(DoneSize, MEMF_PUBLIC | MEMF_CLEAR);
    Queue=AllocMem(QueueSize, MEMF_PUBLIC | MEMF_CLEAR);
    if ((Data==NULL) || (Done==NULL) || (Queue==NULL)) return 0;
    else return 1;
}

void DeallocateBoundary(void) {
    if (Queue) {
		FreeMem(Queue,QueueSize);
	}
    if (Done) {
		FreeMem(Done,DoneSize);
	}
    if (Data) {
		FreeMem(Data,DataSize); 
	}
}

/* Build the queue */
void AddQueue(unsigned p) {
    if(Done[p] & Queued) return;
    Done[p] |= Queued;
    Queue[QueueHead++] = p;
    if(QueueHead == QueueSize) QueueHead = 0;
}

void ClearBoundaryBuffers(void) 
{
	ULONG p;
	for (p=0;p<resx*resy; p++)
	{
		Data[p]=0;
		Done[p]=0;	
	}
}

/* (3a) Mandelbrot */
/* (3b) Julia */
/* (3c) MultiMandel */

UWORD LoadMultiMandel(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = MultiMandelInCGeneric(ITERATIONS,cx,cy);
	
	if (Color)
    {
     	Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

   	   	Color += RESERVED_PENS;
    }
	PutPixel(x,y,Color);
		
    Done[p] |= Loaded;
	return Data[p] = Color;
}

void ScanMultiMandel(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadMultiMandel(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadMultiMandel(p-1) != center);
    r=rr && (LoadMultiMandel(p+1) != center);
    u=uu && (LoadMultiMandel(p-resx) != center);
    d=dd && (LoadMultiMandel(p+resx) != center);
    /* process the queue (which is actually a ring buffer) */
    if (l) AddQueue(p-1);
    if (r) AddQueue(p+1);
    if (u) AddQueue(p-resx);
    if (d) AddQueue(p+resx);
    /* the corner pixels (nw,ne,sw,se) are also neighbors */
    if((uu&&ll)&&(l||u)) AddQueue(p-resx-1);
    if((uu&&rr)&&(r||u)) AddQueue(p-resx+1);
    if((dd&&ll)&&(l||d)) AddQueue(p+resx-1);
    if((dd&&rr)&&(r||d)) AddQueue(p+resx+1);
}

ULONG DrawMultiMandelFractalBoundary (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
UWORD x, y;

 /*printf("DrawMultiMandelFractalBoundary()\n");*/
 
 CurrentTime (&StartSec,&Dummy);

 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);
 
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	TempWin = Win;
	
	/* allocation check */
	if (!AllocateBoundary()) return 0;
    
	/* (1) begin by adding the screen edges into the queue */
    for(y=0; y<resy; ++y) {
        AddQueue(y*resx /*+ 0*/);
        AddQueue(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue(/*0*Width* +*/ x);
        AddQueue((resy-1)*resx + x);
    }
	/* for Multi: add also middle lines */
	for(y=1; y<resy-1; ++y) {
        AddQueue(y*resx + resx/2);
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue((resy/2)*resx + x);
    }
	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanMultiMandel(p);
	    if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
		
	/* (3) lastly, fill uncalculated areas with neighbor color */
	FillUncalculatedAreas();

	DeallocateBoundary();

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

/* (3d) MultiJulia */

UWORD LoadMultiJulia(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = MultiJuliaInCGeneric(ITERATIONS,cx,cy);
	
	if (Color)
    {
     	Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

   	   	Color += RESERVED_PENS;
    }
	PutPixel(x,y,Color);
		
    Done[p] |= Loaded;
	return Data[p] = Color;
}

void ScanMultiJulia(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadMultiJulia(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadMultiJulia(p-1) != center);
    r=rr && (LoadMultiJulia(p+1) != center);
    u=uu && (LoadMultiJulia(p-resx) != center);
    d=dd && (LoadMultiJulia(p+resx) != center);
    /* process the queue (which is actually a ring buffer) */
    if (l) AddQueue(p-1);
    if (r) AddQueue(p+1);
    if (u) AddQueue(p-resx);
    if (d) AddQueue(p+resx);
    /* the corner pixels (nw,ne,sw,se) are also neighbors */
    if((uu&&ll)&&(l||u)) AddQueue(p-resx-1);
    if((uu&&rr)&&(r||u)) AddQueue(p-resx+1);
    if((dd&&ll)&&(l||d)) AddQueue(p+resx-1);
    if((dd&&rr)&&(r||d)) AddQueue(p+resx+1);
}

ULONG DrawMultiJuliaFractalBoundary (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
UWORD x, y;

 CurrentTime (&StartSec,&Dummy);

 /* screenbuffer=Win->RPort->BitMap->Planes[0]; */ /* 20230406 */
 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);
 
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 /* ShowData("BOUNDARY (MANDEL)", a1,b1,a2,b2); */
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	TempWin = Win;
	
	/* allocation check */
	if (!AllocateBoundary()) return 0;
    
	/* (1) begin by adding the screen edges into the queue */
    for(y=0; y<resy; ++y) {
        AddQueue(y*resx /*+ 0*/);
        AddQueue(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue(/*0*Width* +*/ x);
        AddQueue((resy-1)*resx + x);
    }
	/* for Multi: add also middle lines */
	for(y=1; y<resy-1; ++y) {
        AddQueue(y*resx + resx/2);
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue((resy/2)*resx + x);
    }
	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanMultiJulia(p);
	    if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
		
	/* (3) lastly, fill uncalculated areas with neighbor color */
	FillUncalculatedAreas();

	DeallocateBoundary();

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

/* 1d) Burning ship boundary */

UWORD LoadBurningShip(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = BurningShipInCGeneric(ITERATIONS,cx,cy);
	
	if (Color)
    {
     	Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

   	   	Color += RESERVED_PENS;
    }
	PutPixel(x,y,Color);
		
    Done[p] |= Loaded;
	return Data[p] = Color;
}

void ScanBurningShip(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadBurningShip(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadBurningShip(p-1) != center);
    r=rr && (LoadBurningShip(p+1) != center);
    u=uu && (LoadBurningShip(p-resx) != center);
    d=dd && (LoadBurningShip(p+resx) != center);
    /* process the queue (which is actually a ring buffer) */
    if (l) AddQueue(p-1);
    if (r) AddQueue(p+1);
    if (u) AddQueue(p-resx);
    if (d) AddQueue(p+resx);
    /* the corner pixels (nw,ne,sw,se) are also neighbors */
    if((uu&&ll)&&(l||u)) AddQueue(p-resx-1);
    if((uu&&rr)&&(r||u)) AddQueue(p-resx+1);
    if((dd&&ll)&&(l||d)) AddQueue(p+resx-1);
    if((dd&&rr)&&(r||d)) AddQueue(p+resx+1);
}

ULONG DrawBurningShipFractalBoundary (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
UWORD x, y;

 /*printf("DrawBurningShipFractalBoundary (shared)\n");*/

 CurrentTime (&StartSec,&Dummy);

 /* screenbuffer=Win->RPort->BitMap->Planes[0]; */
 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);
 /*Data=screenbuffer;*/
 
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	TempWin = Win;
	
	/* allocation check */
	if (!AllocateBoundary()) return 0;
    
	/* (1) begin by adding the screen edges into the queue */
    for(y=0; y<resy; ++y) {
        AddQueue(y*resx /*+ 0*/);
        AddQueue(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue(/*0*Width* +*/ x);
        AddQueue((resy-1)*resx + x);
    }
	/* for Multi: add also middle lines */
	for(y=1; y<resy-1; ++y) {
        AddQueue(y*resx + resx/2);
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue((resy/2)*resx + x);
    }
	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanBurningShip(p);
	    if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
		
	/* (3) lastly, fill uncalculated areas with neighbor color */
	FillUncalculatedAreas();

	DeallocateBoundary();

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}



/* different algorithms (= no brute, tiling, boundary) */

ULONG DrawBuddhaFractalRandomNumbers (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
/*double cx,cy;*/
double zx, zy/*, xtemp*/;
LONG rpx, rpy;
struct RastPort* Rp;
WORD oldpen;
ULONG loopcounter=0;
/*ULONG wrappedover=0;*/
ULONG i,j;
double fx,fy;
ULONG offset, yoffset=0;
ULONG x,y;
ULONG maxcounter;
/* UBYTE* direct; */
/*double cdx;*/
double tim, tre;
double sim; /* second imaginary point (symmetry) */
UBYTE stop=FALSE;
UBYTE usesymmetry=TRUE;
ULONG resx, resy;
void (*OldPutPixel)(ULONG,ULONG,ULONG);
ULONG maxhist;

/* printf("Grayscale Buddha\n"); */

 CurrentTime (&StartSec,&Dummy);

 /* clear screen */
 /*SetRast(Win->RPort,0);*/ 
 Rp = Win->RPort;
 SetAPen(Rp, oldpen = 0);

 maxhist=NEBULA_WIDTH*NEBULA_HEIGHT*3;
 
 OldPutPixel=PutPixel;
 /*if (DD_BPP>=3) PutPixel=PutPixelPenGray2RGB; */
 	
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 fx=(resx)/(IMAX-IMIN);
 fy=(resy)/(RMAX-RMIN);
  
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

 /* set grayscale palette */
/* cdx=256 / (double)(CURRENT_MAX_COLORS-RESERVED_PENS);
 for (Color=0; Color<=CURRENT_MAX_COLORS-RESERVED_PENS; Color++) 
 {
 	SetRGB32(ViewPortAddress(Win), Color+RESERVED_PENS, (ULONG)(Color*cdx)<<24,(ULONG)(Color*cdx)<<24,(ULONG)(Color*cdx)<<24); /* nr, r, g, b - rgb must be left-adjusted! (OMG) */
/* }
*/ 
 /* reset counters */
 for (offset=0; offset<(a2-a1+1)*(b2-b1+1); offset++)
 {
	BuddhaCounter[offset]=0;
 }
 		
 while (!stop)
 {
    loopcounter++;
	
	/* keep original intervals for random points */
	zx = (rand() / (double)RAND_MAX) * (double)(4.0) - 2.0;	
	zy = 2.7 - (rand() / (double)RAND_MAX) * (double)(5.4); 
		
	i = j = MAX_ITERATIONS + 1;	
	
	/* calculate z0 ... zn */
	i=BuddhaIterationsVampire(i,zx,zy,re,im);

	/* check if orbits should be drawn: BUDDHA: i>BUDDHA_MIN_ITERATIONS vs ANTIBUDDA i==0  */
	if (
		((ANTIBUDDHA) && (i==0))
		||
		((!ANTIBUDDHA) && (i) && ((MAX_ITERATIONS-i)>BUDDHA_MIN_ITERATIONS))
	   )
	{
		/* i>0 => (zx,zy) is not part of the set => use it for Buddhabrot */
		while (j!=i)
		{
			tim=im[j];
			tre=re[j];
			
			if (
				(tim>IMIN) && (tim<IMAX) 
			  	&& (tre>RMIN) && (tre<RMAX)
			   )
			{  
				rpx = (tim - (double)IMIN) * fx;
				rpy = (tre - (double)RMIN) * fy;
				
				yoffset = rpy * resx;
				offset = yoffset + rpx;

		    	if (offset<maxhist) BuddhaCounter[offset]++;
			}
			
			/* use symmetry and draw a second point */
			if (usesymmetry) 
			{
				sim=-tim;
				if (
					(sim>IMIN) && (sim<IMAX) 
				  	&& (tre>RMIN) && (tre<RMAX)
				   )
				{  
					rpx = (sim - (double)IMIN) * fx;
					
					/*offset = rpy * (a2-a1+1) + rpx;*/
					offset = yoffset + rpx;
		    		
					if (offset<maxhist) BuddhaCounter[offset]++;
				}
			}
			
			/* increment loopcounter (which becomes then a "orbitsdrawncounter") */
			loopcounter++;
			
			j--;
		}
		
	}

	/* give user possibility to interrupt calculation each line */
	if (loopcounter%50==0)
	{
			if (InterruptDrawing(Win,a1,b1,a2,b2)) {
			
				/*
				// restore random palette
				for (Color=RESERVED_PENS; Color<=255; Color++) 
 				{
 					SetRGB32(ViewPortAddress(Win), Color, Color,256-Color,(Color*16)%256); // nr, r, g, b
 				}
 				*/
				return TRUE;
			}
			
			if (loopcounter%50000==0) /* 50000 */ 
			{
				/* redraw screen from counters */
				/* find new maxcounter */
				
				maxcounter=1;
				for (offset=0; offset<resx*resy; offset++)
				{
					if (BuddhaCounter[offset]>maxcounter) maxcounter=BuddhaCounter[offset];
				}
				
				for (y=b1; y<=b2; y++) 
				{
					offset=y*resx;
					for (x=a1; x<=a2; x++) 
					{
						/*  Color = BuddhaCounter[offset+x] / divider + RESERVED_PENS; */ /* Buddha "emerging from the dark" ... :) */
						Color =  (CURRENT_MAX_COLORS - RESERVED_PENS) * sqrt((double)BuddhaCounter[offset+x]) / sqrt((double)maxcounter) + RESERVED_PENS; /* Buddha "distilled from the light" ... :) */
						
						PutPixel(x,y,Color);
					}
				}
			}
	}
 }

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 PutPixel=OldPutPixel;
 
 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

void StoreIterations(UBYTE rgb, ULONG i, ULONG j, double fx, double fy, ULONG resx, ULONG resy)
{
double tim, tre, sim;
LONG rpx, rpy;
ULONG offset, yoffset;
/* ULONG loopcounter; */
ULONG rgboffset;
ULONG maxhist;

	if (j<i) return;
	
	rgboffset=NEBULA_WIDTH*NEBULA_HEIGHT*(ULONG)rgb; /* rgb: red = 0, green = 1, blue = 2 */
    maxhist=NEBULA_WIDTH*NEBULA_HEIGHT*3;
	
	while (j!=i)
	{
		tim=im[j];
		tre=re[j];
			
		if (
			(tim>IMIN) && (tim<IMAX) 
		  	&& (tre>RMIN) && (tre<RMAX)
		   )
		{  
			rpx = (tim - (double)IMIN) * fx;
			rpy = (tre - (double)RMIN) * fy;
			
			yoffset = rgboffset + rpy * resx;
			offset = yoffset + rpx;
			
			if (offset<maxhist) BuddhaCounter[offset]++;
		}
			
		/* use symmetry and draw a second point */
		if (BUDDHA_USE_SYMMETRY) 
		{
			sim=-tim;
			if (
				(sim>IMIN) && (sim<IMAX) 
			  	&& (tre>RMIN) && (tre<RMAX)
			   )
			{  
				rpx = (sim - (double)IMIN) * fx;
				
				/*offset = rpy * (a2-a1+1) + rpx;*/
				offset = yoffset + rpx;
	    		
				if (offset<maxhist) BuddhaCounter[offset]++;
			}
		}
		
		/* increment loopcounter (which becomes then a "orbitsdrawncounter") */
/*		loopcounter++; // this is no longer needed ?!? */
		
		j--;
	}
}

void printhistogramstats(void) 
{
	ULONG i;
	ULONG redpixels=0, bluepixels=0, greenpixels=0;
	ULONG totredpixels=0, totbluepixels=0, totgreenpixels=0;
	ULONG maxred=0, maxblue=0, maxgreen=0;
	ULONG histoffsetgreen, histoffsetblue; 
	
	histoffsetgreen=NEBULA_WIDTH*NEBULA_HEIGHT;
	histoffsetblue=histoffsetgreen*2;
	
	for (i=0; i<(DD_WIDTH*DD_HEIGHT); i++)
	{
		
		
		if (BuddhaCounter[i]) 
		{
			redpixels++;
			totredpixels+=BuddhaCounter[i];
		}
		
		if (BuddhaCounter[i+histoffsetgreen])
		{
			greenpixels++;
			totgreenpixels+=BuddhaCounter[i+histoffsetgreen];
		}
		
		if (BuddhaCounter[i+histoffsetblue])
		{
			bluepixels++;
			totbluepixels+=BuddhaCounter[i+histoffsetblue];
		}
		
		if (BuddhaCounter[i]>maxred) maxred=BuddhaCounter[i];
		if (BuddhaCounter[i+histoffsetgreen]>maxgreen) maxgreen=BuddhaCounter[i+histoffsetgreen];
		if (BuddhaCounter[i+histoffsetblue]>maxblue) maxblue=BuddhaCounter[i+histoffsetblue];
	
	}
	
	printf("MAXCOUNTERS: red: %u green: %u blue: %u\n", maxred, maxgreen, maxblue);
	printf("PIXELS:      red: %u green: %u blue: %u\n", redpixels, greenpixels, bluepixels);
	printf("TOTPIXELS:   red: %u green: %u blue: %u\n", totredpixels, totgreenpixels, totbluepixels);
	printf("TOTALTOTAL: %u\n", totredpixels+totgreenpixels+totbluepixels);
	
}

UWORD LoadMandel4Buddha(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = 1.5-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
/*	Color = i = Mandel68k_FPU_Vampire(ITERATIONS-1,cx,cy); */
	Color = i = FractalInCGeneric(ITERATIONS-1,cx,cy);
	
	
	if (Color)
    {
     	Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

   	   	Color += RESERVED_PENS;
    }
	PutPixel(x,y,Color);
   
    Done[p] |= Loaded;
	return Data[p] = Color;
}

void ScanMandel4Buddha(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadMandel4Buddha(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadMandel4Buddha(p-1) != center);
    r=rr && (LoadMandel4Buddha(p+1) != center);
    u=uu && (LoadMandel4Buddha(p-resx) != center);
    d=dd && (LoadMandel4Buddha(p+resx) != center);
    /* process the queue (which is actually a ring buffer) */
    if (l) AddQueue(p-1);
    if (r) AddQueue(p+1);
    if (u) AddQueue(p-resx);
    if (d) AddQueue(p+resx);
    /* the corner pixels (nw,ne,sw,se) are also neighbors */
    if((uu&&ll)&&(l||u)) AddQueue(p-resx-1);
    if((uu&&rr)&&(r||u)) AddQueue(p-resx+1);
    if((dd&&ll)&&(l||d)) AddQueue(p+resx-1);
    if((dd&&rr)&&(r||d)) AddQueue(p+resx+1);
}

ULONG DrawMandelFractalBoundary4Buddha (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
UWORD x, y;

/* printf("Mandel boundary 4 Buddha\n"); */

 /* screenbuffer=Win->RPort->BitMap->Planes[0]; */ /* 20240406 */
 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);
  
 Data=screenbuffer;
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 INCREMREAL = ((double)fabs (4.0)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = ((double)fabs (3.0)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	TempWin = Win;
	
	/* allocation check */
	if (!AllocateBoundary()) return 0;

	ClearBoundaryBuffers();
	    
	/* (1) begin by adding the screen edges into the queue */
    for(y=0; y<resy; ++y) {
        AddQueue(y*resx /*+ 0*/);
        AddQueue(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue(/*0*Width* +*/ x);
        AddQueue((resy-1)*resx + x);
    }
	for(y=1; y<resy-1; ++y) {
        AddQueue(y*resx + resx/2);
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue((resy/2)*resx + x);
    }
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanMandel4Buddha(p);
	    if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
		
	/* (3) lastly, fill uncalculated areas with neighbor color */
	/* FillUncalculatedAreas(); */

	/* DeallocateBoundary(); */

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 return 0;
}


ULONG DrawTrueColorBuddhaFractalRandomNumbers (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , StartTicks = NULL, EndTicks = NULL;
double zx, zy;
struct RastPort* Rp;
ULONG loopcounter=0;
ULONG i,j;
double fx,fy;
ULONG offset; 
ULONG x,y;
UBYTE stop=FALSE;
ULONG resx, resy;
UBYTE buddhared, buddhagreen, buddhablue;
ULONG divred, divgreen, divblue;
ULONG maxred, maxgreen, maxblue;
ULONG deltaiter;
struct IntuiText BTxt;
UBYTE MaxTxt[200];
double mdx, mdy;
ULONG redrawcounter;
double distx, disty;
LONG x2, y2;
ULONG temp_algorithm;
UBYTE showpoints=TRUE;
ULONG tempiter;
ULONG histoffsetgreen, histoffsetblue, maxhist;
ULONG deltaseconds, lastdeltaseconds, refreshseconds, newseconds;
 
 /* clear screen */
 Rp = Win->RPort;
 /*SetRast(Rp,0);*/ 
 
 /* maxcounters text */
 BTxt.FrontPen=2;
 BTxt.BackPen=0;
 BTxt.DrawMode=JAM1;
 BTxt.LeftEdge=10;
 BTxt.TopEdge=10;
 BTxt.ITextFont=NULL;
 BTxt.IText=MaxTxt;
 BTxt.NextText=NULL;

 /* define histogram dimension */
 NEBULA_WIDTH = Win->Width;
 NEBULA_HEIGHT = Win->Height;
 histoffsetgreen=NEBULA_WIDTH*NEBULA_HEIGHT;
 histoffsetblue=histoffsetgreen*2;
 maxhist=histoffsetgreen*3;
 
 /* draw boundary trace first to obtain border points */
 if ((algorithm_buddha==BUDDHA_BOUNDARY) || (algorithm_buddha==BUDDHA_SWITCH)) 
 {
 	tempiter=MAX_ITERATIONS;
	MAX_ITERATIONS=255; /* set boundary trace fix to 256 iterations */
 	DrawMandelFractalBoundary4Buddha (Win,a1,b1,a2,b2);
 	MAX_ITERATIONS=tempiter;
 }
 
 /* mandelbrot deltas */
 mdx = (fabs (4.0)) / ((double) (a2 - a1 + 1));
 mdy = (fabs (3.0)) / ((double) (b2 - b1 + 1));

/*
 // show points marked with "Loaded" (= borders from boundary trace algorithm)
 for (y=0;y<DD_HEIGHT; y++)
 {
	for (x=0;x<DD_WIDTH; x++) 
	{
		if (Done[y*DD_WIDTH+x] & Loaded) PutPixelRGB(x,y,0xff, 0x00, 0x00);	
	} 
 }
*/ 
/*
 printf("QueueHead: %u\n", QueueHead);
 
 i=5000;
 while ((Queue[i]!=0) && (i<DD_WIDTH*DD_HEIGHT))  i++;
 printf("Check QueueHead: %u\n", i);
 
 /* show queue */
/*
 for (i=0; i<DD_HEIGHT*DD_WIDTH; i++)
 {
 	if ((p = Queue[i])!=0)
	{
		y = p / DD_WIDTH;
		x = p % DD_WIDTH;
 		PutPixelRGB(x,y,0xff, 0x00, 0x00);
 	}
 }
*/	
 CurrentTime (&StartSec,&StartTicks);

 redrawcounter= 10000;
 
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 fx=(resx)/(IMAX-IMIN);
 fy=(resy)/(RMAX-RMIN);
  
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

 /* reset counters or leave them as is (in case of resume) */
 if (!BUDDHA_RESUME)
 {
 	for (offset=0; offset<maxhist; offset++)
 	{
		BuddhaCounter[offset]=0;
	}
 } /* else PrintHistData(BuddhaCounter); */

 refreshseconds=1;
 lastdeltaseconds=deltaseconds=StartSec;
 
 while (!stop)
 {
    loopcounter++;
	if (algorithm_buddha==BUDDHA_SWITCH)
	{
		if (loopcounter & 1<<BUDDHA_RANDOMIZATION_FREQUENCY) temp_algorithm=BUDDHA_RANDOM;
		else temp_algorithm=BUDDHA_BOUNDARY;
	} else temp_algorithm=algorithm_buddha;
	
	/* disable visual debugging for used pixels / values */
	showpoints=0;
	
	/* keep original intervals for random points */
	if (temp_algorithm==BUDDHA_RANDOM)
	{
		zx = (rand() / (double)RAND_MAX) * (double)(4.0) - 2.0;	
		zy = (rand() / (double)RAND_MAX) * (double)(3.0) - 1.5; 
	}
	else 
	{
		i=(ULONG)(rand() / (double)RAND_MAX * (double)QueueHead);
		p = Queue[i];
		x = p%DD_WIDTH;
		y = p/DD_WIDTH;
	
		/* show pixel used for rendering */
		if (showpoints) PutPixelRGB(x,y,0x00, 0xff, 0x00); 
			
		zx = x*mdx-2;
		zy = y*mdy-1.5;
	}

	/* randomization for BUDDHA_BOUNDARY */
	if ((temp_algorithm==BUDDHA_BOUNDARY) && (BUDDHA_BOUNDARY_RANDOMIZATION!=0)) 
	{
		distx= (BUDDHA_BOUNDARY_RANDOMIZATION/2.0) - (double) rand() / (double)RAND_MAX*BUDDHA_BOUNDARY_RANDOMIZATION;
		disty= (BUDDHA_BOUNDARY_RANDOMIZATION/2.0) - (double) rand() / (double)RAND_MAX*BUDDHA_BOUNDARY_RANDOMIZATION;
	
		zx+=distx;
		zy+=disty;
	}

	/* show pixel used for rendering */
	if (showpoints)
	{
		x2= (zx+2) / mdx;
		y2= (zy+1.5) / mdy;
	 
	 	/* clipping */
/*
	 	if (x2<0) x2=0;
		if (y2<0) y2=0;
		if (x2>(DD_WIDTH-1)) x2=DD_WIDTH-1;
		if (y2>(DD_HEIGHT-1)) y2=DD_HEIGHT-1;
*/	
		if ((x2>0) && (x2<DD_WIDTH-1) && (y2>0) && (y2<DD_HEIGHT-1))
		{
			if (temp_algorithm==BUDDHA_RANDOM) PutPixelRGB(x2,y2,0xff, 0xff, 0x00);
			else PutPixelRGB(x2,y2,0x00, 0xff, 0xff); 
		}
	}
	
	/* printf("Queue[%u]: %u => x: %u y: %u <=> (%1.2f,%1.2f)\n", i, p, x, y, zx, zy); */
		
	i = j = MAX_ITERATIONS + 1;	
	
	/* calculate z0 ... zn */
	i=FractalInCGenericStoreIterations(i,zx,zy); /*	i=BuddhaIterationsVampire(i,zx,zy,re,im); i=MultiMandelInCStoreIterations(i,zx,zy,re,im); */
	
	/* check if orbits should be drawn: BUDDHA: i>BUDDHA_MIN_ITERATIONS vs ANTIBUDDA i==0  */
	deltaiter=MAX_ITERATIONS-i;
	if (
		((ANTIBUDDHA) && (i==0))
		||
		((!ANTIBUDDHA) && (deltaiter<FM_REDITER) && (deltaiter>FM_REDMIN)) /* && ((MAX_ITERATIONS-i)>BUDDHA_MIN_ITERATIONS)) */
	   )
	{
		
		StoreIterations(0,i,j,fx,fy, resx, resy);
	}

	if (
		((ANTIBUDDHA) && (i<=FM_GREENITER))
		||
		((!ANTIBUDDHA) && (deltaiter<FM_GREENITER) && (deltaiter>FM_GREENMIN))
	   )
	{
		StoreIterations(1,i,j,fx,fy, resx, resy);
	}

	if (
		((ANTIBUDDHA) && (i<=FM_BLUEITER))
		||
		((!ANTIBUDDHA) && (deltaiter<FM_BLUEITER) && (deltaiter>FM_BLUEMIN))
	   )
	{
		
		StoreIterations(2,i,j,fx,fy, resx, resy);
	}

	if (loopcounter%50==0)
	{
			/* give user possibility to interrupt calculation every 50ieth loop */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) stop=1;
		
		if (!showpoints)
		{				
			/* if ((stop) || (loopcounter%redrawcounter==0)) /* 50000 */ 
			CurrentTime (&newseconds,&StartTicks);
			deltaseconds=newseconds-lastdeltaseconds;
			
			/*if (loopcounter%redrawcounter==0)*/
			if ((!stop) && (deltaseconds>=refreshseconds))
			{
				lastdeltaseconds=newseconds;
				if (refreshseconds<=60) refreshseconds*=2;
				/*printf("refreshseconds: %u deltaseconds: %u\n", refreshseconds, deltaseconds);*/
				
				/* find new maxcounters */
				maxred=maxgreen=maxblue=0;
				for (offset=0; offset<resx*resy; offset++)
				{
					if (BuddhaCounter[offset]>maxred) maxred=BuddhaCounter[offset];
					if (BuddhaCounter[offset+histoffsetgreen]>maxgreen) maxgreen=BuddhaCounter[offset+histoffsetgreen];
					if (BuddhaCounter[offset+histoffsetblue]>maxblue) maxblue=BuddhaCounter[offset+histoffsetblue];
				}
				
				for (y=b1; y<=b2; y++) 
				{
					offset=y*resx;
					for (x=a1; x<=a2; x++) 
					{
						divred=max(maxred,1);
						divgreen=max(maxgreen,1);
						divblue = max(maxblue,1);
						
						buddhared   = 255 * sqrt((double)BuddhaCounter[offset+x]) / sqrt((double)divred); 
						buddhagreen = 255 * sqrt((double)BuddhaCounter[histoffsetgreen+offset+x]) / sqrt((double)divgreen);
						buddhablue  = 255 * sqrt((double)BuddhaCounter[histoffsetblue+offset+x]) / sqrt((double)divblue);
						
						/* PutPixel(x,y,Color); */
						PutPixelRGB(x,y, buddhared, buddhagreen, buddhablue);
					}
				}
				
				if (!stop)
				{
					sprintf(MaxTxt,"%u:%d|%d|%d",loopcounter,maxred,maxgreen,maxblue);
					if (SHOW_MAXCOUNTERS) PrintIText(Win->RPort,&BTxt,0,0);	
				} else return TRUE;
			}
		}
		/* if (loopcounter==200000) stop=TRUE; */

	}
 }

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&EndTicks);

 DisplayBeep (Win->WScreen);

 /* draw fractal in the end */
 /* find new maxcounters */
 maxred=maxgreen=maxblue=0;
 for (offset=0; offset<resx*resy; offset++)
 {
	if (BuddhaCounter[offset]>maxred) maxred=BuddhaCounter[offset];
	if (BuddhaCounter[offset+histoffsetgreen]>maxgreen) maxgreen=BuddhaCounter[offset+histoffsetgreen];
	if (BuddhaCounter[offset+histoffsetblue]>maxblue) maxblue=BuddhaCounter[offset+histoffsetblue];
 }
				
 for (y=b1; y<=b2; y++) 
 {
	offset=y*resx;
	for (x=a1; x<=a2; x++) 
	{
		divred=max(maxred,1);
		divgreen=max(maxgreen,1);
		divblue = max(maxblue,1);
						
		buddhared   = 255 * sqrt((double)BuddhaCounter[offset+x]) / sqrt((double)divred); 
		buddhagreen = 255 * sqrt((double)BuddhaCounter[histoffsetgreen+offset+x]) / sqrt((double)divgreen);
		buddhablue  = 255 * sqrt((double)BuddhaCounter[histoffsetblue+offset+x]) / sqrt((double)divblue);
						
		/*PutPixel(x,y,Color); */
		PutPixelRGB(x,y, buddhared, buddhagreen, buddhablue);
	}
 }
				
 /*printf("Seconds: %u Ticks: %u Ticks/second: %d\n", EndSec-StartSec, EndTicks-StartTicks, (EndTicks-StartTicks)/(EndSec-StartSec)); */
 /*printhistogramstats(); */
 
 if ((algorithm_buddha!=BUDDHA_RANDOM) && (Done)) DeallocateBoundary();
 
 BUDDHA_RESUME=FALSE;
 MenuResumeOff(Win); 
 
 return (EndSec-StartSec);
}
