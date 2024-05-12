
/* FlashMandel functions for Vampire */

#ifndef VAMPIRE_H
#define VAMPIRE_H

#include <proto/intuition.h>

#include "fmglobals.h"
#include "fmcgx.h"
#include "shared.h"

/* (1) Brute force */

/* (1a) Mandelbrot */

ULONG DrawMandelFractalBruteVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (1b) Julia */
ULONG DrawJuliaFractalBruteVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (2) Tiling */
/* (2a) Mandelbrot */
/* (2aa) Horizontal */

void MHLine_Vampire (struct RastPort *,const LONG,const LONG,const LONG);
void MVLine_Vampire (struct RastPort *,const LONG,const LONG,const LONG);


/* (2b) Julia */ 
/* (2ba) Horizontal */

void JHLine_Vampire (struct RastPort *,const LONG,const LONG,const LONG);

/* (2bb) Vertical */

void JVLine_Vampire (struct RastPort *,const LONG,const LONG,const LONG);

/* (3) Boundary trace */

/* (3a) Mandelbrot */

UWORD LoadMandelVampire(ULONG); 
void ScanMandelVampire(ULONG); 
ULONG DrawMandelFractalBoundaryVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);


/* (3b) Julia */

UWORD LoadJuliaVampire(ULONG); 
ULONG DrawJuliaFractalBoundaryVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);

#endif /* VAMPIRE_H */

