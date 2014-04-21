#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <signal.h>

#include "dvb.h"
#include "cardinfo.h"
#include "options.h"
#include "ui.h"



void printhelp()
{
	printf("DVBTop %3.f\n Usage: dvbtop -d [refresh] -h\n        -d Refresh rate in seconds\n",VER);
}

void handleResize(int sig)
{
	endwin();
	//werase(stdscr);
	erase();
	int row,col;
	getmaxyx(stdscr,row,col);
	resize_term(row, col);
	refresh();
}

int main(int argc, char* argv[])
{
	/* Args */
	char help = 0;
	int i = 0,refresh = DEF_REFRESH;
	if(argc != 1)
	{
		for(i = 0; i < argc; ++i)
		{	
			if(strcmp(argv[i],"-h") == 0)
			{
				printf("%d\n",i);
				help = 1;
			}
			if(strcmp(argv[i],"-d") == 0)
			{
				if(i+1 < argc)
				{	
					refresh = atoi(argv[++i]);
					refresh = (refresh > 0) ? refresh : DEF_REFRESH;
				} else {
					printhelp();
					exit(0);
				}
			}			
		}
	}

	if(help == 1)
	{
		printhelp();
		exit(0);
	}

	/* Display UI, begin refresh loop */
	int realcnt = countCards();
	int cnt = 3, currentCardIdx = 0;
	
	/* Resize terminal window handler */
	signal(SIGWINCH, handleResize);


	/* ncurses init */
	WINDOW *rootWin = initscr();

	if(has_colors() == FALSE)
	{	
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

	cbreak(); // Do not flush buffer on newline, except ^Z or ^C
	noecho(); // SSsshhh....
	curs_set(0); // No cursor please
	nodelay(rootWin,TRUE);
	start_color();
	
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_GREEN);


	/* Create new window for each card */
	//WINDOW *cwins = (WINDOW*)malloc(cnt*sizeof(WINDOW));
	/*WINDOW *cwins[cnt];
	for (i = 0; i < cnt; ++i)
	{
		cwins[i] = newwin(WINDOW_HEIGHT,COLS,(20*i)+2,0);
	}*/

	WINDOW *cardWin = newwin(WINDOW_HEIGHT,COLS,3,0);

	while(1)
	{	
		/* Title row */	
		showTitle();

		refresh();

		// Check if card idx valid
		//if(currentCardIdx >= realcnt) currentCardIdx = realcnt - 1;

		/* Card menu */
		showCardMenu(cnt,currentCardIdx,2);
		
		/* Get card info and shove it to a window */
		cardInfo cInfo = {
			.name = "N/A",
			.type = "N/A",
			.signal = -1,
			.snr = -1,
			.ber = -1,
			.freq = -1
		};
		
		// Read frontend info
		if(currentCardIdx < realcnt) getCardInfo(currentCardIdx, &cInfo);
		
		// Show card info into window
		showCard(cardWin, currentCardIdx, &cInfo);
		
		// Wait for key input
		timeout(refresh*1000);

		/* Handle keyboard input */
		int ch = 0;
		if((ch = getch()) != ERR)
		{
			/*if(ch == 'q' || ch == 'Q')
			{
				endwin();
				exit(0);
			} else
			{
				sleep(refresh);
			}*/

			switch(ch)
			{
				case 'q' :
					endwin();
					exit(EXIT_SUCCESS);
					break;

				case 'Q' :
					endwin();
					exit(EXIT_SUCCESS);
					break;

				case '1' :
					currentCardIdx = 0;
					wclear(cardWin);
					break;

				case '2' :
					currentCardIdx = 1;
					wclear(cardWin);
					break;

				case '3' :
					currentCardIdx = 2;
					wclear(cardWin);
					break;

				case '4' :
					currentCardIdx = 3;
					wclear(cardWin);
					break;

				case '5' :
					currentCardIdx = 4;
					wclear(cardWin);
					break;

				case '6' :
					currentCardIdx = 5;
					wclear(cardWin);
					break;

				case '7' :
					currentCardIdx = 6;
					wclear(cardWin);
					break;

				case '8' :
					currentCardIdx = 7;
					wclear(cardWin);
					break;

				case '9' :
					currentCardIdx = 8;
					wclear(cardWin);
					break;

				
			}
		}
	}
	endwin();
	return 0;
}
