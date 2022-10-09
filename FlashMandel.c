/*******************************************************************************
**
**  Coded by Dino Papararo        LAST MODIFIED 20-January-2001
**
**  Vampire optimizations 
**  by RedBug                     LAST MODIFIED September 2022
**
*******************************************************************************/

/* 
 *  This is a "hybrid version" - it should run on classic Amigas.
 *  Vampire card - if present - is autodetected.
 *  It is possible to switch forth and back between classic and optimized
 *  algorithm using the additional menu or the keybord: Amiga+( and Amiga+).
 */

#ifndef FLASHMANDEL_RB
#define FLASHMANDEL_RB
#endif

typedef char* CONST_STRPTR;
extern struct Library* IntuitionBase;
extern struct Library* DOSBase;

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <exec/exec.h>
#include <clib/alib_protos.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <graphics/gfxbase.h>
#include <graphics/scale.h>
#include <devices/printer.h>
#include <workbench/workbench.h>
#include <iffp/ilbmapp.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/diskfont.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <proto/utility.h>
#include <proto/iffparse.h>
#include <proto/icon.h>
#include <proto/asl.h>
#include <proto/wb.h>

#include "CompilerSpecific.h"

#define Lib_Version 39L

#define FPU_68K 1L << 1

#ifndef AFF_68060
#define AFF_68060 1L << 7
#endif

#define VERSION  "FlashMandel 2.0 Vamped Edition "
#define AUTHOR   "Dino Papararo "
#define COPYRIGHT_DATE "©1996-2001 (2022)"
#define ADDRESS  "Via Manzoni, 184\n  80123 Napoli\n  Italia"
#define EMAIL    "E-Mail address:\n  DinoP@IName.Com\n\nVampire optimizations by RedBug (2022)"

#ifdef  __SASC
#define DATE     __AMIGADATE__
#else
#define DATE     __DATE__
#endif

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

#define DEF_WIDTH        640
#define DEF_HEIGHT       480
#define DEF_DEPTH        4
#define DEF_MONITOR      0x50011000
#define DEF_MONITORSTR  "0x50011000"
#define DEF_FONTNAMESTR "topaz.font\0"
#define DEF_FONTSIZE     8
#define DEF_STARTPRI    -3

#define MIN_WIDTH  640
#define MIN_HEIGHT 480
#define MAX_WIDTH  16368
#define MAX_HEIGHT 16384
#define MIN_DEPTH  3
#define MAX_DEPTH  8

#define MAX_FONTSIZE 24
#define MIN_FONTSIZE 8

#define MAX_FILELEN 30
#define MAX_DIRLEN  230
#define MAX_PATHLEN 260
#define BARLEN      80

#define MIN_ITERATIONS 31

#define RESERVED_PENS 4L

#define TOBLACK   1
#define FROMBLACK 0

#define RAW_ESC      0x045
#define VAN_ESC      0x01B
#define TAB          0x042
#define HELP         0x05f

#define MINLIMIT 2

#define BLACK      (0)
#define LIGHT_GREY (2)
#define WHITE      (1)
#define DARK_GREY  (3)

#define POINTS       4
#define PAIRS        5

#define BLINKTIMES   5L

#define ONESEC        (50L * 1L)
#define TWOSECS       (50L * 2L) /* 2 secondi */

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

#define WMASK  0x1
#define FMASK  0x2
#define VMASK  0x4
#define SMASK  0x8
#define BMASK  0x10
#define ZMASK  0x20
#define TMASK  0x40
#define MMASK  0x80
#define LMASK  0x100

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

#define PICTURES_DRAWER 0
#define PALETTES_DRAWER 1

#define IDCMP_STANDARD  IDCMP_CLOSEWINDOW|IDCMP_RAWKEY|IDCMP_MOUSEBUTTONS|IDCMP_MOUSEMOVE|IDCMP_MENUPICK

#define WFLG_STANDARD WFLG_ACTIVATE|WFLG_BACKDROP|WFLG_NOCAREREFRESH|WFLG_SMART_REFRESH|WFLG_BORDERLESS|WFLG_REPORTMOUSE|WFLG_NEWLOOKMENUS

#define MODE_ID_MASK (LACE|HIRES|HAM|EXTRA_HALFBRITE)

#define ID_MAND MAKE_ID ('M','A','N','D')

#define SHOWGUIDECOMMAND "Run >NIL: SYS:Utilities/MultiView FlashMandel:Docs/FlashMandel.guide"
#define ASSIGNCURRENTDIR "Assign FLASHMANDEL: \"\""
#define ASSIGNREMOVE     "Assign FLASHMANDEL: remove"

/* variables for optimized routines */
UWORD IterP1;
UWORD IterP2;
UWORD IterP3;
UWORD IterP4;
UWORD modulo;
double px1,py1,px2,py2,px3,py3,px4,py4;

struct MandelChunk { WORD LeftEdge,TopEdge,Width,Height;
                     DOUBLE RMin,RMax,IMin,IMax;
                     DOUBLE RConst,IConst;
                     ULONG Iterations;
                     ULONG Special;
                   };

struct MandelChunk MANDChunk = {0},*MANDChunkTmp = NULL;

struct RastPort TempRP;

struct BitMap *TempBM;

UBYTE *PixelLine;

TEXT VERSION_STRING [] = "\0$VER: " VERSION " " COPYRIGHT_DATE " by " AUTHOR " compiled in " DATE "\0";

TEXT MYFILE [MAX_FILELEN],PICTURESDIR [MAX_DIRLEN],PALETTESDIR [MAX_DIRLEN],MYPATH [MAX_PATHLEN],BAR_STRING [BARLEN],*MYDIR = NULL,MYFONT [MAX_FILELEN];

BOOL JULIA = FALSE,DEPTH_CHANGED;

WORD ZOOMLINE [PAIRS << 1] , MAX_ITERATIONS = 319,RETURNVALUE = 0;

UWORD ITERATIONS;

LONG PRIORITY = DEF_STARTPRI,DELAY = 5L,__oslibversion = Lib_Version;

ULONG MASK = TMASK;

ULONG ELAPSEDTIME = NULL,CURRENT_MAX_COLORS;

double RMIN=INIT_DEF_RMIN,RMAX=INIT_DEF_RMAX,IMIN=INIT_DEF_IMIN,IMAX=INIT_DEF_IMAX;

double DEF_RMIN,DEF_RMAX,DEF_IMIN,DEF_IMAX,DEF_JKRE=INIT_DEF_JKRE,DEF_JKIM=INIT_DEF_JKIM,URMIN=0.0,URMAX=0.0,UIMIN=0.0,UIMAX=0.0,UJKRE=0.0,UJKIM=0.0;

double FACTOR=0.0,INCREMREAL=0.0,INCREMIMAG=0.0,CRE=0.0,CIM=0.0,JKRE=0.0,JKIM=0.0;

ULONG *PALETTE;

CPTR *VINFO = NULL;

VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

/* pointers to accelerated / classic functions */
VOID (*MVLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*MHLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*JVLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*JHLine) (struct RastPort *,const LONG,const LONG,const LONG);
/* variable for vampire detection and selection between vampire and classic */
UBYTE optimized;
UBYTE vampire;

UBYTE *DiskFontLibraryError   =  "I need at least DiskFontLibrary v39 or better, sorry !";
UBYTE *IconLibraryError       =  "I need at least IconLibrary v39 or better, sorry !";
UBYTE *GadToolsLibraryError   =  "I need at least GadToolsibrary v39 or better, sorry !";
UBYTE *AslLibraryError        =  "I need at least AslLibrary v39 or better, sorry !";
UBYTE *UtilityLibraryError    =  "I need at least UtilityLibrary v39 or better, sorry !";
UBYTE *IFFParseLibraryError   =  "I need at least IFFParseLibrary v39 or better, sorry !";
UBYTE *OpenDisplayError       =  "I need at least 8 colors, sorry.";
UBYTE *NoMemForTempRastPort   =  "No mem for temp RastPort";
UBYTE *FontError              =  "Font failed";
UBYTE *NoMonitor              =  "No monitor";
UBYTE *NoChips                =  "No chips";
UBYTE *NoMem                  =  "No mem";
UBYTE *NoChipMem              =  "No chipmem";
UBYTE *PubNotUnique           =  "Pub not unique";
UBYTE *UnknownMode            =  "Unknown mode";
UBYTE *ScreenTooDeep          =  "Screen too deep";
UBYTE *AttachScreen           =  "Attach screen failed";
UBYTE *ModeNotAvailableError  =  "Mode not available";
UBYTE *UnknownScrError        =  "Unknown screen error";
UBYTE *VisualInfoError        =  "Visualinfo failed";
UBYTE *WindowError            =  "Openwindow failed";
UBYTE *MenuError              =  "Menu failed";
UBYTE *GadgetError            =  "Gadget error";
UBYTE *WindowGadgetError      =  "Window gadget error";
UBYTE *PreviewWindowError     =  "Preview window failed";
UBYTE *CreateDisplayError     =  "Createdisplay error";
UBYTE *DimensionInfoError     =  "Dimensioninfo error";
UBYTE *DisplayInfoError       =  "Displayinfo error";
UBYTE *PaletteRequesterError  =  "Palette requester error";
UBYTE *MakeDisplayError       =  "Display error";
UBYTE *QueryMandPicError      =  "Not a FlashMandel picture !";
UBYTE *LoadMandPicError       =  "Load picture error";
UBYTE *SaveMandPicError       =  "Save picture error";
UBYTE *LoadMandPalError       =  "Load palette error";
UBYTE *SaveMandPalError       =  "Save palette error";

void   FreeBitMapSafety (struct BitMap *);
BOOL   AllocTempRast (UWORD,UBYTE);
void   PutPointer    (struct Window *,UWORD *,LONG,LONG,LONG,LONG,UBYTE);
VOID   ClipIt        (WORD,WORD,struct Rectangle *,struct Rectangle *,struct Rectangle *,struct Rectangle *,struct Rectangle *,struct Rectangle *,BOOL);
ULONG  ModeFallBack  (ULONG,WORD,WORD,WORD);
LONG   About         (struct Window *);
LONG   Choice        (struct Window *,TEXT *,TEXT *);
LONG   CheckGFX      (VOID);
UBYTE  GetMaxPlanes  (ULONG);
void   AdjustRatio   (double *,double *,double *,double *,WORD,WORD,BOOL);
VOID   ShowTime      (struct Window *,TEXT *,ULONG);
ULONG  IntegerGad    (struct Window *,TEXT *,TEXT *,TEXT *,ULONG);
VOID   CloseDisplay  (struct ILBMInfo *,CPTR *);
LONG   MakeDisplay   (struct ILBMInfo *);
void   JVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   JHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   MVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   MHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   JVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   JHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   MVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   MHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
BOOL   CheckBox      (struct RastPort *,const LONG,const LONG,const LONG,const LONG);
BOOL   RectangleDraw (struct Window *,const LONG,const LONG,const LONG,const LONG);
BOOL   NewCoords     (struct Window *,const WORD,const WORD,const WORD,const WORD);
void   BlinkRect     (struct Window *,const LONG,const LONG,const LONG,const LONG);
BOOL   DrawFrame     (struct Window *,const WORD,const WORD,const WORD,const WORD);
ULONG  DrawFractal   (struct Window *,const LONG,const LONG,const LONG,const LONG);
BOOL   Preview       (struct Window *,WORD,WORD);
BOOL   ShowCoords    (struct Window *);
void   RestoreCoords (struct Window *);
void   SaveCoords    (struct Window *,BOOL);
BOOL   FileRequest   (struct Window *,TEXT *,TEXT *,BOOL,BOOL);
BOOL   FontRequest   (struct Window *);
BOOL   SMRequest     (struct ILBMInfo *);
VOID   SetMenuStart  (struct Window *);
VOID   SetMenuStop   (struct Window *);
ULONG  ProcessMenu   (struct Window *,UWORD);
void   PickJuliaK    (struct Window *);
void   ProcessMouse  (struct Window *,WORD,WORD);
ULONG  HandleEvents  (struct ILBMInfo *);
LONG   WinDump       (struct Window *);
ULONG  Fail          (UBYTE *,ULONG);
BOOL   PasteBitMap   (struct BitMap *,struct Window *,WORD,WORD);
LONG   main          (LONG,CONST_STRPTR *);

static ULONG HOOKCALL SMFilterFunc (REG (a0,struct Hook *),REG (a2,struct ScreenModeRequester *),REG (a1,ULONG));

struct Screen *OpenIdScreen (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
struct Window *OpenDisplay  (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
struct BitMap *CopyBitMap   (struct Window *,WORD,WORD,WORD,WORD);

struct Hook         SMFILTERHOOK      = {NULL,NULL,(VOID *) SMFilterFunc,NULL};

struct Border       MYBORDER          = {0,0,0,0,COMPLEMENT,PAIRS,ZOOMLINE,0};

struct TextAttr     MYFONTSTRUCT      = {DEF_FONTNAMESTR,DEF_FONTSIZE,FS_NORMAL,FPF_DISKFONT};

struct NewGadget    TEXTGAD           = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

struct NewGadget    BUTTONGAD         = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

struct NewGadget    CHECKBOXGAD       = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

struct BitScaleArgs BSA               = {0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL};

struct Chunk        COPYRIGHT_CHUNK   = {NULL,ID_ILBM,ID_Copyright,IFFSIZE_UNKNOWN,"\n\n" VERSION COPYRIGHT_DATE " by " AUTHOR "\n\n"};

struct Chunk        SPECIAL_CHUNK     = {NULL,ID_ILBM,ID_MAND,sizeof (struct MandelChunk),NULL};

UWORD PENS [] = {BLACK,DARK_GREY,WHITE,WHITE,DARK_GREY,LIGHT_GREY,DARK_GREY,LIGHT_GREY,DARK_GREY,WHITE,LIGHT_GREY,DARK_GREY,(UWORD) ~0};

/*

01      0 DETAILPEN             SFONDO
02      1 BLOCKPEN              TESTO
03      1 TEXTPEN               TESTO EVIDENZIATO
04      2 SHINEPEN              BORDI CHIARI
05      1 SHADOWPEN             BORDI SCURI
06      3 FILLPEN               BARRA TITOLO FINESTRE ATTIVE
07      1 FILLTEXTPEN           TITOLO FINESTRE ATTIVE
08      0 BACKGROUNDPEN
09      2 HIGHLIGHTTEXTPEN

10      1 BARDETAILPEN          TESTO NEI MENU
11      2 BARBLOCKPEN           SFONDO NEI MENU
12      1 BARTRIMPEN

*/

ULONG COLORS_ECS [] = {32L << 16,
                       0x00000000,0x00000000,0x00000000,
                       0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
                       0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA,
                       0x66666666,0x66666666,0x66666666,
                       0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
                       0xBBBBBBBB,0xBBBBBBBB,0xBBBBBBBB,
                       0x77777777,0x77777777,0x77777777,
                       0x33333333,0x33333333,0x33333333,
                       0x33333333,0x33333333,0x00000000,
                       0x77777777,0x77777777,0x00000000,
                       0xBBBBBBBB,0xBBBBBBBB,0x00000000,
                       0xFFFFFFFF,0xFFFFFFFF,0x00000000,
                       0x00000000,0xFFFFFFFF,0x00000000,
                       0x00000000,0xBBBBBBBB,0x00000000,
                       0x00000000,0x77777777,0x00000000,
                       0x00000000,0x33333333,0x00000000,
                       0x33333333,0x00000000,0x33333333,
                       0x77777777,0x00000000,0x77777777,
                       0xBBBBBBBB,0x00000000,0xBBBBBBBB,
                       0xFFFFFFFF,0x00000000,0xFFFFFFFF,
                       0xFFFFFFFF,0x00000000,0x00000000,
                       0xBBBBBBBB,0x00000000,0x00000000,
                       0x77777777,0x00000000,0x00000000,
                       0x33333333,0x00000000,0x00000000,
                       0x00000000,0x33333333,0x33333333,
                       0x00000000,0x77777777,0x77777777,
                       0x00000000,0xBBBBBBBB,0xBBBBBBBB,
                       0x00000000,0xFFFFFFFF,0xFFFFFFFF,
                       0x00000000,0x00000000,0xFFFFFFFF,
                       0x00000000,0x00000000,0xBBBBBBBB,
                       0x00000000,0x00000000,0x77777777,
                       0x00000000,0x00000000,0x33333333,
                       NULL};


ULONG COLORS_AGA []  =  {256L << 16,
                         0x00000000,0x00000000,0x00000000, 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF, 0xAAAAAAAA,0xAAAAAAAA,0xAAAAAAAA, 0x66666666,0x66666666,0x66666666,
                         0x9f9f9f9f,0x9f9f9f9f,0x9f9f9f9f, 0xafafafaf,0xafafafaf,0xafafafaf, 0xbfbfbfbf,0xbfbfbfbf,0xbfbfbfbf, 0xcfcfcfcf,0xcfcfcfcf,0xcfcfcfcf,
                         0xdfdfdfdf,0xdfdfdfdf,0xdfdfdfdf, 0xefefefef,0xefefefef,0xefefefef, 0xdfdfdfdf,0xefefefef,0xffffffff, 0xcfcfcfcf,0xdfdfdfdf,0xefefefef,
                         0xbfbfbfbf,0xcfcfcfcf,0xdfdfdfdf, 0xafafafaf,0xbfbfbfbf,0xcfcfcfcf, 0x9f9f9f9f,0xafafafaf,0xbfbfbfbf, 0x8f8f8f8f,0x9f9f9f9f,0xafafafaf,
                         0x7f7f7f7f,0x8f8f8f8f,0x9f9f9f9f, 0x6f6f6f6f,0x7f7f7f7f,0x8f8f8f8f, 0x5f5f5f5f,0x6f6f6f6f,0x7f7f7f7f, 0x4f4f4f4f,0x5f5f5f5f,0x6f6f6f6f,
                         0x3f3f3f3f,0x4f4f4f4f,0x5f5f5f5f, 0x2f2f2f2f,0x3f3f3f3f,0x4f4f4f4f, 0x1f1f1f1f,0x2f2f2f2f,0x3f3f3f3f, 0x0f0f0f0f,0x1f1f1f1f,0x2f2f2f2f,
                         0x00000000,0x0f0f0f0f,0x1f1f1f1f, 0x1f1f1f1f,0x0f0f0f0f,0x1f1f1f1f, 0x2f2f2f2f,0x1f1f1f1f,0x2f2f2f2f, 0x3f3f3f3f,0x2f2f2f2f,0x3f3f3f3f,
                         0x4f4f4f4f,0x3f3f3f3f,0x4f4f4f4f, 0x5f5f5f5f,0x4f4f4f4f,0x5f5f5f5f, 0x6f6f6f6f,0x5f5f5f5f,0x6f6f6f6f, 0x7f7f7f7f,0x6f6f6f6f,0x7f7f7f7f,
                         0x8f8f8f8f,0x7f7f7f7f,0x8f8f8f8f, 0x9f9f9f9f,0x8f8f8f8f,0x9f9f9f9f, 0xafafafaf,0x9f9f9f9f,0xafafafaf, 0xbfbfbfbf,0xafafafaf,0xbfbfbfbf,
                         0xcfcfcfcf,0xbfbfbfbf,0xcfcfcfcf, 0xdfdfdfdf,0xcfcfcfcf,0xdfdfdfdf, 0xefefefef,0xdfdfdfdf,0xefefefef, 0xffffffff,0xefefefef,0xffffffff,
                         0xefefefef,0xffffffff,0xefefefef, 0xdfdfdfdf,0xefefefef,0xdfdfdfdf, 0xcfcfcfcf,0xdfdfdfdf,0xcfcfcfcf, 0xbfbfbfbf,0xcfcfcfcf,0xbfbfbfbf,
                         0xafafafaf,0xbfbfbfbf,0xafafafaf, 0x9f9f9f9f,0xafafafaf,0x9f9f9f9f, 0x8f8f8f8f,0x9f9f9f9f,0x8f8f8f8f, 0x7f7f7f7f,0x8f8f8f8f,0x7f7f7f7f,
                         0x6f6f6f6f,0x7f7f7f7f,0x6f6f6f6f, 0x5f5f5f5f,0x6f6f6f6f,0x5f5f5f5f, 0x4f4f4f4f,0x5f5f5f5f,0x4f4f4f4f, 0x3f3f3f3f,0x4f4f4f4f,0x3f3f3f3f,
                         0x2f2f2f2f,0x3f3f3f3f,0x2f2f2f2f, 0x1f1f1f1f,0x2f2f2f2f,0x1f1f1f1f, 0x0f0f0f0f,0x1f1f1f1f,0x0f0f0f0f, 0x0f0f0f0f,0x1f1f1f1f,0x1f1f1f1f,
                         0x1f1f1f1f,0x2f2f2f2f,0x2f2f2f2f, 0x2f2f2f2f,0x3f3f3f3f,0x3f3f3f3f, 0x3f3f3f3f,0x4f4f4f4f,0x4f4f4f4f, 0x4f4f4f4f,0x5f5f5f5f,0x5f5f5f5f,
                         0x5f5f5f5f,0x6f6f6f6f,0x6f6f6f6f, 0x6f6f6f6f,0x7f7f7f7f,0x7f7f7f7f, 0x7f7f7f7f,0x8f8f8f8f,0x8f8f8f8f, 0x8f8f8f8f,0x9f9f9f9f,0x9f9f9f9f,
                         0x9f9f9f9f,0xafafafaf,0xafafafaf, 0xafafafaf,0xbfbfbfbf,0xbfbfbfbf, 0xbfbfbfbf,0xcfcfcfcf,0xcfcfcfcf, 0xcfcfcfcf,0xdfdfdfdf,0xdfdfdfdf,
                         0xdfdfdfdf,0xefefefef,0xefefefef, 0xefefefef,0xffffffff,0xffffffff, 0xffffffff,0xefefefef,0xefefefef, 0xefefefef,0xdfdfdfdf,0xdfdfdfdf,
                         0xdfdfdfdf,0xcfcfcfcf,0xcfcfcfcf, 0xcfcfcfcf,0xbfbfbfbf,0xbfbfbfbf, 0xbfbfbfbf,0xafafafaf,0xafafafaf, 0xafafafaf,0x9f9f9f9f,0x9f9f9f9f,
                         0x9f9f9f9f,0x8f8f8f8f,0x8f8f8f8f, 0x8f8f8f8f,0x7f7f7f7f,0x7f7f7f7f, 0x7f7f7f7f,0x6f6f6f6f,0x6f6f6f6f, 0x6f6f6f6f,0x5f5f5f5f,0x5f5f5f5f,
                         0x5f5f5f5f,0x4f4f4f4f,0x4f4f4f4f, 0x4f4f4f4f,0x3f3f3f3f,0x3f3f3f3f, 0x3f3f3f3f,0x2f2f2f2f,0x2f2f2f2f, 0x2f2f2f2f,0x1f1f1f1f,0x1f1f1f1f,
                         0x1f1f1f1f,0x0f0f0f0f,0x0f0f0f0f, 0x1f1f1f1f,0x1f1f1f1f,0x0f0f0f0f, 0x2f2f2f2f,0x2f2f2f2f,0x1f1f1f1f, 0x3f3f3f3f,0x3f3f3f3f,0x2f2f2f2f,
                         0x4f4f4f4f,0x4f4f4f4f,0x3f3f3f3f, 0x5f5f5f5f,0x5f5f5f5f,0x4f4f4f4f, 0x6f6f6f6f,0x6f6f6f6f,0x5f5f5f5f, 0x7f7f7f7f,0x7f7f7f7f,0x6f6f6f6f,
                         0x8f8f8f8f,0x8f8f8f8f,0x7f7f7f7f, 0x9f9f9f9f,0x9f9f9f9f,0x8f8f8f8f, 0xafafafaf,0xafafafaf,0x9f9f9f9f, 0xbfbfbfbf,0xbfbfbfbf,0xafafafaf,
                         0xcfcfcfcf,0xcfcfcfcf,0xbfbfbfbf, 0xdfdfdfdf,0xdfdfdfdf,0xcfcfcfcf, 0xefefefef,0xefefefef,0xdfdfdfdf, 0xffffffff,0xffffffff,0xefefefef,
                         0xefefefef,0xefefefef,0xffffffff, 0xdfdfdfdf,0xdfdfdfdf,0xefefefef, 0xcfcfcfcf,0xcfcfcfcf,0xdfdfdfdf, 0xbfbfbfbf,0xbfbfbfbf,0xcfcfcfcf,
                         0xafafafaf,0xafafafaf,0xbfbfbfbf, 0x9f9f9f9f,0x9f9f9f9f,0xafafafaf, 0x8f8f8f8f,0x8f8f8f8f,0x9f9f9f9f, 0x7f7f7f7f,0x7f7f7f7f,0x8f8f8f8f,
                         0x6f6f6f6f,0x6f6f6f6f,0x7f7f7f7f, 0x5f5f5f5f,0x5f5f5f5f,0x6f6f6f6f, 0x4f4f4f4f,0x4f4f4f4f,0x5f5f5f5f, 0x3f3f3f3f,0x3f3f3f3f,0x4f4f4f4f,
                         0x2f2f2f2f,0x2f2f2f2f,0x3f3f3f3f, 0x1f1f1f1f,0x1f1f1f1f,0x2f2f2f2f, 0x0f0f0f0f,0x0f0f0f0f,0x1f1f1f1f, 0x0f0f0f0f,0x0f0f0f0f,0x00000000,
                         0x1f1f1f1f,0x1f1f1f1f,0x00000000, 0x2f2f2f2f,0x2f2f2f2f,0x00000000, 0x3f3f3f3f,0x3f3f3f3f,0x00000000, 0x4f4f4f4f,0x4f4f4f4f,0x00000000,
                         0x5f5f5f5f,0x5f5f5f5f,0x00000000, 0x6f6f6f6f,0x6f6f6f6f,0x00000000, 0x7f7f7f7f,0x7f7f7f7f,0x00000000, 0x8f8f8f8f,0x8f8f8f8f,0x00000000,
                         0x9f9f9f9f,0x9f9f9f9f,0x00000000, 0xafafafaf,0xafafafaf,0x00000000, 0xbfbfbfbf,0xbfbfbfbf,0x00000000, 0xcfcfcfcf,0xcfcfcfcf,0x00000000,
                         0xdfdfdfdf,0xdfdfdfdf,0x00000000, 0xefefefef,0xefefefef,0x00000000, 0xffffffff,0xffffffff,0x00000000, 0xefefefef,0xffffffff,0x00000000,
                         0xdfdfdfdf,0xffffffff,0x00000000, 0xcfcfcfcf,0xffffffff,0x00000000, 0xbfbfbfbf,0xffffffff,0x00000000, 0xafafafaf,0xffffffff,0x00000000,
                         0x9f9f9f9f,0xffffffff,0x00000000, 0x8f8f8f8f,0xffffffff,0x00000000, 0x7f7f7f7f,0xffffffff,0x00000000, 0x6f6f6f6f,0xffffffff,0x00000000,
                         0x5f5f5f5f,0xffffffff,0x00000000, 0x4f4f4f4f,0xffffffff,0x00000000, 0x3f3f3f3f,0xffffffff,0x00000000, 0x2f2f2f2f,0xffffffff,0x00000000,
                         0x1f1f1f1f,0xffffffff,0x00000000, 0x0f0f0f0f,0xffffffff,0x00000000, 0x00000000,0xffffffff,0x00000000, 0x00000000,0xefefefef,0x00000000,
                         0x00000000,0xdfdfdfdf,0x00000000, 0x00000000,0xcfcfcfcf,0x00000000, 0x00000000,0xbfbfbfbf,0x00000000, 0x00000000,0xafafafaf,0x00000000,
                         0x00000000,0x9f9f9f9f,0x00000000, 0x00000000,0x8f8f8f8f,0x00000000, 0x00000000,0x7f7f7f7f,0x00000000, 0x00000000,0x6f6f6f6f,0x00000000,
                         0x00000000,0x5f5f5f5f,0x00000000, 0x00000000,0x4f4f4f4f,0x00000000, 0x00000000,0x3f3f3f3f,0x00000000, 0x00000000,0x2f2f2f2f,0x00000000,
                         0x00000000,0x1f1f1f1f,0x00000000, 0x00000000,0x0f0f0f0f,0x00000000, 0x0f0f0f0f,0x00000000,0x0f0f0f0f, 0x1f1f1f1f,0x00000000,0x1f1f1f1f,
                         0x2f2f2f2f,0x00000000,0x2f2f2f2f, 0x3f3f3f3f,0x00000000,0x3f3f3f3f, 0x4f4f4f4f,0x00000000,0x4f4f4f4f, 0x5f5f5f5f,0x00000000,0x5f5f5f5f,
                         0x6f6f6f6f,0x00000000,0x6f6f6f6f, 0x7f7f7f7f,0x00000000,0x7f7f7f7f, 0x8f8f8f8f,0x00000000,0x8f8f8f8f, 0x9f9f9f9f,0x00000000,0x9f9f9f9f,
                         0xafafafaf,0x00000000,0xafafafaf, 0xbfbfbfbf,0x00000000,0xbfbfbfbf, 0xcfcfcfcf,0x00000000,0xcfcfcfcf, 0xdfdfdfdf,0x00000000,0xdfdfdfdf,
                         0xefefefef,0x00000000,0xefefefef, 0xffffffff,0x00000000,0xffffffff, 0xffffffff,0x00000000,0xefefefef, 0xffffffff,0x00000000,0xdfdfdfdf,
                         0xffffffff,0x00000000,0xcfcfcfcf, 0xffffffff,0x00000000,0xbfbfbfbf, 0xffffffff,0x00000000,0xafafafaf, 0xffffffff,0x00000000,0x9f9f9f9f,
                         0xffffffff,0x00000000,0x8f8f8f8f, 0xffffffff,0x00000000,0x7f7f7f7f, 0xffffffff,0x00000000,0x6f6f6f6f, 0xffffffff,0x00000000,0x5f5f5f5f,
                         0xffffffff,0x00000000,0x4f4f4f4f, 0xffffffff,0x00000000,0x3f3f3f3f, 0xffffffff,0x00000000,0x2f2f2f2f, 0xffffffff,0x00000000,0x1f1f1f1f,
                         0xffffffff,0x00000000,0x0f0f0f0f, 0xffffffff,0x00000000,0x00000000, 0xefefefef,0x00000000,0x00000000, 0xdfdfdfdf,0x00000000,0x00000000,
                         0xcfcfcfcf,0x00000000,0x00000000, 0xbfbfbfbf,0x00000000,0x00000000, 0xafafafaf,0x00000000,0x00000000, 0x9f9f9f9f,0x00000000,0x00000000,
                         0x8f8f8f8f,0x00000000,0x00000000, 0x7f7f7f7f,0x00000000,0x00000000, 0x6f6f6f6f,0x00000000,0x00000000, 0x5f5f5f5f,0x00000000,0x00000000,
                         0x4f4f4f4f,0x00000000,0x00000000, 0x3f3f3f3f,0x00000000,0x00000000, 0x2f2f2f2f,0x00000000,0x00000000, 0x1f1f1f1f,0x00000000,0x00000000,
                         0x0f0f0f0f,0x00000000,0x00000000, 0x00000000,0x0f0f0f0f,0x0f0f0f0f, 0x00000000,0x1f1f1f1f,0x1f1f1f1f, 0x00000000,0x2f2f2f2f,0x2f2f2f2f,
                         0x00000000,0x3f3f3f3f,0x3f3f3f3f, 0x00000000,0x4f4f4f4f,0x4f4f4f4f, 0x00000000,0x5f5f5f5f,0x5f5f5f5f, 0x00000000,0x6f6f6f6f,0x6f6f6f6f,
                         0x00000000,0x7f7f7f7f,0x7f7f7f7f, 0x00000000,0x8f8f8f8f,0x8f8f8f8f, 0x00000000,0x9f9f9f9f,0x9f9f9f9f, 0x00000000,0xafafafaf,0xafafafaf,
                         0x00000000,0xbfbfbfbf,0xbfbfbfbf, 0x00000000,0xcfcfcfcf,0xcfcfcfcf, 0x00000000,0xdfdfdfdf,0xdfdfdfdf, 0x00000000,0xefefefef,0xefefefef,
                         0x00000000,0xffffffff,0xffffffff, 0x00000000,0xefefefef,0xffffffff, 0x00000000,0xdfdfdfdf,0xffffffff, 0x00000000,0xcfcfcfcf,0xffffffff,
                         0x00000000,0xbfbfbfbf,0xffffffff, 0x00000000,0xafafafaf,0xffffffff, 0x00000000,0x9f9f9f9f,0xffffffff, 0x00000000,0x8f8f8f8f,0xffffffff,
                         0x00000000,0x7f7f7f7f,0xffffffff, 0x00000000,0x6f6f6f6f,0xffffffff, 0x00000000,0x5f5f5f5f,0xffffffff, 0x00000000,0x4f4f4f4f,0xffffffff,
                         0x00000000,0x3f3f3f3f,0xffffffff, 0x00000000,0x2f2f2f2f,0xffffffff, 0x00000000,0x1f1f1f1f,0xffffffff, 0x00000000,0x0f0f0f0f,0xffffffff,
                         0x00000000,0x00000000,0xffffffff, 0x00000000,0x00000000,0xefefefef, 0x00000000,0x00000000,0xdfdfdfdf, 0x00000000,0x00000000,0xcfcfcfcf,
                         0x00000000,0x00000000,0xbfbfbfbf, 0x00000000,0x00000000,0xafafafaf, 0x00000000,0x00000000,0x9f9f9f9f, 0x00000000,0x00000000,0x8f8f8f8f,
                         0x00000000,0x00000000,0x7f7f7f7f, 0x00000000,0x00000000,0x6f6f6f6f, 0x00000000,0x00000000,0x5f5f5f5f, 0x00000000,0x00000000,0x4f4f4f4f,
                         0x00000000,0x00000000,0x3f3f3f3f, 0x00000000,0x00000000,0x2f2f2f2f, 0x00000000,0x00000000,0x1f1f1f1f, 0x00000000,0x00000000,0x0f0f0f0f,
                         NULL};

CHIP UWORD ZoomPointer[] = {0x0000,0x0000,0x0100,0x0000,0x0100,0x0000,0x0000,0x0100,
                            0x0000,0x0100,0x0100,0x0100,0x0100,0x0100,0x0000,0x0000,
                            0xCC66,0x3C78,0x0000,0x0000,0x0100,0x0100,0x0100,0x0100,
                            0x0000,0x0100,0x0000,0x0100,0x0100,0x0000,0x0100,0x0000,
                            0x0000,0x0000};

struct NewMenu ProgMenu[] = { NM_TITLE,"Project",0,0,0,0,

                              NM_ITEM,"About...","A",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Help...","H",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Load picture...","L",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save picture...","S",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Load palette...","Y",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save palette...","E",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Print...","D",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Quit...","Q",NM_ITEMDISABLED,0,0,

/***************************************************************************************/

                              NM_TITLE,"Options",0,0,0,0,

                              NM_ITEM,"Title",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"Title bar","O",CHECKIT|MENUTOGGLE|CHECKED,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Last time","T",0,0,0,

                              NM_ITEM,"Limits...","C",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Iterations",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"64",0,CHECKIT,~(1<<0),0,
                              NM_SUB,"128",0,CHECKIT,~(1<<1),0,
                              NM_SUB,"192",0,CHECKIT,~(1<<2),0,
                              NM_SUB,"256",0,CHECKIT,~(1<<3),0,
                              NM_SUB,"320",0,CHECKIT|CHECKED,~(1<<4),0,
                              NM_SUB,"384",0,CHECKIT,~(1<<5),0,
                              NM_SUB,"448",0,CHECKIT,~(1<<6),0,
                              NM_SUB,"512",0,CHECKIT,~(1<<7),0,
                              NM_SUB,"1024",0,CHECKIT,~(1<<8),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Custom...","I",CHECKIT,~(1<<10),0,

                              NM_ITEM,"Priority",0,0,0,0,
                              NM_SUB,"-5","%",CHECKIT,~(1<<0),0,
                              NM_SUB,"-4","$",CHECKIT,~(1<<1),0,
                              NM_SUB,"-3","£",CHECKIT,~(1<<2),0,
                              NM_SUB,"-2","\"",CHECKIT,~(1<<3),0,
                              NM_SUB,"-1","!",CHECKIT,~(1<<4),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB," 0","0",CHECKIT,~(1<<6),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"+1","1",CHECKIT,~(1<<8),0,
                              NM_SUB,"+2","2",CHECKIT,~(1<<9),0,
                              NM_SUB,"+3","3",CHECKIT,~(1<<10),0,
                              NM_SUB,"+4","4",CHECKIT,~(1<<11),0,
                              NM_SUB,"+5","5",CHECKIT,~(1<<12),0,

                              NM_ITEM,"Fractal",0,0,0,0,
                              NM_SUB,"Julia","J",CHECKIT,~(1<<0),0,
                              NM_SUB,"Mandelbrot","M",CHECKIT,~(1<<1),0,

                              /* new menu to select between Vampire and classic */
                              NM_ITEM,"Optimized",0,0,0,0,
                              NM_SUB,"Vampire","(",CHECKIT,(1<<0),0,
                              NM_SUB,"Classic",")",CHECKIT,(1<<1),0,
                           
/***************************************************************************************/

                              NM_TITLE,"Calculate",0,0,0,0,

                              NM_ITEM,"PreView","W",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"ReCalculate","R",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Undo","U",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Zoom","Z",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Stop","X",0,0,0,

/***************************************************************************************/

                              NM_TITLE,"Video",0,0,0,0,

                              NM_ITEM,"Cycle",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"Forward",">",0,0,0,
                              NM_SUB,"Backward","<",0,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Delay...","-",0,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Palette...","P",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Screen mode...","V",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Font settings...","F",NM_ITEMDISABLED,0,0,

                              NM_END,0,0,0,0,0 };


struct Menu *MAINMENU = NULL;

struct BitMap *MYBITMAP = NULL;

struct Task *THISTASK = NULL;

struct TextFont *NEWFONT = NULL;

struct ILBMInfo MYILBM = {0};

LONG IlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_MAND, TAG_END };

LONG IlbmCollects [] = { TAG_END };

LONG IlbmStops [] = { ID_ILBM, ID_BODY, TAG_END };

/* if IMPORT is not specified the compiler spits out the warning "function declared but not defined" */
IMPORT WORD ASMCALL Mandel68k_FPU_Vampire (REG (d0,WORD),REG (fp0,double),REG (fp1,double));
IMPORT WORD ASMCALL Mandel68k_FPU_Classic (REG (d0,WORD),REG (fp0,double),REG (fp1,double));
IMPORT WORD ASMCALL Julia68k_FPU_Vampire (REG (d0,WORD),REG (fp0,double),REG (fp1,double),REG (fp6,double),REG (fp7,double)); 
IMPORT WORD ASMCALL Julia68k_FPU_Classic (REG (d0,WORD),REG (fp0,double),REG (fp1,double),REG (fp6,double),REG (fp7,double)); 

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

/* Vampire detection */
UWORD __asm detectvamp(void); 
          
IMPORT BOOL ModifyPalette (struct Window *,WORD,WORD,ULONG *);

IMPORT BOOL ScalePalette (struct Window *,ULONG *,ULONG,ULONG);

IMPORT BOOL Fade (struct Window *,ULONG *,ULONG,ULONG,BOOL);

IMPORT BOOL Cycle (struct Window *,ULONG,BOOL);

IMPORT LONG QueryMandPic (struct ILBMInfo *,struct MandelChunk **,UBYTE *);

IMPORT LONG LoadMandPic (struct ILBMInfo *,UBYTE *);

IMPORT LONG SaveMandPic (struct ILBMInfo *,struct Chunk *,struct Chunk *,UBYTE *);

IMPORT LONG LoadPalette (struct ILBMInfo *,UBYTE *);

IMPORT LONG SavePalette (struct ILBMInfo *,struct Chunk *,UBYTE *);


/**************************************************************************************************************/


LONG main (LONG Argc,CONST_STRPTR *Argv)
{
CONST_STRPTR *IconToolTypes;

  /* autodetection for Vampire */
  if (detectvamp()) {
        vampire=1;
        optimized=1;
        MHLine=MHLine_Vampire;
        MVLine=MVLine_Vampire;
        JHLine=JHLine_Vampire;
        JVLine=JVLine_Vampire;
  } else {
        vampire=0;
        optimized=0;
        MHLine=MHLine_Classic;
        MVLine=MVLine_Classic;
        JHLine=JHLine_Classic;
        JVLine=JVLine_Classic; 
  }
  
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

        MYILBM.Bmhd.w = DEF_WIDTH;

        MYILBM.Bmhd.h = DEF_HEIGHT;

        MYILBM.Bmhd.nPlanes = (CheckGFX () ? MAX_DEPTH : DEF_DEPTH);
     }

     if (ModeNotAvailable (MYILBM.camg)) MYILBM.camg = ModeFallBack (MYILBM.camg,MYILBM.Bmhd.w,MYILBM.Bmhd.h,MYILBM.Bmhd.nPlanes);

     if (MYILBM.Bmhd.nPlanes > GetMaxPlanes (MYILBM.camg)) MYILBM.Bmhd.nPlanes = GetMaxPlanes (MYILBM.camg);

     if (MYILBM.Bmhd.nPlanes < MIN_DEPTH)
     {
        Fail (OpenDisplayError,20L);

        goto End;
     }

     PALETTE = (CheckGFX () ? COLORS_AGA : COLORS_ECS);

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

     if (MAINMENU = CreateMenus (ProgMenu,TAG_END))
     {
        if (MYILBM.Bmhd.nPlanes < 6)
        {
           MAX_ITERATIONS = 63;

           ItemAddress (MAINMENU,FULLMENUNUM (1,2,4))->Flags ^= CHECKED;

           ItemAddress (MAINMENU,FULLMENUNUM (1,2,0))->Flags |= CHECKED;
        }

       switch (PRIORITY)
        {
          case -5: ItemAddress (MAINMENU,FULLMENUNUM (1,3,0))->Flags |= CHECKED;

                   break;

          case -4: ItemAddress (MAINMENU,FULLMENUNUM (1,3,1))->Flags |= CHECKED;

                   break;

          case -3: ItemAddress (MAINMENU,FULLMENUNUM (1,3,2))->Flags |= CHECKED;

                   break;

          case -2: ItemAddress (MAINMENU,FULLMENUNUM (1,3,3))->Flags |= CHECKED;

                   break;

          case -1: ItemAddress (MAINMENU,FULLMENUNUM (1,3,4))->Flags |= CHECKED;

                   break;

          case  0: ItemAddress (MAINMENU,FULLMENUNUM (1,3,6))->Flags |= CHECKED;

                   break;

          case  1: ItemAddress (MAINMENU,FULLMENUNUM (1,3,8))->Flags |= CHECKED;

                   break;

          case  2: ItemAddress (MAINMENU,FULLMENUNUM (1,3,9))->Flags |= CHECKED;

                   break;

          case  3: ItemAddress (MAINMENU,FULLMENUNUM (1,3,10))->Flags |= CHECKED;

                   break;

          case  4: ItemAddress (MAINMENU,FULLMENUNUM (1,3,11))->Flags |= CHECKED;

                   break;

          case  5: ItemAddress (MAINMENU,FULLMENUNUM (1,3,12))->Flags |= CHECKED;

                   break;
        }

        /* check vampire menus */        
        if (optimized) ItemAddress(MAINMENU,FULLMENUNUM(1,5,0))->Flags |= CHECKED;  
        else ItemAddress(MAINMENU,FULLMENUNUM(1,5,1))->Flags |= CHECKED;       
        /* disable vampire acceleration if vampire was not detected */
        if (!vampire) ItemAddress (MAINMENU,FULLMENUNUM (1,5,0))->Flags ^= ITEMENABLED;
        
        SetTaskPri (FindTask (NULL),PRIORITY);

        if (JULIA)
        {
           H_LINE = JHLine;

           V_LINE = JVLine;

           ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags |= CHECKED;
        }

        else
        {
           H_LINE = MHLine;

           V_LINE = MVLine;

           ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags |= CHECKED;
        }

        
     
        if (CURRENT_MAX_COLORS = MakeDisplay (&MYILBM))
        {
           CURRENT_MAX_COLORS -= RESERVED_PENS;

           SaveCoords (MYILBM.win,FALSE);

           MANDChunk.LeftEdge = MYILBM.win->LeftEdge;

           MANDChunk.TopEdge = MYILBM.win->TopEdge;

           MANDChunk.Width = MYILBM.win->Width;

           MANDChunk.Height = MYILBM.win->Height;

           MANDChunk.RMin = RMIN;

           MANDChunk.RMax = RMAX;

           MANDChunk.IMin = IMIN;

           MANDChunk.IMax = IMAX;

           MANDChunk.Iterations = MAX_ITERATIONS + 1L;

           MANDChunk.Special = NULL;

           SPECIAL_CHUNK.ch_Data = &MANDChunk;

           PutPointer (MYILBM.win,0,0,0,0,0,BUSY_POINTER);

           ELAPSEDTIME = DrawFractal (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);

           PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

           SetMenuStart (MYILBM.win);

           ShowTime (MYILBM.win,"Rendering elapsed time:",ELAPSEDTIME);

           ModifyIDCMP (MYILBM.win,NULL);

           ClearMenuStrip (MYILBM.win);

           About (MYILBM.win);

           ResetMenuStrip (MYILBM.win,MAINMENU);

           ModifyIDCMP (MYILBM.win,IDCMP_STANDARD);

           Execute (ASSIGNCURRENTDIR,NULL,NULL);

           do
           {
             while (HandleEvents (&MYILBM) & NEWDISPLAY_MSG)
             {
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

                         PasteBitMap (MYBITMAP,MYILBM.win,(WORD) GetBitMapAttr (MYBITMAP,BMA_WIDTH),(WORD) GetBitMapAttr (MYBITMAP,BMA_HEIGHT));

                         if (Choice (MYILBM.win,"Rendering requester","Screen propreties are changed.\nImage and colors can be inaccurate.\n\nRecalculate ?"))
                         {
                            SetMenuStop (MYILBM.win);

                            PutPointer (MYILBM.win,0,0,0,0,0,BUSY_POINTER);

                            ELAPSEDTIME = DrawFractal (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);

                            PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                            SetMenuStart (MYILBM.win);

                            ShowTime (MYILBM.win,"Rendering elapsed time:",ELAPSEDTIME);
                         }
                      }

                      else
                      {
                            Fail (MakeDisplayError,20L);

                            break;
                      }
                   }
             }

             if (RETURNVALUE >= 20L) break;

           } while (! Choice (MYILBM.win,"Exit requester","Are you sure ?"));

           if (BMASK & MASK) FreeBitMapSafety (MYBITMAP);

           Execute (ASSIGNREMOVE,NULL,NULL);
        }

        else Fail (MakeDisplayError,20L);

        Fade (MYILBM.win,PALETTE,50L,1L,TOBLACK);

        CloseDisplay (&MYILBM,VINFO);

        FreeMenus (MAINMENU);
   }

     else Fail (MenuError,20L);

End:

  exit (RETURNVALUE);
}

  /***********************************************************************************************************/

void PutPointer (struct Window *Win,UWORD *PointerImage,LONG Width,LONG Height,LONG XOrigin,LONG YOrigin,UBYTE Type)
{
  switch (Type)
  {
     case CLEAR_POINTER:  SetWindowPointer (Win,TAG_END);

                          break;

     case BUSY_POINTER:   SetWindowPointer (Win,WA_BusyPointer,TRUE,TAG_END);

                          break;

     case ZOOM_POINTER:   SetPointer (Win,PointerImage,Height,Width,XOrigin,YOrigin);
  }
}

UBYTE GetMaxPlanes (ULONG ModeID)
{
DisplayInfoHandle DisplayHandle;

struct DimensionInfo DimensionInfo;

UBYTE Planes = 0;

  DisplayHandle = FindDisplayInfo (ModeID);

  if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,ModeID))
  {
     Planes = DimensionInfo.MaxDepth;
  }

  else Fail (DimensionInfoError,20L);

  return Planes;
}

LONG CheckGFX (VOID)
{
DisplayInfoHandle DisplayHandle;

struct DisplayInfo DisplayInfo;

BOOL AGA = FALSE,RTG = FALSE;

ULONG ModeID = (ULONG) INVALID_ID;

  if (GetMaxPlanes (LORES_KEY) == MAX_DEPTH) AGA = TRUE;

  while (((ModeID = NextDisplayInfo (ModeID)) != INVALID_ID) && RTG == FALSE)
  {
        DisplayHandle = FindDisplayInfo (ModeID);

        if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DisplayInfo,sizeof (struct DisplayInfo),DTAG_DISP,ModeID))
        {
           if ((DisplayInfo.PropertyFlags & DIPF_IS_FOREIGN) && (GetMaxPlanes (ModeID) >= MAX_DEPTH)) RTG = TRUE;
        }
  }

  if (RTG && AGA) return 3L;

  if (RTG) return 2L;

  if (AGA) return 1L;

  return NULL;
}

VOID CloseDisplay (struct ILBMInfo *Ilbm,CPTR *VInfo)
{
  if (WMASK & MASK)
  {
     ClearMenuStrip (Ilbm->win);

     PutPointer (Ilbm->win,0,0,0,0,0,CLEAR_POINTER);

     CloseWindow (Ilbm->win);

     Ilbm->win = NULL;

     Ilbm->wrp = NULL;

     MASK ^= WMASK;
  }

  if (VMASK & MASK)
  {
     FreeVisualInfo (VInfo);

     MASK ^= VMASK;
  }

  if (MMASK & MASK)
  {
     FreeBitMapSafety (TempBM);

     MASK ^= MMASK;
  }

  if (LMASK & MASK)
  {
     FreeVec (PixelLine);

     MASK ^= LMASK;
  }

  if (SMASK & MASK)
  {
     CloseScreen (Ilbm->scr);

     FreeBitMapSafety (Ilbm->brbitmap);

     Ilbm->scr = NULL;

     Ilbm->vp  = NULL;

     Ilbm->srp = NULL;

     MASK ^= SMASK;
  }

  if (FMASK & MASK)
  {
     CloseFont (NEWFONT);

     MASK ^= FMASK;
  }  
}

BOOL AllocTempRast (UWORD Width,UBYTE Planes)
{
BOOL Success = FALSE;

  if (TempBM = AllocBitMap ((((Width + 15) >> 4) << 1), 1, Planes, BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES, NULL))
  {
     InitRastPort (&TempRP);

     TempRP.Layer = NULL;

     TempRP.BitMap = TempBM;

     if (PixelLine = AllocVec (((((Width + 15) * sizeof (*PixelLine)) >> 4)  << 4),MEMF_CLEAR | MEMF_PUBLIC)) Success = TRUE;

     else FreeBitMapSafety (TempBM);
  }

  return Success;
}

struct Window *OpenDisplay (struct ILBMInfo *Ilbm,WORD Width,WORD Height,WORD Depth,ULONG ModeID)
{
struct Screen *Scr;

struct Window *Win = NULL;

  if (Scr = OpenIdScreen (Ilbm,Width,Height,Depth,ModeID))
  {
     Win = OpenWindowTags (NULL,
                           WA_Left,Scr->LeftEdge,
                           WA_Top,Scr->TopEdge,
                           WA_Width,Scr->Width,
                           WA_Height,Scr->Height,
                           WA_ScreenTitle,VERSION,
                           WA_CustomScreen,Scr,
                           WA_IDCMP,IDCMP_STANDARD,
                           WA_Flags,WFLG_STANDARD,
                           WA_MouseQueue,1L,
                           WA_BusyPointer,TRUE,
                           TAG_END);

     if (! Win)
     {
        if (Scr) CloseScreen (Scr);

        return NULL;
     }

     Ilbm->scr = Scr;

     Ilbm->win = Win;

     Ilbm->vp  = &(Scr->ViewPort);

     Ilbm->srp = &(Scr->RastPort);

     Ilbm->wrp = Win->RPort;

     Ilbm->Bmhd.w = (UWORD) Ilbm->win->Width;

     Ilbm->Bmhd.h = Ilbm->win->Height;

     Ilbm->Bmhd.nPlanes = Ilbm->wrp->BitMap->Depth;
  }

  return (Win);
}

struct Screen *OpenIdScreen (struct ILBMInfo *Ilbm,WORD Width,WORD Height,WORD Depth,ULONG ModeID)
{
struct Rectangle Spos,DClip,TxtO,StdO,MaxO,UClip;

struct Rectangle *UClipp;

struct Screen *Scr;

LONG ErrorCode = NULL,TryNew;

ULONG BitMapTag,PassedTags;

  if (! Ilbm) return (NULL);

  TryNew = ((QueryOverscan (ModeID,&TxtO,OSCAN_TEXT)) && (QueryOverscan (ModeID,&StdO,OSCAN_STANDARD)) && (QueryOverscan (ModeID,&MaxO,OSCAN_MAX)));

  if (TryNew)
  {
     if (Ilbm->Video) Ilbm->ucliptype = OSCAN_VIDEO;

     if ((Ilbm->ucliptype) && (QueryOverscan (ModeID,&UClip,(LONG) Ilbm->ucliptype))) UClipp = &UClip;

     else UClipp = NULL;

     ClipIt (Width,Height,&Spos,&DClip,&TxtO,&StdO,&MaxO,UClipp,Ilbm->IFFPFlags & IFFPF_NOCENTER ? TRUE : FALSE);

     BitMapTag = ((Ilbm->brbitmap) && (Ilbm->stype & CUSTOMBITMAP)) ? SA_BitMap : TAG_IGNORE;

     PassedTags = Ilbm->stags ? TAG_MORE : TAG_IGNORE;

     Scr = OpenScreenTags (NULL,
                           SA_DisplayID,   ModeID,
                           SA_Type,        Ilbm->stype,
                           SA_Top,         Spos.MinY,
                           SA_Left,        Spos.MinX,
                           SA_Width,       Width,
                           SA_Height,      Height,
                           SA_Depth,       Depth,
                           SA_DClip,       &DClip,
                           SA_AutoScroll,  Ilbm->Autoscroll,
                           SA_Colors32,    PALETTE,
                           SA_Pens,        PENS,
                           SA_Interleaved, TRUE,
                           SA_Font,        &MYFONTSTRUCT,
                           SA_Title,       VERSION,
                           SA_ErrorCode,   &ErrorCode,
                           SA_ShowTitle,   Ilbm->TBState,
                           BitMapTag,      Ilbm->brbitmap,
                           PassedTags,     Ilbm->stags,
                           TAG_END);

     if (! Scr)
     {
        switch (ErrorCode)
        {
                 case OSERR_NOMONITOR   : Fail (NoMonitor,20L);

                                          break;

                 case OSERR_NOCHIPS     : Fail (NoChips,20L);

                                          break;

                 case OSERR_NOMEM       : Fail (NoMem,20L);

                                          break;

                 case OSERR_NOCHIPMEM   : Fail (NoChipMem,20L);

                                          break;

                 case OSERR_PUBNOTUNIQUE: Fail (PubNotUnique,20L);

                                          break;

                 case OSERR_UNKNOWNMODE : Fail (UnknownMode,20L);

                                          break;

                 case OSERR_TOODEEP     : Fail (ScreenTooDeep,20L);

                                          break;

                 case OSERR_ATTACHFAIL  : Fail (AttachScreen,20L);

                                          break;

                 case OSERR_NOTAVAILABLE: Fail (ModeNotAvailableError,20L);

                                          break;

                 default                : Fail (UnknownScrError,20L);

                                          break;
        }

        return NULL;
     }
  }

  else return NULL;

  return (Scr);
}

ULONG ModeFallBack (ULONG OldModeID,WORD Width,WORD Height,WORD Depth)
{
struct Screen *PubScreen;

ULONG NewModeID = HIRESLACE_KEY,ModeID;

  if (PubScreen = LockPubScreen (NULL))
  {
     NewModeID = GetVPModeID (&(PubScreen->ViewPort));

     UnlockPubScreen (NULL,PubScreen);
  }

  ModeID = BestModeID (BIDTAG_DIPFMustHave,(OldModeID & EXTRAHALFBRITE_KEY ? DIPF_IS_EXTRAHALFBRITE : 0),
                       BIDTAG_NominalWidth,Width,
                       BIDTAG_NominalHeight,Height,
                       BIDTAG_Depth,Depth,
/*                       BIDTAG_SourceID,NewModeID, */
                       TAG_END);

  if (ModeID != INVALID_ID) NewModeID = ModeID;

  return (NewModeID);
}

VOID ClipIt (WORD wide,WORD high,struct Rectangle *spos,struct Rectangle *dclip,struct Rectangle *txto, struct Rectangle *stdo,struct Rectangle *maxo, struct Rectangle *uclip,BOOL NoCenter)
{
struct Rectangle *besto;

WORD minx, maxx, miny, maxy;

WORD txtw, txth, stdw, stdh, bestw, besth;

    /* get the txt, std and max widths and heights */

    txtw = txto->MaxX - txto->MinX + 1;

    txth = txto->MaxY - txto->MinY + 1;

    stdw = stdo->MaxX - stdo->MinX + 1;

    stdh = stdo->MaxY - stdo->MinY + 1;

    if ((wide <= txtw) && (high <= txth))
    {
        besto = txto;

        bestw = txtw;

        besth = txth;
    }

    else
    {
        besto = stdo;

        bestw = stdw;

        besth = stdh;
    }

    if (uclip)
    {
        *dclip = *uclip;

        spos->MinX = uclip->MinX;

        spos->MinY = uclip->MinY;
    }

    else
    {
        /* CENTER the screen based on best oscan prefs
        * but confine dclip within max oscan limits
        *
        * FIX MinX first */

        spos->MinX = minx = besto->MinX - ((wide - bestw) >> 1);

        maxx = wide + minx - 1;

        if (maxx > maxo->MaxX)  maxx = maxo->MaxX;      /* too right */

        if (minx < maxo->MinX)
        {
            minx = maxo->MinX;  /* too left  */

            /* if we want left edge of screen not clipped */

            if (NoCenter) spos->MinX = minx;
        }

        /* FIX MinY */

        spos->MinY = miny = besto->MinY - ((high - besth) >> 1);

        /* if lower than top of txto, move up */

        spos->MinY = miny = MIN (spos->MinY,txto->MinY);

        maxy = high + miny - 1;

        if (maxy > maxo->MaxY)  maxy = maxo->MaxY;      /* too down  */

        if (miny < maxo->MinY)
        {
           miny = maxo->MinY;   /* too up    */

           /* if we want top of screen not clipped */

           if (NoCenter) spos->MinY = miny;
        }

        /* SET up dclip */

        dclip->MinX = minx;

        dclip->MinY = miny;

        dclip->MaxX = maxx;

        dclip->MaxY = maxy;
    }
}

LONG MakeDisplay (struct ILBMInfo *Ilbm)
{
static ULONG SAVED_COMPONENT = NULL,SAVED_POSITION = NULL;

  if (Ilbm->IFFPFlags & IFFPF_USERMODE) Ilbm->camg = Ilbm->usermodeid;

  Ilbm->Bmhd.w = MAX (MIN_WIDTH,Ilbm->Bmhd.w);

  Ilbm->Bmhd.h = MAX (MIN_HEIGHT,Ilbm->Bmhd.h);

  if (ModeNotAvailable (Ilbm->camg) || (Ilbm->IFFPFlags & IFFPF_BESTFIT))

     Ilbm->camg = ModeFallBack (Ilbm->camg,Ilbm->Bmhd.w,Ilbm->Bmhd.h,Ilbm->Bmhd.nPlanes);

  Ilbm->Bmhd.nPlanes = MAX (MIN_DEPTH,Ilbm->Bmhd.nPlanes);

  if (Ilbm->Bmhd.nPlanes > GetMaxPlanes (Ilbm->camg)) Ilbm->Bmhd.nPlanes = GetMaxPlanes (Ilbm->camg);

  if (NEWFONT = OpenDiskFont (&MYFONTSTRUCT)) MASK |= FMASK;

  else
  {
     strcpy (MYFONTSTRUCT.ta_Name,DEF_FONTNAMESTR);

     MYFONTSTRUCT.ta_YSize = DEF_FONTSIZE;

     MYFONTSTRUCT.ta_Style = FS_NORMAL;

     MYFONTSTRUCT.ta_Flags = FPF_ROMFONT;

     if (NEWFONT = OpenFont (&MYFONTSTRUCT)) MASK |= FMASK;

     else
     {
       Fail (FontError,20L);

       return NULL;
     }
  }

  *PALETTE = (1L << Ilbm->Bmhd.nPlanes) << 16;

  if (SAVED_POSITION) (*(PALETTE + SAVED_POSITION) = SAVED_COMPONENT);

  SAVED_POSITION = (3L * (ULONG) (1L << Ilbm->Bmhd.nPlanes)) + 1L;

  SAVED_COMPONENT = *(PALETTE + SAVED_POSITION);

  *(PALETTE + SAVED_POSITION) = NULL;

  if (! (OpenDisplay (Ilbm,MAX (Ilbm->Bmhd.pageWidth,Ilbm->Bmhd.w),MAX (Ilbm->Bmhd.pageHeight,Ilbm->Bmhd.h),Ilbm->Bmhd.nPlanes,Ilbm->camg))) return NULL;

  MASK |= SMASK;

  MASK |= WMASK;

  if (! (AllocTempRast (Ilbm->Bmhd.w,Ilbm->Bmhd.nPlanes)))
  {
     Fail (NoMemForTempRastPort,20L);

     return (NULL);
  }

  MASK |= MMASK;

  MASK |= LMASK;

  if (! (VINFO = GetVisualInfo (Ilbm->scr,TAG_END)))
  {
     Fail (VisualInfoError,20L);

     return NULL;
  }

  MASK |= VMASK;

  if (! (LayoutMenus (MAINMENU,VINFO,GTMN_TextAttr,&MYFONTSTRUCT,GTMN_NewLookMenus,TRUE,TAG_END)))
  {
     Fail (FontError,20L);

     return NULL;
  }

  SetMenuStrip (Ilbm->win,MAINMENU);

  Ilbm->EHB = FALSE;

  return (Ilbm->vp->ColorMap->Count);
}

VOID ShowTime (struct Window *Win,TEXT *String,ULONG Secs)
{
  sprintf (BAR_STRING,"%s %uh %um %us - Average speed: %.1Lf Pix/sec",String,Secs / 3600L,(Secs / 60L) % 60L,Secs % 60L,(double) ((Win->Width - Win->LeftEdge) * (Win->Height - Win->TopEdge)) / (double) Secs);

  SetWindowTitles (Win,(TEXT *) ~0,BAR_STRING);
}

void SaveCoords (struct Window *Win,BOOL ShowMenu)
{
  URMIN = RMIN;

  URMAX = RMAX;

  UIMIN = IMIN;

  UIMAX = IMAX;

  UJKRE = JKRE;

  UJKIM = JKIM;

  if (ShowMenu) OnMenu (Win,FULLMENUNUM (2,4,NOSUB));
}

void RestoreCoords (struct Window *Win)
{
  RMIN = URMIN;

  RMAX = URMAX;

  IMIN = UIMIN;

  IMAX = UIMAX;

  JKRE = UJKRE;

  JKIM = UJKIM;

  OffMenu (Win,FULLMENUNUM (2,4,NOSUB));
}

BOOL NewCoords (struct Window *Win,const WORD a1,const WORD b1,const WORD a2,const WORD b2)
{
double KReal,KImag;

  if (((a2 - a1) < 2L) || ((b2 - b1) < 2L)) return FALSE;

  KReal = fabs (RMAX-RMIN) / ((double) Win->Width);

  KImag = fabs (IMAX-IMIN) / ((double) Win->Height);

  SaveCoords (Win,TRUE);

  RMAX = RMIN + ((double) a2 * KReal);

  IMIN = IMAX - ((double) b2 * KImag);

  RMIN += ((double) a1 * KReal);

  IMAX -= ((double) b1 * KImag);

  return TRUE;
}

void AdjustRatio (double *RMin,double *IMax,double *RMax,double *IMin,WORD ScrWidth,WORD ScrHeight,BOOL KeepReal)
{
double RWidth,IHeight,RCenter,ICenter;

  IHeight = *IMax - *IMin;

  RWidth  = *RMax - *RMin;

  if (KeepReal)
  {
     ICenter = *IMin + (IHeight / 2.0);

     IHeight = (RWidth * ScrHeight) / ScrWidth;

     *IMax = ICenter + (IHeight / 2.0);

     *IMin = ICenter - (IHeight / 2.0);
  }

  else
  {
     RCenter = *RMin + (RWidth / 2.0);

     RWidth = (IHeight * ScrWidth) / ScrHeight;

     *RMax = RCenter + (RWidth / 2.0);

     *RMin = RCenter - (RWidth / 2.0);
  }
}

LONG About (struct Window *Win)
{
static struct EasyStruct MyReq_1 = { sizeof (struct EasyStruct),0,"Informations requester","  " VERSION COPYRIGHT_DATE "\n\n\n  Snail address:" "\n  " AUTHOR "\n  " ADDRESS "\n\n\n  " EMAIL "\n\n*******************************\n* This program is GiftWare !! *\n*******************************\n\n","Ok|More..." };

static struct EasyStruct MyReq_2 = { sizeof (struct EasyStruct),0,"Workgroup","Greetings & thanks in alphabetical order:\n\n  Claudio Pucci\n  Giorgio Signori\n  Giuseppe Staffelli\n  Sergio Tassi\n\n\nIf someone hasn't the courage to risk\nfor his ideals, either his ideals are\nworth nothing, or worth nothing is he\n\nAmiga forever.\n\n","Ok" };

LONG More;

  More = EasyRequest (Win,&MyReq_1,0);

  if (! More) EasyRequest (Win,&MyReq_2,0);

  return More;
}

ULONG Fail (UBYTE *ErrorString,ULONG ErrorLevel)
{
  DisplayBeep (NULL);

  fputs (ErrorString,stderr);

  RETURNVALUE = ErrorLevel;

  return ErrorLevel;
}

LONG Choice (struct Window *Win,TEXT *Title,TEXT *String)
{
struct EasyStruct MyReq = { sizeof (struct EasyStruct),NULL,0,0,"Yes|No" };

  MyReq.es_Title = Title;

  MyReq.es_TextFormat = String;

  return EasyRequest (Win,&MyReq,0);
}

BOOL ShowCoords (struct Window *Win)
{
struct Gadget *GadList = NULL,*StringGad_1,*StringGad_2,*StringGad_3,*StringGad_4,*StringGad_5,*StringGad_6;

struct Gadget *MyButtonGad,*MyCheckBoxGad,*MyGad;

struct Window *GadWin;

struct IntuiMessage *Message;

BOOL Exit = FALSE,Accept = FALSE,Reset = FALSE,Ratio = FALSE,KeepReal = TRUE;

TEXT String [MAXCHARS];

UWORD MyCode;

ULONG MyClass;

double Tmp_RMIN = RMIN,Tmp_IMAX = IMAX,Tmp_RMAX = RMAX,Tmp_IMIN = IMIN,Tmp_JKRE,Tmp_JKIM;

  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = CHECKBOXGAD.ng_VisualInfo  = VINFO;

  StringGad_1 = CreateContext (&GadList);

  TEXTGAD.ng_LeftEdge     = 118;

  TEXTGAD.ng_TopEdge      = 20;

  TEXTGAD.ng_Width        = 194;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = "_Left";

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  sprintf (String,"%+2.15Lf",RMIN);

  StringGad_1 = CreateGadget (STRING_KIND,StringGad_1,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Top ";

  sprintf (String,"%+2.15Lf",IMAX);

  StringGad_2 = CreateGadget (STRING_KIND,StringGad_1,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Right";

  TEXTGAD.ng_Flags        = PLACETEXT_RIGHT;

  sprintf (String,"%+2.15Lf",RMAX);

  StringGad_3 = CreateGadget (STRING_KIND,StringGad_2,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Bottom";

  sprintf (String,"%+2.15Lf",IMIN);

  StringGad_4 = CreateGadget (STRING_KIND,StringGad_3,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (24 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Julia real";

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  sprintf (String,"%+2.15Lf",JKRE);

  StringGad_5 = CreateGadget (STRING_KIND,StringGad_4,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "J_ulia imag";

  sprintf (String,"%+2.15Lf",JKIM);

  StringGad_6 = CreateGadget (STRING_KIND,StringGad_5,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,TRUE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  CHECKBOXGAD.ng_LeftEdge   = 14;

  CHECKBOXGAD.ng_TopEdge    = TEXTGAD.ng_TopEdge + TEXTGAD.ng_Height + 24;

  CHECKBOXGAD.ng_Width      = MYFONTSTRUCT.ta_YSize << 1L;

  CHECKBOXGAD.ng_Height     = MYFONTSTRUCT.ta_YSize + 6;

  CHECKBOXGAD.ng_GadgetText = "_Keep X axis for ratio";

  CHECKBOXGAD.ng_Flags      = PLACETEXT_RIGHT;

  CHECKBOXGAD.ng_GadgetID   = KEEP;

  MyCheckBoxGad = CreateGadget (CHECKBOX_KIND,StringGad_6,&CHECKBOXGAD,GTCB_Checked,TRUE,GTCB_Scaled,TRUE,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge   = 14;

  BUTTONGAD.ng_TopEdge    = CHECKBOXGAD.ng_TopEdge + CHECKBOXGAD.ng_Height + 30;

  BUTTONGAD.ng_Width      = 90;

  BUTTONGAD.ng_Height     = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  BUTTONGAD.ng_GadgetText = "_Accept";

  BUTTONGAD.ng_GadgetID   = ACCEPT;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyCheckBoxGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (14 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "Rat_io";

  BUTTONGAD.ng_GadgetID   = RATIO;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (14 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "_Default";

  BUTTONGAD.ng_GadgetID   = RESET;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (14 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "_Cancel";

  BUTTONGAD.ng_GadgetID   = CANCEL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  if (MyButtonGad)
  {
     GadWin = OpenWindowTags (NULL,WA_Left,Win->LeftEdge + 25,
                                   WA_Top,Win->TopEdge + 35,
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 24,
                                   WA_Height,BUTTONGAD.ng_TopEdge + BUTTONGAD.ng_Height + 35,
                                   WA_Title,"Coordinates requester",
                                   WA_ScreenTitle,"Insert new range...",
                                   WA_CustomScreen,Win->WScreen,
                                   WA_IDCMP,IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW|IDCMP_VANILLAKEY|STRINGIDCMP|BUTTONIDCMP,
                                   WA_Flags,WFLG_ACTIVATE|WFLG_DRAGBAR|WFLG_SIMPLE_REFRESH|WFLG_GIMMEZEROZERO|WFLG_RMBTRAP,
                                   WA_Gadgets,GadList,
                                   TAG_END);
     if (GadWin)
     {
        GT_RefreshWindow (GadWin,NULL);

        do {  WaitPort (GadWin->UserPort);

              if (Message = (struct IntuiMessage *) GT_GetIMsg (GadWin->UserPort))
              {
                     MyGad = (struct Gadget *) Message->IAddress;

                     MyClass = Message->Class;

                     MyCode = Message->Code;

                     GT_ReplyIMsg ((struct IntuiMessage *) Message);

                     switch (MyClass)
                     {
                            case IDCMP_REFRESHWINDOW : GT_BeginRefresh (GadWin);

                                                       GT_EndRefresh (GadWin,TRUE);

                                                       break;

                            case IDCMP_VANILLAKEY    : switch (ToUpper ((ULONG) MyCode))
                                                       {
                                                              case 'L' : ActivateGadget (StringGad_1,GadWin,0);

                                                                         break;

                                                              case 'T' : ActivateGadget (StringGad_2,GadWin,0);

                                                                         break;

                                                              case 'R' : ActivateGadget (StringGad_3,GadWin,0);

                                                                         break;

                                                              case 'B' : ActivateGadget (StringGad_4,GadWin,0);

                                                                         break;

                                                              case 'J' : ActivateGadget (StringGad_5,GadWin,0);

                                                                         break;

                                                              case 'U' : ActivateGadget (StringGad_6,GadWin,0);

                                                                         break;

                                                              case 'A' : Accept = TRUE;

                                                                         break;

                                                              case 'I' : Ratio = TRUE;

                                                                         break;

                                                              case 'D' : Reset = TRUE;

                                                                         break;

                                                              case 'C' : Exit = TRUE;

                                                                         break;

                                                              case 'K' : KeepReal = ! KeepReal;

                                                                         GT_SetGadgetAttrs (MyCheckBoxGad,GadWin,NULL,GTCB_Checked,KeepReal,TAG_END);

                                                                         break;

                                                           case VAN_ESC: Exit = TRUE;
                                                       }

                                                               break;

                            case IDCMP_GADGETUP      : switch (MyGad->GadgetID)
                                                       {
                                                              case ACCEPT:   Accept = TRUE;

                                                                             break;

                                                              case  RATIO:   Ratio = TRUE;

                                                                             break;

                                                              case  RESET:   Reset = TRUE;

                                                                             break;

                                                              case   KEEP:   KeepReal = ! KeepReal;

                                                                             break;

                                                              case CANCEL:   Exit = TRUE;
                                                       }

                                                       break;

                            case IDCMP_CLOSEWINDOW   : Exit = TRUE;
                     }

                     if (Accept)
                     {

                        Tmp_RMIN = strtod (((struct StringInfo *) StringGad_1->SpecialInfo)->Buffer,NULL);

                        Tmp_IMAX = strtod (((struct StringInfo *) StringGad_2->SpecialInfo)->Buffer,NULL);
                        
                        Tmp_RMAX = strtod (((struct StringInfo *) StringGad_3->SpecialInfo)->Buffer,NULL);

                        Tmp_IMIN = strtod (((struct StringInfo *) StringGad_4->SpecialInfo)->Buffer,NULL);

                        Tmp_JKRE = strtod (((struct StringInfo *) StringGad_5->SpecialInfo)->Buffer,NULL);

                        Tmp_JKIM = strtod (((struct StringInfo *) StringGad_6->SpecialInfo)->Buffer,NULL);

                        if ((Tmp_RMIN >= Tmp_RMAX) || (Tmp_IMIN >= Tmp_IMAX))

                           DisplayBeep (Win->WScreen);

                        else
                        {
                           SaveCoords (Win,FALSE);

                           RMIN = Tmp_RMIN; IMAX = Tmp_IMAX; RMAX = Tmp_RMAX; IMIN = Tmp_IMIN; JKRE = Tmp_JKRE; JKIM = Tmp_JKIM;

                           Exit = TRUE;
                        }
                     }

                     if (Ratio)
                     {
                        AdjustRatio (&Tmp_RMIN,&Tmp_IMAX,&Tmp_RMAX,&Tmp_IMIN,Win->Width,Win->Height,KeepReal);

                        TEXTGAD.ng_Flags = PLACETEXT_LEFT;

                        sprintf (String,"%+2.15Lf",Tmp_RMIN);

                        GT_SetGadgetAttrs (StringGad_1,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",Tmp_IMAX);

                        GT_SetGadgetAttrs (StringGad_2,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",Tmp_RMAX);

                        TEXTGAD.ng_Flags = PLACETEXT_RIGHT;

                        GT_SetGadgetAttrs (StringGad_3,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",Tmp_IMIN);

                        GT_SetGadgetAttrs (StringGad_4,GadWin,NULL,GTST_String,String,TAG_END);

                        Ratio = FALSE;
                     }

                     if (Reset)
                     {
                        TEXTGAD.ng_Flags = PLACETEXT_LEFT;

                        sprintf (String,"%+2.15Lf",DEF_RMIN);

                        GT_SetGadgetAttrs (StringGad_1,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",DEF_IMAX);

                        GT_SetGadgetAttrs (StringGad_2,GadWin,NULL,GTST_String,String,TAG_END);

                        TEXTGAD.ng_Flags = PLACETEXT_RIGHT;

                        sprintf (String,"%+2.15Lf",DEF_RMAX);

                        GT_SetGadgetAttrs (StringGad_3,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",DEF_IMIN);

                        GT_SetGadgetAttrs (StringGad_4,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",DEF_JKRE);

                        GT_SetGadgetAttrs (StringGad_5,GadWin,NULL,GTST_String,String,TAG_END);

                        sprintf (String,"%+2.15Lf",DEF_JKIM);

                        GT_SetGadgetAttrs (StringGad_6,GadWin,NULL,GTST_String,String,TAG_END);

                        Reset = FALSE;
                     }
              }

           } while (Exit == FALSE);

       CloseWindow (GadWin);
     }

     else Fail (WindowGadgetError,15L);
  }

  else Fail (GadgetError,15L);

  FreeGadgets (GadList);

  return Accept;
}

ULONG IntegerGad (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr,TEXT *TxtString,ULONG Var)
{
struct IntuiMessage *Message;

struct Gadget *GadList = NULL,*MyIntGad,*MyButtonGad,*MyGad;

struct Window *GadWin;

BOOL Exit = FALSE;

UWORD MyCode;

ULONG MyClass;

  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyIntGad = CreateContext (&GadList);

  TEXTGAD.ng_LeftEdge     = 170;

  TEXTGAD.ng_TopEdge      = 20;

  TEXTGAD.ng_Width        = 90;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = TxtString;

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  MyIntGad = CreateGadget (INTEGER_KIND,MyIntGad,&TEXTGAD,GTIN_Number,Var,GTIN_MaxChars,6,STRINGA_ReplaceMode,TRUE,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge   = 20;

  BUTTONGAD.ng_TopEdge    = TEXTGAD.ng_TopEdge + TEXTGAD.ng_Height + 30;

  BUTTONGAD.ng_Width      = 90;

  BUTTONGAD.ng_Height     = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  BUTTONGAD.ng_GadgetText = "_Accept";

  BUTTONGAD.ng_GadgetID   = ACCEPT;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyIntGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (60 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "_Cancel";

  BUTTONGAD.ng_GadgetID   = CANCEL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  if (MyButtonGad)
  {
     GadWin = OpenWindowTags (NULL,WA_Left,Win->LeftEdge + 25,
                                   WA_Top,Win->TopEdge + 35,
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 30,
                                   WA_Height,BUTTONGAD.ng_TopEdge + BUTTONGAD.ng_Height + 35,
                                   WA_Title,TitleWin,
                                   WA_ScreenTitle,TitleScr,
                                   WA_CustomScreen,Win->WScreen,
                                   WA_IDCMP,IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW|IDCMP_VANILLAKEY|INTEGERIDCMP|BUTTONIDCMP,
                                   WA_Flags,WFLG_ACTIVATE|WFLG_DRAGBAR|WFLG_SIMPLE_REFRESH|WFLG_GIMMEZEROZERO|WFLG_RMBTRAP,
                                   WA_Gadgets,GadList,
                                   TAG_END);

     if (GadWin)
     {
        GT_RefreshWindow (GadWin,NULL);

        do {  WaitPort (GadWin->UserPort);

              if (Message = (struct IntuiMessage *) GT_GetIMsg (GadWin->UserPort))
              {
                     MyGad = (struct Gadget *) Message->IAddress;

                     MyClass = Message->Class;

                     MyCode = Message->Code;

                     GT_ReplyIMsg ((struct IntuiMessage *) Message);

                     switch (MyClass)
                     {
                            case IDCMP_REFRESHWINDOW : GT_BeginRefresh (GadWin);

                                                       GT_EndRefresh (GadWin,TRUE);

                                                       break;

                            case IDCMP_VANILLAKEY    : switch (ToUpper ((ULONG) MyCode))
                                                       {
                                                              case 'D' :

                                                              case 'I' : ActivateGadget (MyIntGad,GadWin,0);

                                                                         break;

                                                              case 'A' : Var = ((struct StringInfo *) MyIntGad->SpecialInfo)->LongInt;

                                                                         Exit = TRUE;

                                                                         break;

                                                              case 'C' : Exit = TRUE;

                                                                         break;

                                                           case VAN_ESC: Exit = TRUE;
                                                       }

                                                       break;

                            case IDCMP_GADGETUP      : switch (MyGad->GadgetID)
                                                       {
                                                               case ACCEPT  : Var = ((struct StringInfo *) MyIntGad->SpecialInfo)->LongInt;

                                                                              Exit = TRUE;

                                                                              break;

                                                               case CANCEL  : Exit = TRUE;
                                                       }

                                                       break;

                            case IDCMP_CLOSEWINDOW   : Exit = TRUE;
                     }
              }

           } while (Exit == FALSE);

        CloseWindow (GadWin);
     }

     else Fail (WindowError,15L);
  }

  else Fail (GadgetError,15L);

  FreeGadgets (GadList);

  return Var;
}

void MVLine_Vampire (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
 REGISTER LONG y;

 ULONG Color;
 ULONG b1y;
 ULONG oldpen;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 y=b2-b1+1;
 oldpen=0;
 SetAPen(Rp,oldpen);
 
 while (y>=4)
 {
     /* prepare 4 points */
     py1=CIM; 
     py2=py1+INCREMIMAG;
     py3=py2+INCREMIMAG;
     py4=py3+INCREMIMAG;

     b1y=b1+y;     
     par4iterateasm(CRE,py1,CRE,py2,CRE,py3,CRE,py4,MAX_ITERATIONS);
     /* p1 */
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-1);
     /* p2 */
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-2);
     /* p3 */
     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-3);
     /* p4 */
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-4);

     CIM = py4+INCREMIMAG;
     y-=4;

 }
 
 /* do the rest */
 while (y--)
 {
     Color = Mandel68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1+y);

     CIM += INCREMIMAG;
 }

}

void MVLine_Classic (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);
 
 for (y = b2; y >= b1; y--)
 {
     Color = Mandel68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     

     SetAPen (Rp,Color);

     WritePixel (Rp,x,y);

     CIM += INCREMIMAG;
 }
}

void MHLine_Vampire (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x;

REGISTER UBYTE *TmpArray;

REGISTER ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x>=4)
 {
     /* prepare 4 points for parallel calculation */
     px1=CRE; 
     px2=px1+INCREMREAL;
     px3=px2+INCREMREAL;
     px4=px3+INCREMREAL;
     
     par4iterateasm(px1,CIM,px2,CIM,px3,CIM,px4,CIM,MAX_ITERATIONS);
            
     /* write 4 results to TmpArray (whatever that is ... :) */
     /* p1 */
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p2 */
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p3 */
     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p4 */
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
        
     CRE = px4+INCREMREAL;
        
     x-=4;
 }

 
  while (x--)
  {
     Color = Mandel68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
  }

 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

void MHLine_Classic (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x;

REGISTER UBYTE *TmpArray;

REGISTER ULONG Color;
 
 x = width;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 TmpArray = PixelLine;

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = Mandel68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }

 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

void JVLine_Vampire (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color, oldpen;
ULONG b1y;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);

 y=b2-b1+1  ;
 oldpen=0;
 SetAPen(Rp,0);
 
 while (y>=4)
 {
     py1=CIM; 
     py2=py1+INCREMIMAG;
     py3=py2+INCREMIMAG;
     py4=py3+INCREMIMAG;

     b1y=y+b1;
     
     juliapar4iterateasmvert(CRE,py1,py2,py3,py4,JKRE,JKIM,MAX_ITERATIONS);
     
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
    
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-1);
 
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-2);

     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-3);
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y-4);

     CIM = py4+INCREMIMAG;
     y-=4;

 }
 
 while (y--)
 {
     Color = Julia68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);
     
     b1y=y+b1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     if (oldpen!=Color) 
     {
        SetAPen (Rp,Color);
        oldpen=Color;
     }
     WritePixel (Rp,x,b1y);

     CIM += INCREMIMAG;
 }

}

void JVLine_Classic (struct RastPort *Rp,const LONG b1,const LONG b2,const LONG x)
{
REGISTER LONG y;

ULONG Color;

 CRE = RMIN + (((double)  x) * INCREMREAL);

 CIM = IMAX - (((double) b2) * INCREMIMAG);

 for (y = b2; y >= b1; y--)
 {
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     SetAPen (Rp,Color);

     WritePixel (Rp,x,y);

     CIM += INCREMIMAG;
 }
}

void JHLine_Vampire (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x = width;

UBYTE *TmpArray = PixelLine;

ULONG Color;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x>=4)
 {
     /* prepare 4 points for parallel calculation */
     px1=CRE; 
     px2=px1+INCREMREAL;
     px3=px2+INCREMREAL;
     px4=px3+INCREMREAL;
     
     juliapar4iterateasmhoriz(px1,CIM,px2,px3,px4,JKRE,JKIM,MAX_ITERATIONS);
     
     /* write 4 results to TmpArray (whatever that is ... :) */
     /* p1 */
     Color=IterP1;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p2 */
     Color=IterP2;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p3 */
     Color=IterP3;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
     /* p4 */
     Color=IterP4;
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     *TmpArray++ = Color;
        
     CRE = px4+INCREMREAL;
        
     x-=4;
 }

 /* do the rest */
 while (x--)
 {
     Color = Julia68k_FPU_Vampire (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);
  
     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }

     CRE += INCREMREAL;

     *TmpArray++ = Color;
  }
  
 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

void JHLine_Classic (struct RastPort *Rp,const LONG a1,const LONG width,const LONG y)
{
REGISTER LONG x = width;

UBYTE *TmpArray = PixelLine;

ULONG Color;

 CRE = RMIN + (((double) a1) * INCREMREAL);

 CIM = IMAX - (((double)  y) * INCREMIMAG);

 while (x--)
 {
     Color = Julia68k_FPU_Classic (MAX_ITERATIONS,CRE,CIM,JKRE,JKIM);

     if (Color)
     {
        Color *= CURRENT_MAX_COLORS,

        Color /= ITERATIONS,

        Color += RESERVED_PENS;
     }
     
     CRE += INCREMREAL;

     *TmpArray++ = Color;
 }
 WritePixelLine8 (Rp, a1, y, width, PixelLine, &TempRP);
}

BOOL CheckBox (struct RastPort *Rp,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
const LONG Color = ReadPixel (Rp,a1,b1);

REGISTER LONG Var;

  if (Color != ReadPixel (Rp,a2,b2)) return FALSE;

  if (Color != ReadPixel (Rp,a2,b1)) return FALSE;

  if (Color != ReadPixel (Rp,a1,b2)) return FALSE;

  for (Var = (a1 + 1L); Var < a2; Var++)
  {
      if (Color != ReadPixel (Rp,Var,b1)) return FALSE;

      if (Color != ReadPixel (Rp,Var,b2)) return FALSE;
  }

  for (Var = (b1 + 1L); Var < b2; Var++)
  {
      if (Color != ReadPixel (Rp,a1,Var)) return FALSE;

      if (Color != ReadPixel (Rp,a2,Var)) return FALSE;
  }

  return TRUE;
}

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

BOOL RectangleDraw (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
struct IntuiMessage *Message;

UWORD MyCode;

LONG helpx,helpy;

ULONG MyClass;  

  if ((helpx = (a2 - a1)) < MINLIMIT) return FALSE;

  if ((helpy = (b2 - b1)) < MINLIMIT) return FALSE;

  if (Win->UserPort->mp_SigBit)
  {
     if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
     {
        MyClass = Message->Class;

        MyCode  = Message->Code;

        GT_ReplyIMsg ((struct IntuiMessage *) Message);

        switch (MyClass)
        {
          case IDCMP_MENUPICK    : if (MyCode != MENUNULL)
                                   {
                                      if (ProcessMenu (Win,MyCode) & STOP_MSG) return TRUE;
                                   }

                                   break;

          case IDCMP_RAWKEY      : if (MyCode == TAB)
                                   {
                                      BlinkRect (Win,a1,b1,a2,b2);

                                      return FALSE;
                                   }

                                   if (MyCode == RAW_ESC)
                                   {
                                      DisplayBeep (Win->WScreen);

                                      return TRUE;
                                   }

                                   break;

          case IDCMP_CLOSEWINDOW : return TRUE;
        }
     }
  }

  if (CheckBox (Win->RPort,a1,b1,a2,b2))
  {
     SetAPen (Win->RPort,ReadPixel (Win->RPort,a1,b1));

     RectFill (Win->RPort,a1+1L,b1+1L,a2-1L,b2-1L);

     return FALSE;
  }

  if ((helpx < (MINLIMIT * 2L)) || (helpy < (MINLIMIT * 2L)))
  {
     for (helpx = (a1 + 1L); helpx < a2; helpx++)
     {
         (*V_LINE) (Win->RPort,b1+1L,b2-1L,helpx);
     }

     return FALSE;
  }

  if (helpx >= helpy)
  {
     helpx = (a1 + a2) >> 1L;

     (*V_LINE) (Win->RPort,b1+1L,b2-1L,helpx);

     if (RectangleDraw (Win,a1,b1,helpx,b2)) return TRUE;

     if (RectangleDraw (Win,helpx,b1,a2,b2)) return TRUE;
  }

  else
  {
     helpy = (b1 + b2) >> 1L;

     (*H_LINE) (Win->RPort,a1+1L,a2-a1-1,helpy);

     if (RectangleDraw (Win,a1,b1,a2,helpy)) return TRUE;

     if (RectangleDraw (Win,a1,helpy,a2,b2)) return TRUE;
  }

  return FALSE;
}

ULONG DrawFractal (struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
ULONG StartSec = NULL , EndSec = NULL , Dummy = NULL;

 CurrentTime (&StartSec,&Dummy);

 INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 ITERATIONS = MAX_ITERATIONS + 1;

 if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

 (*H_LINE) (Win->RPort,a1,a2-a1+1,b1);

 (*V_LINE) (Win->RPort,b1+1,b2-1,a2);

 (*H_LINE) (Win->RPort,a1,a2-a1+1,b2);

 (*V_LINE) (Win->RPort,b1+1,b2-1,a1);

 RectangleDraw (Win,a1,b1,a2,b2);

 if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

 CurrentTime (&EndSec,&Dummy);

 DisplayBeep (Win->WScreen);

 return (EndSec-StartSec);
}

BOOL Preview (struct Window *Win,WORD Width,WORD Height)
{
struct Window *PreviewWin;

struct IntuiMessage *Message;

BOOL Error = FALSE;

UWORD MyCode;

ULONG MyClass;

 MYBITMAP = CopyBitMap (Win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

 PreviewWin = OpenWindowTags (0,WA_Left,Win->LeftEdge + 25,
                                WA_Top,Win->TopEdge + 35,
                                WA_Width,(WORD) (Width  >> 1L),
                                WA_Height,(WORD) (Height >> 1L),
                                WA_Title,"Preview window",
                                WA_ScreenTitle,"¼ Window preview...",
                                WA_CustomScreen,Win->WScreen,
                                WA_IDCMP,IDCMP_RAWKEY|IDCMP_CLOSEWINDOW,
                                WA_Flags,WFLG_ACTIVATE|WFLG_CLOSEGADGET|WFLG_DRAGBAR|WFLG_NOCAREREFRESH|WFLG_SMART_REFRESH|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO,
                                WA_BusyPointer,TRUE,
                                TAG_END);
 if (PreviewWin)
 {
    PasteBitMap (MYBITMAP,PreviewWin,ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

    PutPointer (PreviewWin,0,0,0,0,0,BUSY_POINTER);

    ELAPSEDTIME = DrawFractal (PreviewWin,0L,0L,(LONG) (PreviewWin->GZZWidth) - 1L,(LONG) (PreviewWin->GZZHeight) - 1L);

    PutPointer (PreviewWin,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

    ShowTime (Win,"Preview elapsed time:",ELAPSEDTIME);

    while (1L)
    {
         WaitPort (PreviewWin->UserPort);

         if (Message = (struct IntuiMessage *) GT_GetIMsg (PreviewWin->UserPort))
         {
            MyClass = Message->Class;

            MyCode  = Message->Code;

            GT_ReplyIMsg ((struct IntuiMessage *) Message);

            if (MyClass == IDCMP_CLOSEWINDOW) break;

            if ((MyClass == IDCMP_RAWKEY) && (MyCode == RAW_ESC)) break;
         }
    }

    PutPointer (PreviewWin,0,0,0,0,0,CLEAR_POINTER);

    CloseWindow (PreviewWin);
 }

 else
 {
    if (BMASK & MASK)
    {
       FreeBitMapSafety (MYBITMAP);

       MASK ^= BMASK;
    }

    Fail (PreviewWindowError,15L);

    Error = TRUE;
 }

 return Error;
}

BOOL DrawFrame (struct Window *Win,const WORD Zx,const WORD Zy,const WORD Zw,const WORD Zh)
{
WORD LeftEdge,TopEdge,RightEdge,BottomEdge;

  if ((Zx > 2) && (Zy > 2) && (Zx < Win->Width - 2) && (Zy < Win->Height - 2) && ((Zw >= INITIALZOOM) || (Zh >= INITIALZOOM)))
  {
     LeftEdge   = Zx - Zw;

     TopEdge    = Zy - Zh;

     RightEdge  = Zx + Zw;

     BottomEdge = Zy + Zh;

     if ((LeftEdge >= 0) && (TopEdge >= 0) && (RightEdge <= Win->Width) && (BottomEdge <= Win->Height))
     {
        ZOOMLINE [8] = ZOOMLINE [0] = ZOOMLINE [6] = LeftEdge;

        ZOOMLINE [1] = ZOOMLINE [3] = ZOOMLINE [9] = TopEdge;

        ZOOMLINE [2] = ZOOMLINE [4] = RightEdge;

        ZOOMLINE [5] = ZOOMLINE [7] = BottomEdge;
     }

     DrawBorder (Win->RPort,&MYBORDER,0,0);

     MASK |= ZMASK;
  }

  else MASK ^= ZMASK;

  return (BOOL) (ZMASK & MASK);
}

BOOL FileRequest (struct Window *Win,TEXT *String,TEXT *DrawerTxt,BOOL DrawerType,BOOL Save)
{
struct FileRequester *MyFileReq;

static struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Overwrite file requester","File already exists...\n\nDo you want overwrite it ?","Yes|No" };

BPTR MyLock;

BOOL Success = FALSE;

  if (MyFileReq = AllocAslRequest (ASL_FileRequest,0))
  {
     MYDIR = (DrawerType ? PALETTESDIR : PICTURESDIR);

     if (! strlen (MYDIR)) strncpy (MYDIR,DrawerTxt,MAX_DIRLEN);

     if (AslRequestTags (MyFileReq,ASLFR_Window,Win,
                                   ASLFR_InitialLeftEdge,Win->LeftEdge + 25,
                                   ASLFR_InitialTopEdge,Win->TopEdge + 35,
                                   ASLFR_InitialWidth,300,
                                   ASLFR_InitialHeight,((Win->Height) * 7) >> 3,
                                   ASLFR_SleepWindow,TRUE,
                                   ASLFR_TextAttr,&MYFONTSTRUCT,
                                   ASLFR_TitleText,String,
                                   ASLFR_InitialDrawer,MYDIR,
                                   ASLFR_InitialFile,MYFILE,
                                   ASLFR_DoSaveMode,Save,
                                   ASLFR_RejectIcons,TRUE,
                                   TAG_END))
     {
        strncpy (MYFILE,MyFileReq->rf_File,MAX_FILELEN);

        strncpy (MYDIR,MyFileReq->rf_Dir,MAX_DIRLEN);

        strcpy (MYPATH,MYDIR);

        Success = AddPart (MYPATH,MYFILE,sizeof (MYPATH));
     }

     FreeAslRequest (MyFileReq);

     if (Save)
     {
        if (MyLock = Lock (MYPATH,ACCESS_READ))
        {
           UnLock (MyLock);

           if (! EasyRequest (Win,&MyReq,0)) Success = FALSE;
        }
     }
  }

  return Success;
}

BOOL FontRequest (struct Window *Win)
{
struct FontRequester *MyFontReq;

BOOL Success = FALSE;

  if (MyFontReq = AllocAslRequest (ASL_FontRequest,0))
  {
     if (AslRequestTags (MyFontReq,ASLFO_Window,Win,
                                   ASLFO_InitialLeftEdge,Win->LeftEdge + 25,
                                   ASLFO_InitialTopEdge,Win->TopEdge + 35,
                                   ASLFO_InitialWidth,300,
                                   ASLFO_InitialHeight,((Win->Height) * 7) >> 3,
                                   ASLFO_SleepWindow,TRUE,
                                   ASLFO_TextAttr,&MYFONTSTRUCT,
                                   ASLFO_TitleText,"Font requester",
                                   ASLFO_InitialName,MYFONTSTRUCT.ta_Name,
                                   ASLFO_InitialSize,MYFONTSTRUCT.ta_YSize,
                                   ASLFO_InitialStyle,MYFONTSTRUCT.ta_Style,
                                   ASLFO_InitialFlags,MYFONTSTRUCT.ta_Flags,
                                   ASLFO_DoFrontPen,TRUE,
                                   ASLFO_DoBackPen,TRUE,
                                   ASLFO_DoStyle,TRUE,
                                   ASLFO_DoDrawMode,TRUE,
                                   ASLFO_MinHeight,8,
                                   ASLFO_MaxHeight,24,
                                   TAG_END))
     {
       strncpy (MYFONTSTRUCT.ta_Name,MyFontReq->fo_Attr.ta_Name,MAX_FILELEN);

       MYFONTSTRUCT.ta_YSize = MyFontReq->fo_Attr.ta_YSize;

       MYFONTSTRUCT.ta_Style = MyFontReq->fo_Attr.ta_Style;

       MYFONTSTRUCT.ta_Flags = MyFontReq->fo_Attr.ta_Flags;

       Success = TRUE;
     }

     FreeAslRequest (MyFontReq);
  }

  return Success;
}

static ULONG HOOKCALL SMFilterFunc (REG (a0,struct Hook *Hook),REG (a2,struct ScreenModeRequester *SMReq),REG (a1,ULONG DisplayID))
{
DisplayInfoHandle DisplayHandle;

struct DisplayInfo DisplayInfo;

struct DimensionInfo DimensionInfo;

ULONG Accept = NULL;

  DisplayHandle = FindDisplayInfo (DisplayID);

  if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DisplayInfo,sizeof (struct DisplayInfo),DTAG_DISP,DisplayID))
  {
     if (GetDisplayInfoData (DisplayHandle,(UBYTE *) &DimensionInfo,sizeof (struct DimensionInfo),DTAG_DIMS,DisplayID))
     {
        Accept = (ULONG) ((DimensionInfo.MaxDepth <= MAX_DEPTH) && (DisplayInfo.PropertyFlags & DIPF_IS_WB));
     }
  }

  return Accept;
}

BOOL SMRequest (struct ILBMInfo *Ilbm)
{
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
                              ASLSM_InitialDisplayWidth,Win->Width,
                              ASLSM_InitialDisplayHeight,Win->Height,
                              ASLSM_InitialDisplayDepth,Ilbm->wrp->BitMap->Depth,
                              ASLSM_InitialOverscanType,Ilbm->ucliptype,
                              ASLSM_InitialInfoOpened,TRUE,
                              ASLSM_InitialInfoLeftEdge,Win->LeftEdge + 310 + 15,
                              ASLSM_InitialInfoTopEdge,Win->TopEdge,
                              ASLSM_DoWidth,TRUE,
                              ASLSM_DoHeight,TRUE,
                              ASLSM_DoDepth,TRUE,
                              ASLSM_DoOverscanType,TRUE,
                              ASLSM_MinDepth,MIN_DEPTH,
                              ASLSM_MaxDepth,MAX_DEPTH,
                              ASLSM_PropertyFlags,DIPF_IS_WB,
                              ASLSM_PropertyMask,DIPF_IS_DUALPF|DIPF_IS_PF2PRI|DIPF_IS_HAM|DIPF_IS_EXTRAHALFBRITE,
                              ASLSM_FilterFunc,&SMFILTERHOOK,
                              ASLSM_MinWidth,MIN_WIDTH,
                              ASLSM_MinHeight,MIN_HEIGHT,
                              TAG_END))
    {
       Ilbm->camg = SMReq->sm_DisplayID;

       Ilbm->Bmhd.w = SMReq->sm_DisplayWidth;

       Ilbm->Bmhd.h = SMReq->sm_DisplayHeight;

       Ilbm->Bmhd.nPlanes = SMReq->sm_DisplayDepth;

       Ilbm->ucliptype = SMReq->sm_OverscanType;

       NewScreen = TRUE;
    }

    FreeAslRequest (SMReq);
 }

 return NewScreen;
}

VOID SetMenuStart (struct Window *Win)
{
  OnMenu  (Win,FULLMENUNUM (0,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,5,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,6,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,7,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,9,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,11,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,1,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (2,2,NOSUB));

  OffMenu (Win,FULLMENUNUM (2,7,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,6,NOSUB));
}

VOID SetMenuStop (struct Window *Win)
{
  OffMenu  (Win,FULLMENUNUM (0,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,5,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,6,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,7,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,9,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,11,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,1,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,6,NOSUB));

  OnMenu   (Win,FULLMENUNUM (2,7,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,6,NOSUB));
}

ULONG ProcessMenu (struct Window *Win,UWORD Code)
{
struct MenuItem *Item;

ULONG Choice = NULL;

 while (Code != MENUNULL)
 {
  Item = ItemAddress (MAINMENU,(ULONG) Code);

  switch (MENUNUM (Code))
  {
        case 0:    switch (ITEMNUM (Code))
                   {
                        case 0: Choice |= ABOUT_MSG;

                                break;

                        case 2: Choice |= SHOWGUIDE_MSG;

                                break;

                        case 4: Choice |= LOADPICTURE_MSG;

                                break;

                        case 5: Choice |= SAVEPICTURE_MSG;

                                break;

                        case 6: Choice |= LOADPALETTE_MSG;

                                break;

                        case 7: Choice |= SAVEPALETTE_MSG;

                                break;

                        case 9: Choice |= DUMP_MSG;

                                break;

                        case 11: Choice |= EXIT_MSG;

                                break;
                   }

                   break;

        case 1:    switch (ITEMNUM (Code))
                   {
                        case 0: switch (SUBNUM (Code))
                                {
                                   case 0: Choice |= TITLE_MSG;

                                           break;

                                   case 2: Choice |= TIME_MSG;

                                           break;
                                }

                                break;

                        case 1: Choice |= COORDS_MSG;

                                break;

                        case 2: switch (SUBNUM (Code))
                                {
                                   case 0: MAX_ITERATIONS = 63;

                                           break;

                                   case 1: MAX_ITERATIONS = 127;

                                           break;

                                   case 2: MAX_ITERATIONS = 191;

                                           break;

                                   case 3: MAX_ITERATIONS = 255;

                                           break;

                                   case 4: MAX_ITERATIONS = 319;

                                           break;

                                   case 5: MAX_ITERATIONS = 383;

                                           break;

                                   case 6: MAX_ITERATIONS = 447;

                                           break;

                                   case 7: MAX_ITERATIONS = 511;

                                           break;

                                   case 8: MAX_ITERATIONS = 1023;

                                           break;

                                  case 10: Choice |= ITER_MSG;

                                           break;
                                }

                                break;

                        case 3: switch (SUBNUM (Code))
                                {
                                   case  0: PRIORITY = -5;

                                            break;

                                   case  1: PRIORITY = -4;

                                            break;

                                   case  2: PRIORITY = -3;

                                            break;

                                   case  3: PRIORITY = -2;

                                            break;

                                   case  4: PRIORITY = -1;

                                            break;

                                   case  6: PRIORITY =  0;

                                            break;

                                   case  8: PRIORITY =  1;

                                            break;

                                   case  9: PRIORITY =  2;

                                            break;

                                   case 10: PRIORITY =  3;

                                            break;

                                   case 11: PRIORITY =  4;

                                            break;

                                   case 12: PRIORITY =  5;

                                            break;

                                }

                                Forbid ();

                                THISTASK = FindTask (NULL);

                                Permit ();

                                SetTaskPri (THISTASK,PRIORITY);

                                break;

                        case 4: switch (SUBNUM (Code))
                                {
                                   case 0: if (H_LINE != JHLine)
                                           {
                                              PickJuliaK (Win);

                                              H_LINE = JHLine;

                                              V_LINE = JVLine;

                                              JULIA = TRUE;

                                              SetWindowTitles (Win,(TEXT *) ~0,"Julia constant selected");

                                              Choice |= REDRAW_MSG;

                                              DisplayBeep (Win->WScreen);

                                              Delay (ONESEC);
                                           }

                                           break;

                                   case 1: if (H_LINE != MHLine)
                                           {
                                              SaveCoords (Win,TRUE);

                                              RMIN = DEF_RMIN;

                                              RMAX = DEF_RMAX;

                                              IMIN = DEF_IMIN;

                                              IMAX = DEF_IMAX;

                                              H_LINE = MHLine;

                                              V_LINE = MVLine;

                                              JULIA = FALSE;

                                              Choice |= REDRAW_MSG;
                                           }

                                           break;
                                }

                                break;
                   
                        /* Vampire selection */
                        case 5 : switch (SUBNUM(Code))
                                 {
                                    case 0: if (!optimized) {
                                                    Choice = SWITCH_CPU; 
                                                    optimized=1;
                                                    MHLine=MHLine_Vampire;
                                                    MVLine=MVLine_Vampire;
                                                    JHLine=JHLine_Vampire;
                                                    JVLine=JVLine_Vampire;  
                                                    if (JULIA)
                                                    {
                                                        H_LINE = JHLine;

                                                        V_LINE = JVLine;

                                                    }

                                                    else
                                                    {
                                                        H_LINE = MHLine;

                                                        V_LINE = MVLine;

                                                    }
                                                    ItemAddress(MAINMENU,FULLMENUNUM(1,5,0))->Flags |= CHECKED;   
                                                    ItemAddress(MAINMENU,FULLMENUNUM(1,5,1))->Flags ^= CHECKED;   
                                            
                                            
                                            }
                                            break;
                                            
                                    case 1: if (optimized) {
                                                Choice = SWITCH_CPU; 
                                                optimized=0;
                                                MHLine=MHLine_Classic;
                                                MVLine=MVLine_Classic;
                                                JHLine=JHLine_Classic;
                                                JVLine=JVLine_Classic; 
                                                if (JULIA)
                                                {
                                                    H_LINE = JHLine;

                                                    V_LINE = JVLine;

                                                }

                                                else
                                                {
                                                    H_LINE = MHLine;

                                                    V_LINE = MVLine;

                                                }
                                                ItemAddress(MAINMENU,FULLMENUNUM(1,5,0))->Flags ^= CHECKED;   
                                                ItemAddress(MAINMENU,FULLMENUNUM(1,5,1))->Flags |= CHECKED;   

                                            }
                                            break;
                                 }
                                 break;
                   }

                   break;

        case 2: switch (ITEMNUM (Code))
                {
                   case 0: Choice |= PREVIEW_MSG;

                           break;

                   case 2: Choice |= REDRAW_MSG;

                           break;

                   case 4: Choice |= (UNDO_MSG | REDRAW_MSG);

                           break;

                   case 6: Choice |= DRAW_MSG;

                           break;

                   case 7: Choice |= STOP_MSG;

                           break;
                }

                break;

        case 3: switch (ITEMNUM (Code))
                {
                   case 0: switch (SUBNUM (Code))
                           {
                              case 0: Choice |= CYCLERIGHT_MSG;

                                      break;

                              case 1: Choice |= CYCLELEFT_MSG;

                                      break;

                              case 3: Choice |= DELAY_MSG;

                                      break;

                           }

                           break;

                   case 2: Choice |= PALETTE_MSG;

                           break;

                   case 4: Choice |= NEWDISPLAY_MSG;

                           break;

                   case 6: Choice |= FONTREQ_MSG;

                           break;
                }
  }

 Code = Item->NextSelect;
 }

 return Choice;
}

VOID ProcessMouse (struct Window *Win,WORD CurMouseX,WORD CurMouseY)
{
static WORD MX1 = 0 , MY1 = 0 , MX2 = 0 , MY2 = 0, W = 0, H = 0;

struct IntuiMessage *Message;

UWORD MyCode;

LONG DefaultQueue;

ULONG MyClass;

   ModifyIDCMP (Win,IDCMP_MOUSEBUTTONS | IDCMP_MOUSEMOVE);

   DefaultQueue = SetMouseQueue (Win,10);

   if (ZMASK & MASK) DrawFrame (Win,MX1,MY1,W,H);

   MX1 = CurMouseX;

   MY1 = CurMouseY;

   W = INITIALZOOM;

   H = ScalerDiv ((LONG) W,(ULONG) Win->Height,(ULONG) Win->Width);

   DrawFrame (Win,MX1,MY1,W,H);

   while (1L)
   {
     WaitPort (Win->UserPort);

     if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
     {
        MyClass = Message->Class;

        MyCode = Message->Code;

        MX2 = Message->MouseX;

        MY2 = Message->MouseY;

        GT_ReplyIMsg ((struct IntuiMessage *) Message);

        if (MyClass == IDCMP_MOUSEMOVE)
        {
           DrawFrame (Win,MX1,MY1,W,H);

           W = abs (MX1 - MX2);

           H = abs (MY1 - MY2);

           if (W > H) (W = ScalerDiv ((LONG) H,(ULONG) Win->Width,(ULONG) Win->Height));

           else (H = ScalerDiv ((LONG) W,(ULONG) Win->Height,(ULONG) Win->Width));

           DrawFrame (Win,MX1,MY1,W,H);
        }

        else if ((MyClass == IDCMP_MOUSEBUTTONS) && (MyCode != SELECTDOWN)) break;
     }
   }

   SetMouseQueue (Win,(ULONG) DefaultQueue);

   ModifyIDCMP (Win,IDCMP_STANDARD);

   if (ZMASK & MASK)
   {
      OnMenu  (Win,FULLMENUNUM (2,0,NOSUB));

      OnMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }

   else
   {
      OffMenu  (Win,FULLMENUNUM (2,0,NOSUB));

      OffMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }
}

void PickJuliaK (struct Window *Win)
{
struct IntuiMessage *Message;

BOOL Selected = FALSE;

WORD MouseX = 0,MouseY = 0;

UWORD MyCode = 0;

ULONG MyClass = NULL;

double RealCoord=0.0,ImagCoord=0.0;

 SetWindowTitles (Win,(TEXT *) ~0,"Press left button to choose the Julia constant");

 do { WaitPort (Win->UserPort);

      if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
      {
             MyClass = Message->Class;

             MyCode  = Message->Code;

             MouseX  = Message->MouseX;

             MouseY  = Message->MouseY;

             GT_ReplyIMsg ((struct IntuiMessage *) Message);
      }

      switch (MyClass)
      {
             case    IDCMP_MOUSEMOVE: if (TMASK & MASK)
                                      {
                                         RealCoord = RMIN + ((DOUBLE) MouseX * fabs (RMAX-RMIN) / (DOUBLE) (Win->Width - 1));

                                         ImagCoord = IMAX - ((DOUBLE) MouseY * fabs (IMAX-IMIN) / (DOUBLE) (Win->Height - 1));

                                         sprintf (BAR_STRING,"Julia constant: Real %+2.15Lf Imag %+2.15Lfi",RealCoord,ImagCoord);

                                         WaitTOF ();

                                         SetWindowTitles (Win,(TEXT *) ~0,BAR_STRING);
                                      }

                                      break;

             case IDCMP_MOUSEBUTTONS: if (MyCode == SELECTDOWN)
                                      {
                                         JKRE = RealCoord;

                                         JKIM = ImagCoord;

                                         Selected = TRUE;
                                      }

                                      break;
      }

    } while (Selected != TRUE);
}

ULONG HandleEvents (struct ILBMInfo *Ilbm)
{
struct IntuiMessage *Message;

WORD MouseX = 0,MouseY = 0;

UWORD MyCode = 0;

LONG Error;

ULONG MyClass = NULL,MyMenu = NULL,Index;

double RealCoord,ImagCoord,ScrRatio,FracRatio;

 PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

 do { WaitPort (Ilbm->win->UserPort);

      if (Message = (struct IntuiMessage *) GT_GetIMsg (Ilbm->win->UserPort))
      {
             MyClass = Message->Class;

             MyCode  = Message->Code;

             MouseX  = Message->MouseX;

             MouseY  = Message->MouseY;

             GT_ReplyIMsg ((struct IntuiMessage *) Message);
      }

      switch (MyClass)
      {
             case    IDCMP_MOUSEMOVE: if (TMASK & MASK)
                                      {
                                         RealCoord = RMIN + ((DOUBLE) MouseX * fabs (RMAX-RMIN) / (DOUBLE) (Ilbm->win->Width - 1));

                                         ImagCoord = IMAX - ((DOUBLE) MouseY * fabs (IMAX-IMIN) / (DOUBLE) (Ilbm->win->Height - 1));

                                         ScrRatio  = (DOUBLE) Ilbm->win->Width / (DOUBLE) Ilbm->win->Height;

                                         FracRatio = fabs (RMAX-RMIN) / fabs (IMAX-IMIN);

                                         if (optimized) sprintf (BAR_STRING,"Vampire - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);
                                         else sprintf (BAR_STRING,"Classic - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);

                                         WaitTOF ();

                                         SetWindowTitles (Ilbm->win,(TEXT *) ~0,BAR_STRING);
                                      }

                                      break;

             case IDCMP_MOUSEBUTTONS: if (MyCode == SELECTDOWN) ProcessMouse (Ilbm->win,MouseX,MouseY);

                                      break;

             case     IDCMP_MENUPICK: MyMenu = ProcessMenu (Ilbm->win,MyCode);

                                      if (MyMenu & EXIT_MSG) break;

                                      if (MyMenu & SHOWGUIDE_MSG)
                                      {
                                         Execute (SHOWGUIDECOMMAND,NULL,NULL);

                                         break;
                                      }

                                      if (MyMenu & TITLE_MSG)
                                      {
                                         if (TMASK & MASK) MASK ^= TMASK;

                                         else MASK |= TMASK;

                                         Ilbm->TBState = TMASK & MASK;

                                         ShowTitle (Ilbm->scr,(LONG) Ilbm->TBState);

                                         break;
                                      }

                                      if (MyMenu & TIME_MSG)
                                      {
                                         if (! (TMASK & MASK)) ShowTitle (Ilbm->scr,TRUE);

                                         ShowTime (Ilbm->win,"Last calculating time:",ELAPSEDTIME);

                                         Delay (TWOSECS);

                                         if (! (TMASK & MASK)) ShowTitle (Ilbm->scr,FALSE);

                                         break;
                                      }

                                      if (MyMenu & ITER_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         while (1L)
                                         {
                                            MAX_ITERATIONS = (WORD) IntegerGad (Ilbm->win,"Iterations requester","Insert new iterations...","_Iterations",MAX_ITERATIONS + 1L) - 1L;

                                            if (MAX_ITERATIONS >= MIN_ITERATIONS) break;

                                            DisplayBeep (Ilbm->scr);

                                            MAX_ITERATIONS = MIN_ITERATIONS;
                                         }

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & ABOUT_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         About (Ilbm->win);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & PALETTE_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         if (! ModifyPalette (Ilbm->win,Ilbm->win->LeftEdge + 25,Ilbm->win->TopEdge + 35,PALETTE)) Fail (PaletteRequesterError,15L);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & CYCLERIGHT_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,IDCMP_MOUSEBUTTONS|IDCMP_RAWKEY|IDCMP_MENUPICK);

                                         if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                         ClearMenuStrip (Ilbm->win);

                                         PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                         Cycle (Ilbm->win,DELAY,SHIFTRIGHT);

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                         LoadRGB32 (Ilbm->vp,PALETTE);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & CYCLELEFT_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,IDCMP_MOUSEBUTTONS|IDCMP_RAWKEY|IDCMP_MENUPICK);

                                         if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                         ClearMenuStrip (Ilbm->win);

                                         PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                         Cycle (Ilbm->win,DELAY,SHIFTLEFT);

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                         LoadRGB32 (Ilbm->vp,PALETTE);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & DELAY_MSG)
                                      {
                                         while (1L)
                                         {
                                            DELAY = IntegerGad (Ilbm->win,"Delay requester","Insert delay time...","_Delay time",DELAY);

                                            if ((DELAY >= 0L) && (DELAY <= 100)) break;

                                            DisplayBeep (Ilbm->scr);
                                         }

                                         break;
                                      }

                                      if (MyMenu & STOP_MSG)
                                      {
                                         SetMenuStart (Ilbm->win);

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & LOADPICTURE_MSG)
                                      {
                                         if (FileRequest (Ilbm->win,"Load iff picture","FLASHMANDEL:Pictures",PICTURES_DRAWER,FALSE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               if (! (Error = QueryMandPic (Ilbm,&MANDChunkTmp,MYPATH)))
                                               {
                                                  MANDChunk = *MANDChunkTmp;

                                                  SaveCoords (Ilbm->win,TRUE);

                                                  RMIN = MANDChunk.RMin;

                                                  RMAX = MANDChunk.RMax;

                                                  IMIN = MANDChunk.IMin;

                                                  IMAX = MANDChunk.IMax;

                                                  JKRE  = MANDChunk.RConst;

                                                  JKIM  = MANDChunk.IConst;

                                                  if (MANDChunk.Special & JULIA_BIT) JULIA = TRUE;

                                                  else JULIA = FALSE;

                                                  MAX_ITERATIONS = MANDChunk.Iterations - 1L;

                                                  Fade (MYILBM.win,PALETTE,25L,1L,TOBLACK);

                                                  CloseDisplay (Ilbm,VINFO);

                                                  Ilbm->IFFPFlags = IFFPF_BESTFIT;

                                                  CURRENT_MAX_COLORS = MakeDisplay (Ilbm);

                                                  Ilbm->IFFPFlags = NULL;

                                                  if (! CURRENT_MAX_COLORS)
                                                  {
                                                     Fail (MakeDisplayError,20L);

                                                     MyMenu = EXIT_MSG;

                                                     break;
                                                  }

                                                  if (ZMASK & MASK) MASK ^= ZMASK;

                                                  CURRENT_MAX_COLORS -= RESERVED_PENS;

                                                  if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                                  if (LoadMandPic (Ilbm,MYPATH)) Fail (LoadMandPicError,5L);

                                                  if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                                  GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);

                                                  ClearMenuStrip (Ilbm->win);

                                                  if (JULIA)
                                                  {
                                                     if (ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags & CHECKED) ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags ^= CHECKED;

                                                     if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags |= CHECKED;

                                                     H_LINE = JHLine;

                                                     V_LINE = JVLine;
                                                  }

                                                  else
                                                  {
                                                     if (ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags & CHECKED) ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags ^= CHECKED;

                                                     if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags |= CHECKED;

                                                     H_LINE = MHLine;

                                                     V_LINE = MVLine;
                                                  }

                                                  for (Index = 0; Index <= 10; Index++) if (ItemAddress (MAINMENU,FULLMENUNUM (1,2,Index))->Flags & CHECKED) ItemAddress (MAINMENU,FULLMENUNUM (1,2,Index))->Flags ^= CHECKED;

                                                  switch (MAX_ITERATIONS)
                                                  {
                                                    case 63 :  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,0))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,0))->Flags |= CHECKED;

                                                               break;

                                                    case 127:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,1))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,1))->Flags |= CHECKED;

                                                               break;

                                                    case 191:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,2))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,2))->Flags |= CHECKED;

                                                               break;

                                                    case 255:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,3))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,3))->Flags |= CHECKED;

                                                               break;

                                                    case 319:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,4))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,4))->Flags |= CHECKED;

                                                               break;

                                                    case 383:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,5))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,5))->Flags |= CHECKED;

                                                               break;

                                                    case 447:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,6))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,6))->Flags |= CHECKED;

                                                               break;

                                                    case 511:  if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,7))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,7))->Flags |= CHECKED;

                                                               break;

                                                    case 1023: if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,8))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,8))->Flags |= CHECKED;

                                                               break;

                                                    default:   if (! (ItemAddress (MAINMENU,FULLMENUNUM (1,2,10))->Flags & CHECKED)) ItemAddress (MAINMENU,FULLMENUNUM (1,2,10))->Flags |= CHECKED;

                                                               break;
                                                  }

                                                  ResetMenuStrip (Ilbm->win,MAINMENU);
                                               }

                                               else Fail (QueryMandPicError,NULL);

                                               FreeIFF (Ilbm->ParseInfo.iff);
                                            }
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & SAVEPICTURE_MSG)
                                      {
                                         if (FileRequest (Ilbm->win,"Save iff picture","FLASHMANDEL:Pictures",PICTURES_DRAWER,TRUE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               MANDChunk.LeftEdge = Ilbm->win->LeftEdge;

                                               MANDChunk.TopEdge = Ilbm->win->TopEdge;

                                               MANDChunk.Width = Ilbm->win->Width;

                                               MANDChunk.Height = Ilbm->win->Height;

                                               MANDChunk.RMin = RMIN;

                                               MANDChunk.RMax = RMAX;

                                               MANDChunk.IMin = IMIN;

                                               MANDChunk.IMax = IMAX;

                                               MANDChunk.RConst = JKRE;

                                               MANDChunk.IConst = JKIM;

                                               MANDChunk.Iterations = MAX_ITERATIONS + 1L;

                                               MANDChunk.Special = (REAL_BIT | SPREAD_BIT);

                                               if (JULIA == TRUE) MANDChunk.Special |= JULIA_BIT;

                                               if (ZMASK & MASK)
                                               {
                                                  DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                                  MASK ^= ZMASK;
                                               }

                                               if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                               if (SaveMandPic (Ilbm,&SPECIAL_CHUNK,&COPYRIGHT_CHUNK,MYPATH)) Fail (SaveMandPicError,5L);

                                               if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                               FreeIFF (Ilbm->ParseInfo.iff);
                                            }
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & LOADPALETTE_MSG)
                                      {
                                         if (FileRequest (Ilbm->win,"Load palette","FLASHMANDEL:Palettes",PALETTES_DRAWER,FALSE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               if (LoadPalette (Ilbm,MYPATH)) Fail (LoadMandPalError,5L);

                                               GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);

                                               FreeIFF (Ilbm->ParseInfo.iff);
                                            }

                                            else Fail (NoMem,5L);
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & SAVEPALETTE_MSG)
                                      {
                                         if (FileRequest (Ilbm->win,"Save palette","FLASHMANDEL:Palettes",PALETTES_DRAWER,TRUE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               if (SavePalette (Ilbm,&COPYRIGHT_CHUNK,MYPATH)) Fail (LoadMandPalError,5L);

                                               FreeIFF (Ilbm->ParseInfo.iff);
                                            }

                                            else Fail (NoMem,5L);
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & FONTREQ_MSG)
                                      {
                                         if (FontRequest (Ilbm->win))
                                         {
                                            if (ZMASK & MASK)
                                            {
                                               DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                               MASK ^= ZMASK;
                                            }

                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            MYBITMAP = CopyBitMap (Ilbm->win,Ilbm->win->LeftEdge,Ilbm->win->TopEdge,Ilbm->win->Width,Ilbm->win->Height);

                                            Fade (MYILBM.win,PALETTE,25L,1L,TOBLACK);

                                            CloseDisplay (Ilbm,VINFO);

                                            if (! MakeDisplay (Ilbm))
                                            {
                                               Fail (MakeDisplayError,20L);

                                               MyMenu = EXIT_MSG;

                                               break;
                                            }

                                            PasteBitMap (MYBITMAP,Ilbm->win,Ilbm->win->Width,Ilbm->win->Height);
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         break;
                                      }

                                      if (MyMenu & DUMP_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                         if (Choice (Ilbm->win,"Print requester","Are you sure ?"))
                                         {
                                            WinDump (Ilbm->win);
                                         }

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }

                                      if (MyMenu & PREVIEW_MSG)
                                      {
                                         if (NewCoords (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4],ZOOMLINE [5]))
                                         {
                                            ModifyIDCMP (Ilbm->win,NULL);

                                            ClearMenuStrip (Ilbm->win);

                                            Preview (Ilbm->win,Ilbm->win->Width,Ilbm->win->Height);

                                            ResetMenuStrip (Ilbm->win,MAINMENU);

                                            ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                            RestoreCoords (Ilbm->win);
                                         }

                                         break;
                                      }

                                      if (MyMenu & COORDS_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         if (ShowCoords (Ilbm->win))
                                         {
                                            if (Choice (Ilbm->win,"Rendering requester","Recalculate ?")) MyMenu |= REDRAW_MSG;
                                         }

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);
                                      }

                                      if (MyMenu & UNDO_MSG) RestoreCoords (Ilbm->win);

                                      if (MyMenu & REDRAW_MSG)
                                      {
                                         if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         SetMenuStop (Ilbm->win);

                                         PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                         ELAPSEDTIME = DrawFractal (Ilbm->win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         SetMenuStart (Ilbm->win);

                                         ShowTime (Ilbm->win,"Recalculate elapsed time:",ELAPSEDTIME);

                                         break;
                                      }

                                      if (MyMenu & DRAW_MSG)
                                      {
                                         if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         if (NewCoords (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4],ZOOMLINE [5]))
                                         {
                                            MYBITMAP = CopyBitMap (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

                                            PasteBitMap (MYBITMAP,Ilbm->win,ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

                                            SetMenuStop (Ilbm->win);

                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            ELAPSEDTIME = DrawFractal (Ilbm->win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);

                                            PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                            SetMenuStart (Ilbm->win);

                                            ShowTime (Ilbm->win,"Zoom elapsed time:",ELAPSEDTIME);
                                         }

                                         break;
                                      }
                                      
                                     if (MyMenu & SWITCH_CPU) 
                                     {
                                         RealCoord = RMIN + ((DOUBLE) MouseX * fabs (RMAX-RMIN) / (DOUBLE) (Ilbm->win->Width - 1));
                                         
                                         ImagCoord = IMAX - ((DOUBLE) MouseY * fabs (IMAX-IMIN) / (DOUBLE) (Ilbm->win->Height - 1));

                                         ScrRatio  = (DOUBLE) Ilbm->win->Width / (DOUBLE) Ilbm->win->Height;

                                         FracRatio = fabs (RMAX-RMIN) / fabs (IMAX-IMIN);

                                         if (!optimized) sprintf (BAR_STRING,"Classic - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);
                                          
                                         else sprintf (BAR_STRING,"Vampire - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);
                                          
                                         SetWindowTitles (Ilbm->win,(TEXT *) ~0,BAR_STRING);
                              
                                      }

                                      break;

             case       IDCMP_RAWKEY: if (MyCode == RAW_ESC) MyMenu = EXIT_MSG;

                                      if (MyCode == HELP) Execute (SHOWGUIDECOMMAND,NULL,NULL);

                                      break;

             case  IDCMP_CLOSEWINDOW: MyMenu = EXIT_MSG;
      }

    } while (! ((MyMenu & EXIT_MSG) || (MyMenu & NEWDISPLAY_MSG)));

 return MyMenu;
}

void FreeBitMapSafety (struct BitMap *Bitmap)
{
    WaitBlit ();

    FreeBitMap (Bitmap);
}

struct BitMap *CopyBitMap (struct Window *Win,WORD Left,WORD Top,WORD Width,WORD Height)
{
struct BitMap *NewBM;

  NewBM = AllocBitMap ((ULONG) Width,(ULONG) Height,(ULONG) Win->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES,Win->RPort->BitMap);

  if (NewBM)
  {
     if (ZMASK & MASK) DrawBorder (Win->RPort,&MYBORDER,0,0);

     if (TMASK & MASK) ShowTitle (Win->WScreen,FALSE);

     BltBitMap (Win->RPort->BitMap,Left,Top,NewBM,0,0,Width,Height,0xC0,0xFF,NULL);

     if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE);

     if (ZMASK & MASK) DrawBorder (Win->RPort,&MYBORDER,0,0);

     MASK |= BMASK;
  }

  return NewBM;
}

BOOL PasteBitMap (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

        BitMapScale (&BSA);

        BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);

        FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}

LONG WinDump (struct Window *Win)
{
struct IODRPReq *IODrp;
struct MsgPort  *PrinterPort;
struct ViewPort *Vp;

LONG Error = PDERR_BADDIMENSION;

  if (! Win) return Error;

  if (PrinterPort = CreatePort (0,0))
  {
     if (IODrp = (struct IODRPReq *) CreateExtIO (PrinterPort,sizeof (struct IODRPReq)))
     {
        if (! (Error = OpenDevice ("printer.device",0,(struct IORequest *) IODrp,0)))
        {
           Vp = ViewPortAddress (Win);

           IODrp->io_Command   = PRD_DUMPRPORT;
           IODrp->io_RastPort  = Win->RPort;
           IODrp->io_ColorMap  = Vp->ColorMap;
           IODrp->io_Modes     = (ULONG) Vp->Modes;
           IODrp->io_SrcX      = Win->LeftEdge;
           IODrp->io_SrcY      = Win->TopEdge;
           IODrp->io_SrcWidth  = Win->Width;
           IODrp->io_SrcHeight = Win->Height;
           IODrp->io_DestCols  = 0;
           IODrp->io_Special   = SPECIAL_FULLCOLS | SPECIAL_ASPECT;

           Error = DoIO ((struct IORequest *)IODrp);

           CloseDevice ((struct IORequest *)IODrp);
        }

        DeleteExtIO ((struct IORequest *)IODrp);
     }

     DeletePort (PrinterPort);
  }

  return Error;
}
