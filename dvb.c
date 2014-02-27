#include "dvb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

int countCards()
{
	int cnt = 0, dev = 0;
	char path[256];
	// CONFIG_DVB_MAX_ADAPTERS kernel config default value = 8
	// Assuming everything else than default...
	while(1)
	{
		snprintf(path, sizeof(path), "%s/adapter%d/frontend0", ADAPTER_PATH, cnt);
		//printf("%s\n",path);
		if((dev = open(path, O_RDONLY | O_NONBLOCK)) < 0)
		{
			//perror("OPEN CARD: ");
			break;
		} else
		{
			close(dev);
			++cnt;
		}
	}
	return cnt;
}

void getCardInfo(int cardIdx, cardInfo* ci)
{
	int cd = 0; // Card descriptor
	char path[256]; // Path in /dev
	struct dvb_frontend_info finfo; // Frontend info
	struct dvb_frontend_parameters fparams; // Frontend parameters

	snprintf(path, sizeof(path), "%s/adapter%d/frontend0", ADAPTER_PATH, cardIdx);
	//printf("%s", path);
	if((cd = open(path, O_RDONLY | O_NONBLOCK)) < 0)
	{
		perror("ERROR - Cannot get card info: ");
		exit(-1);
	} else {
		if(ioctl(cd, FE_GET_INFO, &finfo) == -1)
		{
			perror("Cannot get frontend info: ");
		} else {
			//printf("%s\n",finfo.name);
			//ci->name = finfo.name; // Get demodulator name
			strcpy(ci->name, finfo.name);
			// Get readable frontend type
			switch(finfo.type) {
				case FE_QPSK:
					ci->type = "DVB-S";
					break;
				case FE_QAM:
					ci->type = "DVB-C";
					break;
				case FE_OFDM:
					ci->type = "DVB-T";
					break;
			}
		}
		
		if(ioctl(cd, FE_GET_FRONTEND, &fparams) == -1)
		{
			perror("Cannot get frontend parameters: ");
		} else {
			ci->freq = fparams.frequency;
			// TODO QAM/QPSK/OFDM details	
		}

		if(ioctl(cd, FE_READ_SNR, &ci->snr) == -1)
		{
			perror("Cannot get SNR: ");
		}
		
		if(ioctl(cd, FE_READ_SIGNAL_STRENGTH, &ci->signal) == -1)
		{
			perror("Cannot get signal strength: ");
		}
		
		if(ioctl(cd, FE_READ_BER, &ci->ber) == -1)
		{
			perror("Cannot get bit error rate: ");
		}

		close(cd);
	}
}

int getApiVer()
{
	return DVB_API_VERSION;
}

