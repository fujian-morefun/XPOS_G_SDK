/* 
 * Copyright (c) 2015 MoreFun
 *
 * @date 2015-3-16
 * @author ZhaoJinYun
 *
*/

#ifndef __MF_ONCHIP_FLASH_H
#define __MF_ONCHIP_FLASH_H

#ifdef __cplusplus
extern "C" {
#endif

int mf_onchip_flash_init(void);

int mf_onchip_flash_erase_sector(unsigned int addr);
int mf_onchip_flash_erase(unsigned int addr, int size);

int mf_onchip_flash_write(unsigned char *wbuf, unsigned int addr, int size);
int mf_onchip_flash_read(unsigned char *rbuf, unsigned int addr,  int size);


int mf_onchip_flash_erase_sector_free(unsigned int addr);
int mf_onchip_flash_erase_free(unsigned int addr, int size);

int mf_onchip_flash_write_free(unsigned char *wbuf ,unsigned int addr, int size);

int mf_onchip_flash_syncobj(int (*lock)(int status));
int mf_onchip_flash_lock(void);
int mf_onchip_flash_unlock(void);

int mf_otp_write(unsigned char *wbuf ,unsigned int addr, int size);
int mf_otp_read(unsigned char *rbuf, unsigned int addr,  int size);


#ifdef __cplusplus
}
#endif


#endif /* __MF_ONCHIP_FLASH_H */

