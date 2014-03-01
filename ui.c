#include "ui.h"
#include "options.h"
#include <ncurses.h>
#include <time.h>
#include <string.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo)
{
	//mvwprintw(win, 1, 1, "Frontend: %s", cInfo->name);
	mvwprintw(win, 1, 1, "Frontend: %s Type: %s\n  SNR: %d Signal: %d\n  BER: %d Freq: %d Hz Caps: %x\n", cInfo->name, cInfo->type, cInfo->snr, cInfo->signal, cInfo->ber, cInfo->freq, cInfo->capabilites);
	
	box(win,0,0);
	attron(A_BOLD);
	mvwprintw(win, 0, 7, "[Card: %d]", cardIdx);
	wrefresh(win);
	attroff(A_BOLD);
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
