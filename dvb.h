#pragma once

#include <linux/dvb/frontend.h>
#include <linux/dvb/version.h>

#include "cardinfo.h"

#define ADAPTER_PATH "/dev/dvb"

// Check for DVB API version >= 5
#if DVB_API_VERSION < 5
#error "Local DVB API version is less than 5, this program requires version >= 5."
#endif

int countCards(); // Estimate number of available cards

void getCardInfo(int cardIdx, cardInfo* ci); // Get & Fill card info

int getApiVer();

void parseStatus(fe_status_t fest, capInfo_t st[]);

void parseCaps(fe_caps_t caps, capInfo_t cpi[]);

void parseOFDM(struct dvb_ofdm_parameters *ofdm, ofdmInfo_t *ofdmInfo);

// TODO Parse QPSK, VSB, QAM
