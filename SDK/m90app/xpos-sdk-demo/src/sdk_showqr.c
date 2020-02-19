#include "sdk_showqr.h"
#include "libapi_xpos/inc/libapi_system.h"
#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/libapi_util.h"

#define QR_WIDTH	340
#define QR_HEIGHT	340

void showQrTest()
{
	Util_QR_INFO qr_info;
	int i;
	int msg_ret; 
	st_gui_message pmsg;
	char * bitmap = (char *)malloc(QR_HEIGHT*QR_HEIGHT/8);
	int width = 0;
	int ret = 0;
	unsigned int tick1 = Sys_TimerOpen(60000);
	unsigned int tick2 = Sys_TimerOpen(60000);
	int zoom =  1;
	int left,top;
	char *data= "test qr code";


	printf("showQrTest\r\n");
	qr_info.moudleWidth = 1;		// gain
	qr_info.nLevel = 1;				// Error correction level
	qr_info.nVersion = 0;			// Qr version
	
	memset(bitmap , 0 , QR_HEIGHT*QR_HEIGHT/8);

	width = Util_GeneCodePic(data , strlen(data) , &qr_info , bitmap);

	gui_post_message(GUI_GUIPAINT, 0 , 0);  // Send a paint message
	gui_set_full_screen(1);
	if(width > 0){

		printf("width > 0\r\n");
		while(1){
			if (Sys_TimerCheck(tick1) == 0)	{	// Check page timeout
				ret = -3;
				printf("timeout \r\n");
				break;
			}

			msg_ret = gui_get_message(&pmsg, 500);		// Get the message 
			if(msg_ret == 0){
				if (pmsg.message_id == GUI_GUIPAINT) {			// 	If it is a paint message, draw the page	
					gui_begin_batch_paint();
					gui_clear_dc();
					zoom = (gui_get_height() - 10) / width;
					// Calculate barcode position, centered display
					left = (gui_get_width() - width * zoom)  / 2;	
					top = (gui_get_height() - width * zoom) / 2;
					gui_out_bits_zoom(left, top,(unsigned char *)bitmap , width , width , 0, zoom);	
					
					
					gui_end_batch_paint();
				}
				else if (pmsg.message_id == GUI_KEYPRESS){		// Handling key messages
					if(pmsg.wparam == GUI_KEY_OK){
						break;
					}
					else if(pmsg.wparam == GUI_KEY_QUIT){
						ret = -2;
						break;
					}
				}
				gui_proc_default_msg(&pmsg);				//  Let the system handle some common messages
			}


		}
	}
	else{
		printf("width < 0\r\n");

	}
	free(bitmap);
	gui_set_full_screen(0);

	return ;
}
