
#include "fmglobals.h"

TEXT VERSION_STRING [] = "\0$VER: " XSTR(VERSION_YEAR) "." XSTR(VERSION_MONTH) "R" XSTR(VERSION_DAY) "\0";

/* common globals from boundary trace */

UBYTE *Data;
UBYTE *Done;
ULONG *Queue;
ULONG DataSize;
ULONG DoneSize;
ULONG QueueSize;
ULONG QueueHead=0, QueueTail=0;
ULONG p;
UWORD flag;

/* other globals */

struct Window* TempWin;
ULONG resx, resy;
UBYTE* screenbuffer;

#include "enum.h"

/* from main flashmandel */

struct ILBMInfo MYILBM = {0};
ULONG MASK = TMASK;
struct BitScaleArgs BSA               = {0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL};
WORD ZOOMLINE [PAIRS << 1] ;
LONG DELAY = 5L,__oslibversion = Lib_Version;
struct BitMap *MYBITMAP = NULL;

TEXT MYFILE [MAX_FILELEN],PICTURESDIR [MAX_DIRLEN],PALETTESDIR [MAX_DIRLEN];
TEXT MYPATH [MAX_PATHLEN];
TEXT *MYDIR = NULL,MYFONT [MAX_FILELEN];
struct TextFont *NEWFONT = NULL;
struct BitMap *TempBM;
UBYTE title_string[150];

/* legacy MandelChunk definition */
struct MandelChunk MANDChunk = {0},*MANDChunkTmp = NULL;

/* NewMandelChunk for Vamped Edition */

struct NewMandelChunk NewMANDChunk = {0},*NewMANDChunkTmp = NULL;

struct NewRGBMandelChunk NewRGBMANDChunk = {0},*NewRGBMANDChunkTmp = NULL;

LONG IlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_MAND, TAG_END };
LONG NewIlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_NEWMAND, TAG_END };
LONG NewRGBIlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_NEWMAND, TAG_END };

struct Chunk        COPYRIGHT_CHUNK   		= {NULL,ID_ILBM,ID_Copyright,IFFSIZE_UNKNOWN,COPYRIGHT_CHUNK_STRING/*"\n\n" VERSION COPYRIGHT_DATE " by " AUTHOR "\n\n"*/};

struct Chunk        SPECIAL_CHUNK     		= {NULL,ID_ILBM,ID_MAND,sizeof (struct MandelChunk),NULL};

struct Chunk        NEW_SPECIAL_CHUNK 		= {NULL,ID_ILBM,ID_NEWMAND,sizeof (struct NewMandelChunk),NULL};

struct Chunk        NEWRGB_SPECIAL_CHUNK 	= {NULL,ID_ILBM,ID_NEWMAND,sizeof (struct NewRGBMandelChunk),NULL};

UWORD PENS [] = {BLACK,DARK_GREY,WHITE,WHITE,DARK_GREY,LIGHT_GREY,DARK_GREY,LIGHT_GREY,DARK_GREY,WHITE,LIGHT_GREY,DARK_GREY,(UWORD) ~0};


struct Border       MYBORDER          = {0,0,0,0,COMPLEMENT,PAIRS,ZOOMLINE,0};

ULONG detected_system=NOT_RUNNING_ON_AROS;
BOOL JULIA = FALSE;
ULONG ELAPSEDTIME = NULL,CURRENT_MAX_COLORS;
UWORD ITERATIONS;
WORD MAX_ITERATIONS = 319,  BUDDHA_MIN_ITERATIONS = 0, RETURNVALUE = 0;
volatile double FACTOR=0.0,INCREMREAL=0.0,INCREMIMAG=0.0,CRE=0.0,CIM=0.0,JKRE=0.0,JKIM=0.0;
double RMIN=INIT_DEF_RMIN,RMAX=INIT_DEF_RMAX,IMIN=INIT_DEF_IMIN,IMAX=INIT_DEF_IMAX;
double DEF_RMIN,DEF_RMAX,DEF_IMIN,DEF_IMAX,DEF_JKRE=INIT_DEF_JKRE,DEF_JKIM=INIT_DEF_JKIM,URMIN=0.0,URMAX=0.0,UIMIN=0.0,UIMAX=0.0,UJKRE=0.0,UJKIM=0.0;
volatile double px1,py1,px2,py2,px3,py3,px4,py4;

LONG PRIORITY = DEF_STARTPRI;
struct Task *THISTASK = NULL;
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

BOOL DEPTH_CHANGED;
struct RastPort TempRP;
struct Menu *MAINMENU = NULL;

ULONG MultiPower=3;
double MultiPowerFloat=3;

UBYTE DrawOrbits=0;
UBYTE PeriodicityColoring=0;

UWORD BaseFractalType;
UWORD FractalType;
double GlobalPowerVar;

UBYTE *PixelLine;

ULONG* BuddhaCounter=NULL;
TEXT BAR_STRING [BARLEN];
CPTR *VINFO = NULL;

/* arrays with saved orbits for periodicity checking */
double *re;
double *im;

/* variables for optimized routines */
volatile UWORD IterP1;
volatile UWORD IterP2;
volatile UWORD IterP3;
volatile UWORD IterP4;
double FinalXP1, FinalXP2, FinalXP3, FinalXP4;
double FinalYP1, FinalYP2, FinalYP3, FinalYP4;
double GlobalP;

UBYTE ANTIBUDDHA=FALSE;
UBYTE BUDDHA_USE_SYMMETRY=1;
double BUDDHA_BOUNDARY_RANDOMIZATION=0;
UBYTE BUDDHA_RANDOMIZATION_FREQUENCY=0; /* 1<<0 = 1 => every second point is random = 50% */
UBYTE BUDDHA_RESUME=0;

ULONG NEBULA_WIDTH=0;
ULONG NEBULA_HEIGHT=0;
UBYTE SMOOTH_COLORING=0;

ULONG FM_REDITER, FM_GREENITER, FM_BLUEITER;
ULONG FM_REDMIN, FM_GREENMIN, FM_BLUEMIN;

UBYTE SHOW_MAXCOUNTERS;

/* Bailout value must be 4 for backwards compatibility.
 * For smooth coloring a large bailout value (e.g. 4000000000)
 * is better! Solution: Bring up a requester when smooth coloring
 * is used, so that the user can choose a bailout value.
 */
 
double BAILOUT_VALUE=4; 

/* variable for vampire detection and selection between vampire and classic */
UBYTE vampire_type;
UBYTE optimized;
UBYTE vampire;
UBYTE algorithm, algorithm_buddha;

ULONG histogram_save=1;
ULONG histogram_compression=1; 		/* 0 = raw / 1 = RLE */

ULONG ScrCpySize;
struct BitMap* ScrCpyBuffer=NULL;

/* fractal function pointers */

VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);
/* pointers to accelerated / classic functions */
VOID (*MVLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*MHLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*JVLine) (struct RastPort *,const LONG,const LONG,const LONG);
VOID (*JHLine) (struct RastPort *,const LONG,const LONG,const LONG);
void   JVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   JHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   MVLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   MHLine_Classic        (struct RastPort *,const LONG,const LONG,const LONG);
void   JVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   JHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   MVLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
void   MHLine_Vampire        (struct RastPort *,const LONG,const LONG,const LONG);
ULONG  DrawFractalTiling   (struct Window *,const LONG,const LONG,const LONG,const LONG);
ULONG  DrawMandelFractalBrute   (struct Window *,const LONG,const LONG,const LONG,const LONG);
ULONG (*DrawFractal) (struct Window *,const LONG,const LONG,const LONG,const LONG);
ULONG DrawMandelFractalBoundaryVampire (struct Window *,const LONG,const LONG,const LONG,const LONG);

/* General function pointers */

ULONG (*MultiMandelInCGeneric)(ULONG, double, double);
ULONG (*MultiJuliaInCGeneric)(ULONG,double,double);
ULONG (*BurningShipInCGeneric)(ULONG,double,double);
ULONG (*FractalInCGeneric)(ULONG,double,double);
ULONG (*FractalInCGenericStoreIterations)(ULONG,double,double);
void (*PutPixelWithColorMode)(ULONG, ULONG, ULONG);
BOOL (*PasteBitMap) (struct BitMap *,struct Window *, WORD, WORD);

/* from drawing.h */

ULONG DD_WIDTH, DD_HEIGHT;
UBYTE* DD_SCREENBUFFER;
ULONG DD_PIXFMT, DD_BPP=1, DD_OLDBPP=0xff;
ULONG DD_SRC_BPP, DD_SRC_PIXFMT, DD_DST_BPP, DD_DST_PIXFMT;
ULONG DD_SRC_RED, DD_SRC_GREEN, DD_SRC_BLUE;
ULONG DD_ARGB, DD_RED, DD_GREEN, DD_BLUE, DD_ALPHA;
ULONG DD_BKGRD=0; 
struct RastPort* DD_RPORT=NULL;
struct Window* DD_WIN=NULL;
UBYTE drawing;
UBYTE switch_back_to_direct=FALSE;

/* ILBM */

LONG IlbmCollects [] = { TAG_END };

LONG IlbmStops [] = { ID_ILBM, ID_BODY, TAG_END };

/* requester */

struct EasyStruct NotYetImplemented =
{
	sizeof(struct EasyStruct),
	0,
	"Not Yet Implemented",
	"This feature has not yet been implemented!\n",
	"That's a pitty!|Ok (Vampire rulez anyway ... :)",
};

struct TextAttr     MYFONTSTRUCT      = {DEF_FONTNAMESTR DEF_FONTEMPTYSPC,DEF_FONTSIZE,FS_NORMAL,FPF_DISKFONT};

struct NewGadget    TEXTGAD           = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

struct NewGadget    BUTTONGAD         = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

struct NewGadget    CHECKBOXGAD       = {0,0,0,0,0,&MYFONTSTRUCT,0,0,0,0};

/* global color palettes */

ULONG *PALETTE;

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
                       
/* global functions */

UWORD CheckRMB(void)
{
	return (UWORD)(!(*(UWORD*)0xDFF016 & (1<<10)));	/* POTGOR */
}

UWORD CheckLMB(void)
{
    return (UWORD)(!(*(UBYTE*)0xBFE001 & (1<<6)));		/* CIAPRA */
}

UWORD InterruptDrawing(struct Window* Win, const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
struct IntuiMessage *Message;

UWORD MyCode;

ULONG MyClass;  

  if (Win->UserPort->mp_SigBit)
  {
     if (CheckRMB()) DisableDirectDrawing();
     if (CheckLMB()) EnableDirectDrawing();
     
     if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
     {
        MyClass = Message->Class;

        MyCode  = Message->Code;
		/*printf("MyClass: %d MyCode: %d\n", MyClass, MyCode);*/
		
        GT_ReplyIMsg ((struct IntuiMessage *) Message);

        
		switch (MyClass)
        {
          /*case IDCMP_MENUPICK    : if (MyCode != MENUNULL)
                                   {
                                      if (ProcessMenu (Win,MyCode) & STOP_MSG) return TRUE;
                                   }
		*/
                                   break;

          case IDCMP_RAWKEY      : if (MyCode == RAW_ESC) return TRUE; 
          						   else return FALSE;
                                   break;
          
          
          /* if (MyCode == TAB)
                                   {
                                      BlinkRect (Win,a1,b1,a2,b2);

                                      return FALSE;
                                   }
*/
/*                                   if (MyCode == RAW_ESC)
                                   {
                                      DisplayBeep (Win->WScreen);

                                      return TRUE;
                                   }

                                   break;
*/
          
          case IDCMP_CLOSEWINDOW : return TRUE; 
		  
		  						   break;
								   
		  default : 
          
          			if ((switch_back_to_direct) && (MyClass==IDCMP_MENUPICK))
                    {
                    	/*if (MyCode!=MENUNULL)
                        {
                           if (ProcessMenu (Win,MyCode) & STOP_MSG) return TRUE;
                           else return FALSE;
                        }*/
                        EnableDirectDrawing();
                        return FALSE;
                    } else return FALSE;
        }
     } 
  }

}

void FunctionIsOnly24Bit(struct Window *Win)
{
	static struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"ColorMode Requester", 
	 "Function is only available\nin true color mode.\n(Select a 24bit screenmode first.)\n","Ok" };

	EasyRequest (Win,&MyReq,0);
}

void OrbitsNotPossible(struct Window *Win)
{
	static struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Orbits requester", 
	 "Orbits can only be selected\non main (default) view of \nMandelbrot fractal.\n","Ok" };

	EasyRequest (Win,&MyReq,0);
}

void SetDefaultCoordinates(void) 
{
	RMIN = DEF_RMIN;
    RMAX = DEF_RMAX;
    IMIN = DEF_IMIN;
    IMAX = DEF_IMAX;
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

void OrbitsActivated(struct Window *Win)
{
	static struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Orbits requester", 
	 "Orbits have been activated\n(Press ESC to stop.)\n","Ok" };

	EasyRequest (Win,&MyReq,0);
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

                            case IDCMP_VANILLAKEY    : switch (/*ToUpper (*/(ULONG) MyCode)/*)*/
                                                       {
                                                              case 'd' :
                                                              case 'D' :

                                                              case 'i' :
                                                              case 'I' : ActivateGadget (MyIntGad,GadWin,0);

                                                                         break;

                                                              case 'a' :
                                                              case 'A' : Var = ((struct StringInfo *) MyIntGad->SpecialInfo)->LongInt;

                                                                         Exit = TRUE;

                                                                         break;

                                                              case 'c' :
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

                                                               case CANCEL  : Var = 0xffffffff; Exit = TRUE;
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

double FloatGad (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr,TEXT *TxtString,double Var)
{
struct IntuiMessage *Message;
struct Gadget *GadList = NULL,*MyIntGad,*MyButtonGad,*MyGad;
struct Window *GadWin;
double result=0;
char startvalue[30];
BOOL Exit = FALSE;
UWORD MyCode;
ULONG MyClass;

  /* convert number to string */
  if ((ULONG)Var==Var) sprintf(startvalue, "%u", (ULONG)Var);
  else sprintf(startvalue, "%f", Var);
  
  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyIntGad = CreateContext (&GadList);

  TEXTGAD.ng_LeftEdge     = 170;

  TEXTGAD.ng_TopEdge      = 20;

  TEXTGAD.ng_Width        = 90;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = TxtString;

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  MyIntGad = CreateGadget (STRING_KIND,MyIntGad,&TEXTGAD,GTST_String,startvalue,GTST_MaxChars,12,STRINGA_ReplaceMode,TRUE,GT_Underscore,'_',TAG_END);

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
							/* due to linking problems avoid function ToUpper() - normally: string.h */
                            case IDCMP_VANILLAKEY    : switch (/*ToUpper (*/(ULONG) MyCode)/*)*/
                                                       {
                                                              case 'D' :
                                                              case 'd' :
                                                              case 'i' :

                                                              case 'I' : ActivateGadget (MyIntGad,GadWin,0);

                                                                         break;
                                                              case 'a' :
                                                              case 'A' : Var = ((struct StringInfo *) MyIntGad->SpecialInfo)->LongInt;

                                                                         Exit = TRUE;

                                                                         break;
                                                              case 'c' : 

                                                              case 'C' : Exit = TRUE;

                                                                         break;

                                                           case VAN_ESC: Exit = TRUE;
                                                       }

                                                       break;

                            case IDCMP_GADGETUP      : switch (MyGad->GadgetID)
                                                       {
                                                               case ACCEPT  : /*strfloat=((struct StringInfo*)MyIntGad->SpecialInfo)->Buffer; */
																			  result=atof(((struct StringInfo*)MyIntGad->SpecialInfo)->Buffer);
                                                                              Exit = TRUE;

                                                                              break;

                                                               case CANCEL  : result = 0xffffffff; Exit = TRUE;
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

  /* return Var; */
  return result;
}


BOOL CheckBox (struct RastPort *Rp,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{

const LONG Color = GetPixelPenGeneric (Rp,a1,b1);
LONG c1, c2, c3;

/*REGISTER*/ LONG Var;

  if (Color==0xffffffff) return FALSE; /* this is a quick fix for now for 24/32 bit screens */

  if (Color != (c1=GetPixelPenGeneric (Rp,a2,b2))) return FALSE;

  if (Color != (c2=GetPixelPenGeneric (Rp,a2,b1))) return FALSE;

  if (Color != (c3=GetPixelPenGeneric (Rp,a1,b2))) return FALSE;

  for (Var = (a1 + 1L); Var < a2; Var++)
  {
      if (Color != GetPixelPenGeneric (Rp,Var,b1)) return FALSE;

      if (Color != GetPixelPenGeneric (Rp,Var,b2)) return FALSE;
  }

  for (Var = (b1 + 1L); Var < b2; Var++)
  {
      if (Color != GetPixelPenGeneric (Rp,a1,Var)) return FALSE;

      if (Color != GetPixelPenGeneric (Rp,a2,Var)) return FALSE;
  }

/*  printf("CheckBox(%d,%d,%d,%d) Color: %d (c1=%d c2=%d c3=%d) => CHECKPOT!\n",
  	a1,b1,a2,b2, Color, c1, c2, c3);
*/
/*  
  SetAPen(Rp,50);
  Move(Rp,a1+1,b1);
  Draw(Rp,a2-1,b1);
  Move(Rp,a2,b1+1);
  Draw(Rp,a2,b2-1);
  Move(Rp,a2-1,b2);
  Draw(Rp,a1+1,b2);
  Move(Rp,a1,b2-1);
  Draw(Rp,a1,b1+1);
*/
  
  return TRUE;

/* 
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
*/
}


ULONG Fail (UBYTE *ErrorString,ULONG ErrorLevel)
{
  DisplayBeep (NULL);

  fputs (ErrorString,stderr);

  RETURNVALUE = ErrorLevel;

  return ErrorLevel;
}

void SetRGBIterations(void)
{
	FM_REDITER = MAX_ITERATIONS+1;
  	FM_GREENITER = FM_REDITER / 4;
  	FM_BLUEITER = FM_REDITER / 40;
		
	BUDDHA_MIN_ITERATIONS = min(FM_REDITER - FM_GREENITER - 1, 20);
	
	FM_REDMIN = FM_GREENITER;
	FM_GREENMIN = FM_BLUEITER;
	FM_BLUEMIN = min(BUDDHA_MIN_ITERATIONS,FM_BLUEITER/2);
}

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

VOID ShowTime (struct Window *Win,TEXT *String,ULONG Secs)
{
  sprintf (BAR_STRING,"%s %uh %um %us - Average speed: %.1Lf Pix/sec",String,Secs / 3600L,(Secs / 60L) % 60L,Secs % 60L,(double) ((Win->Width - Win->LeftEdge) * (Win->Height - Win->TopEdge)) / (double) Secs);

  SetWindowTitles (Win,(TEXT *) ~0,BAR_STRING);
}

/* printer menu disactivated */
/*
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
*/
