#pragma once
#include <stdint.h>
typedef struct cardInfo {
    char name[256];
	//char *name;
	char *type;
	int16_t signal;
	int16_t snr;
	uint32_t ber;
	uint32_t freq;
	int capabilites;
} cardInfo;
