#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "output.h"
#include "input.h"
#include "menu.h"
#include "search.h"

tInt8 gotomask(WINDOW* win,tUInt64* cursorpos)
{
	tUInt64 actcursorpos=*cursorpos;
	tUInt64 newcursorpos=*cursorpos;
	tInt8	itemnums[3];
	tInt8	selected;
	tMenu	Menu1;
	tInt16	offsx;
	tInt16	offsy;
	tInt8	retval=OK;


	offsx=COLS/2-21;
	offsy=LINES/2-2;

	drawcenterframe(win,3,42,"Goto...");
	setcolor(win,COLOR_BRACKETS);
	mvwprintw(win,offsy+3,offsx+6,"[                 ]");
	clearMenu(&Menu1);
	newMenuItem(&Menu1,"To",3,1,'T',0,&itemnums[0]);
	newMenuItem(&Menu1,"Go",3,28,'G',0,&itemnums[1]);
	newMenuItem(&Menu1,"Cancel",3,33,'C',1,&itemnums[2]);
	selected=-1;
	while (1)
	{
		mvwprintw(win,offsy+3,offsx+7,"%17llx",newcursorpos);
		selected=MenuInteract(win,&Menu1,LINES/2-2,COLS/2-21);
		if (selected==itemnums[0]) {newcursorpos=actcursorpos;hexinput(win,offsy+3,offsx+6,&newcursorpos,17);}
		if (selected==itemnums[1]) {*cursorpos=newcursorpos;return RETOK;}
		if (selected==itemnums[2]) return RETNOK;
	}
}
void searchmask(WINDOW* win,tSearch* search,tBuffer* buf,tUInt64* cursorpos)
{
	tInt8	itemnums[9];
	tMenu	Menu1;
	tInt8	selected;
	tInt16	offsx;
	tInt16	offsy;

	clearsearch(search);
	offsx=COLS/2-30;
	offsy=LINES/2-7;

	drawcenterframe(win,14,60,"Search...");
	clearMenu(&Menu1);

	newMenuItem(&Menu1,"Searchstring:",1,1,'S',0,&itemnums[0]);
	newMenuItem(&Menu1,"Forward",4,1,'F',0,&itemnums[1]);
	newMenuItem(&Menu1,"Backward",4,17,'B',0,&itemnums[2]);
	newMenuItem(&Menu1,"Write to searchlog",6,1,'W',0,&itemnums[3]);
	newMenuItem(&Menu1,"File:",7,1,0,0,&itemnums[4]);
	newMenuItem(&Menu1,"Read from searchlog",9,1,'R',0,&itemnums[5]);
	newMenuItem(&Menu1,"File:",10,1,0,0,&itemnums[6]);
	newMenuItem(&Menu1,"Go",13,1,'G',0,&itemnums[7]);
	newMenuItem(&Menu1,"Cancel",13,51,'C',1,&itemnums[8]);
	selected=-1;
	setcolor(win,COLOR_BRACKETS);
	mvwprintw(win,offsy+2,offsx+1,"[                                                        ]");
	setcolor(win,COLOR_BRACKETS);
	mvwprintw(win,offsy+4,offsx+11,"( )");
	mvwprintw(win,offsy+4,offsx+28,"( )");
	mvwprintw(win,offsy+6,offsx+22,"[ ]");
	mvwprintw(win,offsy+9,offsx+23,"[ ]");
	mvwprintw(win,offsy+7 ,offsx+7,"[                                                  ]");
	mvwprintw(win,offsy+10,offsx+7,"[                                                  ]");
	while (selected!=itemnums[7] && selected!=itemnums[8])
	{
		setcolor(win,COLOR_TEXT);
		mvwprintw(win,offsy+4,offsx+12,"%c",search->forwardnotbackward?'o':' ');
		mvwprintw(win,offsy+4,offsx+29,"%c",search->forwardnotbackward?' ':'o');
		mvwprintw(win,offsy+6,offsx+23,"%c",search->writesearchlog?'X':' ');
		mvwprintw(win,offsy+9,offsx+24,"%c",search->readsearchlog?'X':' ');
		selected=MenuInteract(win,&Menu1,offsy,offsx);
			
		if (selected==itemnums[0]) 
		{
			hexinput2(win,offsy+2,offsx+1,search->searchstring,&search->searchlen,19);
		}
		if (selected==itemnums[1]) search->forwardnotbackward=1;
		if (selected==itemnums[2]) search->forwardnotbackward=0;
		if (selected==itemnums[3]) search->writesearchlog=!search->writesearchlog;
		if (selected==itemnums[4]) 
		{
			stringinput(win,offsy+7,offsx+7,search->writelogfilename,50);
		}
		if (selected==itemnums[5]) search->readsearchlog=!search->readsearchlog;
		if (selected==itemnums[6]) 
		{
			stringinput(win,offsy+10,offsx+7,search->readlogfilename,50);
		}
		if (selected==itemnums[7])
		{
			searchfor(search,buf,cursorpos,1);
			if (search->writesearchlog)
			{
				drawcenterframe(win,14,60,"Found");
				setcolor(win,COLOR_TEXT);
				mvwprintw(win,offsy+13,offsx+1,"%lld occurances found",search->occurancesfound);
				getkey(1);
			}
		}

	}	
}
tInt8 savedialog(WINDOW* win,tBuffer* hBuf)
{
	tInt16	offsx;
	tInt16	offsy;
	tMenu	Menu1;
	tInt8	itemnums[3];
	tInt8	selected;
	int	i;

	offsx=COLS/2-13;
	offsy=LINES/2-2;

	drawcenterframe(win,3,26,"Save changes?");
	setcolor(win,COLOR_TEXT);
	mvwprintw(win,offsy+2,offsx+1,"Save %12i byte%s",hBuf->changesnum,(hBuf->changesnum==1)?"?":"s?");
	clearMenu(&Menu1);
	newMenuItem(&Menu1,"Yes",3,1,'Y',0,&itemnums[0]);		
	newMenuItem(&Menu1,"No",3,9,'N',0,&itemnums[1]);
	newMenuItem(&Menu1,"Cancel",3,19,'C',1,&itemnums[2]);
	selected=MenuInteract(win,&Menu1,offsy,offsx);
	if (selected==itemnums[2]) return RETNOK;
	if (selected==itemnums[0])
	{
		if (savechanges(hBuf)!=RETOK)
		{
			drawcenterframe(win,3,22,"ERROR");
			setcolor(win,COLOR_TEXT);
			mvwprintw(win,offsy+1,offsx+1,"Could not save\n");
			return	RETNOK;
		}
	}	
	return RETOK;
}

