
#ifndef DRAWING_H
#define DRAWING_H

#include <exec/types.h>
#include <proto/intuition.h>
#include <graphics/scale.h>
#include <iffp/ilbmapp.h>
#include <proto/cybergraphics.h>
#include <cybergraphics/cybergraphics.h>

#include "fmcgx.h"
#include "fmglobals.h"

#define DRAW_CLASSIC 0
#define DRAW_DIRECT  1

extern void (*PutPixel)(ULONG,ULONG,ULONG);
extern void (*FillUncalculatedAreas)(void);
extern LONG (*WritePixelLineDirect)(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
extern void (*RectColorFillDirect)(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);
extern ULONG (*GetPixelPen)(ULONG x, ULONG y);

extern ULONG DD_WIDTH, DD_HEIGHT;
extern UBYTE* DD_SCREENBUFFER;
extern ULONG DD_PIXFMT, DD_BPP, DD_OLDBPP;
extern ULONG DD_SRC_BPP, DD_SRC_PIXFMT, DD_DST_BPP, DD_DST_PIXFMT;
extern ULONG DD_SRC_RED, DD_SRC_GREEN, DD_SRC_BLUE;
extern ULONG DD_ARGB, DD_RED, DD_GREEN, DD_BLUE, DD_ALPHA;
extern ULONG DD_BKGRD; 
extern struct RastPort* DD_RPORT;
extern struct Window* DD_WIN;

/* drawing functions */

ULONG InterpolateCLUT(ULONG, ULONG, double);
void CopyAGAPalette2RGB(void);
void PutPixelClassicPen(ULONG, ULONG, ULONG);
void PutPixelPenGray2RGB(ULONG, ULONG, ULONG);
void PutPixelRGB(ULONG, ULONG, UBYTE, UBYTE, UBYTE);
UBYTE FindPenByRGB(UBYTE, UBYTE, UBYTE);
ULONG GetPixelClassicPen(ULONG, ULONG);
ULONG GetPixelRGB24(ULONG, ULONG);
ULONG GetPixelRGB24Pen(ULONG, ULONG);
ULONG GetPixelCLUT8Pen(ULONG, ULONG);
void PutPixelCLUT2RGB(ULONG, ULONG, ULONG);
void PutPixelARGB(ULONG, ULONG, ULONG);
ULONG GetPixelCLUT8(ULONG, ULONG, ULONG);
ULONG GetPixelPenGeneric(struct RastPort*, ULONG, ULONG);
void PutPixelCLUT8(ULONG, ULONG, ULONG);
void FillUncalculatedAreasCLUT8(void);
void FillUncalculatedAreasClassicPen(void);
void FillUncalculatedAreasRGB(void);
LONG WritePixelLineCLUT2RGB(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
LONG WritePixelLineCLUT8(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
LONG WritePixelLine(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
void RectColorFillDirectCLUT8(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);
void RectColorFillDirectCLUT2RGB(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);
void RectColorFill(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);

/* from FlashMandel main program */

/****************************************************************************************************/
/***************************************** BitMap functions *****************************************/
/****************************************************************************************************/

BOOL PasteBitMapStandard (struct BitMap*, struct Window*, WORD, WORD);
BOOL PasteBitMapCLUT2RGB (struct BitMap*, struct Window*, WORD, WORD);
BOOL PasteBitMapRGB2CLUT (struct BitMap*, struct Window*, WORD, WORD);

/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/

void SetUpPixFMTDeltas(ULONG);
void SetUpDirectDrawing(struct Window*);


#endif /* DRAWING_H */
