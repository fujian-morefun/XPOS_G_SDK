#include "inputamount_page.h"
#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/libapi_system.h"
#include <string.h>


static void _setAmount(int amount)
{
	char buf[32] = {0};
    double famt;
    famt = amount / 100.00;
	sprintf(buf, "%0.2f", famt);
	gui_text_out(0, GUI_LINE_TOP(3), buf);
}

static void _inputamount_PanitAll(char* title, int nAmount)
{
	gui_begin_batch_paint();
	gui_clear_dc();
	gui_text_out(0,0, title);
	gui_text_out(0, GUI_LINE_TOP(2), "Input the amount");	
	_setAmount(nAmount);
	gui_end_batch_paint();
}

static long long _keyPressProc(long long nAmount, int maxLength, int presskey)
{
	switch (presskey){
	case GUI_KEY_0:
	case GUI_KEY_1:
	case GUI_KEY_2:
	case GUI_KEY_3:
	case GUI_KEY_4:
	case GUI_KEY_5:
	case GUI_KEY_6:
	case GUI_KEY_7:
	case GUI_KEY_8:
	case GUI_KEY_9:	{
			char buf[32] = {0};
			int bufLen = sprintf(buf, "%lld", nAmount);

			if ( bufLen < maxLength  ){
				nAmount = nAmount * 10 + (presskey- GUI_KEY_0);
			}
			
		}		
		break;
	case GUI_KEY_BACKSPACE:	{
			nAmount = nAmount / 10;
			_setAmount(nAmount);
			
		}
		break;
	default:
		
		break;
	}

	return nAmount;
}

long long inputamount_page(char* title, int maxLength, int timeover)
{
	long long nAmount = 0;
	static long long amnt_0 = 1;
	st_gui_message pmsg;
	unsigned int quitTick = Sys_TimerOpen(timeover);	//Timeout exit timer
	maxLength = maxLength > 12 ? 12 : maxLength;


	gui_post_message(GUI_GUIPAINT, 0 , 0);

	while(1){

		//Determine if the timeout has expired
		if (Sys_TimerCheck(quitTick) == 0)	{
			nAmount = INPUT_INPUT_RET_TIMEOVER;
			break;
		}

		if (gui_get_message(&pmsg , 10) == 0) {
			if (pmsg.message_id == GUI_GUIPAINT) {
				_inputamount_PanitAll( title,nAmount);
			}
			else if (pmsg.message_id == GUI_KEYPRESS) {
				int presskey = pmsg.wparam;
				if(presskey== GUI_KEY_QUIT)	{
					nAmount = INPUT_INPUT_RET_QUIT;
					break;
				}
				else if(presskey == GUI_KEY_OK)	{
					if (nAmount == 0){
						
					}
					else{
						break;
					}
				}
				else{
					nAmount = _keyPressProc(nAmount, maxLength, presskey);
				}
				gui_post_message(GUI_GUIPAINT, 0 , 0);
			}
			else{
				gui_proc_default_msg(&pmsg);
			}

			quitTick = Sys_TimerOpen(timeover);
		}

		Sys_Sleep(10);
	}

	return nAmount;
}

