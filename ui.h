#pragma once
#include "cardinfo.h"
#include <ncurses.h>

void showCard(WINDOW *win, int cardIdx, cardInfo *cInfo);

void showTitle();