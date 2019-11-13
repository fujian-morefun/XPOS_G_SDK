#include <stdio.h>
#include "sdk_simple_page.h"
#include "libapi_xpos/inc/libapi_util.h"
#include "libapi_xpos/inc/libapi_gui.h"

void sdk_simple_page()
{
	int key = UUTIL_TIMEOUT;

	while ( key != GUI_KEY_QUIT ) {
		//TODO: Start your application
		char data[32]={0};
		gui_begin_batch_paint();
		gui_clear_dc();
		if(key == UUTIL_TIMEOUT) 
		{
			sprintf(data,"simple page");
		}
		else{
			sprintf(data,"get key %d" , key);
		}
		gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(2), data);

		get_yyyymmdd_str(data);	
		gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(3), data);
		get_hhmmss_str(data);	
		gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(4), data);

		gui_end_batch_paint();

		key = Util_WaitKey(1);

		switch(key){
			case GUI_KEY_1:
				break;
			default:
				break;
		}
		
	}
}

