
#include "drawing.h"
#include "enum.h"

/*
#define WMASK  0x1
#define FMASK  0x2
#define VMASK  0x4
#define SMASK  0x8
#define BMASK  0x10
#define ZMASK  0x20
#define TMASK  0x40
#define MMASK  0x80
#define LMASK  0x100
*/

/*#define FLASHMANDEL_RB /* see proto/graphics.h line 7 */

/* function pointers */

void (*PutPixel)(ULONG,ULONG,ULONG);
void (*FillUncalculatedAreas)(void);
LONG (*WritePixelLineDirect)(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
void (*RectColorFillDirect)(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);
ULONG (*GetPixelPen)(ULONG x, ULONG y);
/*LONG (*WritePixelLine)(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
*/

/* extern global variables */

extern ULONG COLORS_AGA[];
extern struct Library* IntuitionBase;
extern UBYTE drawing;
extern ULONG resx, resy;
extern UBYTE *Data;
extern UBYTE *Done;
extern ULONG *Queue;
extern struct BitScaleArgs BSA;
extern ULONG MASK;
extern struct ILBMInfo MYILBM;

/* global variables */

extern ULONG DD_WIDTH, DD_HEIGHT;
extern UBYTE* DD_SCREENBUFFER;
extern ULONG DD_PIXFMT, DD_BPP, DD_OLDBPP;
extern ULONG DD_SRC_BPP, DD_SRC_PIXFMT, DD_DST_BPP, DD_DST_PIXFMT;
extern ULONG DD_SRC_RED, DD_SRC_GREEN, DD_SRC_BLUE;
extern ULONG DD_ARGB, DD_RED, DD_GREEN, DD_BLUE, DD_ALPHA;
extern ULONG DD_BKGRD; 
extern struct RastPort* DD_RPORT;
extern struct Window* DD_WIN;
extern UBYTE drawing;

ULONG InterpolateCLUT(ULONG color1, ULONG color2, double factor)
{
	UBYTE red1, green1, blue1;
	UBYTE red2, green2, blue2;
	UBYTE red, green, blue;
	ULONG color;
	
	red1   = COLORS_AGA[color1*3+1];
	green1 = COLORS_AGA[color1*3+2];
	blue1  = COLORS_AGA[color1*3+3];
			
	red2   = COLORS_AGA[color2*3+1];
	green2 = COLORS_AGA[color2*3+2];
	blue2  = COLORS_AGA[color2*3+3];
			
	red = red1 + factor * (double)((double)red2 - (double)red1);
	green = green1 + factor * (double)((double)green2 - (double)green1);
	blue = blue1 + factor * (double)((double)blue2 - (double)blue1);
	
	color = (red << 16) | (green << 8) | (blue);

	return color;
}


void CopyAGAPalette2RGB(void)
{
	UBYTE c;
	ULONG index;	
	for (c=0; c<=255; c++) 
	{
		index=c*3+1;
		if (DD_WIN) SetRGB32(ViewPortAddress(DD_WIN), c, (ULONG)(COLORS_AGA[index])<<24,(ULONG)(COLORS_AGA[index+1])<<24,(ULONG)(COLORS_AGA[index+2])<<24);	
	}
}

void PutPixelClassicPen(ULONG x, ULONG y, ULONG pen) 
{
	SetAPen(DD_RPORT,pen);
	WritePixel(DD_RPORT,x,y);
}

void PutPixelPenGray2RGB(ULONG x, ULONG y, ULONG color)
{
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_RED)= (UBYTE)color;
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_GREEN)= (UBYTE)color;
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_BLUE)= (UBYTE)color;	
}

void PutPixelRGB(ULONG x, ULONG y, UBYTE red, UBYTE green, UBYTE blue)
{
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_RED)= (UBYTE)red;
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_GREEN)= (UBYTE)green;
	*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_BLUE)= (UBYTE)blue;	
}

UBYTE FindPenByRGB(UBYTE red, UBYTE green, UBYTE blue) 
{
	UBYTE i=255;
	UBYTE found=0;
	
	/* printf("FindPenByRGB(): rgb=(%d,%d,%d)\n", red, green, blue); */
	
	while ((i) && (!found))
	{
		if (
			(red==(UBYTE)COLORS_AGA[i*3+1]) &&
			(green==(UBYTE)COLORS_AGA[i*3+2]) &&
			(blue==(UBYTE)COLORS_AGA[i*3+3])
		   )
		   found=i;
		i--;
	}
	return found;
}

ULONG GetPixelClassicPen(ULONG x, ULONG y) 
{
	return ReadPixel(DD_RPORT,x,y);
}

ULONG GetPixelRGB24Pen(ULONG x, ULONG y)
{
	ULONG red, green, blue;
	ULONG pen;
	
	/* printf("GetPixelRGB24Pen()\n"); */
	
	red=*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_RED);
	green=*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_GREEN);
	blue=*(DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP+DD_BLUE);

	pen=FindPenByRGB(red,green,blue);
	/* printf("End of GetPixelRGB24Pen() pen: %d\n", pen); */
	
	return pen;
}

ULONG GetPixelCLUT8Pen(ULONG x, ULONG y)
{
	/*printf("GetPixelCLUT8Pen():�color:�%d\n", *(DD_SCREENBUFFER+y*DD_WIDTH+x) );*/
 
	return *(DD_SCREENBUFFER+y*DD_WIDTH+x);
}

void PutPixelCLUT2RGB(ULONG x, ULONG y, ULONG color)
{
	ULONG offset,index;
	UBYTE red, green, blue;
	
	offset=(y*DD_WIDTH+x)*DD_BPP;
	
	index=color*3+1;
	red=COLORS_AGA[index];
	green=COLORS_AGA[index+1];
	blue=COLORS_AGA[index+2];
	
	/*if (color==0) printf("PutPixelCLUT2RGB(%u,%u,%u): index: %u rgb: (%u,%u,%u)\n", x,y,0,
	index,
	red,green,blue);
	*/
	
	*(DD_SCREENBUFFER+offset+DD_RED)=red;
	*(DD_SCREENBUFFER+offset+DD_GREEN)=green;
	*(DD_SCREENBUFFER+offset+DD_BLUE)=blue;
	
}

void PutPixelARGB(ULONG x, ULONG y, ULONG color)
{
	ULONG offset;
	UBYTE red, green, blue;
	
	offset=(y*DD_WIDTH+x)*DD_BPP;
	
	/*index=color*3+1;*/
	red=(color & 0x00ff0000) >> 16;
	green=(color & 0x0000ff00) >> 8;
	blue=(color & 0x000000ff);
	
	/*if (color==0) printf("PutPixelCLUT2RGB(%u,%u,%u): index: %u rgb: (%u,%u,%u)\n", x,y,0,
	index,
	red,green,blue);
	*/
	
	*(DD_SCREENBUFFER+offset+DD_RED)=red;
	*(DD_SCREENBUFFER+offset+DD_GREEN)=green;
	*(DD_SCREENBUFFER+offset+DD_BLUE)=blue;
	
}

ULONG GetPixelCLUT8(ULONG x, ULONG y, ULONG color)
{
	ULONG offset;	
	offset=y*DD_WIDTH+x;	
	/*printf("GetPixelCLUT8():�color:�%d\n", (ULONG)*(DD_SCREENBUFFER+offset) );*/	
    return (ULONG)*((UBYTE*)DD_SCREENBUFFER+offset);
	
}

ULONG GetPixelPenGeneric(struct RastPort* rp, ULONG x, ULONG y)
{
	UBYTE color;
	/*printf("GetPixelPenGeneric(): x: %d y: %d ", x, y);*/
	switch (drawing)
	{
		case DRAW_DIRECT : 
			return GetPixelPen(x,y);
            break;
					
		default :
			return ReadPixel(rp,x,y);	
	}
}

/*
// not necessary any more
void PutPixelCLUT2RGB32(ULONG x, ULONG y, ULONG color)
{
	ULONG offset,index;
	UBYTE red, green, blue;
	
	offset=(y*DD_WIDTH+x)*DD_BPP;
	
	index=color*3+1;
	red=COLORS_AGA[index];
	green=COLORS_AGA[index+1];
	blue=COLORS_AGA[index+2];
	
	*(DD_SCREENBUFFER+offset)=red;
	*(DD_SCREENBUFFER+offset+1)=green;
	*(DD_SCREENBUFFER+offset+2)=blue;
	
}
*/

void PutPixelCLUT8(ULONG x, ULONG y, ULONG color)
{
	ULONG offset;
	offset=y*DD_WIDTH+x;
	*(DD_SCREENBUFFER+offset)=(BYTE)color;
}

void FillUncalculatedAreasCLUT8(void) {
/* fill optimization */
UWORD WordLoaded;
ULONG LongLoaded;
ULONG colorl;
UWORD colorw;
UBYTE colorb;
ULONG nbytes,nwords,nlong,rbytes,rwords;
ULONG p, p1;
ULONG x;
    /*
    DD_SCREENBUFFER=GetBitMapPtr(MYILBM.win->RPort->BitMap);
    resx=MYILBM.win->Width;
    resy=MYILBM.win->Height;
    */
    
	/*printf("FillUncalculatedAreasCLUT8(): DD_BPP: %d DD_ARGB: %d resx/resy:�%d/%d\n", DD_BPP, DD_ARGB,resx,resy);
	printf("Loaded:�%d Done:�%d\n", Loaded, Done);
    */
	if (DD_BPP==0) DD_BPP=1; /* quick fix */

	
	
	WordLoaded = Loaded + (Loaded<<8);
    LongLoaded = WordLoaded + (WordLoaded<<16);
  
	for(p=0; p<resx*resy-1; ++p) {
        if(Done[p] & Loaded) {
			p1=p+1;
            while (!(*((ULONG*)(Done+p1)) & LongLoaded)) p1+=4;
            while (!(*((UWORD*)(Done+p1)) & WordLoaded)) p1+=2;
            while (!(Done[p1] & Loaded)) p1++;
            nbytes=(p1-p)-1;
            if (nbytes>0) {
                nwords=nbytes>>1;
                rbytes=nbytes-(nwords<<1);
                nlong=nwords>>1;
                rwords=nwords-(nlong<<1);
                colorb=DD_SCREENBUFFER[p];
                colorw=colorb+(colorb<<8);
                colorl=colorw+(colorw<<16);
                p+=1;
                for (x=0;x<nlong;x++) {
                    *((ULONG*)(DD_SCREENBUFFER+p))=colorl;
                    *((ULONG*)(Done+p))|=LongLoaded;
                    p+=4;
                }
                for (x=0;x<rwords;x++) {
					*((UWORD*)(DD_SCREENBUFFER+p))=colorw;
                    *((UWORD*)(Done+p))|=WordLoaded;
                    p+=2;
                }
                for (x=0;x<rbytes;x++) {
                    *((UBYTE*)DD_SCREENBUFFER+p)=colorb;
                    Done[p] |= Loaded;
                    p++;
                }
            }
        }   
    }
}

void FillUncalculatedAreasClassicPen(void) 
{
	ULONG x,y, c;
	c=0xffffffff;
	for (y=0; y<resy; y++)
	{
		for (x=0;x<resx; x++)
		{
			if (Done[y*DD_WIDTH+x] & Loaded) c =GetPixelClassicPen(x,y);
			else
			{
				if (c != 0xffffffff) PutPixelClassicPen(x,y,c);	
				Done[y*DD_WIDTH+x] |= Loaded;
			}
			
		}
	}
}

void FillUncalculatedAreasRGB(void) {
/* fill optimization */
UBYTE red, green, blue;
ULONG p; /*, p1;*/
	
	printf("FillUncalculatedAreasRGB(): DD_WIDTH: %d DD_BPP: %d DD_ARGB: %d resx/y: [%d,%d]\n", 
		DD_WIDTH, DD_BPP, DD_ARGB, resx, resy);
	/*printf("Screenbuffer: %p (SAGA: %p):\n", DD_SCREENBUFFER, *((UBYTE*)0xdfe1f4));
	*/
	if (DD_BPP==0) DD_BPP=1;
	
	for(p=0; p<resx*resy-1; ++p) 
	{
        if(Done[p] & Loaded) 
		{
        	
			/*if ((p>=2550) && (p<2570))
			printf("p: %d Done[p]: %d Loaded: %d Done[p]&Loaded: %d\n",
				p, Done[p], Loaded, Done[p]&Loaded);
			*/	
			red=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_RED);
			green=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_GREEN);
			blue=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_BLUE);
			
			while (!(Done[p+1] & Loaded))
			{
				p+=1;
				/*
				if ((p>=2550) && (p<2570))
					printf("p: %d Done[p+1]: %d Loaded: %d Done[p+1]&Loaded: %d DD_SCREENBUFFER: %d DD_WIDTH: %d DD_BPP: %d\n",
						p, Done[p], Loaded, Done[p]&Loaded, DD_SCREENBUFFER, DD_WIDTH, DD_BPP);
				*/		
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_RED)=red;
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_GREEN)=green;
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_BLUE)=blue;
				/* Done[p] |= Loaded; */
			}
			
		}   
	}
}

LONG WritePixelLineCLUT2RGB(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
	UBYTE *start;
	ULONG index;
	/*ULONG red, green, blue;*/
	UWORD i;
	start=DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
	for (i=0; i<w; i++) 
	{
		index=a[i]*3+1;
		/*
		red=COLORS_AGA[index];
		green=COLORS_AGA[index+1];
		blue=COLORS_AGA[index+2];
		*/
		
		*(start+i*DD_BPP+DD_RED)=COLORS_AGA[index]; 	/* red; */
		*(start+i*DD_BPP+DD_GREEN)=COLORS_AGA[index+1]; /* green; */
		*(start+i*DD_BPP+DD_BLUE)=COLORS_AGA[index+2];  /* blue; */
	}
	return (LONG)i;
}

LONG WritePixelLineCLUT8(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
	UBYTE* start;
	UWORD i;
	start=DD_SCREENBUFFER+(y*DD_WIDTH+x);
	for (i=0; i<w; i++) 
	{
		*(start+i)=a[i];
	}
	return (LONG)i;
}

LONG WritePixelLine(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
	/* printf("WritePixelLine(): x: %d y: %d w: %d\n",
			x, y, w); */
	

	switch (drawing) 
	{
		case DRAW_DIRECT : 
				return WritePixelLineDirect (rp, x, y, w, a, trp);
				break;
		
		default :
			return WritePixelLine8 (rp, x, y, w, a, trp);
	}
}

void RectColorFillDirectCLUT8(struct RastPort* rp, ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color)
{
	ULONG x,y;
 	UBYTE* start;

	for (y=y1; y<=y2; y++)
	{
		start=DD_SCREENBUFFER+y*DD_WIDTH;		
		for (x=0; x<=x2-x1; x++)
		{
			*(start+x1+x)=color;				
		}	
	}
}

void RectColorFillDirectCLUT2RGB(struct RastPort* rp, ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color)
{
	ULONG red,green,blue;
	ULONG x,y;
 	UBYTE* start;
	ULONG index;
	
	index=color*3+1;
	red=COLORS_AGA[index];
	green=COLORS_AGA[index+1];
	blue=COLORS_AGA[index+2];
		
	for (y=y1; y<=y2; y++)
	{
		start=DD_SCREENBUFFER+y*DD_WIDTH*DD_BPP;		
		for (x=0; x<=x2-x1; x++)
		{
			*(start+(x1+x)*DD_BPP+DD_RED)=red;
			*(start+(x1+x)*DD_BPP+DD_GREEN)=green;
			*(start+(x1+x)*DD_BPP+DD_BLUE)=blue;
		}	
	}
}

void RectColorFill(struct RastPort* rp, ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color)
{
	/* printf("RectColorFill(): x1: %d y1: %d x2: %d y2: %d color: %d\n",
			x1, y1, x2, y2, color); */
	

	switch (drawing)
	{
		case DRAW_DIRECT : 
				RectColorFillDirect(rp,x1,y1,x2,y2,color);
				break;
		default :
			  SetAPen(rp, color);
			  RectFill(rp,x1,y1,x2,y2);
	}
}

/****************************************************************************************************/
/***************************************** BitMap functions *****************************************/
/****************************************************************************************************/

BOOL PasteBitMapStandard (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

/* printf("PasteBitMap(): 8bit => 8bit\n"); */

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

        BitMapScale (&BSA);

        BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);

        FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}

/*
BOOL PasteBitMapCLUT2CLUT (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

printf("PasteBitMap(): 8bit => 8bit\n");

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

        BitMapScale (&BSA);

        BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);

        FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}
*/

BOOL PasteBitMapCLUT2RGB (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

UWORD x, y, c;
double fx, fy;
UWORD sx, sy;
UBYTE *srcptr; /*, *dstptr;*/

/* printf("PasteBitMapCLUT2RGB(): 8bit => 24bit\n"); */


	/*
	printf("PasteBitMapCLUT2RGB(): DD_OLDBPP: %d DD_BPP: %d\n", DD_OLDBPP, DD_BPP);
  	*/
  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        /*
		BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

	
        BitMapScale (&BSA);
		*/
		
        /*BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);*/
		
		fx=(double)DstWinWidth / (double)SrcWidth;
		fy=(double)DstWinHeight / (double)SrcHeight;
				
		/* after the copy just convert the colors */
		/*
		printf("DD_BPP: %d\n", DD_BPP);
		*/
		if (DD_BPP==0) DD_BPP=1;
	
		if ((DD_OLDBPP<=1) && (DD_BPP >=3))
		{
			/*
			printf("Copy from CLUT to RGB: Src: %p [%d,%d] => %p [%d|%d,%d|%d]\n",
					MYILBM.win->RPort->BitMap->Planes[0], SrcWidth, SrcHeight,
					TmpBM->Planes[0], DstWinWidth, DD_WIDTH, DstWinHeight, DD_HEIGHT
					);
			*/
			srcptr=GetBitMapPtr(SrcBM);
			
			for (y=0;y<SrcHeight-1; y++)
			{
				for (x=0;x<SrcWidth-1; x++) 
				{
					
					c=*(/*(UBYTE*)(SrcBM->Planes[0])*/ srcptr+(y*SrcWidth+x));	
					
					/* Scaling doesn't work */
					
					sx=(ULONG)(x*fx);
					sy=(ULONG)(y*fy);
					
					RectColorFillDirectCLUT2RGB(MYILBM.win->RPort,
						sx,sy,
						(ULONG)((x+1)*fx),(ULONG)((y+1)*fy),
						c);
					
				}		
			}
		} 
		else 
		{
			BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);
		}

/*
		for (y=0;y<DstWinHeight; y++)
		{
			for (x=0;x<DstWinWidth; x++)
			{
				PutPixel(x,y,12);
			}
		}
*/		
	    FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}

/*
BOOL PasteBitMapRGB2RGB (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

	printf("PasteBitMap(): RGB => RGB\n");

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

        BitMapScale (&BSA);

        BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);

        FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}
*/

BOOL PasteBitMapRGB2CLUT (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
BOOL Success = FALSE;

UWORD DstWinWidth,DstWinHeight;

struct BitMap *TmpBM;

UWORD x, y;
ULONG c;
UBYTE red, green, blue;
double fx, fy;
UWORD sx, sy;
UBYTE* srcptr;

/*
	printf("PasteBitMap(): RGB => CLUT\n");
	printf("SRC: BPP: %d DST: BPP: %d\n", DD_SRC_BPP, DD_DST_BPP);
*/	
  
  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
        /*
		BSA.bsa_SrcWidth    = SrcWidth;
        BSA.bsa_SrcHeight   = SrcHeight;
        BSA.bsa_XSrcFactor  = SrcWidth;
        BSA.bsa_YSrcFactor  = SrcHeight;
        BSA.bsa_XDestFactor = DstWinWidth;
        BSA.bsa_YDestFactor = DstWinHeight;
        BSA.bsa_SrcBitMap   = SrcBM;
        BSA.bsa_DestBitMap  = TmpBM;

	
        BitMapScale (&BSA);
		*/
		
        /*BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);*/
		
		fx=(double)DstWinWidth / (double)SrcWidth;
		fy=(double)DstWinHeight / (double)SrcHeight;
				
		/* after the copy just convert the colors */
		/*
		printf("DD_BPP: %d\n", DD_BPP);
		*/
			/*
			printf("Copy from CLUT to RGB: Src: %p [%d,%d] => %p [%d|%d,%d|%d]\n",
					MYILBM.win->RPort->BitMap->Planes[0], SrcWidth, SrcHeight,
					TmpBM->Planes[0], DstWinWidth, DD_WIDTH, DstWinHeight, DD_HEIGHT
					);
			*/
			srcptr=GetBitMapPtr(SrcBM);
		
			for (y=0;y<SrcHeight-1; y++)
			{
				for (x=0;x<SrcWidth-1; x++) 
				{
					
					red=*(/*(UBYTE*)(SrcBM->Planes[0])*/ srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_RED));	
					green=*(/*(UBYTE*)(SrcBM->Planes[0])*/ srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_GREEN));	
					blue=*(/*(UBYTE*)(SrcBM->Planes[0])*/ srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_BLUE));	
					
					c = FindPenByRGB(red, green, blue);
					
					/* Scaling doesn't work */
					
					sx=(ULONG)(x*fx);
					sy=(ULONG)(y*fy);
					
					/*if (x==y)
					{
						printf("SRC: x: %d y: %d RGB: (%d,%d,%d) DeltaRGB: (%d,%d,%d) BPP: %d => DST: sx: %d sy: %d c: %d\n",
							x, y, red, green, blue, DD_SRC_RED, DD_SRC_GREEN, DD_SRC_BLUE, DD_SRC_BPP, sx, sy, c);
					}
					*/
					/*PutPixel(sx,sy,c);
					*/
					
					RectColorFillDirectCLUT8(MYILBM.win->RPort,
						sx,sy,
						(ULONG)((x+1)*fx),(ULONG)((y+1)*fy),
						c);
						
					/*RectColorFillDirectCLUT2RGB(MYILBM.win->RPort,
						sx,sy,
						(ULONG)((x+1)*fx),(ULONG)((y+1)*fy),
						c);
					
					*/
				}		
			}

/*
		for (y=0;y<DstWinHeight; y++)
		{
			for (x=0;x<DstWinWidth; x++)
			{
				PutPixel(x,y,12);
			}
		}
*/		
	    FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}

/****************************************************************************************************/
/****************************************************************************************************/
/****************************************************************************************************/

void SetUpPixFMTDeltas(ULONG pf)
{
	switch (pf)
	{
		case PIXFMT_RGB24  : DD_RED=0; DD_GREEN=1; DD_BLUE=2; break;
		case PIXFMT_BGR24  : DD_RED=2; DD_GREEN=1; DD_BLUE=0; break;
		case PIXFMT_ARGB32 : DD_ALPHA=0; DD_RED=1; DD_GREEN=2; DD_BLUE=3; break;
		case PIXFMT_BGRA32 : DD_ALPHA=3; DD_RED=2; DD_GREEN=1; DD_BLUE=0; break;
		case PIXFMT_RGBA32 : DD_ALPHA=3; DD_RED=0; DD_GREEN=1; DD_BLUE=2; break;
		default :
			DD_ALPHA=DD_RED=DD_GREEN=DD_BLUE=0;
	}
}

void SetUpDirectDrawing(struct Window* Win) 
{
	
	DD_WIDTH=Win->Width;
	DD_HEIGHT=Win->Height;
	
	/* this is the most system friendly and AmigaOS / ApolloOS / AROS 
	   compatible way to get screen buffer address! 
	*/
	DD_SCREENBUFFER=GetBitMapPtr(Win->RPort->BitMap);
	
	DD_OLDBPP=DD_BPP;
	DD_BPP=GetBitMapBPP(Win->RPort->BitMap);
	DD_PIXFMT=GetBitMapPixFMT(Win->RPort->BitMap);
	DD_RPORT=Win->RPort;
	DD_WIN=Win;
		
	/* set correct red, green, blue, alpha offsets for PixFMT */
	SetUpPixFMTDeltas(DD_PIXFMT);
	
	/* set function pointers for drawing routines */
	if (drawing == DRAW_DIRECT) 
	{
		switch (DD_BPP)
		{
		 	case 3 : /*printf("direct drawing: 24bit\n");*/
					 PutPixel=PutPixelCLUT2RGB; 
					 FillUncalculatedAreas=FillUncalculatedAreasRGB; 
					 WritePixelLineDirect=WritePixelLineCLUT2RGB;
					 RectColorFillDirect=RectColorFillDirectCLUT2RGB;
					 GetPixelPen=GetPixelRGB24Pen;
					 DD_BKGRD=0;
					 break;
			case 4 : PutPixel=PutPixelCLUT2RGB; 
					 FillUncalculatedAreas=FillUncalculatedAreasRGB; 
					 WritePixelLineDirect=WritePixelLineCLUT2RGB;
					 RectColorFillDirect=RectColorFillDirectCLUT2RGB;
					 GetPixelPen=GetPixelRGB24Pen;
					 DD_BKGRD=0;
					 break;
			case 1 : /*printf("Direct drawing 8bit enabled.\n");*/
            		 PutPixel=PutPixelCLUT8; 
					 FillUncalculatedAreas=FillUncalculatedAreasCLUT8;
					 WritePixelLineDirect=WritePixelLineCLUT8; 
					 RectColorFillDirect=RectColorFillDirectCLUT8;
					 GetPixelPen=GetPixelCLUT8Pen;
					 DD_BKGRD=0;
					 break;
		}
		
		
	}
	else
	{
		/* system friendly drawing */
		/*printf("System friendly drawing enabled.\n");*/
		PutPixel=PutPixelClassicPen;
		GetPixelPen=GetPixelClassicPen;
		FillUncalculatedAreas=FillUncalculatedAreasClassicPen;	
	}
/*
	printf("Win->RPort->BitMap->Planes[0]: %p\n", Win->RPort->BitMap->Planes[0]);
	printf("DD_WIDTH: %u DD_HEIGHT: %u DD_SCREENBUFFER: %p DD_BPP: %u DD_ARGB: %u\n", 
				DD_WIDTH, DD_HEIGHT, DD_SCREENBUFFER, DD_BPP, DD_ARGB);
*/
}