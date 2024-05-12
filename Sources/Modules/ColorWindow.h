
#ifndef COLORWINDOW_H
#define COLORWINDOW_H

#define FLASHMANDEL_RB

#include <exec/types.h>
#include <exec/execbase.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <proto/exec.h>
#include <proto/intuition.h>
/*#include <proto/graphics.h>*/
#include "/fmprotographics.h"
#include <proto/gadtools.h>
#include <proto/utility.h>

#include "/compilerspecific.h"
#include "fmglobals.h"

/*
#define MIN(a,b)    ((a)<(b)?(a):(b))
#define MAX(a,b)    ((a)>(b)?(a):(b))
*/


#define CWACCEPT       (1L)
#define CWRESET        (2L)
#define CWCANCEL       (3L)

#define RED          (4L)
#define GREEN        (5L)
#define BLUE         (6L)
#define COPY         (7L)
#define SWAP         (8L)
#define SPREAD       (9L)
#define UNDO         (10L)
#define INVERT       (11L)
#define CWPALETTE    (12L)
#define SHL          (13L)
#define SHR          (14L)


#define STARTPEN     (4L)
#define ENDPEN       (255L)

#define MINVALUE     (0L)
#define MAXVALUE     (255L)
#define DELTA        (10L)

extern struct Library* IntuitionBase;

BOOL ModifyPalette  (struct Window *,WORD,WORD,ULONG *);
VOID KeepPalette    (struct Window *);
VOID Copy           (struct Window *,ULONG);
VOID Paste          (struct Window *,const ULONG);
VOID Swap           (struct Window *,const ULONG,const ULONG);
BOOL Spread         (struct Window *,const ULONG,const ULONG);
VOID InvertPalette  (struct Window *,ULONG,ULONG);
void Shl            (struct Window *Win,ULONG,ULONG);
void Shr            (struct Window *Win,ULONG,ULONG);

extern ULONG COLOR_RGB [],UNDO_RGB [],COPY_RGB [];

extern struct NewGadget BUTTON_GAD,SLIDER_GAD,PALETTE_GAD;

#endif /* COLORWINDOW_H */