
#include "drawing.h"
#include "enum.h"

/* function pointers */

void (*PutPixel)(ULONG,ULONG,ULONG);
void (*FillUncalculatedAreas)(void);
LONG (*WritePixelLineDirect)(struct RastPort*, UWORD, UWORD, UWORD, UBYTE*, struct RastPort*);
void (*RectColorFillDirect)(struct RastPort*, ULONG, ULONG, ULONG, ULONG, ULONG);
ULONG (*GetPixelPen)(ULONG x, ULONG y);

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
    UBYTE* dest=DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;	
	*(dest+DD_RED)= (UBYTE)color;
	*(dest+DD_GREEN)= (UBYTE)color;
	*(dest+DD_BLUE)= (UBYTE)color;	
}

void PutPixelRGB(ULONG x, ULONG y, UBYTE red, UBYTE green, UBYTE blue)
{
    UBYTE* dest=DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;	
    *(dest+DD_RED)= (UBYTE)red;
	*(dest+DD_GREEN)= (UBYTE)green;
	*(dest+DD_BLUE)= (UBYTE)blue;	
}

UBYTE FindPenByRGB(UBYTE red, UBYTE green, UBYTE blue) 
{
	UBYTE i=255;
	UBYTE found=0;
	
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

ULONG GetPixelRGB24(ULONG x, ULONG y)
{
	ULONG red, green, blue;
    UBYTE *src;
    
	src	  = (UBYTE*)DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
    
	red	  = *((UBYTE*)src+DD_RED);
	green = *((UBYTE*)src+DD_GREEN);
	blue  = *((UBYTE*)src+DD_BLUE);
	
    return (ULONG)((red<<16) | (green<<8) | blue);
}

ULONG GetPixelRGB24Pen(ULONG x, ULONG y)
{
	ULONG red, green, blue;
	ULONG pen;
	UBYTE *src;
    
	src	  =	(UBYTE*)DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
    
	red	  =	*(src+DD_RED);
	green =	*(src+DD_GREEN);
	blue  =	*(src+DD_BLUE);

	pen=FindPenByRGB(red,green,blue);
	
	return pen;
}

ULONG GetPixelCLUT8Pen(ULONG x, ULONG y)
{
	return *(DD_SCREENBUFFER+y*DD_WIDTH+x);
}

void PutPixelCLUT2RGB(ULONG x, ULONG y, ULONG color)
{
	UBYTE *dest;
    ULONG index;
	UBYTE red, green, blue;
	
	dest	=	DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
	
	index	=	color*3+1;
    
	red		=	COLORS_AGA[index];
	green	=	COLORS_AGA[index+1];
	blue	=	COLORS_AGA[index+2];
	
	*(dest+DD_RED)=red;
	*(dest+DD_GREEN)=green;
	*(dest+DD_BLUE)=blue;
}

void PutPixelARGB(ULONG x, ULONG y, ULONG color)
{
	UBYTE *dest;
	UBYTE red, green, blue;
	
	dest	=	DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
	
	red		=	(color & 0x00ff0000) >> 16;
	green	=	(color & 0x0000ff00) >> 8;
	blue	=	(color & 0x000000ff);
	
	*(dest+DD_RED)=red;
	*(dest+DD_GREEN)=green;
	*(dest+DD_BLUE)=blue;
}

ULONG GetPixelCLUT8(ULONG x, ULONG y, ULONG color)
{
	UBYTE *src=DD_SCREENBUFFER+(y*DD_WIDTH+x);	
    
    return (ULONG)*(src);
}

ULONG GetPixelPenGeneric(struct RastPort* rp, ULONG x, ULONG y)
{
	switch (drawing)
	{
		case DRAW_DIRECT : 
			return GetPixelPen(x,y);
            break;
					
		default :
			return ReadPixel(rp,x,y);	
	}
}

void PutPixelCLUT8(ULONG x, ULONG y, ULONG color)
{
	UBYTE *dest=DD_SCREENBUFFER+(y*DD_WIDTH+x);
    
	*dest=(UBYTE)color;
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
        /* for convenience: give user the possibility to interrupt */
    	if (InterruptDrawing(MYILBM.win,0,0,resx,resy)) p=resx*resy;
    }
}

void FillUncalculatedAreasClassicPen(void) 
{
	BOOL stop=FALSE;
	ULONG x,y, c;
	c=0xffffffff;
	for (y=0; ((y<resy) && (!stop)); y++)
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
        /* for convenience: give user the possibility to interrupt */
        /* especially important with systemfriendly drawing / on UAE */
    	if (InterruptDrawing(MYILBM.win,0,0,resx,resy)) stop=TRUE;	
    }
}

void FillUncalculatedAreasRGB(void) {
	
    /* fill optimization */
	UBYTE red, green, blue;
	ULONG p;
	BOOL stop=FALSE;

	for(p=0; ((p<resx*resy-1) && (!stop)); ++p) 
	{
        if(Done[p] & Loaded) 
		{
			red=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_RED);
			green=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_GREEN);
			blue=*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_BLUE);
			
			while (!(Done[p+1] & Loaded))
			{
				p+=1;
					
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_RED)=red;
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_GREEN)=green;
				*((UBYTE*)DD_SCREENBUFFER+p*DD_BPP+DD_BLUE)=blue;
				/* Done[p] |= Loaded; */
			}
			
		}   
		/* for convenience: give user the possibility to interrupt */
    	if (InterruptDrawing(MYILBM.win,0,0,resx,resy)) stop=TRUE;	
    }
}

LONG WritePixelLineCLUT2RGB(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
	UBYTE *dest;
	ULONG index;
	UWORD i;
    
	dest=DD_SCREENBUFFER+(y*DD_WIDTH+x)*DD_BPP;
	
    for (i=0; i<w; i++) 
	{
		index=a[i]*3+1;
		
		*(dest+i*DD_BPP+DD_RED)=COLORS_AGA[index]; 	/* red; */
		*(dest+i*DD_BPP+DD_GREEN)=COLORS_AGA[index+1]; /* green; */
		*(dest+i*DD_BPP+DD_BLUE)=COLORS_AGA[index+2];  /* blue; */
	}

	return (LONG)i;
}

LONG WritePixelLineCLUT8(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
	UBYTE *dest;
	UWORD i;
	
    dest = DD_SCREENBUFFER+(y*DD_WIDTH+x);

	for (i=0; i<w; i++) 
	{
		*(dest+i)=a[i];
	}

	return (LONG)i;
}

LONG WritePixelLine(struct RastPort* rp, UWORD x, UWORD y, UWORD w, UBYTE* a, struct RastPort* trp)
{
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
 	UBYTE *dest;

	for (y=y1; y<=y2; y++)
	{
		dest=DD_SCREENBUFFER+y*DD_WIDTH;		
		for (x=0; x<=x2-x1; x++)
		{
			*(dest+x1+x)=color;				
		}	
	}
}

void RectColorFillDirectCLUT2RGB(struct RastPort* rp, ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color)
{
	ULONG red,green,blue;
	ULONG x,y;
 	UBYTE *dest;
	ULONG index;
	
	index	=	color*3+1;

	red	  =	COLORS_AGA[index];
	green =	COLORS_AGA[index+1];
	blue  =	COLORS_AGA[index+2];
		
	for (y=y1; y<=y2; y++)
	{
		dest=DD_SCREENBUFFER+y*DD_WIDTH*DD_BPP;		
		for (x=0; x<=x2-x1; x++)
		{
			*(dest+(x1+x)*DD_BPP+DD_RED)=red;
			*(dest+(x1+x)*DD_BPP+DD_GREEN)=green;
			*(dest+(x1+x)*DD_BPP+DD_BLUE)=blue;
		}	
	}
}

void RectColorFill(struct RastPort* rp, ULONG x1, ULONG y1, ULONG x2, ULONG y2, ULONG color)
{
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

BOOL PasteBitMapCLUT2RGB (struct BitMap *SrcBM,struct Window *DstWin,WORD SrcWidth,WORD SrcHeight)
{
  BOOL Success = FALSE;
  UWORD DstWinWidth,DstWinHeight;
  struct BitMap *TmpBM;
  UWORD x, y, c;
  double fx, fy;
  UWORD sx, sy;
  UBYTE *srcptr;

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
		fx=(double)DstWinWidth / (double)SrcWidth;
		fy=(double)DstWinHeight / (double)SrcHeight;
				
		/* after the copy just convert the colors */
	
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
					
					c=*(srcptr+(y*SrcWidth+x));	
					
					/* Scaling doesn't work */
					
					sx=(ULONG)(x*fx);
					sy=(ULONG)(y*fy);
					
					RectColorFillDirectCLUT2RGB(MYILBM.win->RPort,
						sx,sy,
						(ULONG)((x+1)*fx),(ULONG)((y+1)*fy),
						c);
					
				}		
				if (InterruptDrawing(DstWin,0,0,DstWinWidth,DstWinHeight)) return Success;
            }
		} 
		else 
		{
			BltBitMapRastPort (TmpBM,0,0,DstWin->RPort,0,0,DstWinWidth,DstWinHeight,0xC0);
		}

	    FreeBitMapSafety (TmpBM);

        Success = TRUE;
     }

     FreeBitMapSafety (SrcBM);

     MASK ^= BMASK;
  }

  return Success;
}


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

  if (SrcBM)
  {
     DstWinWidth = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZWidth : DstWin->Width);

     DstWinHeight = ((DstWin->Flags & WFLG_GIMMEZEROZERO) ? DstWin->GZZHeight : DstWin->Height);

     TmpBM = AllocBitMap ((ULONG) DstWinWidth,(ULONG) DstWinHeight,(ULONG) DstWin->RPort->BitMap->Depth,BMF_INTERLEAVED | BMF_MINPLANES | BMF_CLEAR,DstWin->RPort->BitMap);

     if (TmpBM)
     {
		fx=(double)DstWinWidth / (double)SrcWidth;
		fy=(double)DstWinHeight / (double)SrcHeight;
				
		/* after the copy just convert the colors */

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
					
				red=*(srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_RED));	
				green=*(srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_GREEN));	
				blue=*(srcptr+((y*SrcWidth+x)*DD_SRC_BPP+DD_SRC_BLUE));	
				
				c = FindPenByRGB(red, green, blue);
					
				/* Scaling doesn't work */
					
				sx=(ULONG)(x*fx);
				sy=(ULONG)(y*fy);
					
				RectColorFillDirectCLUT8(MYILBM.win->RPort,
					sx,sy,
					(ULONG)((x+1)*fx),(ULONG)((y+1)*fy),
					c);
			}		
			if (InterruptDrawing(DstWin,0,0,DstWinWidth,DstWinHeight)) return Success;			
        }

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
		 	case 3 : PutPixel=PutPixelCLUT2RGB; 
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

			case 1 : PutPixel=PutPixelCLUT8; 
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
		PutPixel=PutPixelClassicPen;
		GetPixelPen=GetPixelClassicPen;
		FillUncalculatedAreas=FillUncalculatedAreasClassicPen;	
	}
}
