#pragma once

#include <linux/dvb/frontend.h>
#include <linux/dvb/version.h>

#include "cardinfo.h"

#define ADAPTER_PATH "/dev/dvb"

int countCards(); // Estimate number of available cards

void getCardInfo(int cardIdx, cardInfo* ci); // Get & Fill card info

int getApiVer();

void parseCaps(int caps, capInfo_t cpi[]);