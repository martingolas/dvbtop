#include "ui.h"
#include "options.h"
#include <ncurses.h>
#include <time.h>
#include <string.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo)
{
	box(win,0,0);
	attron(A_BOLD);
	

	mvwprintw(win, 1, 1, "%s", cInfo->name);
	mvwprintw(win, 2, 1, "Type: %s", cInfo->type);
	attroff(A_BOLD);
	

	mvwprintw(win, 4, 1, "Freq:\t%4.2f MHz",(float)((int)cInfo->freq / 1000000));
	mvwprintw(win, 4, 21, "Signal:\t%u",cInfo->signal);
	
	mvwprintw(win, 5, 1, "BER:\t%u",cInfo->ber);
	mvwprintw(win, 5, 21, "SNR:\t%u",cInfo->snr);


	printCaps(win, cInfo->capsInfo);
	printStatus(win, cInfo->statInfo);	

	/* Modulation */
	printOFDM(win, &cInfo->u.ofdm);

	/* Info */
	mvwprintw(win, INFO_ROFFSET, INFO_COFFSET, "Frequency\tMin:\t%4.2f MHz\tMax:\t%4.2f MHz\tStep:\t%4.2f MHz",
		(float)((int)cInfo->freq_min / 1000000),
		(float)((int)cInfo->freq_max / 1000000),
		(float)((int)cInfo->freq_step / 1000000));
	mvwprintw(win, INFO_ROFFSET+1, INFO_COFFSET, "Symbol rate\tMin:\t%u ppm\t\tMax:\t%u ppm\t\tTolerance:\t%u ppm",
		cInfo->symbol_rate_min,
		cInfo->symbol_rate_max,
		cInfo->symbol_rate_tolerance);
	


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

	//mvwprintw(0, 0, 1, "[Card: %d]", 25);
	attroff(A_BOLD);

	// Print help
	mvprintw(1,0,"Press [q] or ^C to quit.");


}


void printCaps(WINDOW *win, capInfo_t *ci)
{
	wattron(win,A_BOLD);
	mvwprintw(win, CAPS_ROFFSET, 1, "Capabilites:");
	wattroff(win,A_BOLD);

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
	
	mvwprintw(win, OFDM_ROFFSET + 1, OFDM_COFFSET, "Bandwidth:\t%s\tModulation:\t%s", ofdm->bandwidth, ofdm->modulation);
	mvwprintw(win, OFDM_ROFFSET + 2, OFDM_COFFSET, "Trans Mode:\t%s\tGuard interval:\t%s", ofdm->trMode, ofdm->guardInt);
	mvwprintw(win, OFDM_ROFFSET + 3, OFDM_COFFSET, "Hierarchy:\t%s", ofdm->hierarchy);
	mvwprintw(win, OFDM_ROFFSET + 4, OFDM_COFFSET, "Code rate:\tHigh:\t%s\tLow:\t%s", ofdm->crHP, ofdm->crLP);
}

void showCardMenu(uint16_t cardCount, uint16_t currentCard, uint16_t row)
{
	for (uint16_t i = 0; i < cardCount; ++i)
	{
		if(i == currentCard) attron(A_BOLD | COLOR_PAIR(2));
		mvprintw(row, (i*10),"[Card: %u]", i);
		if(i == currentCard) attroff(A_BOLD  | COLOR_PAIR(2));
	}
}