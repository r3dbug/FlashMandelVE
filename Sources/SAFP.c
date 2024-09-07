 
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
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))Julia68k_FPU_Classic;
}

void SAFPClassicMandelBoundaryNoJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))Mandel68k_FPU_Classic;
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
     SetHVLineVampire();
	 SetHVLineMandel();
     DrawFractal = DrawFractalTiling;
}

void SAFPVampireMandelBoundaryWithJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))Julia68k_FPU_Vampire;
}

void SAFPVampireMandelBoundaryNoJulia(void)
{
     /* printf("SAFPVampireMandelBoundaryNoJulia() => Mandel68k_FPU_Vampire\n"); */
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))Mandel68k_FPU_Vampire;
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
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaInC;	
}

void SAFPClassicMultiIntBoundaryNoJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiMandelInC;
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
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=MultiJuliaFloatingPowerInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiFloatTileNoJulia(void)
{
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=MultiMandelFloatingPowerInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicMultiFloatBoundaryWithJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaFloatingPowerInC;
}

void SAFPClassicMultiFloatBoundaryNoJulia(void)
{
 	 DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiMandelFloatingPowerInC;     
}

/* ------ Vampire ------- */
/* MultiInt */

void SAFPVampireMultiIntBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaInASM;
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPVampireMultiIntBruteNoJulia(void)
{
     MultiMandelInCGeneric = (ULONG(*)(ULONG, double, double))MultiMandelInASM; 
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPVampireMultiIntTileWithJulia(void)
{
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=(ULONG(*)(ULONG, double, double))MultiJuliaInC; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiIntTileNoJulia(void)
{
	 SetHVLineMultiMandel();
     MultiMandelInCGeneric=(ULONG(*)(ULONG, double, double))MultiMandelInASM;
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiIntBoundaryWithJulia(void)
{
  	 DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaInASM;
}

void SAFPVampireMultiIntBoundaryNoJulia(void)
{
 	 DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiMandelInASM;
}

/* MultiFloat */

void SAFPVampireMultiFloatBruteWithJulia(void)
{
	 MultiJuliaInCGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaFloatingPowerInASM; 
	 DrawFractal = DrawMultiJuliaFractalBrute;
}

void SAFPVampireMultiFloatBruteNoJulia(void)
{
	 MultiMandelInCGeneric = (ULONG(*)(ULONG, double, double))MultiMandelFloatingPowerInASM; 
	 DrawFractal = DrawMultiMandelFractalBrute;
}

void SAFPVampireMultiFloatTileWithJulia(void)
{
	 SetHVLineMultiJulia();
	 MultiJuliaInCGeneric=(ULONG(*)(ULONG, double, double))MultiJuliaFloatingPowerInASM; /*MultiJuliaFloatingPowerInC;*/
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiFloatTileNoJulia(void)
{
	 SetHVLineMultiMandel();
	 MultiMandelInCGeneric=(ULONG(*)(ULONG, double, double))MultiMandelFloatingPowerInASM; /*MultiMandelFloatingPowerInC; */
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireMultiFloatBoundaryWithJulia(void)
{
	DrawFractal = DrawFractalBoundaryGeneric;
    FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiJuliaFloatingPowerInASM;	
}

void SAFPVampireMultiFloatBoundaryNoJulia(void)
{
	DrawFractal = DrawFractalBoundaryGeneric;
    FractalIterationGeneric = (ULONG(*)(ULONG, double, double))MultiMandelFloatingPowerInASM;
}

/******* Buddha **********/
/* ----- Classic ------- */
/*
void SAFPClassicBuddhaBruteWithJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBruteNoJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaTileWithJulia(void)
{
 	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaTileNoJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBoundaryWithJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}

void SAFPClassicBuddhaBoundaryNoJulia(void)
{
	 DrawFractal = DrawBuddhaFractalRandomNumbers;
}
*/
/* ------ Vampire ------- */
/*
void SAFPVampireBuddhaBruteWithJulia(void)
{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : */
/*        DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

void SAFPVampireBuddhaBruteNoJulia(void)
{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : */
/*        DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

void SAFPVampireBuddhaTileWithJulia(void)
{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default :*/ 
/*        DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

void SAFPVampireBuddhaTileNoJulia(void)
{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : */
/*        DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

void SAFPVampireBuddhaBoundaryWithJulia(void)
/*{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default : */
/*        DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

void SAFPVampireBuddhaBoundaryNoJulia(void)
{
	/*switch (DD_BPP)
	{
		case 1 : DrawFractal = DrawBuddhaFractalRandomNumbers; break;
		default :*/ 
        /*DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	/*}*/
/*}

*/
/******* Burning Ship **********/
/* ----- Classic ------- */
void SAFPClassicBurningShipBruteWithJulia(void)
{
     BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPClassicBurningShipBruteNoJulia(void)
{
     BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPClassicBurningShipTileWithJulia(void)
{
	 SetHVLineBurningShipJulia();
	 BurningShipInCGeneric=BurningShipInCJulia;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicBurningShipTileNoJulia(void)
{
	 SetHVLineBurningShip();
	 BurningShipInCGeneric=BurningShipInC;
	 DrawFractal = DrawFractalTiling;
}

void SAFPClassicBurningShipBoundaryWithJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInCJulia;
}

void SAFPClassicBurningShipBoundaryNoJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInC;     
}

/* ------ Vampire ------- */

void SAFPVampireBurningShipBruteWithJulia(void)
{
	 SetHVLineBurningShipJulia();
	 BurningShipInCGeneric=(ULONG(*)(ULONG, double, double))BurningShipInASMJulia; 
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPVampireBurningShipBruteNoJulia(void)
{
	 SetHVLineBurningShip();
	 BurningShipInCGeneric=(ULONG(*)(ULONG, double, double))BurningShipInASM; 
	 DrawFractal = DrawBurningShipFractalBrute;
}

void SAFPVampireBurningShipTileWithJulia(void)
{
	 SetHVLineBurningShipJulia();
	 BurningShipInCGeneric=(ULONG(*)(ULONG, double, double))BurningShipInASMJulia; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireBurningShipTileNoJulia(void)
{
	 SetHVLineBurningShip();
	 BurningShipInCGeneric=(ULONG(*)(ULONG, double, double))BurningShipInASM; 
	 DrawFractal = DrawFractalTiling;
}

void SAFPVampireBurningShipBoundaryWithJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInASMJulia;
}

void SAFPVampireBurningShipBoundaryNoJulia(void)
{
     DrawFractal = DrawFractalBoundaryGeneric;
     FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInASM;
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
/*
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
*/

/* ----- Vampire ------- */

/*
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
*/
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
    DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;
	
    /* use C functions */
    switch (BaseFractalType)
	{
		case MANDEL : 	FractalIterationGeneric = MandelInC;
                      	break;
		case MULTIINT : FractalIterationGeneric = MultiMandelInC;
        			    break;
		case MULTIFLOAT : 
        				FractalIterationGeneric = MultiMandelFloatingPowerInC; 
        				break;
		case BURNINGSHIP : 
        				FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInC;
        				break;
	}
}

/* ------ Vampire ------- */
void SAFPVampireBuddha(void)
{
	DrawFractal = DrawTrueColorBuddhaFractalRandomNumbers;

	/* use C functions */
	switch (BaseFractalType)
	{
		case MANDEL : 	FractalIterationGeneric = MandelInC;
                      	break;
		case MULTIINT : FractalIterationGeneric = MultiMandelInC;
        			    break;
		case MULTIFLOAT : 
        				FractalIterationGeneric = MultiMandelFloatingPowerInC; 
        				break;
		case BURNINGSHIP : 
        				FractalIterationGeneric = (ULONG(*)(ULONG, double, double))BurningShipInC;
        				break;
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
    SetUpDirectDrawing(MYILBM.win);
	if (optimized) SAFPVampire();
	else SAFPClassic();	
	SetUpGenericFunctions();
    SetUpPutPixelWithColorModeFunctions();
}

void FirstSAFP(void) 
{
    /* same as SAFP() but without 
     * SetUpDirectDrawing(MYILBM.win); 
     */
     
	if (optimized) SAFPVampire();
	else SAFPClassic();
	SetUpGenericFunctions();
    SetUpPutPixelWithColorModeFunctions();
}

/********************************************************************/
/*********************************************************************/
/*********************************************************************/