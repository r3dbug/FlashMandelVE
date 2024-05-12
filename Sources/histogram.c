
/**************************************************************************************
 *                                                                                    *
 * Histograms can be saved as a raw file or compressed with rle algorithm.            *
 * To avoid any problems in the future, the compressed file conains a header of       *
 * two longwords that identify the compressed data and the algorithm used.            *
 *                                                                                    *
 **************************************************************************************
 */
    
#include "histogram.h"

UBYTE extended_filename[MAX_LEN_FILENAME];
ULONG ID_CHST = 0x43485354;  					/* string "CHST" */
ULONG ID_ALG = HISTALG_NONE;

UBYTE* AddFilenameExtension(UBYTE* original)
{
	strcpy(extended_filename, original);
	strcat(extended_filename, ".hst");
	return extended_filename;
}

/*
void PrintHistData(ULONG* data)
{
	ULONG p, n;

	p=n=0;
	while ((p<100000) && (n<20))
	{
		if (data[p])
		{
			printf("%u: data[%u] = %u\n", n, p, data[p]);
			n++;
		}
		p++;
	}
}
*/

ULONG SaveHistogram(UBYTE* filename, ULONG* data, ULONG size, ULONG algorithm)
{
	FILE* fh;
	ULONG t, error=HISTERR_NOERROR, cerror=FALSE;
	UBYTE *cbufferh, *cbuffer/*=NULL*/;
	ULONG compressed_size, tot_size;
	
	/* printf("SaveHistogramRAW(): data: %p size: %u algorithm: %u\n", data, size, algorithm); */

	/* PrintHistData(data); */
	
	if (fh=fopen(filename, "w"))
	{ 
		if (algorithm)
		{
			if (algorithm==HISTALG_RLE)
			{
				/* According to documentation, cbuffer needs 0.4% + 1 BYTE more space.
			   	   => Give it some 10% to be sure */
				cbufferh = AllocVec((ULONG)(sizeof(UBYTE) * size * 1.1), MEMF_FAST);
				
				if (cbufferh)
				{
					/* printf("Start RLE compression ...\n"); */
					
					/* write header and set start of cbuffer */
					*((ULONG*)cbufferh) 	= ID_CHST;
					*((ULONG*)cbufferh+1) 	= algorithm;  /* for the moment, only HISTALG_RLE is supported */
					cbuffer					= cbufferh + sizeof(ULONG)*HIST_HEADER_SIZE; 
			
					compressed_size = RLE_Compress((UBYTE*)data, cbuffer, size);
					tot_size=compressed_size + (HIST_HEADER_SIZE*sizeof(ULONG));
					
					/* printf("Compression finished: original size: %u compressed size: %u (rate: %3.2f%)\n", size, tot_size, 100.0/(double)size*(double)compressed_size); */
			
					t=fwrite(cbufferh,1,tot_size,fh);
		
					if (t!=tot_size) error=HISTERR_FILEWRITING;
						
					FreeVec(cbufferh);
				}
				else 
				{
					cerror=TRUE;
					/* printf("Memory allocation for cbuffer failed.\n"); */
				}
				/* end test */
			} else cerror=HISTERR_INVALIDALG;
			
		}
		
		/* fall back to uncompressed / raw format if compression failed */
		if ((!algorithm) || (cerror))
		{
			/* if (cerror) printf("Compression has failed => save uncompressed data\n"); */
			t=fwrite(data,4,size>>2,fh);
	
			if (t!=size>>2) error=HISTERR_FILEWRITING;
		}
			
		fclose(fh);
	
	} else error=HISTERR_FILEOPEN;
	
	return error;
}

ULONG LoadHistogram(UBYTE* filename, ULONG* data, ULONG size)
{
	FILE *fh;
	ULONG t, tt, t4, error=HISTERR_NOERROR;
	UBYTE *cbufferh/*=NULL*/, *cbuffer/*=NULL*/;
	ULONG header_id, algorithm_id;
	ULONG compressed_size;
	
	/* printf("SaveHistogramRAW(): data: %p size: %u\n", data, size); */

	if (fh=fopen(filename, "r"))
	{
		fseek(fh,0,2);
		t=ftell(fh);
		rewind(fh);
		
		if (t>0)
		{
			
			if (t==size)
			{
				/* this is (probably) an uncompressed histogram => read it as is */
				/* printf("Load uncompressed histogram ...\n"); */
				
				t4=fread(data,4,size>>2,fh);
				if (t4==size>>2)
				{
					/* check header marker to be sure that this is really an uncompressed histogram */	
					if (data[0]!=ID_CHST) error=HISTERR_NOERROR;
					else {
						/* if compressed marker is present: set internal error
						   => rewind file so that it can be reread rom beginning to cbuffer
						*/
						rewind(fh);
						error=HISTERR_SAMESIZE;
					}
					
					/* PrintHistData(data); */
					
				} else error=HISTERR_READERROR;
				
			} 
			
			if ((t!=size) || (error==HISTERR_SAMESIZE))
			{
				/* this is a compressed histogram => uncompress it */
				/* printf("Load compressed histogram ...\n"); */
			
				/* allocate buffer memory and load data into buffer */
				cbufferh=AllocVec(t, MEMF_FAST);
				tt=fread(cbufferh,1,t,fh);
				if (t==tt)
				{
					header_id 		= *((ULONG*)cbufferh);
					algorithm_id 	= *((ULONG*)cbufferh+1);
					cbuffer			= cbufferh + HIST_HEADER_SIZE * sizeof(ULONG);
					compressed_size = t - (HIST_HEADER_SIZE * sizeof(ULONG));
					
					if ((header_id==ID_CHST) && (algorithm_id==HISTALG_RLE))
					{
						/* uncompress it */
						RLE_Uncompress(cbuffer, (UBYTE*)data, compressed_size);
					
						/* RLE_Uncompress has return type void ... so no possibility to check if uncompress worked ... */
						/* (Means: either it worked - or not ... ;) */
					
						error=HISTERR_NOERROR;
					
					} else error=HISTERR_INVALIDALG;
					
				} else error=HISTERR_WRONGSIZE;
				
				FreeVec(cbufferh);
			}
			
		} else error=HISTERR_FILEEMPTY;
	
		fclose(fh);
		
	} else error=HISTERR_FILEOPEN;
	
	return error;
}