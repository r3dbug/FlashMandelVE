
/* includes cgfx and other graphics stuff */

#include "fmcgx.h"

extern ULONG detected_system;

struct Library* CyberGfxBase;

ULONG bmapdepth, pixfmt;
ULONG query[] = { LBMI_DEPTH, (ULONG)&bmapdepth, LBMI_PIXFMT, (ULONG)&pixfmt, TAG_END };
ULONG query2[] = { UBMI_REALLYUNLOCK, 1, TAG_END };

ULONG GetBitMapBPP(struct BitMap* bm)
{
	static ULONG bpp;
	ULONG tl1[] = { LBMI_BYTESPERPIX, (ULONG)&bpp, TAG_END };
	ULONG tl2[] = { UBMI_REALLYUNLOCK, 1, TAG_END };
	   
	UnLockBitMapTagList(LockBitMapTagList(bm, (struct TagItem*) &tl1), (struct TagItem*) &tl2);
	 
	if (!bpp) bpp=1;
	
	return bpp;
}

ULONG GetBitMapPixFMT(struct BitMap* bm)
{
	static ULONG pixfmt;
	ULONG tl1[] = { LBMI_PIXFMT, (ULONG)&pixfmt, TAG_END };
	ULONG tl2[] = { UBMI_REALLYUNLOCK, 1, TAG_END };
	   
	UnLockBitMapTagList(LockBitMapTagList(bm, (struct TagItem*) &tl1), (struct TagItem*) &tl2);
	   
	return pixfmt;
}

void* GetBitMapPtr(struct BitMap* bm)
{
	static ULONG* screenbuffer;
	ULONG tl1[] = { LBMI_BASEADDRESS, (ULONG)&screenbuffer, TAG_END };
	ULONG tl2[] = { UBMI_REALLYUNLOCK, 1, TAG_END };
	   
	UnLockBitMapTagList(LockBitMapTagList(bm, (struct TagItem*) &tl1), (struct TagItem*) &tl2);
	 
	return screenbuffer;
}

/*
void DebugBitmapFlags(ULONG f)
{
	if (f & BMF_CLEAR) printf(" BMF_CLEAR");
    if (f & BMF_DISPLAYABLE) printf(" BMF_DISPLAYABLE");
    if (f & BMF_INTERLEAVED) printf(" BMF_INTERLEAVED");
    if (f & BMF_STANDARD) printf(" BMF_STANDARD");
    if (f & BMF_MINPLANES) printf(" BMF_MINPLANES");
}


void DebugBitmap(struct BitMap *bm)
{
	int i;
	printf("----------------------------\nDebug bitmap:\nPointer: bm = %p\n", bm);
    
    if (bm)
    {
    	printf("BytesPerRow: %u\n", bm->BytesPerRow);
        printf("Rows:        %u\n", bm->Rows);
        printf("Flags:       %x (", bm->Flags);
        DebugBitmapFlags(bm->Flags);
        printf(" )\n");
        printf("Depth:       %u\n", bm->Depth);
        printf("pad:         %u (%x)\n", bm->pad, bm->pad);
        for (i=0; i<9; i++)
        {
        printf("Planes[%u]:   %p\n", i, bm->Planes[i]);
        }
    } 
	printf("----------------------------\n", bm);
   
}
*/

BOOL IsAClassicScreen(LONG ModeID)
{
	return (!IsCyberModeID(ModeID));
}

void FreeBitMapSafety (struct BitMap *Bitmap)
{
    WaitBlit ();
        
    if (Bitmap) 
	{
		/*printf("BytesPerRow: %u Rows: %u Depth: %u pad: %p plane[0]: %p\n",
		      Bitmap->BytesPerRow, Bitmap->Rows, Bitmap->Depth, Bitmap->pad, Bitmap->Planes[0]);
		*/
            FreeBitMap(Bitmap); 
	}
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
     strcpy (MYFONTSTRUCT.ta_Name, DEF_FONTNAMESTR);

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

  if (! (OpenDisplay (Ilbm,MAX (Ilbm->Bmhd.pageWidth,Ilbm->Bmhd.w),MAX (Ilbm->Bmhd.pageHeight,Ilbm->Bmhd.h),Ilbm->Bmhd.nPlanes,Ilbm->camg))) 
  {
  	printf("Could not open display (sorry ... ;-) ...\n");
	
  	return NULL; 
  }

  PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
  
  Ilbm->Bmhd.nPlanes = GetBitMapAttr(Ilbm->wrp->BitMap,BMA_DEPTH);

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
     TempBM=NULL;
     
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
	 Ilbm->brbitmap=NULL;
     
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

  /* use different flags / bitmap types for AmigaOS (BMF_INTERLEAVED) and AROS / ApolloOS (non-interleaved) */
  if (TempBM = AllocBitMap ((((Width + 15) >> 4) << 1), 1, Planes, FM_ALLOCTEMPRAST_FLAGS /*BMF_INTERLEAVED |*//* BMF_CLEAR | BMF_MINPLANES*/, NULL))
  {
     InitRastPort (&TempRP);

     TempRP.Layer = NULL;

     TempRP.BitMap = TempBM;

     if (PixelLine = AllocVec (((((Width + 15) * sizeof (*PixelLine)) >> 4)  << 4),MEMF_CLEAR | MEMF_PUBLIC)) Success = TRUE;

     else FreeBitMapSafety (TempBM);
  }
  /* DebugBitmap(TempBM); */
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
   						   WA_ScreenTitle,VERSION_TITLE_COMPLETE,
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
         * FIX MinX first 
         */

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
                       BIDTAG_SourceID,NewModeID, /* why was this line commented out ?!? */
                       TAG_END);

  if (ModeID != INVALID_ID) NewModeID = ModeID;

  return (NewModeID);
}

struct Screen *OpenIdScreen (struct ILBMInfo *Ilbm,WORD Width,WORD Height,WORD Depth,ULONG ModeID)
{
  struct Rectangle Spos,DClip,TxtO,StdO,MaxO,UClip;

  struct Rectangle *UClipp;

  struct Screen *Scr;

  UBYTE temp;
  
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

/*
	 printf("OpenScreenTags(): ModeID: %p, SA_Type: %d, SA_Top: %d, SA_Left: %d, SA_Width: %d, SA_Height: %d, SA_Depth: %d\n",
	      ModeID, Ilbm->stype, Spos.MinY, Spos.MinX, Width, Height, Depth);
*/	

     Scr = OpenScreenTags (NULL,
                           SA_DisplayID,   ModeID,
                           SA_Type,        Ilbm->stype,
                           SA_Top,         Spos.MinY,
                           SA_Left,        Spos.MinX,
                           SA_Width,       Width,
                           SA_Height,      Height,
                           SA_Depth,       Depth,
                           /*SA_DClip,       &DClip,
                           SA_AutoScroll,  Ilbm->Autoscroll,
                           */
						   SA_Colors32,    PALETTE,
                           SA_Pens,        PENS,
                           /*
						   SA_Interleaved, TRUE,
                           */
						   /*SA_Font,        &MYFONTSTRUCT,*/ /* disabled because it may cause memory issues ?! */
                           SA_Title,       title_string,
                           /*SA_ErrorCode,   &ErrorCode,
                           */
						   /*SA_ShowTitle,   Ilbm->TBState,*/
                           /*BitMapTag,      Ilbm->brbitmap,
                           PassedTags,     Ilbm->stags,
                           */
						   TAG_END);

     temp=drawing;
     
     /* this part has not been tested */
     if (IsAClassicScreen(ModeID)) 
     {
        drawing=DRAW_CLASSIC;
     	SAFP();
     }
     
     if (temp!=drawing) SetDrawingMenu();
     
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
