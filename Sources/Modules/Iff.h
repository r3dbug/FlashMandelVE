
#ifndef IFF_H
#define IFF_H

#include <proto/exec.h>
#include <proto/intuition.h>
/*#include <proto/graphics.h>*/
#include "/fmprotographics.h"
#include <proto/iffparse.h>

#include <exec/types.h>
#include <intuition/intuitionbase.h>
#include <intuition/screens.h>
#include <graphics/gfxbase.h>

/*#include <iffp/iff.h>
#include <iffp/ilbm.h>
*/
#include <libraries/iffparse.h>
#include <iffp/ilbmapp.h>
#include <iffp/packer.h>
#include <iffp/iffpstrings.h>

#include "/compilerspecific.h"
#include "/fmcgx.h"

extern UBYTE *omodes [];

static BYTE *PutDump(BYTE *,int);

static BYTE *PutRun(BYTE *,int,int);
extern struct Library* IntuitionBase;

#define MaxSrcPlanes (25)

/*#define ID_MAND MAKE_ID('M','A','N','D') /* this is defined twice ?! (see FlashMandel.c) */
#define ID_NEWMAND MAKE_ID ('F', 'M', 'V', 'E')

#define NOMAND 4L

#define TOBLACK   1
#define FROMBLACK 0

#define DUMP    0
#define RUN 1
#define MinRun 3
#define MaxRun 128
#define MaxDat 128
#define INBUFSZ 128
#define BODYBUFSZ   5004
#define GetByte()       (*source++)
#define PutByte(c)      { *dest++ = (c);   ++PackPutSize; }
#define OutDump(nn)     dest = PutDump(dest, nn)
#define OutRun(nn,cc)   dest = PutRun(dest, nn, cc)
#define UGetByte()  (*source++)
#define UPutByte(c) (*dest++ = (c))

IMPORT BOOL Fade (struct Window *,ULONG *,ULONG,ULONG,BOOL);

/* local function prototypes */

extern struct  CatCompArrayType CatCompArray [];

LONG QueryMandPic (struct ILBMInfo *,struct MandelChunk **,UBYTE *filename);
LONG QueryMandPicNew (struct ILBMInfo *,struct NewMandelChunk **,UBYTE *);
LONG QueryMandPicNewRGB (struct ILBMInfo *,struct NewRGBMandelChunk **,UBYTE *);

LONG LoadMandPic (struct ILBMInfo *,UBYTE *);
LONG LoadMandPicRGB (struct ILBMInfo *,UBYTE *);

LONG SaveMandPic (struct ILBMInfo *,struct Chunk *,struct Chunk *,UBYTE *);

LONG SavePalette (struct ILBMInfo *,struct Chunk *,UBYTE *);

LONG LoadPalette (struct ILBMInfo *,UBYTE *, UBYTE*);

static LONG SAVEDS ASMCALL stdio_stream (REG (a0, struct Hook *),REG (a2,struct IFFHandle *),REG (a1,struct IFFStreamCmd *));

LONG saveilbm (struct ILBMInfo *,struct BitMap *,ULONG,WORD,WORD,WORD,WORD,APTR,UWORD,UWORD,WORD,WORD,struct Chunk *,struct Chunk *,UBYTE *);

LONG initbmhd (BitMapHeader *,struct BitMap *,WORD,WORD,WORD,WORD,WORD,WORD,WORD,ULONG);

LONG loadcmap (struct ILBMInfo *);

LONG putcmap (struct IFFHandle *,APTR,UWORD,UWORD);

LONG loadbody (struct IFFHandle *,struct BitMap *,BitMapHeader *);

LONG loadbody2 (struct IFFHandle *,struct BitMap *,BYTE *,BitMapHeader *,BYTE *,ULONG);

LONG putbody (struct IFFHandle *,struct BitMap *,BYTE *,BitMapHeader *,BYTE *,LONG);

ULONG getcamg (struct ILBMInfo *);

LONG openifile (struct ParseInfo *,UBYTE *,ULONG);

LONG parseifile (struct ParseInfo *,LONG,LONG,LONG *,LONG *,LONG *);

VOID closeifile (struct ParseInfo *);

VOID initiffasstdio (struct IFFHandle *);

LONG PutCk (struct IFFHandle *,LONG,LONG,VOID *);

LONG getcolors (struct ILBMInfo *);

LONG alloccolortable (struct ILBMInfo *);

LONG setcolors (struct ILBMInfo *,struct ViewPort *);

VOID freecolors (struct ILBMInfo *);

LONG chkcnt (LONG *);

LONG getcontext (struct IFFHandle *);

LONG contextis (struct IFFHandle *,LONG,LONG);

LONG currentchunkis (struct IFFHandle *,LONG,LONG);

UBYTE *findpropdata (struct IFFHandle *,LONG,LONG);

static BYTE *PutDump (BYTE *,int);

static BYTE *PutRun (BYTE *,int,int);

LONG PackRow (BYTE **,BYTE **,LONG);

BOOL UnPackRow (BYTE **,BYTE **,WORD,WORD);

extern LONG PackPutSize;

extern char PackBuffer [];  /* [TBD] should be 128?  on stack?*/

/*struct MandelChunk { WORD LeftEdge,TopEdge,Width,Height;
                     double RMin,RMax,IMin,IMax;
                     double RConst,IConst;
                     ULONG Iterations;
                     ULONG Special;
                   };
*/

/* additional iff / ilbm functions (from official IFF source code) */

#define BitsPerRow(w)  ((w) + 15 >> 4 << 4)

LONG alloccolortableRGB (struct ILBMInfo *);
LONG loadcmapRGB (struct ILBMInfo *, UBYTE*);

#endif /* IFF_H */