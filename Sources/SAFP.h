 
#ifndef SAFP_H
#define SAFP_H

#include <exec/types.h>

/* General functions */
/* Set function pointer variables according to selection */

extern struct ILBMInfo MYILBM;

extern ULONG (*MultiMandelInCGeneric)(ULONG, double, double);
extern ULONG (*MultiJuliaInCGeneric)(ULONG,double,double);
extern ULONG (*BurningShipInCGeneric)(ULONG,double,double);
extern ULONG (*FractalInCGeneric)(ULONG,double,double);
extern ULONG (*FractalInCGenericStoreIterations)(ULONG,double,double);
extern void (*PutPixelWithColorMode)(ULONG, ULONG, ULONG);

#include "fmglobals.h"
#include "shared.h"
#include "classic.h"
#include "vampire.h"
#include "drawing.h"

extern VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

extern VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);
extern VOID (*MVLine) (struct RastPort *,const LONG,const LONG,const LONG);
extern VOID (*MHLine) (struct RastPort *,const LONG,const LONG,const LONG);
extern VOID (*JVLine) (struct RastPort *,const LONG,const LONG,const LONG);
extern VOID (*JHLine) (struct RastPort *,const LONG,const LONG,const LONG);
extern void   JVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   JHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   MVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   MHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   JVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   JHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   MVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
extern void   MHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
extern ULONG  DrawFractalTiling   (struct Window *,const LONG,const LONG,const LONG,const LONG);
extern ULONG  DrawMandelFractalBrute   (struct Window *,const LONG,const LONG,const LONG,const LONG);
extern ULONG (*DrawFractal) (struct Window *,const LONG,const LONG,const LONG,const LONG);
extern ULONG DrawMandelFractalBoundaryVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);

extern BOOL JULIA;

void SetHVLineMandel(void);
void SetHVLineJulia(void) ;
void SetHVLineMultiMandel(void); 
void SetHVLineMultiJulia(void);
void SetHVLineBurningShip(void); 
void SetHVLineBurningShipJulia(void); 
void SetHVLineVampire(void);
void SetHVLineClassic(void);

/* function to set all pointers correctly */

/***********************************************************/
/******* Mandel **********/
/* ----- Classic ------- */
void SAFPClassicMandelBruteWithJulia(void);
void SAFPClassicMandelBruteNoJulia(void);
void SAFPClassicMandelTileWithJulia(void);
void SAFPClassicMandelTileNoJulia(void);
void SAFPClassicMandelBoundaryWithJulia(void);
void SAFPClassicMandelBoundaryNoJulia(void);

/* ------ Vampire ------- */

void SAFPVampireMandelBruteWithJulia(void);
void SAFPVampireMandelBruteNoJulia(void);
void SAFPVampireMandelTileWithJulia(void);
void SAFPVampireMandelTileNoJulia(void);
void SAFPVampireMandelBoundaryWithJulia(void);
void SAFPVampireMandelBoundaryNoJulia(void);

/******* Multi **********/
/* ----- Classic ------- */
/* MultiInt */

void SAFPClassicMultiIntBruteWithJulia(void);
void SAFPClassicMultiIntBruteNoJulia(void);
void SAFPClassicMultiIntTileWithJulia(void);
void SAFPClassicMultiIntTileNoJulia(void);
void SAFPClassicMultiIntBoundaryWithJulia(void);
void SAFPClassicMultiIntBoundaryNoJulia(void);

/* MultiFloat */

void SAFPClassicMultiFloatBruteWithJulia(void);
void SAFPClassicMultiFloatBruteNoJulia(void);
void SAFPClassicMultiFloatTileWithJulia(void);
void SAFPClassicMultiFloatTileNoJulia(void);
void SAFPClassicMultiFloatBoundaryWithJulia(void);
void SAFPClassicMultiFloatBoundaryNoJulia(void);

/* ------ Vampire ------- */
/* MultiInt */

void SAFPVampireMultiIntBruteWithJulia(void);
void SAFPVampireMultiIntBruteNoJulia(void);
void SAFPVampireMultiIntTileWithJulia(void);
void SAFPVampireMultiIntTileNoJulia(void);
void SAFPVampireMultiIntBoundaryWithJulia(void);
void SAFPVampireMultiIntBoundaryNoJulia(void);

/* MultiFloat */

void SAFPVampireMultiFloatBruteWithJulia(void);
void SAFPVampireMultiFloatBruteNoJulia(void);
void SAFPVampireMultiFloatTileWithJulia(void);
void SAFPVampireMultiFloatTileNoJulia(void);
void SAFPVampireMultiFloatBoundaryWithJulia(void);
void SAFPVampireMultiFloatBoundaryNoJulia(void);

/******* Buddha **********/
/* ----- Classic ------- */
void SAFPClassicBuddhaBruteWithJulia(void);
void SAFPClassicBuddhaBruteNoJulia(void);
void SAFPClassicBuddhaTileWithJulia(void);
void SAFPClassicBuddhaTileNoJulia(void);
void SAFPClassicBuddhaBoundaryWithJulia(void);
void SAFPClassicBuddhaBoundaryNoJulia(void);

/* ------ Vampire ------- */

void SAFPVampireBuddhaBruteWithJulia(void);
void SAFPVampireBuddhaBruteNoJulia(void);
void SAFPVampireBuddhaTileWithJulia(void);
void SAFPVampireBuddhaTileNoJulia(void);
void SAFPVampireBuddhaBoundaryWithJulia(void);
void SAFPVampireBuddhaBoundaryNoJulia(void);

/******* Burning Ship **********/
/* ----- Classic ------- */
void SAFPClassicBurningShipBruteWithJulia(void);
void SAFPClassicBurningShipBruteNoJulia(void);
void SAFPClassicBurningShipTileWithJulia(void);
void SAFPClassicBurningShipTileNoJulia(void);
void SAFPClassicBurningShipBoundaryWithJulia(void);
void SAFPClassicBurningShipBoundaryNoJulia(void);

/* ------ Vampire ------- */

void SAFPVampireBurningShipBruteWithJulia(void);
void SAFPVampireBurningShipBruteNoJulia(void);
void SAFPVampireBurningShipTileWithJulia(void);
void SAFPVampireBurningShipTileNoJulia(void);
void SAFPVampireBurningShipBoundaryWithJulia(void);
void SAFPVampireBurningShipBoundaryNoJulia(void);

/**************************************************************************/
/**************************************************************************/

/******* Mandel **********/
/* ----- Classic ------- */

void SAFPClassicMandelBrute(void);
void SAFPClassicMandelTile(void);
void SAFPClassicMandelBoundary(void);

/* ------ Vampire ------- */

void SAFPVampireMandelBrute(void);
void SAFPVampireMandelTile(void);
void SAFPVampireMandelBoundary(void);

/******* Multi **********/
/* ----- Classic ------- */
/* MultiInt */

void SAFPClassicMultiIntBrute(void);
void SAFPClassicMultiIntTile(void);
void SAFPClassicMultiIntBoundary(void);

/* MultiFloat */

void SAFPClassicMultiFloatBrute(void);
void SAFPClassicMultiFloatTile(void);
void SAFPClassicMultiFloatBoundary(void);

/* ------ Vampire ------- */
/* MultiInt */

void SAFPVampireMultiIntBrute(void);
void SAFPVampireMultiIntTile(void);
void SAFPVampireMultiIntBoundary(void);

/* MultiFloat */

void SAFPVampireMultiFloatBrute(void);
void SAFPVampireMultiFloatTile(void);
void SAFPVampireMultiFloatBoundary(void);

/******* Buddha **********/
/* ----- Classic ------- */
void SAFPClassicBuddhaBrute(void);
void SAFPClassicBuddhaTile(void);
void SAFPClassicBuddhaBoundary(void);

/* ----- Vampire ------- */
void SAFPVampireBuddhaBrute(void);
void SAFPVampireBuddhaTile(void);
void SAFPVampireBuddhaBoundary(void);

/******* Burning Ship *****/
/* ----- Classic ------- */
void SAFPClassicBurningShipBrute(void);
void SAFPClassicBurningShipTile(void);
void SAFPClassicBurningShipBoundary(void);

/* ----- Vampire ------- */
void SAFPVampireBurningShipBrute(void);
void SAFPVampireBurningShipTile(void);
void SAFPVampireBurningShipBoundary(void);

/*********************************************************************/
/*********************************************************************/

/******* Mandel **********/
/* ----- Classic ------- */
void SAFPClassicMandel(void);

/* ------ Vampire ------- */
void SAFPVampireMandel(void);

/********* Multi ************/
/* ----- Classic ------- */
void SAFPClassicMultiInt(void);
void SAFPClassicMultiFloat(void);

/* ------ Vampire ------- */
void SAFPVampireMultiInt(void);
void SAFPVampireMultiFloat(void);

/********** Buddha *************/
/* ----- Classic ------------- */
void SAFPClassicBuddha(void);

/* ------ Vampire ------- */
void SAFPVampireBuddha(void);

/********* Burning Ship ****/
/* ----- Classic ------- */
void SAFPClassicBurningShip(void);
	
/* ------ Vampire ------- */
void SAFPVampireBurningShip(void);

/*********************************************************************/
/*********************************************************************/

/* ----- Classic ------- */

void SAFPClassic(void); 

/* ------ Vampire ------- */
void SAFPVampire(void); 

/*********************************************************************/
/*********************************************************************/

void SetUpGenericFunctions(void); 

/*********************************************************************/
/*********************************************************************/

void SetUpPutPixelWithColorModeFunctions(void); 

/*********************************************************************/
/*********************************************************************/

void SAFP(void); 

/********************************************************************/
/*********************************************************************/
/*********************************************************************/

#endif /* SAFP_H */