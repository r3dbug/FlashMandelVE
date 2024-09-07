
#include "fmglobals.h"

TEXT VERSION_STRING [] = "\0$VER: " XSTR(VERSION_YEAR) "." XSTR(VERSION_MONTH) "R" XSTR(VERSION_DAY) "\0";

/* common globals from boundary trace */

UBYTE *Data=NULL;
UBYTE *Done=NULL;
ULONG *Queue=NULL;
ULONG DataSize;
ULONG DoneSize;
ULONG QueueSize;
ULONG QueueHead=0, QueueTail=0;
ULONG p;
UWORD flag;

ULONG* edgequeue=0;
ULONG* lemniscates=0;
ULONG  queue_i=0;
ULONG  lemni_i=0;
ULONG FMG_LAST_QUEUE_WIDTH=0;
ULONG FMG_LAST_QUEUE_HEIGHT=0;
ULONG FMG_BUDDHA_ZOOM_IN=FALSE; 

/* other globals */

struct Window* TempWin;
ULONG resx, resy;
UBYTE* screenbuffer;
struct BitMap *TmpBM=NULL;

#include "enum.h"

/* from main flashmandel */

struct ILBMInfo MYILBM = {0};
ULONG MASK = TMASK;
struct BitScaleArgs BSA = {0,0,0,0,0,0,0,0,0,0,0,0,NULL,NULL,NULL,0,0,NULL,NULL};
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
struct MandelChunk MANDChunk 				= {0},*MANDChunkTmp = NULL;

/* NewMandelChunk for Vamped Edition */
struct NewMandelChunk NewMANDChunk 			= {0},*NewMANDChunkTmp = NULL;
struct NewRGBMandelChunk NewRGBMANDChunk 	= {0},*NewRGBMANDChunkTmp = NULL;

LONG IlbmProps [] 		= { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_MAND, TAG_END };
LONG NewIlbmProps [] 	= { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_NEWMAND, TAG_END };
LONG NewRGBIlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, ID_ILBM, ID_NEWMAND, TAG_END };

struct Chunk        COPYRIGHT_CHUNK   		= {NULL,ID_ILBM,ID_Copyright,IFFSIZE_UNKNOWN,COPYRIGHT_CHUNK_STRING/*"\n\n" VERSION COPYRIGHT_DATE " by " AUTHOR "\n\n"*/};

struct Chunk        SPECIAL_CHUNK     		= {NULL,ID_ILBM,ID_MAND,sizeof (struct MandelChunk),NULL};

struct Chunk        NEW_SPECIAL_CHUNK 		= {NULL,ID_ILBM,ID_NEWMAND,sizeof (struct NewMandelChunk),NULL};

struct Chunk        NEWRGB_SPECIAL_CHUNK 	= {NULL,ID_ILBM,ID_NEWMAND,sizeof (struct NewRGBMandelChunk),NULL};

BOOL LEGACY_PICTURE = FALSE;

UWORD PENS [] = {BLACK,DARK_GREY,WHITE,WHITE,DARK_GREY,LIGHT_GREY,DARK_GREY,LIGHT_GREY,DARK_GREY,WHITE,LIGHT_GREY,DARK_GREY,(UWORD) ~0};


struct Border       MYBORDER          = {0,0,0,0,COMPLEMENT,PAIRS,ZOOMLINE,0};

ULONG detected_system=NOT_RUNNING_ON_AROS;
BOOL FM_APOLLOOS=FALSE;
BOOL JULIA = FALSE;
double ELAPSEDTIME = NULL;
double ACCUMULATED_BUDDHA_TIME = 0;
ULONG FM_PROCESSOR_INFO=0;
ULONG CURRENT_MAX_COLORS;
ULONG ITERATIONS;
ULONG MAX_ITERATIONS = 319,  BUDDHA_MIN_ITERATIONS = 0, RETURNVALUE = 0;
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
/* (older versions of) AROS / ApolloOS can't create / free interleaved bitmpas, so use specific flags */
ULONG FM_ALLOCTEMPRAST_FLAGS=0;

UBYTE FM_BUSY_MOUSE_POINTER=BUSY_POINTER;
BOOL FM_FORBID_MENUS=TRUE;		/* forbid menus during calculation when in direct drawing mode */
BOOL FM_FORBID_SYMMETRY=TRUE;  /* forbid symmetry when in system-friendly drawing */
BOOL FM_FPUCHECK=TRUE;
BOOL FM_FPUWARNING_1ST=TRUE;

BOOL FM_NO32BITMODES=TRUE;
BOOL FM_NO24BITMODES=FALSE;
BOOL FM_NO8BITMODES=FALSE;
BOOL FM_NOPALNTSCMODES=TRUE;

ULONG MultiPower=3;
double MultiPowerFloat=3;

UBYTE DrawOrbits=0;
UBYTE FM_ORBIT_COLOR=50;
BOOL FM_ORBIT_1ST=TRUE;
UBYTE PeriodicityColoring=0;

BOOL FM_ANTIBUDDHA_1ST=TRUE;

UWORD BaseFractalType;
UWORD FractalType;
double GlobalPowerVar;

UBYTE *PixelLine;

ULONG* BuddhaCounter=NULL;
ULONG* HAMMap=NULL; /* Hit&Miss-Map */
BOOL HITMAP_DURING_CALCULATION=FALSE;
BOOL HITMAP_AT_THE_END=FALSE;
UBYTE HITMAP_VISUALIZATION=0; /* 0: lores, 1: hires */
ULONG HITMAP_EXTENSION=8;
ULONG HITMAP_SHIFT=2;
ULONG HITMAP_MEMSIZE=0;
ULONG HITMAP_COLORS[4] = { 0x000000ff, 0x00000088, 0x0000ff00, 0x00ff0000 }; /* edges - lemniscates - undefined (hits) - miss */

ULONG BUDDHA_DIMENSIONS[2];		/* actual width / height of BuddhaCounter & HAMMAP */

TEXT BAR_STRING [BARLEN];
CPTR *VINFO = NULL;

/* variables for optimized routines */
volatile UWORD IterP1;
volatile UWORD IterP2;
volatile UWORD IterP3;
volatile UWORD IterP4;

double FinalXP1, FinalXP2, FinalXP3, FinalXP4;
double FinalYP1, FinalYP2, FinalYP3, FinalYP4;

double GlobalP;

UBYTE ANTIBUDDHA=FALSE;
UBYTE USE_SYMMETRY=1;
UBYTE FM_ORIGINAL_USE_SYMMETRY=1;
BOOL BUDDHA_AUTO_CENTER=TRUE;
double BUDDHA_AUTO_CENTER_EPSILON=0.03;
double BUDDHA_AUTO_CENTER_PERCENTAGE=1;
UBYTE USE_PERIODICITY=0;
UBYTE USE_HAMMAP=NULL;
double PERIODICITY_EPSILON=0.00001;

UBYTE BUDDHA_RESUME=0;

BOOL BUDDHA_RANDOM=TRUE;
BOOL BUDDHA_RANDOM_HAMMAP;
BOOL BUDDHA_EDGES=1;
BOOL BUDDHA_LEMNISCATES=1;

ULONG NEBULA_WIDTH=0;
ULONG NEBULA_HEIGHT=0;
UBYTE SMOOTH_COLORING=0;

ULONG RGB_ITER[6];

double BUDDHA_RANDOMIZE_X=0.71, BUDDHA_RANDOMIZE_Y=0.71;
ULONG BUDDHA_MAX_LOOPS=0; /* 0 = infinite loop */
ULONG FM_MAX_CALC_SECS=0; /* 0 = infinite */
/*BOOL PERIODICITY_COLORING = TRUE;*/

double NEBULA_SAMPLE_RE;
double NEBULA_SAMPLE_IM;
ULONG NEBULA_SAMPLE_X;
ULONG NEBULA_SAMPLE_Y;

BOOL LoadOnlyData=FALSE;
 
UBYTE SHOW_MAXCOUNTERS;

BOOL BOUNDARY_FILL=TRUE;

/* Bailout value must be 4 for backwards compatibility.
 * For smooth coloring a large bailout value (e.g. 4000000000)
 * is better! Solution: Bring up a requester when smooth coloring
 * is used, so that the user can choose a bailout value.
 */
 
double BAILOUT_VALUE=4; 
BOOL FM_SMOOTH_COLORING_BAILOUT_1ST=TRUE;
ULONG algorithm_before_smooth_coloring=0xffffffff; /* 0xffffffff == invalid */
double bailout_before_smooth_coloring=4;

/* arrays with saved orbits for periodicity checking (plus copies for security) */
double *re; 
double *im; 

/* variable for vampire detection and selection between vampire and classic */
UBYTE vampire_type;
UBYTE optimized;
UBYTE vampire;

/*
ULONG dcache_hits_start, dcache_hits_end;
ULONG icache_hits_start, icache_hits_end;
ULONG dcache_miss_start, dcache_miss_end;
ULONG icache_miss_start, icache_miss_end;
*/

UBYTE algorithm, algorithm_buddha;

ULONG histogram_save=1;
BOOL USE_HISTOGRAM_AUTOSAVE=TRUE; 
ULONG HISTOGRAM_AUTOSAVE_TIME=20*60; /* seconds for autosave */ 
ULONG histogram_compression=1; 		/* 0 = raw / 1 = RLE */

ULONG ScrCpySize;
struct BitMap* ScrCpyBuffer=NULL;

/* global buffers */
UBYTE tb[]="Ibqqz!Fbtufs\"";
UBYTE mb[]="Xf!nbef!Bnjhb-!~Uifz!gvdlfe!ju!vq/~~Mpoh!mjwf!Bnjhb/~Cvz!b!Wbnqjsf\"";
UBYTE tb1[40];
UBYTE mb1[200];

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
UWORD TITLE_HIDE_DELAY=0;

/* new screen */

#define FM_NEWSCR_BMCPY		1
#define FM_NEWSCR_RECALC	2

UBYTE FM_NEWSCR_CPY	= FM_NEWSCR_BMCPY; 		/* bitmap copy when new screen is selected */

/* ILBM */

LONG IlbmCollects [] = { TAG_END };

LONG IlbmStops [] = { ID_ILBM, ID_BODY, TAG_END };

/* requester */

struct EasyStruct V2Requester3OperantFPU =
{
	sizeof(struct EasyStruct),
	0,
	"V2 3-operant instructions",
	"It seems that your V2 uses a core\nthat does not yet support\n3-operant FPU instructions.\n(Core >=2.18 needed)\n\nSelecting Vampire-specific code\nmay make your Amiga freeze or crash.",
	"Continue anyway|Use classic m68k instead",
};

struct EasyStruct NotYetImplemented =
{
	sizeof(struct EasyStruct),
	0,
	"Not Yet Implemented",
	"This feature has not yet been implemented!\n",
	"That's a pitty!|Ok (Vampire rulez anyway ... :)",
};

struct EasyStruct ExperimentalFunction =
{
	sizeof(struct EasyStruct),
	0,
	"Experimental function",
	"This function is experimental.\nIt might not be fully working.\n",
	"Ok",
};

struct EasyStruct NoMemory4Buddha =
{
	sizeof(struct EasyStruct),
	0,
	"Out of Memory",
	"Couldn't allocate memory for Buddha ... :-(\n(Try a lower resolution.)",
	"Ok",
};

UBYTE screenmodestr[400];

struct EasyStruct ShowScreenMode =
{
	sizeof(struct EasyStruct),
	0,
	"Screenmode",
	screenmodestr,
	"Ok",
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

CHIP UWORD NoPointer[] =   {0x0000,0x0000,0x0100,0x0000,0x0100,0x0000,0x0000,0x0100,
                            0x0000,0x0100,0x0100,0x0100,0x0100,0x0100,0x0000,0x0000,
                            0xCC66,0x3C78,0x0000,0x0000,0x0100,0x0100,0x0100,0x0100,
                            0x0000,0x0100,0x0000,0x0100,0x0100,0x0000,0x0100,0x0000,
                            0x0000,0x0000};


/* periodicity prime number coloring antinebula */

ULONG PRIME_RED=2, 
      PRIME_GREEN=3,
      PRIME_BLUE=5,
      PRIME_DIVIDER=3;
                         
/* global functions */

struct Task *launched_task;

BOOL LaunchedFromShell(void)
{
    if (launched_task=FindTask(NULL))
    {
        if (((struct Process*)launched_task)->pr_TaskNum!=0) return TRUE;
        else return FALSE;
    } 
}

ULONG CCD (ULONG s, ULONG e) /* CCD = cache counter difference */
{
	if (e<s) return (0xffffffff-s) + (e);
    else return e-s;
}

UWORD CheckRMB(void)
{
	return (UWORD)(!(*(UWORD*)0xDFF016 & (1<<10)));	/* POTGOR */
}

UWORD CheckLMB(void)
{
    return (UWORD)(!(*(UBYTE*)0xBFE001 & (1<<6)));		/* CIAPRA */
}

double GetTime(void) {
    ULONG start, stop, difference;
	ULONG frequency=(*((UBYTE*)(0xDFF3FC+1)))*7.09379*1000000; 		 /* read clock multiplyer */
    start=GetStart();	
    stop=GetStop();
    difference=(stop>start) ? (stop-start) : (0xffffffff-(start-stop));
	return (double)difference/frequency;
}

void Shift(UBYTE* s, UBYTE* d)
{
	ULONG i=0;
    while (*(s+i)!=0) 
    {
        switch (*(s+i))
    	{
          case '~' : d[i] = '\n'; break;
          default :	
        	d[i]=*(s+i)-1;
    	}	
        i++;
    }
	d[i]=0;
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

BOOL V2FPUCHECK(void)
{
	LONG r;	
    if (FM_FPUCHECK) 
    {
    	r=Has3OperantFPU();
    	if (r) return TRUE;
        else return FALSE;
    }
    else return TRUE;
}

double DrawFractalWrapper(struct Window *Win,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
	double calculation_time=0;
    ULONG oldflags;
    
	ULONG StartSec = NULL , EndSec = NULL , StartMicro, EndMicro;	
	
 	INCREMREAL = (fabs (RMAX-RMIN)) / ((double) (a2 - a1 + 1));

 	INCREMIMAG = (fabs (IMAX-IMIN)) / ((double) (b2 - b1 + 1));

 	ITERATIONS = MAX_ITERATIONS + 1;
    
    OffTitle(MYILBM.scr);
    Delay(TITLE_HIDE_DELAY);
    
    /* disable menus when in direct drawing mode */
    oldflags=Win->Flags;
    if ((drawing==DRAW_DIRECT) && (FM_FORBID_MENUS))
    {
        Forbid();
    	Win->Flags^=WFLG_RMBTRAP;
    	Permit();
    }
    
    CurrentTime (&StartSec,&StartMicro);
	if (vampire_type) SetStart();

    DrawFractal(Win, a1, b1, a2, b2);
            
    if (vampire_type) SetStop();	    
    CurrentTime (&EndSec,&EndMicro);
	
 	DisplayBeep (Win->WScreen);
    
    /* restore flags (if menus have been disabled,
     * otherwise this has no effect)
     */
	Forbid();  
   	Win->Flags=oldflags; 
   	Permit();  

	/* if (TMASK & MASK) ShowTitle (Win->WScreen,TRUE); */

	if ((vampire_type) && (UseCCCTiming(EndSec-StartSec))) calculation_time=GetTime();   
    else calculation_time=(double)(EndSec-StartSec);
    
    if (FractalType==BUDDHA) { 
        calculation_time+=ACCUMULATED_BUDDHA_TIME; 
    	ACCUMULATED_BUDDHA_TIME=calculation_time;
    } 

	OnTitle(MYILBM.scr);
    
    return calculation_time;
}


UWORD InterruptDrawing(struct Window* Win, const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
struct IntuiMessage *Message;

UWORD MyCode;

ULONG MyClass;  

  if (Win->UserPort->mp_SigBit)
  {
     if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
     {
        MyClass = Message->Class;

        MyCode  = Message->Code;
		
        GT_ReplyIMsg ((struct IntuiMessage *) Message);

        /* keep only ESC key to react
         * during a calculation.
         */
		switch (MyClass)
        {
          case IDCMP_RAWKEY      : if (MyCode == RAW_ESC) return TRUE; 
          						   else return FALSE;
        
                                   break;
          
          case IDCMP_CLOSEWINDOW : return TRUE; 
		  
		  						   break;
								   
		  default : 
          	        return FALSE;
        }
     } 
  }
}

const UBYTE *fractal_names[10] = { 
							"NONE",				    
                            "Mandelbrot",
    						"Multibrot",
    						"Burning Ship"
                           };
                           
const UBYTE* GetFractalName(ULONG f)
{
	switch (f) 
    {	
		case MANDEL :  return fractal_names[1]; break;
        case MULTIINT :
        case MULTIFLOAT : return fractal_names[2]; break;
        case BURNINGSHIP: return fractal_names[3]; break;
        default:
        	return fractal_names[0]; 
    }
}

char InfoBuffer[500];

void ShowPicInfo(struct Window *Win)
{
	ULONG cpu,y,m,d;
    
    struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Info", InfoBuffer,"Ok" };

	if ((NewMANDChunk.Version==0) && (!LEGACY_PICTURE))  
    { 
    	if (FractalType==BUDDHA) 
        {	
        	if (ANTIBUDDHA)
            {
    			sprintf(InfoBuffer, "Type: %s (Antinebula)\nIter: %ux\nTime: %.2f secs", 
        			GetFractalName(BaseFractalType), MAX_ITERATIONS+1, ELAPSEDTIME); 
    		} 
            else
            {
            	sprintf(InfoBuffer, "Type: %s (Nebula)\nIter: %ux\nTime: %.2f secs", 
        			GetFractalName(BaseFractalType), MAX_ITERATIONS+1, ELAPSEDTIME); 
            }	
        }
    	else
        {
            sprintf(InfoBuffer, "Type: %s\nIter: %ux\nTime: %.2f secs", 
        		GetFractalName(BaseFractalType), MAX_ITERATIONS+1, ELAPSEDTIME); 
		}	
        
        if (JULIA)
        {
        	sprintf(InfoBuffer, "Type: %s (Julia)\nIter: %ux\nTime: %.2f secs", 
        			GetFractalName(BaseFractalType), MAX_ITERATIONS+1, ELAPSEDTIME); 
        }
    }	
    else 
    {
    	if (LEGACY_PICTURE)
        	sprintf(InfoBuffer, "File: %s\nFrom: Legacy FlashMandel\n",MYFILE); 
        else
        {
        	y=GetYear(NewMANDChunk.Version);
            m=GetMonth(NewMANDChunk.Version);
            d=GetDay(NewMANDChunk.Version);
            
            if ((y>=24) && (m>=8) && (d>1))
            {
            	/* new processor and time info 
                 * from version 24.8 R1 on 
                 */
            	cpu=FM_PROCESSOR_INFO&0xffffff;
            	if (cpu==68080) 
            	{		
            		if (ELAPSEDTIME!=0)
                    {		
             	       sprintf(InfoBuffer, "File: %s\nFrom: FMVE %u.%u (R%u)\nCPU:  %lu (%s)\nTime: %1.2f secs\n",MYFILE, 
        					y, m, d, 
            			    cpu, 
            		        GetVampireString(FM_PROCESSOR_INFO>>24), 
            		        ELAPSEDTIME
            			    ); 
                    }
                    else
                    {
                    	/* don´t show time */
                        sprintf(InfoBuffer, "File: %s\nFrom: FMVE %u.%u (R%u)\nCPU:  %lu (%s)\n",MYFILE, 
        					y, m, d, 
            			    cpu, 
            		        GetVampireString(FM_PROCESSOR_INFO>>24)
            			    ); 
                    }
        		}
            	else
            	{
            		if (ELAPSEDTIME!=0)
                    {		
            	        sprintf(InfoBuffer, "File: %s\nFrom: FMVE %u.%u (R%u)\nCPU:  %lu\nTime: %1.2f secs\n",MYFILE, 
        					y, m, d, 
            			    cpu, 
            		        ELAPSEDTIME
            			    ); 
            		}
                    else
                    {
            			/* don´t show time */
                        sprintf(InfoBuffer, "File: %s\nFrom: FMVE %u.%u (R%u)\nCPU:  %lu\n",MYFILE, 
        					y, m, d, 
            			    cpu
            			    );         
                    }
            	}
        	} 
            else
        	{ 
        		/* otherwise display old info */   
                sprintf(InfoBuffer, "File: %s\nFrom: FlashMandelVE %u.%u (R%u)\n",MYFILE, 
        				y, m, d
            		    ); 
            }
        }	
    }
    EasyRequest (Win,&MyReq,0);
}

void ShowDataLoadedInfo(struct Window *Win)
{
    struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Picture Info", 
	 InfoBuffer,"Ok" };
	
    struct EasyStruct NoData = { sizeof (struct EasyStruct),0,"Picture Info", 
	 "No info available.\n(Load a picture or data first.)","Ok" };

	UBYTE recalcinfo[] = "\nOnly data has been loaded.\n=> Click ok to recalculate ...\n";
    
	if ((NewMANDChunk.Version==0) && (!LEGACY_PICTURE))  EasyRequest(Win,&NoData,0);
	else 
    {
    	if (LEGACY_PICTURE)
        	sprintf(InfoBuffer, "Filename: %s\nFrom: Legacy FlashMandel\n%s",MYFILE, recalcinfo); 
        else
    		sprintf(InfoBuffer, "Filename: %s\nFrom: FlashMandelVE %u.%u (R%u)\n%s",MYFILE, 
        		GetYear(NewMANDChunk.Version), GetMonth(NewMANDChunk.Version), GetDay(NewMANDChunk.Version), recalcinfo ); 
        	
        EasyRequest (Win,&MyReq,0);
    }
}

void ShowChangeManuallyTo24Bit(struct Window *Win)
{
    struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"Picture Info", 
	 InfoBuffer,"Ok" };
	
    struct EasyStruct NoData = { sizeof (struct EasyStruct),0,"Picture Info", 
	 "No info available.\n(Load a picture or data first.)","Ok" };

	UBYTE moreinfo[] = "\nOnly data has been loaded.\nThe data is for a 24 Bit image\n=> Change manually to a 24bit screen ...\n=> Select recalculate after that ...\n";
    
	if ((NewMANDChunk.Version==0) && (!LEGACY_PICTURE))  EasyRequest(Win,&NoData,0);
	else 
    {
    	if (LEGACY_PICTURE)
        	sprintf(InfoBuffer, "Filename: %s\nFrom: Legacy FlashMandel\n%s",MYFILE, moreinfo); 
        else
    		sprintf(InfoBuffer, "Filename: %s\nFrom: FlashMandelVE %u.%u (R%u)\n%s",MYFILE, 
        		GetYear(NewMANDChunk.Version), GetMonth(NewMANDChunk.Version), GetDay(NewMANDChunk.Version), moreinfo ); 
        	
        EasyRequest (Win,&MyReq,0);
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
	 "Orbits can only be selected\non main (default) view of \nMandelbrot, Multibrot or\nBurning Ship fractal.\n","Ok" };

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
	 "Orbits have been activated\nPress ESC to keep the last orbit.\nPress DEL to delete it.","Ok" };

	EasyRequest (Win,&MyReq,0);
}

void NotAccurateColorsIn24Bit(struct Window *Win)
{
	static struct EasyStruct MyReq = { sizeof (struct EasyStruct),0,"AmigaOS drawing", 
	 "System friendly drawing in 24Bit mode\nis slow and has inaccurate colors.\n(Use an 8Bit screenmode instead.)","Ok" };

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

  /* menu undo (disabled) */
  /* if (ShowMenu) OnMenu (Win,FULLMENUNUM (2,4,NOSUB)); */
}

void RestoreCoords (struct Window *Win)
{
  RMIN = URMIN;

  RMAX = URMAX;

  IMIN = UIMIN;

  IMAX = UIMAX;

  JKRE = UJKRE;

  JKIM = UJKIM;

  /* menu undo (disabled) */
  /* OffMenu (Win,FULLMENUNUM (2,4,NOSUB)); */
}

void PeriodicityParametersGadget (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr)
{
  struct IntuiMessage *Message;

  char str_eps[20], str_red[20], str_green[20], str_blue[20], str_div[20];

  struct Gadget *GadList = NULL,*IntGad[4],*FloatGad,*MyButtonGad,*MyGad;

  struct Window *GadWin;

  BOOL Exit = FALSE;

  UWORD MyCode;

  ULONG MyClass;
                           
  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyGad = CreateContext(&GadList);
  
  TEXTGAD.ng_LeftEdge     = 90;
  TEXTGAD.ng_TopEdge      = 20;
  TEXTGAD.ng_Width        = 180;
  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;
  TEXTGAD.ng_GadgetText   = (TEXT*)"Epsilon:"; 
  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str_eps, "%f", PERIODICITY_EPSILON);
  FloatGad = CreateGadget( STRING_KIND, MyGad, &TEXTGAD, GTST_String, str_eps,  
  	/*GTMX_TitlePlace, PLACETEXT_LEFT,*/ TAG_END);

  TEXTGAD.ng_GadgetText   = (TEXT*)"Red:"; 
  TEXTGAD.ng_LeftEdge     = 90;
  TEXTGAD.ng_TopEdge      = 40;
  TEXTGAD.ng_Width        = 60;
  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str_red, "%u", PRIME_RED);
  IntGad[0] = CreateGadget( STRING_KIND, FloatGad, &TEXTGAD, GTST_String, str_red, TAG_END);
  

  TEXTGAD.ng_LeftEdge     = 90;
  TEXTGAD.ng_TopEdge      = 60;
  TEXTGAD.ng_GadgetText   = (TEXT*)"Green"; 
  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str_green, "%u", PRIME_GREEN);
  IntGad[1] = CreateGadget( STRING_KIND, IntGad[0], &TEXTGAD, GTST_String, str_green, TAG_END);
  
  TEXTGAD.ng_LeftEdge     = 90;
  TEXTGAD.ng_TopEdge      = 80;
  TEXTGAD.ng_GadgetText   = (TEXT*)"Blue"; 
  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str_blue, "%u", PRIME_BLUE);
  IntGad[2] = CreateGadget( STRING_KIND, IntGad[1], &TEXTGAD, GTST_String, str_blue, TAG_END);
  
  TEXTGAD.ng_LeftEdge     = 90;
  TEXTGAD.ng_TopEdge      = 100;
  TEXTGAD.ng_GadgetText   = (TEXT*)"Divider"; 
  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str_div, "%u", PRIME_DIVIDER);
  IntGad[3] = CreateGadget( STRING_KIND, IntGad[2], &TEXTGAD, GTST_String, str_div, TAG_END);
  
  BUTTONGAD.ng_LeftEdge   = 20;

  BUTTONGAD.ng_TopEdge    = TEXTGAD.ng_TopEdge + TEXTGAD.ng_Height + 30;

  BUTTONGAD.ng_Width      = 90;

  BUTTONGAD.ng_Height     = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  BUTTONGAD.ng_GadgetText = "_Accept";

  BUTTONGAD.ng_GadgetID   = ACCEPT;

  MyButtonGad = CreateGadget (BUTTON_KIND,IntGad[3],&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (60 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "_Cancel";

  BUTTONGAD.ng_GadgetID   = CANCEL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  if (MyButtonGad)
  {
     GadWin = OpenWindowTags (NULL,WA_Left,Win->LeftEdge + 25,
                                   WA_Top,Win->TopEdge + 35,
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 30 +200,
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
                                                              case 'I' : ActivateGadget (MyGad,GadWin,0);

                                                                         break;

                                                              case 'a' :
                                                              case 'A' : 
                                                                         PERIODICITY_EPSILON = atof(((struct StringInfo*)FloatGad->SpecialInfo)->Buffer);
                                                                         PRIME_RED 		= atoi(((struct StringInfo *)IntGad[0]->SpecialInfo)->Buffer); 
                                                                         PRIME_GREEN 	= atoi(((struct StringInfo *)IntGad[1]->SpecialInfo)->Buffer); 
                                                                         PRIME_BLUE 	= atoi(((struct StringInfo *)IntGad[2]->SpecialInfo)->Buffer); 
                                                                         PRIME_DIVIDER 	= atoi(((struct StringInfo *)IntGad[3]->SpecialInfo)->Buffer); 
                                                                         
                                                                         printf("PRIME: RED: %u GREEN: %u BLUE: %u DIV: %u\n", 
                                                                              	PRIME_RED, PRIME_GREEN, PRIME_BLUE, PRIME_DIVIDER);
                                                                          
                                                                         printf("PRIME: RED: %s GREEN: %u BLUE: %u DIV: %u\n", 
                                                                              	((struct StringInfo *)IntGad[0]->SpecialInfo)->Buffer, PRIME_GREEN, PRIME_BLUE, PRIME_DIVIDER);
                                                                              
                                                                         if (PRIME_RED==0) PRIME_RED=1;
                                                                         if (PRIME_GREEN==0) PRIME_GREEN=1;
                                                                         if (PRIME_BLUE==0) PRIME_BLUE=1;
                                                                         if (PRIME_DIVIDER==0) PRIME_DIVIDER=1;
                                                                              
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
                                                               case ACCEPT  : 
                                                                              PERIODICITY_EPSILON 	= atof(((struct StringInfo*)FloatGad->SpecialInfo)->Buffer);
                                                                         	  PRIME_RED 			= atoi(((struct StringInfo *)IntGad[0]->SpecialInfo)->Buffer); 
                                                                         	  PRIME_GREEN 			= atoi(((struct StringInfo *)IntGad[1]->SpecialInfo)->Buffer); 
                                                                         	  PRIME_BLUE 			= atoi(((struct StringInfo *)IntGad[2]->SpecialInfo)->Buffer); 
                                                                         	  PRIME_DIVIDER 		= atoi(((struct StringInfo *)IntGad[3]->SpecialInfo)->Buffer); 
                                                                         
                                                                         	  if (PRIME_RED==0) PRIME_RED=1;
                                                                         	  if (PRIME_GREEN==0) PRIME_GREEN=1;
                                                                         	  if (PRIME_BLUE==0) PRIME_BLUE=1;
                                                                         	  if (PRIME_DIVIDER==0) PRIME_DIVIDER=1;
                                                                              
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

}

void NebulaIterationsGadget (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr)
{
  struct IntuiMessage *Message;

  struct Gadget *GadList = NULL,*MyIntGad,*MyButtonGad,*MyGad,*iteration[6];

  struct Window *GadWin;

  BOOL Exit = FALSE;

  UWORD MyCode;

  ULONG MyClass;

  UWORD i;

  const UBYTE *labels[6] = { "Red: Max:", "Min:", "Green: Max:", "Min:", "Blue: Max:", "Min:" };
                           
  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyIntGad = CreateContext(&GadList);
  
  for (i=0; i<6; i++)
  {
  	TEXTGAD.ng_LeftEdge     = (i%2==0) ? 120 : 260;
	
  	TEXTGAD.ng_TopEdge      = 20+(i/2)*20;

  	TEXTGAD.ng_Width        = 90;

  	TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  	TEXTGAD.ng_GadgetText   = (TEXT*)labels[i]; /*TxtString;*/

 	TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  	iteration[i] = MyIntGad = CreateGadget (INTEGER_KIND, MyIntGad,&TEXTGAD,GTIN_Number,RGB_ITER[i],GTIN_MaxChars,6,STRINGA_ReplaceMode,/*TRUE*/FALSE,GT_Underscore,'_',TAG_END);
  }
  
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
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 30 +200,
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
                                                              case 'A' : for (i=0; i<6; i++) RGB_ITER[i] = ((struct StringInfo *)iteration[i]->SpecialInfo)->LongInt;
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
                                                               case ACCEPT  : for (i=0; i<6; i++) RGB_ITER[i] = ((struct StringInfo *)iteration[i]->SpecialInfo)->LongInt;
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

}

void HitmapColorsGadget (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr)
{
  struct IntuiMessage *Message;

  struct Gadget *GadList = NULL,*MyIntGad,*MyButtonGad,*MyGad,*color[16];

  struct Window *GadWin;

  BOOL Exit = FALSE;

  UWORD MyCode;

  ULONG MyClass;

  UWORD i;

  const UBYTE *labels[16] = {  "EDGE  = a:", "r:", "g:", "b:",
 							   "LEMNI = a:", "r:", "g:", "b:",
							   "TRY   = a:", "r:", "g:", "b:", 
							   "MISS  = a:", "r:", "g:", "b:" 
                         };
                           
  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyIntGad = CreateContext(&GadList);
  
  for (i=0; i<16; i++)
  {
  	switch (i%4)
    {	
    	case 0 : TEXTGAD.ng_LeftEdge     = 90; break;
        case 1 : TEXTGAD.ng_LeftEdge     = 160; break;
        case 2 : TEXTGAD.ng_LeftEdge     = 230; break;
	    case 3 : TEXTGAD.ng_LeftEdge     = 300; break;
    }
    
  	TEXTGAD.ng_TopEdge      = 20+i/4*20;

  	TEXTGAD.ng_Width        = 40;

  	TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  	TEXTGAD.ng_GadgetText   = (TEXT*)labels[i];

 	TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  	color[i] = MyIntGad = CreateGadget (INTEGER_KIND, 
    										MyIntGad,&TEXTGAD,
                                            GTIN_Number,(HITMAP_COLORS[i/4] & 0xff<<((3-(i%4))*8))>>((3-(i%4))*8),
                                            GTIN_MaxChars,3,
                                            STRINGA_ReplaceMode,FALSE,
                                            GT_Underscore,'_',
                                            TAG_END);
  }
  
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
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 30 +120,
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
                                                              case 'A' : for (i=0; i<4; i++) HITMAP_COLORS[i]=0; 
                                                              			 for (i=0; i<16; i++) 
                                                              			 {
                                                                         	HITMAP_COLORS[i/4] 
                                                                            |= 
                                                                            ((struct StringInfo *)color[i]->SpecialInfo)->LongInt
                                                                            <<((3-(i%4))*8)
                                                                            ;
                                                                         	
                                                                         }
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
                                                               case ACCEPT  : 
                                                              			 for (i=0; i<4; i++) HITMAP_COLORS[i]=0; 		 
                                                                         for (i=0; i<16; i++) 
                                                              			 {
                                                                         	HITMAP_COLORS[i/4] 
                                                                            |= 
                                                                            ((struct StringInfo *)color[i]->SpecialInfo)->LongInt
                                                                            <<((3-(i%4))*8)
                                                                            ;
                                                                         	
                                                                         }
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

}

UBYTE GetActiveLemniscates(void)
{
	UBYTE selection=0;	
    if (BUDDHA_EDGES && BUDDHA_LEMNISCATES) selection=3;
    else if (BUDDHA_EDGES) selection=1;
    else if (BUDDHA_LEMNISCATES) selection=2;
    
    return selection;
}

void SetLemniscatesVariables(UBYTE selection)
{
    switch (selection)
    {
       case 0 : BUDDHA_EDGES=FALSE; BUDDHA_LEMNISCATES=FALSE; BUDDHA_RANDOM=TRUE; break; /* set RANDOM if no LEMNISCATES */
       case 1 : BUDDHA_EDGES=TRUE; BUDDHA_LEMNISCATES=FALSE; BUDDHA_RANDOM=FALSE; break;
       case 2 : BUDDHA_EDGES=FALSE; BUDDHA_LEMNISCATES=TRUE; BUDDHA_RANDOM=FALSE; break;
       case 3 : BUDDHA_EDGES=TRUE; BUDDHA_LEMNISCATES=TRUE; BUDDHA_RANDOM=FALSE; break;
    }
}

void LemniscatesParametersGadget (struct Window *Win,TEXT *TitleWin,TEXT *TitleScr)
{
  struct IntuiMessage *Message;

  struct Gadget *GadList = NULL,*MyMXGad,*MyCYGad,*MyFloatGad[2],*MyCHKGad,*MyButtonGad,*MyGad/*,*iteration[6]*/;
  UBYTE lemni_selection;
  BOOL lemni_alternate;
  char str1[20], str2[20];

  struct Window *GadWin;

  BOOL Exit = FALSE;

  UWORD MyCode;

  ULONG MyClass;

  const UBYTE *labels[5] = { "None (disable)", "Only Edges", "Only Lemniscates", "Edges & Leminscates", NULL };
                           
  TEXTGAD.ng_VisualInfo = BUTTONGAD.ng_VisualInfo = VINFO;

  MyMXGad = CreateContext(&GadList);
  
  TEXTGAD.ng_LeftEdge     = 90;
	
  TEXTGAD.ng_TopEdge      = 20;

  TEXTGAD.ng_Width        = 180;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = (TEXT*)"Queues:"; 

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
    
  lemni_selection=GetActiveLemniscates();
  MyCYGad = MyMXGad = CreateGadget( /*MX*/ CYCLE_KIND, MyMXGad, &TEXTGAD, GTCY_Labels, labels, GTCY_Active, lemni_selection, 
  	/*GTMX_TitlePlace, PLACETEXT_LEFT,*/ TAG_END);

  TEXTGAD.ng_GadgetText   = (TEXT*)"Randomize:"; 
  TEXTGAD.ng_LeftEdge     = 20;
  TEXTGAD.ng_TopEdge      = 40;
  TEXTGAD.ng_Width        = 0;
  TEXTGAD.ng_Flags        = PLACETEXT_RIGHT;
  MyMXGad = CreateGadget( TEXT_KIND, MyMXGad, &TEXTGAD, TAG_END);
  

  TEXTGAD.ng_LeftEdge     = 40;
  TEXTGAD.ng_TopEdge      = 60;

  TEXTGAD.ng_Width        = 90;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = (TEXT*)"x"; 

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str1, "%f", BUDDHA_RANDOMIZE_X);
  MyFloatGad[0] = CreateGadget( STRING_KIND, MyMXGad, &TEXTGAD, GTST_String, str1, TAG_END);
  
  TEXTGAD.ng_TopEdge      = 80;

  TEXTGAD.ng_Width        = 90;

  TEXTGAD.ng_Height       = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  TEXTGAD.ng_GadgetText   = (TEXT*)"y"; 

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;
  sprintf(str2, "%f", BUDDHA_RANDOMIZE_Y);
  MyFloatGad[1] = CreateGadget( STRING_KIND, MyFloatGad[0], &TEXTGAD, GTST_String, str2, TAG_END);

  
  TEXTGAD.ng_GadgetText   = (TEXT*)"Random points (alternate)"; 
  TEXTGAD.ng_LeftEdge     = 20;
  TEXTGAD.ng_TopEdge      = 100;
  TEXTGAD.ng_Width        = 20;
  TEXTGAD.ng_Flags        = PLACETEXT_RIGHT;
  lemni_alternate=BUDDHA_RANDOM;
  MyCHKGad = CreateGadget( CHECKBOX_KIND, MyFloatGad[1], &TEXTGAD, GTCB_Checked, lemni_alternate, TAG_END);
  
  
  
  BUTTONGAD.ng_LeftEdge   = 20;

  BUTTONGAD.ng_TopEdge    = TEXTGAD.ng_TopEdge + TEXTGAD.ng_Height + 30;

  BUTTONGAD.ng_Width      = 90;

  BUTTONGAD.ng_Height     = (MYFONTSTRUCT.ta_YSize * 3L) >> 1L;

  BUTTONGAD.ng_GadgetText = "_Accept";

  BUTTONGAD.ng_GadgetID   = ACCEPT;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyCHKGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  BUTTONGAD.ng_LeftEdge  += (60 + BUTTONGAD.ng_Width);

  BUTTONGAD.ng_GadgetText = "_Cancel";

  BUTTONGAD.ng_GadgetID   = CANCEL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTONGAD,GT_Underscore,'_',TAG_END);

  if (MyButtonGad)
  {
     GadWin = OpenWindowTags (NULL,WA_Left,Win->LeftEdge + 25,
                                   WA_Top,Win->TopEdge + 35,
                                   WA_Width,BUTTONGAD.ng_LeftEdge + BUTTONGAD.ng_Width + 30 +200,
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
                     
                     if (MyGad == MyCYGad)
                     {
                        lemni_selection=MyCode;
                     }
                     else if (MyGad == MyCHKGad)
                     {
                        lemni_alternate = (MyCHKGad->Flags & GFLG_SELECTED);
                     }

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
                                                              case 'I' : ActivateGadget (MyMXGad,GadWin,0);

                                                                         break;

                                                              case 'a' :
                                                              case 'A' : 
                                                                         SetLemniscatesVariables(lemni_selection);
                                                                         BUDDHA_RANDOM = lemni_alternate;
                                                                         BUDDHA_RANDOMIZE_X = atof((char const*)(struct StringInfo*)MyFloatGad[0]->SpecialInfo);
                                                                         BUDDHA_RANDOMIZE_Y = atof((char const*)(struct StringInfo*)MyFloatGad[1]->SpecialInfo);
                                                                         
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
                                                               case ACCEPT  : 
                                                                              SetLemniscatesVariables(lemni_selection);
                                                                         	  BUDDHA_RANDOM = lemni_alternate;
                                                                              BUDDHA_RANDOMIZE_X = atof(((struct StringInfo*)MyFloatGad[0]->SpecialInfo)->Buffer);
                                                                         	  BUDDHA_RANDOMIZE_Y = atof(((struct StringInfo*)MyFloatGad[1]->SpecialInfo)->Buffer);
                                                                         
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

  MyIntGad = CreateGadget (INTEGER_KIND,MyIntGad,&TEXTGAD,GTIN_Number,Var,GTIN_MaxChars,6,STRINGA_ReplaceMode,/*TRUE*/FALSE,GT_Underscore,'_',TAG_END);

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

  MyIntGad = CreateGadget (STRING_KIND,MyIntGad,&TEXTGAD,GTST_String,startvalue,GTST_MaxChars,12,STRINGA_ReplaceMode,/*TRUE*/FALSE,GT_Underscore,'_',TAG_END);

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
							/* due to linking problems avoid function 
                             * ToUpper() - normally: string.h 
                             */
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

  return result;
}


BOOL CheckBox (struct RastPort *Rp,const LONG a1,const LONG b1,const LONG a2,const LONG b2)
{
  const LONG Color = GetPixelPenGeneric (Rp,a1,b1);
  LONG c1, c2, c3;
  LONG Var;

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

  return TRUE;
}


ULONG Fail (UBYTE *ErrorString,ULONG ErrorLevel)
{
  DisplayBeep (NULL);
  
  printf("ERROR %u!\n", ErrorLevel); /* error string could be empty - output at least en ERROR! :) */
  
  fputs (ErrorString,stderr);
  printf("\n");
  
  if (
  		(ErrorString==QueryMandPicError)
        ||
        (ErrorString==LoadMandPicError)
        ||
        (ErrorString==SaveMandPicError)
        ||
        (ErrorString==LoadMandPalError)
        ||
        (ErrorString==SaveMandPalError)
     )
  {
  		printf("Filename: %s\n", MYPATH);
  }
  
  RETURNVALUE = ErrorLevel;

  return ErrorLevel;
}

void SetRGBIterations(void)
{
	RGB_ITER[FM_RED_MAX] = MAX_ITERATIONS+1;
  	RGB_ITER[FM_GREEN_MAX] = (ULONG)(RGB_ITER[FM_RED_MAX] * 0.375);
  	RGB_ITER[FM_BLUE_MAX] = (ULONG)(RGB_ITER[FM_RED_MAX] * 0.1875);
		
	BUDDHA_MIN_ITERATIONS = min(RGB_ITER[FM_RED_MAX] - RGB_ITER[FM_GREEN_MAX] - 1, 20);
	
	RGB_ITER[FM_RED_MIN] = (ULONG)(RGB_ITER[FM_RED_MAX] * 0.25);
	RGB_ITER[FM_GREEN_MIN] = (ULONG)(RGB_ITER[FM_RED_MAX] * 0.125);
	RGB_ITER[FM_BLUE_MIN] = (ULONG)(RGB_ITER[FM_RED_MAX] * 0.0625);
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
  
  						  break;
                          
  	 case NO_POINTER:	  SetPointer(Win,NoPointer,0,0,0,0);
     						
                          break;
  
  }
}

VOID ShowLegacyTime (struct Window *Win,TEXT *String,ULONG Secs)
{
  sprintf (BAR_STRING,"%s %uh %um %us - Speed: %.1Lf Pix/sec",String,Secs / 3600L,(Secs / 60L) % 60L,Secs % 60L,(double) ((Win->Width - Win->LeftEdge) * (Win->Height - Win->TopEdge)) / (double) Secs);

  SetWindowTitles (Win,(TEXT *) ~0,BAR_STRING);
}

VOID ShowVampireCycleTime(struct Window *Win, TEXT *String, double precise_time)
{
  sprintf (BAR_STRING,"%s %2.2f secs - Speed: %.1Lf Pix/sec",String,precise_time,(double) ((Win->Width - Win->LeftEdge) * (Win->Height - Win->TopEdge)) / (double) precise_time);

  SetWindowTitles (Win,(TEXT *) ~0,BAR_STRING);
}

VOID ShowTime(struct Window *Win,TEXT *String,double Secs)
{
	if ((!vampire_type) || (!UseCCCTiming((ULONG)Secs)))  ShowLegacyTime (MYILBM.win,"Time:",(ULONG)Secs);
    else ShowVampireCycleTime(MYILBM.win,"Time:",GetTime());  
}

ULONG PackCPUInfo(void)
{
	/* FMO_CPU_INFO (ULONG): 
     *  WORD (31-25): Vampire type (if 68080) 
     *  WORD (24-0): 680x0 
     */
    ULONG cpu_info=0;
    
	if (vampire) 
    {
    	cpu_info |= 68080;
		cpu_info |= ((ULONG)detectvamp())<<24;	
    } else cpu_info |= detectm68k();
	return cpu_info;
}

const UBYTE *vampire_names[10] = { 
						    "none",
    						"V600",
    						"V500",
    						"V4 Firebird",
    						"V4 Icedrake",
    						"V4SA",
    						"V1200",
    						"V4 Manticore",
    						"Vampire"
                           };
                           
UBYTE* GetVampireString(UWORD type)
{
    if (type<=7) return vampire_names[type];
    else return vampire_names[8];
}

UWORD GetVampireCore(void)
{
	UWORD core;
	core = *((UWORD*)(0xdff3ea));
    if (core) return core;
    else if (HasUSBScrollWheel()) return (UWORD)9435;
	else return NULL;
}

BOOL HasUSBScrollWheel(void)
{
	if (*((UBYTE*)(0xdff212+1))) return TRUE;
    else return FALSE;
}

UBYTE GetClockMultiplyer(void)
{
	return *((UBYTE*)0xdff3fc+1);
}

BOOL UseCCCTiming(ULONG secs)
{
	if (secs < (45.0 / GetClockMultiplyer() * 13.0)) return TRUE; /* show up to 45 secs precise timing for a 13x and adapt according to multiplyer (shorter if higher) */
    else return FALSE;
}

