#include "upay_barscan.h"
#include "../pages/scan_page.h"
#include "libapi_xpos/inc/libapi_gui.h"

int upay_barscan( void )
{
	char code[64]={0};
	char amt[16]={0};
	int ret;
	char msg[128];
	char *title = "Code Pay";
	long long namt = 0;
	
	namt = inputamount_page(title, 9, 90000);
	if(namt <= 0)
	{
		return -1;
	}
	sprintf(amt, "%lld" , namt);

	memset(code, 0x00, sizeof(code));
	ret = scan_page_proc(title , code,  sizeof(code), 5000);	// Get scanned barcode

	if ( ret >0 ){
		sprintf(msg, "amt:%s\r\ncode:%s", amt, code);
		gui_messagebox_show(title, msg, "" , "ok", 0);	// Prompt result
	}

	return ret;

}
