
#ifndef CLASSIC_H
#define CLASSIC_H

#include <exec/types.h>
#include "fmglobals.h"
#include "math_functions.h"

/* FlashMandel functions for classic Amiga */

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteClassic (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (1b) Julia */
ULONG DrawJuliaFractalBruteClassic (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (2) Tiling */

/* (2a) Mandelbrot */
/* (2aa) Horizontal */

void MHLine_Classic (struct RastPort *,const LONG,const LONG,const LONG);

/* (2ab) Vertical */

void MVLine_Classic (struct RastPort *,const LONG,const LONG,const LONG);

/* (2b) Julia */ 
/* (3ba) Horizontal */

void JHLine_Classic (struct RastPort *,const LONG,const LONG,const LONG);

/* (3bb) Vertical */

void JVLine_Classic (struct RastPort *,const LONG,const LONG,const LONG);

/* (3) Boundary trace */

/* (3a) Mandelbrot */
int LoadMandelClassic(unsigned);
void ScanMandelClassic(unsigned);
ULONG DrawMandelFractalBoundaryClassic (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (3b) Julia */

void AddQueueJuliaClassic(unsigned);
int LoadJuliaClassic(unsigned); 
void ScanJuliaClassic(unsigned); 
ULONG DrawJuliaFractalBoundaryClassic (struct Window *,const LONG,const LONG,const LONG,const LONG);

#endif /* CLASSIC_H */
