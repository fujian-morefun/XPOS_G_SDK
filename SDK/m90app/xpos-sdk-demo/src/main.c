#include "libapi_xpos/inc/libapi_system.h"
#include "sdk_xgui.h"
//#include "libapi_xpos/inc/libapi_emv.h"


#define APP_VER "DEMO-V1.1.7"

void app_main()
{
	xgui_default_msg_func_add((void*)sdk_power_proc_page);	// Default message processing
	Sys_Init(0,0,"data");// Application initialization
	Sys_SetAppVer(APP_VER);
	EMV_iKernelInit();//Init EMV
	sdk_readcard_init(); //prepare for card reading
	sdk_main_page(); 
}
#ifndef WIN32
void main(){
	app_main();
}
#endif

