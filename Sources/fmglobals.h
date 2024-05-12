
#ifndef FMGLOBALS_H
#define FMGLOBALS_H

/* (1) proto/graphics.h conflicts with (2) iffp/ilbm.h when defining GfxBase
 * (1) uses GfxBase*
 * (2) uses Library*
 * => use the following include that replace the original PROTO_GRAPHICS_H
 */

#include "fmprotographics.h"

/* END OF FIX FOR CONFLICTING GfxBase */

#include "version.h"

#define FPU_68K 1L << 1

#ifndef AFF_68060
#define AFF_68060 1L << 7
#endif

#define XSTR(s)	STR(s)
#define STR(s)	#s

#define AUTHOR   " "
#define COPYRIGHT_DATE "©1996-2001 (2024)\n\n[Everything beyond 256 colors is experimental ... ;] "
#define ADDRESS  "Dino Papararo (original version): dino.papararo at gmail dot com\n"
#define EMAIL    "RedBug (Vampire optimizations): marcel.maci at gmx dot ch\n"

#ifdef  __SASC
#define DATE     __AMIGADATE__
#else
#define DATE     __DATE__
#endif

#define DEF_WIDTH        640
#define DEF_HEIGHT       480
#define DEF_DEPTH        4

#define VAMPIRE_WIDTH	1280		/* 1280 */
#define VAMPIRE_HEIGHT	720		/* 720 */
#define VAMPIRE_DEPTH	8       /* default color depth for Vampire: 8 = like classic / 24 = true color */

#define DEF_MONITOR      0x50011000		/* uae & p96 */
#define DEF_MONITORSTR  "0x50011000"

#define DEF_FONTNAMESTR 	"topaz.font\0"
#define DEF_FONTEMPTYSPC 	"                    "    /* leave enough space (total: min. 30 characters) for longer font names ! */

#define WMASK  0x1
#define FMASK  0x2
#define VMASK  0x4
#define SMASK  0x8
#define BMASK  0x10
#define ZMASK  0x20
#define TMASK  0x40
#define MMASK  0x80
#define LMASK  0x100

#define VERSION_TITLE 			"FlashMandel Vamped Edition BurningShip"
#define VERSION_YEAR 			24
#define VERSION_MONTH			5
#define VERSION_DAY				11				/* see numbering convention in version.h */

#define VERSION_AUTHOR1     	"Dino Papararo"
#define VERSION_MAIL1			"dino.papararo@gmail.com"
#define VERSION_DATE1			"1996-2001"
#define VERSION_AUTHOR2			"RedBug"
#define VERSION_MAIL2			"m.maci@gmx.ch"
#define VERSION_DATE2			"2023-2024"
#define COPYRIGHT_SYMBOL		"©"

#define COPYRIGHT_STRING1		COPYRIGHT_SYMBOL VERSION_DATE1 ": " VERSION_AUTHOR1
#define COPYRIGHT_STRING2		COPYRIGHT_SYMBOL VERSION_DATE2 ": " VERSION_AUTHOR2

#define CONTACT_TITLE			"Contact:"
#define CONTACT_STRING1			VERSION_MAIL1
#define CONTACT_STRING2			VERSION_MAIL2

#define COPYRIGHT_STRING_ALL	COPYRIGHT_STRING1 "\n" COPYRIGHT_STRING2

#define CONTACT_STRING_ALL		CONTACT_TITLE "\n" CONTACT_STRING1 "\n" CONTACT_STRING2

#define VERSION_TITLE_COMPLETE	VERSION_TITLE " (Version: " XSTR(VERSION_YEAR) "." XSTR(VERSION_MONTH) " R" XSTR(VERSION_DAY) ")"

#define COPYRIGHT_CHUNK_STRING	VERSION_TITLE_COMPLETE "\n\n" COPYRIGHT_STRING_ALL "\n\n" CONTACT_STRING_ALL

#define VERSION_COMMENT			"Everything beyond 256 colors\nhas to be considered\nexperimental ... ;]"

#define Lib_Version 39L

#define DEF_FONTNAMESTR 	"topaz.font\0"
#define DEF_FONTEMPTYSPC 	"                    "    /* leave enough space (total: min. 30 characters) for longer font names ! */

#define MIN_WIDTH  320
#define MIN_HEIGHT 200
#define MAX_WIDTH  16368
#define MAX_HEIGHT 16384

#define MIN_DEPTH  3
#define MAX_DEPTH  32 /* 24*/ /*32*/ /*8*/		/* 32bit doesn't work ?! (not returned as depth>24 from SMRequester) */

#define MAX_FONTSIZE 24
#define MIN_FONTSIZE 8

#define SHIFTRIGHT   FALSE
#define SHIFTLEFT    TRUE

#define INITIALZOOM  18

#define CLEAR_POINTER 0
#define BUSY_POINTER  1
#define ZOOM_POINTER  2

#define ZPW          15
#define ZPH          15
#define ZPXO         -8
#define ZPYO         -7

#define MAXCHARS     20

#define ACCEPT       (1)
#define RATIO        (2)
#define RESET        (3)
#define CANCEL       (4)
#define KEEP         (5)

#define MAX_FILELEN 30
#define MAX_DIRLEN  230
#define MAX_PATHLEN 260
#define BARLEN      80

#define MIN_ITERATIONS 31

#define RAW_ESC      0x045
#define VAN_ESC      0x01B
#define TAB          0x042
#define HELP         0x05f

#define BLACK      (0)
#define LIGHT_GREY (2)
#define WHITE      (1)
#define DARK_GREY  (3)

#define POINTS       4
#define PAIRS        5

#define BLINKTIMES   5L
#define ONESEC        (50L * 1L)
#define TWOSECS       (50L * 2L) /* 2 secondi */

#define DEF_FONTSIZE     8
#define DEF_STARTPRI    -3

#define SPREAD_BIT   (1L << 16)
#define REPEATED_BIT (1L << 17)
#define TRUE_BIT     (1L << 18)
#define INTEGER_BIT  (1L << 19)
#define TITLE_BIT    (1L << 20)
#define REAL_BIT     (1L << 21)
#define PPC_BIT      (1L << 22)
#define JULIA_BIT    (1L << 23)

#define LOADPICTURE_MSG 0x1
#define SAVEPICTURE_MSG 0x2
#define DUMP_MSG        0x4
#define REDRAW_MSG      0x8
#define UNDO_MSG        0x10
#define DRAW_MSG        0x20
#define PREVIEW_MSG     0x40
#define NEWDISPLAY_MSG  0x80
#define STOP_MSG        0x100
#define EXIT_MSG        0x200
#define COORDS_MSG      0x400
#define ITER_MSG        0x800
#define ABOUT_MSG       0x1000
#define TITLE_MSG       0x2000
#define TIME_MSG        0x4000
#define PALETTE_MSG     0x8000
#define CYCLERIGHT_MSG  0x10000
#define CYCLELEFT_MSG   0x20000
#define DELAY_MSG       0x40000
#define COLOR_MSG       0x80000
#define LOADPALETTE_MSG 0x100000
#define SAVEPALETTE_MSG 0x200000
#define FONTREQ_MSG     0x400000
#define SHOWGUIDE_MSG   0x800000
#define SWITCH_CPU      0x1000000           /* switch between Vampire and classic */
#define SPLASH_MSG      0x2000000			/* message to show splash screen */

#define PICTURES_DRAWER 0
#define PALETTES_DRAWER 1

#define IDCMP_STANDARD  IDCMP_CLOSEWINDOW|IDCMP_RAWKEY|IDCMP_MOUSEBUTTONS|IDCMP_MOUSEMOVE|IDCMP_MENUPICK

/* added WFLG_RMBTRAP */
#define WFLG_STANDARD /*WFLG_RMBTRAP|*/WFLG_ACTIVATE|WFLG_BACKDROP|WFLG_NOCAREREFRESH|WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS

#define MODE_ID_MASK (LACE|HIRES|HAM|EXTRA_HALFBRITE)

#define ID_MAND MAKE_ID ('M','A','N','D')
#define ID_NEWMAND MAKE_ID ('F', 'M', 'V', 'E')   /* use this for both NEW_SPECIAL_CHUNK (8bit) and NEWRGB_SPECIAL_CHUNK (24bit) */

#define WMASK  0x1
#define FMASK  0x2
#define VMASK  0x4
#define SMASK  0x8
#define BMASK  0x10
#define ZMASK  0x20
#define TMASK  0x40
#define MMASK  0x80
#define LMASK  0x100

#define NOT_RUNNING_ON_AROS 0
#define RUNNING_ON_AROS 	1

#define TOBLACK   1
#define FROMBLACK 0

#define RESERVED_PENS 4L

#define MANDEL		1
#define MULTIINT	2
#define MULTIFLOAT  3
#define MULTINEG    4	/* not yet implemented */

#define BUDDHA      5  	/* this is not a base type fractal */
#define BURNINGSHIP 6  	/* this is a base type fractal */
#define NEWTON		7	/* not yet implemented */

#define INIT_DEF_RMIN      -2.0
#define INIT_DEF_RMAX      +2.0
#define INIT_DEF_IMIN      -1.5
#define INIT_DEF_IMAX      +1.5
#define INIT_DEF_RMINSTR   "-2.0"
#define INIT_DEF_RMAXSTR   "+2.0"
#define INIT_DEF_IMINSTR   "-1.5"
#define INIT_DEF_IMAXSTR   "+1.5"
#define INIT_DEF_JKRE      -0.12
#define INIT_DEF_JKIM      +0.75
#define INIT_DEF_JKRESTR   "-0.72"
#define INIT_DEF_JKIMSTR   "-0.26"

#define MINLIMIT 2

#define SHOWGUIDECOMMAND "Run >NIL: MultiView FlashMandel:Docs/FlashMandel.guide"
#define ASSIGNCURRENTDIR "Assign FLASHMANDEL: \"\""
#define ASSIGNREMOVE     "Assign FLASHMANDEL: remove"

#define MAX_NEBULA_WIDTH	1920
#define MAX_NEBULA_HEIGHT	1080

#define BRUTE 				0
#define TILE  				1
#define BOUNDARY 			2
#define BUDDHA_RANDOM	    4
#define BUDDHA_SWITCH		5
#define BUDDHA_BOUNDARY		6

#define TRUECOLOR_IMAGE		1 << 31

#define SMOOTH_RIC	1			/* renormalized iteration count */
#define SMOOTH_FMNG	2			/* FlashMandelNG */

#include <exec/types.h>
#include <iffp/ilbmapp.h>
//#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <graphics/scale.h>
#include <intuition/gadgetclass.h>
#include <math.h>
#include <devices/printer.h>
#include <stdio.h>
/*#include <ctype.h>*/
#include <string.h>

#include "compilerspecific.h"
#include "menu.h"

#define MAXITERPERIODICITY 10000

extern TEXT VERSION_STRING [];

/* Vampire detection */
IMPORT UWORD ASMCALL detectvamp(void);

/* FlashMandel functions shared between Classic Amigas and  Vampire */

IMPORT double ASMCALL powFPU (REG (fp0,double),REG (fp1,double)); 
IMPORT double ASMCALL sinFPU (REG (fp0,double)); 
IMPORT double ASMCALL cosFPU (REG (fp0,double)); 
IMPORT void ASMCALL sincosFPU (REG (fp0,double), REG(fp1,double)); 
IMPORT double ASMCALL atan2FPU (REG (fp0,double),REG (fp1,double)); 
IMPORT double ASMCALL atan2andpowFPU (REG (fp0,double),REG (fp1,double), REG(fp2,double), REG(fp3,double)); 
IMPORT double ASMCALL arctanFPU (REG (fp0,double)); 

/* parallelized mandelbrot functions */
void __asm par2iterateasm(register __fp0 double x1,
                    register __fp1 double y1,
                    register __fp2 double x2,
                    register __fp3 double y2,
                    register __d1 UWORD MaxIter);

void __asm par4iterateasm(register __fp0 double x1,
                    register __fp1 double y1,
                    register __fp2 double x2,
                    register __fp3 double y2,
                    register __fp4 double x3,
                    register __fp5 double y3,
                    register __fp6 double x4,
                    register __fp7 double y4,
                    register __d1 unsigned long MaxIter
                   );

/* parallelized julia functions */
void __asm juliapar4iterateasmhoriz(register __fp0 double x1,
                    register __fp1 double y1,
                    register __fp2 double x2,
                    register __fp3 double x3,
                    register __fp4 double x4,
                    register __fp6 double cx,
                    register __fp7 double cy,
                    register __d1 unsigned long MaxIter
                   );

void __asm juliapar4iterateasmvert(register __fp0 double x1,
                    register __fp1 double y1,
                    register __fp2 double y2,
                    register __fp3 double y3,
                    register __fp4 double y4,
                    register __fp6 double cx,
                    register __fp7 double cy,
                    register __d1 unsigned long MaxIter
                   );

IMPORT UWORD ASMCALL Mandel68k_FPU_Classic( REG(d0,UWORD), REG(fp0,double), REG(fp1,double));
IMPORT UWORD ASMCALL Mandel68k_FPU_Vampire( REG(d0,UWORD), REG(fp0,double), REG(fp1,double));
IMPORT UWORD ASMCALL Julia68k_FPU_Classic( REG(d0,UWORD), REG(fp0,double), REG(fp1,double), REG(fp6,double), REG(fp7,double));
IMPORT UWORD ASMCALL Julia68k_FPU_Vampire( REG(d0,UWORD), REG(fp0,double), REG(fp1,double), REG(fp6,double), REG(fp7,double));

extern VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

extern VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

UWORD InterruptDrawing(struct Window*, const LONG,const LONG,const LONG,const LONG);
void FunctionIsOnly24Bit(struct Window*);
void OrbitsNotPossible(struct Window *);
void SetDefaultCoordinates(void);
void PickJuliaK (struct Window *);
void OrbitsActivated(struct Window *);
void SaveCoords (struct Window *,BOOL);
void RestoreCoords (struct Window *);
ULONG IntegerGad (struct Window *,TEXT *,TEXT *,TEXT *,ULONG);
double FloatGad (struct Window *,TEXT *,TEXT *,TEXT *,double);
BOOL CheckBox (struct RastPort *,const LONG,const LONG,const LONG,const LONG);
ULONG Fail (UBYTE *,ULONG);
void SetRGBIterations(void);
/*LONG WinDump (struct Window *);*/
void PutPointer (struct Window *,UWORD *,LONG,LONG,LONG,LONG,UBYTE);
VOID ShowTime (struct Window *,TEXT *,ULONG);
UWORD CheckRMB(void);
UWORD CheckLMB(void);

extern struct TextAttr     MYFONTSTRUCT;
extern struct NewGadget    TEXTGAD;
extern struct NewGadget    BUTTONGAD;  
extern struct NewGadget    CHECKBOXGAD;

/* Common global variables (from boundary) */
/*UWORD* Data; */
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
extern struct Border MYBORDER;
extern WORD ZOOMLINE[];
extern ULONG resx, resy;
extern UBYTE* screenbuffer;

extern TEXT BAR_STRING [];
extern UBYTE *DiskFontLibraryError;
extern UBYTE *IconLibraryError;
extern UBYTE *GadToolsLibraryError;
extern UBYTE *AslLibraryError;
extern UBYTE *UtilityLibraryError;
extern UBYTE *IFFParseLibraryError;
extern UBYTE *OpenDisplayError;
extern UBYTE *NoMemForTempRastPort;
extern UBYTE *FontError;
extern UBYTE *NoMonitor;
extern UBYTE *NoChips;
extern UBYTE *NoMem;
extern UBYTE *NoChipMem;
extern UBYTE *PubNotUnique;
extern UBYTE *UnknownMode;
extern UBYTE *ScreenTooDeep;
extern UBYTE *AttachScreen;
extern UBYTE *ModeNotAvailableError;
extern UBYTE *UnknownScrError;
extern UBYTE *VisualInfoError;
extern UBYTE *WindowError;
extern UBYTE *MenuError;
extern UBYTE *GadgetError;
extern UBYTE *WindowGadgetError;
extern UBYTE *PreviewWindowError;
extern UBYTE *CreateDisplayError;
extern UBYTE *DimensionInfoError;
extern UBYTE *DisplayInfoError;
extern UBYTE *PaletteRequesterError;
extern UBYTE *MakeDisplayError;
extern UBYTE *QueryMandPicError;
extern UBYTE *LoadMandPicError;
extern UBYTE *SaveMandPicError;
extern UBYTE *LoadMandPalError;
extern UBYTE *SaveMandPalError;

extern BOOL DEPTH_CHANGED;
extern ULONG CURRENT_MAX_COLORS;
extern ULONG* PALETTE;
extern UWORD ITERATIONS;
extern WORD MAX_ITERATIONS,  BUDDHA_MIN_ITERATIONS, RETURNVALUE;
extern double RMIN,RMAX,IMIN,IMAX;
extern CPTR *VINFO;

extern double DEF_RMIN,DEF_RMAX,DEF_IMIN,DEF_IMAX,DEF_JKRE,DEF_JKIM,URMIN,URMAX,UIMIN,UIMAX,UJKRE,UJKIM;
extern volatile double px1,py1,px2,py2,px3,py3,px4,py4;

extern double *re;
extern double *im;

extern ULONG MultiPower;
extern double MultiPowerFloat;

extern struct ILBMInfo MYILBM;

extern volatile double FACTOR,INCREMREAL,INCREMIMAG,CRE,CIM,JKRE,JKIM;

extern ULONG MultiPower;
extern double MultiPowerFloat;

extern UBYTE DrawOrbits;
extern UBYTE PeriodicityColoring;

extern UWORD BaseFractalType;
extern UWORD FractalType;
extern double GlobalPowerVar;

/* variables for optimized routines */
extern volatile UWORD IterP1;
extern volatile UWORD IterP2;
extern volatile UWORD IterP3;
extern volatile UWORD IterP4;
extern double FinalXP1, FinalXP2, FinalXP3, FinalXP4;
extern double FinalYP1, FinalYP2, FinalYP3, FinalYP4;
extern double GlobalP;
extern ULONG detected_system;
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

extern UBYTE vampire_type;
extern UBYTE optimized;
extern UBYTE vampire;
extern UBYTE algorithm, algorithm_buddha;

extern ULONG FM_REDITER, FM_GREENITER, FM_BLUEITER;
extern ULONG FM_REDMIN, FM_GREENMIN, FM_BLUEMIN;

extern UBYTE SHOW_MAXCOUNTERS;

extern ULONG (*MultiMandelInCGeneric)(ULONG, double, double);
extern ULONG (*MultiJuliaInCGeneric)(ULONG,double,double);
extern ULONG (*BurningShipInCGeneric)(ULONG,double,double);
extern ULONG (*FractalInCGeneric)(ULONG,double,double);
extern ULONG (*FractalInCGenericStoreIterations)(ULONG,double,double);
extern void (*PutPixelWithColorMode)(ULONG, ULONG, ULONG);
extern BOOL (*PasteBitMap) (struct BitMap *,struct Window *, WORD, WORD);

extern struct Menu* MAINMENU;
extern UBYTE drawing;
extern UBYTE switch_back_to_direct;

extern ULONG histogram_save;
extern ULONG histogram_compression; 		/* 0 = raw / 1 = RLE */

extern ULONG ScrCpySize;
extern struct BitMap* ScrCpyBuffer;

extern double BAILOUT_VALUE;
extern LONG PRIORITY;
extern struct Task *THISTASK;

extern struct EasyStruct NotYetImplemented;
extern CHIP UWORD ZoomPointer[];
extern ULONG ELAPSEDTIME,CURRENT_MAX_COLORS;
extern LONG DELAY,__oslibversion;

extern LONG IlbmProps [];
extern LONG NewIlbmProps [];
extern LONG NewRGBIlbmProps [];
extern struct BitMap *MYBITMAP;

/* legacy MandelChunk definition */
struct MandelChunk { WORD LeftEdge,TopEdge,Width,Height;
                     DOUBLE RMin,RMax,IMin,IMax;
                     DOUBLE RConst,IConst;
                     ULONG Iterations;
                     ULONG Special;
                   };

extern struct MandelChunk MANDChunk;

/* NewMandelChunk for Vamped Edition */
struct NewMandelChunk { /* legacy elements (for compatibility) */
						WORD LeftEdge,TopEdge,Width,Height;
                     	DOUBLE RMin,RMax,IMin,IMax;
                     	DOUBLE RConst,IConst;
                     	ULONG Iterations;
                     	ULONG Special;
                   	  	/* new elements */
					  	UWORD Version;
						UWORD FractalType;
						ULONG Flags;
						/* from here on: fractal calculation data 
						   not all fields are used to reserve space for future extensions
						*/
						ULONG Algorithm[4]; /* 0 = algorithm (BRUTE, TILING, BOUNDARY), 1 = algorithm1 (RANDOM, SWITCH, BOUNDARY/BORDER */
						/* boolean fields */
						BOOL Switch[6]; /* 0 = Julia, 1 = ANTIBUDDHA */
						
						/* integer data (ULONG) */
						ULONG IntData[20]; /* FM__REDITER, FM_REDMIN ... */
					  	
						/* float data */
						double FloatData[20];
					  };

/* new MandelRGBChunk (with color table for 24bit images)  */
struct NewRGBMandelChunk { /* legacy elements (for compatibility) */
						WORD LeftEdge,TopEdge,Width,Height;
                     	DOUBLE RMin,RMax,IMin,IMax;
                     	DOUBLE RConst,IConst;
                     	ULONG Iterations;
                     	ULONG Special;
                   	  	/* new elements */
					  	UWORD Version;
						UWORD FractalType;
						ULONG Flags;
						/* from here on: fractal calculation data 
						   not all fields are used to reserve space for future extensions
						*/
						ULONG Algorithm[4]; /* 0 = algorithm (BRUTE, TILING, BOUNDARY), 1 = algorithm1 (RANDOM, SWITCH, BOUNDARY/BORDER */
						/* boolean fields */
						BOOL Switch[6]; /* 0 = Julia, 1 = ANTIBUDDHA */
						
						/* integer data (ULONG) */
						ULONG IntData[20]; /* FM__REDITER, FM_REDMIN ... */
					  	
						/* float data */
						double FloatData[20];
						/* color table */
						UBYTE Colors[3*256+2]; /* for 256 colors maximum, 3 bytes (8bits) RGB for each color, 2 bytes at beginning: 0: number of colors, 1: reserved */
					  };

extern struct MandelChunk MANDChunk,*MANDChunkTmp;
extern struct NewMandelChunk NewMANDChunk,*NewMANDChunkTmp;
extern struct NewRGBMandelChunk NewRGBMANDChunk,*NewRGBMANDChunkTmp;

extern LONG IlbmCollects [];

extern LONG IlbmStops [];

extern struct Chunk COPYRIGHT_CHUNK;

extern struct Chunk SPECIAL_CHUNK;

extern struct Chunk NEW_SPECIAL_CHUNK;

extern struct Chunk NEWRGB_SPECIAL_CHUNK;

extern UWORD PENS [];

extern TEXT MYFILE [],PICTURESDIR [],PALETTESDIR [];
extern TEXT MYPATH [];
extern TEXT *MYDIR,MYFONT [];

extern ULONG COLORS_ECS[];
extern ULONG COLORS_AGA[];

extern struct TextFont *NEWFONT;
extern struct BitMap *TempBM;
extern UBYTE title_string[];

#endif /* FMGLOBALS_H*/