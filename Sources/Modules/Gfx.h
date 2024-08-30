
#ifndef GFX_H
#define GFX_H

#include <exec/types.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <intuition/intuition.h>
#include <graphics/gfxbase.h>
#include "fmglobals.h"

#define RESERVED_PENS 4L

extern struct Library* IntuitionBase;
extern struct Library* DOSBase;

BOOL Fade (struct Window *,ULONG *,ULONG,ULONG,BOOL);
BOOL Cycle (struct Window *,ULONG,BOOL);

#endif /* GFX_H */