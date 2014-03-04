#include "dvb.h"
#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
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
		/* Frontend Info */
		if(ioctl(cd, FE_GET_INFO, &finfo) == -1)
		{
			perror("Cannot get frontend info: ");
		} else {
		
			strcpy(ci->name, finfo.name);  // Get demodulator name
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
				case FE_ATSC:
					ci->type = "ATSC";
					break;
			}
			ci->freq_min = finfo.frequency_min;
			ci->freq_max = finfo.frequency_max;
			ci->freq_step = finfo.frequency_stepsize;
			ci->freq_tol = finfo.frequency_tolerance;
			ci->symbol_rate_min = finfo.symbol_rate_min;
			ci->symbol_rate_max = finfo.symbol_rate_max;
			ci->symbol_rate_tolerance = finfo.symbol_rate_tolerance;
			parseCaps(finfo.caps, ci->capsInfo);
		}
		

		/* Frontend params */
		if(ioctl(cd, FE_GET_FRONTEND, &fparams) == -1)
		{
			perror("Cannot get frontend parameters: ");
		} else {
			ci->freq = fparams.frequency;
			switch(finfo.type) {
				case FE_QPSK:
					
					break;
				case FE_QAM:
					
					break;
				case FE_OFDM:
					parseOFDM(&fparams.u.ofdm, &ci->u.ofdm);
					break;
				case FE_ATSC:
					
					break;
			}
		}

		/* Frontend SNR */
		if(ioctl(cd, FE_READ_SNR, &ci->snr) == -1)
		{
			perror("Cannot get SNR: ");
		}
		
		/* Frontend SIGNAL */
		if(ioctl(cd, FE_READ_SIGNAL_STRENGTH, &ci->signal) == -1)
		{
			perror("Cannot get signal strength: ");
		}
			
		/* Frontend BER */
		if(ioctl(cd, FE_READ_BER, &ci->ber) == -1)
		{
			perror("Cannot get bit error rate: ");
		}
		
		/* Frontend STATUS */
		fe_status_t fest;
		if(ioctl(cd, FE_READ_STATUS, &fest) == -1)
		{
			perror("Cannot get frontend status: ");
		} else {
			parseStatus(fest, ci->statInfo);	
		}
		
		close(cd);
	}
}

int getApiVer()
{
	return DVB_API_VERSION;
}


void parseStatus(fe_status_t fest, capInfo_t st[])
{
	
	/*  FE_HAS_SIGNAL */
	strcpy(st[0].title,"SIGNAL");	
	st[0].has = ((fest & FE_HAS_SIGNAL) != 0) ? 1 : 0;
	/*  FE_HAS_CARRIER */
	strcpy(st[1].title,"CARRIER");	
	st[1].has = ((fest & FE_HAS_CARRIER) != 0) ? 1 : 0;
	/*  FE_HAS_VITERBI */
	strcpy(st[2].title,"STABLE");	
	st[2].has = ((fest & FE_HAS_VITERBI) != 0) ? 1 : 0;
	/*  FE_HAS_SYNC */
	strcpy(st[3].title,"SYNC");	
	st[3].has = ((fest & FE_HAS_SYNC) != 0) ? 1 : 0;
	/*  FE_HAS_LOCK */
	strcpy(st[4].title,"LOCK");	
	st[4].has = ((fest & FE_HAS_LOCK) != 0) ? 1 : 0;
	/*  FE_TIMEDOUT */
	strcpy(st[5].title,"TIMEOUT");	
	st[5].has = ((fest & FE_TIMEDOUT) != 0) ? 1 : 0;
	/*  FE_REINIT */
	strcpy(st[6].title,"REINIT");	
	st[6].has = ((fest & FE_REINIT) != 0) ? 1 : 0;

}

void parseCaps(fe_caps_t caps, capInfo_t cpi[])
{
	/* FE_IS_STUPID */
	strcpy(cpi[0].title,"N/A");	
	//cpi[0].title = "N/A";	
	cpi[0].has = (caps == 0) ? 1 : 0;

	/*  FE_CAN_INVERSION_AUTO */
	strcpy(cpi[1].title,"AUTO INV");	
	cpi[1].has = ((caps & FE_CAN_INVERSION_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_FEC_1_2 */
	strcpy(cpi[2].title,"FEC 1/2");	
	cpi[2].has = ((caps & FE_CAN_FEC_1_2) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_2_3 */
	strcpy(cpi[3].title,"FEC 2/3");
	cpi[3].has = ((caps & FE_CAN_FEC_2_3) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_3_4 */
	strcpy(cpi[4].title,"FEC 3/4");	
	cpi[4].has = ((caps & FE_CAN_FEC_3_4) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_4_5 */
	strcpy(cpi[5].title,"FEC 4/5");	
	cpi[5].has = ((caps & FE_CAN_FEC_4_5) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_5_6 */
	strcpy(cpi[6].title,"FEC 5/6");
	cpi[6].has = ((caps & FE_CAN_FEC_5_6) != 0) ? 1 : 0;

	/*  FE_CAN_FEC_7_8 */
	strcpy(cpi[7].title,"FEC 7/8");	
	cpi[7].has = ((caps & FE_CAN_FEC_7_8) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_8_9 */
	strcpy(cpi[8].title,"FEC 8/9");	
	cpi[8].has = ((caps & FE_CAN_FEC_8_9) != 0) ? 1 : 0;
	
	/*  FE_CAN_FEC_AUTO */
	strcpy(cpi[9].title,"FEC AUTO");
	cpi[9].has = ((caps & FE_CAN_FEC_AUTO) != 0) ? 1 : 0;
	
	/*  FE_CAN_QPSK */
	strcpy(cpi[10].title,"QPSK");
	cpi[10].has = ((caps & FE_CAN_QPSK) != 0) ? 1 : 0;
	
	/*  FE_CAN_QAM_16 */
	strcpy(cpi[11].title,"QAM16");	
	cpi[11].has = ((caps & FE_CAN_QAM_16) != 0) ? 1 : 0;

	/*  FE_CAN_QAM_32 */
	strcpy(cpi[12].title,"QAM32");	
	cpi[12].has = ((caps & FE_CAN_QAM_32) != 0) ? 1 : 0;

	/*  FE_CAN_QAM_64 */
	strcpy(cpi[13].title,"QAM64");	
	cpi[13].has = ((caps & FE_CAN_QAM_64) != 0) ? 1 : 0;

	/*  FE_CAN_QAM_128 */
	strcpy(cpi[14].title,"QAM128");	
	cpi[14].has = ((caps & FE_CAN_QAM_128) != 0) ? 1 : 0;

	/*  FE_CAN_QAM_256 */
	strcpy(cpi[15].title,"QAM256");	
	cpi[15].has = ((caps & FE_CAN_QAM_256) != 0) ? 1 : 0;

	/*  FE_CAN_QAM_AUTO */
	strcpy(cpi[16].title,"QAM AUTO");	
	cpi[16].has = ((caps & FE_CAN_QAM_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_TRANSMISSION_MODE_AUTO */
	strcpy(cpi[17].title,"TX AUTO");	
	cpi[17].has = ((caps & FE_CAN_TRANSMISSION_MODE_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_BANDWIDTH_AUTO */
	strcpy(cpi[18].title,"BW AUTO");	
	cpi[18].has = ((caps & FE_CAN_BANDWIDTH_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_GUARD_INTERVAL_AUTO */
	strcpy(cpi[19].title,"GI AUTO");	
	cpi[19].has = ((caps & FE_CAN_GUARD_INTERVAL_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_HIERARCHY_AUTO */
	strcpy(cpi[20].title,"HI AUTO");	
	cpi[20].has = ((caps & FE_CAN_HIERARCHY_AUTO) != 0) ? 1 : 0;

	/*  FE_CAN_MUTE_TS */
	strcpy(cpi[21].title,"MUTE TS");	
	cpi[21].has = ((caps & FE_CAN_MUTE_TS) != 0) ? 1 : 0;

	/*  FE_CAN_8VSB */
	strcpy(cpi[22].title,"8VSB");	
	cpi[22].has = ((caps & FE_CAN_8VSB) != 0) ? 1 : 0;

	/*  FE_CAN_16VSB */
	strcpy(cpi[23].title,"16VSB");	
	cpi[23].has = ((caps & FE_CAN_16VSB) != 0) ? 1 : 0;
		
	/*  FE_CAN_RECOVER */
	strcpy(cpi[24].title,"RECOVER");	
	cpi[24].has = ((caps & FE_CAN_RECOVER) != 0) ? 1 : 0;
		
	/*  FE_CAN_TURBO_FEC */
	strcpy(cpi[25].title,"TURBOFEC");	
	cpi[25].has = ((caps & FE_CAN_TURBO_FEC) != 0) ? 1 : 0;
	
	/*  FE_CAN_2G_MODULATION */
	strcpy(cpi[26].title,"DVB-S2");	
	cpi[26].has = ((caps & FE_CAN_2G_MODULATION) != 0) ? 1 : 0;

			
}

void parseOFDM(struct dvb_ofdm_parameters *ofdm, ofdmInfo_t *ofdmInfo)
{
	// Bandwidth
	switch (ofdm->bandwidth)
	{
		case BANDWIDTH_8_MHZ :
			strcpy(ofdmInfo->bandwidth, "8 Mhz");
			break;
		case BANDWIDTH_7_MHZ :
			strcpy(ofdmInfo->bandwidth, "7 Mhz");
			break;
		case BANDWIDTH_6_MHZ :
			strcpy(ofdmInfo->bandwidth, "6 Mhz");
			break;
		case BANDWIDTH_AUTO :
			strcpy(ofdmInfo->bandwidth, "Auto");
			break;
		case BANDWIDTH_5_MHZ :
			strcpy(ofdmInfo->bandwidth, "5 Mhz");
			break;
		case BANDWIDTH_10_MHZ :
			strcpy(ofdmInfo->bandwidth, "10 Mhz");
			break;
		case BANDWIDTH_1_712_MHZ : 
			strcpy(ofdmInfo->bandwidth, "1.712 Mhz");
			break;
	}
  	
  	// CR LP
  	parseCodeRate(ofdm->code_rate_LP, ofdmInfo->crLP); 
  	// CR HP
  	parseCodeRate(ofdm->code_rate_HP, ofdmInfo->crHP);
  	// Modulation
  	parseModulation(ofdm->constellation, ofdmInfo->modulation);

  	// Tx Mode
  	switch(ofdm->transmission_mode)
  	{
  		case TRANSMISSION_MODE_2K :
  			strcpy(ofdmInfo->trMode,"2K");
			break;
		case TRANSMISSION_MODE_8K :
  			strcpy(ofdmInfo->trMode,"8K");
			break;
		case TRANSMISSION_MODE_AUTO :
  			strcpy(ofdmInfo->trMode,"Auto");
			break;
		case TRANSMISSION_MODE_4K :
  			strcpy(ofdmInfo->trMode,"4K");
			break;
		case TRANSMISSION_MODE_1K :
  			strcpy(ofdmInfo->trMode,"1K");
			break;
		case TRANSMISSION_MODE_16K :
  			strcpy(ofdmInfo->trMode,"16K");
			break;
		case TRANSMISSION_MODE_32K :
  			strcpy(ofdmInfo->trMode,"32K");
			break;
  	}

  	// Guard interval
  	switch(ofdm->guard_interval)
  	{
  		case GUARD_INTERVAL_1_32 :
  			strcpy(ofdmInfo->guardInt,"1-32");
			break;
		case GUARD_INTERVAL_1_16 :
  			strcpy(ofdmInfo->guardInt,"1-16");
			break;
		case GUARD_INTERVAL_1_8 :
  			strcpy(ofdmInfo->guardInt,"1-8");
			break;
		case GUARD_INTERVAL_1_4 :
  			strcpy(ofdmInfo->guardInt,"1-4");
			break;
		case GUARD_INTERVAL_AUTO :
  			strcpy(ofdmInfo->guardInt,"Auto");
			break;
		case GUARD_INTERVAL_1_128 :
  			strcpy(ofdmInfo->guardInt,"1-128");
			break;
		case GUARD_INTERVAL_19_128 :
  			strcpy(ofdmInfo->guardInt,"19-128");
			break;
		case GUARD_INTERVAL_19_256 :
  			strcpy(ofdmInfo->guardInt,"19-256");
			break;
  	}

  	// Hiearchy
  	/*switch(ofdm->hiearchy)
  	{
  		HIERARCHY_NONE,
		HIERARCHY_1,
		HIERARCHY_2,
		HIERARCHY_4,
		HIERARCHY_AUTO,
  	}*/

}

void parseCodeRate(fe_code_rate_t coderate, char crInfo[])
{
	switch(coderate) 
	{
		case FEC_NONE :
			strcpy(crInfo, "NONE");
			break;
		case FEC_1_2 :
			strcpy(crInfo, "1/2");
			break;
		case FEC_2_3 :
			strcpy(crInfo, "2/3");
			break;
		case FEC_3_4 :
			strcpy(crInfo, "3/4");
			break;
		case FEC_4_5 :
			strcpy(crInfo, "4/5");
			break;
		case FEC_5_6 :
			strcpy(crInfo, "5/6");
			break;
		case FEC_6_7 :
			strcpy(crInfo, "6/7");
			break;
		case FEC_7_8 :
			strcpy(crInfo, "7/8");
			break;
		case FEC_8_9 :
			strcpy(crInfo, "8/9");
			break;
		case FEC_AUTO :
			strcpy(crInfo, "Auto");
			break;
		case FEC_3_5 :
			strcpy(crInfo, "3/5");
			break;
		case FEC_9_10 :
			strcpy(crInfo, "9/10");
			break;
		default:
			strcpy(crInfo, "N/A");
			break;

		
	}

}

void parseModulation(fe_modulation_t modulation, char modInfo[])
{
	switch(modulation)
	{
		case QPSK :
			strcpy(modInfo,"QPSK");
			break;
		case QAM_16 :
			strcpy(modInfo,"QAM16");
			break;
		case QAM_32 :
			strcpy(modInfo,"QAM32");
			break;
		case QAM_64 :
			strcpy(modInfo,"QAM64");
			break;
		case QAM_128 :
			strcpy(modInfo,"QAM128");
			break;
		case QAM_256 :
			strcpy(modInfo,"QAM256");
			break;
		case QAM_AUTO :
			strcpy(modInfo,"Auto");
			break;
		case VSB_8 :
			strcpy(modInfo,"VSB 8");
			break;
		case VSB_16 :
			strcpy(modInfo,"VSB 16");
			break;
		case PSK_8 :
			strcpy(modInfo,"PSK 8");
			break;
		case APSK_16 :
			strcpy(modInfo,"APSK 16");
			break;
		case APSK_32 :
			strcpy(modInfo,"APSK 32");
			break;
		case DQPSK :
			strcpy(modInfo,"DQPSK");
			break;
	}
}



/*typedef enum fe_caps {
        FE_IS_STUPID                  = 0,
        FE_CAN_INVERSION_AUTO         = 0x1,
        FE_CAN_FEC_1_2                = 0x2,
        FE_CAN_FEC_2_3                = 0x4,
        FE_CAN_FEC_3_4                = 0x8,
        FE_CAN_FEC_4_5                = 0x10,
        FE_CAN_FEC_5_6                = 0x20,
        FE_CAN_FEC_6_7                = 0x40,
        FE_CAN_FEC_7_8                = 0x80,
        FE_CAN_FEC_8_9                = 0x100,
        FE_CAN_FEC_AUTO               = 0x200,
        FE_CAN_QPSK                   = 0x400,
        FE_CAN_QAM_16                 = 0x800,
        FE_CAN_QAM_32                 = 0x1000,
        FE_CAN_QAM_64                 = 0x2000,
        FE_CAN_QAM_128                = 0x4000,
        FE_CAN_QAM_256                = 0x8000,
        FE_CAN_QAM_AUTO               = 0x10000,
        FE_CAN_TRANSMISSION_MODE_AUTO = 0x20000,
        FE_CAN_BANDWIDTH_AUTO         = 0x40000,
        FE_CAN_GUARD_INTERVAL_AUTO    = 0x80000,
        FE_CAN_HIERARCHY_AUTO         = 0x100000,
        FE_CAN_MUTE_TS                = 0x80000000,
        FE_CAN_CLEAN_SETUP            = 0x40000000
} fe_caps_t;*/
