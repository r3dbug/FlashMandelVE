
#include "menu.h"
/*
extern VOID (*V_LINE) (struct RastPort *,const LONG,const LONG,const LONG);

extern VOID (*H_LINE) (struct RastPort *,const LONG,const LONG,const LONG);
*/

struct NewScreen MyScreen = {
	0,0,1280,720,24,			/* toplr, width, height, depth */
	DETAILPEN, BLOCKPEN,
	HIRES,
	PUBLICSCREEN,
	NULL,
	"TestScreen",
	NULL,
	NULL
};
struct Screen* scrptr;

/**************************** menu 0 *******************************************************/

struct NewMenu ProgMenu[] = { NM_TITLE,"Project",0,0,0,0,

                              NM_ITEM,"About",0,NM_ITEMDISABLED,0,0,
							  NM_SUB,"Classic","A",/*CHECKIT*/0,(1<<0),0,
                              NM_SUB,"Splash!","!",/*CHECKIT*/0,(1<<1),0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Help","H",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Load picture","L",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save picture","S",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Load palette","Y",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save palette","E",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /* eliminate print and replace it by histogram */
							  /* NM_ITEM,"Print...","D",NM_ITEMDISABLED,0,0, */
							  NM_ITEM,"Histogram",0,NM_ITEMDISABLED,0,0,
							  NM_SUB,"None",0,CHECKIT,(1<<0),0,
                              NM_SUB,"Raw",0,CHECKIT,(1<<1),0,
							  NM_SUB,"RLE",0,CHECKIT,(1<<2),0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Quit","Q",NM_ITEMDISABLED,0,0,

/**************************** menu1 ****************************************************/

                              NM_TITLE,"Options",0,0,0,0,

                              /* item 0 */
							  NM_ITEM,"Title",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"Title bar","O",CHECKIT|MENUTOGGLE|CHECKED,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Last time","T",0,0,0,

							  /* item 1 */
                              NM_ITEM,"Limits...","C",NM_ITEMDISABLED,0,0,

							  /* item 2 */
                              NM_ITEM,"Iterations",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"64",0,CHECKIT,~(1<<0),0,
                              NM_SUB,"128",0,CHECKIT,~(1<<1),0,
                              NM_SUB,"192",0,CHECKIT,~(1<<2),0,
                              NM_SUB,"256",0,CHECKIT,~(1<<3),0,
                              NM_SUB,"320",0,CHECKIT|CHECKED,~(1<<4),0,
                              NM_SUB,"384",0,CHECKIT,~(1<<5),0,
                              NM_SUB,"448",0,CHECKIT,~(1<<6),0,
                              NM_SUB,"512",0,CHECKIT,~(1<<7),0,
                              NM_SUB,"1024",0,CHECKIT,~(1<<8),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Custom...",0,CHECKIT,~(1<<10),0,
							  NM_SUB,"Red Max",0,CHECKIT,~(1<<11),0,
							  NM_SUB,"Green Max",0,CHECKIT,~(1<<12),0,
							  NM_SUB,"Blue Max",0,CHECKIT,~(1<<13),0,
							
							  NM_SUB,"Red Min",0,CHECKIT,~(1<<14),0,
							  NM_SUB,"Green Min",0,CHECKIT,~(1<<15),0,
							  NM_SUB,"Blue Min",0,CHECKIT,~(1<<16),0,
							
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Minimum...",0,CHECKIT,~(1<<18),0,
							  NM_SUB,"Orbits","O",CHECKIT,~(1<<19),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
						      NM_SUB,"Bailout",0,CHECKIT,~(1<<21),0,
							
							  /* item 3 */
                              NM_ITEM,"Priority",0,0,0,0,
                              NM_SUB,"-5",0,CHECKIT,~(1<<0),0,
                              NM_SUB,"-4",0,CHECKIT,~(1<<1),0,
                              NM_SUB,"-3",0,CHECKIT,~(1<<2),0,
                              NM_SUB,"-2",0,CHECKIT,~(1<<3),0,
                              NM_SUB,"-1",0,CHECKIT,~(1<<4),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB," 0",0,CHECKIT,~(1<<6),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"+1",0,CHECKIT,~(1<<8),0,
                              NM_SUB,"+2",0,CHECKIT,~(1<<9),0,
                              NM_SUB,"+3",0,CHECKIT,~(1<<10),0,
                              NM_SUB,"+4",0,CHECKIT,~(1<<11),0,
                              NM_SUB,"+5",0,CHECKIT,~(1<<12),0,

							  /* item 4 */
                              NM_ITEM,"Fractal",0,0,0,0,
                              NM_SUB,"Julia Set",0,CHECKIT,~(1<<0),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Mandelbrot",0,CHECKIT,~(1<<2),0,
							  NM_SUB,"Multibrot",0,CHECKIT,~(1<<3),0,
							  NM_SUB,"Burning Ship",0,CHECKIT,~(1<<4),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Nebula",0,CHECKIT,~(1<<6),0,
							  NM_SUB,"Antinebula",0,CHECKIT,~(1<<7),0,
		/*					  NM_SUB,"Newton","P",CHECKIT,~(1<<4),0,*/
                   
				   			  /* item 5 */
				              /* new menu to select between Vampire and classic */
                              NM_ITEM,"Optimized",0,0,0,0,
                              NM_SUB,"Vampire","(",CHECKIT,(1<<0),0,
                              NM_SUB,"Classic",")",CHECKIT,(1<<1),0,

							  /* item 6 */
 							  /* new menu item to select between brute force, tiled and boundary trace algorithm */
							  NM_ITEM,"Algorithm",0,0,0,0,
                              NM_SUB,"Brute","1",CHECKIT,(1<<0),0,
                              NM_SUB,"Tiled","2",CHECKIT,(1<<1),0,
                           	  NM_SUB,"Boundary","3",CHECKIT,(1<<2),0,                           
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Random","4",CHECKIT,(1<<4),0,
                              NM_SUB,"Switch","5",CHECKIT,(1<<5),0,
                              NM_SUB,"Borders","6",CHECKIT,(1<<6),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Symmetry","7",CHECKIT,(1<<8),0,
                            
							  /* item 7 */
						 	  NM_ITEM,"Drawing",0,0,0,0,
                              NM_SUB,"AmigaOS",0,CHECKIT,(1<<0),0,
                              NM_SUB,"Screenbuffer",0,CHECKIT,(1<<1),0,
							  
							  /* item 8 */
							  /* menu for tests */
						      NM_ITEM,"Coloring",0,0,0,0,
                              NM_SUB,"Classic Iteration Count",0,CHECKIT,(1<<0),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Renormalized Iteration Count",0,CHECKIT,(1<<2),0,
                           	  NM_SUB,"Cyclic Sinus Coloring (FlashMandelNG)",0,CHECKIT,(1<<3),0,
/*							  NM_SUB,"another one","",CHECKIT,(1<<4),0,
						 	  NM_SUB,"yet another one","",CHECKIT,(1<<5),0,
							  NM_SUB,"and a last one","",CHECKIT,(1<<6),0,
*/							  
#ifdef TEST_MENU_ON
							  /* item 9 */
						 	  /* menu for tests */
						      NM_ITEM,"TEST",0,0,0,0,
                              NM_SUB,"Tricorn","",CHECKIT,(1<<0),0,
                              NM_SUB,"ScreenTest & all off","6",CHECKIT,(1<<1),0,
                           	  NM_SUB,"Inner Colors","",CHECKIT,(1<<2),0,
							  NM_SUB,"AGA => RGB","",CHECKIT,(1<<3),0,
						 	  NM_SUB,"Maxcounter on/off","",CHECKIT,(1<<4),0,
							  NM_SUB,"Smooth coloring","",CHECKIT,(1<<5),0,
#endif /* TEST_MENU_ON */
						 	  
/**************************** menu 2 *****************************************************/

                              NM_TITLE,"Calculate",0,0,0,0,

                              NM_ITEM,"PreView","W",NM_ITEMDISABLED,0,0,

                              /* NM_ITEM,NM_BARLABEL,0,0,0,0, */
							  NM_ITEM,"Resume",0,/*NM_ITEMDISABLED*/0,0,0,

                              NM_ITEM,"ReCalculate","R",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Undo","U",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Zoom","Z",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Stop","X",0,0,0,

/**************************** menu 3 *******************************************************/

                              NM_TITLE,"Video",0,0,0,0,

                              NM_ITEM,"Cycle",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"Forward",">",0,0,0,
                              NM_SUB,"Backward","<",0,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Delay...","-",0,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Palette...","P",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Screen mode...","V",NM_ITEMDISABLED,0,0,
/* disable font requester (crashes) */
/*
                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Font settings...","F",NM_ITEMDISABLED,0,0,
*/
                              NM_END,0,0,0,0,0 };

BOOL PrepareMainMenuWindowAndPointer(void)
{
	 if (MAINMENU = CreateMenus (ProgMenu,TAG_END))
     {
        if (MYILBM.Bmhd.nPlanes < 6)
        {
           MAX_ITERATIONS = 63;

           ItemAddress (MAINMENU,FULLMENUNUM (1,2,4))->Flags ^= CHECKED;

           ItemAddress (MAINMENU,FULLMENUNUM (1,2,0))->Flags |= CHECKED;
        }

        switch (PRIORITY)
        {
          case -5: ItemAddress (MAINMENU,FULLMENUNUM (1,3,0))->Flags |= CHECKED;

                   break;

          case -4: ItemAddress (MAINMENU,FULLMENUNUM (1,3,1))->Flags |= CHECKED;

                   break;

          case -3: ItemAddress (MAINMENU,FULLMENUNUM (1,3,2))->Flags |= CHECKED;

                   break;

          case -2: ItemAddress (MAINMENU,FULLMENUNUM (1,3,3))->Flags |= CHECKED;

                   break;

          case -1: ItemAddress (MAINMENU,FULLMENUNUM (1,3,4))->Flags |= CHECKED;

                   break;

          case  0: ItemAddress (MAINMENU,FULLMENUNUM (1,3,6))->Flags |= CHECKED;

                   break;

          case  1: ItemAddress (MAINMENU,FULLMENUNUM (1,3,8))->Flags |= CHECKED;

                   break;

          case  2: ItemAddress (MAINMENU,FULLMENUNUM (1,3,9))->Flags |= CHECKED;

                   break;

          case  3: ItemAddress (MAINMENU,FULLMENUNUM (1,3,10))->Flags |= CHECKED;

                   break;

          case  4: ItemAddress (MAINMENU,FULLMENUNUM (1,3,11))->Flags |= CHECKED;

                   break;

          case  5: ItemAddress (MAINMENU,FULLMENUNUM (1,3,12))->Flags |= CHECKED;

                   break;
        }
        SetTaskPri (FindTask (NULL),PRIORITY);

        /* set initial histogram saving / compression */        
        /*if (!histogram_save)
		{
			printf("CheckItem: 1,9,0\n");
			CheckMenuItem(1, 9, 0);
		}
		else
		{
			if (!histogram_compression) { CheckMenuItem(1, 9, 1); printf("CheckItem: 1,9,1\n"); }
			else { CheckMenuItem(1, 9, 2); printf("CheckItem: 1,9,2\n"); }
       	}
		*/
		ItemAddress(MAINMENU,FULLMENUNUM(0,9,2))->Flags |= CHECKED;  
		
		MenuResumeOff(MYILBM.win);
		
		/* check vampire menus */        
        if (optimized) ItemAddress(MAINMENU,FULLMENUNUM(1,5,0))->Flags |= CHECKED;  
        else ItemAddress(MAINMENU,FULLMENUNUM(1,5,1))->Flags |= CHECKED;       
        /* disable vampire acceleration if vampire was not detected */
        /* if (!vampire) ItemAddress (MAINMENU,FULLMENUNUM (1,5,0))->Flags ^= ITEMENABLED; */
		if (!vampire) OffMenu (MYILBM.win,FULLMENUNUM (1,5,0));

		/* check algorithm menu */
		switch (algorithm) 
        {
			case BRUTE    : ItemAddress(MAINMENU,FULLMENUNUM(1,6,0))->Flags |= CHECKED; break;
			case TILE 	  : ItemAddress(MAINMENU,FULLMENUNUM(1,6,1))->Flags |= CHECKED; break;
			case BOUNDARY : ItemAddress(MAINMENU,FULLMENUNUM(1,6,2))->Flags |= CHECKED; break;
		}
		
		/* check alogrithm_buddha menu */
		ItemAddress(MAINMENU,FULLMENUNUM(1,6,algorithm_buddha))->Flags |= CHECKED;
		
		/* check algorithm buddha symmetry */
		if (BUDDHA_USE_SYMMETRY) ItemAddress(MAINMENU,FULLMENUNUM(1,6,8))->Flags |= CHECKED;
		else ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags ^= CHECKED;
		
		/* drawing menu */
		/*if (drawing==DRAW_CLASSIC) ItemAddress(MAINMENU,FULLMENUNUM(1,7,0))->Flags |= CHECKED;  
        else ItemAddress(MAINMENU,FULLMENUNUM(1,7,1))->Flags |= CHECKED;       
       	*/
        SetDrawingMenu();
	    
		
        if (JULIA)
        {
           H_LINE = JHLine;

           V_LINE = JVLine;

           ItemAddress (MAINMENU,FULLMENUNUM (1,4,0))->Flags |= CHECKED;
        }

        else
        {
           H_LINE = MHLine;

           V_LINE = MVLine;

           ItemAddress (MAINMENU,FULLMENUNUM (1,4,1))->Flags |= CHECKED;
        }

        
     
        if (CURRENT_MAX_COLORS = MakeDisplay (&MYILBM))
        {
           if (vampire) ShowSplashRequester(MYILBM.win, 5);

		   CURRENT_MAX_COLORS -= RESERVED_PENS;

           SaveCoords (MYILBM.win,FALSE);

           MANDChunk.LeftEdge = MYILBM.win->LeftEdge;

           MANDChunk.TopEdge = MYILBM.win->TopEdge;

           MANDChunk.Width = MYILBM.win->Width;

           MANDChunk.Height = MYILBM.win->Height;

           MANDChunk.RMin = RMIN;

           MANDChunk.RMax = RMAX;

           MANDChunk.IMin = IMIN;

           MANDChunk.IMax = IMAX;

           MANDChunk.Iterations = MAX_ITERATIONS + 1L;

           MANDChunk.Special = NULL;

           SPECIAL_CHUNK.ch_Data = &MANDChunk;

		   NEW_SPECIAL_CHUNK.ch_Data = &NewMANDChunk;
		  
		   NEWRGB_SPECIAL_CHUNK.ch_Data = &NewRGBMANDChunk;
		   
           PutPointer (MYILBM.win,0,0,0,0,0,BUSY_POINTER);

		   SetRGBIterations();

		   /* SetRast(MYILBM.win->RPort,0); */
						 
		   SetUpDirectDrawing(MYILBM.win);
           /* in the future: put all menu selection inside this function */
		   SetMenuSelection();
		
		   ELAPSEDTIME = DrawFractal (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);

           PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

           SetMenuStart (MYILBM.win);

           ShowTime (MYILBM.win,"Rendering elapsed time:",ELAPSEDTIME);

        
           ModifyIDCMP (MYILBM.win,NULL);

           ClearMenuStrip (MYILBM.win);

           /*About (MYILBM.win);*/

           ResetMenuStrip (MYILBM.win,MAINMENU);

           ModifyIDCMP (MYILBM.win,IDCMP_STANDARD);

           Execute (ASSIGNCURRENTDIR,NULL,NULL);

		   return TRUE;	
    	}
    }
    else 
    {
    	 Fail (MenuError,20L);
		 return FALSE;
    }
}

/* Handle menu selection */

/* start full menus (e.g. after a calculation */
VOID SetMenuStart (struct Window *Win)
{
  OnMenu  (Win,FULLMENUNUM (0,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,5,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,6,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,7,NOSUB));

  OnMenu  (Win,FULLMENUNUM (0,9,NOSUB));   /* disable Project -> Print => this is now Histogram*/

  OnMenu  (Win,FULLMENUNUM (0,11,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,1,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (1,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (2,2,NOSUB));

  OffMenu (Win,FULLMENUNUM (2,7,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,0,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,2,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,4,NOSUB));

  OnMenu  (Win,FULLMENUNUM (3,6,NOSUB));

  if (FractalType==BUDDHA) MenuResumeOn(Win);
}

/* stop menus (e.g. before a calculation */
VOID SetMenuStop (struct Window *Win)
{
  OffMenu  (Win,FULLMENUNUM (0,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,5,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,6,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,7,NOSUB));

  OffMenu  (Win,FULLMENUNUM (0,9,NOSUB));  /* Project => Histogram */

  OffMenu  (Win,FULLMENUNUM (0,11,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,1,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (1,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (2,6,NOSUB));

  OnMenu   (Win,FULLMENUNUM (2,7,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,0,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,2,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,4,NOSUB));

  OffMenu  (Win,FULLMENUNUM (3,6,NOSUB));
	
  MenuResumeOff(Win);
}

void SwitchMenuSelection(UWORD a, UWORD b, UWORD from, UWORD to) 
{
	/* clear old selection */
	/*ItemAddress(MAINMENU,FULLMENUNUM(a,b,from))->Flags ^= CHECKED; */
	ItemAddress(MAINMENU,FULLMENUNUM(a,b,from))->Flags &= ~CHECKED; 
	/* set new selection */
	ItemAddress(MAINMENU,FULLMENUNUM(a,b,to))->Flags |= CHECKED;
}

void MenuResumeOff(struct Window* Win)
{
	OffMenu(Win, FULLMENUNUM (2,1, NOSUB));
}

void MenuResumeOn(struct Window* Win)
{
	OnMenu(Win, FULLMENUNUM (2,1, NOSUB));
}

void CheckMenuItem(UWORD a, UWORD b, UWORD c)
{
	/* menutimes may not have MENUTOGGLE active! 
	(otherwise os will toggle and this function will check the item, 
	leading to an unclear state */
	
/*	ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags |= CHECKED; */

    if ((ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags & CHECKED) == 0)
	{
		ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags |= CHECKED;
	}

}

void UncheckMenuItem(UWORD a, UWORD b, UWORD c)
{
/*	ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags ^= CHECKED; */

	if ((ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags & CHECKED) == CHECKED)
	{
		/*ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags ^= CHECKED;*/
		ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags &= ~CHECKED;
	}

}

/* not sure if this function works correctly ... */
void ClearMenuRange(UWORD x, UWORD y, UWORD z1, UWORD z2)
{
	UWORD temp;
	for (temp=z1; temp<=z2; temp++)
	{
		UncheckMenuItem(x,y,temp);
	}
}

UWORD GetBaseFractalType(UWORD ft)
{
	switch (ft)
	{
		case MANDEL : BaseFractalType=MANDEL; break;
		case MULTIINT : BaseFractalType=MULTIINT; break;
		case MULTIFLOAT : BaseFractalType=MULTIFLOAT; break;
		case BURNINGSHIP : BaseFractalType=BURNINGSHIP; break;
	}
	return BaseFractalType;
}

void Return2BaseFractal(struct Window* Win, ULONG type)
{
   /* enable / uncheck Julia Set menu */
   OnMenu(Win, FULLMENUNUM (1,4,0));
   UncheckMenuItem(1,4,0); 
   JULIA = FALSE;
	
   /* uncheck Buddha menuitems */									   
   if (FractalType==BUDDHA)
   {
   		if (ANTIBUDDHA)
   		{
   			ANTIBUDDHA=FALSE;
  			UncheckMenuItem(1,4,7); 												   
   		}
   	    UncheckMenuItem(1,4,6); 
   }
	
   /* uncheck base fractal items */
   FractalType=BaseFractalType=type;
}

void SetIterationsMenu(void)
{
	UWORD i;
	ClearMenuRange(1,2,0,7);
	ClearMenuRange(1,2,9,15);
	ClearMenuRange(1,2,17,19);
	
	i=9;
	switch (MAX_ITERATIONS)
	{
		case  63 : i=0; break;
		case 127 : i=1; break;
		case 191 : i=2; break;
		case 255 : i=3; break;
		case 319 : i=4; break;
		case 383 : i=5; break;
		case 447 : i=6; break;
		case 511 : i=7; break;
	}
	CheckMenuItem(1,2,i);


	if (DrawOrbits) CheckMenuItem(1,2,19);
	else UncheckMenuItem(1,2,19);
}

void DisableDirectDrawingMenu(struct Window* Win)
{
	OffMenu(Win, FULLMENUNUM (1,7,1));	
}

void EnableDirectDrawingMenu(struct Window* Win)
{
	OnMenu(Win, FULLMENUNUM (1,7,1));	
}

void SetDrawingMenu(void)
{
/*		if (drawing==DRAW_CLASSIC) ItemAddress(MAINMENU,FULLMENUNUM(1,7,0))->Flags |= CHECKED;  
        else ItemAddress(MAINMENU,FULLMENUNUM(1,7,1))->Flags |= CHECKED;       
*/
	ClearMenuRange(1,7,0,1);
    if (drawing==DRAW_CLASSIC) CheckMenuItem(1,7,0);
    else CheckMenuItem(1,7,1);       
	
    if (IsAClassicScreen(MYILBM.camg)) DisableDirectDrawingMenu(MYILBM.win);
    else EnableDirectDrawingMenu(MYILBM.win);    
}

void SetMenuSelection(void) 
{
	UWORD s;

	/* check selected menus */
	/* Julia Set selection */
	if (JULIA) ItemAddress(MAINMENU,FULLMENUNUM(1,4,0))->Flags |= CHECKED;

	/* Fractal type selection */
	ClearMenuRange(1,4,2,4);
	switch (BaseFractalType)
	{
		case MANDEL : s=2; break;
		case MULTIINT :
		case MULTIFLOAT : s=3; break;
		case BURNINGSHIP : s=4; break;
	}
	ItemAddress(MAINMENU,FULLMENUNUM(1,4,s))->Flags |= CHECKED;
	
	/* Nebula brot selection*/
	if (FractalType==BUDDHA)
	{
		if (ANTIBUDDHA) s=7;
		else s=6;
		ItemAddress(MAINMENU,FULLMENUNUM(1,4,s))->Flags |= CHECKED;
	}
	/*ResetMenuStrip (MYILBM.win,MAINMENU);*/

	/* algorithm selection */
	/* base fractal algorithm */
	ClearMenuRange(1,6,0,2);
	switch (algorithm)
	{
		case BRUTE : CheckMenuItem(1,6,0); break;
		case TILE : CheckMenuItem(1,6,1); break;
		case BOUNDARY : CheckMenuItem(1,6,2); break;
	}	
	
	/* nebula algorithm */
	ClearMenuRange(1,6,4,6);
	switch (algorithm_buddha)
	{
		case BUDDHA_RANDOM : CheckMenuItem(1,6,4); break;
		case BUDDHA_SWITCH : CheckMenuItem(1,6,5); break;
		case BUDDHA_BOUNDARY : CheckMenuItem(1,6,6); break;
	}	

	/* symmetry */
	if (BUDDHA_USE_SYMMETRY) CheckMenuItem(1,6,8);
	else UncheckMenuItem(1,6,8);

	/* disable color cycling for rgb */
	if (DD_BPP>1) 
	{
		OffMenu(MYILBM.win, FULLMENUNUM (3,0,0));
		OffMenu(MYILBM.win, FULLMENUNUM (3,0,1));
		OffMenu(MYILBM.win, FULLMENUNUM (3,0,3));
	}
	else 
	{
		OnMenu(MYILBM.win, FULLMENUNUM (3,0,0));
		OnMenu(MYILBM.win, FULLMENUNUM (3,0,1));
		OnMenu(MYILBM.win, FULLMENUNUM (3,0,3));
	}
	
	/* resume on or off */
	if (FractalType==BUDDHA) MenuResumeOn(MYILBM.win);
	else MenuResumeOff(MYILBM.win);

	/* histogram */
	ClearMenuRange(0,9,0,2);
	if (!histogram_save)
	{
		/* printf("CheckItem: 1,9,0\n"); */
		CheckMenuItem(0, 9, 0);
	}
	else
	{
		if (!histogram_compression) { CheckMenuItem(0, 9, 1); /* printf("CheckItem: 1,9,1\n"); */ }
		else { CheckMenuItem(0, 9, 2); /* printf("CheckItem: 1,9,2\n"); */ }
    }							

	/* smooth coloring */
	ClearMenuRange(1,8,0,3);
	switch (SMOOTH_COLORING)
	{
		case SMOOTH_RIC : CheckMenuItem(1,8,2); break;
		case SMOOTH_FMNG : CheckMenuItem(1,8,3); break;
		default :
			CheckMenuItem(1,8,0);
	}
	
	if ((DD_BPP==1) || (IsAClassicScreen(MYILBM.camg)))
	{
		OffMenu(MYILBM.win, FULLMENUNUM(1,8,2));
		OffMenu(MYILBM.win, FULLMENUNUM(1,8,3));
	} 
	else
	{
		OnMenu(MYILBM.win,FULLMENUNUM(1,8,2));
		OnMenu(MYILBM.win,FULLMENUNUM(1,8,3));
	}
	
	SetIterationsMenu();
	
	/* disable font requester (crashes) */
	/* take it completely out of the menu */
	/* OffMenu(MYILBM.win, FULLMENUNUM (3,10, NOSUB)); */

	SetDrawingMenu();
}

ULONG ProcessMenu (struct Window *Win,UWORD Code)
{
struct MenuItem *Item;
ULONG temp;
ULONG Choice = NULL;
double tempfloat;
double templong;
/*ULONG x,y; */
/*UBYTE* scrptr,*tptr;*/
ULONG tempold;

 while (Code != MENUNULL)
 {
  Item = ItemAddress (MAINMENU,(ULONG) Code);

  switch (MENUNUM (Code))
  {
        case 0:    switch (ITEMNUM (Code))
                   {
                        case 0: switch(SUBNUM(Code))
								{
									case 0 : Choice |= ABOUT_MSG; break; /* classic about message */
									case 1 : Choice |= SPLASH_MSG; break; /* new splash screen */
								}
								
                                break;

                        case 2: Choice |= SHOWGUIDE_MSG;

                                break;

                        case 4: Choice |= LOADPICTURE_MSG;

                                break;

                        case 5: Choice |= SAVEPICTURE_MSG;

                                break;

                        case 6: Choice |= LOADPALETTE_MSG;

                                break;

                        case 7: Choice |= SAVEPALETTE_MSG;

                                break;

                        /* disactivate main menu Project => Print (which has become histogram with submenus) */
						/*
						case 9: Choice |= DUMP_MSG;
								
                                break;
						*/
                        case 9:	switch (SUBNUM(Code))
								{
									/* don't save histogram */
									case 0 : histogram_save=FALSE; break;
									/* save raw histogram */
									case 1 : histogram_save=TRUE; histogram_compression=0; break;
									/* save histogram with rle compression */
									case 2 : histogram_save=TRUE; histogram_compression=1; break;
								}
								/* printf("histogram_save: %u histogram_compression: %u\n", histogram_save, histogram_compression); */
								SetMenuSelection();
								break;
							
						case 11: Choice |= EXIT_MSG;

                                break;
                   }

                   break;

        case 1:    switch (ITEMNUM (Code))
                   {
                        case 0: switch (SUBNUM (Code))
                                {
                                   case 0: Choice |= TITLE_MSG;

                                           break;

                                   case 2: Choice |= TIME_MSG;

                                           break;
                                }

                                break;

                        case 1: Choice |= COORDS_MSG;

                                break;

                        case 2: switch (SUBNUM (Code))
                                {
                                   case 0: MAX_ITERATIONS = 63;
										   SetRGBIterations();
										   
                                           break;

                                   case 1: MAX_ITERATIONS = 127;
										   SetRGBIterations();
										   
                                           break;

                                   case 2: MAX_ITERATIONS = 191;
								   		   SetRGBIterations();

                                           break;

                                   case 3: MAX_ITERATIONS = 255;
								   		   SetRGBIterations();

                                           break;

                                   case 4: MAX_ITERATIONS = 319;
								   		   SetRGBIterations();

                                           break;

                                   case 5: MAX_ITERATIONS = 383;
								   		   SetRGBIterations();

                                           break;

                                   case 6: MAX_ITERATIONS = 447;
										   SetRGBIterations();
										   
                                           break;

                                   case 7: MAX_ITERATIONS = 511;
										   SetRGBIterations();
										   
                                           break;

                                   case 8: MAX_ITERATIONS = 1023;
										   SetRGBIterations();
										   
                                           break;

                                  case 10: Choice |= ITER_MSG;

                                           break;
								
								 /* rgb max */  
								  case 11: templong = (ULONG)IntegerGad (Win,"Red Max iterations","Red Max: ","_Red", FM_REDITER);
								  		   FM_REDITER = (templong!=0xffffffff) ? templong : FM_REDITER;
										   MAX_ITERATIONS = max(FM_REDITER, MAX_ITERATIONS);
										   
                                           break;
										   
								  case 12: templong = (ULONG)IntegerGad (Win,"Green Max iterations","Green Max: ","_Green", FM_GREENITER);
								  		   FM_GREENITER = (templong!=0xffffffff) ? templong : FM_GREENITER;
										   MAX_ITERATIONS = max(FM_GREENITER, MAX_ITERATIONS);
										   
                                           break;
										   
								  case 13: templong = (ULONG)IntegerGad (Win,"Blue Max iterations","Blue Max: ","_Blue", FM_BLUEITER);
										   FM_BLUEITER = (templong!=0xffffffff) ? templong : FM_BLUEITER;
										   MAX_ITERATIONS = max(FM_BLUEITER, MAX_ITERATIONS);
										   
                                           break;
								/* rgb min */  
								  case 14: templong = (ULONG)IntegerGad (Win,"Red Min iterations","Red Min: ","_Red", FM_REDMIN);
								  		   FM_REDMIN = (templong!=0xffffffff) ? templong : FM_REDMIN;
										   
                                           break;
										   
								  case 15: templong = (ULONG)IntegerGad (Win,"Green Min iterations","Green Min: ","_Green", FM_GREENMIN);
								  		   FM_GREENMIN = (templong!=0xffffffff) ? templong : FM_GREENMIN;
										   
                                           break;
										   
								  case 16: templong = (ULONG)IntegerGad (Win,"Blue Min iterations","Blue Min: ","_Blue", FM_BLUEMIN);
										   FM_BLUEMIN = (templong!=0xffffffff) ? templong : FM_BLUEMIN;
										   
                                           break;
										   
								  /* 17 = menu bar */
								  
								  case 18 : /* Minimum (for Buddhabrot) */
								  			BUDDHA_MIN_ITERATIONS = (ULONG)FloatGad (Win,"Buddhabrot requester","Minimum Iterations for Buddhabrot rendering ","_Minimum:",(double)BUDDHA_MIN_ITERATIONS);
								   			/* printf("BUDDHA_MIN_ITERATIONS: %d\n", BUDDHA_MIN_ITERATIONS); */
								  		   break;
								  case 19 : /* Show Orbits */
								  			if (((FractalType==MANDEL) || (FractalType==MULTIINT) || (FractalType==MULTIFLOAT) || (FractalType=BURNINGSHIP))  &&
											   	(RMIN==INIT_DEF_RMIN) &&
												(RMAX==INIT_DEF_RMAX) &&
												(IMIN==INIT_DEF_IMIN) &&
												(IMAX==INIT_DEF_IMAX)
											   )
											{
												DrawOrbits=1;
											
												ScrCpySize=sizeof(UBYTE)*Win->Width*Win->Height;
												/* printf("Allocate BitMap: Width: %d Height: %d => Size: %d\n", Win->Width, Win->Height, ScrCpySize); */
												if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer);
												/* BltBitMap does not work 100% with truecolor screens */
												ScrCpyBuffer=AllocBitMap(Win->Width, Win->Height,/*DD_BPP**/8,BMF_CLEAR,NULL);
												BltBitMap(Win->RPort->BitMap,0,0, ScrCpyBuffer,0,0,Win->Width-1,Win->Height-1, 0xc0, 0xff,NULL);
												OrbitsActivated(Win);
											} 
											else
								  		    {
												/*printf("Show message that orbits can only be selected on main view.\n");*/
												DrawOrbits=0;
												OrbitsNotPossible(Win);
												
											}
										    SetMenuSelection();
										   break;
								
								  case 21 : /* bailout value */
										  templong = (ULONG)FloatGad (Win,"Bailout value requester","Bailout value ","_Bailout:",(double)BAILOUT_VALUE);
								   		  BAILOUT_VALUE = (templong!=0xffffffff) ? templong : BAILOUT_VALUE;
										   
										  break;
								}

                                break;

                        case 3: switch (SUBNUM (Code))
                                {
                                   case  0: PRIORITY = -5;

                                            break;

                                   case  1: PRIORITY = -4;

                                            break;

                                   case  2: PRIORITY = -3;

                                            break;

                                   case  3: PRIORITY = -2;

                                            break;

                                   case  4: PRIORITY = -1;

                                            break;

                                   case  6: PRIORITY =  0;

                                            break;

                                   case  8: PRIORITY =  1;

                                            break;

                                   case  9: PRIORITY =  2;

                                            break;

                                   case 10: PRIORITY =  3;

                                            break;

                                   case 11: PRIORITY =  4;

                                            break;

                                   case 12: PRIORITY =  5;

                                            break;

                                }

                                Forbid ();

                                THISTASK = FindTask (NULL);

                                Permit ();

                                SetTaskPri (THISTASK,PRIORITY);

                                break;

                        case 4: /* selection of fractal type: Julia / Mandel (&Multi) */
								if (FractalType==BUDDHA)
								{
									/* restore normal palette after BUDDHA */
									LoadRGB32 (ViewPortAddress(Win),PALETTE);
								}
								
								switch (SUBNUM (Code))
                                {
                                   case 0: /* Julia selected */
								           if (!JULIA)
										   {
                                              PickJuliaK (Win);
											  JULIA = TRUE;
											  FractalType=BaseFractalType;
											  ANTIBUDDHA=FALSE;
											  
											  /*SetRast(Win->RPort,DD_BKGRD);*/
											  SAFP();
											  
									          SetWindowTitles (Win,(TEXT *) ~0,"Julia constant selected");

                                              Choice |= REDRAW_MSG;

                                              DisplayBeep (Win->WScreen);

                                              Delay (ONESEC);
										   }
										   else
										   {
										   		/* if JULIA is active => switch back to base fractal */
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,0); 
												
												SaveCoords (Win,TRUE); /* what does this do? */
										   
										   		/*SetRast(Win->RPort,DD_BKGRD);*/
										   		SAFP();
												
										   		Choice = REDRAW_MSG;
										   }

                                           break;

                                   case 1: /* empty bar */
								           break;
								   case 2: /* Mandelbrot */
								   		   
										   /*printf("Select Mandel ... \n");*/
										   
										   Return2BaseFractal(Win, MANDEL);
										   
										   /*
										   if (JULIA)
										   {
										   		/* enable / uncheck Julia Set menu */
										   /*		OnMenu(Win, FULLMENUNUM (1,4,0));
										   		UncheckMenuItem(1,4,0); 
										   
										   		JULIA = FALSE;
										   }
										   
										   if (FractalType==BUDDHA)
										   {
										   		if (ANTIBUDDHA)
										   		{
										   			ANTIBUDDHA=FALSE;
										   			UncheckMenuItem(1,4,7); 												   
										   		}
										   	    UncheckMenuItem(1,4,6); 
										   }
										   
										   FractalType=MANDEL;
										   FractalType=BaseFractalType=MANDEL;
									*/
												 
										   SaveCoords (Win,TRUE); /* what does this do? */
										   
										   
										   /*SetRast(Win->RPort,DD_BKGRD);*/
										   SAFP();
											
										   SetDefaultCoordinates();

										   EnableSymmetry();
										   
										   Choice = REDRAW_MSG;
											
                                           break;
								   case 3: /* Multibrot => possible to select Mandelbrot via value 2 */
										   /*JULIA = FALSE;
								   		   ANTIBUDDHA=FALSE;
										   
										   /* enable / uncheck Julia Set menu */
										   /*OnMenu(Win, FULLMENUNUM (1,4,0));
										   UncheckMenuItem(1,4,0); 
										   */
										   
										   tempfloat = FloatGad (Win,"Multibrot requester","Insert (p)ower: zn1 = zn^p + c","_Power (2=Mandelbrot)",MultiPowerFloat);
								   		   temp=(ULONG)tempfloat;
										   /* printf("tempfloat: %f temp: %d\n", tempfloat, temp); */
										   if (temp!=0xffffffff)
										   {
												if ((double)temp != tempfloat) 
												{
													if (tempfloat<1) {
														EasyRequest(MYILBM.win, &NotYetImplemented, NULL);
													}
													else
													{
														/* printf("FractalType: %d (MULTIFLOAT)\n", FractalType); */
														/*BaseFractalType= FractalType=MULTIFLOAT;
														*/
														Return2BaseFractal(Win, MULTIFLOAT);
														MultiPowerFloat = tempfloat;
													}
												}
												else 
												{
													if (temp==2) 
													{
														/* printf("FractalType: %d (MANDEL)\n", FractalType); */
														/*FractalType=BaseFractalType=MANDEL;*/
														Return2BaseFractal(Win, MANDEL);
														MultiPower=temp;
														MultiPowerFloat=tempfloat;
													}
													else 
													{
														/* printf("FractalType: %d (MULTIINT)\n", FractalType); */
														/*BaseFractalType=FractalType = MULTIINT; */
														Return2BaseFractal(Win, MULTIINT);
														MultiPower=temp;
														MultiPowerFloat=(double)temp;
													}
												}
										   
										   /* copied from below so that REDRAW_MSG is only generated when cancel has not been hit */
										   SaveCoords (Win,TRUE); /* what does this do? */
										   
										   /*SetRast(Win->RPort,DD_BKGRD);*/
										   SAFP();
											
										   SetDefaultCoordinates();

										   EnableSymmetry();
										   Choice = REDRAW_MSG;
										   
										   }
										   
											
                                           break;
									
									case 4 : /* Burning ship */
											 /*JULIA = FALSE;
											 ANTIBUDDHA=FALSE;
											 BaseFractalType=FractalType = BURNINGSHIP;
											 
											 /* enable Julia Set menu */
											 /*OnMenu(Win, FULLMENUNUM (1,4,0));
										     UncheckMenuItem(1,4,0); 
											 
											 /* EasyRequest(MYILBM.win, &NotYetImplemented, NULL); */
											 
											 /*printf("Select BurningShip ...\n");*/
											 
											 Return2BaseFractal(Win, BURNINGSHIP);
											 
											 SaveCoords(Win, TRUE);
											 
											  /* SetRast(Win->RPort,0 /*DD_BKGRD*//*);*/
											 SAFP();
											 
											 SetDefaultCoordinates();
											
											 Choice = REDRAW_MSG;
											 
									       break;
									case 5 : /* empty bar */
										   break;
									case 6 : /* Buddha */
											
											if (GetBitMapBPP(Win->RPort->BitMap)<=1) 
											{
												FunctionIsOnly24Bit(Win);
												break;
											}

											if (
												(FractalType!=BUDDHA) 
												||
												((ANTIBUDDHA) && (FractalType==BUDDHA))
											   )
											{
												JULIA=FALSE;
												FractalType = BUDDHA;
												ANTIBUDDHA = FALSE;
												/* disable Julia Set menu */
												OffMenu(Win, FULLMENUNUM (1,4,0));
											
												/* DrawFractal = DrawBuddhaFractalRandomNumbers; */
												SAFP();
												SaveCoords (Win,TRUE); /* what does this do? */
 						
												/* Coordinates to make the Buddha sit ... :) */
 												RMAX=2.0;
 												RMIN=-2.0;
 
 												IMAX=2.7;
												IMIN=-2.7;
											
												Choice = REDRAW_MSG;
											}
											else
											{
												/* if BUDDHA is active 
												=> switch back to base fractal */
												
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,6); 
												
												/* enable / uncheck Julia Set menu */
										   		OnMenu(Win, FULLMENUNUM (1,4,0));
										   		UncheckMenuItem(1,4,0); 
										   
												SaveCoords (Win,TRUE); /* what does this do? */
										   
										   		SAFP();
												
												SetDefaultCoordinates();

												/*SetRast(Win->RPort,DD_BKGRD);*/
												
										   		Choice = REDRAW_MSG;
											}
											
										   break;
								    case 7 : /* Antibuddha */
											if (GetBitMapBPP(Win->RPort->BitMap)<=1) 
											{
												FunctionIsOnly24Bit(Win);
												break;
											}

											
											if (
												(FractalType!=BUDDHA) 
												||
											    ((FractalType==BUDDHA) && (!ANTIBUDDHA))
											   )
											{
												JULIA=FALSE;
												FractalType = BUDDHA;
												ANTIBUDDHA = TRUE;
												/* disable Julia Set menu */
												OffMenu(Win, FULLMENUNUM (1,4,0));
											
												/* DrawFractal = DrawBuddhaFractalRandomNumbers; */
												SAFP();
												SaveCoords (Win,TRUE); /* what does this do? */
 						
												/* Coordinates to make the Buddha sit ... :) */
 												RMAX=2.0;
 												RMIN=-2.0;
 
 												IMAX=2.7;
												IMIN=-2.7;
											
												Choice = REDRAW_MSG;
											}
											else
											{
												/* if ANTIBUDDHA is active 
												=> switch back to base fractal */
												
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,7); 
												
												/* enable / uncheck Julia Set menu */
										        OnMenu(Win, FULLMENUNUM (1,4,0));
										        UncheckMenuItem(1,4,0); 
										   
												SaveCoords (Win,TRUE); /* what does this do? */
										   
										   		SAFP();
												
												SetDefaultCoordinates();

												/* somehow background is not erased when burningship is selected ... why? */
												/*SetRast(Win->RPort,DD_BKGRD);*/
												/*SetRast(Win->RPort,DD_BKGRD); */
												
										   		Choice = REDRAW_MSG;
											}
											
										   break;
									case 8 : /* Newton */
											/* FractalType = NEWTON; */
											EasyRequest(MYILBM.win, &NotYetImplemented, NULL);
									       break;
								}
								
								SetMenuSelection();
								
                                break;
                   
                        /* Vampire selection */
                        case 5 : SwitchMenuSelection(1,5,1-optimized,SUBNUM(Code));
								 optimized=1-SUBNUM(Code);
								 /* printf("optimized: %d\n", optimized); */
								 
								 SAFP();
								 
								 Choice=SWITCH_CPU;
								 
                                 break;
						case 6 : /* algorithm */
							     temp=SUBNUM(Code);
								 /* adjust selection */ 
								 if (temp<8) /* item 8 = BUDDHA_USE_SYMMETRY */
								 {
								 	if (temp<=BOUNDARY)
								 	{
								 		/* clean old selection */ 
								 		SwitchMenuSelection(1,6,algorithm-BRUTE,temp);
								 		algorithm=SUBNUM(Code);
								 	} 
								 	else
								 	{
								 		/* buddhabrot algorithm selection */
										SwitchMenuSelection(1,6,algorithm_buddha,temp);
								 		algorithm_buddha=temp;	
										if (algorithm_buddha==BUDDHA_BOUNDARY)
										{
										   tempfloat = FloatGad (Win,"Boundary randomization","Max delta z0: ","_Delta",BUDDHA_BOUNDARY_RANDOMIZATION);
								   		   temp=(ULONG)tempfloat;
										   if (temp!=0xffffffff)
										   {
										   		BUDDHA_BOUNDARY_RANDOMIZATION=tempfloat;
										   }
										}
										if (algorithm_buddha==BUDDHA_SWITCH)
										{
										   templong = IntegerGad (Win,"Randomization frequency (1<<x): shift left value (0<=>50% / higher values = less randomization)","Frenquency value: ","_Frequency",BUDDHA_RANDOMIZATION_FREQUENCY);
										   if (templong!=0xffffffff)
										   {
										   		BUDDHA_BOUNDARY_RANDOMIZATION=templong;
										   }
										}		
								 	}
								 } 
								 else 
								 {
								 	BUDDHA_USE_SYMMETRY = BUDDHA_USE_SYMMETRY ? FALSE : TRUE;
									
									if (BUDDHA_USE_SYMMETRY) ItemAddress(MAINMENU,FULLMENUNUM(1,6,8))->Flags |= CHECKED;
									else ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags ^= CHECKED;
								 }
								 
								 
								 SAFP();
								 	
								 break;
									
                   
				   		case 7 : /* menu Drawing */
								 tempold= (drawing == DRAW_CLASSIC) ? 0 : 1;
								 SwitchMenuSelection(1,7,tempold,SUBNUM(Code));
								 
								 switch (SUBNUM(Code)) 
								 {
								 	case 0: /* OS functions */ 
											drawing=DRAW_CLASSIC;
											SetUpDirectDrawing(Win);
											break;
									case 1: /* Screenbuffer */ 
											drawing=DRAW_DIRECT;
											SetUpDirectDrawing(Win);
											break;
								 }
								 break;
								 
						case 8 : /* smooth coloring */
								 
								 if (DD_BPP>1)
								 {
								 	switch (SUBNUM(Code))
								 	{
								 		case 0 : SMOOTH_COLORING = FALSE; break;
										case 2 : SMOOTH_COLORING = SMOOTH_RIC; break;
									    case 3 : SMOOTH_COLORING = SMOOTH_FMNG; break;
								 
								 	}
								 	/* set automatically BRUTE force algorithm when smooth coloring 
								 	 * is selected 
								 	 */
								 	if (SMOOTH_COLORING) 
								 	{	
								 		algorithm=BRUTE;
								 		templong = (ULONG)FloatGad (Win,"Bailout value requester","Bailout value ","_Bailout:",(double)BAILOUT_VALUE);
								 	  	BAILOUT_VALUE = (templong!=0xffffffff) ? templong : BAILOUT_VALUE;
								 
								 	} 
								 }
								 SAFP();
								 SetMenuSelection();
								 
								 break;
#ifdef TEST_MENU_ON						
						case 9 : /* menu TEST */
								 SwitchMenuSelection(1,9,TestMenu,SUBNUM(Code));
								 TestMenu=SUBNUM(Code);
								 
								 switch (TestMenu) 
								 {
								 	case 0: /* test1 */ 
											/* printf("Test1\n"); */
											/*EasyRequest(MYILBM.win, &NotYetImplemented, NULL);*/
											/*FractalType = BURNINGSHIP;*/
											 /* EasyRequest(MYILBM.win, &NotYetImplemented, NULL); */
											 
											 /* SetDefaultCoordinates(); */
											
											 
											 	DrawFractal = DrawTricornFractalBrute; 
											 
											 /*
											 DrawFractal = DrawBurningShipFractalBoundary;
											 */
											 
											 Choice = REDRAW_MSG;
											 
											 
											break;
									case 1: /* test2 */ 
											DrawOrbits=0;
											PeriodicityColoring=0;
											if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer);
											
											
											/* test truecolor screen */
											/* scrptr=OpenScreen(&MyScreen); */
											/*
											scrptr=OpenScreenTags( NULL,
												SA_Left, 0,
												SA_Top, 0,
												SA_Width, 640,
												SA_Height, 480,
												SA_Depth, 8,
												SA_Title, "TestScreen",
												TAG_END
											);
											*/
											/* printf("scrptr: %p\n", scrptr); */
											/*
											Delay(300);
											if (scrptr) CloseScreen(scrptr);
											*/
											scrptr=Win->RPort->BitMap->Planes[0];
											for (x=0;x<Win->Width>>1; x++) 
											{
												/*SetRGB32(ViewPortAddress(Win), 255, (ULONG)(x)<<24,(ULONG)(x)<<24,(ULONG)(x)<<24);*/
												SetAPen(Win->RPort, 255);
												for (y=0; y<Win->Height>>1; y++) 
												{
													SetRGB32(ViewPortAddress(Win), 255, (ULONG)(y)<<24,(ULONG)(x)<<24,(ULONG)(x)<<24);	
													WritePixel(Win->RPort,x,y);
													tptr = scrptr+((y*Win->Width*3)+(x*3));
													/*
														printf("Pixel(%u,%u) = [%u,%u,%u]\n", x, y, *tptr, *(tptr+1), *(tptr+2));
													*/
												}
											}
											/* printf("Test2\n"); */
											Delay(300);
											break;
									
									case 2: /* test3 */ 
											PeriodicityColoring=1;
											/* printf("Test3\n"); */
											/*if (drawing==DRAW_CLASSIC) drawing=DRAW_DIRECT;
											else if (drawing==DRAW_DIRECT) drawing=DRAW_CLASSIC;
											SetUpDirectDrawing(Win);
											*/
											printf("NewRGBMANDChunk.Colors[2-4]: rgb=(%u,%u,%u)\n", 
											NewRGBMANDChunk.Colors[2], NewRGBMANDChunk.Colors[3], NewRGBMANDChunk.Colors[4]);
											
											printf("AGA_COLORS[0]: rgb=(%u,%u,%u)\n", COLORS_AGA[1+0] && 0xff, COLORS_AGA[1+1] & 0xff, COLORS_AGA[1+2] & 0xff);
											
											break;
									case 3 : /* test4 */
											
											/*WritePixelLine(Win->RPort, 100, 50, 16, b, temprp);
											*/
											/*
											for (Color=0; Color<=255; Color++) 
 											{
 												
												SetRGB32(ViewPortAddress(Win), 
													Color, 
													(ULONG)(COLORS_AGA[Color*3+1])<<24,
													(ULONG)(COLORS_AGA[Color*3+2])<<24,
													(ULONG)(COLORS_AGA[Color*3+3])<<24); /* nr, r, g, b - rgb must be left-adjusted! (OMG) */
 											/*}*/

											break;
									case 4 : /* test4 */
											SHOW_MAXCOUNTERS = SHOW_MAXCOUNTERS ? 0 : 1;
											SetMenuSelection();
											break;
									
									case 5 : /* test5 */
											
											SMOOTH_COLORING = SMOOTH_COLORING ? FALSE : TRUE;
											
											if (SMOOTH_COLORING) CheckMenuItem(1,6,9);
											else UncheckMenuItem(1,6,9);
											
											
											/* resume */
											/*BUDDHA_RESUME=TRUE;
											Choice |= REDRAW_MSG;
											*/
											/* toggle menu */
											/*printf("CHECKED: %d Value: %d\n", CHECKED, ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags & CHECKED);
											
											switch (ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags & CHECKED)
											{
												case 0 : printf("Symmetry unchecked => check it.\n");
														 /*ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags |= CHECKED; */
											/*			 CheckMenuItem(1,6,8);
														 BUDDHA_USE_SYMMETRY=TRUE;
														 break;
												case 256 : printf("Symmetry checked  => uncheck it.\n");
														 /* ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags ^= CHECKED; */
											/*			 UncheckMenuItem(1,6,8);
														 BUDDHA_USE_SYMMETRY=FALSE;
														 break;		
											}
											*/
											
											break;
											
								 }
								 break;
#endif /* TEST_MENU_ON */
				   }

                   break;

        case 2: /* menu calculate */
				switch (ITEMNUM (Code))
                {
                   /* normally, Preview is disabled, so 0 shouldn't occur */
				   case 0: Choice |= PREVIEW_MSG;

                           break;

				   case 1: BUDDHA_RESUME=TRUE;
						   
						   Choice |= REDRAW_MSG;
						   break;
						   
                   case 2: Choice |= REDRAW_MSG;

                           break;

                   case 4: Choice |= (UNDO_MSG | REDRAW_MSG);

                           break;

                   case 6: Choice |= DRAW_MSG;

                           break;

                   case 7: Choice |= STOP_MSG;

                           break;
                }

                break;

        case 3: /* menu screen */
				switch (ITEMNUM (Code))
                {
                   case 0: switch (SUBNUM (Code))
                           {
                              case 0: Choice |= CYCLERIGHT_MSG;

                                      break;

                              case 1: Choice |= CYCLELEFT_MSG;

                                      break;

                              case 3: Choice |= DELAY_MSG;

                                      break;

                           }

                           break;

                   case 2: Choice |= PALETTE_MSG;

                           break;

                   case 4: Choice |= NEWDISPLAY_MSG;

                           break;
/* font requester crashes */
/*
                   case 6: Choice |= FONTREQ_MSG;

                           break;
*/
                }
  }

 Code = Item->NextSelect;
 
 }
 return Choice;
}