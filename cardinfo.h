#pragma once
#include <stdint.h>
#include <linux/dvb/frontend.h>
#include "options.h"

/*typedef enum frontendType {
	FE_QPSK,
	FE_QAM,
	FE_OFDM,
	FE_ATSC
} frontendType_t;*/

typedef struct ofdmInfo {
	char bandwidth[6]; // Bandwidth
	char crHP[6]; // Code rate high priority
	char crLP[6]; // Code rate low priority
	char modulation[10]; // OFDM,QPSK...
	char trMode[6]; // Transmission mode
	char guardInt[10]; // Guard interval
	char hierarchy[10]; // Hiearchy	
} ofdmInfo_t;


typedef struct qpskInfo {
	uint32_t symbol_rate;
	char code_rate[6];

} qpskInfo_t;



typedef struct capInfo {
	char has;
	char title[20];
} capInfo_t;


typedef struct cardInfo {
    char name[256]; // Name of the frontend chip
	char *type; // DVB-(T,S,C)
	fe_type_t typeEnum;
	uint16_t signal; // Signal strength
	uint16_t snr; // Signal to noise ratio
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


