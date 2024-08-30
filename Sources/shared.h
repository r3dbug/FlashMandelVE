
#ifndef SHARED_H
#define SHARED_H

#include <proto/exec.h>
#include <exec/types.h>
#include <math.h>
#include "fmglobals.h"
#include "math_functions.h"
#include "compilerspecific.h"

/* FlashMandel functions shared between Classic Amigas and  Vampire */

IMPORT double ASMCALL powFPU (REG (fp0,double),REG (fp1,double)); 
IMPORT double ASMCALL sinFPU (REG (fp0,double)); 
IMPORT double ASMCALL cosFPU (REG (fp0,double)); 
IMPORT void ASMCALL sincosFPU (REG (fp0,double), REG(fp1,double)); 
IMPORT double ASMCALL atan2FPU (REG (fp0,double),REG (fp1,double)); 
IMPORT double ASMCALL atan2andpowFPU (REG (fp0,double),REG (fp1,double), REG(fp2,double), REG(fp3,double)); 
IMPORT double ASMCALL arctanFPU (REG (fp0,double)); 

extern VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

extern VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

/* Common global variables (from boundary) */
extern UBYTE *Data;
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

extern ULONG CURRENT_MAX_COLORS;
/*extern UWORD ITERATIONS;
extern WORD MAX_ITERATIONS,  BUDDHA_MIN_ITERATIONS, RETURNVALUE;
*/
extern double RMIN,RMAX,IMIN,IMAX;

extern double DEF_RMIN,DEF_RMAX,DEF_IMIN,DEF_IMAX,DEF_JKRE,DEF_JKIM,URMIN,URMAX,UIMIN,UIMAX,UJKRE,UJKIM;

extern double *re;
extern double *im;

extern ULONG MultiPower;
extern double MultiPowerFloat;


/*extern double FACTOR,INCREMREAL,INCREMIMAG,CRE,CIM,JKRE,JKIM; */
extern ULONG MultiPower;
extern double MultiPowerFloat;

extern UBYTE DrawOrbits;
extern UBYTE PeriodicityColoring;

extern UWORD BaseFractalType;
extern UWORD FractalType;
extern double GlobalPowerVar;

/* variables for optimized routines */
extern double FinalXP1, FinalXP2, FinalXP3, FinalXP4;
extern double FinalYP1, FinalYP2, FinalYP3, FinalYP4;
extern double GlobalP;

extern ULONG MASK;

extern UBYTE *PixelLine;
extern struct RastPort TempRP;

extern ULONG* BuddhaCounter;

extern UBYTE ANTIBUDDHA;
extern UBYTE BUDDHA_USE_SYMMETRY;
extern double BUDDHA_BOUNDARY_RANDOMIZATION;
extern UBYTE BUDDHA_RANDOMIZATION_FREQUENCY; /* 1<<0 = 1 => every second point is random = 50% */
extern UBYTE BUDDHA_RESUME;

extern ULONG NEBULA_WIDTH;
extern ULONG NEBULA_HEIGHT;
extern UBYTE SMOOTH_COLORING;

extern UBYTE optimized;
extern UBYTE vampire;
extern UBYTE algorithm, algorithm_buddha;

extern ULONG FM_REDITER, FM_GREENITER, FM_BLUEITER;
extern ULONG FM_REDMIN, FM_GREENMIN, FM_BLUEMIN;

extern UBYTE SHOW_MAXCOUNTERS;

extern ULONG (*FractalIterationGeneric)(ULONG,double,double);

#include "drawing.h"
#include "math_functions.h"

/***********************************************/
/* Functions / Algorithms for smooth coloring  */
/***********************************************/
BOOL AllocIterArray(void);
void FreeIterArray(void);
BOOL AllocBuddha(void);
void FreeBuddha(void);
void InitBuddha(void);
void FreeHitmap(void);

LONG lround(double);
double RenormalizedIterationCount(ULONG, double, double, double);
/* PutPixel function depending on color mode */
/* Classic functions with palette for CLUT and RGB */
void PutPixelWithColorModeCLUT(ULONG, ULONG, ULONG);
/* A special function for RGB is not necessary: 
 * the already defined PutPixel(x,y,Color) can be used.
 */
/* New functions for smooth colors */
/* Renormalized Iteration Count (RIC) */
void PutPixelWithColorModeRGBSmoothRIC(ULONG, ULONG, ULONG);
/* Cyclic Sinus Coloring (FlashMandelNG / FMNG) */
void PutPixelWithColorModeRGBSmoothFMNG(ULONG, ULONG, ULONG);

/******************************/
/* Mandel and Julia functions */
/******************************/

ULONG MandelInC(ULONG, double, double);
ULONG MandelInCStoreIterations(ULONG, double, double);
ULONG JuliaInC(ULONG, double, double, double, double); 
ULONG MultiMandelInC(ULONG, double, double);
ULONG MultiMandelInCStoreIterations(ULONG, double, double);
ULONG MultiJuliaInC(ULONG, double, double);
/* functions when MultiPowerFloat is a float */
ULONG MultiJuliaFloatingPowerInC(ULONG, double, double);
ULONG MultiMandelFloatingPowerInC(ULONG, double, double);
ULONG MultiMandelFloatingPowerInCStoreIterations(ULONG, double, double);

/************ generic boundary ***********/
UWORD LoadFractalGeneric(ULONG); 
void ScanFractalGeneric(ULONG); 
ULONG DrawFractalBoundaryGeneric (struct Window *,const LONG,const LONG,const LONG,const LONG);

/**********************************************************************************************/
/********************************** BRUTE FORCE ***********************************************/
/**********************************************************************************************/

/* (1) Brute force */
/* (1a) Mandelbrot */
/* (1b) Julia */
/* (1c) Multibrot */

ULONG DrawMultiMandelFractalBrute (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* (1d) MultiJulia */

ULONG DrawMultiJuliaFractalBrute (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* 1e) burning ship */

ULONG BurningShipInC(ULONG, double, double);
ULONG BurningShipInCStoreIterations(ULONG, double, double);
ULONG BurningShipInCJulia(ULONG, double, double);
ULONG DrawBurningShipFractalBrute (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* Tricorn */

ULONG TricornInC(ULONG, double, double);
ULONG DrawTricornFractalBrute (struct Window *,const LONG,const LONG,const LONG,const LONG);

/**********************************************************************************************/
/********************************** TILING ****************************************************/
/**********************************************************************************************/

/* (2) Tiling - for both Mandelbrot and Julia*/
/* These funtions are separated via function pointers H_LINE, V_LINE */

BOOL RectangleDraw (struct Window *,const LONG,const LONG,const LONG,const LONG);
ULONG DrawFractalTiling (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* Multibrot => needs separate functions */

/* (2aa) Horizontal */

void MMHLine (struct RastPort *,const LONG,const LONG,const LONG);

/* (2ab) Vertical */

void MMVLine (struct RastPort *,const LONG,const LONG,const LONG);

/* Multibrot Julia Horizontal */

void MJHLine (struct RastPort *,const LONG,const LONG,const LONG);

/* (3bb) Vertical */

void MJVLine (struct RastPort *,const LONG,const LONG,const LONG);

/* Burning Ship => separate functions */

/* (4aa) Horizontal */

void BSHLine (struct RastPort *,const LONG,const LONG,const LONG);

/* (4ab) Vertical */

void BSVLine (struct RastPort *,const LONG,const LONG,const LONG);


/**********************************************************************************************/
/********************************** BOUNDARY TRACE ********************************************/
/**********************************************************************************************/

/* (3) Boundary trace */

/* Memory allocation */
int AllocateBoundary(void);
void DeallocateBoundary(void);

/* Build the queue */
void AddQueue(unsigned);
void ClearBoundaryBuffers(void); 

/* (3a) Mandelbrot */
/* (3b) Julia */
/* (3c) MultiMandel */

/* (3d) MultiJulia */

/* 1d) Burning ship boundary */

/* different algorithms (= no brute, tiling, boundary) */

ULONG DrawBuddhaFractalRandomNumbers (struct Window *,const LONG,const LONG,const LONG,const LONG);
void StoreIterations(UBYTE, ULONG, ULONG, double, double, ULONG, ULONG);
void printhistogramstats(void);
ULONG DrawTrueColorBuddhaFractalRandomNumbers (struct Window *,const LONG,const LONG,const LONG,const LONG);

#endif /* SHARED_H */
