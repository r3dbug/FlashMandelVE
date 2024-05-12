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
 *  doesn´t change anything.]
 *  This version is partially compatible with ApolloOS (AROS).
 *
 *  Compilers used: SAS/C 6.58 / VASM 1.9a
 *
 *  Build instruction: smake
 *  (See smakefile for details.)
 *
 */

/*
NOTES:
- optimize Buddha
- all variants of Mandel

KNOWN ISSUES:

Screen modes (opened by OpenScreenTags):
- not 100% reliable (but same with official screen mode utility in Coffin)
- when switching from SAGA => PAL HIRES => NTSC HIRES the picture is TILTED
- bitmap copy from PAL => SAGA 8bit works, but not from PAL => SAGA 24bit
(Improved a bit PAL => SAGA 24bit by using standard bitmap paste function,
gives false colors, but at least the screen looks better than before.
Final fix is a custom routine to convert PAL => 24bit or set correct colors
for AmigaOS draw functions when used with SAGA screens.)

Bitmaps:
- Freeing of AROS HIDD bitmaps is not possible (memory is lost)

*/

/* comment out to compile without test menu */
/* #define TEST_MENU_ON */

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
/*#include <devices/printer.h>*/ /* printing functions disabled */
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

UWORD neww, newh, newd;
static UWORD lastw, lasth, lastd;
UBYTE likelast;
ULONG Accept = NULL;

  /* filter all native graphics modes (vampire: only SAGA modes) */
  /* if (!IsCyberModeID(DisplayID)) return FALSE; */

  DisplayHandle = FindDisplayInfo (DisplayID);

  if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DisplayInfo,sizeof (struct DisplayInfo),DTAG_DISP,DisplayID))
  {
     if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,DisplayID))
     {
        neww = DimensionInfo.Nominal.MaxX;
		newh = DimensionInfo.Nominal.MaxY;
		newd = DimensionInfo.MaxDepth;
		
		/* Cybergraphics BitMapAttrs function doesn't distinguish between 24/32bit screens.
		   In the vampire graphics driver, the 24bit screen is listed first (followed by 32bit screen).
		   Therefore, check if a  24bit screen has already been listed and don't list it
		   when another 24bit screen appears (means: 32bit screen which is listed 2nd won't appear
		   in screen mode requester.
		*/

	    /*likelast=0;*/

		likelast = (lastw == neww) && (lasth == newh) && (lastd == newd);
		
				
		/* the variable Accept affects the screen modes that can be selected */
		/*
		Accept = (ULONG) ((DimensionInfo.MaxDepth <= MAX_DEPTH) && (DisplayInfo.PropertyFlags & DIPF_IS_WB));
		Accept = (ULONG) ((DimensionInfo.MaxDepth > MAX_DEPTH) && (DisplayInfo.PropertyFlags & DIPF_IS_WB));
		Accept = TRUE; /* accept all screenmodes */
    
		/* on classic Amiga (or UAE) show all screen modes */
		
		if (!vampire) likelast=0;
		
		/* likelast=0; // enable all screen modes */
		
	 	Accept = ((!likelast) && (((newd > MIN_DEPTH) && (newd <=8)) || (newd >=24)) && (DisplayInfo.PropertyFlags & DIPF_IS_WB)); /* exclude 15/16 bit and 32 bit screens */
	 
	 	lastw = neww;
		lasth = newh;
		lastd = newd;
	 
	 }
  }
  /*	
	if (Accept) printf("ACCEPT: ");
	else printf("EXCLUDE: ");
	
	printf("DisplayHandle: %p Width: %d Height: %d DimensionInfo.MaxDepth: %d (likelast: %d)\n",
		DisplayHandle, neww, newh, 
		newd, likelast);
  */
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
                              /*ASLSM_InitialDisplayWidth,Win->Width,
                              ASLSM_InitialDisplayHeight,Win->Height,
                              ASLSM_InitialDisplayDepth,Ilbm->wrp->BitMap->Depth,*/
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
	   /* GetDisplayInfoData (DisplayHandle,(UBYTE *) &DisplayInfo,sizeof (struct DisplayInfo),DTAG_DISP,DisplayID); */
	   
	   GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,SMReq->sm_DisplayID);
	   
	   /*
	   printf("DimensionInfo.Nominal.MaxX: %d DimensionInfo.Nominal.MaxY: %d DimensionInfo.MaxDepth: %d SMReq->sm_DisplayDepth: %d DisplayID: %p (= Ilbm.camg)\n", 
	   		DimensionInfo.Nominal.MaxX, DimensionInfo.Nominal.MaxY, DimensionInfo.MaxDepth, SMReq->sm_DisplayDepth, SMReq->sm_DisplayID);
	   */	   
       
	   Ilbm->camg = SMReq->sm_DisplayID;

       Ilbm->Bmhd.w = DimensionInfo.Nominal.MaxX+1; /*SMReq->sm_DisplayWidth;*/

       Ilbm->Bmhd.h = DimensionInfo.Nominal.MaxY+1; /*SMReq->sm_DisplayHeight;*/

	   Ilbm->Bmhd.nPlanes = DimensionInfo.MaxDepth;
	   
       Ilbm->ucliptype = SMReq->sm_OverscanType;

	   /* find out screen depth with LockBitMapTagList() */
	   bmapdepth=0; pixfmt=0;
	   UnLockBitMapTagList(LockBitMapTagList(Ilbm->wrp->BitMap, (struct TagItem*) &query), (struct TagItem*) &query2);
	   
	   /*
	   	printf("CGFX: bmapdepth: %d pixfmt: %d\n", bmapdepth, pixfmt);
	   	printf("SMRequest(): SAGA-Register: Pixelformat: 0xdfe1f5: %p\n", *((UBYTE*)(0xdfe1f5)));
	   	printf("SMRequest(): SAGA-Register: Resolution: 0xdfe1f4: %p\n", *((UBYTE*)(0xdfe1f4)));
   	   */
	   
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
  if (AROSBase = OpenLibrary("aros.library",0L))
  {
	    detected_system=RUNNING_ON_AROS;
		CloseLibrary(AROSBase);  
		printf("Support on AROS / ApolloOS has some problems (e.g. certain memory can not be freed and is lost)\n");
  } else detected_system=NOT_RUNNING_ON_AROS;
}

void CheckVampire(void)
{
   /* autodetection for Vampire */
  if (vampire_type=detectvamp()) {
        vampire=1;
        optimized=1;

		/* Vampire: select BOUNDARY as default */
		SwitchMenuSelection(1,6,algorithm-BRUTE,2);
		algorithm=BOUNDARY;	
		drawing=DRAW_DIRECT;				
		
  } else {
        vampire=0;
        optimized=0;
		
  		/* Classic: select TILE as default */
  		algorithm = TILE;
		drawing=DRAW_CLASSIC;
  }
}

void SetGlobalVariables(void)
{
  BUDDHA_RANDOMIZATION_FREQUENCY=0;
  BUDDHA_USE_SYMMETRY=TRUE;
  BUDDHA_BOUNDARY_RANDOMIZATION=0;
  algorithm_buddha=BUDDHA_RANDOM;
  FractalType = MANDEL;
  BaseFractalType = MANDEL;
  MultiPower = 2;
  histogram_save=1;
  histogram_compression=1;
  
  /* set title string */
  strcpy(title_string, VERSION_TITLE_COMPLETE);
  
  /* set all function pointers */
  SAFP();
}

BOOL DoInitialChecks(void)
{
  if (!OpenCGX()) return 1;
  CheckAROS();
  if (!AllocBuddha()) return 1;
  CheckVampire();
}

void GetIconToolTypesOrSetDefault(LONG Argc, CONST_STRPTR *Argv)
{
	CONST_STRPTR *IconToolTypes;
	if (IconToolTypes = (CONST_STRPTR *) ArgArrayInit (Argc,Argv))
  	{
        MYILBM.Bmhd.w = ArgInt (IconToolTypes,"SCREENWIDTH",DEF_WIDTH);
        MYILBM.Bmhd.h = ArgInt (IconToolTypes,"SCREENHEIGHT",DEF_HEIGHT);
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

        ArgArrayDone ();

        MYILBM.Bmhd.w = MIN (MAX_WIDTH,MAX (MYILBM.Bmhd.w,DEF_WIDTH));

        MYILBM.Bmhd.h = MIN (MAX_HEIGHT,MAX (MYILBM.Bmhd.h,DEF_HEIGHT));

        MYILBM.Bmhd.nPlanes = MIN (MAX_DEPTH,MAX (MYILBM.Bmhd.nPlanes,MIN_DEPTH));

        MYFONTSTRUCT.ta_YSize = MIN (MAX_FONTSIZE,MAX (MYFONTSTRUCT.ta_YSize,MIN_FONTSIZE));

        PRIORITY = MIN (5,MAX (PRIORITY,-5));
	}

    else
    {
        MYILBM.camg = DEF_MONITOR;

		MYILBM.Bmhd.w = (vampire) ? VAMPIRE_WIDTH : DEF_WIDTH;

        MYILBM.Bmhd.h = (vampire) ? VAMPIRE_HEIGHT : DEF_HEIGHT;

		/* maximum colors */
        if (vampire) MYILBM.Bmhd.nPlanes = VAMPIRE_DEPTH;
		else MYILBM.Bmhd.nPlanes = (CheckGFX () ? MAX_DEPTH : DEF_DEPTH); /* 24 */
     	
		/* printf("CheckGFX (1 = AGA, 2 = RTG, 3 = RTG && AGA): %d\n", CheckGFX()); */
		/* MAX_COLORS when MakeDisplay(&ILBM) is called with nPlanes = 24 is 256 (=/= TRUECOLOR) */
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
	 /* PALETTE = (CheckGFX () ? COLORS_AGA : COLORS_ECS); */
	 if (vampire) PALETTE = COLORS_AGA;
	 else PALETTE = (CheckGFX () ? COLORS_AGA : COLORS_ECS); /* not sure if that maintains compatibility with classic machines */
	 
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
  
  
  if (!DoInitialChecks()) return 1;  
  SetGlobalVariables();
  
  GetIconToolTypesOrSetDefault(Argc, Argv);
  
  if (OpenScreenFails())
  {
     Fail (OpenDisplayError,20L);
	 goto End;
  }
  
  SetPalette();
  SetMYILBM();

/* enable symmetry for first run for speed */
  EnableSymmetry();
     
  
  if (!PrepareMainMenuWindowAndPointer()) 
  {
  	Fail (MakeDisplayError,20L);
  	goto End;
  }
  
  
  do
  {
     while (HandleEvents (&MYILBM) & NEWDISPLAY_MSG)
     {
                   
        /* store source screen format */
        if (IsAClassicScreen(MYILBM.camg))
        {
           	DD_SRC_BPP=0; /* set DD_SRC_BPP to 0 for PAL / NTSC screens in order to select */
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
					  
            if (CURRENT_MAX_COLORS)
            {
                CURRENT_MAX_COLORS -= RESERVED_PENS;

				/* SetUpDirectDrawing(MYILBM.win); */
			    SAFP(); /* change grayscale to truecolor buddha */
				SetMenuSelection();
						 
				SetRast(MYILBM.win->RPort,0);
						 
				/* store destination screen format */
				DD_DST_BPP=DD_BPP; 			/* GetBitMapBPP(MYILBM.win->RPort->BitMap); */
				DD_DST_PIXFMT=DD_PIXFMT; 	/* GetBitMapPixFMT(MYILBM.win->RPort->BitMap); */
						 
				/* set bitmap copy function */
				switch (DD_SRC_BPP)
				{
					case 0 : PasteBitMap = PasteBitMapStandard; break;	/* DD_SRC_BPP == 0 means PAL / NTSC screen */					
                    case 1 : PasteBitMap = (DD_DST_BPP==1) ? PasteBitMapStandard : PasteBitMapCLUT2RGB; break;
					default : PasteBitMap = (DD_DST_BPP==1) ? PasteBitMapRGB2CLUT : PasteBitMapStandard; break;					
				}
						 
				if (TMASK & MASK) ShowTitle (MYILBM.scr,FALSE);
						
				PasteBitMap (MYBITMAP,MYILBM.win,(WORD) GetBitMapAttr (MYBITMAP,BMA_WIDTH),(WORD) GetBitMapAttr (MYBITMAP,BMA_HEIGHT));
					     
				if (TMASK & MASK) ShowTitle (MYILBM.scr,TRUE);
						 
				PasteBitMap = PasteBitMapStandard;
						 
                if (Choice (MYILBM.win,"Rendering requester","Screen propreties are changed.\nImage and colors can be inaccurate.\n\nRecalculate ?"))
                {
                   	SetMenuStop (MYILBM.win);

                   	PutPointer (MYILBM.win,0,0,0,0,0,BUSY_POINTER);

					ELAPSEDTIME = DrawFractal (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);

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

               break; /*????*/
            }
        }
     }
     if (RETURNVALUE >= 20L) break;

  } while (! Choice (MYILBM.win,"Exit requester","Are you sure ?"));

  if (BMASK & MASK) FreeBitMapSafety (MYBITMAP);

  Execute (ASSIGNREMOVE,NULL,NULL);

  Fade (MYILBM.win,PALETTE,50L,1L,TOBLACK);

  CloseDisplay (&MYILBM,VINFO);

  FreeMenus (MAINMENU);
   
End:
  /* free buffers / close libraries / exit */
  if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer); /* background copy for orbits */
  FreeBuddha();
  if (CyberGfxBase) CloseLibrary(CyberGfxBase);
  exit (RETURNVALUE);
}

  /***********************************************************************************************************/

