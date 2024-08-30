
/**************************************************************************************
 *                                                                                    *
 * Histograms can be saved as a raw file or compressed with rle algorithm.            *
 * To avoid any problems in the future, the compressed file contains a header of       *
 * two longwords that identify the compressed data and the algorithm used.            *
 *                                                                                    *
 **************************************************************************************
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <exec/types.h>
#include <string.h>
#include <stdio.h>
#include <proto/exec.h>
#include <exec/exec.h>
#include <exec/memory.h>

#include "rle.h"

#define HISTERR_NOERROR			0
#define HISTERR_FILEOPEN 		1
#define HISTERR_FILEWRITING		2
#define HISTERR_FILEEMPTY		3
#define HISTERR_WRONGSIZE		4
#define HISTERR_READERROR		5
#define HISTERR_INVALIDALG		6
#define HISTERR_SAMESIZE		7				/* internal error: compressed image has same size as raw image */

#define MAX_LEN_FILENAME 		150

#define HIST_HEADER_SIZE		2				/* two ULONG as header */

#define HISTALG_NONE			0
#define HISTALG_RLE				1

UBYTE* AddFilenameExtension(UBYTE*);
ULONG SaveHistogram(UBYTE*, ULONG*, ULONG, ULONG);
ULONG LoadHistogram(UBYTE*, ULONG*, ULONG);

#endif /* HISTOGRAM_H */