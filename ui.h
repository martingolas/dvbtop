#pragma once
#include "cardinfo.h"
#include <ncurses.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo);

void showTitle();

void showCardMenu(uint16_t cardCount, uint16_t currentCard, uint16_t row);

void printCaps(WINDOW *win, capInfo_t *ci);

void printStatus(WINDOW *win, capInfo_t *st);

void printOFDM(WINDOW *win, ofdmInfo_t *ofdm);
