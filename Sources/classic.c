
#include "shared.h"
#include "classic.h"
#include "enum.h"

/* FlashMandel functions for classic Amiga */

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , StartMicro, EndMicro;
double cx,cy;
WORD x, y;

/*double log_zn, nu, mu;
ULONG color1, color2;
double iteration;*/
ULONG yoffset; 
/*UBYTE red1,green1,blue1;
UBYTE red2,green2,blue2;
UBYTE red, green, blue;
double fraction;*/
 
 CurrentTime (&StartSec,&StartMicro);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		yoffset=y*Win->Width;
		for (x=a1; x<=a2; x++) {
		 
			cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			cx = x*INCREMREAL+RMIN;	
			
			Color=Mandel68k_FPU_Classic(ITERATIONS-1,cx,cy);
			
			PutPixelWithColorMode(x,y,Color);
			
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
		}
	}

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&EndMicro);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);

}

/* (1b) Julia */
ULONG DrawJuliaFractalBruteClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG Color;
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
WORD x, y;
double tx, ty;

/*ULONG color1, color2;
double iteration, fraction, mu, nu;
*/
 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 GlobalP = 2.0;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {

		for (x=a1; x<=a2; x++) {
		
			tx= x*INCREMREAL+RMIN;	

			ty= IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			
			Color = Julia68k_FPU_Classic (MAX_ITERATIONS,tx,ty,JKRE,JKIM); 
			
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

/* (2) Tiling */

/* (2a) Mandelbrot */
/* (2aa) Horizontal */

void MHLine_Classic (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
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
     Color = Mandel68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }

 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

/* (2ab) Vertical */

void MVLine_Classic (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 for (y = b2; y >= b1; y--)
 {
     Color = Mandel68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     

     SetAPen (Rp,Color);

     WritePixel (Rp,x,y);

     CIM += INCREMIMAG;
 }
}

/* (2b) Julia */ 
/* (3ba) Horizontal */

void JHLine_Classic (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x = width;

UBYTE *TmpArray = PixelLine;

ULONG Color;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     
     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }
 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

/* (3bb) Vertical */

void JVLine_Classic (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);

 for (y = b2; y >= b1; y--)
 {
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     SetAPen (Rp,Color);

     WritePixel (Rp,x,y);

     CIM += INCREMIMAG;
 }
}


/* (3) Boundary trace */

/* (3a) Mandelbrot */

int LoadMandelClassic(unsigned p) {
    UWORD x,y;
	ULONG Color;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color = Mandel68k_FPU_Classic(ITERATIONS-1,cx,cy);
	
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

void ScanMandelClassic(unsigned p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadMandelClassic(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadMandelClassic(p-1) != center);
    r=rr && (LoadMandelClassic(p+1) != center);
    u=uu && (LoadMandelClassic(p-resx) != center);
    d=dd && (LoadMandelClassic(p+resx) != center);
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

ULONG DrawMandelFractalBoundaryClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
WORD x, y;

 CurrentTime (&StartSec,&Dummy);

 screenbuffer=Win->RPort->BitMap->Planes[0];
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
        ScanMandelClassic(p);
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

void AddQueueJuliaClassic(unsigned p) {
    if(Done[p] & Queued) return;
    Done[p] |= Queued;
    Queue[QueueHead++] = p;
    if(QueueHead == QueueSize) QueueHead = 0;
}

int LoadJuliaClassic(unsigned p) {
    UWORD x,y;
	ULONG Color;
	double cx, cy;

    if(Done[p] & Loaded) return Data[p];
    x = p % resx;
    y = p / resx;
	
	cy = IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
	cx = x*INCREMREAL+RMIN;	
    
	Color=Julia68k_FPU_Classic(ITERATIONS-1,cx,cy,JKRE,JKIM);
	
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

void ScanJuliaClassic(unsigned p) {
    UWORD x = p % resx, y = p / resx;
    UWORD center = LoadJuliaClassic(p);
    UWORD ll = x >= 1, rr = x < resx-1;
    UWORD uu = y >= 1, dd = y < resy-1;
    UWORD l, r, u, d; 
	/* booleans */
   	l=ll && (LoadJuliaClassic(p-1) != center);
    r=rr && (LoadJuliaClassic(p+1) != center);
    u=uu && (LoadJuliaClassic(p-resx) != center);
    d=dd && (LoadJuliaClassic(p+resx) != center);
    /* process the queue (which is actually a ring buffer) */
    if (l) AddQueueJuliaClassic(p-1);
    if (r) AddQueueJuliaClassic(p+1);
    if (u) AddQueueJuliaClassic(p-resx);
    if (d) AddQueueJuliaClassic(p+resx);
    /* the corner pixels (nw,ne,sw,se) are also neighbors */
    if((uu&&ll)&&(l||u)) AddQueueJuliaClassic(p-resx-1);
    if((uu&&rr)&&(r||u)) AddQueueJuliaClassic(p-resx+1);
    if((dd&&ll)&&(l||d)) AddQueueJuliaClassic(p+resx-1);
    if((dd&&rr)&&(r||d)) AddQueueJuliaClassic(p+resx+1);
}

ULONG DrawJuliaFractalBoundaryClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
WORD x, y;

 screenbuffer=Win->RPort->BitMap->Planes[0];
 Data=screenbuffer;
 
 CurrentTime (&StartSec,&Dummy);

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
        AddQueueJuliaClassic(y*resx);
        AddQueueJuliaClassic(y*resx + (resx-1));
    }
    for(x=1; x<resx-1; ++x) {
        AddQueueJuliaClassic(x);
        AddQueueJuliaClassic((resy-1)*resx + x);
    }
	/* for julia: add also middle lines */
	for(y=1; y<resy-1; ++y) {
        AddQueueJuliaClassic(y*resx + resx/2);
    }
    for(x=1; x<resx-1; ++x) {
        AddQueueJuliaClassic((resy/2)*resx + x);
    }
	
	/* (2) process the queue (which is actually a ring buffer) */
    flag=0;
    while(QueueTail != QueueHead) {
        if(QueueHead <= QueueTail || ++flag & 3) {
            p = Queue[QueueTail++];
            if(QueueTail == QueueSize) QueueTail=0;
        } else p = Queue[--QueueHead];
        ScanJuliaClassic(p);
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

