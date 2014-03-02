#pragma once
#include "cardinfo.h"
#include <ncurses.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo);

void showTitle();

void printCaps(WINDOW *win, capInfo_t *ci);

void printStatus(WINDOW *win, capInfo_t *st);
