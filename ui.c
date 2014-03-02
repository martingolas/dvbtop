#include "ui.h"
#include "options.h"
#include <ncurses.h>
#include <time.h>
#include <string.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo)
{
	//mvwprintw(win, 1, 1, "Frontend: %s", cInfo->name);
	mvwprintw(win, 1, 1, "Frontend: %s Type: %s\n  SNR: %d Signal: %d\n  BER: %d Freq: %d Hz \n", cInfo->name, cInfo->type, cInfo->snr, cInfo->signal, cInfo->ber, cInfo->freq);
	
	box(win,0,0);
	attron(A_BOLD);
	mvwprintw(win, 0, 7, "[Card: %d]", cardIdx);
	attroff(A_BOLD);

	printCaps(win, cInfo->capsInfo);

	printStatus(win, cInfo->statInfo);	
	wrefresh(win);

}


void showTitle()
{
	/* Print first row */
	attron(A_BOLD);
	mvprintw(0,0,"DVBTop v. %.2f",VER);

	// Print current time
	time_t ct = time(NULL);
	struct tm *ctime = localtime(&ct);
	char currentTime[50];
	strftime(currentTime, sizeof(currentTime), "Last update: %H:%M:%S %d.%m.%y",ctime);
	mvprintw(0, COLS-strlen(currentTime), currentTime);
	attroff(A_BOLD);

	// Print help
	mvprintw(1,0,"Press [q] or ^C to quit.");


}


void printCaps(WINDOW *win, capInfo_t *ci)
{
	attron(A_BOLD);
	mvwprintw(win, CAPS_ROFFSET, 1, "Capabilites:");
	attroff(A_BOLD);

	int i = 0, rowOffset = CAPS_ROFFSET, colOffset = 0;
	for (i = 0; i < CAPS_COUNT; ++i)
	{
		if(ci[i].has == 0)
		{
			wattron(win,COLOR_PAIR(1));
		} else
		{
			wattron(win,COLOR_PAIR(2));		
		}
		
		colOffset = CAPS_COFFSET + (i % 5)*10;
		if((i%5) == 0) rowOffset++;
		
		mvwprintw(win, rowOffset, colOffset , "%s", ci[i].title);
		wattroff(win,COLOR_PAIR(1));		
		wattroff(win,COLOR_PAIR(2));		
	}
}

void printStatus(WINDOW *win, capInfo_t *st)
{
	
	attron(A_BOLD);
	mvwprintw(win, STAT_ROFFSET, STAT_COFFSET, "Status:");
	attroff(A_BOLD);

	int i = 0;
	for (i = 0; i < STAT_COUNT; ++i)
	{
		if(st[i].has == 0)
		{
			wattron(win,COLOR_PAIR(1));
		} else
		{
			wattron(win,COLOR_PAIR(2));		
		}
		
		mvwprintw(win, STAT_ROFFSET + i + 1, STAT_COFFSET, "%s", st[i].title);
		wattroff(win,COLOR_PAIR(1));		
		wattroff(win,COLOR_PAIR(2));		
	}
}
