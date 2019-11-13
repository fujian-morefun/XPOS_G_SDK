/* 
 * Copyright (c) 2015 MoreFun
 *
 * @date 2015-02-14
 * @author ZhaoJinYun
 *
*/

#ifndef __MF_TAMPER_H
#define __MF_TAMPER_H

#ifdef __cplusplus
extern "C" {
#endif

enum
{
	TAMPER_SWITCH 				= (1 << 0),
	TAMPER_MESH 				= (1 << 1),
	TAMPER_TEMPERTURE 		    = (1 << 2),
	TAMPER_VOLTAGE 		        = (1 << 3),
	TAMPER_FREQUENCY 		    = (1 << 4),
};

struct mf_sec_recovery_info {
	unsigned int status;
	unsigned int rtc;
	unsigned int info;
};


int mf_tamper_init(void);

int mf_tamper_reset(void);
int mf_tamper_detect_enable(int src);

int mf_tamper_read(void);

int mf_tamper_pci_mode(void);

int mf_tamper_read_recoveryinfo(struct mf_sec_recovery_info *info);

//
//84
//
int mf_tamper_ram_write(unsigned int addr, unsigned char *data, int size);
int mf_tamper_ram_read(unsigned int addr, unsigned char *data, int size);


int mf_get_random_number(int len, unsigned char *pbuf);

#ifdef __cplusplus
}
#endif


#endif /* __MF_TAMPER_H */

