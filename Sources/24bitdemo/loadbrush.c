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
