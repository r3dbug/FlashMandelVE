/*  
 * parse.c - iffparse file IO support module
 *   based on some of looki.c by Leo Schwab
 *
 * The filename for clipboard is -c or -cUnit as in -c0 -c1 etc. (default 0)
 */

#include <exec/types.h>

#include "iffp/iff.h"

/* local function prototypes */

LONG stdio_stream(struct Hook *, struct IFFHandle *, struct IFFStreamCmd *);

UBYTE *omodes[2] = {"r","w"};


/* openifile
 *
 * Passed a ParseInfo structure with a not-in-use IFFHandle, filename
 *   ("-c" or -cUnit like "-c1" for clipboard), and IFF open mode
 *   (IFFF_READ or IFFF_WRITE) opens file or clipboard for use with
 *   iffparse.library support modules.
 *
 * Returns 0 for success or an IFFERR (libraries/iffparse.h)
 */

LONG openifile(struct ParseInfo *pi, UBYTE *filename, ULONG iffopenmode)
{
        struct IFFHandle        *iff;
        BOOL    cboard;
        ULONG   unit = PRIMARY_CLIP;
        LONG    error;

        if(!pi)                 return(CLIENT_ERROR);
        if(!(iff=pi->iff))      return(CLIENT_ERROR);

        cboard = (*filename == '-'  &&  filename[1] == 'c');
        if(cboard && filename[2])       unit = atoi(&filename[2]);

        if (cboard)
                {
                /*
                 * Set up IFFHandle for Clipboard I/O.
                 */
                pi->clipboard = TRUE;
                if (!(iff->iff_Stream =
                                (ULONG)OpenClipboard(unit)))
                        {
                        message("Clipboard open of unit %ld failed.\n",unit);
                        return(NOFILE);
                        }
                InitIFFasClip(iff);
                }
        else
                {
                pi->clipboard = FALSE;
                /*
                 * Set up IFFHandle for buffered stdio I/O.
                 */
                if (!(iff->iff_Stream = (ULONG)
                   fopen(filename, omodes[iffopenmode & 1])))
                        {
                        message("%s: File open failed.\n",filename);
                        return(NOFILE);
                        }
                else initiffasstdio(iff);
                }

        D(bug("%s file opened: \n", cboard ? "[Clipboard]" : filename));

        pi->filename = filename;

        error=OpenIFF(iff, iffopenmode);

        pi->opened = error ? FALSE : TRUE;      /* currently open handle */

        D(bug("OpenIFF error = %ld\n",error));
        return(error);
}
