
#ifndef SPLASH_H
#define SPLASH_H

#include <proto/intuition.h>
#include <intuition/intuition.h>
#include <proto/iffparse.h>
#include <iffp/ilbmapp.h>
#include <iffp/iff.h>
#include <stdio.h>
#include <proto/dos.h>
#include <proto/gadtools.h>

#include "fmglobals.h"

LONG QuerySplashPic (struct ILBMInfo *,UBYTE *);
void ShowSplashRequester(struct Window*, UBYTE);
LONG About (struct Window *);
void AltRequest (struct Window *);
LONG Choice (struct Window *,TEXT *,TEXT *);

#endif /* SPLASH_H */
