
#include "shared.h"
#include "classic.h"
#include "enum.h"

/* FlashMandel functions for classic Amiga */

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
  ULONG Color;
  double cx,cy;
  WORD x, y;
  ULONG yoffset; 
  
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

  return 0;
}

/* (1b) Julia */

ULONG DrawJuliaFractalBruteClassic (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
  ULONG Color;
  WORD x, y;
  double tx, ty;

  GlobalP = 2.0;

  if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

  for (y=b1; y<=b2; y++) {

		for (x=a1; x<=a2; x++) {
		
			tx= x*INCREMREAL+RMIN;	

			ty= IMAX-y*INCREMIMAG;  /* hm ... FlashMandel is upside down ... */
			
			Color = Julia68k_FPU_Classic (MAX_ITERATIONS,tx,ty); 
			
			PutPixelWithColorMode(x,y,Color);
			
		}
		/* give user possibility to interrupt calculation each line */
		if (InterruptDrawing(Win,a1,b1,a2,b2)) return TRUE;
  }

  return 0;
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

  if (drawing==DRAW_CLASSIC) WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
  else WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
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
     
	 PutPixel(x,y,Color);
     
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
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     
     CRE += INCREMREAL;

     *TmpArray++ = Color;
  }
 
  if (drawing==DRAW_CLASSIC) WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
  else WritePixelLine (Rp, a1, y, width, PixelLine, &TempRP);
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
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);

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

