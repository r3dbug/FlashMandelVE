 
/* General functions */
/* Set function pointer variables according to selection */

#include "SAFP.h"

void SetHVLineMandel(void) 
{
	H_LINE = MHLine;
    V_LINE = MVLine;
    JULIA = FALSE;
}

void SetHVLineJulia(void) 
{
	H_LINE = JHLine;
    V_LINE = JVLine;
    JULIA = TRUE;											  
}

void SetHVLineMultiMandel(void) 
{
	H_LINE = MMHLine;
    V_LINE = MMVLine;
    JULIA = FALSE;
}

void SetHVLineMultiJulia(void) 
{
	H_LINE = MJHLine;
    V_LINE = MJVLine;
    JULIA = TRUE;											  
}

void SetHVLineBurningShip(void) 
{
	H_LINE = BSHLine;
    V_LINE = BSVLine;
    JULIA = FALSE;
}

void SetHVLineBurningShipJulia(void) 
{
	H_LINE = BSHLine;
    V_LINE = BSVLine;
    JULIA = TRUE;
}

void SetHVLineVampire(void) 
{
    /*printf("SetHVLineVampire()\n");*/
    optimized=1;
    MHLine=MHLine_Vampire;
    MVLine=MVLine_Vampire;
    JHLine=JHLine_Vampire;
    JVLine=JVLine_Vampire;  
}

void SetHVLineClassic(void) 
{
	optimized=0;
    MHLine=MHLine_Classic;
    MVLine=MVLine_Classic;
    JHLine=JHLine_Classic;
    JVLine=JVLine_Classic; 
}


/* function to set all pointers correctly */

/***********************************************************/
/******* Mandel **********/
/* ----- Classic ------- */
void SAFPClassicMandelBruteWithJulia(void)
{
	 SetHVLineClassic();
	 SetHVLineJulia();	 
	 DrawFractal = DrawJuliaFractalBruteClassic;
}

void SAFPClassicMandelBruteNoJulia(void)
{
     SetHVLineClassic();
	 SetHVLineMandel();
	 /* SetHVLineClassic(); */
	 DrawFractal = DrawMandelFractalBruteClassic;
}

void SAFPClassicMandelTileWithJulia(void)
{
	 SetHVLineClassic();
	 SetHVLineJulia();
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMandelTileNoJulia(void)
{
	 SetHVLineClassic();
	 SetHVLineMandel();
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMandelBoundaryWithJulia(void)
{
	 DrawFractal = DrawJuliaFractalBoundaryClassic;
}

void SAFPClassicMandelBoundaryNoJulia(void)
{
	 DrawFractal = DrawMandelFractalBoundaryClassic;
}

/* ------ Vampire ------- */

void SAFPVampireMandelBruteWithJulia(void)
{
	 SetHVLineVampire	();
	 SetHVLineJulia();	 
	 DrawFractal = DrawJuliaFractalBruteVampire;
}

void SAFPVampireMandelBruteNoJulia(void)
{
	 SetHVLineVampire();
	 SetHVLineMandel();
	 DrawFractal = DrawMandelFractalBruteVampire;
}

void SAFPVampireMandelTileWithJulia(void)
{
	 SetHVLineVampire();
	 SetHVLineJulia();
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMandelTileNoJulia(void)
{
	 /*printf("SAFPVampireMandelTileNoJulia()\n");*/	 
     SetHVLineVampire();
	 SetHVLineMandel();
	 /*printf("DrawFractal = DrawFractalTiling (%p)\n", DrawFractalTiling);*/
     DrawFractal = DrawFractalTiling;
}

void SAFPVampireMandelBoundaryWithJulia(void)
{
	 DrawFractal = DrawJuliaFractalBoundaryVampire;
}

void SAFPVampireMandelBoundaryNoJulia(void)
{
	 DrawFractal = DrawMandelFractalBoundaryVampire;
}

/******* Multi **********/
/* ----- Classic ------- */
/* MultiInt */

void SAFPClassicMultiIntBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPClassicMultiIntBruteNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelFloatingPowerInC;
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPClassicMultiIntTileWithJulia(void)
{
	 /* test */
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=MultiJuliaInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiIntTileNoJulia(void)
{
	 /* test */
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=MultiMandelInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiIntBoundaryWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaInC;
	 DrawFractal = DrawMultiJuliaFractalBoundary;	
}

void SAFPClassicMultiIntBoundaryNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelInC;
	 DrawFractal = DrawMultiMandelFractalBoundary;
}

/* MultiFloat */

void SAFPClassicMultiFloatBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPClassicMultiFloatBruteNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelFloatingPowerInC;
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPClassicMultiFloatTileWithJulia(void)
{
	 /* test */
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiFloatTileNoJulia(void)
{
	 /* test */
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=MultiMandelFloatingPowerInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiFloatBoundaryWithJulia(void)
{
 	 MultiJuliaInCGeneric = MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawMultiJuliaFractalBoundary;	
}

void SAFPClassicMultiFloatBoundaryNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelFloatingPowerInC;
	 DrawFractal = DrawMultiMandelFractalBoundary;
}

/* ------ Vampire ------- */
/* MultiInt */

void SAFPVampireMultiIntBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaInC;
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPVampireMultiIntBruteNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelInC;
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPVampireMultiIntTileWithJulia(void)
{
	 /* test */
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=MultiJuliaInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiIntTileNoJulia(void)
{
	 /* test */
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=MultiMandelInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiIntBoundaryWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaInC;
	 DrawFractal = DrawMultiJuliaFractalBoundary;	
}

void SAFPVampireMultiIntBoundaryNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelInC;
	 DrawFractal = DrawMultiMandelFractalBoundary;
}

/* MultiFloat */

void SAFPVampireMultiFloatBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPVampireMultiFloatBruteNoJulia(void)
{
	 MultiMandelInCGeneric = MultiMandelFloatingPowerInC;
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPVampireMultiFloatTileWithJulia(void)
{
	 /* test */
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiFloatTileNoJulia(void)
{
	 /* test */
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=MultiMandelFloatingPowerInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiFloatBoundaryWithJulia(void)
{
	MultiJuliaInCGeneric = MultiJuliaFloatingPowerInC;
	DrawFractal = DrawMultiJuliaFractalBoundary;	
}

void SAFPVampireMultiFloatBoundaryNoJulia(void)
{
	MultiMandelInCGeneric = MultiMandelFloatingPowerInC;
	DrawFractal = DrawMultiMandelFractalBoundary;
}

/******* Buddha **********/
/* ----- Classic ------- */
void SAFPClassicBuddhaBruteWithJulia(void)
{
/*	 SetHVLineJulia();
	 SetHVLineClassic();
*/	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBruteNoJulia(void)
{
/*	 SetHVLineMandel();
	 SetHVLineClassic();
*/	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaTileWithJulia(void)
{
/*	 SetHVLineJulia();
	 SetHVLineClassic();
*/	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaTileNoJulia(void)
{
/*	 SetHVLineMandel();
	 SetHVLineClassic();
*/	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBoundaryWithJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBoundaryNoJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

/* ------ Vampire ------- */

void SAFPVampireBuddhaBruteWithJulia(void)
{
/*	 SetHVLineJulia();
	 SetHVLineVampire();
*/	 
	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}

void SAFPVampireBuddhaBruteNoJulia(void)
{
/*	 SetHVLineMandel();
	 SetHVLineVampire();
*/	 
	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}

void SAFPVampireBuddhaTileWithJulia(void)
{
/*	 SetHVLineJulia();
	 SetHVLineVampire();
*/	 
	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}

void SAFPVampireBuddhaTileNoJulia(void)
{
/*	 SetHVLineMandel();
	 SetHVLineVampire();
*/	 
	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}

void SAFPVampireBuddhaBoundaryWithJulia(void)
{
/*	 DrawFractal = DrawBuddhaFractalRandomNumbers;
*/	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}

void SAFPVampireBuddhaBoundaryNoJulia(void)
{
/*	 DrawFractal = DrawBuddhaFractalRandomNumbers;
*/
	switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	}
}


/******* Burning Ship **********/
/* ----- Classic ------- */
void SAFPClassicBurningShipBruteWithJulia(void)
{
/*	 SetHVLineJulia();
	 SetHVLineClassic();
*/    BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawBurningShipFractalBrute; /* JuliaFractalBruteClassic; */
}

void SAFPClassicBurningShipBruteNoJulia(void)
{
/*	 SetHVLineMandel();
	 SetHVLineClassic();
*/     BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPClassicBurningShipTileWithJulia(void)
{
	 SetHVLineBurningShipJulia();
/*	 SetHVLineClassic();
*/	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicBurningShipTileNoJulia(void)
{
	 SetHVLineBurningShip();
/*	 SetHVLineClassic();
*/	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicBurningShipBoundaryWithJulia(void)
{
	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawBurningShipFractalBoundary;
}

void SAFPClassicBurningShipBoundaryNoJulia(void)
{
	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawBurningShipFractalBoundary;
}

/* ------ Vampire ------- */

void SAFPVampireBurningShipBruteWithJulia(void)
{
	 SetHVLineBurningShipJulia();
/*	 SetHVLineVampire();
*/	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPVampireBurningShipBruteNoJulia(void)
{
	 SetHVLineBurningShip();
/*	 SetHVLineVampire();
*/	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPVampireBurningShipTileWithJulia(void)
{
	 SetHVLineBurningShipJulia();
/*	 SetHVLineVampire();
*/	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireBurningShipTileNoJulia(void)
{
	 SetHVLineBurningShip();
/*	 SetHVLineVampire();
*/	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireBurningShipBoundaryWithJulia(void)
{
	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawBurningShipFractalBoundary;
}

void SAFPVampireBurningShipBoundaryNoJulia(void)
{
	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawBurningShipFractalBoundary;
}


/**************************************************************************/
/**************************************************************************/

/******* Mandel **********/
/* ----- Classic ------- */

void SAFPClassicMandelBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMandelBruteWithJulia(); break;
		case FALSE : SAFPClassicMandelBruteNoJulia(); break;	 
	 }
}

void SAFPClassicMandelTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMandelTileWithJulia(); break;
		case FALSE : SAFPClassicMandelTileNoJulia(); break;	 
	 }
}

void SAFPClassicMandelBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMandelBoundaryWithJulia(); break;
		case FALSE : SAFPClassicMandelBoundaryNoJulia(); break;	 
	 }
}

/* ------ Vampire ------- */

void SAFPVampireMandelBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMandelBruteWithJulia(); break;
		case FALSE : SAFPVampireMandelBruteNoJulia(); break;	 
	 }
}

void SAFPVampireMandelTile(void)
{
	 /*printf("SAFPVampireMandelTile()\n");*/ 
     switch (JULIA) 
	 {
		case TRUE : SAFPVampireMandelTileWithJulia(); break;
		case FALSE : SAFPVampireMandelTileNoJulia(); break;	 
	 }
}

void SAFPVampireMandelBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMandelBoundaryWithJulia(); break;
		case FALSE : SAFPVampireMandelBoundaryNoJulia(); break;	 
	 }
}

/******* Multi **********/
/* ----- Classic ------- */
/* MultiInt */

void SAFPClassicMultiIntBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiIntBruteWithJulia(); break;
		case FALSE : SAFPClassicMultiIntBruteNoJulia(); break;	 
	 }
}

void SAFPClassicMultiIntTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiIntTileWithJulia(); break;
		case FALSE : SAFPClassicMultiIntTileNoJulia(); break;	 
	 }
}

void SAFPClassicMultiIntBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiIntBoundaryWithJulia(); break;
		case FALSE : SAFPClassicMultiIntBoundaryNoJulia(); break;	 
	 }
}

/* MultiFloat */

void SAFPClassicMultiFloatBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiFloatBruteWithJulia(); break;
		case FALSE : SAFPClassicMultiFloatBruteNoJulia(); break;	 
	 }
}

void SAFPClassicMultiFloatTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiFloatTileWithJulia(); break;
		case FALSE : SAFPClassicMultiFloatTileNoJulia(); break;	 
	 }
}

void SAFPClassicMultiFloatBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicMultiFloatBoundaryWithJulia(); break;
		case FALSE : SAFPClassicMultiFloatBoundaryNoJulia(); break;	 
	 }
}

/* ------ Vampire ------- */
/* MultiInt */

void SAFPVampireMultiIntBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiIntBruteWithJulia(); break;
		case FALSE : SAFPVampireMultiIntBruteNoJulia(); break;	 
	 }
}

void SAFPVampireMultiIntTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiIntTileWithJulia(); break;
		case FALSE : SAFPVampireMultiIntTileNoJulia(); break;	 
	 }
}

void SAFPVampireMultiIntBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiIntBoundaryWithJulia(); break;
		case FALSE : SAFPVampireMultiIntBoundaryNoJulia(); break;	 
	 }
}

/* MultiFloat */

void SAFPVampireMultiFloatBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiFloatBruteWithJulia(); break;
		case FALSE : SAFPVampireMultiFloatBruteNoJulia(); break;	 
	 }
}

void SAFPVampireMultiFloatTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiFloatTileWithJulia(); break;
		case FALSE : SAFPVampireMultiFloatTileNoJulia(); break;	 
	 }
}

void SAFPVampireMultiFloatBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireMultiFloatBoundaryWithJulia(); break;
		case FALSE : SAFPVampireMultiFloatBoundaryNoJulia(); break;	 
	 }
}

/******* Buddha **********/
/* ----- Classic ------- */
void SAFPClassicBuddhaBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBuddhaBruteWithJulia(); break;
		case FALSE : SAFPClassicBuddhaBruteNoJulia(); break;	 
	 }
}

void SAFPClassicBuddhaTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBuddhaTileWithJulia(); break;
		case FALSE : SAFPClassicBuddhaTileNoJulia(); break;	 
	 }
}

void SAFPClassicBuddhaBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBuddhaBoundaryWithJulia(); break;
		case FALSE : SAFPClassicBuddhaBoundaryNoJulia(); break;	 
	 }
}

/* ----- Vampire ------- */
void SAFPVampireBuddhaBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBuddhaBruteWithJulia(); break;
		case FALSE : SAFPVampireBuddhaBruteNoJulia(); break;	 
	 }
}

void SAFPVampireBuddhaTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBuddhaTileWithJulia(); break;
		case FALSE : SAFPVampireBuddhaTileNoJulia(); break;	 
	 }
}

void SAFPVampireBuddhaBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBuddhaBoundaryWithJulia(); break;
		case FALSE : SAFPVampireBuddhaBoundaryNoJulia(); break;	 
	 }
}

/******* Burning Ship *****/
/* ----- Classic ------- */
void SAFPClassicBurningShipBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBurningShipBruteWithJulia(); break;
		case FALSE : SAFPClassicBurningShipBruteNoJulia(); break;	 
	 }
}

void SAFPClassicBurningShipTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBurningShipTileWithJulia(); break;
		case FALSE : SAFPClassicBurningShipTileNoJulia(); break;	 
	 }
}

void SAFPClassicBurningShipBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPClassicBurningShipBoundaryWithJulia(); break;
		case FALSE : SAFPClassicBurningShipBoundaryNoJulia(); break;	 
	 }
}

/* ----- Vampire ------- */
void SAFPVampireBurningShipBrute(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBurningShipBruteWithJulia(); break;
		case FALSE : SAFPVampireBurningShipBruteNoJulia(); break;	 
	 }
}

void SAFPVampireBurningShipTile(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBurningShipTileWithJulia(); break;
		case FALSE : SAFPVampireBurningShipTileNoJulia(); break;	 
	 }
}

void SAFPVampireBurningShipBoundary(void)
{
	 switch (JULIA) 
	 {
		case TRUE : SAFPVampireBurningShipBoundaryWithJulia(); break;
		case FALSE : SAFPVampireBurningShipBoundaryNoJulia(); break;	 
	 }
}

/*********************************************************************/
/*********************************************************************/

/******* Mandel **********/
/* ----- Classic ------- */
void SAFPClassicMandel(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPClassicMandelBrute(); break;
		case TILE : SAFPClassicMandelTile(); break;
		case BOUNDARY : SAFPClassicMandelBoundary(); break;	
	}
}

/* ------ Vampire ------- */
void SAFPVampireMandel(void)
{
	/*printf("SAFPVampireMandel()\n");*/
    switch (algorithm)
	{
		case BRUTE : SAFPVampireMandelBrute(); break;
		case TILE : SAFPVampireMandelTile(); break;
		case BOUNDARY : SAFPVampireMandelBoundary(); break;	
	}
}

/********* Multi ************/
/* ----- Classic ------- */
void SAFPClassicMultiInt(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPClassicMultiIntBrute(); break;
		case TILE : SAFPClassicMultiIntTile(); break;
		case BOUNDARY : SAFPClassicMultiIntBoundary(); break;	
	}
}

void SAFPClassicMultiFloat(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPClassicMultiFloatBrute(); break;
		case TILE : SAFPClassicMultiFloatTile(); break;
		case BOUNDARY : SAFPClassicMultiFloatBoundary(); break;	
	}
}

/* ------ Vampire ------- */
void SAFPVampireMultiInt(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPVampireMultiIntBrute(); break;
		case TILE : SAFPVampireMultiIntTile(); break;
		case BOUNDARY : SAFPVampireMultiIntBoundary(); break;	
	}
}

void SAFPVampireMultiFloat(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPVampireMultiFloatBrute(); break;
		case TILE : SAFPVampireMultiFloatTile(); break;
		case BOUNDARY : SAFPVampireMultiFloatBoundary(); break;	
	}
}

/********** Buddha *************/
/* ----- Classic ------------- */
void SAFPClassicBuddha(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPClassicBuddhaBrute(); break;
		case TILE : SAFPClassicBuddhaTile(); break;
		case BOUNDARY : SAFPClassicBuddhaBoundary(); break;	
	}
}

/* ------ Vampire ------- */
void SAFPVampireBuddha(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPVampireBuddhaBrute(); break;
		case TILE : SAFPVampireBuddhaTile(); break;
		case BOUNDARY : SAFPVampireBuddhaBoundary(); break;	
	}
}

/********* Burning Ship ****/
/* ----- Classic ------- */
void SAFPClassicBurningShip(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPClassicBurningShipBrute(); break;
		case TILE : SAFPClassicBurningShipTile(); break;
		case BOUNDARY : SAFPClassicBurningShipBoundary(); break;	
	}
}
	
/* ------ Vampire ------- */
void SAFPVampireBurningShip(void)
{
	switch (algorithm)
	{
		case BRUTE : SAFPVampireBurningShipBrute(); break;
		case TILE : SAFPVampireBurningShipTile(); break;
		case BOUNDARY : SAFPVampireBurningShipBoundary(); break;	
	}
}

/*********************************************************************/
/*********************************************************************/

/* ----- Classic ------- */

void SAFPClassic(void) 
{
	switch (FractalType)
	{
		case MANDEL : SAFPClassicMandel(); break;
		case MULTIINT : SAFPClassicMultiInt(); break;
		case MULTIFLOAT : SAFPClassicMultiFloat(); break;
		case BUDDHA : SAFPClassicBuddha(); break;
		case BURNINGSHIP : SAFPClassicBurningShip(); break;
	}
}

/* ------ Vampire ------- */
void SAFPVampire(void) 
{
	/*printf("SAFPVampire()\n");*/	
    switch (FractalType)
	{
		case MANDEL : SAFPVampireMandel(); break;
		case MULTIINT : SAFPVampireMultiInt(); break;
		case MULTIFLOAT : SAFPVampireMultiFloat(); break;
		case BUDDHA : SAFPVampireBuddha(); break;
		case BURNINGSHIP : SAFPVampireBurningShip(); break;
	}
}

/*********************************************************************/
/*********************************************************************/

void SetUpGenericFunctions(void) 
{
	switch (BaseFractalType)
	{
		case MANDEL : 	FractalInCGeneric = MandelInC;
						FractalInCGenericStoreIterations = MandelInCStoreIterations;	
						break;
		case MULTIINT :
						FractalInCGeneric = MultiMandelInCGeneric;
					    FractalInCGenericStoreIterations = MultiMandelInCStoreIterations;
						break;
		case MULTIFLOAT :
						FractalInCGeneric = MultiMandelInCGeneric;
						FractalInCGenericStoreIterations = MultiMandelFloatingPowerInCStoreIterations;
						break;
		case BURNINGSHIP:
						FractalInCGeneric = BurningShipInCGeneric;
						FractalInCGenericStoreIterations = BurningShipInCStoreIterations;
						break;
	}	
}

/*********************************************************************/
/*********************************************************************/

void SetUpPutPixelWithColorModeFunctions(void) 
{
	/*printf("DD_BPP: %d, GetBitMapBPP(): %d\n", DD_BPP, GetBitMapBPP(MYILBM.win));*/
	switch (DD_BPP) {
		case 1 : PutPixelWithColorMode = PutPixelWithColorModeCLUT; break; /* no smooth coloring in 8bit CLUT */
		default :
			switch (SMOOTH_COLORING)
			{
				case SMOOTH_RIC : PutPixelWithColorMode = PutPixelWithColorModeRGBSmoothRIC; break;
				case SMOOTH_FMNG : PutPixelWithColorMode = PutPixelWithColorModeRGBSmoothFMNG; break; 		
				default :
					PutPixelWithColorMode = PutPixelWithColorModeCLUT; /* no smooth coloring, classic palette on RGB */
			}
			    
	}
}

/*********************************************************************/
/*********************************************************************/

void SAFP(void) 
{
	/*printf("SAFP(void)\n");*/	
    SetUpDirectDrawing(MYILBM.win);
	if (optimized) SAFPVampire();
	else SAFPClassic();	
	SetUpGenericFunctions();
	SetUpPutPixelWithColorModeFunctions();
}

/********************************************************************/
/*********************************************************************/
/*********************************************************************/