#pragma once
#include <stdint.h>
#include "options.h"

typedef struct ofdmInfo {
	char bandw[6]; // Bandwidth
	char crHP[6]; // Code rate high priority
	char crLP[6]; // Code rate low priority
	char modulation[10]; // OFDM,QPSK...
	char trMode[6]; // Transmission mode
	char guardInt[10]; // Guard interval
	char hiearchy[10]; // Hiearchy	
} ofdmInfo_t;



typedef struct capInfo {
	char has;
	char title[20];
} capInfo_t;


typedef struct cardInfo {
    char name[256]; // Name of the frontend chip
	char *type; // DVB-(T,S,C)
	int16_t signal; // Signal strength
	int16_t snr; // Signal to noise ratio
	uint32_t ber; // Bit error rate
	uint32_t freq; // Current tuned frequency
	uint32_t freq_min;
	uint32_t freq_max;
	uint32_t freq_step;
	uint32_t freq_tol;
	uint32_t symbol_rate_min;
	uint32_t symbol_rate_max;
	uint32_t symbol_rate_tolerance;

	capInfo_t capsInfo[CAPS_COUNT]; // Capabailites
	capInfo_t statInfo[STAT_COUNT]; // Current status

	union {
		ofdmInfo_t ofdm;
		// TODO: QPSK, QAM, VSB
	} u;
} cardInfo;


