/*******************************************************************************
**
**  Coded by Dino Papararo                 29-Nov-2000
**
*******************************************************************************/

#include "ColorWindow.h"

ULONG COLOR_RGB [3L * 256L + 2L],UNDO_RGB [3L * 256L + 2L],COPY_RGB [3L];

struct NewGadget BUTTON_GAD,SLIDER_GAD,PALETTE_GAD;

BOOL ModifyPalette (struct Window *Win,WORD LeftEdge,WORD TopEdge,ULONG *Palette32)
{
  struct Window *ColorWin;

  struct IntuiMessage *Message;

  struct Gadget *GadList = NULL,*MyButtonGad,*MyPaletteGad;

  struct Gadget *RedSliderGad,*GreenSliderGad,*BlueSliderGad,*MyGad;

  BOOL Copy_Msg = FALSE,Swap_Msg = FALSE,Spread_Msg = FALSE,Exit = FALSE;

  ULONG SelectedPen = STARTPEN,OldPen = NULL,NewPen;

  UWORD MyCode;

  ULONG MyClass,Colors,ColorBase = 3L * STARTPEN + 1L;

  ULONG RedLevel,GreenLevel,BlueLevel;

  CPTR *VInfo;

  Colors = 1L << Win->RPort->BitMap->Depth;

  if (Colors < 2L) return FALSE;

  VInfo = GetVisualInfo (Win->WScreen,TAG_END);

  if (! VInfo) return FALSE;

  COLOR_RGB [0L] = UNDO_RGB [0L] = Colors << 16L;

  COLOR_RGB [3L * Colors + 1L] = UNDO_RGB [3L * Colors + 1L] = NULL;

  COPY_RGB [0L] = COPY_RGB [1L] = COPY_RGB [2L] = NULL;

  GetRGB32 (ViewPortAddress (Win)->ColorMap,0L,Colors,&COLOR_RGB [1L]);

  GetRGB32 (ViewPortAddress (Win)->ColorMap,0L,Colors,&UNDO_RGB [1L]);

  RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

  GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

  BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

  MyButtonGad = CreateContext (&GadList);

  BUTTON_GAD.ng_VisualInfo = PALETTE_GAD.ng_VisualInfo = SLIDER_GAD.ng_VisualInfo = VInfo;

  BUTTON_GAD.ng_LeftEdge   = 266;

  BUTTON_GAD.ng_TopEdge    = 11;

  BUTTON_GAD.ng_Width      = 90;

  BUTTON_GAD.ng_Height     = (Win->WScreen->Font->ta_YSize * 3) >> 1;

  BUTTON_GAD.ng_GadgetText = "C_opy";

  BUTTON_GAD.ng_GadgetID   = COPY;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "S_wap";

  BUTTON_GAD.ng_GadgetID   = SWAP;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "_Spread";

  BUTTON_GAD.ng_GadgetID   = SPREAD;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "_Invert";

  BUTTON_GAD.ng_GadgetID   = INVERT;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "_<<";

  BUTTON_GAD.ng_GadgetID   = SHL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "_>>";

  BUTTON_GAD.ng_GadgetID   = SHR;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_TopEdge   += (10 + BUTTON_GAD.ng_Height);

  BUTTON_GAD.ng_GadgetText = "_Undo";

  BUTTON_GAD.ng_GadgetID   = UNDO;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  PALETTE_GAD.ng_LeftEdge   = 10;

  PALETTE_GAD.ng_TopEdge    = 10;

  PALETTE_GAD.ng_Width      = 250;

  PALETTE_GAD.ng_Height     = MAX (200,BUTTON_GAD.ng_TopEdge + BUTTON_GAD.ng_Height);

  PALETTE_GAD.ng_GadgetID   = CWPALETTE;

  MyPaletteGad = CreateGadget (PALETTE_KIND,MyButtonGad,&PALETTE_GAD,GTPA_Color,4,GTPA_IndicatorWidth,10,GTPA_NumColors,(UWORD) Colors,TAG_END);

  SLIDER_GAD.ng_LeftEdge    = 25;

  SLIDER_GAD.ng_TopEdge     = MAX (PALETTE_GAD.ng_TopEdge + PALETTE_GAD.ng_Height,BUTTON_GAD.ng_TopEdge + BUTTON_GAD.ng_Height) + 10;

  SLIDER_GAD.ng_Width       = 290;

  SLIDER_GAD.ng_Height      = Win->WScreen->Font->ta_YSize;

  SLIDER_GAD.ng_GadgetText  = "R";

  SLIDER_GAD.ng_GadgetID    = RED;

  RedSliderGad = CreateGadget (SLIDER_KIND,MyPaletteGad,&SLIDER_GAD,GA_RelVerify,TRUE,GTSL_Max,MAXVALUE,GTSL_Level,(WORD) RedLevel,GTSL_LevelFormat,"%03ld",GTSL_MaxLevelLen,4,GTSL_LevelPlace,PLACETEXT_RIGHT,TAG_END);

  SLIDER_GAD.ng_TopEdge    += (8 + SLIDER_GAD.ng_Height);

  SLIDER_GAD.ng_GadgetText  = "G";

  SLIDER_GAD.ng_GadgetID    = GREEN;

  GreenSliderGad = CreateGadget (SLIDER_KIND,RedSliderGad,&SLIDER_GAD,GA_RelVerify,TRUE,GTSL_Max,MAXVALUE,GTSL_Level,(WORD) GreenLevel,GTSL_LevelFormat,"%03ld",GTSL_MaxLevelLen,4,GTSL_LevelPlace,PLACETEXT_RIGHT,TAG_END);

  SLIDER_GAD.ng_TopEdge    += (8 + SLIDER_GAD.ng_Height);

  SLIDER_GAD.ng_GadgetText  = "B";

  SLIDER_GAD.ng_GadgetID    = BLUE;

  BlueSliderGad = CreateGadget (SLIDER_KIND,GreenSliderGad,&SLIDER_GAD,GA_RelVerify,TRUE,GTSL_Max,MAXVALUE,GTSL_Level,(WORD) BlueLevel,GTSL_LevelFormat,"%03ld",GTSL_MaxLevelLen,4,GTSL_LevelPlace,PLACETEXT_RIGHT,TAG_END);

  BUTTON_GAD.ng_LeftEdge   = 10;

  BUTTON_GAD.ng_TopEdge    =  SLIDER_GAD.ng_TopEdge + SLIDER_GAD.ng_Height + 20;

  BUTTON_GAD.ng_GadgetText = "_Accept";

  BUTTON_GAD.ng_GadgetID   = CWACCEPT;

  MyButtonGad = CreateGadget (BUTTON_KIND,BlueSliderGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  BUTTON_GAD.ng_LeftEdge  += (38 + BUTTON_GAD.ng_Width);

  BUTTON_GAD.ng_GadgetText = "Reset";

  BUTTON_GAD.ng_GadgetID   = CWRESET;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,TAG_END);

  BUTTON_GAD.ng_LeftEdge  += (38 + BUTTON_GAD.ng_Width);

  BUTTON_GAD.ng_GadgetText = "_Cancel";

  BUTTON_GAD.ng_GadgetID   = CWCANCEL;

  MyButtonGad = CreateGadget (BUTTON_KIND,MyButtonGad,&BUTTON_GAD,GT_Underscore,'_',TAG_END);

  if (! MyButtonGad)
  {
     FreeGadgets (GadList);

     FreeVisualInfo (VInfo);

     return FALSE;
  }

  ColorWin = OpenWindowTags (0,WA_Left,LeftEdge,
                               WA_Top,TopEdge,
                               WA_Width,BUTTON_GAD.ng_LeftEdge + BUTTON_GAD.ng_Width + 25,
                               WA_Height,BUTTON_GAD.ng_TopEdge + BUTTON_GAD.ng_Height + 35,
                               WA_Title,"Palette requester",
                               WA_ScreenTitle,"Modify palette...",
                               WA_CustomScreen,Win->WScreen,
                               WA_IDCMP,IDCMP_CLOSEWINDOW|IDCMP_REFRESHWINDOW|IDCMP_VANILLAKEY|IDCMP_GADGETDOWN|BUTTONIDCMP|SLIDERIDCMP|PALETTEIDCMP,
                               WA_Flags,WFLG_ACTIVATE|WFLG_DRAGBAR|WFLG_SIMPLE_REFRESH|WFLG_RMBTRAP|WFLG_GIMMEZEROZERO,
                               WA_Gadgets,GadList,
                               TAG_END);

  if (! ColorWin)
  {
     FreeGadgets (GadList);

     FreeVisualInfo (VInfo);

     return FALSE;
  }

  GT_RefreshWindow (ColorWin,NULL);

  do { WaitPort (ColorWin->UserPort);

       if (Message = (struct IntuiMessage *) GT_GetIMsg (ColorWin->UserPort))
       {
          MyGad   = (struct Gadget *) Message->IAddress;

          MyClass = Message->Class;

          MyCode  = Message->Code;

          GT_ReplyIMsg ((struct IntuiMessage *) Message);

          switch (MyClass)
          {
                 case IDCMP_REFRESHWINDOW : GT_BeginRefresh (ColorWin);

                                            GT_EndRefresh   (ColorWin,TRUE);

                                            break;

                 case IDCMP_VANILLAKEY    : switch (MyCode)
                                            {
                                                    case 'a' :
                                                    case 'A' : GetRGB32 (ViewPortAddress (ColorWin)->ColorMap,0L,Colors,&Palette32 [1L]);

                                                               Exit = TRUE;

                                                               break;

                                                    case 'c' :
                                                    case 'C' : LoadRGB32 (ViewPortAddress (ColorWin),Palette32);

                                                               Exit = TRUE;

                                                               break;

                                                    case 'o' :
                                                    case 'O' : Copy (ColorWin,SelectedPen);

                                                               Copy_Msg = TRUE;

                                                               Swap_Msg = FALSE;

                                                               Spread_Msg = FALSE;

                                                               break;

                                                    case 'w' :
                                                    case 'W' : OldPen = SelectedPen;

                                                               Copy_Msg = FALSE;

                                                               Swap_Msg = TRUE;

                                                               Spread_Msg = FALSE;

                                                               break;

                                                    case 's' :
                                                    case 'S' : OldPen = SelectedPen;

                                                               Copy_Msg = FALSE;

                                                               Swap_Msg = FALSE;

                                                               Spread_Msg = TRUE;

                                                               break;

                                                    case '<' : KeepPalette (ColorWin);

                                                               Shl (ColorWin,STARTPEN,ENDPEN);

                                                               break;

                                                    case '>' : KeepPalette (ColorWin);

                                                               Shr (ColorWin,STARTPEN,ENDPEN);

                                                               break;

                                                    case 'u' :
                                                    case 'U' : LoadRGB32 (ViewPortAddress (ColorWin),UNDO_RGB);

                                                               GetRGB32 (ViewPortAddress (ColorWin)->ColorMap,0L,Colors,&COLOR_RGB [1L]);

                                                               RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                               GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                               BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                               GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                               GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                               GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                               break;

                                                    case 'E' : RedLevel = COLOR_RGB [ColorBase] >> 24L;

                                                               if (RedLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  RedLevel = (RedLevel < (MINVALUE + DELTA)) ? MINVALUE : RedLevel - DELTA;

                                                                  GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                  COLOR_RGB [ColorBase] = RedLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'e' : RedLevel = COLOR_RGB [ColorBase] >> 24L;

                                                               if (RedLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  RedLevel--;

                                                                  GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                  COLOR_RGB [ColorBase] = RedLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'T' : RedLevel = COLOR_RGB [ColorBase] >> 24L;

                                                               if (RedLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  RedLevel += DELTA;

                                                                  if (RedLevel > MAXVALUE) RedLevel = MAXVALUE;

                                                                  GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                  COLOR_RGB [ColorBase] = RedLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 't' : RedLevel = COLOR_RGB [ColorBase] >> 24L;

                                                               if (RedLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  RedLevel++;

                                                                  GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                  COLOR_RGB [ColorBase] = RedLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'F' : GreenLevel = COLOR_RGB [ColorBase + 1L] >> 24L;

                                                               if (GreenLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  GreenLevel = (GreenLevel < (MINVALUE + DELTA)) ? MINVALUE : GreenLevel - DELTA;

                                                                  GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                  COLOR_RGB [ColorBase + 1L] = GreenLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'f' : GreenLevel = COLOR_RGB [ColorBase + 1L] >> 24L;

                                                               if (GreenLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  GreenLevel--;

                                                                  GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                  COLOR_RGB [ColorBase + 1L] = GreenLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'H' : GreenLevel = COLOR_RGB [ColorBase + 1L] >> 24L;

                                                               if (GreenLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  GreenLevel += DELTA;

                                                                  if (GreenLevel > MAXVALUE) GreenLevel = MAXVALUE;

                                                                  GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                  COLOR_RGB [ColorBase + 1L] = GreenLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'h' : GreenLevel = COLOR_RGB [ColorBase + 1L] >> 24L;

                                                               if (GreenLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  GreenLevel++;

                                                                  GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                  COLOR_RGB [ColorBase + 1L] = GreenLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'V' : BlueLevel = COLOR_RGB [ColorBase + 2L] >> 24L;

                                                               if (BlueLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  BlueLevel = (BlueLevel < (MINVALUE + DELTA)) ? MINVALUE : BlueLevel - DELTA;

                                                                  GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                  COLOR_RGB [ColorBase + 2L] = BlueLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'v' : BlueLevel = COLOR_RGB [ColorBase + 2L] >> 24L;

                                                               if (BlueLevel > MINVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  BlueLevel--;

                                                                  GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                  COLOR_RGB [ColorBase + 2L] = BlueLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'N' : BlueLevel = COLOR_RGB [ColorBase + 2L] >> 24L;

                                                               if (BlueLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  BlueLevel += DELTA;

                                                                  if (BlueLevel > MAXVALUE) BlueLevel = MAXVALUE;

                                                                  GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                  COLOR_RGB [ColorBase + 2L] = BlueLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'n' : BlueLevel = COLOR_RGB [ColorBase + 2L] >> 24L;

                                                               if (BlueLevel < MAXVALUE)
                                                               {
                                                                  KeepPalette (Win);

                                                                  BlueLevel++;

                                                                  GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                  COLOR_RGB [ColorBase + 2L] = BlueLevel << 24L;

                                                                  LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);
                                                               }

                                                               break;

                                                    case 'I' :
                                                    case 'i' : KeepPalette (Win);

                                                               InvertPalette (Win,STARTPEN,Colors - 1L);

                                                               RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                               GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                               BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                               GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                               GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                               GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                               break;
                                            }

                                            break;

                 case IDCMP_GADGETUP      : switch (MyGad->GadgetID)
                                            {
                                                    case CWACCEPT  : GetRGB32 (ViewPortAddress (ColorWin)->ColorMap,0L,Colors,&Palette32 [1L]);

                                                                   Exit = TRUE;

                                                                   break;

                                                    case CWRESET   : KeepPalette (Win);

                                                                   LoadRGB32 (ViewPortAddress (ColorWin),Palette32);

                                                                   GetRGB32 (ViewPortAddress (ColorWin)->ColorMap,0L,Colors,&COLOR_RGB [1L]);

                                                                   RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                                   GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                                   BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                                   GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                   GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                   GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                   break;

                                                    case CWCANCEL  : LoadRGB32 (ViewPortAddress (ColorWin),Palette32);

                                                                   Exit = TRUE;

                                                                   break;

                                                    case COPY    : Copy (ColorWin,SelectedPen);

                                                                   Copy_Msg = TRUE;

                                                                   Swap_Msg = FALSE;

                                                                   Spread_Msg = FALSE;

                                                                   break;

                                                    case SWAP    : OldPen = SelectedPen;

                                                                   Copy_Msg = FALSE;

                                                                   Swap_Msg = TRUE;

                                                                   Spread_Msg = FALSE;

                                                                   break;

                                                    case SPREAD  : OldPen = SelectedPen;

                                                                   Copy_Msg = FALSE;

                                                                   Swap_Msg = FALSE;

                                                                   Spread_Msg = TRUE;

                                                                   break;

                                                    case INVERT  : KeepPalette (Win);

                                                                   InvertPalette (Win,STARTPEN,Colors - 1L);

                                                                   RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                                   GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                                   BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                                   GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                   GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                   GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                   break;

                                                    case SHL     : KeepPalette (ColorWin);

                                                                   Shl (ColorWin,STARTPEN,ENDPEN);

                                                                   break;

                                                    case SHR     : KeepPalette (ColorWin);

                                                                   Shr (ColorWin,STARTPEN,ENDPEN);

                                                                   break;

                                                    case UNDO    : LoadRGB32 (ViewPortAddress (ColorWin),UNDO_RGB);

                                                                   GetRGB32 (ViewPortAddress (ColorWin)->ColorMap,0L,Colors,&COLOR_RGB [1L]);

                                                                   RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                                   GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                                   BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                                   GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                   GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                   GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                   break;

                                                    case RED     : KeepPalette (Win);

                                                                   GT_GetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,&RedLevel);

                                                                   COLOR_RGB [ColorBase]      = RedLevel << 24L;

                                                                   LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);

                                                                   break;

                                                    case GREEN   : KeepPalette (Win);

                                                                   GT_GetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,&GreenLevel);

                                                                   COLOR_RGB [ColorBase + 1L] = GreenLevel << 24L;

                                                                   LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);

                                                                   break;

                                                    case BLUE    : KeepPalette (Win);

                                                                   GT_GetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,&BlueLevel);

                                                                   COLOR_RGB [ColorBase + 2L] = BlueLevel << 24L;

                                                                   LoadRGB32 (ViewPortAddress (ColorWin),COLOR_RGB);

                                                                   break;

                                                    case CWPALETTE : GT_GetGadgetAttrs (MyPaletteGad,ColorWin,NULL,GTPA_Color,&SelectedPen);

                                                                   if (Copy_Msg)
                                                                   {
                                                                      KeepPalette (Win);

                                                                      Paste (ColorWin,SelectedPen);

                                                                      Copy_Msg = FALSE;
                                                                   }

                                                                   if (Swap_Msg)
                                                                   {
                                                                      KeepPalette (Win);

                                                                      NewPen = SelectedPen;

                                                                      Swap (ColorWin,OldPen,NewPen);

                                                                      Swap_Msg = FALSE;
                                                                   }

                                                                   if (Spread_Msg)
                                                                   {
                                                                      KeepPalette (Win);

                                                                      NewPen = SelectedPen;

                                                                      Spread (ColorWin,OldPen,NewPen);

                                                                      Spread_Msg = FALSE;
                                                                   }

                                                                   ColorBase = 3L * SelectedPen + 1L;

                                                                   RedLevel   =  (COLOR_RGB [ColorBase]      >> 24L);

                                                                   GreenLevel =  (COLOR_RGB [ColorBase + 1L] >> 24L);

                                                                   BlueLevel  =  (COLOR_RGB [ColorBase + 2L] >> 24L);

                                                                   GT_SetGadgetAttrs (RedSliderGad,ColorWin,NULL,GTSL_Level,(WORD) RedLevel);

                                                                   GT_SetGadgetAttrs (GreenSliderGad,ColorWin,NULL,GTSL_Level,(WORD) GreenLevel);

                                                                   GT_SetGadgetAttrs (BlueSliderGad,ColorWin,NULL,GTSL_Level,(WORD) BlueLevel);

                                                                   break;
                                            }

                                            break;

                 case IDCMP_CLOSEWINDOW   : Exit = TRUE;

                                            break;
          }
       }

     } while (Exit == FALSE);

  CloseWindow (ColorWin);

  FreeGadgets (GadList);

  FreeVisualInfo (VInfo);

  return TRUE;
}

VOID Copy (struct Window *Win,ULONG PenNumber)
{
  GetRGB32 (ViewPortAddress (Win)->ColorMap,PenNumber,1L,COPY_RGB);
}

VOID Paste (struct Window *Win,const ULONG PenNumber)
{
  const ULONG ColorBase = 3L * PenNumber + 1L;

  ULONG Index;

  for (Index = 0L; Index < 3L; Index++) COLOR_RGB [ColorBase + Index] = COPY_RGB [Index];

  LoadRGB32 (ViewPortAddress (Win),COLOR_RGB);
}

void Swap (struct Window *Win,const ULONG Pen_1,const ULONG Pen_2)
{
   ULONG Tmp_RGB [3L];

   ULONG Index;

   GetRGB32 (ViewPortAddress (Win)->ColorMap,Pen_1,1L,Tmp_RGB);

   Copy (Win,Pen_2);

   Paste (Win,Pen_1);

   for (Index = 0L; Index < 3L; Index++) COPY_RGB [Index] = Tmp_RGB [Index];

   Paste (Win,Pen_2);
}

BOOL Spread (struct Window *Win,const ULONG OldPen,const ULONG NewPen)
{
  const ULONG StartPen = MIN (OldPen,NewPen) , EndPen = MAX (OldPen,NewPen);

  const ULONG  Range   = EndPen - StartPen;

  LONG RedStep,GreenStep,BlueStep;

  ULONG Index,RedLevel,GreenLevel,BlueLevel,ColorBase;

  if (Range < 2L) return TRUE;

  ColorBase   = 3L * StartPen + 1L;

  RedLevel    = COLOR_RGB [ColorBase]      >> 8L;

  GreenLevel  = COLOR_RGB [ColorBase + 1L] >> 8L;

  BlueLevel   = COLOR_RGB [ColorBase + 2L] >> 8L;

  ColorBase   = 3L * EndPen + 1L;

  RedStep     = (LONG) (COLOR_RGB [ColorBase]      >> 8L);

  GreenStep   = (LONG) (COLOR_RGB [ColorBase + 1L] >> 8L);

  BlueStep    = (LONG) (COLOR_RGB [ColorBase + 2L] >> 8L);

  RedStep    -= (LONG) RedLevel;

  GreenStep  -= (LONG) GreenLevel;

  BlueStep   -= (LONG) BlueLevel;

  RedStep    /= (LONG) Range;

  GreenStep  /= (LONG) Range;

  BlueStep   /= (LONG) Range;

  for (Index = (StartPen + 1L); Index < EndPen; Index++)
  {
      RedLevel   += RedStep;

      GreenLevel += GreenStep;

      BlueLevel  += BlueStep;

      ColorBase   = 3L * Index + 1L;

      COLOR_RGB [ColorBase]      = RedLevel   << 8L;

      COLOR_RGB [ColorBase + 1L] = GreenLevel << 8L;

      COLOR_RGB [ColorBase + 2L] = BlueLevel  << 8L;
  }

  LoadRGB32 (ViewPortAddress (Win),COLOR_RGB);

  return FALSE;
}

VOID KeepPalette (struct Window *Win)
{
  ULONG Colors;

  Colors = 1L << (Win->RPort->BitMap->Depth);

  GetRGB32 (ViewPortAddress (Win)->ColorMap,0L,Colors,&UNDO_RGB [1L]);
}

VOID InvertPalette (struct Window *Win,ULONG StartPen,ULONG EndPen)
{
  while (StartPen < EndPen) Swap (Win,StartPen++,EndPen--);
}

void Shl (struct Window *Win,ULONG StartPen,ULONG EndPen)
{
  while (StartPen < EndPen) Swap (Win,StartPen,StartPen+1) , StartPen++;
}

void Shr (struct Window *Win,ULONG StartPen,ULONG EndPen)
{
  while (EndPen > StartPen) Swap (Win,EndPen,EndPen-1) , EndPen--;
}
