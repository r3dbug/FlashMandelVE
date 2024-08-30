
#include "shared.h"
#include "enum.h"

UWORD InterruptDrawing(struct Window*, const LONG,const LONG,const LONG,const LONG);
BOOL   CheckBox      (struct RastPort *,const LONG,const LONG,const LONG,const LONG);
ULONG GetCyclePeriod(double *, double *, ULONG, double);

BOOL AllocIterArray(void)
{
	if (re) 
    {
    	FreeVec(re);
    	re=NULL;
    }
	re = AllocVec(sizeof(double) * MAXITERPERIODICITY, MEMF_FAST | MEMF_CLEAR);
    
    if (im) 
    {
    	FreeVec(im);
    	im=NULL;
    }
	im = AllocVec(sizeof(double) * MAXITERPERIODICITY, MEMF_FAST | MEMF_CLEAR);
    
	if (re && im) return 1;
    else 
    {
		if (re) { FreeVec(re); re=NULL; } 
        if (im) { FreeVec(im); im=NULL; }
        return NULL;   
    }
}

void FreeIterArray(void)
{
	if (re) 
    {
    	FreeVec(re);
    	re=NULL;
    }
	if (im) 
    {
    	FreeVec(im);
    	im=NULL;
    }
}

void InitBuddha(void)
{
	BUDDHA_DIMENSIONS[0] = 0;
  	BUDDHA_DIMENSIONS[1] = 0;
  	BuddhaCounter = NULL; 
    re=NULL;
    im=NULL;
    HAMMap=NULL;
    edgequeue=NULL;
    lemniscates=NULL;
	queue_i=lemni_i=0;
}

void FreeBuddha(void) 
{
 	/*printf("FreeBuddha()\n");*/
    	
    /* free memory and set all pointers to NULL */
	if (BuddhaCounter) {
    	FreeVec(BuddhaCounter); 
		BuddhaCounter=NULL;	
    }	
    
    if (re) 
    {	
    	FreeVec(re);  /* Array with orbits for periodicity checking */
  		re=NULL;	
    }	
  
    if (im) 
    {
    	FreeVec(im);
		im=NULL;	
    }	

    if (HAMMap) 
    {
    	FreeVec(HAMMap);
    	HAMMap=NULL;
    }
    
    if (edgequeue) 
    {
    	FreeVec(edgequeue);
    	edgequeue=NULL;
        queue_i=0;
    }
    
    if (lemniscates) 
    {
    	FreeVec(lemniscates);
    	lemniscates=NULL;
    	lemni_i=0;
    }
    
    /* set dimensions to 0 */
    BUDDHA_DIMENSIONS[0]=0;
    BUDDHA_DIMENSIONS[1]=0;
}

void FreeHitmap(void)
{
	FreeVec(HAMMap);
    HAMMap=NULL;
    HITMAP_MEMSIZE=0;
}

BOOL AllocBuddha(void)
{
	ULONG needed1 = sizeof(ULONG)*3*DD_WIDTH*DD_HEIGHT; /* counters */
    ULONG needed3 = sizeof(double)*MAXITERPERIODICITY; /* re & im */
    ULONG needed4 = sizeof(ULONG)*(DD_WIDTH>>HITMAP_SHIFT)*DD_HEIGHT*HITMAP_EXTENSION; /* for HAMMAP - a lot of memory! */
    ULONG needed5 = DD_WIDTH*DD_HEIGHT*sizeof(ULONG); /* edges+lemniscates => less memory will probably also do ... ;) */
    UBYTE error=0xff;
    
    SHOW_MAXCOUNTERS = FALSE;	/* only for debugging */
    
    /*
    printf("AllocBuddha() - AvailMem: %lu\n", AvailMem( MEMF_LARGEST));
    printf("USE_HAMMAP: %u needed4: %u\n", USE_HAMMAP, needed4);
    */
    
    /* first check, if BuddhaCounter has already been allocated */	
    if (BuddhaCounter)
    {
/*        printf("BuddhaCounter already allocated ...\n");
        /* if allocated, check if size is sufficient */
        if (
        	(BUDDHA_DIMENSIONS[0]>=DD_WIDTH)
            &&
            (BUDDHA_DIMENSIONS[1]>=DD_HEIGHT)
           )
        {
        	/* allocated and size is sufficient 
             * => now check if HAMMap is needed
                and allocated 
             */
/*            printf("Allocated size is sufficient ...\n"); */
            if (USE_HAMMAP) 
            {
/*            	printf("USE_HAMMap is true => check/allocate HAMMap ...\n"); */	
                if (HAMMap) 
                { 
                	/* HAMMap already allocated, but it might be too small */
                	if (needed4>HITMAP_MEMSIZE)
                    {
                		/* existing HAMMap is too small => free and reallocate */
                        /* printf("HAMMap too small => reallocate...\n"); */
/*                       printf("Existing HAMMap is too small => free it and reallocate ...\n"); */
                        FreeVec(HAMMap);
                        HAMMap=AllocVec(needed4, MEMF_FAST | MEMF_CLEAR);
                        if (HAMMap)
                        {
/*                        	printf("Reallocation of HAMMap successful (Size: old: %lu new: %lu=)\n", HITMAP_MEMSIZE, needed4);
*/                            HITMAP_MEMSIZE=needed4;
                            error=FALSE;
                        } 
                        else 
                    	{
/*                        	printf("Reallocation of HAMMap NOT successful (Size: old: %lu new: %lu)\n", HITMAP_MEMSIZE, needed4);
*/							HITMAP_MEMSIZE=0;     
                            error=1;                    
                        }
                    }	
                    else
                    {
/*                    	printf("HAMMap already allocated and memsize sufficient (Size: old: %lu new: %lu)\n", HITMAP_MEMSIZE, needed4);
*/                    	error=FALSE; /* nothing to do => return with success */
                    }
                } 
                else 
                {
/*                	printf("No HAMMap allocated => allocate it for the first time...\n");
*/                	HAMMap=AllocVec(needed4, MEMF_FAST | MEMF_CLEAR);
                    if (HAMMap) 
                    { 
/*                    	printf("HAMMap allocated - size: %u\n", needed4);
*/                        HITMAP_MEMSIZE=needed4;
                        error=FALSE; 
                    } /* allocation HAMMap successful */
                    else 
                    { 
/*                    	printf("HAMMap allocation error\n");
*/                        HITMAP_MEMSIZE=0; 
                        error=1; 
                    }/* allocation of HAMMap failed */
                }
            } else error=FALSE; /* no HAMMap needed */
        	
            /* check edges & lemniscates */
            if (BUDDHA_EDGES)
            {
          		if (!edgequeue) edgequeue=AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
                if (!edgequeue) error=3;
            }
            if (BUDDHA_LEMNISCATES)
            {
          		if (!lemniscates) lemniscates=AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
                if (!lemniscates) error=4;
            }
        }
        else
        {
/*           printf("Allocated size is not sufficient => free memory\n");
*/           /* if size is not sufficient => first free memory */
           FreeBuddha();
           /* now allocate new BuddhaCounter */
/*           printf("Reallocate new memory for Buddha\n");
*/           BuddhaCounter=AllocVec(needed1, MEMF_FAST| MEMF_CLEAR); 		/* allocate enough memory for FullHD: 1920x1080 ULONG */
		   if (USE_HAMMAP) 
           {
/*           		printf("Reallocate HAMMap because memory for entire Buddha has to be reallocated\n");	
*/                HAMMap=AllocVec(needed4, MEMF_FAST | MEMF_CLEAR);	
           }
           re = AllocVec(needed3, MEMF_FAST | MEMF_CLEAR);
  		   im = AllocVec(needed3, MEMF_FAST | MEMF_CLEAR);
    	   
           /* check edges & lemniscates */
           if (BUDDHA_EDGES)
           {
            	if (!edgequeue) edgequeue=AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
           }
           if (BUDDHA_LEMNISCATES)
           {
            	if (!lemniscates) lemniscates=AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
           }   
           
           /* check if allocation worked */   
           if ((BuddhaCounter) && (re) && (im))
           {
           		if (USE_HAMMAP) 
                {
                	if (HAMMap) 
                    {
                    	HITMAP_MEMSIZE=needed4;	
                        error=FALSE;	/* HAMMap needed and allocated */
               	 	}	
                    else 
                    {
                    	HITMAP_MEMSIZE=0;	
                        error=1;				/* fail: HAMMap needed but not allocated */
           			}	
                } else error=FALSE; /* HAMMap not needed => all good */
           } 
           else error=2; /* allocation of main counters failed */     
    	   if ((BUDDHA_EDGES) && (!edgequeue)) error=3;	
           if ((BUDDHA_LEMNISCATES) && (!lemniscates)) error=3;	
        }
    }	
    else
    {
    	/* if BuddhaCounter has not yet been allocated 
     	 * => allocate it
    	 */
/*     	printf("No memory allocated for Buddha => allocate it for the first time ...\n");
*/    	BuddhaCounter=AllocVec(needed1, MEMF_FAST| MEMF_CLEAR); 		/* allocate enough memory for FullHD: 1920x1080 ULONG */
		if (USE_HAMMAP) HAMMap=AllocVec(needed4, MEMF_FAST | MEMF_CLEAR);	
 		re = AllocVec(needed3, MEMF_FAST | MEMF_CLEAR);
  		im = AllocVec(needed3, MEMF_FAST | MEMF_CLEAR);
   		if (BUDDHA_EDGES) edgequeue=AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
 		if (BUDDHA_LEMNISCATES) lemniscates = AllocVec(needed5, MEMF_ANY | MEMF_CLEAR);
 
    	/* check if allocation worked */   
    	if ((BuddhaCounter) && (re) && (im))
    	{
    		if (USE_HAMMAP) 
    	    {
    	       	if (HAMMap) 
                {
/*                	printf("HAMMap successfully allocated (for the first time) - size: %u\n", needed4);	
*/                    HITMAP_MEMSIZE=needed4;	
                    error=FALSE;	/* HAMMap needed and allocated */
    	   		}	 	
                else 
                {
/*                	printf("HAMMap allocation FAILED (needed size: %u)\n", needed4);	
*/                    HITMAP_MEMSIZE=0;	
                    error=1;				/* HAMMap needed but not allocated */
    	 		}   
            } else error=FALSE; /* HAMMap not needed => all good */
    		
            if ((BUDDHA_EDGES) && (!edgequeue))
                error=3; /* edgequeue allocation failed */            
            if ((BUDDHA_LEMNISCATES) && (!lemniscates))
                error=4; /* lemniscates allocation failed */  
        } 
    	else error=2; /* allocation failed */     
    }
    
    if (!error) 
    {
    	/* set dimensions and return success */	
        BUDDHA_DIMENSIONS[0]=DD_WIDTH;
        BUDDHA_DIMENSIONS[1]=DD_HEIGHT;
        /*printf("Success allocation Buddhacounters:\n Error %u\nBuddhaCounter: %p re: %p im: %p HAMMap: %p\nDD_WIDTH: %u DD_HEIGHT: %u (NEBUBLA_WIDTH: %u NEBULA_HEIGHT: %u\n",
        	error, BuddhaCounter, re, im, HAMMap, DD_WIDTH, DD_HEIGHT, NEBULA_WIDTH, NEBULA_HEIGHT);
        */
        return TRUE; 
    }
    else 
    {
        /* printf("Error %u allocating Buddhacounters ...\nBuddhaCounter: %p re: %p im: %p HAMMap: %p\n",
        	error, BuddhaCounter, re, im, HAMMap);
        */
        /* since insufficient Buddha memory is of no use => free it */
        /* (dimensions are reset to 0 by FreeBuddha() */
        FreeBuddha();
        return FALSE;
    }
}


LONG lround(double v)
{
	if (v>=0) return (LONG)(v+0.5);
	else return (LONG)(v-0.5);
}

double RenormalizedIterationCount(ULONG i, double fx, double fy, double p)
{
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
	while ((i) && (zx*zx+zy*zy<=BAILOUT_VALUE)) {
		xtemp = zx * zx - zy * zy;
		zy = 2 * zx * zy + cy;
		zx = xtemp + cx;
		i--;
	}	
	return i;
}

ULONG MandelInCStoreIterations(ULONG i, double cx, double cy)
{
    /* this is a simple C2ASM wrapper for the moment */
    /* to do it properly, main fractal iteration function pointer would have to be the
       same (pointer to asm function) for all functions (not possible as long as there are
       C functions remaining (..)).
     */
     
    return Mandel68k_FPU_Classic_SI(i,cx,cy);
}

ULONG JuliaInC(ULONG i, double zx, double zy, double cx, double cy) {
double xtemp;
	while ((i) && (zx*zx+zy*zy<=BAILOUT_VALUE)) {
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
	
	while ((i) && (ax*ax+ay*ay<=BAILOUT_VALUE))
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
	 
	while ((i) && (ax*ax+ay*ay<=BAILOUT_VALUE))
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
	
	while ((i) && (ax*ax+ay*ay<=BAILOUT_VALUE))
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

	while ((i) && ((ax2ay2=ax*ax+ay*ay)<=BAILOUT_VALUE))
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
	
    tortoise=i;
	hare=i-1;
	isperiodic=0;
    
	while ((i) /*&& (!isperiodic) */ && ((ax2ay2=ax*ax+ay*ay)<=BAILOUT_VALUE))
	{
		patan2ayax=p*atan2C(ax,ay);		/* 31.61 */
        
		powvar = pow((ax2ay2),p2); 
		
		ax = powvar * cos(patan2ayax) + cx;
		ay = powvar * sin(patan2ayax) + cy;

		i--;

/* periodicity checking disabled for the moment */		
/*		if (i==hare) 
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
/* end of disabled periodicity checking */
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
	while ((i) && /*(!isperiodic) &&*/ ((ax2ay2=ax*ax+ay*ay)<=BAILOUT_VALUE))
	{
		patan2ayax=p*atan2C(ax,ay);
		
		powvar = powFPU(ax2ay2,p2);
		
		ax = powvar * cos(patan2ayax) + cx;
		ay = powvar * sin(patan2ayax) + cy;

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
 double cx,cy;
 WORD x, y;
 double power;

 power = (FractalType==MULTIFLOAT) ? MultiPowerFloat : MultiPower;

 GlobalP = (FractalType == MULTIINT) ? MultiPower : MultiPowerFloat; 

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
			cx = x*INCREMREAL+RMIN;	
			
			Color=MultiMandelInCGeneric(ITERATIONS,cx,cy);

			PutPixelWithColorMode(x,y,Color);
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 return 0;
}

/* (1d) MultiJulia */

ULONG DrawMultiJuliaFractalBrute (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
 ULONG Color;
 double cx,cy;
 WORD x, y;
 double power;

 power = (FractalType==MULTIFLOAT) ? MultiPowerFloat : MultiPower;

 GlobalP = (FractalType == MULTIINT) ? MultiPower : MultiPowerFloat; 
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
			cx = x*INCREMREAL+RMIN;	
			
			Color=MultiJuliaInCGeneric(ITERATIONS,cx,cy);

			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 return 0; 
}

/* 1e) burning ship */

ULONG BurningShipInC(ULONG i, double x, double y)
{
double xn, yn, cx, cy, xtemp;

	xn=0;
	yn=0;
	
	cx = x;
	cy = -y;   /* flip it around, because otherwise ship will be upside down ... */
	
	while ((i) && (xn*xn + yn*yn < BAILOUT_VALUE))
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
	
	while ((i) && (xn*xn + yn*yn < BAILOUT_VALUE))
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
	
	while ((i) && (xn*xn + yn*yn < BAILOUT_VALUE))
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
 double cx,cy;
 WORD x, y;

 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
			cx = x*INCREMREAL+RMIN;	
			
			Color=BurningShipInCGeneric(ITERATIONS,cx,cy);
			
			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 return 0; 
}

/* Tricorn */
/* the following 2 functions are not used */
/*
ULONG TricornInC(ULONG i, double x, double y)
{
double xn, yn, cx, cy, xtemp;

	xn=0;
	yn=0;
	
	cx = x;
	cy = y;   
	
	while ((i) && (xn*xn + yn*yn < BAILOUT_VALUE))
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
 double cx,cy;
 WORD x, y;
 
 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
/*			cx = x*INCREMREAL+RMIN;	
			
			Color=TricornInC(ITERATIONS,cx,cy);
			
			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
/*		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 return 0; 
}
*/
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
  INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

  INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

  ITERATIONS = MAX_ITERATIONS + 1;

  if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

  (*H_LINE) (Win->RPort,a1,a2-a1+1,b1);

  (*V_LINE) (Win->RPort,b1+1,b2-1,a2);

  (*H_LINE) (Win->RPort,a1,a2-a1+1,b2);

  (*V_LINE) (Win->RPort,b1+1,b2-1,a1);

  RectangleDraw (Win,a1,b1,a2,b2);

  if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

  DisplayBeep (Win->WScreen);

  return 0; 
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

  WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
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
  WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
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

  WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
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

     CIM += INCREMIMAG;
  }
}


/**********************************************************************************************/
/********************************** BOUNDARY TRACE ********************************************/
/**********************************************************************************************/

/* (3) Boundary trace */

/* Memory allocation */
int AllocateBoundary(void) {

    /* check if boundary has already been allocated */
    /* => this can occur when a previous calculation was interrupted */

    if ((Done!=NULL) || (Queue!=NULL)) 
    {
    	/*printf("Interrupted calculation => first deallocate boundary queue ...\n");*/
    	DeallocateBoundary();
    }
    
    /* calculate size and allocate */
    DataSize=sizeof(UWORD)*(DD_WIDTH*DD_HEIGHT);  /* reserve words for 0xffff iterations */
    DoneSize=sizeof(UBYTE)*(DD_WIDTH*DD_HEIGHT);
    QueueSize=sizeof(ULONG)*((DD_WIDTH*DD_HEIGHT)*4);
    Data=AllocVec(DataSize, MEMF_PUBLIC | MEMF_CLEAR); 
    Done=AllocVec(DoneSize, MEMF_PUBLIC | MEMF_CLEAR);
    Queue=AllocVec(QueueSize, MEMF_PUBLIC | MEMF_CLEAR);

    /* return success */
    if ((Data==NULL) || (Done==NULL) || (Queue==NULL)) return 0;
    else return 1;
}

void DeallocateBoundary(void) {
    if (Queue) {
		FreeVec(Queue);
        Queue=NULL;
	}
    if (Done) {
		FreeVec(Done);
    	Done=NULL; 
    }
    if (Data) {
		FreeVec(Data); /* Data pointer needs more investigation ... */
    	Data=NULL;	
    }
}

/* Build the queue */
void AddQueue(unsigned p) {
    
    if(Done[p] & Queued) return;
    Done[p] |= Queued;
    Queue[QueueHead++] = p;
    if(QueueHead == QueueSize) QueueHead = 0;
}

void ClearHAMMap(void)
{
  	ULONG i;
    
    if (HAMMap)
    {
    	for (i=0; i<(DD_WIDTH>>HITMAP_SHIFT)*DD_HEIGHT*HITMAP_EXTENSION; i++)
    	{
    		HAMMap[i]=0;
        }
	}
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
/* (3d) MultiJulia */
/* 1d) Burning ship */

/******************************/
/* Generic Boundary functions */
/******************************/

ULONG (*FractalIterationGeneric)(ULONG,double,double);

UWORD LoadFractalGeneric(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = FractalIterationGeneric(ITERATIONS,cx,cy);
	
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

void ScanFractalGeneric(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadFractalGeneric(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadFractalGeneric(p-1) != center);
    r=rr && (LoadFractalGeneric(p+1) != center);
    u=uu && (LoadFractalGeneric(p-resx) != center);
    d=dd && (LoadFractalGeneric(p+resx) != center);
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

ULONG DrawFractalBoundaryGeneric (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
 UWORD x, y;
 
 /*Data=*/screenbuffer=GetBitMapPtr(Win->RPort->BitMap);
 
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	TempWin = Win;
	
	/* allocation check */
	if (!AllocateBoundary()) return 0;
 
	/* (1) begin by adding the screen edges into the queue */
    /* vertical */
    for(y=0; y<resy; ++y) {
        AddQueue(y*resx /*+ 0*/);
        AddQueue(y*resx + (resx-1));
    }
    /* horizontal */
    for(x=1; x<resx-2; ++x) {
        AddQueue(/*0*Width* +*/ x);
        AddQueue((resy-1)*resx + x);
    }
    
    /* add also middle lines */
    for(y=1; y<resy-1; ++y) {
	        AddQueue(y*resx + resx/2);
	}
	for(x=1; x<resx-1; ++x) {
		 AddQueue((resy>>1)*resx + x);        
         /*   
            AddQueue((resy/3)*resx + x);
            AddQueue((resy/3*2)*resx + x);
		 */	
    }
    	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanFractalGeneric(p);
	    if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
		
	/* (3) lastly, fill uncalculated areas with neighbor color */
	if (BOUNDARY_FILL) FillUncalculatedAreas();

	if (FractalType!=BUDDHA) DeallocateBoundary();
 	/* else printf("No DeallocateBoundary() => FractalType==BUDDHA\n"); */
 	return 0; 
}


/* different algorithms (= no brute, tiling, boundary) */

ULONG DrawBuddhaFractalRandomNumbers (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
double zx, zy;
LONG rpx, rpy;
struct RastPort* Rp;
WORD oldpen;
ULONG loopcounter=0;
ULONG i,j;
double fx,fy;
ULONG offset, yoffset=0;
ULONG x,y;
ULONG maxcounter;
double tim, tre;
double sim; /* second imaginary point (symmetry) */
UBYTE stop=FALSE;
UBYTE usesymmetry=TRUE;
ULONG resx, resy;
void (*OldPutPixel)(ULONG,ULONG,ULONG);
ULONG maxhist;

 CurrentTime (&StartSec,&Dummy);

 /* clear screen */
 Rp = Win->RPort;
 SetAPen(Rp, oldpen = 0);

 maxhist=NEBULA_WIDTH*NEBULA_HEIGHT*3;
 
 OldPutPixel=PutPixel;
 	
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 fx=(resx)/(IMAX-IMIN);
 fy=(resy)/(RMAX-RMIN);
  
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

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
			if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
			
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

 PutPixel=OldPutPixel;
 
 CurrentTime (&EndSec,&Dummy);

 return (EndSec-StartSec);
}

void StoreIterationsCoreC(double* re, double* im, ULONG* bc, ULONG i, ULONG j, double fx, double fy, ULONG resx, BOOL dosymmetry)
{
	double tim, tre, sim;
	LONG rpx, rpy;
	ULONG offset, yoffset;

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
			
			yoffset = /*rgboffset +*/ rpy * resx;
			offset = yoffset + rpx;
			
            /*BuddhaCounter*/
            bc[offset]++;
        }
		
        if (dosymmetry)
		{
            sim=-tim;
			rpx = (sim - (double)IMIN) * fx;
				
			offset = yoffset + rpx;
	    		
            /* BuddhaCounter */
            bc[offset]++;
		}	
		j--;
	}
}

void StoreIterations(UBYTE rgb, ULONG i, ULONG j, double fx, double fy, ULONG resx, ULONG resy)
{
	ULONG rgboffset;
	BOOL dosymmetry;
	ULONG *bc;

	if (j<i) return;

	if (USE_SYMMETRY)
	{
		if (BaseFractalType==BURNINGSHIP) dosymmetry=FALSE;
        else if (fabs(fabs(IMIN)-fabs(IMAX))<BUDDHA_AUTO_CENTER_EPSILON) dosymmetry=TRUE;
        else dosymmetry=FALSE;
	} else dosymmetry=FALSE;
    
    rgboffset=NEBULA_WIDTH*NEBULA_HEIGHT*(ULONG)rgb; /* rgb: red = 0, green = 1, blue = 2 */
	
    bc=&BuddhaCounter[rgboffset];

	/* StoreIterationsCoreASM creates Illegal Memory Access on UAE */	
    if (vampire) StoreIterationsCoreASM(re,im,bc,i,j,fx,fy,resx,dosymmetry); /* replaces the following while */
    else StoreIterationsCoreC(re,im,bc,i,j,fx,fy,resx,dosymmetry); /* replaces the following while */
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

ULONG GetCyclePeriod(double *re, double *im, ULONG maxi, double epsilon)
{
	ULONG tortoise=0, hare=1;
    BOOL stop=FALSE;
    double tr, ti, hr, hi;
    double act_eps_re, act_eps_im;
    ULONG r=0;
    
    do 
   	{
    	/* read values from table */
    	tr=re[tortoise]; ti=im[tortoise];
        hr=re[hare]; hi=im[hare];
        
        /* calculate epsilon */
        act_eps_re=fabs(tr-hr); 
        act_eps_im=fabs(ti-hi); 
              
        if ((act_eps_re<=epsilon) && (act_eps_im<=epsilon)) 
        {
            stop=TRUE;
            r=hare-tortoise; 
        }
        else
        {
        	hare+=2;
            tortoise+=1;
            if (hare>=maxi) 
            {
            	r=0;	
                stop=TRUE;
        	}
        }
    } while (!stop);
    
	return r;
}

/************************************************************************************
*************************************************************************************/

void GetSampleNumber(double mdx, double mdy, 
	ULONG *edgequeue, ULONG queue_i, 
	ULONG *lemniscates, ULONG lemni_i)
{
	static UBYTE method=0;
	double zx, zy;    
	ULONG p, x, y, i;
    /* char s[20]; */
    ULONG hmbx, hmby, hmby2;
    BOOL repeat;
    BOOL valid_point=FALSE;
    
    /* choose a point using one or several methods (taking turns) */
    do
    {
    	valid_point=FALSE;
        	
        if (method==0)
        {	
            if (BUDDHA_RANDOM)
            {
                    zx = (rand() / (double)RAND_MAX) * (double)(4.0)/* - 2.0*/;	
					zy = (rand() / (double)RAND_MAX) * (double)(3.0)/* - 1.5*/; 
            		
                    /* additional randomizing to increase precision */
                    /* resolution with RAND_MAX: 0.0000000004656612 => not enough! */
            		zx += (BUDDHA_RANDOMIZE_X*2.0*mdx) 
                    * (
                    	(double)rand()+(double)rand()
                      )
                         
                    / (
                    	(double)RAND_MAX+(double)RAND_MAX
                      ) 
                    - mdx;
            		zy += (BUDDHA_RANDOMIZE_Y*2.0*mdy) * ((double)rand()+(double)rand()) / ((double)RAND_MAX+(double)RAND_MAX) - mdy; 	
				
                	x=zx/mdx;
                	y=zy/mdy; 
                    hmbx=(ULONG)(zx * (double)HITMAP_EXTENSION / mdx);
                        hmby=(ULONG)(zy * (double)HITMAP_EXTENSION / mdy); 
            		zx-=2.0;
                	zy-=1.5;
                    
                    valid_point=TRUE;
            	
            } else method++;
        }
       
        if (method==1)
        {
            if (BUDDHA_EDGES)
            {
            		 i = (ULONG)((double)rand() / (double)RAND_MAX * (double)queue_i);
					 p = edgequeue[i];
					 x = p%DD_WIDTH;
					 y = p/DD_WIDTH;
                     zx = x*mdx-2;
					 zy = y*mdy-1.5;
                     
                     /* randomize */
            		 zx += (BUDDHA_RANDOMIZE_X*2.0*mdx) * ((double)rand() / (double)RAND_MAX) - mdx;
            		 zy += (BUDDHA_RANDOMIZE_Y*2.0*mdy) * ((double)rand() / (double)RAND_MAX) - mdy;
                 	 
                   	 hmbx=(ULONG)((zx+2.0) * (double)HITMAP_EXTENSION / mdx);
                        hmby=(ULONG)((zy+1.5) * (double)HITMAP_EXTENSION / mdy);  
            
            		 valid_point=TRUE;
                     
            } else method++;
        }
         
        if (method==2)
        {
            if (BUDDHA_LEMNISCATES)
        	{
            		 i = (ULONG)((double)rand() / (double)RAND_MAX * (double)lemni_i);
					 p = lemniscates[i];
					 x = p%DD_WIDTH;
					 y = p/DD_WIDTH;
                     zx = x*mdx-2;
					 zy = y*mdy-1.5;
                     
                     /* randomize */
            		 zx += (BUDDHA_RANDOMIZE_X*2.0*mdx) * ((double)rand() / (double)RAND_MAX) - mdx;
            		 zy += (BUDDHA_RANDOMIZE_Y*2.0*mdy) * ((double)rand() / (double)RAND_MAX) - mdy;

					 /* these lines fix the memory bug!? */    				 
                     hmbx=(ULONG)((zx+2.0) * (double)HITMAP_EXTENSION / mdx);
                        hmby=(ULONG)((zy+1.5) * (double)HITMAP_EXTENSION / mdy);  
            
            		 valid_point=TRUE;
                     
            } else method++;
        }
        
        if (method>2) method=0;
     
    	if (valid_point)
        {	
    	    if (USE_HAMMAP)
    	    {	
    	  		if (FractalType != BURNINGSHIP)  	
          		{
                	repeat = HAMMap[
    	                            		hmby * (DD_WIDTH>>HITMAP_SHIFT)
    	                                    +
    	                                    (x>>HITMAP_SHIFT)
    	                                  ] 
    	                                  & 
    	                                  (1<<(hmbx%32));
    	 		}
                else
                {
    				hmby2 = DD_HEIGHT*HITMAP_EXTENSION - hmby - 1; 
             		repeat = HAMMap[
    	                            		hmby2 * (DD_WIDTH>>HITMAP_SHIFT)
    	                                    +
    	                                    (x>>HITMAP_SHIFT)
    	                                  ] 
    	                                  & 
    	                                  (1<<(hmbx%32));
                }   
            } else repeat=FALSE;
                                      
    		if (!repeat) 
    	    {
    			method++;		
    	        if (method>2) method=0;
    	    }
	    } else repeat=FALSE;

    }   
    while (repeat);
        
	NEBULA_SAMPLE_RE=zx;
    NEBULA_SAMPLE_IM=zy;
    NEBULA_SAMPLE_X=x;
    NEBULA_SAMPLE_Y=y;
}

void ShowHAMMap(ULONG* edgequeue, ULONG edge_i, ULONG* lemniscates, ULONG lemni_i) 
{
	ULONG i, p, x, y, dx=10, dy=10;
    UBYTE yshift;
    
    switch (HITMAP_EXTENSION)
    {
    	case  8 : yshift = 3; break;
        case 16 : yshift = 4; break;
        case 32 : yshift = 5; break;
    }
    
    /* alpha blending: a == 0 => write pixel ; a != 0 => don't write pixel */
    if (USE_HAMMAP)
    {
    	switch (HITMAP_VISUALIZATION)
        {
        	case 0 : /* lores */
    				 /* rudimentary (fast) visualization */
        
        			 for (y=0;y<(DD_HEIGHT>>2);y++)
    				 {
 						for (x=0;x<(DD_WIDTH>>2);x++)
    	    			{
    	    				if (
                    
                    			HAMMap[
                    					(y<<2)*HITMAP_EXTENSION*(DD_WIDTH>>HITMAP_SHIFT)+((x<<2)>>HITMAP_SHIFT)
                    			      ]
    	           			) 
                            {
    	           		 		if ((HITMAP_COLORS[3] & 0xff000000)==0) PutPixelRGB(x+dx,y+dy, (HITMAP_COLORS[3] & 0xff0000)>>16, (HITMAP_COLORS[3] & 0xff00)>>8, (HITMAP_COLORS[3] & 0xff));
    	    				} 
                            else 
                            {
                            	if ((HITMAP_COLORS[2] & 0xff000000)==0) PutPixelRGB(x+dx,y+dy, (HITMAP_COLORS[2] & 0xff0000)>>16, (HITMAP_COLORS[2] & 0xff00)>>8, (HITMAP_COLORS[2] & 0xff));
                        	}
                        }   
                     }
                     break;
            case 1 : /* hires */
    				 /* detailed (slow) visualization */
    				 for (y=0;y<DD_HEIGHT*HITMAP_EXTENSION;y++)
    				 {
 					 	for (x=0;x<DD_WIDTH;x++)
    	    			{
    	    				if (
                    			HAMMap[
                    					y *(DD_WIDTH>>HITMAP_SHIFT)+(x>>HITMAP_SHIFT)
                    			      ]
    	           			) 
    	           			{			
                            	if ((HITMAP_COLORS[3] & 0xff000000)==0) PutPixelRGB((x>>2)+dx,((y>>2)>>yshift)+dy, (HITMAP_COLORS[3] & 0xff0000)>>16, (HITMAP_COLORS[3] & 0xff00)>>8, (HITMAP_COLORS[3] & 0xff));
    	    				} 
                            else 
                            {
                            	if ((HITMAP_COLORS[2] & 0xff000000)==0) if (GetPixelRGB24((x>>2)+dx,((y>>2)>>yshift)+dy) != 0xff0000) PutPixelRGB((x>>2)+dx,((y>>2)>>yshift)+dy, (HITMAP_COLORS[2] & 0xff0000)>>16, (HITMAP_COLORS[2] & 0xff00)>>8, (HITMAP_COLORS[2] & 0xff));
                         	}
                         }   
    				 }	
    				 break;
        } 
    }

    if (BUDDHA_EDGES)
    {
    	/* edges */	
        for (i=0; i<edge_i;i++)
    	{
    	    p = edgequeue[i];	
    	    y = p / DD_WIDTH;
			x = p % DD_WIDTH;	
        
    	    if (
                GetPixelRGB24((x>>2)+dx,(y>>2)+dy) != HITMAP_COLORS[3] /* != MISS */  
               )	
    	       {   
               		if ((HITMAP_COLORS[0] & 0xff000000) == 0) PutPixelRGB((x>>2)+dx, (y>>2)+dy,(HITMAP_COLORS[0] & 0xff0000)>>16, (HITMAP_COLORS[0] & 0xff00)>>8, (HITMAP_COLORS[0] & 0xff));
    		   }	
        }
    }
    
    if (BUDDHA_LEMNISCATES)
    {
    	/* lemniscates */
    	for (i=0; i<lemni_i;i++)
    	{
    		p = lemniscates[i];	
    	    y = p / DD_WIDTH;
			x = p % DD_WIDTH;	
        
    	    if (
                GetPixelRGB24((x>>2)+dx,(y>>2)+dy) != HITMAP_COLORS[3] /* = MISS */ 
    	       )
    	       {   
               		if ((HITMAP_COLORS[1] & 0xff000000) == 0) PutPixelRGB((x>>2)+dx, (y>>2)+dy,(HITMAP_COLORS[1] & 0xff0000)>>16, (HITMAP_COLORS[1] & 0xff00)>>8, (HITMAP_COLORS[1] & 0xff));
    		   }	
        }
    }
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
 ULONG maxred, maxgreen, maxblue;
 ULONG deltaiter;
 struct IntuiText BTxt;
 UBYTE MaxTxt[200];
 double mdx, mdy;
 ULONG redrawcounter;
 ULONG temp_algorithm;
 UBYTE showpoints=TRUE;
 ULONG tempiter;
 ULONG histoffsetgreen, histoffsetblue, maxhist;
 ULONG deltaseconds, lastdeltaseconds, refreshseconds, newseconds;
 double rfx, rfy; 
 double IMIN1,IMAX1,RMIN1, RMAX1;
 double dx, dy, d;
 ULONG period;
 UBYTE nc[9]; /* nc = nine colors */
 UBYTE ncc; /* nine colors count */
 BOOL antibuddha_hit;
 ULONG hmx, hmy, hmy2;
 double fr, fg, fb;
 ULONG hmbx, hmby, hmby2; /* HamMapBitX/Y for 32bit HAMMap */
 ULONG accumulated_delta_for_autosave=0;
 ULONG autosave_count=0;
 BOOL zoom_reuse;

 PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,FM_BUSY_MOUSE_POINTER);
 OffTitle(MYILBM.scr);

 zoom_reuse = FMG_BUDDHA_ZOOM_IN && (DD_WIDTH==FMG_LAST_QUEUE_WIDTH) && (DD_HEIGHT==FMG_LAST_QUEUE_HEIGHT);

 if (((!BUDDHA_RESUME) || (!lemni_i) || (!queue_i)) && ((BUDDHA_EDGES) || (BUDDHA_LEMNISCATES)))
 {
    if (!Queue)
    {
        if (!AllocateBoundary()) return 0;
 	}
 }

 /* just to be sure that we have enough memory for buddha */
 AllocBuddha(); /* this function does all by itself (checks and resizing of buffers etc.) */
 
 rfx=BUDDHA_RANDOMIZE_X;
 rfy=BUDDHA_RANDOMIZE_Y;

 SHOW_MAXCOUNTERS=FALSE;
 BUDDHA_RANDOM_HAMMAP=TRUE;
 
 Rp = Win->RPort;
 
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
 
 algorithm_buddha=BUDDHA_EDGES;
 resx=a2-a1+1;
 resy=b2-b1+1;
 
 /* for leminscates boundary fractal */
 INCREMREAL = ((double)fabs (4.0)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = ((double)fabs (3.0)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 /* draw boundary trace first to obtain border points */
 if ((((!BUDDHA_RESUME) && (!zoom_reuse)) || (!lemni_i) || (!queue_i)) && ((BUDDHA_EDGES) || (BUDDHA_LEMNISCATES)))
 {
 	tempiter=MAX_ITERATIONS;
	MAX_ITERATIONS=MAX_ITERATIONS; /* set boundary trace fix to 256 iterations */

    /* save old coordinates */
    RMIN1=RMIN;
    RMAX1=RMAX;
    IMIN1=IMIN;
    IMAX1=IMAX;
    
    IMIN=-1.5;
    IMAX=1.5;
    RMIN=-2.0;
    RMAX=2.0;
    
    DrawFractalBoundaryGeneric(Win,a1,b1,a2,b2);
    
    /* restore old coordinates */
    RMIN=RMIN1;
    RMAX=RMAX1;
    IMIN=IMIN1;
    IMAX=IMAX1;
    
    INCREMREAL = ((double)fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = ((double)fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;
 
    MAX_ITERATIONS=tempiter;
 }
 
 /* mandelbrot deltas */
 mdx = (fabs (4.0)) / ((double) (a2 - a1 + 1));
 mdy = (fabs (3.0)) / ((double) (b2 - b1 + 1));
  
/* show queue end build edge queue */
/*
printf("Queue: %p Done: %p\n", Queue, Done);
printf("edgequeue: %p lemniscates: %p\n", edgequeue, lemniscates);
printf("DD_WIDTH: %u DD_HEIGHT: %u\n", DD_WIDTH, DD_HEIGHT);
printf("BUDDHA_EGDES: %u BUDDHA_LEMNISCATES: %u\n", BUDDHA_EDGES, BUDDHA_LEMNISCATES);
*/

if ((((!BUDDHA_RESUME) && (!zoom_reuse)) || (!lemni_i) || (!queue_i)) && ((BUDDHA_EDGES) || (BUDDHA_LEMNISCATES)))
{
 queue_i=0; lemni_i=0;
 for (i=0; i<DD_HEIGHT*DD_WIDTH; i++)
 {
 	if ((p = Queue[i])!=0)
	{
        y = p / DD_WIDTH;
		x = p % DD_WIDTH;
		
        dx=x*mdx-2.0; 
        dy=y*mdy-1.5; 
        
        d=dx*dx+dy*dy;
        
        if (Done[p] & Loaded)
        {
        	/* check if pixel is surrounded by other colors (= lemniscate) */
            
            nc[0] = GetPixelPen(x-1,y-1);
            nc[1] = GetPixelPen(x,y-1);
            nc[2] = GetPixelPen(x+1,y-1);
            
            nc[3] = GetPixelPen(x-1,y);
            nc[4] = GetPixelPen(x,y);
            nc[5] = GetPixelPen(x+1,y);
                
        	nc[6] = GetPixelPen(x-1,y+1);
            nc[7] = GetPixelPen(x,y+1);
            nc[8] = GetPixelPen(x+1,y+1);
            
            ncc=0;
            for (j=0;j<9;j++)
            {
            	if (nc[j]==nc[4]) ncc++;
            }
            
            if ((ncc!=9) || (nc[4]==0))
            {
            
            	if (!((x==0) || (y==0) || (x==resx-1) || (y==resy-1)))		
        		{
                
            		if (GetPixelPen(x,y)==0)
            		{
						/* exclude "ring" */
						if ( 
							(
					        	(dx*dx)
					   		    +
					        	(dy*dy)
					        )
					        < 4.0
						   )
						   {         
		    		            if (y!=resy>>1)
		    		            {		
		    		                if (x!=resx>>1)
		    		                {	
		    		                    /* PutPixelRGB(x,y,0x00, 0xff, 0x00); */
		    		            		if (edgequeue) edgequeue[queue_i++]=p;    
		    		        		}	
		    		            }
		    		            /* not sure about this else part ... */
		    		            else
		    		            {
		    		            	if ((nc[1]!=nc[4]) && (nc[7]!=nc[4]))
		    		            	{
		    		            		/* PutPixelRGB(x,y,0x00, 0xff, 0x00); */
		    		            		if (edgequeue) edgequeue[queue_i++]=p;    
		    		                }
		    		            }
						    }  
						    else 
						    {
						        /* add it to lemniscates */
						        if (BUDDHA_LEMNISCATES && lemniscates) lemniscates[lemni_i++]=p;
						    }          
	            
	        		}
        			else
            		{
            		    if (lemniscates) lemniscates[lemni_i++]=p;   
                	}
        		}
    		}

 		} 		

	}
	FMG_LAST_QUEUE_WIDTH=DD_WIDTH;
	FMG_LAST_QUEUE_HEIGHT=DD_HEIGHT;
 }
 
 SetRast(Win->RPort,0);
 
 /* show edge queue */ 
 if (BUDDHA_EDGES)
 {
 	for (i=0; i<queue_i; i++)
 	{
 	   p = edgequeue[i];
 	   y = p / DD_WIDTH;
	   x = p % DD_WIDTH;
       PutPixelRGB(x,y,0xff, 0x00, 0x00);
	}
 }

 /* show lemniscates */
 if (BUDDHA_LEMNISCATES)
 {
 	for (i=0; i<lemni_i; i++)
 	{
 	   	p = lemniscates[i];
   		y = p / DD_WIDTH;
		x = p % DD_WIDTH;
   		PutPixelRGB(x,y,0xff, 0x00, 0xff);
	}
 }
}

 SetRast(Win->RPort,0);
  
/* show queues */
/* use ShowHAMMap for that now */
/* if (!BUDDHA_RESUME)
 {
    	if (BUDDHA_EDGES) 
        {
        	for (i=0; i<queue_i; i++) 
            {
          		p=edgequeue[i];
                y = p / DD_WIDTH;
				x = p % DD_WIDTH;
    			PutPixelRGB(x,y,0xff, 0x00, 0x00);
            }
        }
    	if (BUDDHA_LEMNISCATES) 
        {
        	for (i=0; i<lemni_i; i++) 
            {
          		p=lemniscates[i];	
                y = p / DD_WIDTH;
				x = p % DD_WIDTH;
    			PutPixelRGB(x,y,0x00, 0xff, 0x00);
            }
        }
 }
*/ 

 CurrentTime (&StartSec,&StartTicks);

 redrawcounter= 10000;
 
 resx=a2-a1+1;
 resy=b2-b1+1;

 fx=(resx)/(IMAX-IMIN);
 fy=(resy)/(RMAX-RMIN);

 /*printf("RE: %2.17lf - %2.17lf IM: %2.17lf - %2.17lf => fx: %f fy: %f (resx: %u resy: %u)\n", 
 	RMIN, RMAX, IMIN, IMAX, fx, fy, resx, resy);
 */
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

 /* reset counters or leave them as is (in case of resume) */
 if (!BUDDHA_RESUME)
 {
 	for (offset=0; offset<maxhist; offset++)
 	{
		BuddhaCounter[offset]=0;
	}
	if (USE_HAMMAP) ClearHAMMap();
    ACCUMULATED_BUDDHA_TIME=0;
 
 } /* else PrintHistData(BuddhaCounter); */
 
 refreshseconds=1;
 lastdeltaseconds=deltaseconds=StartSec;

 while (!stop)
 {
    loopcounter++;
	
    /* disable visual debugging for used pixels / values */
	showpoints=0;
	temp_algorithm=BUDDHA_RANDOM;
    
	/* keep original intervals for random points */
	/* search for a good point */	
    GetSampleNumber(mdx, mdy, edgequeue, queue_i, lemniscates, lemni_i);
    
    zx = NEBULA_SAMPLE_RE;
    zy = NEBULA_SAMPLE_IM;
	
    hmx = NEBULA_SAMPLE_X; /* hammap x */
	hmy = NEBULA_SAMPLE_Y; 	
	
    hmbx = (ULONG)((NEBULA_SAMPLE_RE+2.0) * (double)HITMAP_EXTENSION / mdx);
    hmby = (ULONG)((NEBULA_SAMPLE_IM+1.5) * (double)HITMAP_EXTENSION / mdy);
    
    /* symmetry for hitmap */
    hmy2  = DD_HEIGHT-hmy-1; 
    hmby2 = DD_HEIGHT*HITMAP_EXTENSION - hmby - 1; 
     
	i = j = MAX_ITERATIONS + 1;	
    
	/* calculate z0 ... zn */
    i=FractalInCGenericStoreIterations(i,zx,zy);
    
    if (ANTIBUDDHA)
    {
        /* all antinebula (Antibuddhabrot) functions */  
        if (i==0)
        {		
			/* only use points that do not escape */
	    	
            if (USE_PERIODICITY)
	        {
                /* prime number coloring */
                period=GetCyclePeriodASM(re,im,MAX_ITERATIONS,PERIODICITY_EPSILON);
	            /*printf("period: %u\n", period);*/
                
            	antibuddha_hit=FALSE;
            
            	if (period%PRIME_RED==0)
                {
                	/* red */
                    antibuddha_hit=TRUE;
                	for (tempiter=0; tempiter<period/PRIME_DIVIDER; tempiter++)      
            			StoreIterations(0,i,j,fx,fy, resx, resy);  
                }
            	
                if (period%PRIME_GREEN==0)
                {
                	/* green */
                    antibuddha_hit=TRUE;
                    for (tempiter=0; tempiter<period/PRIME_DIVIDER; tempiter++)      
            			StoreIterations(1,i,j,fx,fy, resx, resy);  
                }
                
                if (period%PRIME_BLUE==0)
                {
                	/* blue */
                    antibuddha_hit=TRUE;
                    for (tempiter=0; tempiter<period/PRIME_DIVIDER; tempiter++)      
            			StoreIterations(2,i,j,fx,fy, resx, resy);  
                }
            	
            	if ((!antibuddha_hit) && (period!=1)) /* asm routine leads to period 1 ... */
                {
                    /* if no hit => increment all rgb-values */    
                	StoreIterations(0,i,j,fx,fy, resx, resy);
        			StoreIterations(1,i,j,fx,fy, resx, resy);
        			StoreIterations(2,i,j,fx,fy, resx, resy);
                }
            }
    		else
            {
      			/* no periodicity => false color image drawing partial orbits */		
            	StoreIterations(0,RGB_ITER[FM_RED_MIN],RGB_ITER[FM_RED_MAX],fx,fy, resx, resy);  	
            	StoreIterations(1,RGB_ITER[FM_GREEN_MIN],RGB_ITER[FM_GREEN_MAX],fx,fy, resx, resy);  
            	StoreIterations(2,RGB_ITER[FM_BLUE_MIN],RGB_ITER[FM_BLUE_MAX],fx,fy, resx, resy);  		
            }
        }
        else
        {
        	if (USE_HAMMAP)
            {	
            	/* Traditionally, fractals are "upside down" in FM.
                   This is no problem for classic Mandelbrots, Multibrots, Julia sets etc.
                   But it is a problem for the Burning Ship fractal (which is not
                   symmetric relative to x- / real-axis.
                   The consequence is that the SampleMap of the Burning Ship fractal is 
                   not correct (flipped around the x-axis.
                   To correct that, simply invert hmby and hmby2 for the SampleMap entry
                 */	
                
                /* if x>0 => mark it in HAMMap (so that it won't be reused) */
                if ((hmx<DD_WIDTH) && (hmx>=0) && (hmy>=0) && (hmy<DD_HEIGHT))
                {	
                    /*printf("Marking HAMMap[%u] (hmby: %u hmbx: %u hmy: %u hmx: %u) (MAX: hmx: %lu zx: %f)\n",
                    	hmby*DD_WIDTH+hmx, hmby, hmbx, hmy, hmx, maxhmx, maxzx);
                    */
                    HAMMap[
                    		/*hmby*/ hmby2*(DD_WIDTH>>HITMAP_SHIFT)+(hmx>>HITMAP_SHIFT)
                          ]
                          |=
                          1<<(hmbx%32);
                
                
                	/* symmetry for hitmap */
                    if (USE_SYMMETRY) 
                    {
                    	  HAMMap[
                    		/*hmby2*/ hmby*(DD_WIDTH>>HITMAP_SHIFT)+(hmx>>HITMAP_SHIFT)
                          ]
                          |=
                          1<<(hmbx%32);
                	}
                }
            } 
        }
    	
    }    
	else
    {
		/* nebula (Buddhabrot) */
        deltaiter=MAX_ITERATIONS-i;
        
      	if (i!=0) 
      	{
      	  /* red */
      	  	if ((deltaiter<RGB_ITER[FM_RED_MAX]) && (deltaiter>RGB_ITER[FM_RED_MIN]))
      	  	{
      	      StoreIterations(0,i,j,fx,fy, resx, resy);
        	}
        	/* green */
        	if ((deltaiter<RGB_ITER[FM_GREEN_MAX]) && (deltaiter>RGB_ITER[FM_GREEN_MIN]))
        	{
        	    StoreIterations(1,i,j,fx,fy, resx, resy);
        	}
        	/* blue */
        	if ((deltaiter<RGB_ITER[FM_BLUE_MAX]) && (deltaiter>RGB_ITER[FM_BLUE_MIN]))
        	{
        	    StoreIterations(2,i,j,fx,fy, resx, resy);
        	}
      	} 
      	else
      	{
      		if (USE_HAMMAP)
       	 	{
            	/* Traditionally, fractals are "upside down" in FM.
                   This is no problem for classic Mandelbrots, Multibrots, Julia sets etc.
                   But it is a problem for the Burning Ship fractal (which is not
                   symmetric relative to x- / real-axis.
                   The consequence is that the SampleMap of the Burning Ship fractal is 
                   not correct (flipped around the x-axis.
                   To correct that, simply invert hmby and hmby2 for the SampleMap entry
                 */	
        	
        	    /* if x==0 => mark it in HAMMap (so that it won't be reused) */
        	    if ((hmx<DD_WIDTH) && (hmx>=0) && (hmy>=0) && (hmy<DD_HEIGHT))
                {	
                    HAMMap[
                    		hmby2*(DD_WIDTH>>HITMAP_SHIFT)+(hmx>>HITMAP_SHIFT)
                          ]
                          |=
                          1<<(hmbx%32);
                
                
                	/* symmetry for hitmap */
                    if (USE_SYMMETRY) 
                    {
                    	  HAMMap[
                    		hmby*(DD_WIDTH>>HITMAP_SHIFT)+(hmx>>HITMAP_SHIFT)
                          ]
                          |=
                          1<<(hmbx%32);
                	}
                }
        	}
      	} 
    }
    
    /* use periodicity iterations for antibuddha */
	if (loopcounter%50==0)
	{
		/* give user possibility to interrupt calculation every 50ieth loop */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) stop=1;
		
		if (!showpoints)
		{				
			CurrentTime (&newseconds,&StartTicks);
			deltaseconds=newseconds-lastdeltaseconds;

            if ((!stop) && (deltaseconds>=refreshseconds))
			{
				/* deltaseconds "stabilizes" at 1, 2, 8, 16, 32, 64 seconds (approx. 1 minute) */			
				accumulated_delta_for_autosave+=deltaseconds;
                
                lastdeltaseconds=newseconds;
				if (refreshseconds<=60) refreshseconds*=2;

				/* find new maxcounters */
				FindMaxCounters(BuddhaCounter,resx,resy,&maxred,&maxgreen,&maxblue);			

                fr=255.0/sqrt((double)maxred); 
                fg=255.0/sqrt((double)maxgreen); 
                fb=255.0/sqrt((double)maxblue); 

				for (y=b1; y<=b2; y++) 
				{
					offset=y*resx;
					for (x=a1; x<=a2; x++) 
					{
                    	/* FindMaxCounters() returns always max >= 1 */
						buddhared   = fr * sqrt((double)BuddhaCounter[offset+x]); 
						buddhagreen = fg * sqrt((double)BuddhaCounter[histoffsetgreen+offset+x]); 
						buddhablue  = fb * sqrt((double)BuddhaCounter[histoffsetblue+offset+x]); 
						
						PutPixelRGB(x,y, buddhared, buddhagreen, buddhablue);
					}
				}

                if (accumulated_delta_for_autosave>=HISTOGRAM_AUTOSAVE_TIME)
                {
                	/* autosave */	
                    if (autosave_count>0)
                    {
                    	/* first backup old autosave */
                    	Execute("copy AUTOSAVE.IFF AUTOSAVE1.IFF", NULL, NULL);
						Execute("copy AUTOSAVE.IFF.HST AUTOSAVE1.IFF.HST", NULL, NULL);
                    }
                    
                    /* now write new backup */

                    if ((USE_HAMMAP  || BUDDHA_EDGES || BUDDHA_LEMNISCATES) && HITMAP_AT_THE_END) ShowHAMMap(edgequeue, queue_i, lemniscates, lemni_i);
                    
                    strcpy(MYPATH,"AUTOSAVE.IFF");
                    
                    PrepareAndSaveAllNewSpecialChunkPic(&MYILBM);
                    
                    if ((USE_HAMMAP  || BUDDHA_EDGES || BUDDHA_LEMNISCATES) && HITMAP_DURING_CALCULATION && (!HITMAP_AT_THE_END)) ShowHAMMap(edgequeue, queue_i, lemniscates, lemni_i);	
                	
                    accumulated_delta_for_autosave=0;
                    autosave_count++;
                }
                else if ((USE_HAMMAP || BUDDHA_EDGES || BUDDHA_LEMNISCATES) && HITMAP_DURING_CALCULATION) ShowHAMMap(edgequeue, queue_i, lemniscates, lemni_i);
                
				if (!stop)
				{
					sprintf(MaxTxt,"%u -> %d %d %d",loopcounter,maxred,maxgreen,maxblue);				
					if (SHOW_MAXCOUNTERS) PrintIText(Win->RPort,&BTxt,0,0);	
				} else return TRUE;
			}
		}

        if ((BUDDHA_MAX_LOOPS) && (loopcounter >= BUDDHA_MAX_LOOPS)) stop=TRUE;
		CurrentTime (&EndSec,&EndTicks);
        if ((FM_MAX_CALC_SECS) && ((EndSec-StartSec)>FM_MAX_CALC_SECS)) stop=TRUE;
    }
 }

 CurrentTime (&EndSec,&EndTicks);

 /* draw fractal in the end */
 /* find new maxcounters */
 FindMaxCounters(BuddhaCounter,resx,resy,&maxred,&maxgreen,&maxblue);

 fr=255.0/sqrt((double)maxred); 
 fg=255.0/sqrt((double)maxgreen);
 fb=255.0/sqrt((double)maxblue); 
                                		
 for (y=b1; y<=b2; y++) 
 {
	offset=y*resx;
	for (x=a1; x<=a2; x++) 
	{
		buddhared   = fr * sqrt((double)BuddhaCounter[offset+x]); 
		buddhagreen = fg * sqrt((double)BuddhaCounter[histoffsetgreen+offset+x]); 
		buddhablue  = fb * sqrt((double)BuddhaCounter[histoffsetblue+offset+x]); 
						
		PutPixelRGB(x,y, buddhared, buddhagreen, buddhablue);
	}
 }
 
 if ((USE_HAMMAP || BUDDHA_EDGES || BUDDHA_LEMNISCATES) && HITMAP_AT_THE_END) ShowHAMMap(edgequeue, queue_i, lemniscates, lemni_i);
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);
 	
 BUDDHA_RESUME=FALSE;
 MenuResumeOff(Win);
 
 /* do allocations / freeing via AllocBuddha() / FreeBuddha() */
 
 /* Boundary algorithm doesn't free memory when calculating for Nebulabrots */
 /* => in this case the memory must be freed here */
 if ((BUDDHA_EDGES) || (BUDDHA_LEMNISCATES)) DeallocateBoundary(); 
 
 FMG_BUDDHA_ZOOM_IN=FALSE;
 
 OnTitle(MYILBM.scr);
 
 return (EndSec-StartSec);
}

/************************************************************************************
*************************************************************************************/


