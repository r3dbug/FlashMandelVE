/**********************************************************************************************************
**
**  Coded by Dino Papararo     11-Feb-1999
**
**  FUNCTION
**
**    Fade -- perform screen color fading from/to black.
**
**  SYNOPSIS
**
**    BOOL Fade (struct Window *Win,ULONG *PaletteSrc,ULONG MaxStep,ULONG TimeDelay,BOOL ToBlack)
**
**  DESCRIPTION
**
**    According with ToBlack value screen will fade from black to PaletteSrc or from PaletteSrc to black.
**
**    MaxStep is number of times colors will be decreased/increased from/to black.
**
**    TimeDelay is the wait time between two palette changes, is necessary for multitasking and faster cpu.
**
**  RETURN
**
**    FALSE if screen proprieties not available or Depth < 2 or MaxStep < 2, TRUE color cycling performed.
**
***********************************************************************************************************
**
**  Coded by Claudio Pucci & Dino Papararo     26-Oct-1997
**
**  FUNCTION
**
**    Cycle -- perform screen color cycling.
**
**  SYNOPSIS
**
**    BOOL Cycle (struct Window *Win,ULONG TimeDelay,BOOL Left)
**
**  DESCRIPTION
**
**    Function uses a double sized table to manage color cycling.
**
**    So we do not need to save and shift all colors for every cycle,
**
**    but only save and set correct first and last values !
**
**    This function do not load the first four color registers reserved for GUI pens.
**
**  RETURN
**
**    FALSE if screen proprieties not available or Depth < 2, TRUE color cycling performed.
**
**********************************************************************************************************/

#include <exec/types.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/gadtools.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>

#define RAW_ESC 0x45

#define RESERVED_PENS 4L

#define BLACK 0L

extern struct Library* IntuitionBase;
extern struct Library* DOSBase;

BOOL Fade (struct Window *,ULONG *,ULONG,ULONG,BOOL);
BOOL Cycle (struct Window *,ULONG,BOOL);

BOOL Fade (struct Window *Win,ULONG *PaletteSrc,ULONG MaxStep,ULONG TimeDelay,BOOL ToBlack)
{
DisplayInfoHandle DHandle;

struct DisplayInfo DInfo;

static ULONG PaletteTmp [3L * 252L + 2L];

LONG Var,Step;

ULONG Range,ModeID;

BOOL AllBlack;

  if ((Win->RPort->BitMap->Depth < 2L) || (MaxStep < 2L)) return FALSE;

  ModeID = GetVPModeID (ViewPortAddress (Win));

  DHandle = FindDisplayInfo (ModeID);

  if (GetDisplayInfoData (DHandle,(UBYTE *) &DInfo,sizeof (struct DisplayInfo),DTAG_DISP,ModeID))
  {
     if (DInfo.PropertyFlags & DIPF_IS_EXTRAHALFBRITE) Range = 32L - 4L;

     else Range = (1L << Win->RPort->BitMap->Depth) - 4L;

     PaletteTmp [0L] = (Range << 16L) + 4L;

     GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,Range,&PaletteTmp [1L]);

     PaletteTmp [3L * Range + 1L] = NULL;

     if (ToBlack)
     {
        for (Step = 0L; Step <= MaxStep; Step++)
        {
            AllBlack = TRUE;

            for (Var = 1; Var <= 3L * Range; Var++)
            {
                if (PaletteTmp [Var])
                {
                   PaletteTmp [Var] = (((PaletteSrc [Var + (3 * RESERVED_PENS)] >> 24L) * (MaxStep - Step)) / MaxStep) << 24L;

                   AllBlack = FALSE;
                }
            }

            if (AllBlack) break;

            WaitTOF ();

            LoadRGB32 (ViewPortAddress (Win),PaletteTmp);

            Delay (TimeDelay);
        }
     }

     else
     {
        for (Step = MaxStep; Step >= 0L; Step--)
        {
            for (Var = 1; Var <= 3L * Range; Var++)

                PaletteTmp [Var] = (((PaletteSrc [Var + (3 * RESERVED_PENS)] >> 24L) * (MaxStep - Step)) / MaxStep) << 24L;

            WaitTOF ();

            LoadRGB32 (ViewPortAddress (Win),PaletteTmp);

            Delay (TimeDelay);
        }

        LoadRGB32 (ViewPortAddress (Win),PaletteSrc);
     }

     return TRUE;
  }

  return FALSE;
}

BOOL Cycle (struct Window *Win,ULONG TimeDelay,BOOL Left)
{
DisplayInfoHandle DHandle;

struct DisplayInfo DInfo;

struct IntuiMessage *Message;

static ULONG Palette_Tmp [2L * 3L * 252L + 2L];

BOOL Loop = TRUE;

UWORD MyCode;

ULONG MyClass,Counter = NULL,OldBlue,OldRed,Tmp_1,Tmp_2,HalfRange;

ULONG ModeID,Range;

  if (Win->RPort->BitMap->Depth < 2L) return FALSE;

  ModeID = GetVPModeID (ViewPortAddress (Win));

  DHandle = FindDisplayInfo (ModeID);

  if (GetDisplayInfoData (DHandle,(UBYTE *) &DInfo,sizeof (struct DisplayInfo),DTAG_DISP,ModeID))
  {
     if (DInfo.PropertyFlags & DIPF_IS_EXTRAHALFBRITE)
     {
        Range = 64L - 8L;

        HalfRange = Range >> 1L;

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,HalfRange,&Palette_Tmp [1L]);

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,HalfRange,&Palette_Tmp [3L * HalfRange + 1L]);

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,HalfRange,&Palette_Tmp [3L * (2L * HalfRange) + 1L]);

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,HalfRange,&Palette_Tmp [3L * (3L * HalfRange) + 1L]);

        Palette_Tmp [3L * 2L * (4L * HalfRange) + 1L] = NULL;
     }

     else
     {
        Range = (1L << Win->RPort->BitMap->Depth) - 4L;

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,Range,&Palette_Tmp [1L]);

        GetRGB32 (ViewPortAddress (Win)->ColorMap,4L,Range,&Palette_Tmp [3L * Range + 1L]);

        Palette_Tmp [2L * 3L * Range + 1L] = NULL;
     }

     Palette_Tmp [0L] = (Range << 16L) + 4L;

     if (! Left) Counter = Range + 1L;

     while (Loop)
     {
           if (Win->UserPort->mp_SigBit)
           {
              if (Message = (struct IntuiMessage *) GT_GetIMsg (Win->UserPort))
              {
                 MyClass = Message->Class;

                 MyCode  = Message->Code;

                 GT_ReplyIMsg ((struct IntuiMessage *) Message);

                 switch (MyClass)
                 {
                    case IDCMP_MOUSEBUTTONS:  if (MyCode == SELECTDOWN) Loop = FALSE;

                                              break;

                    case IDCMP_MENUPICK    :  Loop = FALSE;

                                              break;

                    case IDCMP_RAWKEY      :  if (MyCode == RAW_ESC) Loop = FALSE;

                                              break;
                 }
              }
           }

           if (Left)
           {
              Counter++;

              if (Counter > Range) Counter = 1L;
           }

           else
           {
              Counter--;

              if (Counter < 1L) Counter = Range;
           }

           Tmp_1 = 3L * Counter;

           Tmp_2 = 3L * (Counter + Range) + 1L;

           OldBlue = Palette_Tmp [Tmp_1];

           OldRed  = Palette_Tmp [Tmp_2];

           Palette_Tmp [Tmp_1] = (Range << 16L) + 4L;

           Palette_Tmp [Tmp_2] = NULL;

           WaitTOF ();

           LoadRGB32 (ViewPortAddress (Win),&Palette_Tmp [3L * Counter]);

           Delay (TimeDelay);

           Palette_Tmp [Tmp_1] = OldBlue;

           Palette_Tmp [Tmp_2] = OldRed;
     }

     return TRUE;
  }

  return FALSE;
}
