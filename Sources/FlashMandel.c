/*******************************************************************************
**																			   *
**  ©1996-2001: Version 2.0 [LAST MODIFIED 20-January-2001]                    *
**  Dino Papararo, Via Manzoni 184, 80123 Napoli (Italia)                      *
**                                                                             *
**  ©2023-2024:	Vamped Edition                                                 *
**  RedBug aka Marcel Maci, Rue des Augustins 9, 1700 Fribourg (Switzerland)   *             
**                                                                             *
********************************************************************************/

/* 
 *  This is a "hybrid version". 
 *  It should run, at least partially, on classic Amigas (FPU required) or
 *  on UAE. Vampire card - if present - is autodetected.
 *  Certain fractals (not all) have optimized assembly routines for Vampire
 *  cards. In this case it is possible to switch forth and back between 
 *  classic and optimized algorithm using the additional menu or the keybord: 
 *  Amiga+( and Amiga+). [If no optimized routine is available this setting
 *  has no effect.]
 *  This version is partially compatible with ApolloOS (AROS).
 *
 *  Compilers used: SAS/C 6.58 / VASM 1.9f (2.0beta)
 *
 *  Build instruction: smake
 *
 *  (For a complete new build of all files, use make cleanall before smake,
 *  see smakefile for details.)
 *
 */

/*
Ideas / to do's:
- all variants of MultiMandel (<=1)
- rotating zoom
- Newton fractal
- direct formulas for MultiInt 2,3,4,5
- inner coloring (periodicity checking)
- histogram halving (?) 
- linear histogram (or other)
- New tooltypes: APOLLOCORE=10000,OPTIMIZED=CLASSIC, NOHISTOGRAM, NOBUDDHA etc.
- quick (= one key) screenshot save function (screenshot0001.iff etc.)
- Networking (distributed calculation with nodes)
- LCD screen support (Ultimate Expansion Board)
- default aspect ratios (4:3, 16:10, 16:9 etc.)

Animations:
- "Fly in": automated zoom (automated saving of calculated frames)
- "Voyage": define a path (several points, with Bezier curves) in Julia set an "travel"
through the curve (with possibility to visualize path in an smaller window)

KNOWN ISSUES:

Screenmodes (opened via ASLRequester, OpenScreenTags):
- not 100% reliable (but same with official screenmode utility in Coffin) 
=> might be ASLRequester that is not 100% reliable ?!
- when switching from SAGA => PAL HIRES => NTSC HIRES the picture is TILTED
- bitmap copy from PAL => SAGA 8bit works, but not from PAL => SAGA 24bit
(Improved a bit PAL => SAGA 24bit by using standard bitmap paste function,
gives false colors, but at least the screen looks better than before.
Final fix is a custom routine to convert PAL => 24bit or set correct colors
for AmigaOS draw functions when used with SAGA screens.)

Bitmaps:
- Freeing of AROS HIDD bitmaps is not possible (memory is lost)

Load/Save:
- When opening a Nebulabrot and trying to save it to the same file it 
doesn´t work (all other fractal types aren´t affected, saving Nebulabrot
several times to the same file works, as long as the same file wasn´t 
loaded before).

- Maxtime parameter to interrupt Nebulabrot isn´t precise.

- Many includes fail during compilation (according to Snoopdos - probably
wrong path, there is always a correct include found, so the whole thing 
compiles ...)

- Filerequester may have a semaphore problem (?!, has been impossible
to verify reliably so far).

- Precision when zooming into Nebulabrots diminuishes.

- FlashMandel still looses 104 bytes sometimes (even when the program is
just opened and closed, but it doesn´t happen always and might be related
to not 100% reliable AVAIL on AmigaOS due to multitasking ?!).

- Potential illegal memory access in StoreIterationsCoreASM() (impossible
to verify so far).

- Keyboard support is limited (e.g. parameters for Multibrot  can´t be 
entered via keyboard)
 
- Version number might different in About Requester and Title bar
if no smake cleanall is done beforehand.

- Screenmode filtering for 32bit screens might only work on Vampire 
(untested, DisplayInfo doesn´t seem to report MaxDepth higher than
24 bits (?!)).

- Symmetry (= flipped bitmap copy) cannot be used (reliably) in combination 
with system-friendly draw routines (= bitmap copy does a direct screenbuffer
access).
*/

typedef char* CONST_STRPTR;
extern struct Library* IntuitionBase;
extern struct Library* DOSBase;
struct Library* AROSBase;
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <exec/exec.h>
#include <clib/alib_protos.h>
#include <clib/cybergraphics_protos.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <graphics/gfxbase.h>
#include <graphics/scale.h>
#include <workbench/workbench.h>
#include <iffp/ilbmapp.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/diskfont.h>
#include <proto/gadtools.h>
#include <proto/utility.h>
#include <proto/iffparse.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <proto/wb.h>
#include <pragmas/cybergraphics_pragmas.h>
#include <proto/cybergraphics.h>
#include <cybergraphics/cybergraphics.h>

#include "fmglobals.h"
#include "ifferr.h"
#include "drawing.h"
#include "histogram.h"
#include "rle.h"
#include "CompilerSpecific.h"
#include "Modules/ColorWindow.h"
#include "Modules/iff.h"
#include "fmevents.h"
#include "fmcgx.h"
#include "shared.h"
#include "classic.h"
#include "vampire.h"
#include "symmetry.h"
#include "splash.h"
#include "menu.h"

static ULONG HOOKCALL SMFilterFunc (REG (a0,struct Hook *),REG (a2,struct ScreenModeRequester *),REG (a1,ULONG));

struct Screen *OpenIdScreen (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
struct Window *OpenDisplay  (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
struct BitMap *CopyBitMap   (struct Window *,WORD,WORD,WORD,WORD);

struct Hook         SMFILTERHOOK      = {NULL,NULL,(VOID *) SMFilterFunc,NULL};

void BlinkRect (struct Window *Win,const LONG LeftEdge,const LONG TopEdge,const LONG RightEdge,const LONG BottomEdge)
{
UBYTE Blink = BLINKTIMES;

  DisplayBeep (Win->WScreen);

  ZOOMLINE [8] = ZOOMLINE [0] = ZOOMLINE [6] = LeftEdge;

  ZOOMLINE [1] = ZOOMLINE [3] = ZOOMLINE [9] = TopEdge;

  ZOOMLINE [2] = ZOOMLINE [4] = RightEdge;

  ZOOMLINE [5] = ZOOMLINE [7] = BottomEdge;

  while (Blink--)
  {
     DrawBorder (Win->RPort,&MYBORDER,0,0);

     Delay (8L);

     DrawBorder (Win->RPort,&MYBORDER,0,0);

     Delay (8L);
  }
}

static ULONG HOOKCALL SMFilterFunc (REG (a0,struct Hook *Hook),REG (a2,struct ScreenModeRequester *SMReq),REG (a1,ULONG DisplayID))
{
DisplayInfoHandle DisplayHandle;

struct DisplayInfo DisplayInfo;

struct DimensionInfo DimensionInfo;

UWORD neww, newh, newd, newdv;
static UWORD lastw, lasth, lastd;
UBYTE likelast, eliminate;
ULONG Accept = NULL;

  DisplayHandle = FindDisplayInfo (DisplayID);

  if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DisplayInfo,sizeof (struct DisplayInfo),DTAG_DISP,DisplayID))
  {
     if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,DisplayID))
     {
        neww = DimensionInfo.Nominal.MaxX;
		newh = DimensionInfo.Nominal.MaxY;
		newdv = newd = DimensionInfo.MaxDepth;
		
		/* Cybergraphics BitMapAttrs function doesn't distinguish between 24/32bit screens.
		   In the vampire graphics driver, the 24bit screen is listed first (followed by 32bit screen).
		   Therefore, check if a  24bit screen has already been listed and don't list it
		   when another 24bit screen appears (means: 32bit screen which is listed 2nd won't appear
		   in screen mode requester.
		*/
		
        likelast=0;
		if (vampire)
        {
        	likelast = (lastw == neww) && (lasth == newh) && (lastd == newd);
        }
        
        if (likelast) newdv=32;
        
   		eliminate = ((FM_NO32BITMODES) & ((newd==32) || (newdv==32)));
             
	 	lastw = neww;
		lasth = newh;
		lastd = newd;
	 
		/* the variable Accept affects the screen modes that can be selected */
     	Accept = TRUE;

        if ((newd==15) || (newd==16)) Accept=FALSE; /* FM doesn´t support 15/16 bit modes */
         
        if (eliminate) Accept=FALSE;
         
     	if ((FM_NOPALNTSCMODES) && (!IsCyberModeID(DisplayID))) Accept=FALSE;
         
        if ((FM_NO24BITMODES) && (newd==24) && (newdv==24)) Accept=FALSE;

        if ((FM_NO8BITMODES) && (newd==8) && (IsCyberModeID(DisplayID))) Accept=FALSE;
     }
  }
  
  return Accept;
}

BOOL SMRequest (struct ILBMInfo *Ilbm)
{
  /* taken from filter function */
  DisplayInfoHandle DisplayHandle;
  struct DimensionInfo DimensionInfo;
  struct ScreenModeRequester *SMReq;
  struct Window *Win = Ilbm->win;
  BOOL NewScreen = FALSE;

  if (SMReq = AllocAslRequest (ASL_ScreenModeRequest,NULL))
  {
    if (AslRequestTags (SMReq,ASLSM_Window,Win,
                              ASLSM_SleepWindow,TRUE,
                              ASLSM_TitleText,"ScreenMode requester",
                              ASLSM_InitialLeftEdge,Win->LeftEdge + 25,
                              ASLSM_InitialTopEdge,Win->TopEdge + 35,
                              ASLSM_InitialWidth,310,
                              ASLSM_InitialHeight,((Win->Height) * 7) >> 3,
                              ASLSM_InitialDisplayID,GetVPModeID (Ilbm->vp),
                              /*
                              ASLSM_InitialDisplayWidth,Win->Width,
                              ASLSM_InitialDisplayHeight,Win->Height,
                              ASLSM_InitialDisplayDepth,Ilbm->wrp->BitMap->Depth, */
							  /*ASLSM_InitialOverscanType,Ilbm->ucliptype,*/
                              /*ASLSM_InitialInfoOpened,TRUE,*/
                              ASLSM_InitialInfoLeftEdge,Win->LeftEdge + 310 + 15,
                              ASLSM_InitialInfoTopEdge,Win->TopEdge,
                              /*ASLSM_DoWidth,TRUE,
                              ASLSM_DoHeight,TRUE,
                              ASLSM_DoDepth,TRUE,
                              ASLSM_DoOverscanType,TRUE,
                              ASLSM_MinDepth,MIN_DEPTH,
                              ASLSM_MaxDepth,MAX_DEPTH,*/
							  ASLSM_PropertyFlags,DIPF_IS_WB,
                              ASLSM_PropertyMask,DIPF_IS_DUALPF|DIPF_IS_PF2PRI|DIPF_IS_HAM|DIPF_IS_EXTRAHALFBRITE,
                              ASLSM_FilterFunc,&SMFILTERHOOK,
                              /*ASLSM_MinWidth,MIN_WIDTH,
                              ASLSM_MinHeight,MIN_HEIGHT,*/
							  TAG_END))
    {
       /* taken from filter function => get exact display info */
	   DisplayHandle = FindDisplayInfo (SMReq->sm_DisplayID);
	   
	   GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,SMReq->sm_DisplayID);
	   
	   Ilbm->camg = SMReq->sm_DisplayID;

       Ilbm->Bmhd.w = DimensionInfo.Nominal.MaxX+1;

       Ilbm->Bmhd.h = DimensionInfo.Nominal.MaxY+1;

	   Ilbm->Bmhd.nPlanes = DimensionInfo.MaxDepth;
	   
       Ilbm->ucliptype = SMReq->sm_OverscanType;

	   /* get screen depth via CGFX */
	   bmapdepth=0; pixfmt=0;
	   UnLockBitMapTagList(LockBitMapTagList(Ilbm->wrp->BitMap, (struct TagItem*) &query), (struct TagItem*) &query2);
	   
       /* disactivate smooth coloring when 8bit screen is opened */
       if (SMOOTH_COLORING)
       {
       		if (bmapdepth==8) 
            {
            	SMOOTH_COLORING=0;  
       	
        		if (algorithm_before_smooth_coloring!=0xffffffff)
                {
        			BAILOUT_VALUE=bailout_before_smooth_coloring;
            		algorithm = algorithm_before_smooth_coloring;
                    algorithm_before_smooth_coloring = 0xffffffff; 
                    FM_SMOOTH_COLORING_BAILOUT_1ST=TRUE;                         
                }              	
            }
       }
       
       NewScreen = TRUE;
    }

    FreeAslRequest (SMReq);
  }

  return NewScreen;
}

BOOL OpenCGX(void)
{
  if (!(CyberGfxBase = OpenLibrary("cybergraphics.library",0L)))
  {
	printf("Impossible to open cybergraphics library\n");
	return FALSE;
  } else return TRUE;
}

void CheckAROS(void)
{
  /* the following is the beginning of a deal with the devil ... 
   * but AROS / ApolloOS has some quirks and incompatibilities,
   * so detected_system can be used to work around those ... 
   */
  detected_system=NOT_RUNNING_ON_AROS;
  AROSBase = NULL;
  FM_ALLOCTEMPRAST_FLAGS=BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES;
  
  if (AROSBase = OpenLibrary("aros.library",0L))
  {
    detected_system=RUNNING_ON_AROS;
	CloseLibrary(AROSBase);  
  } 
  else 
  {
  	detected_system=NOT_RUNNING_ON_AROS;
  }
}

void SystemSpecificMessageAndValues(void)
{
  if (detected_system==RUNNING_ON_AROS)
  {
  	/* AROS doesn't hide title bare reliably before calculation
       => creates corrupted areas when direct drawing is used
     * => define a delay as workaround
     */
    /* TempBM allocated with AllocTempRast() in FCGX.c 
     * cannot be freed on AROS.
       => See MMASK & MASK, followed by FreeBitMap().
       => Fall back to non-interleaved bitmaps on AROS / ApolloOS (these can be created and freed without any problem)
     */	
    FM_ALLOCTEMPRAST_FLAGS=BMF_CLEAR | BMF_MINPLANES; /* no BMF_INTERLEAVED */
    TITLE_HIDE_DELAY=DEF_AROS_TITLE_HIDE_DELAY;   
  }
  else
  {
  	/* BMF_INTERLEAVED works on AmigaOS */
    FM_ALLOCTEMPRAST_FLAGS=BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES;
    /* no delay necessary for AmigaOS */ 
  	TITLE_HIDE_DELAY=0;  
  }
}

UBYTE DetectArne(void) {
	UBYTE value;
	UBYTE* potgor=(UBYTE*)0xdff016;
	value=(UBYTE)*potgor;
	if (value & 2) return 1; /* ARNE */
	else return 0; 			 /* Paula */	
}

BOOL isV2(UBYTE type)
{
    switch(type)
    {
    	case 1: /* v600 */
        case 2: /* v500 */
        case 6: /* v1200 */
        	return TRUE;
            break;
        default :
        	return FALSE;
    }
}

void CheckVampire(void)
{
  UBYTE mpl;
  
  /* autodetection for Vampire */
  vampire_type=detectvamp();
 
  /* problem: sometimes (on uae) detectvamp() returns >0 */
  /* => do further tests to be sure it is a Vampire */
  /* (detectvamp() corrected - this problem might be solved now) */
  switch (vampire_type)
  {
  		/* V2 */	
        case 1: /* V600 */
        case 2: /* V500 */
        case 6: /* V1200 */
  			/* what further check could be done here? */
            mpl=GetClockMultiplyer();
           
            if ((mpl<11) || (mpl>14)) vampire_type=0; /* not 100% bullet proof ... but better than nothing ;) */	
            break;  
    	/* V4 */    
        default :
        	/* all V4 have ARNE => search for that */
  			if (!DetectArne()) vampire_type=0; /* not a Vampire if there is no Arne (greetings;) */          
  }
 
  if (vampire_type) 
  {
        vampire=1;
        optimized=1;
		
        /* Vampire: select BOUNDARY as default */
		algorithm=BOUNDARY;	
		drawing=DRAW_DIRECT;				
		
  } else {
        vampire=0;
        optimized=0;
		
  		/* Classic: select TILE as default */
        algorithm=BOUNDARY;
        /* for classic: use direct drawing but hide mouse pointer */
        drawing=DRAW_DIRECT; 
        /* hide mouse pointer during calculation */
        FM_BUSY_MOUSE_POINTER = NO_POINTER; 
  }
}

void SetGlobalVariables(void)
{
  USE_SYMMETRY=TRUE;
  algorithm_buddha=1;
  FractalType = MANDEL;
  BaseFractalType = MANDEL;
  MultiPower = 2;
  histogram_save=1;
  histogram_compression=1;
  
  /* set title string */
  strcpy(title_string, VERSION_TITLE_COMPLETE);
  
  /* set all function pointers */
  FirstSAFP();  
}

void DoInitialChecks(void)
{
  if (!OpenCGX()) printf("Unable to open CGX\n");
  CheckAROS();
  CheckVampire();
}

void GetIconToolTypesOrSetDefault(LONG Argc, CONST_STRPTR *Argv)
{
	CONST_STRPTR *IconToolTypes;
	if (IconToolTypes = (CONST_STRPTR *) ArgArrayInit (Argc,Argv))
  	{
        DD_WIDTH = MYILBM.Bmhd.w = ArgInt (IconToolTypes,"SCREENWIDTH",DEF_WIDTH);
        DD_HEIGHT = MYILBM.Bmhd.h = ArgInt (IconToolTypes,"SCREENHEIGHT",DEF_HEIGHT);
        MYILBM.Bmhd.nPlanes = ArgInt (IconToolTypes,"SCREENDEPTH",MAX_DEPTH);

        sscanf (ArgString (IconToolTypes,"SCREENMODE",DEF_MONITORSTR),"%lx",&MYILBM.camg);

        RMIN = strtod (ArgString (IconToolTypes,"REALMIN",INIT_DEF_RMINSTR),NULL);
        RMAX = strtod (ArgString (IconToolTypes,"REALMAX",INIT_DEF_RMAXSTR),NULL);
        IMIN = strtod (ArgString (IconToolTypes,"IMAGMIN",INIT_DEF_IMINSTR),NULL);
        IMAX = strtod (ArgString (IconToolTypes,"IMAGMAX",INIT_DEF_IMAXSTR),NULL);

        JULIA = (BOOL) ArgInt (IconToolTypes,"STARTWITHJULIA",FALSE);
     
        JKRE = strtod (ArgString (IconToolTypes,"JULIACONSTREAL",INIT_DEF_JKRESTR),NULL);
        JKIM = strtod (ArgString (IconToolTypes,"JULIACONSTIMAG",INIT_DEF_JKIMSTR),NULL);

        MYFONTSTRUCT.ta_Name = strncpy (MYFONT,ArgString (IconToolTypes,"FONTNAME",DEF_FONTNAMESTR),MAX_FILELEN);
        
        MYFONTSTRUCT.ta_YSize = ArgInt (IconToolTypes,"FONTSIZE",DEF_FONTSIZE);

        PRIORITY = ArgInt (IconToolTypes,"STARTPRI",DEF_STARTPRI);
        
		/* override automatic detection of AmigaOS vs AROS/ApolloOS */		
        if ( 
        	strcmp( ArgString (IconToolTypes,"SYSTEM",""), "AROS"
            	  )
                  == NULL
           ) detected_system=RUNNING_ON_AROS;
        else if (
        		 strcmp( ArgString (IconToolTypes,"SYSTEM",""), "APOLLOOS"
            	       )
                  	   == NULL
           		) detected_system=RUNNING_ON_AROS;
        else if (
        		 strcmp( ArgString (IconToolTypes,"SYSTEM",""), "AMIGAOS"
            	  	   )
                  	   == NULL
           		) detected_system=RUNNING_ON_AMIGAOS;
        
        ArgArrayDone ();

        MYILBM.Bmhd.w = MIN (MAX_WIDTH,MAX (MYILBM.Bmhd.w,DEF_WIDTH));

        MYILBM.Bmhd.h = MIN (MAX_HEIGHT,MAX (MYILBM.Bmhd.h,DEF_HEIGHT));

        MYILBM.Bmhd.nPlanes = MIN (MAX_DEPTH,MAX (MYILBM.Bmhd.nPlanes,MIN_DEPTH));

        MYFONTSTRUCT.ta_YSize = MIN (MAX_FONTSIZE,MAX (MYFONTSTRUCT.ta_YSize,MIN_FONTSIZE));

        PRIORITY = MIN (5,MAX (PRIORITY,-5));
	}

    else
    {
        if (isV2(vampire_type))
        {
            MYILBM.camg = DEF_MONITOR_VAMPIRE;

			MYILBM.Bmhd.w = V2_WIDTH;

        	MYILBM.Bmhd.h = V2_HEIGHT;
            
        	MYILBM.Bmhd.nPlanes = V2_DEPTH;
        }
        else 
        {
        	MYILBM.camg = (vampire) ? DEF_MONITOR_VAMPIRE : DEF_MONITOR;

			MYILBM.Bmhd.w = (vampire) ? VAMPIRE_WIDTH : DEF_WIDTH;

        	MYILBM.Bmhd.h = (vampire) ? VAMPIRE_HEIGHT : DEF_HEIGHT;

        	if (vampire) MYILBM.Bmhd.nPlanes = VAMPIRE_DEPTH;
			else MYILBM.Bmhd.nPlanes = (CheckGFX () ? MAX_DEPTH : DEF_DEPTH);
		}	
    }
}

BOOL OpenScreenFails(void)
{
	if (ModeNotAvailable (MYILBM.camg))
       MYILBM.camg = ModeFallBack (MYILBM.camg,MYILBM.Bmhd.w,MYILBM.Bmhd.h,MYILBM.Bmhd.nPlanes);

    if (MYILBM.Bmhd.nPlanes > GetMaxPlanes (MYILBM.camg)) MYILBM.Bmhd.nPlanes = GetMaxPlanes (MYILBM.camg);

	if (MYILBM.Bmhd.nPlanes < MIN_DEPTH) return TRUE; /* fails */
    else return FALSE; /* success */
    
}

void SetPalette(void) 
{
	 if (vampire) PALETTE = COLORS_AGA;
	 else PALETTE = COLORS_AGA; 
	 
     if ((RMIN >= RMAX) || (IMIN >= IMAX))
     {
       DEF_RMIN = INIT_DEF_RMIN;

       DEF_RMAX = INIT_DEF_RMAX;

       DEF_IMIN = INIT_DEF_IMIN;

       DEF_IMAX = INIT_DEF_IMAX;
     }

     else
     {
       DEF_RMIN = RMIN;

       DEF_RMAX = RMAX;

       DEF_IMIN = IMIN;

       DEF_IMAX = IMAX;
     }
}

void SetMYILBM(void)
{
    MYILBM.ParseInfo.propchks = IlbmProps;

    MYILBM.ParseInfo.collectchks = IlbmCollects;

    MYILBM.ParseInfo.stopchks = IlbmStops;

    MYILBM.Bmhd.pageWidth  = 0;

    MYILBM.Bmhd.pageHeight = 0;

    MYILBM.stype = CUSTOMSCREEN | SCREENQUIET;

    MYILBM.TBState = TMASK & MASK;

    MYILBM.ucliptype = OSCAN_TEXT;

    MYILBM.EHB = FALSE;

    MYILBM.Autoscroll = TRUE;

    MYILBM.IFFPFlags = NULL;
}

/**************************************************************************************************************/

LONG main (LONG Argc,CONST_STRPTR *Argv)
{

  if (LaunchedFromShell()) Execute(ASSIGNPARENTDIR,NULL,NULL);
  else Execute (ASSIGNCURRENTDIR,NULL,NULL);
  
  DoInitialChecks();
  
  /* make exec save new e-regs in addition to classic ones */
  if (vampire) SetVampireTaskSwitching();
   	
  /* V2 line may have some problems with optimized FPU code 
   *  => set optimized to false by default 
   */
  if (isV2(vampire_type)) optimized=FALSE;
  
  InitBuddha();
  
  SetGlobalVariables();
  
  GetIconToolTypesOrSetDefault(Argc, Argv);
  
  SystemSpecificMessageAndValues();
  
  if (OpenScreenFails())
  {
     Fail (OpenDisplayError,20L);
	 goto End;
  }
  
  SetPalette();
  SetMYILBM();
  
  if (!PrepareMainMenuWindowAndPointer()) 
  {
  
  	Fail (MakeDisplayError,20L);
  	goto End;
  }
   
  SetMenuSelection();
  
  do
  {
     while (HandleEvents (&MYILBM) & NEWDISPLAY_MSG)
     {
        /* store source screen format */
        if ((IsAClassicScreen(MYILBM.camg)) || (!vampire))
        {
           	/* set DD_SRC_BPP to 0 for PAL / NTSC screens */
            DD_SRC_BPP=0; 
        	drawing=DRAW_CLASSIC;
        }
        else
        {			   
           	DD_SRC_BPP=GetBitMapBPP(MYILBM.win->RPort->BitMap);
			DD_SRC_PIXFMT=GetBitMapPixFMT(MYILBM.win->RPort->BitMap);
			SetUpPixFMTDeltas(DD_SRC_PIXFMT);
			DD_SRC_RED=DD_RED;
			DD_SRC_GREEN=DD_GREEN;
			DD_SRC_BLUE=DD_BLUE;
		}
        
		/* open new screen */
		if (SMRequest (&MYILBM))
        {
			
            if (ZMASK & MASK)
            {
               DrawBorder (MYILBM.wrp,&MYBORDER,0,0);

               MASK ^= ZMASK;
            }

            MYBITMAP = CopyBitMap (MYILBM.win,MYILBM.win->LeftEdge,MYILBM.win->TopEdge,MYILBM.win->Width,MYILBM.win->Height);

            Fade (MYILBM.win,PALETTE,25L,1L,TOBLACK);
							
            CloseDisplay (&MYILBM,VINFO);

            MYILBM.Bmhd.pageWidth  = 0;

            MYILBM.Bmhd.pageHeight = 0;

            CURRENT_MAX_COLORS = MakeDisplay (&MYILBM);
            
            /* store source screen format */
        	if ((IsAClassicScreen(MYILBM.camg)) || (!vampire))
        	{
        	   	/* set DD_SRC_BPP to 0 for PAL / NTSC screens */
        	    DD_SRC_BPP=0; 
        		drawing=DRAW_CLASSIC;
                FM_NEWSCR_CPY=FM_NEWSCR_RECALC;
        	}
        	else
        	{			   
        	   	DD_SRC_BPP=GetBitMapBPP(MYILBM.win->RPort->BitMap);
				DD_SRC_PIXFMT=GetBitMapPixFMT(MYILBM.win->RPort->BitMap);
				SetUpPixFMTDeltas(DD_SRC_PIXFMT);
				DD_SRC_RED=DD_RED;
				DD_SRC_GREEN=DD_GREEN;
				DD_SRC_BLUE=DD_BLUE;
			}         
					  
            if (CURRENT_MAX_COLORS)
            {
                CURRENT_MAX_COLORS -= RESERVED_PENS;

			    SAFP();
				SetMenuSelection();
						 
    			/* select recalculate or bitmap copy */			
                if ((FM_NEWSCR_CPY==FM_NEWSCR_BMCPY) || (FM_NEWSCR_CPY==NULL))
                {
    	         	/* use bitmap copy */   
                    /* store destination screen format */
					DD_DST_BPP=DD_BPP; 			
					DD_DST_PIXFMT=DD_PIXFMT; 	
						 
					/* set bitmap copy function */
					switch (DD_SRC_BPP)
					{
                    	/* DD_SRC_BPP == 0 means PAL / NTSC screen */
						case 0 : PasteBitMap = PasteBitMapStandard; break;						
    	                case 1 : PasteBitMap = (DD_DST_BPP==1) ? PasteBitMapStandard : PasteBitMapCLUT2RGB; break;
						default : PasteBitMap = (DD_DST_BPP==1) ? PasteBitMapRGB2CLUT : PasteBitMapStandard;				
					}
						 
					if (TMASK & MASK) ShowTitle (MYILBM.scr,FALSE);
							
					PasteBitMap (MYBITMAP,MYILBM.win,(WORD) GetBitMapAttr (MYBITMAP,BMA_WIDTH),(WORD) GetBitMapAttr (MYBITMAP,BMA_HEIGHT));
					     
					if (TMASK & MASK) ShowTitle (MYILBM.scr,TRUE);
						 
					PasteBitMap = PasteBitMapStandard;
				
                	if (FM_NEWSCR_CPY==NULL)
                    {	
                    	if (Choice (MYILBM.win,"Rendering requester","Screen propreties are changed.\nImage and colors can be inaccurate\nusing bitmap copy function.\n\nRecalculate ?"))
    					{
                            /* use recalculate */				 
                   			SetMenuStop (MYILBM.win);

                   			PutPointer (MYILBM.win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
                            
                    		ELAPSEDTIME = DrawFractalWrapper (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);
                    
                    		PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);
								
                    		SetMenuStart (MYILBM.win);
                            
                    		ShowTime (MYILBM.win,"Rendering elapsed time:",ELAPSEDTIME);
                        }		
                	}
                }
                else 			
                {	
                    /* use recalculate instead of bitmap copy */				 
                   	SetMenuStop (MYILBM.win);

                   	PutPointer (MYILBM.win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
          
          			if (
        				(USE_SYMMETRY) 
                        &&
                        (RMIN==INIT_DEF_RMIN)
                        &&
                        (RMAX==INIT_DEF_RMAX)
                        &&
                        (IMIN==INIT_DEF_IMIN)
                        &&
                        (IMAX==INIT_DEF_IMAX)
                        &&
                        (!JULIA)
                        && 
                        (FractalType!=BURNINGSHIP)
                        && 
                        (FractalType!=BUDDHA)
                       )
                          EnableSymmetry();

                    ELAPSEDTIME = DrawFractalWrapper (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);
                    
                    PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);
							
                    SetMenuStart (MYILBM.win);

                    ShowTime (MYILBM.win,"Rendering elapsed time:",ELAPSEDTIME);
 				}                     	 
	   			bmapdepth=0; pixfmt=0;
	   			UnLockBitMapTagList(LockBitMapTagList(MYILBM.wrp->BitMap, (struct TagItem*) &query), (struct TagItem*) &query2);
	   
				/* disable resume for BUDDHA */
				MenuResumeOff(MYILBM.win);
			}
            else
            {
                Fail (MakeDisplayError,20L);
            }
        }
     }
     if (RETURNVALUE >= 20L) break;

  } while (! Choice (MYILBM.win,"Exit requester","Are you sure ?"));

  if (BMASK & MASK) FreeBitMapSafety (MYBITMAP);
 
  /* disable fading */
  /* Fade (MYILBM.win,PALETTE,50L,1L,TOBLACK); */
  ClearMenuStrip(MYILBM.win);

  FreeMenus (MAINMENU);

  CloseDisplay (&MYILBM,VINFO);

End:
  /* free buffers / close libraries / exit */
  if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer); /* background copy for orbits */

  DeallocateBoundary();

  if (CyberGfxBase) CloseLibrary(CyberGfxBase);
  
  FreeBuddha();

  /* NOTE:
   * ASSIGNREMOVE may be a problem if several instances of FM are running.
   * (Instance A removes ASSIGN, then instance B tries to access FLASHMANDEL:).
   */
  Execute (ASSIGNREMOVE,NULL,NULL); 

  exit (RETURNVALUE);
}

  /***********************************************************************************************************/

