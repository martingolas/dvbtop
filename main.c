#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "dvb.h"
#include "cardinfo.h"
#include "options.h"
#include "ui.h"



void printhelp()
{
	printf("DVBTop %3.f\n Usage: dvbtop -d [refresh] -h\n        -d Refresh rate in seconds\n",VER);
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
	//int cnt = countCards();
	int cnt = 1;

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


	/* Create new window for each card */
	//WINDOW *cwins = (WINDOW*)malloc(cnt*sizeof(WINDOW));
	WINDOW *cwins[cnt];
	for (i = 0; i < cnt; ++i)
	{
		cwins[i] = newwin(20,COLS,(20*i)+2,0);
	}

	while(1)
	{	
		/* Title row */	
		showTitle();
		refresh();
		/* Get card info and shove it to a window */
		for(i = 0; i < cnt; ++i)
		{	
			cardInfo cInfo = {
				.name = "N/A",
				.type = "N/A",
				.signal = -1,
				.snr = -1,
				.ber = -1,
				.freq = -1
			};
			getCardInfo(i, &cInfo);
			//parseCaps(cInfo.caps, cInfo.capsInfo);
			//printw("Card: %d Name: %s Type: %s SNR: %d Signal: %d BER: %d Freq: %d Hz\n",i,cInfo.name, cInfo.type,cInfo.snr,cInfo.signal, cInfo.ber, cInfo.freq);
			showCard(cwins[i], i, &cInfo);
		}	

		/* Handle keyboard input */
		char ch = ' ';
		if((ch = getch()) != ERR)
		{
			if(ch == 'q' || ch == 'Q')
			{
				endwin();
				exit(0);
			}
		}

		/* Delay before next refresh */
		sleep(refresh);
	}

	endwin();
	return 0;
}
