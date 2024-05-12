
#include "splash.h"

LONG QuerySplashPic (struct ILBMInfo *,UBYTE *);

extern LONG IlbmCollects []; /* = { TAG_END }; */
extern LONG IlbmStops []; /* = { ID_ILBM, ID_BODY, TAG_END };*/
extern struct ILBMInfo MYILBM;
extern ULONG* PALETTE;
extern struct Library* DOSBase;

void ShowSplashRequester(struct Window* w, UBYTE seconds)
{
    LONG temp;
    struct Requester SR;        /* Splash requester */
    struct ILBMInfo SplashILBM = {0};
	LONG ClassicIlbmProps [] = { ID_ILBM, ID_BMHD, ID_ILBM, ID_CMAP, ID_ILBM, ID_CAMG, TAG_END };
  	struct ViewPort *vp;
  
    char filename[] = "FMVE43_splash_burningship_beta.iff"; /* www.stef.be/dpaint/ */
	
    LONG error;
    
    SplashILBM.ParseInfo.propchks = ClassicIlbmProps;
    SplashILBM.ParseInfo.collectchks = IlbmCollects;
    SplashILBM.ParseInfo.stopchks = IlbmStops;
    
    SplashILBM.ParseInfo.iff = AllocIFF ();
    
	SplashILBM.vp = MYILBM.vp;
	
    if (error = openifile (&(SplashILBM.ParseInfo),filename,IFFF_READ)) return;

    vp = ViewPortAddress (w);
    /*printf("vp: %p MYILBM.vp: %p\n", vp, MYILBM.vp);*/
	
    
 if (!error) {
 
    error=loadbrush(&SplashILBM, filename);
	
	error = getcolors(&SplashILBM);
	
	/*
	printf("result getcolors(): %d\n", error);
    printf("SplashILBM.Bmhd.w: %d .h: %d\n", SplashILBM.Bmhd.w, SplashILBM.Bmhd.h);
    printf("SplashILBM.wrp: %p SplashILBM.wrp->BitMap: %p\n", SplashILBM.wrp, SplashILBM.wrp->BitMap);
    printf("SplashILBM.brbitmap: %p\n", SplashILBM.brbitmap);
    printf("getcolors(SplashILBM): %d\n", getcolors(&SplashILBM));
    printf("SplashILBM.colortable: %p SplashILBM.ncolors: %d SplashILBM.ctabsize: %d\n", 
	  SplashILBM.colortable, SplashILBM.ncolors, SplashILBM.ctabsize);
	*/
	
	/*for (temp=0; temp</*SplashILBM.ncolors*//*15; temp++)
	{
		printf("%d: rgb: %d/%d/%d\n", 
			temp,
			*((ULONG*)SplashILBM.colortable+temp*3+1) & 0xff,
			*((ULONG*)SplashILBM.colortable+temp*3+2) & 0xff,
			*((ULONG*)SplashILBM.colortable+temp*3+3) & 0xff
			);
	}
	*/
	
	LoadRGB4(vp,(void*)SplashILBM.colortable, SplashILBM.ncolors);
	
    SR.OlderRequest = NULL;
    SR.LeftEdge = (w->Width-SplashILBM.Bmhd.w) >> 1;
    SR.TopEdge = (w->Height-SplashILBM.Bmhd.h) >> 1;;
    SR.Width=SplashILBM.Bmhd.w;
    SR.Height=SplashILBM.Bmhd.h;
    SR.RelLeft = 0;
    SR.RelTop = 0;
    SR.ReqGadget = NULL;
    SR.ReqBorder = NULL;
    SR.ReqText = NULL;
    SR.Flags = PREDRAWN;
    SR.BackFill = 1;
    SR.ReqLayer = NULL;
    /* ReqPad1 */
    SR.ImageBMap = SplashILBM.brbitmap; /*NULL;*/
    /*SR.ImageBMap=SplashILBM.wrp->BitMap;*/
    SR.RWindow = w;
    SR.ReqImage = NULL;
    /* ReqPad2 */
    
    /*InitRequester(&SR);*/
    Request(&SR,w);
	
	/* display splash screen */ 
	if (!seconds) 
	{
		/* display it until LMB is pressed */
		Delay(70);
		while (*((UBYTE*)0xbfe001) & 64) Delay(10);
	}
	else
	{
		/* display it for some seconds (stop earlier if LMB is pressed) */
		Delay(100);
		for (temp=1;temp<seconds*5;temp++)
		{
			if (*((UBYTE*)0xbfe001) & 64) Delay(10);
			else break;
    	}
	}
	
	EndRequest(&SR, w);
    unloadbrush(&SplashILBM);
 }
    FreeIFF(SplashILBM.ParseInfo.iff);

	LoadRGB32(MYILBM.vp, PALETTE);
	
}

LONG About (struct Window *Win)
{
/*static struct EasyStruct MyReq_1 = { sizeof (struct EasyStruct),0,"Informations requester","  " VERSION COPYRIGHT_DATE "\n\n\n  Authors:" "\n  " AUTHOR "\n  " ADDRESS "\n\n\n  " EMAIL "\n\n*******************************\n* This program is GiftWare !! *\n*******************************\n\n","Ok|More..." };
*/
static struct EasyStruct MyReq_1 = { sizeof (struct EasyStruct),0,"Informations requester", COPYRIGHT_CHUNK_STRING "\n\n" VERSION_COMMENT "\n\n*******************************\n* This program is GiftWare !! *\n*******************************\n\n","Ok|More..." };

static struct EasyStruct MyReq_2 = { sizeof (struct EasyStruct),0,"Workgroup","Greetings & thanks in alphabetical order:\n\n  Claudio Pucci\n  Giorgio Signori\n  Giuseppe Staffelli\n  Sergio Tassi\n\n\nIf someone hasn't the courage to risk\nfor his ideals, either his ideals are\nworth nothing, or worth nothing is he\n\nAmiga forever.\n\n","Ok" };

LONG More;

  More = EasyRequest (Win,&MyReq_1,0);

  if (! More) EasyRequest (Win,&MyReq_2,0);

  return More;
}


LONG Choice (struct Window *Win,TEXT *Title,TEXT *String)
{
struct EasyStruct MyReq = { sizeof (struct EasyStruct),NULL,0,0,"Yes|No" };

  MyReq.es_Title = Title;

  MyReq.es_TextFormat = String;

  return EasyRequest (Win,&MyReq,0);
}
