#pragma once
#include <stdint.h>
typedef struct cardInfo {
c	har name[256];
	//char *name;
	char *type;
	int16_t signal;
	int16_t snr;
	uint32_t ber;
	uint32_t freq;
} cardInfo;