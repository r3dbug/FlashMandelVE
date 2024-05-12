
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
      /* OnMenu  (Win,FULLMENUNUM (2,0,NOSUB)); */ /* discable Calculate -> Preview */

      OnMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }

   else
   {
      /* OffMenu  (Win,FULLMENUNUM (2,0,NOSUB)); */

      OffMenu  (Win,FULLMENUNUM (2,6,NOSUB));
   }
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
  printf("End of FontRequest: Success = %u\n", Success);
  printf("MYFONTSTRUCT.ta_Name: %s\n", MYFONTSTRUCT.ta_Name);
  printf("MyFontReq->fo_Attr.ta_Name: %s\n", MyFontReq->fo_Attr.ta_Name);
  
  return Success;
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

	if (FractalType==BUDDHA)
	{
	  /* printf("Special calculation for BUDDHA zoom ...\n"); */
 
	  if (((a2 - a1) < 2L) || ((b2 - b1) < 2L)) return FALSE;

	  KReal = fabs (RMAX-RMIN) / ((double) Win->Height);

	  KImag = fabs (IMAX-IMIN) / ((double) Win->Width);

	  SaveCoords (Win,TRUE);

	  /* printf("OLD COORDS: RMIN: %f RMAX: %f IMIN: %f IMAX: %f\n", RMIN, RMAX, IMIN, IMAX); */
  
	  rmaxt = RMIN + ((double) b2 * KReal);

	  imint = IMIN + ((double) a1 * KImag);
	  
	  rmint = RMIN + ((double) b1 * KReal);

	  imaxt = IMIN + ((double) a2 * KImag);
  
  
	  RMAX = rmaxt;
	  RMIN = rmint;
	  IMIN = imint;
	  IMAX = imaxt;
  
	  /* printf("NEW COORDS: RMIN: %f RMAX: %f IMIN: %f IMAX: %f\n", RMIN, RMAX, IMIN, IMAX); */
  
	  return TRUE;
  	
	}
	else
	{
	  if (((a2 - a1) < 2L) || ((b2 - b1) < 2L)) return FALSE;
	
	  KReal = fabs (RMAX-RMIN) / ((double) Win->Width);
	
	  KImag = fabs (IMAX-IMIN) / ((double) Win->Height);

	  SaveCoords (Win,TRUE);

	  /* printf("OLD COORDS: RMIN: %f RMAX: %f IMIN: %f IMAX: %f\n", RMIN, RMAX, IMIN, IMAX); */
  
	  RMAX = RMIN + ((double) a2 * KReal);

	  IMIN = IMAX - ((double) b2 * KImag);

	  RMIN += ((double) a1 * KReal);

	  IMAX -= ((double) b1 * KImag);

	  /* printf("NEW COORDS: RMIN: %f RMAX: %f IMIN: %f IMAX: %f\n", RMIN, RMAX, IMIN, IMAX); */
  
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
// quick workaround for compiling ************
        Success = AddPart (MYPATH,MYFILE,/*20*/ /*sizeof (MYPATH)*/ /*strlen(MYPATH)*/ MAX_PATHLEN);
     } else Success = FALSE; /* when Cancel is pressed ?! */

     FreeAslRequest (MyFileReq);

	 if (!Success) return FALSE;
	 
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

void DisableDirectDrawing(void)
{
	if (drawing == DRAW_DIRECT)
    { 
    	/*printf("Select AmigaOS drawing (drawing: %d => 0)\n",
      		drawing);*/
       	switch_back_to_direct=TRUE;	
   	   	drawing=DRAW_CLASSIC;
   	   	SetUpDirectDrawing(MYILBM.win);
   	}
}

void EnableDirectDrawing(void)
{
	if (switch_back_to_direct)
    {
    	/*printf("Switch back to direct drawing (drawing: %d => 1).\n");*/	
        drawing=DRAW_DIRECT;
        switch_back_to_direct=FALSE;
        SetUpDirectDrawing(MYILBM.win);
    }
}

ULONG HandleEvents (struct ILBMInfo *Ilbm)
{
struct IntuiMessage *Message;

WORD MouseX = 0,MouseY = 0;

UWORD MyCode = 0;

/*LONG Error;*/
LONG ErrorLegacyFormat, ErrorNewFormat, ErrorNewRGBFormat;

ULONG MyClass = NULL,MyMenu = NULL,Index;

double RealCoord,ImagCoord,ScrRatio,FracRatio;
int iter, final_iter, rpx, rpy;
double xn,yn,/*xtemp,*/stepi, stepr, cx, cy;
ULONG templong;
WORD temp;
BOOL truecolorimage=FALSE;
/*UBYTE tempb;*/

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
		   SetAPen(Ilbm->win->RPort,12);
		   
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
		
								/*xtemp=xn*xn-yn*yn+cx;
							      yn=2*xn*yn+cy;		
		   						  xn=xtemp;
								  */
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

                                         if (optimized) sprintf (BAR_STRING,"Vampire - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);
                                         else sprintf (BAR_STRING,"Classic - Real %#+2.15Lf Imag %#+2.15Lfi W:H %.3Lf R:I %.3Lf",RealCoord,ImagCoord,ScrRatio,FracRatio);

                                         WaitTOF ();

                                         SetWindowTitles (Ilbm->win,(TEXT *) ~0,BAR_STRING);
                                      }

                                      break;

             case IDCMP_MOUSEBUTTONS: /*printf("IDCMP_MOUSEBUTTONS: MyCode: %d \n",
             										MyCode);*/
                                      
			 						  if (MyCode == SELECTDOWN) 
                                      {
			 								
											ProcessMouse (Ilbm->win,MouseX,MouseY);
									  }
                                      break;

             case     IDCMP_MENUPICK: /*printf("IDCMP_MENUPICK: MyCode: %d MENUNULL: %d\n", 
             								MyCode, MENUNULL);*/
             
                                      /*if (MyCode != MENUNULL)*/ EnableDirectDrawing();
                                      
             						  MyMenu = ProcessMenu (Ilbm->win,MyCode);
                                      
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
                                            templong = (LONG) IntegerGad (Ilbm->win,"Iterations requester","Insert new iterations...","_Iterations",MAX_ITERATIONS + 1L);
											
											if (templong==0xffffffff) break;
											
											templong-=1;
											
											MAX_ITERATIONS = /*(templong!=0xffffffff) ?*/ (UWORD)templong /*: MAX_ITERATIONS*/;
										  
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
											   			
														/*
														if (ErrorNewRGBFormat) 
														{
															printf("Query error with newRGB format: error = %d\n", ErrorNewFormat);
														} else printf("NewRGB format worked (loaded).\n");
											   			*/
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
														   (24bit with color table inside NewMANDRGBChunk 
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
												  }
												  
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
														/* printf("Version: Year: %u Month: %u Day: %u\n",
														 		2000 + GetYear(NewMANDChunk.Version),
																GetMonth(NewMANDChunk.Version),
																GetDay(NewMANDChunk.Version)
															);
														*/
														
														/* UWORD Version / NewMandChunk.Version = 1 */
														FractalType = NewMANDChunk.FractalType;
														BaseFractalType = GetBaseFractalType(FractalType);
														/* UWORD Flags / NewMANDChunk.Flags = NULL */
														
														/* switches */
														JULIA = NewMANDChunk.Switch[FMO_JULIA];
														ANTIBUDDHA = NewMANDChunk.Switch[FMO_ANTIBUDDHA];
														
														/* algorithms */
														algorithm = NewMANDChunk.Algorithm[FMO_ALGORITHM_BASE];
														algorithm_buddha = NewMANDChunk.Algorithm[FMO_ALGORITHM_BUDDHA];
														
												  		/* set integer and float data (for each fractal type) */
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
											   								FM_REDMIN = NewMANDChunk.IntData[FMO_FM_REDMIN];
											   								FM_REDITER = NewMANDChunk.IntData[FMO_FM_REDITER];
											   								FM_GREENMIN = NewMANDChunk.IntData[FMO_FM_GREENMIN];
											   								FM_GREENITER = NewMANDChunk.IntData[FMO_FM_GREENITER];
											   								FM_BLUEMIN = NewMANDChunk.IntData[FMO_FM_BLUEMIN];
											   								FM_BLUEITER = NewMANDChunk.IntData[FMO_FM_BLUEITER];
																			BUDDHA_MIN_ITERATIONS = NewMANDChunk.IntData[FMO_BUDDHA_MIN_ITERATIONS];
																			SMOOTH_COLORING=NewMANDChunk.IntData[FMO_SMOOTH_COLORING];
																	        BAILOUT_VALUE=NewMANDChunk.FloatData[FMO_BAILOUT_VALUE];
																			break;
											   			}
											   
												  		
												  
												  
												  } 
												  else
												  {
												  		/* set new data fields when old format picture is loaded */
														
														/* UWORD Version; */
														FractalType = MANDEL;
														BaseFractalType = GetBaseFractalType(FractalType);
														/* UWORD Flags; */
														/* JULIA = NewMANDChunk.Julia; */ /* JULIA is set by old MANDChunk */
														
														/* the types didn't exist: set them to NULL / default values or leave as is */
														ANTIBUDDHA = FALSE;
														
														/* leave as is: 
														   BUDDHA_MIN_ITERATIONS, FM_REDMIN, FM_REDITER, FM_GREENMIN, FM_GREENITER,
														   FM_BLUEMIN, FM_BLUEITER
														*/
														
														algorithm = TILE;
														
														/* Bailout value of 4 is necessary for legacy pictures */
														BAILOUT_VALUE=4;
														
												  		/* printf("Old picture loaded => Default values set\n"); */
												  }
												  
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
												  
												  /* nebulabrot: load histogram */
										 		  if (FractalType == BUDDHA)
										 		  {
										 				NEBULA_WIDTH=MYILBM.win->Width;
														NEBULA_HEIGHT=MYILBM.win->Height;
														
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

												  SAFP();
												  SetMenuSelection();
												  
                                                  if (ZMASK & MASK) MASK ^= ZMASK;

                                                  CURRENT_MAX_COLORS -= RESERVED_PENS;

                                                  if (TMASK & MASK) ShowTitle (Ilbm->scr,FALSE);

                                                  if (!ErrorLegacyFormat) 
												  {
												  		if (LoadMandPic (Ilbm,MYPATH)) Fail (LoadMandPicError,5L);
												  }
												  else
												  {
												  		if (GetBitMapAttr (Ilbm->wrp->BitMap,BMA_DEPTH) <= 8)
														{
															/* printf("Load a classic image <=256 colors\n"); */
															Ilbm->ParseInfo.propchks = NewIlbmProps;
															if (LoadMandPic (Ilbm, MYPATH)) Fail (LoadMandPicError,5L);
												  			/* restore props to legacy */
															Ilbm->ParseInfo.propchks = IlbmProps;
														}
														else
														{
															/* printf("Load a classic image <=256 colors\n"); */
															Ilbm->ParseInfo.propchks = NewIlbmProps;
															if (LoadMandPicRGB (Ilbm, MYPATH)) Fail (LoadMandPicError,5L);
												  			/* restore props to legacy */
															Ilbm->ParseInfo.propchks = IlbmProps;
												  		}
												  }
												  
                                                  if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                                  /* only use GetRGB32 if palette has not been set (on truecolor image) */
												  /*if (!(NewMANDChunkTmp->Special & TRUECOLOR_IMAGE))
												   		GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);
												  */
												  if (!truecolorimage) GetRGB32 (Ilbm->vp->ColorMap,0L,(ULONG) Ilbm->vp->ColorMap->Count,PALETTE + 1L);
												  
												  /* if it is a truecolor image, define palette here */
												  
												  if (truecolorimage) 
												  {
											   			/* picture is a NEWRGB_SPECIAL_CHUNK with colortable => copy colors to COLORS_AGA */
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
												  
												  SetMenuSelection();

                                                  ResetMenuStrip (Ilbm->win,MAINMENU);
                                               
											   	  SetUpDirectDrawing(Ilbm->win);
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
											   
										    PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

                                            /* save in new format */
											/* Ilbm->ParseInfo.propchks=NewIlbmProps;*/
											
											if (Ilbm->ParseInfo.iff = AllocIFF ())
                                            {
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
                                               
											   /* mark TRUECOLOR_IMAGE in MANDChunk.Special if it is a truecolor screen */
											   if (GetBitMapBPP(Ilbm->win->RPort->BitMap) >= 3)
											   {
											   		NewMANDChunk.Special |= TRUECOLOR_IMAGE;
													/* maybe mark this in (new) Flags ?! => doesn't seem to work: keep Special for the moment */
													NewMANDChunk.Flags |= TRUECOLOR_IMAGE;
											   	    NewRGBMANDChunk.Flags |= TRUECOLOR_IMAGE;
													/* printf("NewMANDChunk.Flags: %u ored with TRUECOLOR_IMAGE: %u\n",
														NewMANDChunk.Flags, NewMANDChunk.Flags & TRUECOLOR_IMAGE);
											   		*/
											   }
											   
											   if (ZMASK & MASK)
                                               {
                                                  DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                                  MASK ^= ZMASK;
                                               }
											   
											   /* fill in new data fields */
											   NewMANDChunk.Version = PackVersionWord( VERSION_YEAR, VERSION_MONTH, VERSION_DAY ); /* FlashMandelVE Version (evtl. divide into two bytes MAJOR/SUB) */
											   NewMANDChunk.FractalType = FractalType;
											   NewMANDChunk.Flags = NULL; /* default value: NULL */
											   
											   /* algorithmm */
											   NewMANDChunk.Algorithm[FMO_ALGORITHM_BASE] = algorithm;
											   NewMANDChunk.Algorithm[FMO_ALGORITHM_BUDDHA] = algorithm_buddha;
											   NewMANDChunk.Algorithm[FMO_ALGORITHM_3] = NULL;
											   NewMANDChunk.Algorithm[FMO_ALGORITHM_4] = NULL;
											   
											   /* switches */
											   NewMANDChunk.Switch[FMO_JULIA] = JULIA;
											   NewMANDChunk.Switch[FMO_ANTIBUDDHA] = ANTIBUDDHA;
											   NewMANDChunk.Switch[FMO_SWITCH_3] = NULL;
											   NewMANDChunk.Switch[FMO_SWITCH_4] = NULL;
											
											   /* integer and float data (only set used fields for each fractal type 
											   	  and leave other fields as is)
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
											   						NewMANDChunk.IntData[FMO_FM_REDMIN] = FM_REDMIN;
											   						NewMANDChunk.IntData[FMO_FM_REDITER] = FM_REDITER;
											   						NewMANDChunk.IntData[FMO_FM_GREENMIN]= FM_GREENMIN;
											   						NewMANDChunk.IntData[FMO_FM_GREENITER] = FM_GREENITER;
											   						NewMANDChunk.IntData[FMO_FM_BLUEMIN] = FM_BLUEMIN;
											   						NewMANDChunk.IntData[FMO_FM_BLUEITER] = FM_BLUEITER;
																	NewMANDChunk.IntData[FMO_BUDDHA_MIN_ITERATIONS] =BUDDHA_MIN_ITERATIONS;
																	NewMANDChunk.IntData[FMO_SMOOTH_COLORING] =SMOOTH_COLORING;
																	NewMANDChunk.FloatData[FMO_BAILOUT_VALUE]=BAILOUT_VALUE;
																	break;
											   }
											   
											   /* copy NewMANDChunk data to NewRGBMANDChunk */
											   memcpy(&NewRGBMANDChunk, &NewMANDChunk, sizeof(NewMANDChunk));
											   
											   /* for truecolor images: add color map inside NewRGBMANDChunk */
											   for (temp=0; temp<=255; temp++)
											   {
											   		NewRGBMANDChunk.Colors[2+temp*3] = (UBYTE)COLORS_AGA[1+temp*3];
													NewRGBMANDChunk.Colors[2+temp*3+1] = (UBYTE)COLORS_AGA[1+temp*3+1];
													NewRGBMANDChunk.Colors[2+temp*3+2] = (UBYTE)COLORS_AGA[1+temp*3+2];
											   }

											   /* save picture with NEW_SPECIAL_CHUNK (new format) */
											   switch (DD_BPP)
											   {
											   		case 3 :
													case 4 :
															/* use NewRGBMANDChunk for truecolor images (no CMAP) */
															/* printf("Save truecolor image (no CMAP, extended FMVE chunk).\n"); */
															Ilbm->ParseInfo.propchks=NewRGBIlbmProps;
															if (SaveMandPic (Ilbm,&NEWRGB_SPECIAL_CHUNK,&COPYRIGHT_CHUNK,MYPATH)) Fail (SaveMandPicError,5L);
															break;
											   		default :
											   				/* use NewMANDChunk for 8bit image with CMAP */
															/* printf("Save 8bit image (with CMAP, standard FMVE chunk\n"); */
															Ilbm->ParseInfo.propchks=NewIlbmProps;
															if (SaveMandPic (Ilbm,&NEW_SPECIAL_CHUNK,&COPYRIGHT_CHUNK,MYPATH)) Fail (SaveMandPicError,5L);
											   }

                                               if (TMASK & MASK) ShowTitle (Ilbm->scr,TRUE);

                                               FreeIFF (Ilbm->ParseInfo.iff);
                                            }
                                         }
										 
										 /* nebulabrot: save histogram */
										 if (FractalType == BUDDHA)
										 {
										 	if (histogram_save)
											{
												NEBULA_WIDTH=Ilbm->win->Width;
												NEBULA_HEIGHT=Ilbm->win->Height;
											
												/* printf("Save histogram: Width: %u Height: %u\n", NEBULA_WIDTH, NEBULA_HEIGHT); */
												if (!(templong=SaveHistogram(AddFilenameExtension(MYPATH),&BuddhaCounter[0],NEBULA_WIDTH*NEBULA_HEIGHT*3*4, histogram_compression)));
												else printf("Error saving histogram: %u\n", templong);
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
										 /*printf("MyMenu & REDRAW_MSG\n");*/
										 
										 if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         SetMenuStop (Ilbm->win);

                                         PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

										 ELAPSEDTIME = DrawFractal (/*Ilbm->win*/MYILBM.win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);

                                         PutPointer (Ilbm->win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

                                         SetMenuStart (Ilbm->win);

                                         ShowTime (Ilbm->win,"Recalculate elapsed time:",ELAPSEDTIME);

                                         break;
                                      }

                                      if (MyMenu & DRAW_MSG)
                                      {
                                         /*printf("MyMenu & DRAW_MSG\n");*/
										 
										 if (ZMASK & MASK)
                                         {
                                            DrawBorder (Ilbm->wrp,&MYBORDER,0,0);

                                            MASK ^= ZMASK;
                                         }

                                         if (NewCoords (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4],ZOOMLINE [5]))
                                         {
                                            MYBITMAP = CopyBitMap (Ilbm->win,ZOOMLINE [6],ZOOMLINE [3],ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

                                            PasteBitMapStandard (MYBITMAP,Ilbm->win,ZOOMLINE [4]-ZOOMLINE [6]+1,ZOOMLINE [5]-ZOOMLINE [3]+1);

                                            SetMenuStop (Ilbm->win);

                                            PutPointer (Ilbm->win,0,0,0,0,0,BUSY_POINTER);

											ELAPSEDTIME = DrawFractal (/*Ilbm->win*/MYILBM.win,(LONG) (Ilbm->win->LeftEdge),(LONG) (Ilbm->win->TopEdge),(LONG) (Ilbm->win->Width) - 1L,(LONG) (Ilbm->win->Height) - 1L);

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

             case       IDCMP_RAWKEY: /*printf("RAWKEY: MyCode: %d\n", MyCode);*/
			 						  if (MyCode == RAW_ESC) {
			 								if (DrawOrbits==1) 
											{
												DrawOrbits=0;
												BltBitMap(ScrCpyBuffer, 0,0, Ilbm->win->RPort->BitMap,0,0,Ilbm->win->Width,Ilbm->win->Height, 0xc0, 0xff,NULL);
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
