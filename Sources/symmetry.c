
#include "fmglobals.h"
#include "symmetry.h"

ULONG (*OriginalDrawFractal) (struct Window *,const LONG,const LONG,const LONG,const LONG);

void DisableSymmetry(void)
{
	DrawFractal = OriginalDrawFractal;
/*
	printf("Symmetry disabled: INCREMREAL: %1.2f INCREMIMAG: %1.2f\n", INCREMREAL, INCREMIMAG);
	printf("RMIN: %1.2f RMAX: %1.2f IMIN: %1.2f IMAX: %1.2f\n", RMIN, RMAX, IMIN, IMAX); 
*/	
}

ULONG DrawFractalUsingSymmetry(struct Window *Win, const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
static double CpyIMIN, CpyIMAX;
ULONG NewB2;
struct BitMap *bm;
ULONG y1, y2, stop;
double di;

	if (vampire) SetRast(Win->RPort,1);
	
	CurrentTime (&StartSec,&Dummy);
	
	CpyIMIN = IMIN;
	CpyIMAX = IMAX;
	

	/* calculate di for middle line */
 	/* INCREMIMAG =*/ di = (fabs (IMAX-IMIN)) / Win->Height;

	/* calculate new b1, b2 */
	NewB2=b2 / 2 + 1;

	/* adapt IMIN / IMAX */	
	IMIN = -di; /* -0.00625; //(IMIN + IMAX) / 2.0; */
	
	OriginalDrawFractal(Win, a1, b1, a2, NewB2);	
	
	/* now blit other half of the screen in inverse order */
	if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);
	y2=Win->Height-1;
	stop=NewB2;
	bm=Win->RPort->BitMap;
	for (y1=1; y1<=stop; y1++)
	{
		BltBitMap(bm, 0, y1, bm, 0, y2, Win->Width, 1, 0xc0, 0xff, NULL);
		y2--;
	}
	
	CurrentTime (&EndSec,&Dummy);
	
	/* restore original values */
	IMIN=CpyIMIN;
	IMAX=CpyIMAX;
	
	if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);
	
	DisableSymmetry();
	
	return (EndSec-StartSec);
}

void EnableSymmetry(void) 
{
	OriginalDrawFractal = DrawFractal;
	DrawFractal =  DrawFractalUsingSymmetry; /*OriginalDrawFractal;*/
}
