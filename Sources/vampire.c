
/* FlashMandel functions for Vampire */

#include "fmglobals.h"
#include "vampire.h"
#include "enum.h"

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
double cx,cy;
WORD x, y;
double px1, px2, px3, px4;
struct RastPort* Rp;
WORD oldpen;
/*ULONG color1, color2;
double iteration, fraction, mu, nu;
*/
 SetUpDirectDrawing(Win);
 
 Rp = Win->RPort;
 
 SetAPen(Rp, oldpen = 0);
 
 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x+=4) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
            /* prepare 4 points */
            px1=cx; 
            px2=px1+INCREMREAL;
            px3=px2+INCREMREAL;
            px4=px3+INCREMREAL;

            par4iterateasm(px1,cy,px2,cy,px3,cy,px4,cy,ITERATIONS-1);
            /* p1 */
            Color=IterP1;
			PutPixelWithColorMode(x,y,Color);
			
			/* p2 */
            Color=IterP2;
        	FinalXP1 = FinalXP2;
			FinalYP1 = FinalYP2;
			PutPixelWithColorMode(x+1,y,Color);
			
			/* p3 */
            Color=IterP3;
            FinalXP1 = FinalXP3;
			FinalYP1 = FinalYP3;
			PutPixelWithColorMode(x+2,y,Color);
			
			/* p4 */
            Color=IterP4;
            FinalXP1 = FinalXP4;
			FinalYP1 = FinalYP4;
			PutPixelWithColorMode(x+3,y,Color);

		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

/* (1b) Julia */
ULONG DrawJuliaFractalBruteVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
WORD x, y;
double tx, ty;
double px1,px2,px3,px4;
BYTE oldpen;
struct RastPort* Rp;
/*ULONG color1, color2;
double iteration, fraction, mu, nu;
*/
 Rp = Win->RPort;
 SetAPen(Rp, oldpen=0);

 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;
 
 /* ShowData("BRUTE (JULIA)", a1,b1,a2,b2); */

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {

		for (x=a1; x<=a2; x+=4) {
		
			tx= x*INCREMREAL+RMIN;	

			ty= IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			
			/* prepare 4 points */
            px1=tx; 
            px2=px1+INCREMREAL;
            px3=px2+INCREMREAL;
            px4=px3+INCREMREAL;

            juliapar4iterateasmhoriz(px1,ty,px2,px3,px4,JKRE,JKIM,MAX_ITERATIONS);
            /* p1 */
            Color=IterP1;
			PutPixelWithColorMode(x,y,Color);
			
            /* p2 */
            Color=IterP2;
            FinalXP1 = FinalXP2;
			FinalYP1 = FinalYP2;
			PutPixelWithColorMode(x+1,y,Color);
			
            /* p3 */
            Color=IterP3;
            FinalXP1 = FinalXP3;
			FinalYP1 = FinalYP3;
			PutPixelWithColorMode(x+2,y,Color);
			
            /* p4 */
            Color=IterP4;
            FinalXP1 = FinalXP4;
			FinalYP1 = FinalYP4;
			PutPixelWithColorMode(x+3,y,Color);
			
		}
		
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
	}
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

/* (2) Tiling */
/* (2a) Mandelbrot */
/* (2aa) Horizontal */

void MHLine_Vampire_works (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
/*REGISTER*/ LONG x;

/*REGISTER*/ UBYTE *TmpArray;

/*REGISTER*/ ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     
     Color = Mandel68k_FPU_Vampire/*MandelInC*/ (MAX_ITERATIONS+1,CRE,CIM);
	 /*par4iterateasm(CRE,CIM,CRE,CIM,CRE,CIM,CRE,CIM,MAX_ITERATIONS);
     Color=IterP1;*/
     	 
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

void MVLine_Vampire_works (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 for (y = b2; y >= b1; y--)
 {
     /*Color = /*MandelInC*/ /*Mandel68k_FPU_Vampire (MAX_ITERATIONS+1,CRE,CIM);*/
     par4iterateasm(CRE,CIM,CRE,CIM,CRE,CIM,CRE,CIM,MAX_ITERATIONS);
    Color=IterP1;
   
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

void MHLine_Vampire (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
/*REGISTER*/ LONG x;

/*REGISTER*/ UBYTE *TmpArray;

/*REGISTER*/ ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x>=4)
 {
     /* prepare 4 points for parallel calculation */
     px1=CRE; 
     px2=px1+INCREMREAL;
     px3=px2+INCREMREAL;
     px4=px3+INCREMREAL;
     
     par4iterateasm(px1,CIM,px2,CIM,px3,CIM,px4,CIM,MAX_ITERATIONS);
            
     /* write 4 results to TmpArray (whatever that is ... :) */
     /* p1 */
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p2 */
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p3 */
     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p4 */
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
        
     CRE = px4+INCREMREAL;
        
     x-=4;
 }

 
  while (x--)
  {
     Color = Mandel68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
  }

 WritePixelLine /*8*/ (Rp, a1, y, width, PixelLine, &TempRP);
}


void MVLine_Vampire/*_buggy*/ (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
 REGISTER LONG y;

 ULONG Color;
 ULONG b1y;
 ULONG oldpen;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 y=b2-b1+1;
 oldpen=0;
 SetAPen(Rp,oldpen);
 
 while (y>=4)
 {
     /* prepare 4 points */
     py1=CIM; 
     py2=py1+INCREMIMAG;
     py3=py2+INCREMIMAG;
     py4=py3+INCREMIMAG;

     b1y=b1+y;     
     par4iterateasm(CRE,py1,CRE,py2,CRE,py3,CRE,py4,MAX_ITERATIONS);
     /* p1 */
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-1,Color);
	 
     /* p2 */
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-2,Color);
	 
     /* p3 */
     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-3,Color);
	 
     /* p4 */
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-4,Color);
	 
     CIM = py4+INCREMIMAG;
     y-=4;

 }
 
 /* do the rest */
 while (y--)
 {
     Color = Mandel68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
	 PutPixel(x,b1+y,Color);
	 
     CIM += INCREMIMAG;
 }

}

/* (2b) Julia */ 
/* (2ba) Horizontal */

void JHLine_Vampire (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
/*REGISTER*/ LONG x = width;

UBYTE *TmpArray = PixelLine;

ULONG Color;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x>=4)
 {
     /* prepare 4 points for parallel calculation */
     px1=CRE; 
     px2=px1+INCREMREAL;
     px3=px2+INCREMREAL;
     px4=px3+INCREMREAL;
     
     juliapar4iterateasmhoriz(px1,CIM,px2,px3,px4,JKRE,JKIM,MAX_ITERATIONS);
     
     /* write 4 results to TmpArray (whatever that is ... :) */
     /* p1 */
     Color=IterP1; /*Julia68k_FPU_Classic( MAX_ITERATIONS, px1, CIM, JKRE, JKIM); //IterP1;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p2 */
     Color=IterP2; /*Julia68k_FPU_Classic( MAX_ITERATIONS, px2, CIM, JKRE, JKIM); //IterP2;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p3 */
     Color=IterP3; /*Julia68k_FPU_Classic( MAX_ITERATIONS, px3, CIM, JKRE, JKIM); //IterP3;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p4 */
     Color=IterP4; /*Julia68k_FPU_Classic( MAX_ITERATIONS, px4, CIM, JKRE, JKIM); //IterP4;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
        
     CRE = px4+INCREMREAL;
        
     x-=4;
 }

 /* do the rest */
 while (x--)
 {
     Color = Julia68k_FPU_Vampire( MAX_ITERATIONS, CRE, CIM, JKRE, JKIM); /*Julia68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);*/
  
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

/* (2bb) Vertical */

void JVLine_Vampire (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color, oldpen;
ULONG b1y;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);

 y=b2-b1+1  ;
 oldpen=0;
 SetAPen(Rp,0);
 
 while (y>=4)
 {
     py1=CIM; 
     py2=py1+INCREMIMAG;
     py3=py2+INCREMIMAG;
     py4=py3+INCREMIMAG;

     b1y=y+b1;
     
     juliapar4iterateasmvert(CRE,py1,py2,py3,py4,JKRE,JKIM,MAX_ITERATIONS);
     
     Color=IterP1; /*Julia68k_FPU_Classic( MAX_ITERATIONS, CRE, py1, JKRE, JKIM); //IterP1;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-1,Color);
	 
     Color=IterP2; /*Julia68k_FPU_Classic( MAX_ITERATIONS, CRE, py2, JKRE, JKIM); //IterP2;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-2,Color);
	 
     Color=IterP3; /*Julia68k_FPU_Classic( MAX_ITERATIONS, CRE, py3, JKRE, JKIM); //IterP3;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-3,Color);
	 
	 Color=IterP4; /*Julia68k_FPU_Classic( MAX_ITERATIONS, CRE, py4, JKRE, JKIM); //IterP4;*/
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-4,Color);
	 
     CIM = py4+INCREMIMAG;
     y-=4;

 }
 
 while (y--)
 {
     Color = Julia68k_FPU_Vampire( MAX_ITERATIONS, CRE, CIM, JKRE, JKIM); /*Julia68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);*/
     
     b1y=y+b1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
	 PutPixel(x,b1y,Color);
     
     CIM += INCREMIMAG;
 }

}

/* (3) Boundary trace */

/* (3a) Mandelbrot */

UWORD LoadMandelVampire(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = Mandel68k_FPU_Vampire(ITERATIONS-1,cx,cy);
	
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

void ScanMandelVampire(ULONG p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadMandelVampire(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadMandelVampire(p-1) != center);
    r=rr && (LoadMandelVampire(p+1) != center);
    u=uu && (LoadMandelVampire(p-resx) != center);
    d=dd && (LoadMandelVampire(p+resx) != center);
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

ULONG DrawMandelFractalBoundaryVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
UWORD x, y;
/*WORD temp;*/

 /*printf("Mandel boundary Vampire: MYILBM.win.Width: %d .Height: %d", MYILBM.win->Width, MYILBM.win->Height); 
*/
 CurrentTime (&StartSec,&Dummy);

 /* screenbuffer=Win->RPort->BitMap->Planes[0];*/ /* 20240406 */
 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);

 Data=screenbuffer;
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
	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanMandelVampire(p);
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

/* (3b) Julia */

UWORD LoadJuliaVampire(ULONG p) {
    UWORD x,y;
	ULONG Color, i;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = i = Julia68k_FPU_Vampire(ITERATIONS-1,cx,cy,JKRE,JKIM);
	
	if (Color)
    {
     	Color *= CURRENT_MAX_COLORS;

        Color /= ITERATIONS;

   	   	Color += RESERVED_PENS;
    }
	PutPixel(x,y,Color);
			
    Done[p] |= Loaded;
    return Data[p] = Color;
}

void ScanJuliaVampire(ULONG p) {
    ULONG x = p % resx, y = p / resx;
    UWORD center = LoadJuliaVampire(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadJuliaVampire(p-1) != center);
    r=rr && (LoadJuliaVampire(p+1) != center);
    u=uu && (LoadJuliaVampire(p-resx) != center);
    d=dd && (LoadJuliaVampire(p+resx) != center);
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

ULONG DrawJuliaFractalBoundaryVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
WORD x, y;

 CurrentTime (&StartSec,&Dummy);

 /* screenbuffer=Win->RPort->BitMap->Planes[0]; */ /* 20240406 */
 screenbuffer=GetBitMapPtr(Win->RPort->BitMap);

 Data=screenbuffer;
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
        AddQueue(y*resx);
        AddQueue(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueue(x);
        AddQueue((resy-1)*resx + x);
    }
	/* for julia: add also middle lines */
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
        ScanJuliaVampire(p);
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


