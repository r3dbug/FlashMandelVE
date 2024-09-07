
#include "splash.h"

#define LEFTBUTTON 		(!(*(UBYTE*)0xBFE001 & (1<<6)))		/* CIAPRA */
#define RIGHTBUTTON 	(!(*(UWORD*)0xDFF016 & (1<<10)))	/* POTGOR */
#define DOUBLETOGGLE    ((LEFTBUTTON) && (RIGHTBUTTON))     /* toggle by clicking on left+right button simultaneously */

LONG QuerySplashPic (struct ILBMInfo *,UBYTE *);

extern LONG IlbmCollects []; 
extern LONG IlbmStops []; 
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
    BOOL dbl=FALSE;
    
    char filename_beta[] = "FLASHMANDEL:Pictures/splash/FMVE43_splash_burningship_beta.iff"; /* www.stef.be/dpaint/ */
	char filename_stable[] = "FLASHMANDEL:Pictures/splash/FMVE43_splash_burningship_clean.iff"; /* www.stef.be/dpaint/ */
	char* filename;
    
    LONG error;
 
 
 	if (VERSION_DAY & 1) filename=filename_beta;
    else filename=filename_stable;
    
 	/* convert global buffers */
 	Shift( tb, tb1 );
    Shift( mb, mb1 );
         
    SplashILBM.ParseInfo.propchks = ClassicIlbmProps;
    SplashILBM.ParseInfo.collectchks = IlbmCollects;
    SplashILBM.ParseInfo.stopchks = IlbmStops;
    
    SplashILBM.ParseInfo.iff = AllocIFF ();
    
	SplashILBM.vp = MYILBM.vp;
	
    if (error = openifile (&(SplashILBM.ParseInfo),filename,IFFF_READ)) return;
    
    vp = ViewPortAddress (w);
	
 	if (!error) 
    {
 
    
    	error = loadbrush(&SplashILBM, filename);
    
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
			Delay(20);
			while (*((UBYTE*)0xbfe001) & 64) 
            {
            	Delay(10);
                if (DOUBLETOGGLE) dbl=TRUE;		
            }
		}
		else
		{
			/* display it for some seconds (stop earlier if LMB is pressed) */
			Delay(10);
			for (temp=1;temp<(seconds+1)*5+2;temp++)
			{
                if (DOUBLETOGGLE) dbl=TRUE;	
                if (*((UBYTE*)0xbfe001) & 64) Delay(10);
				else break;
    		}
		}
	
		EndRequest(&SR, w);
    
        unloadbrush(&SplashILBM); 
        closeifile(&SplashILBM.ParseInfo); /* done via unloadbrush() */
    	
    }
    
    FreeIFF(SplashILBM.ParseInfo.iff);
    	
	LoadRGB32(MYILBM.vp, PALETTE);
       
    if (GetBitMapBPP(w->RPort->BitMap)>1) 
    {
    	/* for un unknown reason colors are not correctly restored
           on 24/32bit screens
           => this is a quick fix to make menus look more or less 
              correct
         */ 
        SetRGB32(vp,0,0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa);
        SetRGB32(vp,1,0x08080808, 0x08080808, 0x08080808);
        SetRGB32(vp,2,0xffffffff, 0xffffffff, 0xffffffff);
        SetRGB32(vp,3,0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa); /* text color inside save requester */
    }
	
    if (dbl) AltRequest(MYILBM.win);
}

LONG About (struct Window *Win)
{
  static struct EasyStruct MyReq_1 = { sizeof (struct EasyStruct),0,"Informations requester", VERSION_TITLE "\n\nVersion: " XSTR(VERSION_YEAR) "." XSTR(VERSION_MONTH) " (R" XSTR(VERSION_DAY) ")\n\n" COPYRIGHT_STRING_ALL "\n\n" CONTACT_STRING_ALL  "\n\n" VERSION_COMMENT "\n\n*******************************\n* This program is GiftWare !! *\n*******************************\n\n","Ok|More..." };

  static struct EasyStruct MyReq_2 = { sizeof (struct EasyStruct),0,"Workgroup","Greetings & thanks in alphabetical order:\n\n  Claudio Pucci\n  Giorgio Signori\n  Giuseppe Staffelli\n  Sergio Tassi\n\n\nIf someone hasn't the courage to risk\nfor his ideals, either his ideals are\nworth nothing, or worth nothing is he\n\nAmiga forever.\n\n","Ok" };

  struct EasyStruct MyReq = { sizeof (struct EasyStruct),NULL,0,0,"Yes|No" };

  ULONG StartSec = NULL , EndSec = NULL, StartMicro = NULL, EndMicro = NULL;	

  LONG More;

  Shift( tb, tb1 );
  Shift( mb, mb1 );

  CurrentTime (&StartSec,&StartMicro);
  
  More = EasyRequest (Win,&MyReq_1,0);
  
  CurrentTime (&EndSec,&EndMicro);
  
  if (! More) EasyRequest (Win,&MyReq_2,0);
  else if (!(EndSec-StartSec)) AltRequest(MYILBM.win);
  
  return More;
}

void AltRequest (struct Window *Win)
{
  static struct EasyStruct MessReq = { sizeof (struct EasyStruct),0, tb1, mb1, ALTBUTTON };
  
  EasyRequest (Win,&MessReq,0);
}

LONG Choice (struct Window *Win,TEXT *Title,TEXT *String)
{
  struct EasyStruct MyReq = { sizeof (struct EasyStruct),NULL,0,0,"Yes|No" };

  MyReq.es_Title = Title;

  MyReq.es_TextFormat = String;

  return EasyRequest (Win,&MyReq,0);
}
