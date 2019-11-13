#include "libapi_xpos/inc/libapi_system.h"
#include "sdk_xgui.h"




void app_main()
{

	Sys_Init(0,0,"test");					// Application initialization
	EMV_iKernelInit();//Init EMV
	sdk_main_page(); 
}
#ifndef WIN32
void main(){
	app_main();
}
#endif

