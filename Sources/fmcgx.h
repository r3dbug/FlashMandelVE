
#ifndef FMCGX_H
#define FMCGX_H

#include <pragmas/cybergraphics_pragmas.h>
#include <proto/cybergraphics.h>
#include <cybergraphics/cybergraphics.h>
#include "fmglobals.h"
#include "fmcgx.h"

ULONG GetBitMapBPP(struct BitMap*);
ULONG GetBitMapPixFMT(struct BitMap*);
void* GetBitMapPtr(struct BitMap*);
void   FreeBitMapSafety (struct BitMap *);
BOOL IsAClassicScreen(LONG);
struct BitMap *CopyBitMap (struct Window *,WORD,WORD,WORD,WORD);

LONG MakeDisplay (struct ILBMInfo *);
struct Window *OpenDisplay (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
struct Screen *OpenIdScreen (struct ILBMInfo *,WORD ,WORD,WORD,ULONG);
VOID CloseDisplay (struct ILBMInfo *,CPTR *);
UBYTE GetMaxPlanes (ULONG);
BOOL AllocTempRast (UWORD,UBYTE);
struct Window *OpenDisplay (struct ILBMInfo *,WORD,WORD,WORD,ULONG);
ULONG ModeFallBack (ULONG,WORD,WORD,WORD);
VOID ClipIt (WORD,WORD,struct Rectangle *,struct Rectangle *,struct Rectangle *, struct Rectangle *,struct Rectangle *, struct Rectangle *,BOOL);
LONG CheckGFX (VOID);

void DebugBitmap(struct BitMap *);

extern ULONG bmapdepth, pixfmt;
extern ULONG query[];
extern ULONG query2[];

#endif /* FMCGX_H */