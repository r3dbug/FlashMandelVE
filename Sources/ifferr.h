/*
 * IFFerr
 *
 * Returns pointer to IFF Error string or NULL (no error)
 */
UBYTE *IFFerr(error)
LONG    error;
{
        /*
         * English error messages for possible IFFERR_#? returns from various
         * IFF routines.  To get the index into this array, take your IFFERR
         * code, negate it, and subtract one.
         *  idx = -error - 1;
         */
        static UBYTE    *errormsgs[] = {
                "End of file (not an error).",
                "End of context (not an error).",
                "No lexical scope.",
                "Insufficient memory.",
                "Stream read error.",
                "Stream write error.",
                "Stream seek error.",
                "File is corrupt.",
                "IFF syntax error.",
                "Not an IFF file.",
                "Required hook vector missing.",
                "Return to client."
        };
        static UBYTE unknown[32];
        static UBYTE client[] = "Client error";
        static UBYTE nofile[] = "File not found or wrong type";

        if (error < 0)
                {
                return(errormsgs[(-error) - 1]);
                }
        else if(error = CLIENT_ERROR)
                {
                return(client);
                }
        else if(error = NOFILE)
                {
                return(nofile);
                }
        else if(error)
                {
                sprintf(unknown,"Unknown error %ld",error);
                return(unknown);
                }
        else return(NULL);
}
