#include "sdk_driver.h"


#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/libapi_comm.h"
#include "libapi_xpos/inc/libapi_iccard.h"
//#include "libapi_xpos/inc/libapi_mag.h"

static void sdk_show_msg(char *title, char *msg)
{
	// Batch refresh, because the LCD refresh takes a long time, all systems provide gui_end_batch_paint interface to uniformly send display data to the LCD screen
	gui_begin_batch_paint();				
	gui_clear_dc();			//  Clear the entire screen
	gui_text_out(0, GUI_LINE_TOP(0), title );		// Display the content of title at line 0
	gui_text_out(0, GUI_LINE_TOP(2), msg);   // Display msg content on line 2
	gui_end_batch_paint();     
}




void sdk_driver_led()
{
	int i;
	char msg[32];

	printf("sdk_driver_led\r\n");
	for(i =1 ; i < 5 ; i ++){
		sprintf(msg, "test led:%d", i);
		Util_Led(i, 1);		// led power on
		gui_messagebox_show("LED", msg, "", "confirm" , 500);
		//Sys_Sleep(500);
		Util_Led(i, 0);		// led power off
		//Sys_Sleep(500);
	}
	gui_post_message(GUI_GUIPAINT, 0 , 0);
	
}

void wifi_link_test()
{
	int ret;
	st_wifi_ap_list ap_list[10];

	ret = comm_wifi_list_ap(ap_list);
	if(ret > 0){
		comm_wifi_link_ap(&ap_list[0], "12345678");
		comm_wifi_unlink_ap();
	}
}