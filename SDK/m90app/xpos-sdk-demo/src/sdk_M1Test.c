#include "sdk_M1Test.h"
#include "libapi_xpos/inc/libapi_util.h"


#ifndef RFID_CMD_MIFARE_AUTH1A
#define RFID_CMD_MIFARE_AUTH1A	0x60
#define RFID_CMD_MIFARE_AUTH1B	0x61
#endif

#define MIFARE_CL_PAGE_SIZE	0x10
#define mf_log_debug printf
#define mdelay 

static char * hexdump( const char *buff, int len  ){
	static char out[1024];
	Util_Bcd2Asc( buff, out,len * 2 );
	return out;
};



void rfid_mfcl_test(void)
{
	int rc;
	unsigned char *rxbuf;
	unsigned short rxlen = 0;
	unsigned char uid[16];
	int uidlen = 0;
	unsigned char buf[MIFARE_CL_PAGE_SIZE];
	int len = 16;
	unsigned char key[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

	mf_log_debug("start rfid tcl test\n");

	while ( 1) 
	{
		rc = mf_rfid_mfcl_open();
		if(rc >= 0) {
			uidlen = mf_rfid_mfcl_getuid(uid);
			mf_log_debug("mf_rfid_mfcl_getuid %s\n", hexdump(uid, uidlen));
			mf_log_debug("PICC ATQA 0x%x\n", mf_rfid_mfcl_atqa());

			mf_rfid_mfcl_setkey(key);

			do {

				if(mf_rfid_mfcl_auth(RFID_CMD_MIFARE_AUTH1A, 0) == 0) {
					mf_log_debug("auth ok....\n");



					rc = mf_rfid_mfcl_read(0, buf, &len);
					if(rc != 0) {
						mf_log_debug("read error\n");
						break;
					}

					mf_log_debug("read block 0 dump %s\n", hexdump(buf, len));

					memset(buf, 0x0, sizeof(buf));

					buf[4] = 0xff;
					buf[5] = 0xff;
					buf[6] = 0xff;
					buf[7] = 0xff;

					buf[12] = ~0;
					buf[13] = 0;
					buf[14] = ~0;
					buf[15] = 0;

					rc = mf_rfid_mfcl_write(1, buf, 16);
					if(rc != 0) {
						mf_log_debug("write block 1 error\n");
						break;
					}

					mf_log_debug("mf_rfid_mfcl_write ok\n");

					len = 16;
					rc = mf_rfid_mfcl_read(1,(unsigned char*) buf, (unsigned int*)&len);
					if(rc != 0)
						break;
					mf_log_debug("read page 1 dump %s\n", hexdump(buf, len));

					///////////////////////////////

					if(mf_rfid_mfcl_increment(1, 10) < 0)
					{
						mf_log_debug("mfcl_increment :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_increment OK\n");

					if(mf_rfid_mfcl_transfer(1) < 0)
					{
						mf_log_debug("mfcl_transfer :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_transfer OK\n");



					len = 16;
					rc = mf_rfid_mfcl_read(1,(unsigned char*) buf, (unsigned int*)&len);
					if(rc != 0)
						break;
					mf_log_debug("read page 1 dump %s\n", hexdump(buf, len));

					if(mf_rfid_mfcl_restore(1) < 0)
					{
						mf_log_debug("mfcl_restore :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_restore OK\n");

					if(mf_rfid_mfcl_decrement(1,10) < 0)
					{
						mf_log_debug("mfcl_decrement :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_decrement OK\n");

					if(mf_rfid_mfcl_transfer(1) < 0)
					{
						mf_log_debug("mfcl_transfer :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_transfer OK\n");

					if(mf_rfid_mfcl_restore(1) < 0)
					{
						mf_log_debug("mfcl_restore :failed\r\n");
						break;
					}

					mf_log_debug(" mfcl_restore OK\n");



					len = 16;
					rc = mf_rfid_mfcl_read(1,(unsigned char*) buf, (unsigned int*)&len);
					if(rc != 0)
						break;
					mf_log_debug("read page 1 dump %s\n", hexdump(buf, len));

					/////////////////////////////

				}

			}while(0);

			mf_rfid_mfcl_close();

			break;
		}
		else{
			mf_log_debug(" mf_rfid_mfcl_open =%d\n" , rc );

			Sys_Sleep(100);
		}
	}
	

}
void sdk_M1test()
{
	rfid_mfcl_test();
}

