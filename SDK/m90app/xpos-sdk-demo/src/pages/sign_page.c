#include "sign_page.h"
#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/ui_components/ui_touchpanel.h"


int sign_page()
{
	const char *title = "Sign Page";
	int ret=0;
	st_gui_message pmsg;

	ui_touchpanel tp = uitp_create();
	// Display scan
	gui_post_message(GUI_GUIPAINT, 0 , 0);

	uitp_watermark_set(tp,"3244A2BC");
	while(1){
		if (gui_get_message(&pmsg, 0) == 0) {

			if (pmsg.message_id == GUI_KEYPRESS) {	

				if (pmsg.wparam == GUI_KEY_QUIT){
					break;
				}
				else if (pmsg.wparam == GUI_KEY_BACKSPACE){
					uitp_clear( tp);				
				}
				if (pmsg.wparam == GUI_KEY_OK){
					UPrint_Init();
					uitp_print_add( tp );
					UPrint_Feed(108);
					UPrint_Start();
				}
			}
			if (pmsg.message_id == GUI_GUIPAINT) {

				gui_begin_batch_paint();
				gui_clear_dc();

				gui_text_out((gui_get_width() - gui_get_text_width(title)) / 2, GUI_LINE_TOP(0), title);
				//gui_text_out(0, GUI_LINE_TOP(1), "scan...");

				uitp_paint( tp);

				gui_page_op_paint("Quit","Print");
				gui_end_batch_paint();
			}
		}
		else{
			uitp_uiloop( tp);
		}
	}

	uitp_destory( tp);
	return ret ;
}
