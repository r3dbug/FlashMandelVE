
#ifndef MENU_H
#define MENU_H

#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/dos.h>
#include <exec/types.h>

#include "fmglobals.h"
#include "drawing.h"
#include "fmcgx.h"
#include "symmetry.h"
#include "fmevents.h"

/* SAFP = set all function pointers */
#include "SAFP.h"

extern struct NewMenu ProgMenu[];
extern struct Library* DOSBase;

BOOL PrepareMainMenuWindowAndPointer(void);

/* Handle menu selection */

void SwitchMenuSelection(UWORD, UWORD, UWORD, UWORD); 
void MenuResumeOff(struct Window*);
void MenuResumeOn(struct Window*);
void CheckMenuItem(UWORD, UWORD, UWORD);
void UncheckMenuItem(UWORD, UWORD, UWORD);

/* not sure if this function works correctly ... */
void ClearMenuRange(UWORD, UWORD, UWORD, UWORD);
UWORD GetBaseFractalType(UWORD);
void Return2BaseFractal(struct Window*, ULONG);
void SetIterationsMenu(void);
void DisableDirectDrawingMenu(struct Window*);
void EnableDirectDrawingMenu(struct Window*);
void SetDrawingMenu(void);
void SetMenuSelection(void);
ULONG ProcessMenu (struct Window *,UWORD);

/* start full menus (e.g. after a calculation */
VOID SetMenuStart (struct Window *);

/* stop menus (e.g. before a calculation */
VOID SetMenuStop (struct Window *);


#endif /* MENU_H */