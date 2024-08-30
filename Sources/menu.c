
#include "splash.h"
#include "menu.h"

struct NewScreen MyScreen = {
	0,0,1280,720,24,			/* top, width, height, depth */
	DETAILPEN, BLOCKPEN,
	HIRES,
	PUBLICSCREEN,
	NULL,
	"TestScreen",
	NULL,
	NULL
};
struct Screen* scrptr;

struct EasyStruct AntiNebulaSwitchToRandom =
{
	sizeof(struct EasyStruct),
	0,
	"AntiNebula requester",
	"It is recommended to switch\nto random algorithm\nand periodicity coloring\nfor AntiNebula.\n",
	"_Accept|_Cancel",
};

struct EasyStruct DoEnableHitmap =
{
	sizeof(struct EasyStruct),
	0,
	"SampleMap requester",
	"If you want to show the Hitmap\nduring calculation or at the end\nyou can additionally enable\n'Hitmap' under algorithms\n(if Hitmap is not activated\nunder Algorithms, FM will show\nEdges&Lemniscates if active)\n\nEnable it now?\n",
	"_Yes|_No",
};

/**************************** menu 0 *******************************************************/

struct NewMenu ProgMenu[] = { NM_TITLE,"Project",0,0,0,0,

                              NM_ITEM,"About",0,NM_ITEMDISABLED,0,0,
							  NM_SUB,"Classic","F",0,(1<<0),0,
                              NM_SUB,"Splash!","A",0,(1<<1),0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Help","H",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Load picture","L",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save picture","S",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Load palette","Y",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Save palette","E",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /* eliminate print and replace it by histogram */
							  NM_ITEM,"Histogram",0,NM_ITEMDISABLED,0,0,
							  NM_SUB,"None",".",CHECKIT,(1<<0),0,
                              NM_SUB,"Raw","W",CHECKIT,(1<<1),0,
							  NM_SUB,"RLE","X",CHECKIT,(1<<2),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Save","D",CHECKIT,(1<<4),0,


                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Info","M",NM_ITEMDISABLED,0,0,
                              
                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Quit","Q",0,0,0,

/**************************** menu1 ****************************************************/

                              NM_TITLE,"Options",0,0,0,0,

                              /* item 0 */
							  NM_ITEM,"Title",0,NM_ITEMDISABLED,0,0,
                              NM_SUB,"Bar","B",CHECKIT|MENUTOGGLE|CHECKED,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Time","T",0,0,0,

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
                              NM_SUB,"Custom...","I",0,~(1<<10),0,
							  NM_SUB,"Nebula RGB","N",0,~(1<<11),0,
							
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Minimum...",0,0,~(1<<13),0,
							  NM_SUB,"Orbits","O",0,~(1<<14),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
						      NM_SUB,"Bailout",0,0,~(1<<16),0,
                              NM_SUB,"MaxLoops",0,0,~(1<<17),0,
							  NM_SUB,"MaxTime",0,0,~(1<<18),0,
							
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
                              NM_SUB,"Julia Set","!",CHECKIT,~(1<<0),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Mandelbrot","\"",CHECKIT,~(1<<2),0,
							  NM_SUB,"Multibrot","§",CHECKIT,~(1<<3),0,
							  NM_SUB,"Burning Ship","$",CHECKIT,~(1<<4),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Nebula","%",CHECKIT,~(1<<6),0,
							  NM_SUB,"Antinebula","&",CHECKIT,~(1<<7),0,
                   
				   			  /* item 5 */
				              /* select between Vampire and classic */
                              NM_ITEM,"Optimized",0,0,0,0,
                              NM_SUB,"Vampire","(",CHECKIT,(1<<0),0,
                              NM_SUB,"Classic",")",CHECKIT,(1<<1),0,

							  /* item 6 */
 							  /* select between brute force, tiled and boundary trace */
							  NM_ITEM,"Algorithm",0,0,0,0,
                              NM_SUB,"Brute","1",CHECKIT,(1<<0),0,
                              NM_SUB,"Tiling","2",CHECKIT,(1<<1),0,
                           	  NM_SUB,"Boundary","3",CHECKIT,(1<<2),0,                           
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Random","4",CHECKIT,(1<<4),0,
                              NM_SUB,"Hitmap","5",CHECKIT,(1<<5),0,
                              NM_SUB,"Lemniscates","6",CHECKIT,(1<<6),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Symmetry","7",CHECKIT,(1<<8),0,
                              NM_SUB,"Periodicity","8",CHECKIT,(1<<9),0,
                            
							  /* item 7 */
						 	  NM_ITEM,"Drawing",0,0,0,0,
                              NM_SUB,"AmigaOS",0,CHECKIT,(1<<0),0,
                              NM_SUB,"Screenbuffer",0,CHECKIT,(1<<1),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Hide Mouse Pointer",0,CHECKIT,(1<<3),0,
                              NM_SUB,"Forbid Menus",0,CHECKIT,(1<<4),0,
                              NM_SUB,"No Symmetry",0,CHECKIT,(1<<5),0,
                              
							  /* item 8 */
						      NM_ITEM,"Coloring",0,0,0,0,
                              NM_SUB,"Classic Iteration Count",0,CHECKIT,(1<<0),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Renormalized Iteration Count",0,CHECKIT,(1<<2),0,
                           	  NM_SUB,"Cyclic Sinus Coloring (FlashMandelNG)",0,CHECKIT,(1<<3),0,
							  NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Boundary fill",0,CHECKIT,(1<<5),0,

							  /* item 9 */					  
                              NM_ITEM,"Hitmap",0,0,0,0,
                              NM_SUB,"Lores",0,CHECKIT,(1<<0),0,
                              NM_SUB,"Hires",0,CHECKIT,(1<<1),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Calculation",0,CHECKIT,(1<<3),0,
                              NM_SUB,"End",0,CHECKIT,(1<<4),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"32x",0,CHECKIT,(1<<6),0,
                              NM_SUB,"16x",0,CHECKIT,(1<<7),0,
                              NM_SUB," 8x",0,CHECKIT,(1<<8),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Colors",0,0,(1<<10),0,
						 	  
/**************************** menu 2 *****************************************************/

                              NM_TITLE,"Calculate",0,0,0,0,

                              NM_ITEM,"PreView",0,NM_ITEMDISABLED,0,0,

							  NM_ITEM,"Resume",0,0,0,0,

                              NM_ITEM,"ReCalculate","R",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Undo",0,NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Zoom","Z",NM_ITEMDISABLED,0,0,

                              NM_ITEM,"Stop",0,0,0,0,

/**************************** menu 3 *******************************************************/

                              NM_TITLE,"Video",0,0,0,0,

                              /*** 0 ***/
                              NM_ITEM,"Cycle",0,NM_ITEMDISABLED,0,0,
                              
                              /* 0-3 */
                              NM_SUB,"Forward",">",0,0,0,
                              NM_SUB,"Backward","<",0,0,0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
                              NM_SUB,"Delay","-",0,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /*** 1 ***/
                              NM_ITEM,"Palette","P",NM_ITEMDISABLED,0,0,

                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /*** 3 ***/
                              NM_ITEM,"Screen",0,NM_ITEMDISABLED,0,0,
                              
                              /* new sub */
                              NM_SUB,"Resolution","V",0,(1<<0),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Bitmap copy",0,CHECKIT,(1<<2),0,
                           	  NM_SUB,"Recalculate",0,CHECKIT,(1<<3),0,
                              NM_SUB,NM_BARLABEL,0,0,0,0,
							  NM_SUB,"Load Only Data",0,CHECKIT,(1<<5),0,
                              
                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /*** 5 ***/
                              NM_ITEM,"Info",0,0,0,0,
                              
                              /* screen modes filtering */
                              
                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              /*** 8 ***/
                              NM_ITEM,"Enable",0,0,0,0,
                              NM_SUB,"32bit modes",0,CHECKIT,(1<<0),0,
                              NM_SUB,"24bit modes",0,CHECKIT,(1<<1),0,
                              NM_SUB,"8bit modes",0,CHECKIT,(1<<2),0,
                              NM_SUB,"PAL/NTSC modes",0,CHECKIT,(3<<1),0,
                              

/* disable font requester (crashes) */
/*
                              NM_ITEM,NM_BARLABEL,0,0,0,0,

                              NM_ITEM,"Font settings...","F",NM_ITEMDISABLED,0,0,
*/
                              NM_END,0,0,0,0,0 };


/******************************************************************************************
 *************** Handle menu selections ***************************************************
 ******************************************************************************************/

void SetMenuSelection(void) 
{
	UWORD s;

	/* check selected menus */
	/* Julia Set selection */
	ClearMenuRange(1,4,0,0);	
    if (JULIA) CheckMenuItem(1,4,0); 

	/* Fractal type selection */
	ClearMenuRange(1,4,2,4);
	switch (BaseFractalType)
	{
		case MANDEL : s=2; break;
		case MULTIINT :
		case MULTIFLOAT : s=3; break;
		case BURNINGSHIP : s=4; break;
	}
	CheckMenuItem(1,4,s);
	
	/* Nebula brot selection*/
	ClearMenuRange(1,4,6,7);	
    if (FractalType==BUDDHA)
	{
		if (ANTIBUDDHA) s=7;
		else s=6;
		ItemAddress(MAINMENU,FULLMENUNUM(1,4,s))->Flags |= CHECKED;
	}

	/* algorithm selection */
	/* base fractal algorithm */
	ClearMenuRange(1,6,0,2);
	switch (algorithm)
	{
		case BRUTE : CheckMenuItem(1,6,0); break;
		case TILE : CheckMenuItem(1,6,1); break;
		case BOUNDARY : CheckMenuItem(1,6,2); break;
	}	
    /* disactivate/reactivate tiling&boundary algorithms (smooth coloring) */
	if (SMOOTH_COLORING)
    {
    	OffMenu(MYILBM.win,FULLMENUNUM (1,6,1));
        OffMenu(MYILBM.win,FULLMENUNUM (1,6,2));
    }
    else
    {
    	OnMenu(MYILBM.win,FULLMENUNUM (1,6,1));
        OnMenu(MYILBM.win,FULLMENUNUM (1,6,2));
    }
    
	/* nebula algorithm */
	ClearMenuRange(1,6,4,6);
	if (BUDDHA_RANDOM) CheckMenuItem(1,6,4);
    else UncheckMenuItem(1,6,4);
    if (USE_HAMMAP) CheckMenuItem(1,6,5);
    else UncheckMenuItem(1,6,5);
    if (BUDDHA_EDGES || BUDDHA_LEMNISCATES) CheckMenuItem(1,6,6);
    else UncheckMenuItem(1,6,6);
    	
    /* general use_ options */
    ClearMenuRange(1,6,8,9);
    if (USE_SYMMETRY) CheckMenuItem(1,6,8);
    else UncheckMenuItem(1,6,8);
	if (USE_PERIODICITY) CheckMenuItem(1,6,9);
    else UncheckMenuItem(1,6,9);

	/* disable vampire optimizations when not on vampire */
	if (!vampire) OffMenu (MYILBM.win,FULLMENUNUM (1,5,0));
    
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
    /* MEMORY BUG IN MenuReumeOff ?! */
	if (FractalType==BUDDHA) MenuResumeOn(MYILBM.win);
	else MenuResumeOff(MYILBM.win);

	/* histogram */
	ClearMenuRange(0,9,0,2);
	if (!histogram_save)
	{
		CheckMenuItem(0, 9, 0);
	}
	else
	{
		if (!histogram_compression) CheckMenuItem(0, 9, 1); 
		else CheckMenuItem(0, 9, 2); 
    }							
	ClearMenuRange(0,9,4,4);
    if (USE_HISTOGRAM_AUTOSAVE) CheckMenuItem(0,9,4);
    
	/* smooth coloring */
	ClearMenuRange(1,8,0,5);
	switch (SMOOTH_COLORING)
	{
		case SMOOTH_RIC : CheckMenuItem(1,8,2); break;
		case SMOOTH_FMNG : CheckMenuItem(1,8,3); break;
		default :
			CheckMenuItem(1,8,0);
	}
	if (BOUNDARY_FILL) CheckMenuItem(1,8,5);
    
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
	
    /* hitmap */
	/* hitmap lores / hires */	
    ClearMenuRange(1,9,0,1);
    if (HITMAP_VISUALIZATION==0) CheckMenuItem(1,9,0);
    else CheckMenuItem(1,9,1);
   	/* show hitmap */
    ClearMenuRange(1,9,3,4);
	if (HITMAP_DURING_CALCULATION) CheckMenuItem(1,9,3);
    if (HITMAP_AT_THE_END) CheckMenuItem(1,9,4);
    /* hitmap extension */
    ClearMenuRange(1,9,6,8);
    switch (HITMAP_EXTENSION)
    {
    	case 32 : CheckMenuItem(1,9,6); break;
        case 16 : CheckMenuItem(1,9,7); break;
        case  8 : CheckMenuItem(1,9,8); break;
    }
    
    /* new screen copy method */
	ClearMenuRange(3,4,2,5);
    if (FM_NEWSCR_CPY == FM_NEWSCR_BMCPY) CheckMenuItem(3,4,2);
    else if (FM_NEWSCR_CPY == FM_NEWSCR_RECALC) CheckMenuItem(3,4,3);
    if (LoadOnlyData) CheckMenuItem(3,4,5);
    /* disable bitmap copy for classic screen */
    if (IsAClassicScreen(MYILBM.camg)) 
    	OffMenu(MYILBM.win, FULLMENUNUM(3,4,2));
    else
    	OnMenu(MYILBM.win, FULLMENUNUM(3,4,2));
        
    SetIterationsMenu();
	
	/* disable font requester (crashes) */
	/* take it completely out of the menu */
	/* OffMenu(MYILBM.win, FULLMENUNUM (3,10, NOSUB)); */

	SetDrawingMenu();

	/* filter screen modes */
    ClearMenuRange(3,8,0,3);
	if (!FM_NO32BITMODES) CheckMenuItem(3,8,0);
    if (!FM_NO24BITMODES) CheckMenuItem(3,8,1);
    if (!FM_NO8BITMODES) CheckMenuItem(3,8,2);
    if (!FM_NOPALNTSCMODES) CheckMenuItem(3,8,3);
}

void SetDrawingMenu(void)
{
    ClearMenuRange(1,7,0,1);
    if (drawing==DRAW_CLASSIC) CheckMenuItem(1,7,0);
    else CheckMenuItem(1,7,1);
    
	if (FM_BUSY_MOUSE_POINTER==NO_POINTER) CheckMenuItem(1,7,3);
    else UncheckMenuItem(1,7,3);

    if (FM_FORBID_MENUS) CheckMenuItem(1,7,4);
    else UncheckMenuItem(1,7,4);
    
    if (FM_FORBID_SYMMETRY) CheckMenuItem(1,7,5);
    else UncheckMenuItem(1,7,5);

	/* disable direct drawing for classic screens */
/* these lines make FM crash */
/*    if (IsAClassicScreen(MYILBM.camg)) 
    	OffMenu(MYILBM.win, FULLMENUNUM(1,7,1));
    else
    	OnMenu(MYILBM.win, FULLMENUNUM(1,7,1));
*/
}

void ClearMenuRange(UWORD x, UWORD y, UWORD z1, UWORD z2)
{
	UWORD temp;
	for (temp=z1; temp<=z2; temp++)
	{
		UncheckMenuItem(x,y,temp);
	}
}

void CheckMenuItem(UWORD a, UWORD b, UWORD c)
{
	/* menuitems may not have MENUTOGGLE active! 
	(otherwise os will toggle and this function will check the item, 
	leading to an unclear state */
    if ((ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags & CHECKED) == 0)
	{
		ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags |= CHECKED;
	}

}

void UncheckMenuItem(UWORD a, UWORD b, UWORD c)
{
	if ((ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags & CHECKED) == CHECKED)
	{
		ItemAddress (MAINMENU,FULLMENUNUM (a,b,c))->Flags &= ~CHECKED;
	}

}

void SwitchMenuSelection(UWORD a, UWORD b, UWORD from, UWORD to) 
{
	/* clear old selection */
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

/******************************************************************************************
 ******************************************************************************************
 ******************************************************************************************/

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

		ItemAddress(MAINMENU,FULLMENUNUM(0,9,2))->Flags |= CHECKED;  

		/* check vampire menus */        
        if (optimized) ItemAddress(MAINMENU,FULLMENUNUM(1,5,0))->Flags |= CHECKED;  
        else ItemAddress(MAINMENU,FULLMENUNUM(1,5,1))->Flags |= CHECKED;       
        
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
		if (USE_SYMMETRY) ItemAddress(MAINMENU,FULLMENUNUM(1,6,8))->Flags |= CHECKED;
		else ItemAddress (MAINMENU,FULLMENUNUM (1,6,8))->Flags ^= CHECKED;
		
        if (BOUNDARY_FILL) ItemAddress(MAINMENU,FULLMENUNUM(1,8,5))->Flags |= CHECKED;
		else ItemAddress (MAINMENU,FULLMENUNUM (1,8,5))->Flags ^= CHECKED;

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

           PutPointer (MYILBM.win,0,0,0,0,0,FM_BUSY_MOUSE_POINTER);
           
		   SetRGBIterations();

		   SAFP();			 

		   SetUpDirectDrawing(MYILBM.win);

           /* in the future: put all menu selection 
            * inside this function (..)
            */

		   SetMenuSelection();

 		   /* enable symmetry for first run for speed */
           EnableSymmetry();
  
           ELAPSEDTIME = DrawFractalWrapper (MYILBM.win,(LONG) (MYILBM.win->LeftEdge),(LONG) (MYILBM.win->TopEdge),(LONG) (MYILBM.win->Width) - 1L,(LONG) (MYILBM.win->Height) - 1L);

           PutPointer (MYILBM.win,ZoomPointer,ZPW,ZPH,ZPXO,ZPYO,ZOOM_POINTER);

           SetMenuStart (MYILBM.win);

           ModifyIDCMP (MYILBM.win,NULL);

		   if (vampire) ShowSplashRequester(MYILBM.win,2);
           else About (MYILBM.win);
        
           ResetMenuStrip (MYILBM.win,MAINMENU);

           ModifyIDCMP (MYILBM.win,IDCMP_STANDARD);

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

  /* other option menus */
  OnMenu  (Win,FULLMENUNUM (1,5,NOSUB));
  OnMenu  (Win,FULLMENUNUM (1,6,NOSUB));
  OnMenu  (Win,FULLMENUNUM (1,7,NOSUB));
  OnMenu  (Win,FULLMENUNUM (1,8,NOSUB));
  OnMenu  (Win,FULLMENUNUM (1,9,NOSUB));
  /* end other option menus */
  
  /* disable undo */
  /* OffMenu (Win,FULLMENUNUM (2,4,NOSUB)); */
        
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
  
  /* other option menus */
  OffMenu  (Win,FULLMENUNUM (1,5,NOSUB));
  OffMenu  (Win,FULLMENUNUM (1,6,NOSUB));
  OffMenu  (Win,FULLMENUNUM (1,7,NOSUB));
  OffMenu  (Win,FULLMENUNUM (1,8,NOSUB));
  OffMenu  (Win,FULLMENUNUM (1,9,NOSUB));
  /* end other option menus */

  /* disable undo */
  /* OffMenu  (Win,FULLMENUNUM (2,4,NOSUB)); */
        
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

void OffTitle(struct Screen* scr)
{
	MASK ^= TMASK;
    ShowTitle(scr, FALSE);
    
}

void OnTitle(struct Screen* scr)
{
	MASK |= TMASK;
    ShowTitle(scr, TRUE);
}

/*
void IdentifyMenu(ULONG Code)
{
	printf("IdentifyMenu(): MENUNUM: %u ITEMNUM: %u SUBNUM: %u\n", MENUNUM (Code), ITEMNUM (Code), SUBNUM (Code));
}
*/

ULONG ProcessMenu (struct Window *Win,UWORD Code)
{
 struct MenuItem *Item;
 ULONG temp;
 ULONG Choice = NULL;
 double tempfloat;
 double templong;
 BOOL noclrscr=FALSE;
 
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

                        /* empty bar */
                        
                        case 9:	switch (SUBNUM(Code))
								{
									/* don't save histogram */
									case 0 : histogram_save=FALSE; break;
									/* save raw histogram */
									case 1 : histogram_save=TRUE; histogram_compression=0; break;
									/* save histogram with rle compression */
									case 2 : histogram_save=TRUE; histogram_compression=1; break;
									/* empty bar */
                                    case 4 : USE_HISTOGRAM_AUTOSAVE=!USE_HISTOGRAM_AUTOSAVE; 
                                    		 if (USE_HISTOGRAM_AUTOSAVE)
                                             {
                                    			templong=IntegerGad (MYILBM.win,"Autosave requester","Autosave time (secs)","Autosave _Time (secs)",HISTOGRAM_AUTOSAVE_TIME);
                                             	printf("templong: %u\n", templong);	
                                                if (templong) HISTOGRAM_AUTOSAVE_TIME=templong;
                                             }
                                    		 break;						
                                
                                }
								SetMenuSelection();
								break;
							
						/* 10 = empty bar */
                        
                        case 11: Choice = PICINFO_MSG;
                                 break;
                        
                        /* 12 = empry bar */
                        					
                        case 13: Choice |= EXIT_MSG;

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
								  case 11: NebulaIterationsGadget(Win,"Nebulabrot: select RGB iterations", "RGB iterations");
										   MAX_ITERATIONS = max(RGB_ITER[FM_RED_MAX], MAX_ITERATIONS);
                                           break;
										   
								  /* 12 = menu bar */
								  
								  case 13 : /* Minimum (for Buddhabrot) */
								  			BUDDHA_MIN_ITERATIONS = (ULONG)FloatGad (Win,"Buddhabrot requester","Minimum Iterations for Buddhabrot rendering ","_Minimum:",(double)BUDDHA_MIN_ITERATIONS);
								  		    break;
                                            
								  case 14 : /* Show Orbits */
								  			if (((FractalType==MANDEL) || (FractalType==MULTIINT) || (FractalType==MULTIFLOAT) || (FractalType=BURNINGSHIP))  &&
											   	(RMIN==INIT_DEF_RMIN) &&
												(RMAX==INIT_DEF_RMAX) &&
												(IMIN==INIT_DEF_IMIN) &&
												(IMAX==INIT_DEF_IMAX)
											   )
											{
												if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer);
                                                ScrCpySize=sizeof(UBYTE)*Win->Width*Win->Height;
                                                
                                                if (AllocIterArray() && (ScrCpyBuffer=AllocBitMap(Win->Width, Win->Height,MYILBM.Bmhd.nPlanes,BMF_CLEAR,/*NULL*/Win->RPort->BitMap)))
                                               	{	
                                                	OffTitle(MYILBM.scr);	
                                                    FM_ORBIT_COLOR = IntegerGad (MYILBM.win,"Orbit color","Insert orbit color...","Co_lor", FM_ORBIT_COLOR);
										
                                                	DrawOrbits=1;
													
													BltBitMap(Win->RPort->BitMap,0,0, ScrCpyBuffer,0,0,Win->Width,Win->Height, 0xc0, 0xff,NULL);
													
                                                    if (FM_ORBIT_1ST) {
                                                    	OrbitsActivated(Win);
                                                    	FM_ORBIT_1ST=FALSE;
                                                    }
												} 
                                                else 
                                                {
                                                	if (ScrCpyBuffer) FreeBitMap(ScrCpyBuffer);
                                                	printf("Could not allocate memory for orbits.\n");
												}									
                                            } 
											else
								  		    {
												/* Show message that orbits can only be 
                                                 * selected on main view.
                                                 */
												DrawOrbits=0;
												OrbitsNotPossible(Win);
												
											}
										    SetMenuSelection();
										    break;
								
                                  /* 15 = menu bar */
                                
								  case 16 : /* bailout value */
										  templong = (ULONG)FloatGad (Win,"Bailout value requester","Bailout value ","_Bailout:",(double)BAILOUT_VALUE);
								   		  BAILOUT_VALUE = (templong!=0xffffffff) ? templong : BAILOUT_VALUE;
										   
										  break;
                                          
                                  case 17 : /* buddha loop value */
										  templong = (ULONG)FloatGad (Win,"Nebulaloops requester","Nebulaloops ","_Loops:",(double)BUDDHA_MAX_LOOPS);
								   		  BUDDHA_MAX_LOOPS = (templong!=0xffffffff) ? templong : BUDDHA_MAX_LOOPS;
										   
										  break;
                                          
                                  case 18 : /* maxtime (seconds) for calculation (for buddha) */
										  templong = (ULONG)FloatGad (Win,"Define maximum calculation time","MaxTime ","_MaxTime:",(double)FM_MAX_CALC_SECS);
								   		  FM_MAX_CALC_SECS = (templong!=0xffffffff) ? templong : FM_MAX_CALC_SECS;
										   
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
                                                        
                                if (FractalType=BURNINGSHIP) USE_SYMMETRY=FM_ORIGINAL_USE_SYMMETRY;
                                
                                if (FractalType==BUDDHA)
								{
									/* restore normal palette 
                                     * after BUDDHA 
                                     */
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
											  
											  SAFP();
											  
									          SetWindowTitles (Win,(TEXT *) ~0,"Julia constant selected");

                                              Choice |= REDRAW_MSG;

                                              DisplayBeep (Win->WScreen);

                                              Delay (ONESEC);
										   }
										   else
										   {
										   		/* if JULIA is active 
                                                 * => switch back to 
                                                 * base fractal 
                                                 */
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,0); 
												
												SaveCoords (Win,TRUE); 
										   
										   		SAFP();
												
										   		Choice = REDRAW_MSG;
										   }

                                           break;

                                   /* empty bar */
                                   
								   case 2: /* Mandelbrot */
								   		   
										   Return2BaseFractal(Win, MANDEL);
										   
										   SaveCoords (Win,TRUE); 
										   
										   SAFP();
											
										   SetDefaultCoordinates();

										   if (USE_SYMMETRY) EnableSymmetry();
										   
										   Choice = REDRAW_MSG;
											
                                           break;
								   
                                   case 3: /* Multibrot 
                                   			* => possible to select Mandelbrot 
                                            * via value 2 
                                            */
										   
										   tempfloat = FloatGad (Win,"Multibrot requester","Insert (p)ower: zn1 = zn^p + c          Note: p>1, fractional numbers allowed","_Power (2=Mandel)",MultiPowerFloat);
								   		   temp=(ULONG)tempfloat;
										   
										   if (temp!=0xffffffff)
										   {
												if ((double)temp != tempfloat) 
												{
													if (tempfloat<1) {
														EasyRequest(MYILBM.win, &NotYetImplemented, NULL);
													}
													else
													{
														Return2BaseFractal(Win, MULTIFLOAT);
														MultiPowerFloat = tempfloat;
                                                        /* disable symmetry by default,
                                                         * when MultiFloat is selected.
                                                         */
                                                        USE_SYMMETRY=FALSE;
													}
												}
												else 
												{
													if (temp==2) 
													{
														Return2BaseFractal(Win, MANDEL);
														MultiPower=temp;
														MultiPowerFloat=tempfloat;
													}
													else 
													{
														Return2BaseFractal(Win, MULTIINT);
														MultiPower=temp;
														MultiPowerFloat=(double)temp;
													}
												}
										   
										   /* copied from below so that REDRAW_MSG 
                                            * is only generated when cancel has not 
                                            * been hit 
                                            */
                                            
										   SaveCoords (Win,TRUE); 
										   
										   SAFP();
											
										   SetDefaultCoordinates();

										   if (USE_SYMMETRY) EnableSymmetry();
										   Choice = REDRAW_MSG;
										   
										   } else noclrscr=TRUE;
										   
											
                                           break;
									
									case 4 : /* Burning ship */
											 if (FractalType!=BURNINGSHIP) FM_ORIGINAL_USE_SYMMETRY=USE_SYMMETRY;
                                            
											 Return2BaseFractal(Win, BURNINGSHIP);
											 
											 SaveCoords(Win, TRUE);
											 
                                             USE_SYMMETRY=FALSE;
                                             
											 SAFP();
											 
											 SetDefaultCoordinates();
											
											 Choice = REDRAW_MSG;
											 
									       break;
									
                                    /* empty bar */
                                    
									case 6 : /* Buddha */
											
											if (GetBitMapBPP(Win->RPort->BitMap)<=1) 
											{
												FunctionIsOnly24Bit(Win);
                                                noclrscr=TRUE;
												break;
											}

											if (
												(FractalType!=BUDDHA) 
												||
												((ANTIBUDDHA) && (FractalType==BUDDHA))
											   )
											{
                                            	/* allocate Buddha memory */
                                                if (AllocBuddha())
                                                {
                                                	JULIA=FALSE;
													FractalType = BUDDHA;
													ANTIBUDDHA = FALSE;
												
                                                	/* disable Julia Set menu */
													OffMenu(Win, FULLMENUNUM (1,4,0));
											
													SAFP();
                                                
													SaveCoords (Win,TRUE); 
 						
													/* Coordinates to make 
                                                     * the Buddha sit ... :) 
                                                     */
                                                     
 													RMAX=2.0;
 													RMIN=-2.0;
 
 													IMAX=2.7;
													IMIN=-2.7;
											
													Choice = REDRAW_MSG;
												 } 
                                                 else
                                                 {
                                                 	/* not enough memory 
                                                     * to calculate Buddha 
                                                     */
                                                 	EasyRequest(MYILBM.win, &NoMemory4Buddha, NULL);
                                                 	SAFP();
                                                    SetMenuSelection();
                                                 }									
                                            }
											else
											{
												/* if BUDDHA is active 
												 * => switch back to base fractal
												 * NOTE: do not free Buddha memory 
                                                 * (avoid fragmentation) 
                                                 */
                                                
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,6); 
												
												/* enable / uncheck Julia Set menu */
										   		OnMenu(Win, FULLMENUNUM (1,4,0));
										   		UncheckMenuItem(1,4,0); 
										   
												SaveCoords (Win,TRUE); 
										   
										   		SAFP();
												
												SetDefaultCoordinates();

										   		Choice = REDRAW_MSG;
											}
											
										   break;
								    
                                    case 7 : /* Antibuddha */
											if (GetBitMapBPP(Win->RPort->BitMap)<=1) 
											{
												FunctionIsOnly24Bit(Win);
												noclrscr=TRUE;	
                                                SetMenuSelection();							
                                                break;
											}

											if (
												(!ANTIBUDDHA)
											   )
											{
												if (AllocBuddha())
                                                {
                                                	JULIA=FALSE;
													FractalType = BUDDHA;
													ANTIBUDDHA = TRUE;
												
                                                	/* disable Julia Set menu */
													OffMenu(Win, FULLMENUNUM (1,4,0));
											
                                                	if ((!BUDDHA_RANDOM) || (BUDDHA_LEMNISCATES) || (!USE_PERIODICITY))
                                                	{
                                                	    if (FM_ANTIBUDDHA_1ST)
                                                        {
															temp = EasyRequest(MYILBM.win, &AntiNebulaSwitchToRandom, NULL);
                                                	    	if (temp)
                                                	    	{
                                                				 SetLemniscatesVariables(0);   
                                                	    		 USE_PERIODICITY=TRUE;
                                                	    	}
                                                			FM_ANTIBUDDHA_1ST=FALSE;		
                                                        }	
                                                    }
													SAFP();
													SaveCoords (Win,TRUE); 
 						
													/* Coordinates to make 
                                                     * the Buddha sit ... :) 
                                                     *
                                                     /
 													RMAX=2.0;
 													RMIN=-2.0;
 	
 													IMAX=2.7;
													IMIN=-2.7;
											
													Choice = REDRAW_MSG;
												} 
                                                else
                                                {
                                                 	/* not enough memory 
                                                     * to calculate Buddha 
                                                     */
                                                 	EasyRequest(MYILBM.win, &NoMemory4Buddha, NULL);
                                                 	SAFP();
                                                    SetMenuSelection();
                                                }										
                                            }
											else
											{
												/* if ANTIBUDDHA is active 
												 * => switch back to base fractal 
												 * NOTE: do not free Buddha memory 
                                                 * (avoid fragmentation) 
                                                 */
                                                
												FractalType=BaseFractalType;;
										   		ANTIBUDDHA=FALSE;
												JULIA = FALSE;
												
										   		UncheckMenuItem(1,4,7); 
												
												/* enable / uncheck Julia Set menu */
										        OnMenu(Win, FULLMENUNUM (1,4,0));
										        UncheckMenuItem(1,4,0); 
										   
												SaveCoords (Win,TRUE); 
										   
										   		SAFP();
												
												SetDefaultCoordinates();

										   		Choice = REDRAW_MSG;
											}
											
										   break;
								}
								
								SetMenuSelection();
								
        						if (!noclrscr) SetRast(Win->RPort,0);
                                else noclrscr=FALSE;
                                
                                break;
                   
                        /* Vampire selection */
                        
                        case 5 : SwitchMenuSelection(1,5,1-optimized,SUBNUM(Code));
								 optimized=1-SUBNUM(Code);
                                 
								 SAFP();
								 
								 Choice=SWITCH_CPU;
								 
                                 break;
						
                        case 6 : /* algorithm */
							     temp=SUBNUM(Code);
                                 
								 /* adjust selection */ 
								 if (temp<=BOUNDARY)
								 {
								 		/* clean old selection */ 
								 		SwitchMenuSelection(1,6,algorithm-BRUTE,temp);
								 		algorithm=SUBNUM(Code);
								 } 
								 else
								 {	
                                    switch(temp)
                                    {
                                    	case 4 : /* random */ 
                                        		 BUDDHA_RANDOM=TRUE;
                                                 BUDDHA_EDGES=BUDDHA_LEMNISCATES=FALSE;
                                                 
                                                 SetLemniscatesVariables(0); 
                                                 
                                                 break;
                                        
                                        case 5 : /* hitmap */
                                        		 USE_HAMMAP = USE_HAMMAP ? FALSE : TRUE;	
                                                 
                                                 if (!USE_HAMMAP) FreeHitmap();
                                                 
                                        		 break;
                                                
                                        case 6 : /* lemniscates */
                                        		 LemniscatesParametersGadget(Win,"Lemniscates", "Select Leminscates Parameters");
                                        		 
                                                 break;
                                        
                                        /* empty bar */
                                        
                                        case 8 : /* symmetry */ 
                                                 
                                                 /* FM_ORIGINAL_USE_SYMMETRY allows to keep original setting for use_symmetry
                                                  * when switching from Mandelbrot/Multibrot to Burningship (that can not
                                                  * be calculated with symmetry) and back.
                                                  */
                                                  
                                        		 FM_ORIGINAL_USE_SYMMETRY = USE_SYMMETRY = USE_SYMMETRY ? FALSE : TRUE;	
                                                 if (USE_SYMMETRY)
                                                 {
                                                 	tempfloat = FloatGad (Win,"Nebula autocenter (tolerance)","Tolerance","_Percentage (0-100%)",BUDDHA_AUTO_CENTER_PERCENTAGE);
								   		  			temp=(ULONG)tempfloat;
										   			
                                                 	if (temp!=0xffffffff)
                                            		{
                                                    	BUDDHA_AUTO_CENTER_PERCENTAGE = tempfloat;
                                            			BUDDHA_AUTO_CENTER_EPSILON=3.0 / 100.0 * BUDDHA_AUTO_CENTER_PERCENTAGE;
                                                        
                                                        if (BUDDHA_AUTO_CENTER_EPSILON>0) BUDDHA_AUTO_CENTER=FALSE;
                                                        else BUDDHA_AUTO_CENTER=TRUE;
                                                    }
                                                 }
                                                 break;
                                        
                                        case 9 : /* periodicity */ 
                                                 USE_PERIODICITY = USE_PERIODICITY ? FALSE : TRUE; 
                                                 
                                                 
                                                 if (USE_PERIODICITY)
                                                 {
                                                 	PeriodicityParametersGadget(Win,"Periodicity", "Select Epsilon / Prime Number Coloring Parameters");
                                                 }
                                                 
                                                 break;
                                    }
                                    
                                 	
                                 }
                                 SetMenuSelection();
                                 
								 SAFP();
								 	
								 break;
									
                   
				   		case 7 : /* menu Drawing */
								 switch (SUBNUM(Code)) 
								 {
								 	case 0: /* OS functions */ 
                                            drawing=DRAW_CLASSIC;
                                            SetUpDirectDrawing(Win);
                                            if (DD_BPP>1) NotAccurateColorsIn24Bit(MYILBM.win);
											break;
									case 1: /* Screenbuffer */ 
											drawing=DRAW_DIRECT;
											SetUpDirectDrawing(Win);
											break;
                                    
                                    /* 2 = empty bar */
                                    
                                    case 3: /* Hide Mouse Pointer */
                                    		FM_BUSY_MOUSE_POINTER = (FM_BUSY_MOUSE_POINTER==NO_POINTER) ? BUSY_POINTER : NO_POINTER;
                                            break;
                                            
                                    case 4: /* forbid menus */
                                    		FM_FORBID_MENUS = !FM_FORBID_MENUS ;
                                            break;
                                    case 5: /* forbid symmetry */
                                    		FM_FORBID_SYMMETRY = !FM_FORBID_SYMMETRY;
                                            break;
								 }
								 SetMenuSelection();						 
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
								 	 * is selected (store old algorithm / bailout value)
								 	 */
								 	if (SMOOTH_COLORING) 
								 	{	
								 		if (algorithm_before_smooth_coloring==0xffffffff) 
                                        {
                                        	bailout_before_smooth_coloring=BAILOUT_VALUE;
                                        	algorithm_before_smooth_coloring=algorithm;
                                        }
                                        algorithm=BRUTE;
								 	  	
             						    if (FM_SMOOTH_COLORING_BAILOUT_1ST)
                           				{
                                        	/* fill in a high default value the first time smooth coloring is selected */	
											templong = (ULONG)FloatGad (Win,"Bailout value requester","Select a high bailout value for smooth coloring (or hit cancel to keep default)","_Bailout (Cancel=4):",(double)4000000000);
                                            BAILOUT_VALUE = (templong!=0xffffffff) ? templong : BAILOUT_VALUE;
                                        	if (templong!=0xffffffff) FM_SMOOTH_COLORING_BAILOUT_1ST=FALSE;
                                            else algorithm_before_smooth_coloring=0xffffffff;
                                        }
                                        else
                                        {	
                                        	templong = (ULONG)FloatGad (Win,"Bailout value requester","A high bailout value is recommended for smooth coloring","_Bailout:",(double)BAILOUT_VALUE);
                                            BAILOUT_VALUE = (templong!=0xffffffff) ? templong : BAILOUT_VALUE;
								 		}
								 	} 
                                    else
                                    {
              							/* no smooth coloring => restore old algorithm */
                                        if (algorithm_before_smooth_coloring!=0xffffffff)
                                        {
              								BAILOUT_VALUE=bailout_before_smooth_coloring;
                                            algorithm = algorithm_before_smooth_coloring;
                                            algorithm_before_smooth_coloring = 0xffffffff; 
                                            FM_SMOOTH_COLORING_BAILOUT_1ST=TRUE;                         
                                        }                      
                                    }
								 }
								 if (SUBNUM(Code) == 5)
                                 {
  									if (BOUNDARY_FILL) BOUNDARY_FILL=FALSE;
                                    else BOUNDARY_FILL=TRUE;                                
                                 }
                                 						 
                                 SAFP();
								 SetMenuSelection();
								 
								 break;
                        
                        case 9 : /* hitmap */
								 
                                 switch (SUBNUM(Code))
								 {
                                 		case 0 : HITMAP_VISUALIZATION = 0; break; /* lores */
										case 1 : HITMAP_VISUALIZATION = 1; break; /* hires */
										/* 2=empty bar */
								 		case 3 : HITMAP_DURING_CALCULATION = !HITMAP_DURING_CALCULATION; break;
										case 4 : HITMAP_AT_THE_END = !HITMAP_AT_THE_END; break;
										/* 5=empty bar */
                                        case 6 : HITMAP_EXTENSION=32; HITMAP_SHIFT=0; break;
                                        case 7 : HITMAP_EXTENSION=16; HITMAP_SHIFT=1; break;
                                        case 8 : HITMAP_EXTENSION=8; HITMAP_SHIFT=2; break;
                                        /* 9=empty bar */
                                        case 10 : HitmapColorsGadget(Win,"SampleMap colors (enter RGB values)", "SampleMap colors"); break;
                                 }
                                 
                                 /* enable hitmap automatically when displaying is selected? */
                                 /* => ask the user */
                                 if ((HITMAP_DURING_CALCULATION || HITMAP_AT_THE_END) && (!USE_HAMMAP))	
                                 {
                                 	temp = EasyRequest(MYILBM.win, &DoEnableHitmap, NULL);
                                 	if (temp==1) USE_HAMMAP=TRUE;
                                    else USE_HAMMAP=FALSE;
                                 }
                                 					 
                                 SAFP();
								 SetMenuSelection();
								 
								 break;
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
						   if (
                           		(USE_SYMMETRY) 
                                &&
                                (RMIN==INIT_DEF_RMIN)
                                &&
                                (RMAX==INIT_DEF_RMAX)
                                &&
                                (IMIN==INIT_DEF_IMIN)
                                &&
                                (IMAX==INIT_DEF_IMAX)
                                &&
                                (!JULIA)
                                && 
                                (FractalType!=BURNINGSHIP)
                                && 
                                (FractalType!=BUDDHA)
                              )
                              		EnableSymmetry();
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

                   case 4: /* screen menu */
                   		   switch (SUBNUM(Code))
                           {
                   		   		case 0 : Choice |= NEWDISPLAY_MSG;
                                	     break;
                                
                                /* empty bar */
                                
                                case 2 : /* bitmap copy */
                                		 FM_NEWSCR_CPY = (FM_NEWSCR_CPY==FM_NEWSCR_BMCPY) ? NULL : FM_NEWSCR_BMCPY;
                                         break;
                                
                                case 3 : /* recalculate */
                                		 FM_NEWSCR_CPY = (FM_NEWSCR_CPY==FM_NEWSCR_RECALC) ? NULL : FM_NEWSCR_RECALC;
                                         break;
                                         
                                /* empty bar */
                                
                                case 5 : /* load only data (no screen mode switch when loading a picture */
                                		 LoadOnlyData = (LoadOnlyData) ? FALSE : TRUE;
                                         if (LoadOnlyData) EasyRequest(MYILBM.win, &ExperimentalFunction, NULL);
                                         break;
                                         
						   }
                           SetMenuSelection();
                           break;
                   
                   /* empty bar */
                   
                   case 6: sprintf(screenmodestr,
                   				"Width:  %lu\nHeight: %lu\nDepth:  %lu\nCAMG:   0x%x\n",
                                MYILBM.Bmhd.w, MYILBM.Bmhd.h, MYILBM.Bmhd.nPlanes, MYILBM.camg);
                   
                   		   EasyRequest(MYILBM.win, &ShowScreenMode, NULL);
                   
                           break;
                           
                   /* empty bar */
                   
                   case 8 : switch (SUBNUM (Code))
                            {
                  
                          		case 0 : FM_NO32BITMODES = !FM_NO32BITMODES;
                           				 SetMenuSelection();
                   		   				 break;
                           
                   				case 1 : FM_NO24BITMODES = !FM_NO24BITMODES;
                   						 SetMenuSelection();
                   				         break;
                                case 2 : FM_NO8BITMODES = !FM_NO8BITMODES;
                           				 SetMenuSelection();
                   		   				 break;
                           
                   				case 3 : FM_NOPALNTSCMODES = !FM_NOPALNTSCMODES;
                   						 SetMenuSelection();
                   				         break;
                            }
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