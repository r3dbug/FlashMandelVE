
/* FlashMandel functions for Vampire */

#include "fmglobals.h"
#include "vampire.h"
#include "enum.h"

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
 ULONG Color;
 double cx,cy;
 WORD x, y;
 double px1, px2, px3, px4;
 struct RastPort* Rp;
 WORD oldpen;
 
 Rp = Win->RPort;
 
 SetAPen(Rp, oldpen = 0);
 
 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {
		for (x=a1; x<=a2; x+=4) {
		 
			cy = IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
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
 
 return 0;
}

/* (1b) Julia */
ULONG DrawJuliaFractalBruteVampire (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
 ULONG Color;
 WORD x, y;
 double tx, ty;
 double px1,px2,px3,px4;
 BYTE oldpen;
 struct RastPort* Rp;
 Rp = Win->RPort;
 SetAPen(Rp, oldpen=0);
 
 GlobalP = 2.0;
 
 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

	for (y=b1; y<=b2; y++) {

		for (x=a1; x<=a2; x+=4) {
		
			tx= x*INCREMREAL+RMIN;	

			ty= IMAX-y*INCREMIMAG;  /* FlashMandel is upside down (..) */
			
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

 return 0; 
}

/* (2) Tiling */
/* (2a) Mandelbrot */
/* (2aa) Horizontal */

void MHLine_Vampire (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
 LONG x;

 UBYTE *TmpArray;

 ULONG Color;
 
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

 WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
}


void MVLine_Vampire (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
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
 LONG x = width;

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

 /* do the rest */
 while (x--)
 {
     Color = Julia68k_FPU_Vampire( MAX_ITERATIONS, CRE, CIM); 
  
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
     
     Color=IterP1; 
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-1,Color);
	 
     Color=IterP2; 
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-2,Color);
	 
     Color=IterP3; 
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     PutPixel(x,b1y-3,Color);
	 
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
 
 while (y--)
 {
     Color = Julia68k_FPU_Vampire( MAX_ITERATIONS, CRE, CIM); 
     
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
/* (all done with generic function) */

/* (3a) Mandelbrot */

/* (3b) Julia */

