
#ifndef GFX_H
#define GFX_H

#define FLASHMANDEL_RB

#include <exec/types.h>
#include <proto/dos.h>
#include <proto/intuition.h>
/*#include <proto/graphics.h>*/
#include "fmprotographics.h"
#include <proto/gadtools.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include "fmglobals.h"

/*#define RAW_ESC 0x45
*/
#define RESERVED_PENS 4L
/*
#define BLACK 0L
*/

extern struct Library* IntuitionBase;
extern struct Library* DOSBase;

BOOL Fade (struct Window *,ULONG *,ULONG,ULONG,BOOL);
BOOL Cycle (struct Window *,ULONG,BOOL);

#endif /* GFX_H */