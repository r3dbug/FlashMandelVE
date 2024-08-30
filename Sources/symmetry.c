
#include "fmglobals.h"
#include "symmetry.h"

ULONG (*OriginalDrawFractal) (struct Window *,const LONG,const LONG,const LONG,const LONG);

void DisableSymmetry(void)
{
	DrawFractal = OriginalDrawFractal;
}

double DrawFractalUsingSymmetry(struct Window *Win, const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;
static double CpyIMIN, CpyIMAX;
ULONG NewB2;
struct BitMap *bm;
ULONG y1, y2, stop;
double di;

	CurrentTime (&StartSec,&Dummy);
	if (optimized) SetStart();
    
	CpyIMIN = IMIN;
	CpyIMAX = IMAX;
	

	/* calculate di for middle line */
 	di = (fabs (IMAX-IMIN)) / Win->Height;

	/* calculate new b1, b2 */
	NewB2=b2 / 2 + 1;

	/* adapt IMIN / IMAX */	
	IMIN = -di; 
	
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
	
    if (optimized) SetStop();
	CurrentTime (&EndSec,&Dummy);
	
	/* restore original values */
	IMIN=CpyIMIN;
	IMAX=CpyIMAX;
	
	if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);
	
	DisableSymmetry();
	
	if ((optimized) && ((EndSec-StartSec)<45)) return GetTime();
    else return (double)(EndSec-StartSec);
}

void EnableSymmetry(void) 
{
    if (
    	(USE_SYMMETRY) && 
        
            (!	
              (
            	/* forbid symmetry for system-friendly drawing when selected
                   (symmetry uses direct buffer access which might created
                   wrong results)
                 */	
                (FM_FORBID_SYMMETRY)
                &&
                (drawing==DRAW_CLASSIC)
              )
            )
       )
    {	
    	OriginalDrawFractal = DrawFractal;
		DrawFractal = DrawFractalUsingSymmetry;
	}
    else
    {
    	OriginalDrawFractal = DrawFractal;
    }
}
