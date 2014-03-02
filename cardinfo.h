#pragma once
#include <stdint.h>
#include "options.h"

typedef struct capInfo {
	char has;
	char title[20];
} capInfo_t;


typedef struct cardInfo {
    char name[256];
	//char *name;
	char *type;
	int16_t signal;
	int16_t snr;
	uint32_t ber;
	uint32_t freq;
	capInfo_t capsInfo[CAPS_COUNT];
	capInfo_t statInfo[STAT_COUNT];
} cardInfo;


