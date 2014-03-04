#include "ui.h"
#include "options.h"
#include <ncurses.h>
#include <time.h>
#include <string.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo)
{
	box(win,0,0);
	attron(A_BOLD);
	mvwprintw(win, 0, 1, "[Card: %d]", cardIdx);

	mvwprintw(win, 1, 1, "%s", cInfo->name);
	mvwprintw(win, 1, 21, "Type: %s", cInfo->type);
	attroff(A_BOLD);
	

	mvwprintw(win, 3, 1, "Freq: %d Hz",cInfo->freq);
	mvwprintw(win, 3, 21, "Signal: %d",cInfo->signal);
	
	mvwprintw(win, 4, 1, "BER:  %d",cInfo->ber);
	mvwprintw(win, 4, 21, "SNR:    %d",cInfo->snr);


	printCaps(win, cInfo->capsInfo);
	printStatus(win, cInfo->statInfo);	

	/* Modulation */
	printOFDM(win, &cInfo->u.ofdm);

	/* Info */
	mvwprintw(win, INFO_ROFFSET, INFO_COFFSET, "Frequency: Min %d Hz Max: %d Hz Step: %d Hz",cInfo->freq_min,cInfo->freq_max,cInfo->freq_step);
	mvwprintw(win, INFO_ROFFSET+1, INFO_COFFSET, "Symbol rate - Min: %d ppm Max: %d ppm Tolerance: %d ppm",cInfo->symbol_rate_min,cInfo->symbol_rate_max,cInfo->symbol_rate_tolerance);
	


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
	int i = 0, coffset = STAT_COFFSET, roffset = STAT_ROFFSET;
	
	wattron(win,A_BOLD);
	mvwprintw(win, roffset, coffset, "Status: ");
	wattroff(win,A_BOLD);

	for (i = 0; i < STAT_COUNT; ++i)
	{
		if(st[i].has == 0)
		{
			wattron(win,COLOR_PAIR(1));
		} else
		{
			wattron(win,COLOR_PAIR(2));		
		}
		wprintw(win, "%s ", st[i].title);
		wattroff(win,COLOR_PAIR(1));		
		wattroff(win,COLOR_PAIR(2));		
	}
}

void printOFDM(WINDOW *win, ofdmInfo_t *ofdm)
{	
	wattron(win,A_BOLD);
	mvwprintw(win, OFDM_ROFFSET, OFDM_COFFSET, "OFDM Status: ");
	wattroff(win,A_BOLD);
	
	mvwprintw(win, OFDM_ROFFSET + 1, OFDM_COFFSET, "Bandwidth:\t%s Modulation:\t%s", ofdm->bandwidth, ofdm->modulation);
	mvwprintw(win, OFDM_ROFFSET + 2, OFDM_COFFSET, "Code rate: [HP]:\t%s\t[LP]:%s", ofdm->crHP, ofdm->crLP);
}