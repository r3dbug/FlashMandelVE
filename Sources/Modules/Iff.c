/******************************************************************************
**
**  Coded by Dino Papararo 10-October-2000
**
**  Based on NewIff package relased by Commodore
**
**  FUNCTION
**
**    QueryMandPic -- Examine an IFF picture.
**
**  SYNOPSIS
**
**    LONG QueryMandPic (struct ILBMInfo *,struct MandelChunk **,UBYTE *);
**
**  DESCRIPTION
**
**    Passed an initialized ILBMInfo with a not-in-use IFFHandle, a MandelChunk
**
**    structure and a filename, will open an ILBM, fill in ilbm->camg and
**
**    ilbm->bmhd, and close the ILBM.
**
**    This allows you to determine if the ILBM is a size and
**
**    type you want to deal with.
**
**    For succes is necessary the ID_MAND into the iff file !
**
**    Returns 0 for success or an IFFERR (libraries/iffparse.h).
**
**
**  FUNCTION
**
**    LoadMandPic -- Load an IFF picture.
**
**  SYNOPSIS
**
**    LONG LoadMandPic (struct ILBMInfo *,UBYTE *);
**
**  DESCRIPTION
**
**    Function uses a ILBMInfo struct with record ParseInfo.iff initialized
**
**    with AllocIFF() function, and a pointer to a FileName.
**
**    It's necessary a valid Window RPort and ViewPort initialized in the passed
**
**    ILBM Structure for the BODY and COLORS, at end all memory will be freed.
**
**
**  FUNCTION
**
**    SaveMandPic -- save a screen as IFF picture.
**
**  SYNOPSIS
**
**    LONG SaveMandPic (struct ILBMInfo *,struct Chunk *,struct Chunk *,UBYTE *);
**
**  DESCRIPTION
**
**    Function uses a ILBMInfo struct with record ParseInfo.iff initialized
**
**    with AllocIFF() function, two custom chunks, and a pointer to a FileName.
**
**    In the 1st custom chunk I put the copyright infos and in the 2nd the
**
**    special chunk MAND used for window limits, fractal limits, iterations
**
**    type...
**
**    Function will save the icon file too with support for NewIcons.
**
******************************************************************************/

#ifndef FLASHMANDEL_RB
#define FLASHMANDEL_RB
#endif

#include "Iff.h"

UBYTE *omodes [2] = {"r","w"};

static BYTE *PutDump(BYTE *,int);

static BYTE *PutRun(BYTE *,int,int);
extern struct Library* IntuitionBase;

char PackBuffer [256];  /* [TBD] should be 128?  on stack?*/
LONG PackPutSize;

/* createbrush
 *
 * Passed an initialized ILBMInfo with a parsed IFFHandle (chunks parsed,
 * stopped at BODY),
 * gets the bitmap and colors
 * Sets up ilbm->brbitmap, ilbm->colortable, ilbm->ncolors
 * Returns 0 for success
 */
LONG createbrush(struct ILBMInfo *ilbm)
        {
        int error;

        error                   = getbitmap(ilbm);
        if(!error) error        = loadbody(ilbm->ParseInfo.iff,
                                                ilbm->brbitmap,&ilbm->Bmhd);
        if(!error)              getcolors(ilbm);
        if(error)               deletebrush(ilbm);
        return(error);
        }

/* deletebrush
 *
 * closes and deallocates created brush bitmap and colors
 */
void deletebrush(ilbm)
struct ILBMInfo         *ilbm;
        {
        freebitmap(ilbm);
        freecolors(ilbm);
        }


/* getbitmap
 *
 * Passed an initialized ILBMInfo with parsed IFFHandle (chunks parsed,
 * stopped at BODY), allocates a BitMap structure and planes just large
 * enough for the BODY.  Generally used for brushes but may be used
 * to load backgrounds without displaying, or to load deep ILBM's.
 * Sets ilbm->brbitmap.  Returns 0 for success.
 */
LONG getbitmap(struct ILBMInfo *ilbm)
        {
        struct IFFHandle        *iff;
        BitMapHeader    *bmhd;
        USHORT                  wide, high;
        LONG  error = NULL;
        int k, extra=0;
        BYTE deep;

        if(!(iff=ilbm->ParseInfo.iff))  return(CLIENT_ERROR);

        ilbm->brbitmap = NULL;

        if (!( bmhd = (BitMapHeader *)
                        findpropdata(iff, ID_ILBM, ID_BMHD)))
                {
                message("No ILBM.BMHD chunk!\n");
                return(IFFERR_SYNTAX);
                }

        *(&ilbm->Bmhd) = *bmhd; /* copy contents of BMHD */

        wide = BitsPerRow(bmhd->w);
        high = bmhd->h;
        deep = bmhd->nPlanes;

        ilbm->camg = getcamg(ilbm);

        D(bug("allocbitmap: bmhd=$%lx wide=%ld high=%ld deep=%ld\n",
                        bmhd,wide,high,deep));
        /*
         * Allocate Bitmap and planes
         */
        extra = deep > 8 ? deep - 8 : 0;
        if(ilbm->brbitmap = AllocMem(sizeof(struct BitMap)+(extra<<2),MEMF_CLEAR))
                {
                InitBitMap(ilbm->brbitmap,deep,wide,high);
                for(k=0; k<deep && (!error); k++) 
                    {
                    if(!(ilbm->brbitmap->Planes[k] = AllocRaster(wide,high))) 
                        error = 1;
                    if(! error)
                        BltClear(ilbm->brbitmap->Planes[k],RASSIZE(wide,high),0);
                    }

                if(error)
                    {
                    message("Failed to allocate raster\n");
                    freebitmap(ilbm);
                    }
                }
        else error = 1;
        return(error);
        }

        
/* freebitmap
 *
 * deallocates ilbm->brbitmap BitMap structure and planes 
 */
void freebitmap(struct ILBMInfo * ilbm)
        {
        int k, extra=0;

        if(ilbm->brbitmap)
                {       
                for(k=0; k< ilbm->brbitmap->Depth; k++) 
                        {
                        if(ilbm->brbitmap->Planes[k]) 
                                FreeRaster(ilbm->brbitmap->Planes[k],
                                        (USHORT)(ilbm->brbitmap->BytesPerRow << 3),
                                        ilbm->brbitmap->Rows);
                        }

                extra = ilbm->brbitmap->Depth > 8 ? ilbm->brbitmap->Depth - 8 : 0;
                FreeMem(ilbm->brbitmap,sizeof(struct BitMap) + (extra << 2));
                ilbm->brbitmap = NULL;
                }
        }

/* loadbrush
 *
 * Passed an initialized ILBMInfo with a not-in-use ParseInfo.iff
 *   IFFHandle and desired propchks, collectchks, and stopchks, and filename,
 *   will load an ILBM as a brush, setting up ilbm->Bmhd, ilbm->camg,
 *   ilbm->brbitmap, ilbm->colortable, and ilbm->ncolors
 *
 *   Note that ncolors may be more colors than you can LoadRGB4.
 *   Use MIN(ilbm->ncolors,MAXAMCOLORREG) for color count if you change
 *   the colors yourself using 1.3/2.0 functions.
 *
 * Returns 0 for success or an IFFERR (libraries/iffparse.h)
 */

LONG loadbrush(struct ILBMInfo *ilbm, UBYTE *filename)
{
LONG error = 0L;

    if(!(ilbm->ParseInfo.iff))  return(CLIENT_ERROR);

    if(!(error = openifile((struct ParseInfo *)ilbm, filename, IFFF_READ)))
        {
        error = parseifile((struct ParseInfo *)ilbm,
                                ID_FORM, ID_ILBM,
                                ilbm->ParseInfo.propchks,
                                ilbm->ParseInfo.collectchks,
                                ilbm->ParseInfo.stopchks);
        if((!error)||(error == IFFERR_EOC)||(error == IFFERR_EOF))
            {
            if(contextis(ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
                {
                if(error = createbrush(ilbm))   deletebrush(ilbm);
                }
            else
                {
                closeifile((struct ParseInfo *)ilbm);
                message("Not an ILBM\n");
                error = NOFILE;
                }
            }
        }
    return(error);
}


/* unloadbrush
 *
 * frees and close everything alloc'd/opened by loadbrush
 */
void unloadbrush(struct ILBMInfo *ilbm)
{
    closeifile((struct ParseInfo *)ilbm);
    deletebrush(ilbm);
}
		
/* end additional functions */

/* saveilbm
 *
 * Given an ILBMInfo with a currently available (not-in-use)
 *   ParseInfo->iff IFFHandle, a BitMap ptr, modeid, widths/heights,
 *   colortable, ncolors, bitspergun, masking, transparent color,
 *   optional chunklists, and filename, will save the bitmap as an ILBM.
 *
 *  if bitspergun=4,  colortable is words, each with nibbles 0RGB
 *  if bitspergun=8,  colortable is byte guns of RGBRGB etc. (like a CMAP)
 *  if bitspergun=32, colortable is ULONG guns of RGBRGB etc.
 *     Only the high eight bits of each gun will be written to CMAP.
 *     Four bit guns n will be saved as nn
 *
 * The struct Chunk *chunklist is for chunks you wish written
 * other than BMHD, CMAP, and CAMG (they will be screened out)
 * because they are calculated and written separately
 *
 * Returns 0 for success, or an IFFERR
 */

LONG saveilbm (struct ILBMInfo *ilbm,struct BitMap *bitmap,
               ULONG modeid,WORD width,WORD height,WORD pagewidth,WORD pageheight,
               APTR colortable,UWORD ncolors,UWORD bitspergun,
               WORD masking,WORD transparentColor,
               struct Chunk *chunklist1,struct Chunk *chunklist2,
               UBYTE *filename)
{
struct IFFHandle *iff;

struct Chunk *chunk;

ULONG chunkID;

UBYTE *bodybuf;

LONG size, error;

  iff = ilbm->ParseInfo.iff;

  if (! (modeid & 0xFFFF0000)) modeid &= OLDCAMGMASK;

  if (! (bodybuf = AllocVec (BODYBUFSZ,MEMF_PUBLIC))) return (IFFERR_NOMEM);

  error = openifile (&(ilbm->ParseInfo), filename, IFFF_WRITE);

  if (! error)
  {
    error = PushChunk (iff, ID_ILBM, ID_FORM, IFFSIZE_UNKNOWN);

    initbmhd (&(ilbm->Bmhd), bitmap, masking, cmpByteRun1, transparentColor, width, height, pagewidth, pageheight, modeid);

    CkErr (putbmhd (iff,&ilbm->Bmhd));

    if (colortable) CkErr (putcmap (iff,colortable,ncolors,bitspergun));

    if (ilbm->IFFPFlags & IFFPF_NOMONITOR) modeid &= (~MONITOR_ID_MASK);

    ilbm->camg = modeid;

    CkErr (putcamg (iff,&modeid));

    /* Write out chunklists 1 & 2 (if any), except for

     * any BMHD, CMAP, or CAMG (computed/written separately) */

    for (chunk = chunklist1; chunk; chunk = chunk->ch_Next)
    {
      chunkID = chunk->ch_ID;

      if ((chunkID != ID_BMHD) && (chunkID != ID_CMAP) && (chunkID != ID_CAMG))
      {
        size = chunk->ch_Size == IFFSIZE_UNKNOWN ? strlen (chunk->ch_Data) : chunk->ch_Size;

        CkErr (PutCk (iff, chunkID, size, chunk->ch_Data));
      }
    }

    for (chunk = chunklist2; chunk; chunk = chunk->ch_Next)
    {
      chunkID = chunk->ch_ID;

      if ((chunkID != ID_BMHD) && (chunkID != ID_CMAP) && (chunkID != ID_CAMG))
      {
        size = chunk->ch_Size == IFFSIZE_UNKNOWN ? strlen (chunk->ch_Data) : chunk->ch_Size;

        CkErr (PutCk (iff, chunkID, size, chunk->ch_Data));
      }
    }

    /* Write out the BODY */

    CkErr (putbody (iff, bitmap, NULL, &ilbm->Bmhd, bodybuf, BODYBUFSZ));

    CkErr (PopChunk (iff)); /* close out the FORM */

    closeifile (&(ilbm->ParseInfo));    /* and the file */
  }

  FreeVec (bodybuf);

  return (error);
}

/* openifile
 *
 * Passed a ParseInfo structure with a not-in-use IFFHandle, filename
 *   and IFF open mode (IFFF_READ or IFFF_WRITE) opens file for use with
 *   iffparse.library support modules.
 *
 * Returns 0 for success or an IFFERR (libraries/iffparse.h)
 */

LONG openifile (struct ParseInfo *pi, UBYTE *filename, ULONG iffopenmode)
{
struct IFFHandle *iff;

LONG error = CLIENT_ERROR;

  if (! pi) return (error);

  if (! (iff = pi->iff)) return (error);

  pi->clipboard = FALSE;

  /* Set up IFFHandle for buffered stdio I/O. */

  if (! (iff->iff_Stream = (ULONG) fopen (filename,omodes [iffopenmode & 1]))) return (NOFILE);

  else initiffasstdio (iff);

  pi->filename = filename;

  error = OpenIFF (iff,iffopenmode);

  pi->opened = error ? FALSE : TRUE;    /* currently open handle */

  return (error);
}

/* closeifile
 *
 * closes file or clipboard opened with openifile, and frees all
 *   iffparse context parsed by parseifile.
 *
 */

void closeifile (struct ParseInfo *pi)
{
struct IFFHandle *iff;

  if (! pi) return;

  if (! (iff = pi->iff)) return;

  if (pi->opened) CloseIFF (iff);

  if (iff->iff_Stream)
  {
    if (pi->clipboard) CloseClipboard ((struct ClipboardHandle *) (iff->iff_Stream));

    else fclose ((FILE *) (iff->iff_Stream));
  }

  iff->iff_Stream = NULL;

  pi->clipboard = NULL;

  pi->opened = NULL;
}

/*
 * File I/O hook functions which the IFF library will call.
 * A return of 0 indicates success (no error).
 *
 * Iffparse.library calls this code via struct Hook and Hook.asm
 */
static LONG SAVEDS ASMCALL stdio_stream (REG (a0,struct Hook *hook),REG (a2,struct IFFHandle *iff),REG (a1,struct IFFStreamCmd *actionpkt))
{
register FILE *stream;
register LONG nbytes;
register LONG actual;
register UBYTE *buf;
LONG len;

  stream = (FILE *) iff->iff_Stream;

  if (! stream) return (1L);

  nbytes = actionpkt->sc_NBytes;

  buf = (UBYTE *) actionpkt->sc_Buf;

  switch (actionpkt->sc_Command)
  {
    case IFFSCC_READ: do
                      {  actual = (nbytes > 32767 ? 32767 : nbytes);

                         if ((len = fread (buf, 1, (size_t) actual, stream)) != actual) break;

                         nbytes -= actual;

                         buf += actual;

                      } while (nbytes > NULL);

                      return (nbytes ? IFFERR_READ : NULL);

    case IFFSCC_WRITE: do
                       {  actual = (nbytes > 32767 ? 32767 : nbytes);

                          if ((len = fwrite (buf, 1, (size_t) actual, stream)) != actual) break;

                          nbytes -= actual;

                          buf += actual;

                       } while (nbytes > NULL);

                       return (nbytes ? IFFERR_WRITE : NULL);

    case IFFSCC_SEEK:  return ((fseek (stream, nbytes, 1) == -1) ? IFFERR_SEEK : NULL);

    default:           return (NULL);  /*  No _INIT or _CLEANUP required.  */
  }
}

/* initiffasstdio (ie. init iff as stdio)
 *
 * sets up hook callback for the file stream handler above
 */

void initiffasstdio (struct IFFHandle *iff)
{
static struct Hook stdiohook = {{NULL},(ULONG (*)()) stdio_stream,NULL,NULL};

/*
 * Initialize the IFF structure to point to the buffered I/O
 * routines.  Unbuffered I/O is terribly slow.
 */

  InitIFF (iff, IFFF_FSEEK | IFFF_RSEEK, &stdiohook);
}

/* PutCk
 * Writes one chunk of data to an iffhandle
 */

LONG PutCk (struct IFFHandle *iff,long id,long size,void *data)
{
LONG error,wlen;

  if (! (error = PushChunk (iff,0,id,size)))
  {
     /* Write the actual data */

     if ((wlen = WriteChunkBytes (iff,data,size)) != size) error = IFFERR_WRITE;

     else error = PopChunk (iff);
  }

  return (error);
}

/*---------- initbmhd -------------------------------------------------*/

LONG initbmhd (BitMapHeader *bmhd, struct BitMap *bitmap,
               WORD masking, WORD compression, WORD transparentColor,
               WORD width, WORD height, WORD pageWidth, WORD pageHeight,
               ULONG modeid)
{
  struct DisplayInfo DI;

  if ((! bmhd) || (! bitmap) || (! width) || (! height)) return (CLIENT_ERROR);

  bmhd->w = width;

  bmhd->h = height;

  bmhd->x = bmhd->y = 0;    /* Default position is (0,0).*/

  bmhd->nPlanes = bitmap->Depth;

  bmhd->masking = masking;

  bmhd->compression = compression;

  bmhd->flags = BMHDF_CMAPOK;   /* we will store 8 significant bits */

  bmhd->transparentColor = transparentColor;

  bmhd->pageWidth = pageWidth;

  bmhd->pageHeight = pageHeight;

  bmhd->xAspect = 0;    /* So we can tell when we've got it */

  if (GetDisplayInfoData (NULL, (UBYTE *) &DI, sizeof (struct DisplayInfo), DTAG_DISP, modeid))
  {
     bmhd->xAspect = DI.Resolution.x;

     bmhd->yAspect = DI.Resolution.y;
  }

    /* If running under 1.3 or GetDisplayInfoData failed, use old method
     * of guessing aspect ratio */

  if (! bmhd->xAspect)
  {
    bmhd->xAspect =  44;

    bmhd->yAspect = ((struct GfxBase *) GfxBase)->DisplayFlags & PAL ? 44 : 52;

    if (modeid & HIRES) bmhd->xAspect = bmhd->xAspect >> 1;

    if (modeid & LACE)  bmhd->yAspect = bmhd->yAspect >> 1;
  }

  return (IFF_OKAY);
}

/*---------- putcmap ---------------------------------------------------*/
/*  This function will accept a table of color values in one of the
 *  following forms:
 *  if bitspergun=4,  colortable is words, each with nibbles 0RGB
 *  if bitspergun=8,  colortable is bytes of RGBRGB etc. (like a CMAP)
 *  if bitspergun=32, colortable is ULONGS of RGBRGB etc.
 *  (only the high eight bits of each gun will be written to CMAP)
 */

LONG putcmap (struct IFFHandle *iff,APTR colortable,UWORD ncolors,UWORD bitspergun)
{
LONG error, offs;

UWORD  *tabw;

UBYTE *tab8;

ColorRegister cmapReg;

  if ((! iff) || (! colortable)) return (CLIENT_ERROR);

  /* size of CMAP is 3 bytes * ncolors */

  if (error = PushChunk (iff,NULL,ID_CMAP,(LONG) (ncolors * sizeofColorRegister))) return (error);

  if (bitspergun == 4)
  {
    /* Store each 4-bit value n as nn */

    tabw = (UWORD *) colortable;

    for (; ncolors; --ncolors)
    {
      cmapReg.red    = ( *tabw >> 4 ) & 0xf0;

      cmapReg.red   |= (cmapReg.red >> 4);

      cmapReg.green  = ( *tabw      ) & 0xf0;

      cmapReg.green |= (cmapReg.green >> 4);

      cmapReg.blue   = ( *tabw << 4 ) & 0xf0;

      cmapReg.blue  |= (cmapReg.blue >> 4);

      if ((WriteChunkBytes (iff,(BYTE *) &cmapReg,sizeofColorRegister)) != sizeofColorRegister) return (IFFERR_WRITE);

      ++tabw;
    }
  }

  else

    if ((bitspergun == 8) || (bitspergun == 32))
    {
      tab8 = (UBYTE *) colortable;

      offs = (bitspergun == 8) ? 1 : 4;

      for ( ;  ncolors;  --ncolors )
      {
        cmapReg.red   = *tab8;

        tab8 += offs;

        cmapReg.green = *tab8;

        tab8 += offs;

        cmapReg.blue  = *tab8;

        tab8 += offs;

        if ((WriteChunkBytes (iff,(BYTE *) &cmapReg,sizeofColorRegister)) != sizeofColorRegister) return (IFFERR_WRITE);
      }
    }

  error = PopChunk (iff);

  return (error);
}

/*---------- putbody ---------------------------------------------------*/
/* NOTE: This implementation could be a LOT faster if it used more of the
 * supplied buffer. It would make far fewer calls to IFFWriteBytes (and
 * therefore to DOS Write).
 *
 * Incorporates modification by Jesper Steen Moller to accept source
 * rows wider than dest rows, with one modulo variable for source bitplane
 * rows and one for the ILBM bitmap rows.
 */

LONG putbody (struct IFFHandle *iff, struct BitMap *bitmap, BYTE *mask,BitMapHeader *bmhd, BYTE *buffer, LONG bufsize)
{
LONG error;

LONG rowBytes = bitmap->BytesPerRow;   /* for source modulo only */

LONG FileRowBytes = RowBytes (bmhd->w); /* width to write in bytes */

int dstDepth = bmhd->nPlanes;

UBYTE compression = bmhd->compression;

int planeCnt;                          /* number of bit planes including mask */

register int iPlane, iRow;

register LONG packedRowBytes;

BYTE *buf;

BYTE *planes [MAXSAVEDEPTH + 1];        /* array of ptrs to planes & mask */

  if ( bufsize < MaxPackedSize (FileRowBytes) ||  /* Must buffer a comprsd row*/

       compression > cmpByteRun1              ||  /* bad arg */

       bitmap->Rows != bmhd->h                ||  /* inconsistent */

       rowBytes < FileRowBytes                ||  /* inconsistent*/

       bitmap->Depth < dstDepth               ||  /* inconsistent */

       dstDepth > MAXSAVEDEPTH )                  /* too many for this routine*/

    return (CLIENT_ERROR);

  planeCnt = dstDepth + (mask == NULL ? 0 : 1);

  /* Copy the ptrs to bit & mask planes into local array "planes" */

  for (iPlane = 0; iPlane < dstDepth; iPlane++) planes [iPlane] = (BYTE *) bitmap->Planes [iPlane];

  if (mask != NULL) planes [dstDepth] = mask;

  /* Write out a BODY chunk header */

  if (error = PushChunk (iff, NULL, ID_BODY, IFFSIZE_UNKNOWN)) return (error);

  /* Write out the BODY contents */

  for (iRow = bmhd->h; iRow > 0; iRow--)
  {
    for (iPlane = 0; iPlane < planeCnt; iPlane++)
    {
      /* Write next row.*/

      if (compression == cmpNone)
      {
        if (WriteChunkBytes (iff,planes [iPlane],FileRowBytes) != FileRowBytes) error = IFFERR_WRITE;

        planes [iPlane] += rowBytes; /* Possibly skipping unused bytes */
      }

      else                     /* Compress and write next row.*/
      {
        buf = buffer;

        packedRowBytes = PackRow (&planes [iPlane], &buf, FileRowBytes);

        /* Note that packrow incremented planes already by FileRowBytes */

        planes [iPlane] += rowBytes-FileRowBytes; /* Possibly skipping unused bytes */

        if (WriteChunkBytes (iff,buffer,packedRowBytes) != packedRowBytes) error = IFFERR_WRITE;
      }

      if (error) return (error);
    }
  }

  /* Finish the chunk */

  error = PopChunk (iff);

  return (error);
}

LONG loadcmap (struct ILBMInfo *ilbm)
{
struct StoredProperty *sp;

struct IFFHandle *iff;

BOOL AllShifted;

UBYTE *rgb, rb, gb, bb;

LONG k;

ULONG ncolors, gun, ncheck, nc, r, g, b;

  if (! (iff = ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  if (! (ilbm->colortable)) return (1L);

  if (! (sp = FindProp (iff, ID_ILBM, ID_CMAP))) return (1L);

  rgb = sp->sp_Data;

  /* file has this many colors */

  nc = sp->sp_Size / sizeofColorRegister;

  ncolors = nc;

  /* if ILBMInfo can't hold that many, we'll load less */

  if (ilbm->ncolors < ncolors) ncolors = ilbm->ncolors;

  /* set to how many we are loading */

  ilbm->ncolors = ncolors;

  /* how many colors to check for shifted nibbles (i.e. used colors) */

  ncheck = 1L << ilbm->Bmhd.nPlanes;

  if (ncheck > ncolors) ncheck = ncolors;

  if ((! (ilbm->IFFPFlags & IFFPF_NOCOLOR32)) && (ilbm->colorrecord))
  {
    ilbm->colorrecord [0] = ncolors << 16L;

    /* Assign to 32-bit table, examine for all-shifted nibbles at same time */

    AllShifted = TRUE;

    k = 0;

    while (ncheck--)
    {
      ilbm->colortable32 [k].r = rb = *rgb++;

      ilbm->colortable32 [k].g = gb = *rgb++;

      ilbm->colortable32 [k].b = bb = *rgb++;

      if (((rb & 0x0F) || (gb & 0x0F) || (bb & 0x0F))) AllShifted = FALSE;

      k++;
    }

    /* If no file/user indication that this is an 8-bit significant CMAP... */

    if ((! (ilbm->IFFPFlags & IFFPF_CMAPOK)) && (! (ilbm->Bmhd.flags & BMHDF_CMAPOK)))
    {
      /* If all nibbles appear shifted (4 bit), duplicate the nibbles */

      if (AllShifted)
      {
        for (k = 0; k < nc; k++)
        {
          ilbm->colortable32 [k].r |= (ilbm->colortable32 [k].r >> 4);

          ilbm->colortable32 [k].g |= (ilbm->colortable32 [k].g >> 4);

          ilbm->colortable32 [k].b |= (ilbm->colortable32 [k].b >> 4);
        }
      }
    }

    /* Now scale to 32 bits */

    for (k = 0; k < nc; k++)
    {
      gun = ilbm->colortable32 [k].r;

      ilbm->colortable32 [k].r |= ((gun << 24) | (gun << 16) | (gun << 8));

      gun = ilbm->colortable32 [k].g;

      ilbm->colortable32 [k].g |= ((gun << 24) | (gun << 16) | (gun << 8));

      gun = ilbm->colortable32 [k].b;

      ilbm->colortable32 [k].b |= ((gun << 24) | (gun << 16) | (gun << 8));
    }
  }

  /* always make old-style table */

  rgb = sp->sp_Data;

  ncolors = nc;

  k = 0;

  while (ncolors--)
  {
    r = (*rgb++ & 0xF0) << 4;

    g = *rgb++ & 0xF0;

    b = *rgb++ >> 4;

    ilbm->colortable [k] = r | g | b;

    k++;
  }

  return (NULL);
}

LONG loadcmapRGB (struct ILBMInfo *ilbm, UBYTE* clut)
{
BOOL AllShifted;

UBYTE *rgb, rb, gb, bb;

LONG k;

ULONG ncolors, gun, ncheck, nc, r, g, b;

  if (! (ilbm->colortable)) return (1L);

  rgb = clut; /* clut from NewRGBMANDChunk->Colors */

  /* file has this many colors */

  nc = 256; 

  ncolors = nc;

  /* printf("loadcmapRGB: ilbm->ncolors: %u ncolors: %u\n", ilbm->ncolors, ncolors); */
  
  if (ilbm->ncolors < ncolors) ncolors = ilbm->ncolors;

  /* set to how many we are loading */

  ilbm->ncolors = ncolors; /* must be 256 (because it is / must be a truecolor image) */

  /* how many colors to check for shifted nibbles (i.e. used colors) */

  /* printf("loadcmapRGB(): ilbm->Bmhd.nPlanes: %u\n", ilbm->Bmhd.nPlanes); */
  ncheck = 1L << ilbm->Bmhd.nPlanes;

  /* printf("loadcmapRGB(): ncheck: %u\n", ncheck); */
  if (ncheck > ncolors) ncheck = ncolors;

  if ((! (ilbm->IFFPFlags & IFFPF_NOCOLOR32)) && (ilbm->colorrecord))
  {
    ilbm->colorrecord [0] = ncolors << 16L;

    /* Assign to 32-bit table, examine for all-shifted nibbles at same time */

	AllShifted = TRUE;

    k = 0;

    while (ncheck--)
    {
      ilbm->colortable32 [k].r = rb = *rgb++;

      ilbm->colortable32 [k].g = gb = *rgb++;

      ilbm->colortable32 [k].b = bb = *rgb++;

      if (((rb & 0x0F) || (gb & 0x0F) || (bb & 0x0F))) AllShifted = FALSE;

      k++;
    }

    /* If no file/user indication that this is an 8-bit significant CMAP... */
	
/*    if ((! (ilbm->IFFPFlags & IFFPF_CMAPOK)) && (! (ilbm->Bmhd.flags & BMHDF_CMAPOK))) */
	if (1)
    {
      /* If all nibbles appear shifted (4 bit), duplicate the nibbles */

      if (AllShifted)
      {
        for (k = 0; k < nc; k++)
        {
          ilbm->colortable32 [k].r |= (ilbm->colortable32 [k].r >> 4);

          ilbm->colortable32 [k].g |= (ilbm->colortable32 [k].g >> 4);

          ilbm->colortable32 [k].b |= (ilbm->colortable32 [k].b >> 4);
        }
      }
    }

    /* Now scale to 32 bits */

    for (k = 0; k < nc; k++)
    {
      gun = ilbm->colortable32 [k].r;

      ilbm->colortable32 [k].r |= ((gun << 24) | (gun << 16) | (gun << 8));

      gun = ilbm->colortable32 [k].g;

      ilbm->colortable32 [k].g |= ((gun << 24) | (gun << 16) | (gun << 8));

      gun = ilbm->colortable32 [k].b;

      ilbm->colortable32 [k].b |= ((gun << 24) | (gun << 16) | (gun << 8));
    }
  }

  /* always make old-style table */

  rgb = clut; 

  ncolors = nc;

  k = 0;

  while (ncolors--)
  {
    r = (*rgb++ & 0xF0) << 4;

    g = *rgb++ & 0xF0;

    b = *rgb++ >> 4;

    ilbm->colortable [k] = r | g | b;

    k++;
  }

  return (NULL);
}


LONG getcolors (struct ILBMInfo *ilbm)
{
struct IFFHandle *iff;

LONG error = CLIENT_ERROR;

  if (! (iff = ilbm->ParseInfo.iff)) return (error);

  if (! (error = alloccolortable (ilbm))) error = loadcmap (ilbm);

  if (error) freecolors (ilbm);

  return (error);
}

/* alloccolortable - allocates ilbm->colortable and sets ilbm->ncolors
 *  to the number of colors we have room for in the table.
 *
 * V39 and above: unless ilbm->IFFPFlags & IFFPF_NOCOLOR32, will also
 *  allocate and build a 32-bit per gun colortable (ilbm->colortable32)
 *  and ilbm->colorrecord for LoadRGB32()
 */

LONG alloccolortable (struct ILBMInfo *ilbm)
{
struct IFFHandle *iff;

struct StoredProperty *sp;

LONG error = NULL;

ULONG ctabsize;

UWORD ncolors;

  if (! (iff = ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  if (sp = FindProp (iff,ID_ILBM,ID_CMAP))
  {
     /*
      * Compute the size table we need
      */

     ncolors = sp->sp_Size / 3;     /* how many in CMAP */

     ncolors = MAX (ncolors,16);    /* alloc at least 16 */

     ctabsize = ncolors * sizeof (Color4);

     if (ilbm->colortable = (Color4 *) AllocVec (ctabsize,MEMF_PUBLIC | MEMF_CLEAR))
     {
    ilbm->ncolors = ncolors;

    ilbm->ctabsize = ctabsize;

        if ((! (ilbm->IFFPFlags & IFFPF_NOCOLOR32)))
    {
           ctabsize = (ncolors * sizeof (Color32)) + (2 * sizeof (LONG));

       if (ilbm->colorrecord = (WORD *) AllocVec (ctabsize,MEMF_PUBLIC | MEMF_CLEAR))
           {
          ilbm->crecsize = ctabsize;

          ilbm->colortable32 = (Color32 *) (&ilbm->colorrecord [1L]);

          ilbm->colorrecord [0L] = ncolors << 16L;   /* For LoadRGB32 */

              ilbm->colorrecord [ncolors * sizeof (Color32) + 1L] = NULL;
           }

           else error = IFFERR_NOMEM;
        }
     }

     else error = IFFERR_NOMEM;
  }

  if (error) freecolors (ilbm);

  return (error);
}

LONG alloccolortableRGB (struct ILBMInfo *ilbm)
{
/*struct IFFHandle *iff;

struct StoredProperty *sp;
*/
LONG error = NULL;

ULONG ctabsize;

UWORD ncolors;

     /*
      * Compute the size table we need
      */

	 ncolors = 256;
	
     ctabsize = ncolors * sizeof (Color4);

     if (ilbm->colortable = (Color4 *) AllocVec (ctabsize,MEMF_PUBLIC | MEMF_CLEAR))
     {
    
		ilbm->ncolors = ncolors;

    	ilbm->ctabsize = ctabsize;

        if ((! (ilbm->IFFPFlags & IFFPF_NOCOLOR32)))
    	{
    	   ctabsize = (ncolors * sizeof (Color32)) + (2 * sizeof (LONG));

   		   if (ilbm->colorrecord = (WORD *) AllocVec (ctabsize,MEMF_PUBLIC | MEMF_CLEAR))
     	   {
		  		/*printf("alloccolortableRGB(): Color32 (?) allocated.\n");*/
		  		ilbm->crecsize = ctabsize;

          		ilbm->colortable32 = (Color32 *) (&ilbm->colorrecord [1L]);

          		ilbm->colorrecord [0L] = ncolors << 16L;   /* For LoadRGB32 */

           		ilbm->colorrecord [ncolors * sizeof (Color32) + 1L] = NULL;
          	}

          	else error = IFFERR_NOMEM;
        }
     }

     else error = IFFERR_NOMEM;
  
  if (error) freecolors (ilbm);

  return (error);
}

VOID freecolors (struct ILBMInfo *ilbm)
{
   if (ilbm->colortable) FreeVec (ilbm->colortable);

   ilbm->colortable = NULL;

   ilbm->ctabsize = 0;

   if (ilbm->colorrecord) FreeVec (ilbm->colorrecord);

   ilbm->colorrecord  = NULL;

   ilbm->colortable32 = NULL;

   ilbm->crecsize = 0;
}

LONG currentchunkis (struct IFFHandle *iff,LONG type,LONG id)
{
register struct ContextNode *cn;

LONG result = 0;

  if (cn = CurrentChunk (iff))

    if ((cn->cn_Type == type) && (cn->cn_ID == id)) result = 1;

  return (result);
}

/*---------- loadbody ---------------------------------------------------*/

LONG loadbody (struct IFFHandle *iff,struct BitMap *bitmap,BitMapHeader *bmhd)
{
BYTE *buffer;

ULONG bufsize;

LONG error = 1L;

  if (! (currentchunkis (iff,ID_ILBM,ID_BODY))) return (IFF_OKAY);

  if ((bitmap) && (bmhd))
  {
     bufsize = MaxPackedSize (RowBytes (bmhd->w)) << 4L;

     if (! (buffer = AllocVec (bufsize,0L))) return (IFFERR_NOMEM);

     error = loadbody2 (iff,bitmap,NULL,bmhd,buffer,bufsize);

     FreeVec (buffer);
  }

  return (error);
}

LONG loadbody2 (struct IFFHandle *iff,struct BitMap *bitmap,BYTE *mask,BitMapHeader *bmhd,BYTE *buffer,ULONG bufsize)
{
UBYTE srcPlaneCnt = bmhd->nPlanes;   /* Haven't counted for mask plane yet*/

WORD srcRowBytes = RowBytes (bmhd->w);

WORD destRowBytes = bitmap->BytesPerRow;   /* used as a modulo only */

LONG bufRowBytes = MaxPackedSize (srcRowBytes);

WORD nRows = bmhd->h;

WORD destWidthBytes;            /* used for width check */

WORD compression = bmhd->compression;

register WORD iPlane, iRow, nEmpty;

register WORD nFilled;

BYTE *buf, *nullDest, *nullBuf, **pDest;

BYTE *planes [MaxSrcPlanes]; /* array of ptrs to planes & mask */

struct ContextNode *cn;

   cn = CurrentChunk (iff);

   if (compression > cmpByteRun1) return (CLIENT_ERROR);

   /* If >=V39, this may be an interleaved bitmap with a BytesPerRow
    * which is truly just a modulo and actually includes ALL planes.
    * So instead, for bounds checking, we use the pixel width of
    * the BitMap rounded up to nearest WORD, since saved ILBMs
    * are always saved as their width rounded up to nearest WORD.
    */

   destWidthBytes = RowBytes (GetBitMapAttr (bitmap,BMA_WIDTH));

   /* Complain if client asked for a conversion GetBODY doesn't handle.*/

   if (srcRowBytes > destWidthBytes || bufsize < (bufRowBytes * 2) || srcPlaneCnt > MaxSrcPlanes) return (CLIENT_ERROR);

   if (nRows > bitmap->Rows) nRows = bitmap->Rows;

   /* Initialize array "planes" with bitmap ptrs; NULL in empty slots.*/

   for (iPlane = 0; iPlane < bitmap->Depth; iPlane++) planes [iPlane] = (BYTE *) bitmap->Planes [iPlane];

   for ( ; iPlane < MaxSrcPlanes; iPlane++) planes [iPlane] = NULL;

   /* Copy any mask plane ptr into corresponding "planes" slot.*/

   if (bmhd->masking == mskHasMask)
   {
      if (mask) planes [srcPlaneCnt] = mask;  /* If there are more srcPlanes than

                                               * dstPlanes, there will be NULL plane-pointers before this. */

      else  planes [srcPlaneCnt] = NULL;  /* In case more dstPlanes than src. */

      srcPlaneCnt += 1;  /* Include mask plane in count.*/
   }

   /* Setup a sink for dummy destination of rows from unwanted planes.*/

   nullDest = buffer;

   buffer  += srcRowBytes;

   bufsize -= srcRowBytes;

   /* Read the BODY contents into client's bitmap.
    * De-interleave planes and decompress rows.
    * MODIFIES: Last iteration modifies bufsize.*/

   buf = buffer + bufsize;  /* Buffer is currently empty.*/

   for (iRow = nRows; iRow > 0; iRow--)
   {
       for (iPlane = 0; iPlane < srcPlaneCnt; iPlane++)
       {
        pDest = &planes [iPlane];

            /* Establish a sink for any unwanted plane.*/

            if (*pDest == NULL)
            {
            nullBuf = nullDest;

                pDest   = &nullBuf;
            }

            /* Read in at least enough bytes to uncompress next row.*/

            nEmpty  = buf - buffer;   /* size of empty part of buffer.*/

            nFilled = bufsize - nEmpty;   /* this part has data.*/

        if (nFilled < bufRowBytes)
            {
            /* Need to read more.*/
            /* Move the existing data to the front of the buffer.*/
            /* Now covers range buffer[0]..buffer[nFilled-1].*/

                CopyMem (buf,buffer,nFilled);  /* Could be moving 0 bytes.*/

                if (nEmpty > ChunkMoreBytes (cn))
        {
                    /* There aren't enough bytes left to fill the buffer.*/

                    nEmpty = ChunkMoreBytes (cn);

                    bufsize = nFilled + nEmpty;  /* heh-heh */
                }

            /* Append new data to the existing data.*/

                if (ReadChunkBytes (iff,&buffer [nFilled],(LONG) nEmpty) < nEmpty) return (CLIENT_ERROR);

                buf = buffer;

            nFilled = bufsize;
            }

        /* Copy uncompressed row to destination plane.*/

            if (compression == cmpNone)
            {
                if (nFilled < srcRowBytes)  return (IFFERR_MANGLED);

            CopyMem (buf,*pDest,srcRowBytes);

            buf    += srcRowBytes;

                *pDest += destRowBytes;
            }

        else
            {
            /* Decompress row to destination plane.*/

                if (UnPackRow (&buf,pDest,nFilled,srcRowBytes)) return (IFFERR_MANGLED);

                           /*  pSource, pDest, srcBytes, dstBytes  */

            else *pDest += (destRowBytes - srcRowBytes);

            }
    }
   }

   return (IFF_OKAY);
}

ULONG getcamg (struct ILBMInfo *ilbm)
{
struct IFFHandle *iff;

struct StoredProperty *sp;

UWORD wide,high,deep;

ULONG modeid = NULL;

  if (! (iff = ilbm->ParseInfo.iff)) return (NULL);

  wide = ilbm->Bmhd.pageWidth;

  high = ilbm->Bmhd.pageHeight;

  deep = ilbm->Bmhd.nPlanes;

  /* Grab CAMG's idea of the viewmodes */

  if (sp = FindProp (iff, ID_ILBM, ID_CAMG))
  {
     modeid = (* (ULONG *) sp->sp_Data);

     /* knock bad bits out of old-style 16-bit viewmode CAMGs */

     if ((! (modeid & MONITOR_ID_MASK)) || ((modeid & EXTENDED_MODE) && (! (modeid & 0xFFFF0000)))) modeid &= (~(EXTENDED_MODE | SPRITES | GENLOCK_AUDIO | GENLOCK_VIDEO | VP_HIDE));

     /* check for bogus CAMG like DPaintII brushes
      * with junk in upper word and extended bit
      * not set in lower word.
      */

     if ((modeid & 0xFFFF0000) && (! (modeid & 0x00001000))) sp = NULL;
  }

  if (! sp)
  {
     /*
      * No CAMG (or bad CAMG) present; use computed modes.
      */

     modeid = NULL;     /* added in 39.6 */

     if (wide >= 640) modeid = HIRES;

     if (high >= 400) modeid |= LACE;

     if (deep == 6)
     {
        modeid |= ilbm->EHB ? EXTRA_HALFBRITE : HAM;
     }
  }

  if (ilbm->IFFPFlags & IFFPF_NOMONITOR) modeid &= (~MONITOR_ID_MASK);

  return (modeid);
}

LONG chkcnt (LONG *TaggedArray)
{
LONG k = 0;

  while (TaggedArray [k] != TAG_END) k++;

  return (k >> 1);
}

LONG getcontext (struct IFFHandle *iff)
{
LONG error;

  /* Based on our parse initialization, ParseIFF() will return on a stop chunk
   * (error = 0) or end of context for an ILBM FORM (error = IFFERR_EOC) or end of
   * file (error = IFFERR_EOF)
   */

  return (error = ParseIFF (iff,IFFPARSE_SCAN));
}

LONG parseifile (struct ParseInfo *pi,LONG groupid,LONG grouptype,LONG *propchks,LONG *collectchks,LONG *stopchks)
{
struct IFFHandle *iff;

register struct ContextNode *cn;

LONG error;

  if (! (iff = pi->iff)) return (CLIENT_ERROR);

  if (! iff->iff_Stream) return (IFFERR_READ);

  pi->hunt = FALSE;

  /* Declare property, collection and stop chunks.*/

  if (propchks)

      if (error = PropChunks (iff, propchks, chkcnt (propchks))) return (error);

  if (collectchks)

          if (error = CollectionChunks (iff, collectchks, chkcnt (collectchks))) return (error);

  if (stopchks)

          if (error = StopChunks (iff, stopchks, chkcnt (stopchks))) return (error);

    /* We want to stop at the end of an ILBM context. */

  if (grouptype)

      if (error = StopOnExit (iff, grouptype, groupid)) return (error);

    /* Take first parse step to enter main chunk. */

  if (error = ParseIFF (iff,IFFPARSE_STEP)) return (error);

    /* Test the chunk info to see if simple form of type we want (ILBM).*/

  if (! (cn = CurrentChunk (iff))) return (NOFILE); /* This really should never happen.  If it does, it means our parser is broken. */

  if (cn->cn_ID != groupid || cn->cn_Type != grouptype) pi->hunt = TRUE; /* Warning - this is a complex file */

  if (! error) error = getcontext (iff);

  return (error);
}

LONG contextis (struct IFFHandle *iff,LONG type,LONG id)
{
register struct ContextNode *cn;

LONG result = NULL;

  if (cn = (CurrentChunk (iff)))
  {
    if (cn = (ParentChunk (cn)))
    {
      if ((cn->cn_Type == type) && (cn->cn_ID == id)) result = 1L;
    }
  }

  return (result);
}

UBYTE *findpropdata (struct IFFHandle *iff,LONG type,LONG id)
{
register struct StoredProperty *sp;

  if (sp = FindProp (iff,type,id)) return (sp->sp_Data);

  return (0);
}

LONG setcolors (struct ILBMInfo *ilbm,struct ViewPort *vp)
{
LONG ncolors;

  if (! vp) return (CLIENT_ERROR);

  ncolors = MIN (ilbm->ncolors,vp->ColorMap->Count);

  /* printf("setcolors(): ncolors: %d\n", ncolors); */
  
  if ((! (ilbm->IFFPFlags & IFFPF_NOCOLOR32)) && (ilbm->colorrecord))

     Fade (ilbm->win,(ULONG *) ilbm->colorrecord,25L,1L,FROMBLACK);

  else

     if (ilbm->colortable) LoadRGB4 (vp,(UWORD *) ilbm->colortable,ncolors);

  return (NULL);
}

/**********  ByteRun1 ***************************************************/

static BYTE *PutDump (BYTE *dest, int nn)
{
  LONG i;

  PutByte (nn-1);

  for (i = 0;  i < nn;  i++) PutByte (PackBuffer [i]);

  return (dest);
}

static BYTE *PutRun (BYTE *dest, int nn, int cc)
{
  PutByte (-(nn-1));

  PutByte (cc);

  return( dest);
}

/*----------- packrow --------------------------------------------------*/

/* Given POINTERS TO POINTERS, packs one row, updating the source and
 * destination pointers.  RETURNs count of packed bytes. */

LONG PackRow (BYTE **pSource, BYTE **pDest, LONG rowSize)
{
BYTE *source, *dest;
char c,lastc;
BOOL mode = DUMP;
WORD nbuf;       /* number of chars in buffer */
WORD rstart = 0; /* buffer index current run starts */

  source = *pSource;

  dest = *pDest;

  PackPutSize = 0;

  PackBuffer [0] = lastc = c = GetByte ();  /* so have valid lastc */

  nbuf = 1;   rowSize--;    /* since one byte eaten.*/

  for (;  rowSize;  --rowSize)
  {
    PackBuffer [nbuf++] = c = GetByte ();

    switch (mode)
    {
      case DUMP:

        /* If the buffer is full, write the length byte, then the data */
        if (nbuf>MaxDat)
        {
          OutDump(nbuf-1);
          PackBuffer[0] = c;
          nbuf = 1;
          rstart = 0;
          break;
        }

        if (c == lastc)
        {
          if (nbuf-rstart >= MinRun)
          {
            if (rstart > 0)
              OutDump(rstart);
            mode = RUN;
          }
          else
            if (rstart == 0)
              mode = RUN; /* no dump in progress, so can't lose by making these 2 a run.*/
        }
        else
          rstart = nbuf-1;      /* first of run */
        break;

      case RUN:

        if ( (c != lastc)|| ( nbuf-rstart > MaxRun))
        {
          /* output run */
          OutRun(nbuf-1-rstart,lastc);
          PackBuffer[0] = c;
          nbuf = 1;
          rstart = 0;
          mode = DUMP;
        }
        break;
    }

    lastc = c;
  }

  switch (mode)
  {
    case DUMP: OutDump (nbuf);

               break;

    case RUN:  OutRun (nbuf-rstart,lastc);

               break;
  }

  *pSource = source;

  *pDest = dest;

  return (PackPutSize);
}

BOOL UnPackRow (BYTE **pSource, BYTE **pDest, WORD srcBytes0, WORD dstBytes0)
{
  register BYTE *source = *pSource;
  register BYTE *dest   = *pDest;
  register WORD n;
  register WORD srcBytes = srcBytes0;
  register WORD dstBytes = dstBytes0;
  BOOL error = TRUE;    /* assume error until we make it through the loop */
  WORD minus128 = -128;  /* get the compiler to generate a CMP.W */
  register BYTE c;

  while( dstBytes > 0 )
  {
    if ( (srcBytes -= 1) < 0 )
    {
      *pSource = source;
      *pDest = dest;
      return(error);
    }
    n = UGetByte();

    if (n >= 0)
    {
      n += 1;
      if ( (srcBytes -= n) < 0 )
      {
        *pSource = source;
        *pDest = dest;
        return(error);
      }
      if ( (dstBytes -= n) < 0 )
      {
        *pSource = source;
        *pDest = dest;
        return(error);
      }
      do
      {
        UPutByte(UGetByte());
      } while (--n > 0);
    }
    else
    if (n != minus128)
    {
      n = -n + 1;
      if ( (srcBytes -= 1) < 0 )
      {
        *pSource = source;
        *pDest = dest;
        return(error);
      }
      if ( (dstBytes -= n) < 0 )
      {
        *pSource = source;
        *pDest = dest;
        return(error);
      }
      c = UGetByte();
      do
      {
        UPutByte(c);
      } while (--n > 0);
    }
  }
  error = FALSE;    /* success! */
  *pSource = source;
  *pDest = dest;
  return(error);
}

/*----------------------------------------------------------------------*/

/* queryilbm
 *
 * Passed an initilized ILBMInfo with a not-in-use IFFHandle,
 *   and a filename,
 *   will open an ILBM, fill in ilbm->camg and ilbm->bmhd,
 *   and close the ILBM.
 *
 * This allows you to determine if the ILBM is a size and
 *   type you want to deal with.
 *
 * Returns 0 for success or an IFFERR (libraries/iffparse.h)
 */

LONG QuerySplashPic (struct ILBMInfo *ilbm,UBYTE *filename)
{
LONG error;

BitMapHeader *bmhd;

  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
    if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
    {
              if (bmhd = (BitMapHeader *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_BMHD))
              {
                 *(&ilbm->Bmhd) = *bmhd;

                 ilbm->camg = getcamg (ilbm);
              }

              else error = NOFILE;
    }

    else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG QueryMandPic (struct ILBMInfo *ilbm,struct MandelChunk **ManChk,UBYTE *filename)
{
LONG error;

BitMapHeader *bmhd;

  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
    if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
    {
       if (*ManChk = (struct MandelChunk *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_MAND))
       {
              if (bmhd = (BitMapHeader *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_BMHD))
              {
                 *(&ilbm->Bmhd) = *bmhd;

                 ilbm->camg = getcamg (ilbm);
              }

              else error = NOFILE;
       }

       else error = NOMAND;
    }

    else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG QueryMandPicNew (struct ILBMInfo *ilbm,struct NewMandelChunk **ManChk,UBYTE *filename)
{
LONG error;

BitMapHeader *bmhd;

  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     
	 error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
    if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
    {
       if (*ManChk = (struct NewMandelChunk *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_NEWMAND))
       {
              /*printf("QueryMandPicNew(): ManChk->FractalType: %d\n", ((struct NewMANDChunk*)(*ManChk)->ch_Data)->FractalType); */
			  
			  if (bmhd = (BitMapHeader *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_BMHD))
              {
                 *(&ilbm->Bmhd) = *bmhd;

                 ilbm->camg = getcamg (ilbm);
              }

              else error = NOFILE;
       }

       else error = NOMAND;
    }

    else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG QueryMandPicNewRGB (struct ILBMInfo *ilbm,struct NewRGBMandelChunk **ManChk,UBYTE *filename)
{
LONG error;

BitMapHeader *bmhd;

  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     
	 error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
    	if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
    	{
       		if (*ManChk = (struct NewRGBMandelChunk *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_NEWMAND))
       		{
              /*printf("QueryMandPicNewRGB(): ManChk->FractalType: %d\n", ((struct NewMANDChunk*)(*ManChk)->ch_Data)->FractalType); */
			  
			  if (bmhd = (BitMapHeader *) findpropdata (ilbm->ParseInfo.iff,ID_ILBM,ID_BMHD))
              {
                 *(&ilbm->Bmhd) = *bmhd;

                 ilbm->camg = getcamg (ilbm);
              }

              else error = NOFILE;
       		}

       		else error = NOMAND;
    	}

    	else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

/* LoadMandPic
 *
 * Passed a not-in-use IFFHandle, an initialized ILBMInfo, and filename,
 *   will load an ILBM into your already opened ilbm->scr, setting up
 *   ilbm->Bmhd, ilbm->camg, ilbm->colortable, and ilbm->ncolors
 *   and loading the colors into the screen's viewport
 *
 *   Note that ncolors may be more colors than you can LoadRGB4.
 *   Use MIN(ilbm->ncolors,vp->ColorMap->Count) for color count if
 *   you change the colors yourself using 1.3/2.0 functions.
 *
 * V39 - unless ilbm->IFFPFlags & IFFPF_NOCOLOR32, will do 32-bit
 *   color load under V39 and higher
 *
 * Returns 0 for success or an IFFERR (libraries/iffparse.h)
 *
 * NOTE - LoadMandPic () keeps the IFFHandle open so you can copy
 *   or examine other chunks.  You must call closeifile(iff,ilbm)
 *   to close the file and deallocate the parsed context
 *
 */

LONG LoadMandPic (struct ILBMInfo *ilbm,UBYTE *filename)
{
struct BitMap *TmpBM;

LONG error;
  /* since QueryMandPic() has already been done, we already know if it is a 24bit image */
  /* NOT TRUE: nPlanes is again 8 instead of 24 */
  /* It's only available again after openifile() */
  /* printf("LoadMandPic(): ilbm->Bmhd.nPlanes: %d\n", ilbm->Bmhd.nPlanes); */
  
  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  if (! ilbm->scr) return (CLIENT_ERROR);

  if (! (ilbm->vp)) ilbm->vp = &ilbm->scr->ViewPort;

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     /*printf("LoadMandPic(): ilbm->Bmhd.nPlanes: %d\n", ilbm->Bmhd.nPlanes);*/
	 
	 if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
        if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
        {
           if (GetBitMapAttr (ilbm->wrp->BitMap,BMA_FLAGS) & BMF_STANDARD)
           {
              if (! (error = loadbody (ilbm->ParseInfo.iff,ilbm->wrp->BitMap,&ilbm->Bmhd)));
           }

           else
           {
              if (TmpBM = AllocBitMap ((ULONG) ilbm->win->Width,(ULONG) ilbm->win->Height,(ULONG) ilbm->wrp->BitMap->Depth,BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES,NULL))
              {
                 if (! (error = loadbody (ilbm->ParseInfo.iff,TmpBM,&ilbm->Bmhd)))

                    BltBitMapRastPort (TmpBM,(LONG) ilbm->win->LeftEdge,(LONG) ilbm->win->TopEdge,ilbm->wrp,(LONG) ilbm->win->LeftEdge,(LONG) ilbm->win->TopEdge,(LONG) ilbm->win->Width,(LONG) ilbm->win->Height,0xC0);

                 FreeBitMap (TmpBM);
              }
           }

           if (! (getcolors (ilbm)))
           {
              setcolors (ilbm,ilbm->vp);

              freecolors (ilbm);
           }
        }

        else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG LoadMandPicRGB (struct ILBMInfo *ilbm,UBYTE *filename)
{
/* struct BitMap *TmpBM; */
UBYTE *s, *d;
WORD x,y;
ULONG /* xoffset8, yoffset8, */yoffset, xoffset, totoffset;
LONG b, boffset8;
UBYTE data;
UBYTE cred, cgreen, cblue;
LONG error;
LONG w,h;
UBYTE* tempd;

/* ULONG longdata; */
  
  if (! (ilbm->ParseInfo.iff)) return (CLIENT_ERROR);

  if (! ilbm->scr) return (CLIENT_ERROR);

  if (! (ilbm->vp)) ilbm->vp = &ilbm->scr->ViewPort;

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     /* use NewMANDChunk format => NewIlbmProp set in FlashMandel.c */
	 error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     /* printf("LoadMandPic(): ilbm->Bmhd.nPlanes: %d\n", ilbm->Bmhd.nPlanes); */
	 
	 if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
        if (contextis (ilbm->ParseInfo.iff,ID_ILBM,ID_FORM))
        {
           if (GetBitMapAttr (ilbm->wrp->BitMap,BMA_FLAGS) & BMF_STANDARD)
           {
              if (! (error = loadbody (ilbm->ParseInfo.iff,ilbm->wrp->BitMap,&ilbm->Bmhd)));
           }

           else
           {
		   
            /*printf("BMF_STANDARD not set => this is a truecolor BitMap\n"); */
			
			if (! (error = loadbrush (ilbm,filename)))
			{	
				SetRast(ilbm->wrp,0); 
					
				/* custom blit routine */
					
				/*d=ilbm->wrp->BitMap->Planes[0];*/
				d=GetBitMapPtr(ilbm->wrp->BitMap); 
                /*Win->RPort->BitMap); */ /* compatible with AROS / ApolloOS */
                /*d=DD_SCREENBUFFER;*/
                
				/*
				printf("start blitting: Width: %u Height: %u\n", ilbm->win->Width, ilbm->win->Height);
				printf("ilbm->wrp->BitMap->Planes[0]: %p\n", d);
			    printf("ilbm->Bmhd.nPlanes: %u\n", ilbm->Bmhd.nPlanes);
				*/
				
				w=ilbm->win->Width;
                h=ilbm->win->Height;
                			
                for (y=/*ilbm->win->Height*/h-1; y>=0; y--)
				{
					yoffset  = y*w; /*(y*ilbm->win->Width);*/
						
					for (x=/*ilbm->win->Width*/w-1 ; x>=0; x--) 
					{
						xoffset  = x;
						
						cred=cgreen=cblue=0;
						
						for (b=ilbm->Bmhd.nPlanes-1; b>0; b--)
						{
							s=ilbm->brbitmap->Planes[b];			
							totoffset=yoffset+xoffset;						
                            /*data=*(s+(yoffset+xoffset)/8);*/
                            data=*(s+((/*yoffset+xoffset*/totoffset)>>3));
						
							/*boffset8 = (yoffset+xoffset) % 8;*/
							boffset8 = 7 & totoffset;
						
							if (data & (1<<(7-boffset8)))
							{
								if (b>15) cblue |= (1<<(b-16));
								else if (b>7) cgreen |= (1<<(b-8));
								else cred |= (1<<b);
							}
                            /*
                            *(d+3*(yoffset+xoffset)+0)=cred;
							*(d+3*(yoffset+xoffset)+1)=cgreen;
							*(d+3*(yoffset+xoffset)+2)=cblue;
                            */
                            
                            
                            tempd=d+3*(yoffset+xoffset);
                            /**tempd=cred;
                            *(tempd+1)=cgreen;
                            */
                            *((UWORD*)tempd) = (cred<<8) + cgreen;
                            /**(tempd+1)=cgreen;*/
                            *(tempd+2)=cblue;
                        }
					
						
					}
				}
				/* unload brush (frees ilbm->brbitmap) */
				unloadbrush(ilbm);
				 
              } else error = NOFILE; /* printf("Error loading 24bit BitMap.\n"); */
           } 

		   /* palette: colors will be set in FlashMandel.c */
		}

        else error = NOFILE;
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG SaveMandPicPal (struct ILBMInfo *ilbm,struct Chunk *chunklist1,struct Chunk *chunklist2,UBYTE *filename)
{
struct BitMap *TmpBM;

Color32 *colortable32;

UWORD count;

ULONG modeid;

LONG error = IFFERR_NOMEM;

  modeid = GetVPModeID (ilbm->vp);

  count = ilbm->vp->ColorMap->Count;

  if (colortable32 = (Color32 *) AllocVec ((ULONG) (sizeof (Color32) * count),MEMF_CLEAR))
  {
     if (GetBitMapAttr (ilbm->wrp->BitMap,BMA_FLAGS) & BMF_STANDARD)
     {
        GetRGB32 (ilbm->vp->ColorMap,0L,(ULONG) count,(ULONG *) colortable32);

        error = saveilbm (ilbm,ilbm->wrp->BitMap,modeid,ilbm->win->Width,ilbm->win->Height,ilbm->win->Width,ilbm->win->Height,colortable32,count,32,mskNone,0,chunklist1,chunklist2,filename);
     }

     else
     {
        if (TmpBM = AllocBitMap ((ULONG) ilbm->win->Width,(ULONG) ilbm->win->Height,(ULONG) ilbm->wrp->BitMap->Depth,BMF_INTERLEAVED | BMF_CLEAR | BMF_MINPLANES,NULL))
        {
           GetRGB32 (ilbm->vp->ColorMap,0L,(ULONG) count,(ULONG *) colortable32);

           BltBitMap (ilbm->wrp->BitMap,(LONG) ilbm->win->LeftEdge,(LONG) ilbm->win->TopEdge,TmpBM,(LONG) ilbm->win->LeftEdge,(LONG) ilbm->win->TopEdge,(LONG) ilbm->win->Width,(LONG) ilbm->win->Height,0xC0,0xFF,NULL);

           error = saveilbm (ilbm,TmpBM,modeid,ilbm->win->Width,ilbm->win->Height,ilbm->win->Width,ilbm->win->Height,colortable32,count,32,mskNone,0,chunklist1,chunklist2,filename);

           FreeBitMap (TmpBM);
        }
     }

     FreeVec (colortable32);
  }

  return (error);
}

LONG SaveMandPicRGB (struct ILBMInfo *ilbm,struct Chunk *chunklist1,struct Chunk *chunklist2,UBYTE *filename)
{
	ULONG modeid;
	struct RastPort* rp=NULL;
	struct BitMap *bm;
	LONG error=0L;
	UWORD pwidth, pheight, extra, depth, pmode;
	ULONG plsize;
/*	UBYTE *tpp; */
	int 	k; /*, p, s, n;*/
	UBYTE *screenbuffer, *d;
	UWORD x,y;
	int b;
	UBYTE red, green, blue,bitmask,bitmaskw;
	ULONG offset, offset2,ioffset, yoffset;

	/* ILBM Property chunks to be grabbed
	 * List BMHD, CMAP and CAMG first so we can skip them when we write
	 * the file back out (they will be written out with separate code)
	 */
	LONG	ilbmprops[] = {
			ID_ILBM, ID_BMHD,
			ID_ILBM, ID_CMAP,
			ID_ILBM, ID_CAMG,
			ID_ILBM, ID_CCRT,
			ID_ILBM, ID_AUTH,
			ID_ILBM, ID_Copyright,
			TAG_DONE
			};

	/* ILBM Collection chunks (more than one in file) to be gathered */
	LONG	ilbmcollects[] = {
			ID_ILBM, ID_CRNG,
			TAG_DONE
			};

	/* ILBM Chunk to stop on */
	LONG	ilbmstops[] = {
			ID_ILBM, ID_BODY,
			TAG_DONE
			};
	struct ILBMInfo *MyIlbm;

    if (!(MyIlbm = (struct ILBMInfo *) AllocMem(sizeof(struct ILBMInfo),MEMF_PUBLIC|MEMF_CLEAR)))
        return IFFERR_NOMEM;
    
	modeid = GetVPModeID (ilbm->vp);
    
	MyIlbm->ParseInfo.propchks = ilbmprops;
   	MyIlbm->ParseInfo.collectchks = ilbmcollects;
   	MyIlbm->ParseInfo.stopchks = ilbmstops;
   	MyIlbm->windef = (struct NewWindow*)ilbm->win;
    
   	/* suppose the following is not necessary because IFF parsing is already done ?! */
   	/* if(!(ilbm[0]->ParseInfo.iff = AllocIFF())) bye(noiffh,RETURN_FAIL); */
   	MyIlbm->ParseInfo.iff = ilbm->ParseInfo.iff;
   
   	/* copy more infos from original ILBMInfo */
	MyIlbm->camg = modeid;
	
	/* define dimension of the picture */
	pwidth=ilbm->win->Width;
   	pheight=ilbm->win->Height;
	depth=GetBitMapAttr (ilbm->wrp->BitMap,BMA_DEPTH);
	extra = depth > 8 ? depth - 8 : 0; /* can be simplified */
	
	/* define BMHD */
	MyIlbm->Bmhd.w = pwidth;
	MyIlbm->Bmhd.h = pheight;
	MyIlbm->Bmhd.x = 0;
	MyIlbm->Bmhd.y = 0;
	MyIlbm->Bmhd.masking = mskNone;
	MyIlbm->Bmhd.compression = 0;
	MyIlbm->Bmhd.nPlanes = depth;
	
	/* not sure if these are necessary? */
   	pmode   = pwidth >= 640  ? HIRES : 0L;
   	pmode  |= pheight >= 400 ? LACE  : 0L;
    
   	plsize = RASSIZE(pwidth,pheight);
    
   	/* allocate bitmap and planes */
   	if (MyIlbm->brbitmap = AllocMem(sizeof(struct BitMap) + (extra<<2), MEMF_CLEAR))
	{
	    bm = MyIlbm->brbitmap;
   	    InitBitMap(bm,depth,pwidth,pheight);
   	    for (k=0, error=0; k<depth && (!error); k++)
   	    {
           	if(!(bm->Planes[k] = AllocRaster(pwidth,pheight)))
			error = IFFERR_NOMEM;
           	if(! error)
	    	{
           	    /*printf("BitPlane[%d] allocated\n", k);*/
				BltClear(bm->Planes[k], RASSIZE(pwidth,pheight),0);
           	}
	    }
    
    
	    if(!error)
	    {
	        if(!(rp = AllocMem(sizeof(struct RastPort),MEMF_CLEAR)))
		    error = IFFERR_NOMEM;
	        else
		    {
		        InitRastPort(rp);
		        rp->BitMap = bm;
		        rp->Mask = 0x01;	/* we'll render 1 plane at a time */
		        SetAPen(rp,1);
		        SetDrMd(rp,JAM1);
		    }
	    }
	
        if (!error) 			
		{				
			screenbuffer=ilbm->win->RPort->BitMap->Planes[0];
            /*screenbuffer=GetBitMapPtr(ilbm->wrp->BitMap); 
            /*Win->RPort->BitMap); /* compatible with AROS / ApolloOS */
			
            for (y=0; y<pheight; y++)
			{
                for (x=0; x<pwidth; x++) 
				{
					offset=(y*pwidth+x)*3;
                    red= *(screenbuffer+offset);
					green= *(screenbuffer+offset+1);
					blue= *(screenbuffer+offset+2);
					
					for (b=0; b<8; b++)
					{
						bitmask=1<<b;
						ioffset=(y*pwidth+x) & 7;
						bitmaskw=1<<(7-ioffset);
						offset2=(y*pwidth+x)>>3;
					
						if (red&bitmask) *(bm->Planes[b]+offset2) |= bitmaskw;
						if (green&bitmask) *(bm->Planes[b+8]+offset2) |= bitmaskw;
						if (blue&bitmask) *(bm->Planes[b+16]+offset2) |= bitmaskw;
					
					}			
			
				}	
			}
		}
			
		if(!error)
	    {
	        /* Code from original 24BitDemo.c */
			/* Functions that can be used to render things the system friendly way
			   Put something recognizable in the planes.
			*/
			/*
			tpp = bm->Planes[0];	// save first plane pointer
	        bm->Depth = 1;
	        for(k=0; k<depth; k++)	//swap in planeptrs 1 at a time
	    	{
	    	    bm->Planes[0] = bm->Planes[k];
	    	    Move(rp,k * 10, (k * 8) + 8);	// render rgb bitname text
	    	    Text(rp, rgbnames[k], 2);
	              	
                Move(rp, k*20, k*8+8);	                
				Draw(rp, k*20+200, k*8+108);      	            
			}
		    bm->Depth = depth;		// restoredepth
		    bm->Planes[0] = tpp;	// and firt pointer
	   		*/ 
		
	        /* Save the 24-bit ILBM */
	        /* printf("Saving %s\n",filename); */
	        
			error = saveilbm(MyIlbm, MyIlbm->brbitmap, pmode,
                    pwidth,  pheight, pwidth, pheight,      /* width = pwidth (2x) */	                    
					NULL, 0, 0,	/* colortable */
	            	/* strangely, this function doesn't take 15 arguments when used
					   for 24 bitplanes (so, no colortable23, ncolors, 32
					*/
					mskNone, 0,	/* masking, transparent */
	            	chunklist1, chunklist2, /* chunklists */
	            	filename);
					
	    }
    
		/* Free our bitmap */
	    for (k=0; k<depth; k++)
	    {
	        if (MyIlbm->brbitmap->Planes[k])
			FreeRaster(MyIlbm->brbitmap->Planes[k],pwidth,pheight);
		}
		FreeMem(MyIlbm->brbitmap, sizeof(struct BitMap) + (extra << 2));
		MyIlbm->brbitmap = NULL;
		if (rp) FreeMem(rp, sizeof(struct RastPort));
	
		/* check if more memory has to be freed */
		
	}
	
    return (error);
}


LONG SaveMandPic (struct ILBMInfo *ilbm,struct Chunk *chunklist1,struct Chunk *chunklist2,UBYTE *filename)
{
    UBYTE depth;
    depth=GetBitMapAttr (ilbm->wrp->BitMap,BMA_DEPTH);
    
    if (depth<=8) 
    {
        /* printf("SaveMandPic(): Save a classic picture with maximum 8 bitplanes\n"); */
        return SaveMandPicPal(ilbm, chunklist1, chunklist2, filename);
    }
    else
    {
        /* printf("SaveMandPic(): Save a truecolor picture with %d bitplanes\n",depth); */
        return SaveMandPicRGB(ilbm, chunklist1, chunklist2, filename);
    }
}


LONG LoadPalette (struct ILBMInfo *ilbm,UBYTE *filename, UBYTE* copy)
{
LONG error;
LONG temp;

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_READ);

  if (! error)
  {
     error = parseifile (&(ilbm->ParseInfo),ID_FORM,ID_ILBM,ilbm->ParseInfo.propchks,ilbm->ParseInfo.collectchks,ilbm->ParseInfo.stopchks);

     if ((! error) || (error == IFFERR_EOC) || (error == IFFERR_EOF))
     {
        if (! (error = getcolors (ilbm)))
        {
           setcolors (ilbm,ilbm->vp);

		   if (copy) 
		   {
				/* copy color table */
				/* printf("ncolors: %u\n", ilbm->ncolors); */
					
				for (temp=0; temp<=ilbm->ncolors; temp++)
				{
					copy[2+temp*3]=ilbm->colortable32 [temp].r & 0xff;
					copy[2+temp*3+1]=ilbm->colortable32 [temp].g & 0xff;
					copy[2+temp*3+2]=ilbm->colortable32 [temp].b & 0xff;
				}
			}
           freecolors (ilbm);
        }
     }

     closeifile (&(ilbm->ParseInfo));
  }

  return (error);
}

LONG SavePalette (struct ILBMInfo *ilbm,struct Chunk *chunklist,UBYTE *filename)
{
struct IFFHandle *iff;

struct Chunk *chunk;

Color32 *colortable32;

UWORD ncolors;

LONG size,error;

ULONG chunkID;

  iff = ilbm->ParseInfo.iff;

  ncolors = ilbm->vp->ColorMap->Count;

  error = openifile (&(ilbm->ParseInfo),filename,IFFF_WRITE);

  if (! error)
  {
    error = PushChunk (iff,ID_ILBM,ID_FORM,IFFSIZE_UNKNOWN);

    if (colortable32 = (Color32 *) AllocVec ((ULONG) (sizeof (Color32) * ncolors),MEMF_CLEAR))
    {
       GetRGB32 (ilbm->vp->ColorMap,0L,(ULONG) ncolors,(ULONG *) colortable32);

       CkErr (putcmap (iff,colortable32,ncolors,32));

       FreeVec (colortable32);
    }

    for (chunk = chunklist; chunk; chunk = chunk->ch_Next)
    {
      chunkID = chunk->ch_ID;

      if ((chunkID != ID_BMHD) && (chunkID != ID_CMAP) && (chunkID != ID_CAMG))
      {
        size = chunk->ch_Size == IFFSIZE_UNKNOWN ? strlen (chunk->ch_Data) : chunk->ch_Size;

        CkErr (PutCk (iff,chunkID,size,chunk->ch_Data));
      }
    }

    CkErr (PopChunk (iff)); /* close out the FORM */

    closeifile (&(ilbm->ParseInfo));    /* and the file */
  }

  return (error);
}
