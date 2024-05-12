
#ifndef FMEVENTS_H
#define FMEVENTS_H

#include "fmglobals.h"
#include "data.h"
#include <exec/types.h>
#include <proto/diskfont.h>
#include <proto/asl.h>
#include <proto/utility.h>   /* ToUpper() */
#include "Modules/Iff.h"
#include "splash.h"
#include "menu.h"
#include "histogram.h"
#include "version.h"
#include "Modules/Gfx.h"
#include "Modules/ColorWindow.h"

ULONG HandleEvents (struct ILBMInfo *);
BOOL FontRequest (struct Window *);
VOID ProcessMouse (struct Window *,WORD,WORD);
BOOL DrawFrame (struct Window *,const WORD,const WORD,const WORD,const WORD);
BOOL FileRequest (struct Window *,TEXT *,TEXT *,BOOL,BOOL);
BOOL NewCoords (struct Window *,const WORD,const WORD,const WORD,const WORD);
LONG Choice (struct Window *,TEXT *,TEXT *);
BOOL ShowCoords (struct Window *);
void AdjustRatio (double *,double *,double *,double *,WORD,WORD,BOOL);
void DisableDirectDrawing(void);
void EnableDirectDrawing(void);

/* probably obsolete */
BOOL Preview (struct Window *,WORD,WORD);

#endif /* FMEVENTS_H */
