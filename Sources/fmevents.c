
#include "fmglobals.h"
#include "drawing.h"
#include "fmevents.h"
#include "version.h"

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
   	  /* discable Calculate -> Preview */
      /* OnMenu  (Win,FULLMENUNUM (2,0,NOSUB)); */ 

      OnMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }

   else
   {
      /* OffMenu  (Win,FULLMENUNUM (2,0,NOSUB)); */

      OffMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }
}

/* font requester disabled */
/*
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
  printf("End of FontRequest: Success = %u\n", Success);
  printf("MYFONTSTRUCT.ta_Name: %s\n", MYFONTSTRUCT.ta_Name);
  printf("MyFontReq->fo_Attr.ta_Name: %s\n", MyFontReq->fo_Attr.ta_Name);
  
  return Success;
}
*/

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

  StringGad_1 = CreateGadget (STRING_KIND,StringGad_1,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Top ";

  sprintf (String,"%+2.15Lf",IMAX);

  StringGad_2 = CreateGadget (STRING_KIND,StringGad_1,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Right";

  TEXTGAD.ng_Flags        = PLACETEXT_RIGHT;

  sprintf (String,"%+2.15Lf",RMAX);

  StringGad_3 = CreateGadget (STRING_KIND,StringGad_2,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Bottom";

  sprintf (String,"%+2.15Lf",IMIN);

  StringGad_4 = CreateGadget (STRING_KIND,StringGad_3,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (24 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "_Julia real";

  TEXTGAD.ng_Flags        = PLACETEXT_LEFT;

  sprintf (String,"%+2.15Lf",JKRE);

  StringGad_5 = CreateGadget (STRING_KIND,StringGad_4,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

  TEXTGAD.ng_TopEdge     += (14 + TEXTGAD.ng_Height);

  TEXTGAD.ng_GadgetText   = "J_ulia imag";

  sprintf (String,"%+2.15Lf",JKIM);

  StringGad_6 = CreateGadget (STRING_KIND,StringGad_5,&TEXTGAD,GTST_String,String,STRINGA_ReplaceMode,/*TRUE*/FALSE,GTST_MaxChars,MAXCHARS,GT_Underscore,'_',TAG_END);

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


/*
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

    PutPointer (PreviewWin,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

    ELAPSEDTIME = DrawFractalWrapper (PreviewWin,0L,0L,(LONG) (PreviewWin->GZZWidth) - 1L,(LONG) (PreviewWin->GZZHeight) - 1L);

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
       printf("FreeBitMap: BMASK & MASK\n");
       FreeBitMapSafety (MYBITMAP);

       MASK ^= BMASK;
    }

    Fail (PreviewWindowError,15L);

    Error = TRUE;
 }

 return Error;
}
*/

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

BOOL NewCoords (struct Window *Win,const WORD a1,const WORD b1,const WORD a2,const WORD b2)
{
    double KReal,KImag;
    double rmaxt, rmint, imaxt, imint;
    double center_delta;

	if (FractalType==BUDDHA)
	{
	  /* printf("Special calculation for BUDDHA zoom ...\n"); */
 
	  if (((a2 - a1) < 2L) || ((b2 - b1) < 2L)) return FALSE;

	  KReal = fabs (RMAX-RMIN) / ((double) Win->Height);

	  KImag = fabs (IMAX-IMIN) / ((double) Win->Width);

	  SaveCoords (Win,TRUE);

	  rmaxt = RMIN + ((double) b2 * KReal);

	  imint = IMIN + ((double) a1 * KImag);
	  
	  rmint = RMIN + ((double) b1 * KReal);

	  imaxt = IMIN + ((double) a2 * KImag);
  
	  RMAX = rmaxt;
	  RMIN = rmint;
	  IMIN = imint;
	  IMAX = imaxt;
  
	  if (BUDDHA_AUTO_CENTER)
      {
      	center_delta=fabs(fabs(IMAX)-fabs(IMIN));
        
        /* adjust epsilon with each zoom using percentage */
        BUDDHA_AUTO_CENTER_EPSILON=(IMAX-IMIN) / 100 * BUDDHA_AUTO_CENTER_PERCENTAGE;
        
      	if (center_delta < BUDDHA_AUTO_CENTER_EPSILON)
        {
        	IMAX -= center_delta / 2;
            IMIN = -IMAX; 
        }
      
      }
	  return TRUE;
  	
	}
	else
	{
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
                                   /*ASLFR_TextAttr,&MYFONTSTRUCT,*/  /* does this do a bad memory access => disabled for the moment */
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

		/* quick workaround for compiling */
        Success = AddPart (MYPATH,MYFILE,MAX_PATHLEN);
        
     } else Success = FALSE; /* when Cancel is pressed ?! */

     FreeAslRequest (MyFileReq);

	 if (!Success) return FALSE;
	 
     if (Save)
     {
        if (MyLock = Lock (MYPATH,ACCESS_READ))
        {
           
           if (! EasyRequest (Win,&MyReq,0)) Success = FALSE;
           
           UnLock (MyLock);
           
        }
     } 
  }
  return Success;
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

void HandleBarString(double RealCoord, double ImagCoord)
{
	UWORD core;

	/* vampire running in vampire mode */	
    UBYTE title_long[] = "%s [Core: %lu%s %ux] Mouse [%#+2.10Lf, %#+2.10Lfi] Time [%2.2f secs] Screen [%ux%ux%ubit] I [%lu]";
    UBYTE title_short[] = "%s [%lu%s %ux] M [%#+2.5Lf, %#+2.5Lfi] T [%2.2fs] S [%ux%ux%ubit] I [%lu]";
    UBYTE title_very_short[] = "%s [%#+2.3Lf, %#+2.3Lfi] %2.2fs %lux";
    
    /* vampire running classic code */
    UBYTE noopt_title_long[] = "%s [%lu] Mouse [%#+2.10Lf, %#+2.10Lfi] Time [%2.2f secs] Screen [%ux%ux%ubit] I [%lu]";
    UBYTE noopt_title_short[] = "%s [%lu] M [%#+2.5Lf, %#+2.5Lfi] T [%2.2fs] S [%ux%ux%ubit] I [%lu]";
    UBYTE noopt_title_very_short[] = "%s [%#+2.3Lf, %#+2.3Lfi] %2.2fs %lux";
    
    UBYTE *title;
    ULONG cpu;
    
    char plus[] = "+";
    
    if (!vampire) 
    {
        sprintf (BAR_STRING,"Classic [%lu] - M [%#+2.6Lf, %#+2.6Lfi] T [%u s] Screen [%ux%ux%u] I [%lu]",
    		detectm68k(), RealCoord, ImagCoord, (ULONG)ELAPSEDTIME, MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MAX_ITERATIONS +1);
    }                                      
    else 
    {
    	if (optimized)
        {
        	/* vampire running in vampire mode */
            if (MYILBM.Bmhd.w <= 320) title=title_very_short;
    		else if (MYILBM.Bmhd.w <= 640) title=title_short;
    		else title=title_long;
    	
    		if (core=GetVampireCore())
    	    {
    	    	if (core!=9435) plus[0]=0;
    	        
    	        if (title==title_very_short)
    	        	sprintf (BAR_STRING,title,
    	        		GetVampireString(vampire_type), RealCoord,ImagCoord, ELAPSEDTIME, MAX_ITERATIONS+1);
    	    	else
    	    		sprintf (BAR_STRING,title,
    		        	GetVampireString(vampire_type), core, plus, GetClockMultiplyer(), RealCoord,ImagCoord, ELAPSEDTIME, MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MAX_ITERATIONS+1);
        
    	    }
    	    else
    	    {	
    	    	sprintf (BAR_STRING,"%s [%ux] Mouse: [%#+2.10Lf, %#+2.10Lfi] Time [%2.2f secs] Screen [%ux%ux%ubit] I [%lu]",
    	        	GetVampireString(vampire_type), GetClockMultiplyer(), RealCoord, ImagCoord, ELAPSEDTIME, MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MAX_ITERATIONS+1);
			}	
    	}
        else
        {
    		/* vampire running classic code */
            if (MYILBM.Bmhd.w <= 320) title=noopt_title_very_short;
    		else if (MYILBM.Bmhd.w <= 640) title=noopt_title_short;
    		else title=noopt_title_long;
    	
            if (cpu=detectm68k())
    	    {
    	    	plus[0]=0; 
    	        
    	        if (title==title_very_short)
    	        	sprintf (BAR_STRING,title,
    	        		GetVampireString(vampire_type), RealCoord,ImagCoord, ELAPSEDTIME, MAX_ITERATIONS+1);
    	    	else
    	    		sprintf (BAR_STRING,title,
    		        	GetVampireString(vampire_type), cpu, RealCoord,ImagCoord, ELAPSEDTIME, MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MAX_ITERATIONS+1);
        
    	    }
    	    else
    	    {	
    	    	sprintf (BAR_STRING,"%s - Mouse: (%#+2.10Lf, %#+2.10Lfi) Time: %2.2f secs [%ux%ux%ubit] I: %lu",
    	        	GetVampireString(vampire_type), RealCoord, ImagCoord, ELAPSEDTIME, MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MAX_ITERATIONS+1);
			} 
        }
    }
}

void PrepareAllNewMANDChunkData(struct ILBMInfo *Ilbm)
{
	ULONG temp;
                                             
    NewMANDChunk.LeftEdge = MANDChunk.LeftEdge = Ilbm->win->LeftEdge;

    NewMANDChunk.TopEdge = MANDChunk.TopEdge = Ilbm->win->TopEdge;

    NewMANDChunk.Width = MANDChunk.Width = Ilbm->win->Width;

    NewMANDChunk.Height = MANDChunk.Height = Ilbm->win->Height;

    NewMANDChunk.RMin = MANDChunk.RMin = RMIN;

    NewMANDChunk.RMax = MANDChunk.RMax = RMAX;

    NewMANDChunk.IMin = MANDChunk.IMin = IMIN;

    NewMANDChunk.IMax = MANDChunk.IMax = IMAX;

    NewMANDChunk.RConst = MANDChunk.RConst = JKRE;

    NewMANDChunk.IConst = MANDChunk.IConst = JKIM;

    NewMANDChunk.Iterations = MANDChunk.Iterations = MAX_ITERATIONS + 1L;

    NewMANDChunk.Special = MANDChunk.Special = (REAL_BIT | SPREAD_BIT);

  	if (JULIA == TRUE) {
   		MANDChunk.Special |= JULIA_BIT;
  		NewMANDChunk.Special = MANDChunk.Special;
    }
                                               
   /* mark TRUECOLOR_IMAGE in MANDChunk.Special 
    * if it is a truecolor screen 
    */
   if (GetBitMapBPP(Ilbm->win->RPort->BitMap) >= 3)
   {
   		NewMANDChunk.Special |= TRUECOLOR_IMAGE;
	
		NewMANDChunk.Flags |= TRUECOLOR_IMAGE;
   	    NewRGBMANDChunk.Flags |= TRUECOLOR_IMAGE;
   }
											   
   if (ZMASK & MASK)
   {
      DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

      MASK ^= ZMASK;
   }
											   
   /* fill in new data fields */
   NewMANDChunk.Version = PackVersionWord( VERSION_YEAR, VERSION_MONTH, VERSION_DAY ); 
   NewMANDChunk.FractalType = BaseFractalType; 
   NewMANDChunk.Flags = NULL; /* default value: NULL */
											   
   /* algorithmm */
   NewMANDChunk.Algorithm[FMO_ALGORITHM_BASE] = algorithm;
   NewMANDChunk.Algorithm[FMO_ALGORITHM_BUDDHA] = algorithm_buddha;
   NewMANDChunk.Algorithm[FMO_ALGORITHM_3] = NULL;
   NewMANDChunk.Algorithm[FMO_ALGORITHM_4] = NULL;
											   
   /* switches */
   /* SWITCH_1 = JULIA */
   NewMANDChunk.Switch[FMO_JULIA] = JULIA;
   /* SWITCH_2 (former: ANTIBUDDHA) */								   
   NewMANDChunk.Switch[FMO_SWITCH_2] = (ANTIBUDDHA ? 1 << FMO_BIT_ANTIBUDDHA : 0);
   NewMANDChunk.Switch[FMO_SWITCH_2] |= (FractalType==5) ? 1 << FMO_BIT_BUDDHA : 0;
                                               
   /* SWITCH_3 */                                   
   NewMANDChunk.Switch[FMO_SWITCH_3] = (BOUNDARY_FILL ? 1 : 0); /* use this as flags */
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (USE_SYMMETRY ? 1 << FMO_BIT_SYMMETRY : 0);
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (USE_PERIODICITY ? 1 << FMO_BIT_PERIODICITY : 0);
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (USE_HAMMAP ? 1 << FMO_BIT_HITMAP : 0);
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (BUDDHA_EDGES ? 1 << FMO_BIT_BUDDHA_EDGES : 0);
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (BUDDHA_LEMNISCATES ? 1 << FMO_BIT_BUDDHA_LEMNISCATES : 0);
   NewMANDChunk.Switch[FMO_SWITCH_3] |= (BUDDHA_RANDOM ? 1 << FMO_BIT_BUDDHA_RANDOM : 0);
                                               
   /* SWITCH_4 */
   NewMANDChunk.Switch[FMO_SWITCH_4] = NULL;
											
   /* integer and float data (only set used fields for each fractal type 
    * and leave other fields as is)
    */
   switch (FractalType)
   {
   		case MANDEL : 	/* no additional data */ 
						NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
						NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
						break;
		case MULTIINT : 
						NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
						NewMANDChunk.IntData[FMO_MULTIPOWER] = MultiPower; 
   						NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
						break;
   	    case MULTIFLOAT : 
						NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
						NewMANDChunk.FloatData[FMO_MULTIPOWERFLOAT] = MultiPowerFloat; 
 						NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
						break;
		case BURNINGSHIP : 
						NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
   						NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
						break;
   	    case BUDDHA :	/* includes ANTIBUDDHA */
           				/* Algorithm for BUDDHA not saved !!!
                         * BUDDHA_EDGES, BUDDHA_LEMNISCATES, BUDDHA_HAMMAP
                         */
                                                              
                        NewMANDChunk.IntData[FMO_FM_REDMIN] = RGB_ITER[FM_RED_MIN];
 						NewMANDChunk.IntData[FMO_FM_REDITER] = RGB_ITER[FM_RED_MAX];
  						NewMANDChunk.IntData[FMO_FM_GREENMIN]= RGB_ITER[FM_GREEN_MIN];
 						NewMANDChunk.IntData[FMO_FM_GREENITER] = RGB_ITER[FM_GREEN_MAX];
  						NewMANDChunk.IntData[FMO_FM_BLUEMIN] = RGB_ITER[FM_BLUE_MIN];
  						NewMANDChunk.IntData[FMO_FM_BLUEITER] = RGB_ITER[FM_BLUE_MAX];
						NewMANDChunk.IntData[FMO_BUDDHA_MIN_ITERATIONS] =BUDDHA_MIN_ITERATIONS;
						NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
                        NewMANDChunk.IntData[FMO_PRIME_RED] =PRIME_RED;
                        NewMANDChunk.IntData[FMO_PRIME_GREEN] =PRIME_GREEN;
                        NewMANDChunk.IntData[FMO_PRIME_BLUE] =PRIME_BLUE;
                        NewMANDChunk.IntData[FMO_PRIME_DIVIDER] =PRIME_DIVIDER;
                        NewMANDChunk.IntData[FMO_MULTIPOWER] = MultiPower; 
						NewMANDChunk.IntData[FMO_HITMAP_EXTENSION] = HITMAP_EXTENSION | (HITMAP_SHIFT << 8) | (HITMAP_DURING_CALCULATION<<16) | (HITMAP_AT_THE_END<<17);
                        NewMANDChunk.IntData[FMO_HITMAP_COLOR0]=HITMAP_COLORS[0];                                                                    NewMANDChunk.IntData[FMO_HITMAP_COLOR1]=HITMAP_COLORS[1];
                        NewMANDChunk.IntData[FMO_HITMAP_COLOR2]=HITMAP_COLORS[2];
                        NewMANDChunk.IntData[FMO_HITMAP_COLOR3]=HITMAP_COLORS[3];
                        NewMANDChunk.FloatData[FMO_MULTIPOWERFLOAT]=MultiPowerFloat;
                        NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
                        NewMANDChunk.FloatData[FMO_PERIODICITY_EPSILON]=PERIODICITY_EPSILON;
                        NewMANDChunk.FloatData[FMO_BUDDHA_RANDOMIZE_X]=BUDDHA_RANDOMIZE_X;
                        NewMANDChunk.FloatData[FMO_BUDDHA_RANDOMIZE_Y]=BUDDHA_RANDOMIZE_Y;
                        
						break;
   }
											   
   /* common data for all fractal types 
    * (= data that is always saved) 
    */
   NewMANDChunk.FloatData[FMO_ELAPSEDTIME]=ELAPSEDTIME;
   NewMANDChunk.IntData[FMO_PROCESSOR_INFO]=PackCPUInfo();
                                                        
   /* copy NewMANDChunk data to NewRGBMANDChunk */
   memcpy(&NewRGBMANDChunk, &NewMANDChunk, sizeof(NewMANDChunk));
											   
   /* for truecolor images: add color map inside NewRGBMANDChunk */
   for (temp=0; temp<=255; temp++)
   {
   		NewRGBMANDChunk.Colors[2+temp*3] = (UBYTE)COLORS_AGA[1+temp*3];
		NewRGBMANDChunk.Colors[2+temp*3+1] = (UBYTE)COLORS_AGA[1+temp*3+1];
		NewRGBMANDChunk.Colors[2+temp*3+2] = (UBYTE)COLORS_AGA[1+temp*3+2];
   }
}

void SaveNewSpecialChunkPic(struct ILBMInfo *Ilbm)
{
   /* save picture with NEW_SPECIAL_CHUNK (new format) */
   switch (DD_BPP)
   {
   		case 3 :
		case 4 :
				/* use NewRGBMANDChunk for truecolor images (no CMAP) */
				/* printf("Save truecolor image (no CMAP, extended FMVE chunk).\n"); */
				Ilbm->ParseInfo.propchks=NewRGBIlbmProps;
				if (SaveMandPic (Ilbm,&NEWRGB_SPECIAL_CHUNK,&COPYRIGHT_CHUNK,MYPATH)) 
                {
					printf("save rgb fails ...\n");
                	Fail (SaveMandPicError,5L);
				}			
                break;
   		default :
   				/* use NewMANDChunk for 8bit image with CMAP */
				/* printf("Save 8bit image (with CMAP, standard FMVE chunk\n"); */
				Ilbm->ParseInfo.propchks=NewIlbmProps;
				if (SaveMandPic (Ilbm,&NEW_SPECIAL_CHUNK,&COPYRIGHT_CHUNK,MYPATH)) Fail (SaveMandPicError,5L);
   }
}

void PrepareAndSaveHistogram(struct ILBMInfo *Ilbm)
{
 ULONG templong;
 
 /* nebulabrot: save histogram */
 if (FractalType == BUDDHA)
 {
 	if (histogram_save)
	{
		NEBULA_WIDTH=Ilbm->win->Width;
		NEBULA_HEIGHT=Ilbm->win->Height;
											
		if (!(templong=SaveHistogram(AddFilenameExtension(MYPATH),&BuddhaCounter[0],NEBULA_WIDTH*NEBULA_HEIGHT*3*4, histogram_compression)));
		else printf("Error saving histogram: %u\n", templong);
 	}
 }
}

void PrepareAndSaveAllNewSpecialChunkPic(struct ILBMInfo *Ilbm)
{
	/* filename must be in global variable MYPATH */
	/* save in new format */

	/* Ilbm->ParseInfo.propchks=NewIlbmProps;*/
	if (Ilbm->ParseInfo.iff = AllocIFF ())
    {
         /* prepare all mandchunk data */
         PrepareAllNewMANDChunkData(Ilbm);
                                             
         SaveNewSpecialChunkPic(Ilbm);

         if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

         FreeIFF (Ilbm->ParseInfo.iff);
    }
                                         
   	PrepareAndSaveHistogram(Ilbm);
}
                                       
ULONG HandleEvents (struct ILBMInfo *Ilbm)
{
 struct IntuiMessage *Message;
 WORD MouseX = 0,MouseY = 0;
 UWORD MyCode = 0;
 LONG ErrorLegacyFormat, ErrorNewFormat, ErrorNewRGBFormat;
 ULONG MyClass = NULL,MyMenu = NULL,Index;
 double RealCoord,ImagCoord,ScrRatio,FracRatio;
 int iter, final_iter, rpx=0, rpy=0;
 double xn,yn, stepi, stepr, cx, cy;
 ULONG templong;
 WORD temp;
 BOOL truecolorimage=FALSE;
 /* ULONG mips1_1, mips1_2; */
 
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

	  /* Orbits drawing (Mandel & Julia) */
	  if (DrawOrbits) 
	  {
		   /* maybe not necessary (?!) */	   
           OffTitle(Ilbm->scr);
           	   
           SetAPen(Ilbm->win->RPort, FM_ORBIT_COLOR);
		   
		   RealCoord = RMIN + ((DOUBLE) MouseX * fabs (RMAX-RMIN) / (DOUBLE) (Ilbm->win->Width - 1));

           ImagCoord = IMAX - ((DOUBLE) MouseY * fabs (IMAX-IMIN) / (DOUBLE) (Ilbm->win->Height - 1));
		   
		   if (JULIA)
		   {
		   		
		   		cx=JKRE;
				cy=JKIM;
		   		xn=RealCoord;
		   		yn=ImagCoord;
		   }
		   else 
		   {
		   		cx=RealCoord;
				cy=ImagCoord;
				xn=0;
		   		yn=0;
		   }
		   
           if ((rpx!=0) && (rpy!=0)) 
           		Move(Ilbm->win->RPort,rpx,rpy);
           			
		   switch (FractalType)
		   {
		   		case MANDEL : final_iter = MandelInCStoreIterations(MAX_ITERATIONS,cx,cy); break;
				case MULTIINT : final_iter = MultiMandelInCStoreIterations(MAX_ITERATIONS,cx,cy); break;
				case MULTIFLOAT : final_iter = MultiMandelFloatingPowerInCStoreIterations(MAX_ITERATIONS,cx,cy); break;
				case BURNINGSHIP : final_iter = BurningShipInCStoreIterations(MAX_ITERATIONS,cx,cy); break;
		   }
		
		   stepi = (double)(Ilbm->win->Height) / (double)(IMAX-IMIN);
		   stepr = (double)(Ilbm->win->Width) / (double)(RMAX-RMIN);
		   
           rpx=(RealCoord-RMIN)*stepr;
		   rpy=(IMAX-ImagCoord)*stepi;
		   Move(Ilbm->win->RPort,rpx,rpy);
		   
           
		   BltBitMap(ScrCpyBuffer, 0,0, Ilbm->win->RPort->BitMap,0,0,Ilbm->win->Width,Ilbm->win->Height, 0xc0, 0xff,NULL);
		   
		   for (iter=MAX_ITERATIONS;iter>final_iter;iter--) 
		   {
		   		switch (FractalType)
				{
					case MANDEL : yn = im[iter];
								  xn = re[iter];
		   						  break;
				    case MULTIINT :
					case MULTIFLOAT : 
				   			  	  yn = im[iter];
								  xn = re[iter];
								  break;
					case BURNINGSHIP :
								  yn = -im[iter]; /* recorrect flipping */
								  xn = re[iter];
								  break;
					
				}	
				
				rpx=(xn-RMIN)*stepr;
				rpy=(IMAX-yn)*stepi;
				
		   		if ((rpx>=0) && (rpx<Ilbm->win->Width) 
				   && (rpy >= 0) && (rpy <Ilbm->win->Height))
				{
					Draw(Ilbm->win->RPort,rpx,rpy);
		   		}
		   }
		   

	  }
	  
      switch (MyClass)
      {
             case    IDCMP_MOUSEMOVE: if (TMASK & MASK)
                                      {
                                         RealCoord = RMIN + ((DOUBLE) MouseX * fabs (RMAX-RMIN) / (DOUBLE) (Ilbm->win->Width - 1));

                                         ImagCoord = IMAX - ((DOUBLE) MouseY * fabs (IMAX-IMIN) / (DOUBLE) (Ilbm->win->Height - 1));

                                         ScrRatio  = (DOUBLE) Ilbm->win->Width / (DOUBLE) Ilbm->win->Height;

                                         FracRatio = fabs (RMAX-RMIN) / fabs (IMAX-IMIN);

                                         HandleBarString(RealCoord, ImagCoord);										 
                                         
                                         WaitTOF ();

                                         SetWindowTitles (Ilbm->win,(TEXT *) ~0,BAR_STRING);
                                      }

                                      break;

             case IDCMP_MOUSEBUTTONS: if (MyCode == SELECTDOWN) 
                                      {
			 								
											ProcessMouse (Ilbm->win,MouseX,MouseY);
									  }
                                      break;

             case     IDCMP_MENUPICK: MyMenu = ProcessMenu (Ilbm->win,MyCode);
                                      
                                      if (MyMenu & EXIT_MSG) break;

                                      if (MyMenu & SHOWGUIDE_MSG)
                                      {
                                         /* launching MultiView only works 
                                          * when FM is launched from shell 
                                          * (WBRun must be used when launched
                                          * from WB
                                          */
                                         if (LaunchedFromShell()) 
                                         {	
                                         	Execute (SHOWGUIDECOMMAND,NULL,NULL);
                                         }
                                         else 
                                         {
                                            Execute (SHOWGUIDECOMMANDWB,NULL,NULL);
										 }
                                         
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
                                            templong = (LONG) IntegerGad (Ilbm->win,"Iterations requester","Insert new iterations...","_Iterations",MAX_ITERATIONS + 1L);
											if (templong==0xffffffff) break;
											
											templong-=1;
											
											if (templong < MAXITERPERIODICITY-2)
                                            {									
                                            	MAX_ITERATIONS = (ULONG)templong;
										  	} 
                                            else 
                                            {
                                            	/* quick and dirty way to communicate the limit (..) */	
                                                printf("MAX_ITERATIONS limited to: %u\n", MAXITERPERIODICITY);	
                                                MAX_ITERATIONS=MAXITERPERIODICITY-2;
                                            }
                                            
                                            if (MAX_ITERATIONS >= MIN_ITERATIONS) break;

                                            DisplayBeep (Ilbm->scr);

                                            MAX_ITERATIONS = MIN_ITERATIONS;
                                         	
										 }

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

									     SetRGBIterations();
										 
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
									  
									  if (MyMenu & SPLASH_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         ShowSplashRequester (Ilbm->win, 0);

                                         ResetMenuStrip (Ilbm->win,MAINMENU);

                                         ModifyIDCMP (Ilbm->win,IDCMP_STANDARD);

                                         break;
                                      }
                                      
                                      if (MyMenu & PICINFO_MSG)
                                      {
                                         ModifyIDCMP (Ilbm->win,NULL);

                                         ClearMenuStrip (Ilbm->win);

                                         ShowPicInfo (Ilbm->win);

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

                                         PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

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

                                         PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

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
                                         /* these 2 must be set to 0 when a new pic is loaded 
                                          * => make sure that edges&lemniscates are calculated 
                                          * again for the newly loaded picture 
                                          */
                                         queue_i=0; 
                                         lemni_i=0; 

                                         ELAPSEDTIME=0;
                                         
                                         /* set BaseFractalType to MANDEL for those pictures 
                                          * saved without correct information in older versions 
                                          */
                                         BaseFractalType=MANDEL; 
                                         
                                         if (FileRequest (Ilbm->win,"Load iff picture","FLASHMANDEL:Pictures",PICTURES_DRAWER,FALSE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
                                            
                                            LEGACY_PICTURE=FALSE;

					                        /* try the old format first */
											truecolorimage=FALSE;
											Ilbm->ParseInfo.propchks = IlbmProps;
											
											if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               /* printf("Try to load legacy format first.\n"); */
											   ErrorLegacyFormat=ErrorNewFormat=ErrorNewRGBFormat=FALSE;
											   ErrorLegacyFormat=QueryMandPic(Ilbm,&MANDChunkTmp,MYPATH);
											   if (ErrorLegacyFormat)
											   {
											   	  	/* try new format */
													/* printf("Loading legacy failed => try new format ... \n"); */
													Ilbm->ParseInfo.propchks = NewIlbmProps;
													ErrorNewFormat = QueryMandPicNew (Ilbm,&NewMANDChunkTmp,MYPATH);
													
													truecolorimage = NewMANDChunkTmp->Special & TRUECOLOR_IMAGE ? 1 : 0;
													/*printf("ErrorNewFormat: %u NewMANDChunkTmp->Special: %u NewMANDChunk->Flags: %u truecolorimage: %u\n",
														ErrorNewFormat, NewMANDChunkTmp->Special, NewMANDChunkTmp->Flags, truecolorimage);
													*/
													
													if (truecolorimage) 
													{
														/* try newRGB format */
														/* printf("Loading NewFormat failed => load NewRGBFormat ...\n"); */
														
														Ilbm->ParseInfo.propchks = NewRGBIlbmProps;
														ErrorNewRGBFormat = QueryMandPicNewRGB (Ilbm,&NewRGBMANDChunkTmp,MYPATH);
											   			
													} /* else printf("NewMANDChunk format worked (classic CMAP, 8bit).\n"); */
											   }
											   
											   if ((!ErrorLegacyFormat) || (!ErrorNewFormat) || (!ErrorNewRGBFormat))
                                               {
												  /* loading of MANDChunk (one of the three formats) worked => set variables */
												  /* start with classic MANDChunk */ 
												  MANDChunk = *MANDChunkTmp;
												  
												  /* if new format worked => copy data to MANDChunk (for compatibility) */
												  if (ErrorLegacyFormat)
												  {
													if (!truecolorimage)
													{
														/* printf("set pointers for new format (classic CMAP 8bit)\n"); */
														NewMANDChunk= *NewMANDChunkTmp;
														MANDChunkTmp = (struct MandelChunk*)NewMANDChunkTmp;
														MANDChunk = *MANDChunkTmp;
													}
													else
													{
														/* Image is a NEWRGB_SPECIAL_CHUNK 
														 * (24bit with color table inside NewMANDRGBChunk 
														 */
														/* printf("set pointers for newRGB format (CMAP inside FMVE): newRGB: %p new: %p\n", 
														   							NewRGBMANDChunkTmp, NewMANDChunkTmp); 
														*/
														
														if (NewRGBMANDChunkTmp == 0) NewRGBMANDChunkTmp = (struct NewRGBMandelChunk*)NewMANDChunkTmp;
														
														NewRGBMANDChunk = *NewRGBMANDChunkTmp;
														NewMANDChunkTmp = (struct NewMandelChunk*)NewRGBMANDChunkTmp;
														NewMANDChunk = *NewMANDChunkTmp;
														MANDChunkTmp = (struct MandelChunk*)NewMANDChunkTmp;
														MANDChunk = *MANDChunkTmp;
													}
												  } else LEGACY_PICTURE=TRUE; /* for ShowPicInfo() */
												  
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
 												  
												  if (ErrorLegacyFormat)
												  {
												  		/* set new data fields */
														BaseFractalType = FractalType = NewMANDChunk.FractalType;
                                                        
														/* switches */
														/* SWITCH_1 = JULIA */											
                                                        JULIA = NewMANDChunk.Switch[FMO_JULIA];
														/* SWITCH_2 (former: ANTIBUDDHA) */											
                                                        ANTIBUDDHA = NewMANDChunk.Switch[FMO_SWITCH_2] & (1<<FMO_BIT_ANTIBUDDHA);
                                                        FractalType = (NewMANDChunk.Switch[FMO_SWITCH_2] & (1<<FMO_BIT_BUDDHA)) ? BUDDHA : FractalType;
                                                        
														/* SWITCH_3 */											
                                                        BOUNDARY_FILL 		= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_BOUNDARY_FILL) ? 1 : 0);
                                                        USE_SYMMETRY 		= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_SYMMETRY)) ? 1 : 0;
                                                        USE_PERIODICITY 	= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_PERIODICITY)) ? 1 : 0;
                                                        USE_HAMMAP 			= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_HITMAP)) ? 1 : 0;
                                                        BUDDHA_EDGES 		= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_BUDDHA_EDGES)) ? 1 : 0;
                                                        BUDDHA_LEMNISCATES 	= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_BUDDHA_LEMNISCATES)) ? 1 : 0;
                                                        BUDDHA_RANDOM 		= (NewMANDChunk.Switch[FMO_SWITCH_3] & (1<<FMO_BIT_BUDDHA_RANDOM)) ? 1 : 0;
                                                        
                                                        /* SWITCH_4 (unused) */
                                                        
														/* algorithms */
														algorithm = NewMANDChunk.Algorithm[FMO_ALGORITHM_BASE];
														algorithm_buddha = NewMANDChunk.Algorithm[FMO_ALGORITHM_BUDDHA];
														
												  		/* set integer and float data 
                                                         * (for each fractal type) 
                                                         */
											   			switch (FractalType)
											   			{
											   				case MANDEL : 	/* no additional data */ 
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																	        BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
																			break;
															case MULTIINT : 
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																	        MultiPower = NewMANDChunk.IntData[FMO_MULTIPOWER]; 
																			BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
											   								break;
											   	    		case MULTIFLOAT : 
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																	        MultiPowerFloat = NewMANDChunk.FloatData[FMO_MULTIPOWERFLOAT]; 
																			BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
											   								break;
											   	    		case BURNINGSHIP : 
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																			BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
											   								break;
															case BUDDHA :	/* includes ANTIBUDDHA */
											   								RGB_ITER[FM_RED_MIN] = NewMANDChunk.IntData[FMO_FM_REDMIN];
											   								RGB_ITER[FM_RED_MAX] = NewMANDChunk.IntData[FMO_FM_REDITER];
											   								RGB_ITER[FM_GREEN_MIN] = NewMANDChunk.IntData[FMO_FM_GREENMIN];
											   								RGB_ITER[FM_GREEN_MAX] = NewMANDChunk.IntData[FMO_FM_GREENITER];
											   								RGB_ITER[FM_BLUE_MIN] = NewMANDChunk.IntData[FMO_FM_BLUEMIN];
											   								RGB_ITER[FM_BLUE_MAX] = NewMANDChunk.IntData[FMO_FM_BLUEITER];
																			BUDDHA_MIN_ITERATIONS = NewMANDChunk.IntData[FMO_BUDDHA_MIN_ITERATIONS];
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																	        PRIME_RED = NewMANDChunk.IntData[FMO_PRIME_RED];
                                                                            PRIME_GREEN = NewMANDChunk.IntData[FMO_PRIME_GREEN];
                                                                            PRIME_BLUE = NewMANDChunk.IntData[FMO_PRIME_BLUE];
                                                                            PRIME_DIVIDER = NewMANDChunk.IntData[FMO_PRIME_DIVIDER];
                                                                            
                                                                            HITMAP_EXTENSION = NewMANDChunk.IntData[FMO_HITMAP_EXTENSION] & (ULONG)0xff;
                                                                            HITMAP_SHIFT = (NewMANDChunk.IntData[FMO_HITMAP_EXTENSION] & (ULONG)0xff00)>>8;
                                                                            HITMAP_DURING_CALCULATION = (NewMANDChunk.IntData[FMO_HITMAP_EXTENSION] & (ULONG)(1<<16))>>16;
                                                                            HITMAP_AT_THE_END = (NewMANDChunk.IntData[FMO_HITMAP_EXTENSION] & (ULONG)(1<<17))>>17;
                                                                            
                                                                            HITMAP_COLORS[0] = NewMANDChunk.IntData[FMO_HITMAP_COLOR0];
                                                                            HITMAP_COLORS[1] = NewMANDChunk.IntData[FMO_HITMAP_COLOR1];
                                                                            HITMAP_COLORS[2] = NewMANDChunk.IntData[FMO_HITMAP_COLOR2];
                                                                            HITMAP_COLORS[3] = NewMANDChunk.IntData[FMO_HITMAP_COLOR3];
                                                                            
                                                                            BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
                                                                            MultiPower=NewMANDChunk.IntData[FMO_MULTIPOWER]; 
                                                                            MultiPowerFloat=NewMANDChunk.FloatData[FMO_MULTIPOWERFLOAT];
                                                                            PERIODICITY_EPSILON=NewMANDChunk.FloatData[FMO_PERIODICITY_EPSILON];
                                                                            
                                                                            /* integrated in 24.8 R17 */
                                                                            if (ANewerThanB(NewMANDChunk.Version, PackVersionWord(24,8,17)))
                                                                            {
                                                                            	/* printf("picture newer that 24.8 (R17)\n"); */
                                                                                BUDDHA_RANDOMIZE_X=NewMANDChunk.FloatData[FMO_BUDDHA_RANDOMIZE_X];
                                                                            	BUDDHA_RANDOMIZE_Y=NewMANDChunk.FloatData[FMO_BUDDHA_RANDOMIZE_Y];
                                                                            }
                                                                            else
                                                                            {
                                                                            	/* printf("picture older that 24.8 (R17)\n"); */
                                                                            	BUDDHA_RANDOMIZE_X=0.71;
                                                                            	BUDDHA_RANDOMIZE_Y=0.71;
                                                                            }
                                                                            
                                                                            /* correct a problem where MultiPower was not saved for Buddhabrots in 
                                                                               earlier versions */
                                                                            /* can be deleted once the files have been saved with corrected
                                                                               saving algorithm */
                                                                            if ((ULONG)MultiPowerFloat!=MultiPower) 
                                                                            {
                                                                                MultiPower=(ULONG)MultiPowerFloat;
																			}															
                                                                            break;
											   			}
														/* data for all fractal types 
                                                         * (= data that is always loaded) */
                                                        
                                                        /* strictly speaking these infos 
                                                         * are only valid for versions >24.8R1 
                                                         * => maybe check for that in ShowPicInfo() ?! 
                                                         */
                                                        ELAPSEDTIME=NewMANDChunk.FloatData[FMO_ELAPSEDTIME];
                                                        FM_PROCESSOR_INFO=NewMANDChunk.IntData[FMO_PROCESSOR_INFO];
                                                       
                                                        if (BUDDHA) ACCUMULATED_BUDDHA_TIME=ELAPSEDTIME;
                                                        
                                                  } 
												  else
												  {
												  		/* set new data fields 
                                                         * when old format picture 
                                                         * is loaded 
                                                         */
														
														FractalType = MANDEL;
														BaseFractalType = GetBaseFractalType(FractalType);
														
														/* the types didn't exist: 
                                                         * set them to NULL / default values 
                                                         * or leave as is 
                                                         */
														ANTIBUDDHA = FALSE;
														
														/* leave as is: 
														 * BUDDHA_MIN_ITERATIONS, FM_REDMIN, FM_REDITER, FM_GREENMIN, FM_GREENITER,
														 * FM_BLUEMIN, FM_BLUEITER
														 */
														
														algorithm = TILE;
														
														/* Bailout value of 4 is necessary 
                                                         * for legacy pictures 
                                                         */
														BAILOUT_VALUE=4;
														
												  		/* printf("Old picture loaded => Default values set\n"); */
												  }
                                                   
                                                  if (!LoadOnlyData)
                                                  {      
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
												  
                                                  }
                                                  else
                                                  {
                                                  	Ilbm->Bmhd.w=DD_WIDTH;
                                                    Ilbm->Bmhd.h=DD_HEIGHT;
                                                    Ilbm->Bmhd.nPlanes=GetBitMapAttr (Ilbm->wrp->BitMap,BMA_DEPTH);
                                                  
                                                  }
                                                  
                                                  SAFP(); 

												  SetMenuSelection();
												  
												  /* nebulabrot: load histogram */
										 		  if (FractalType == BUDDHA)
										 		  {
										 				NEBULA_WIDTH=Ilbm->win->Width;
														NEBULA_HEIGHT=Ilbm->win->Height;
														
														if (!LoadOnlyData)
                                                        {											
                                                        	/* make sure memory for Buddha has been allocated */
                                                            AllocBuddha();	
                                                            
                                                            /* printf("Load histogram: Width: %u Height: %u\n", NEBULA_WIDTH, NEBULA_HEIGHT); */
															if (!(templong=LoadHistogram(AddFilenameExtension(MYPATH),BuddhaCounter,NEBULA_WIDTH*NEBULA_HEIGHT*3*4)))
															{
																MenuResumeOn(MYILBM.win);
															}
															else 
															{
																MenuResumeOff(MYILBM.win);
																printf("Error loading histogram: %u\n", templong);
												  			}
														}									  
                                                  }	
										  
                                                  if (ZMASK & MASK) MASK ^= ZMASK;

                                                  CURRENT_MAX_COLORS -= RESERVED_PENS;

                                                if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                                  if (!ErrorLegacyFormat) 
												  {
														if (!LoadOnlyData)
                                                        {								  		
                                                        	if (LoadMandPic (Ilbm,MYPATH)) Fail (LoadMandPicError,5L);
                                                        }
                                                        else
                                                        {
                                                            if (LoadPalette(Ilbm, MYPATH, NewRGBMANDChunk.Colors)) Fail(LoadMandPalError,5L);
                                                        }
												  }
												  else
												  {
												  		if (GetBitMapAttr (Ilbm->wrp->BitMap,BMA_DEPTH) <= 8)
														{
															/* printf("Load a classic image <=256 colors\n"); */
															Ilbm->ParseInfo.propchks = NewIlbmProps;
															if (!LoadOnlyData)
                                                            {
                                                            	if (LoadMandPic (Ilbm, MYPATH)) Fail (LoadMandPicError,5L);
                                                            }
                                                            /* restore props to legacy */
															Ilbm->ParseInfo.propchks = IlbmProps;
                                                        }
														else
														{
															/* printf("Load an rgb image >256 colors\n"); */
															Ilbm->ParseInfo.propchks = NewIlbmProps;
															if (!LoadOnlyData)
                                                        	{												
                                                            	if (LoadMandPicRGB (Ilbm, MYPATH)) {
                                                                    Fail (LoadMandPicError,5L);
                                                            	}
                                                            }
                                                            /* NOTE: 24bit images have no palette
                                                               ("palette" is implicitely loaded
                                                               inside NewRGBMandChunk
                                                             */
                                                            
                                                            /* restore props to legacy */
															Ilbm->ParseInfo.propchks = IlbmProps;
												  		}
												  }

                                      			  if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                                  /* only use GetRGB32 if palette 
                                                   * has not been set (on truecolor image) 
                                                   */
                                                   
												  if (!truecolorimage) GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);
												  
												  /* if it is a truecolor image, 
                                                   * define palette here 
                                                   */
												  
												  if (truecolorimage) 
												  {
											   			/* picture is a NEWRGB_SPECIAL_CHUNK with colortable 
                                                         * => copy colors to COLORS_AGA 
                                                         */
											   			for (temp=0; temp<=255; temp++)
											   			{
															templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3];
															COLORS_AGA[1+temp*3] = templong | templong << 8 | templong << 16 | templong << 24;
																
															templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3+1];
															COLORS_AGA[1+temp*3+1] = templong | templong << 8 | templong << 16 | templong << 24;
																
															templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3+2];
													   		COLORS_AGA[1+temp*3+2] = templong | templong << 8 | templong << 16 | templong << 24;
													   }
													
													   /* set palette */
													   alloccolortableRGB(Ilbm);
													   loadcmapRGB(Ilbm, NewRGBMANDChunk.Colors+2);
													   setcolors(Ilbm, Ilbm->vp);
													
													   /* is this necessary ?! */   
													   freecolors(Ilbm);	   
												  }
                                                  
                              
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
												  
												  SetMenuSelection();

                                                  ResetMenuStrip (Ilbm->win,MAINMENU);
                                               
											   	  SetUpDirectDrawing(Ilbm->win);
                                             
                                                  if (!LoadOnlyData) 
                                                  {
                                                  	ShowPicInfo(Ilbm->win);
                                                  }
                                                  else 
                                                  {
                                                        if ((truecolorimage) && (DD_BPP<=1))	
                                                        {
                                                        	/* if data for a 24bit image has been loaded
                                                               but actual screen mode is less then show
                                                               an info to user to change manually to a
                                                               24 bit mode before recalculating the image 
                                                             */	
                                                            ShowChangeManuallyTo24Bit(Ilbm->win);
                                                        }
                                                        else
                                                        {
                                                        	ShowDataLoadedInfo(Ilbm->win);
                                                  
                                                  			/* recalculate */				 
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
											   		printf("Both legacy and new formats failed. (Errors: legacy: %d new: %d newRGB: %d)\n", ErrorLegacyFormat, ErrorNewFormat, ErrorNewRGBFormat);
													Fail (QueryMandPicError,NULL);
											   }
											   
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
                                            if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);
											   
										    PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
                                            
                                            /* filename in MYPATH - use new format */
                                            PrepareAndSaveAllNewSpecialChunkPic(Ilbm);
                                            
                                            PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);
                                         }
                                         break;
                                      }

                                      if (MyMenu & LOADPALETTE_MSG)
                                      {
                                         if (FileRequest (Ilbm->win,"Load palette","FLASHMANDEL:Palettes",PALETTES_DRAWER,FALSE))
                                         {
                                            PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

                                            if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
                                               if (LoadPalette (Ilbm,MYPATH, NewRGBMANDChunk.Colors)) Fail (LoadMandPalError,5L);

                                               GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);


											   for (temp=0; temp<=255; temp++)
											   {
													templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3];
													COLORS_AGA[1+temp*3] = templong | templong << 8 | templong << 16 | templong << 24;
																
													templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3+1];
													COLORS_AGA[1+temp*3+1] = templong | templong << 8 | templong << 16 | templong << 24;
																
													templong = (ULONG)NewRGBMANDChunk.Colors[2+temp*3+2];
													COLORS_AGA[1+temp*3+2] = templong | templong << 8 | templong << 16 | templong << 24;
											   }
														   
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
                                            PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

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

                                      /* font requester disabled */
                                      /*
                                      if (MyMenu & FONTREQ_MSG)
                                      {
                                         if (FontRequest (Ilbm->win))
                                         {
                                            if (ZMASK & MASK)
                                            {
                                               DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                               MASK ^= ZMASK;
                                            }

                                            PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

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
                                      */

                                      /* print function disabled */
                                      /*
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
									  */
                                      /* preview disabled */
                                      /*
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
									  */
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
                                      	 NewMANDChunk.Version = 0; /* for ShowPicInfo() */
                                         LEGACY_PICTURE=FALSE;
                                         
										 if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         SetMenuStop (Ilbm->win);
                                         
                                         if (!BOUNDARY_FILL) SetRast(Ilbm->win->RPort,0);
                                         

                                         PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);

/* some code to measure icache/dcache hits/miss 
 * (disabled for the moment)
 */
/*
	dcache_hits_start=get_dcache_hits();
    dcache_miss_start=get_dcache_miss();
    icache_hits_start=get_icache_hits();
    icache_miss_start=get_icache_miss();
    mips1_1=get_mips1();
 */		
                                         ELAPSEDTIME = DrawFractalWrapper (/*Ilbm->win*/MYILBM.win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);
/*
	mips1_2=get_mips1();
                    
	dcache_hits_end=get_dcache_hits();
    dcache_miss_end=get_dcache_miss();
    icache_hits_end=get_icache_hits();
    icache_miss_end=get_icache_miss();
                    
    printf("\n-----\ndcache_hits: start: %d end: %d => %d\n",
           	dcache_hits_start, dcache_hits_end, CCD(dcache_hits_start,dcache_hits_end));
    printf("dcache_miss: start: %d end: %d => %d\n",
           	dcache_miss_start, dcache_miss_end, CCD(dcache_miss_start,dcache_miss_end));
    printf("icache_hits: start: %d end: %d => %d\n",
           	icache_hits_start, icache_hits_end, CCD(icache_hits_start,icache_hits_end));
    printf("icache_miss: start: %d end: %d => %d\n",
          	icache_miss_start, icache_miss_end, CCD(icache_miss_start,icache_miss_end));
    printf("mips1: [1] %lu [2]: %lu => %lu\n\n", mips1_1, mips1_2, CCD(mips1_1, mips1_2));
*/
                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         SetMenuStart (Ilbm->win);

                                         ShowTime (Ilbm->win,"Recalculate elapsed time:",ELAPSEDTIME);

                                         break;
                                      }

                                      if (MyMenu & DRAW_MSG)
                                      {
                                         NewMANDChunk.Version = 0; /* for ShowPicInfo() */
                                         LEGACY_PICTURE=FALSE;
                                         								 
                                         if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         if (NewCoords (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4],ZOOMLINE [5]))
                                         {
                                            MYBITMAP = CopyBitMap (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);
                  
                                            if (algorithm==BOUNDARY)
                                            {
                                            	if (BOUNDARY_FILL) PasteBitMapStandard (MYBITMAP,Ilbm->win,ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);
												else SetRast(Ilbm->win->RPort,0);
                                            } else PasteBitMapStandard (MYBITMAP,Ilbm->win,ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);
                                            
                                            FMG_BUDDHA_ZOOM_IN=TRUE;
                                            
                                            SetMenuStop (Ilbm->win);

                                            PutPointer (Ilbm->win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
                                            
											ELAPSEDTIME = DrawFractalWrapper (MYILBM.win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);

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

                                         HandleBarString(RealCoord, ImagCoord);
                                           
                                         SetWindowTitles (Ilbm->win,(TEXT *) ~0,BAR_STRING);
                              
                                      }

                                      break;

             case       IDCMP_RAWKEY: if ((MyCode == RAW_ESC) || (MyCode == RAW_DEL)) {
			 						
                                    		if (DrawOrbits==1) 
											{
												DrawOrbits=0;
												
                                                /* let last orbit visible or delete it
                                                 * blitting in a screen copy
                                                 */
                                                if (MyCode == RAW_DEL) BltBitMap(ScrCpyBuffer, 0,0, Ilbm->win->RPort->BitMap,0,0,Ilbm->win->Width,Ilbm->win->Height, 0xc0, 0xff,NULL);
                                                FreeIterArray();
                                                
                                                if (ScrCpyBuffer) { FreeBitMap(ScrCpyBuffer); ScrCpyBuffer=NULL; }									
                                            	
                                                OnTitle(MYILBM.scr);
                                            }
											else MyMenu = EXIT_MSG;
		
        							  		SetMenuSelection();
									  }

                                      if (MyCode == HELP) Execute (SHOWGUIDECOMMAND,NULL,NULL);
                                      
									  break;

             case  IDCMP_CLOSEWINDOW: MyMenu = EXIT_MSG;
      }

    } while (! ((MyMenu & EXIT_MSG) || (MyMenu & NEWDISPLAY_MSG)));

 return MyMenu;
}
