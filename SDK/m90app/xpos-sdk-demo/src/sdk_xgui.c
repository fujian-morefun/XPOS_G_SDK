#include "sdk_http.h"
#include "sdk_showqr.h"
#include "sdk_file.h"

#include "libapi_xpos/inc/libapi_system.h"
#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/libapi_security.h"
#include "libapi_xpos/inc/libapi_emv.h"

#define LOGOIMG "xxxx\\logo2.bmp"



#define MAIN_MENU_PAGE	"main"



// Define the menu array, the first parameter is the name of the parent menu, 
// the second parameter is the name of the current menu,
// and the second parameter is set when the name is duplicated.
static  const st_gui_menu_item_def _menu_def[] = {

	{MAIN_MENU_PAGE ,	"Sale",			""},
	{MAIN_MENU_PAGE ,	"CodePay",		""},
	{MAIN_MENU_PAGE ,	"Version",		""},
	{MAIN_MENU_PAGE ,	"Test",			""},
	{MAIN_MENU_PAGE ,	"Settings",		""},
	{MAIN_MENU_PAGE ,	"Others",		""},

	{"Test" ,		"Print",			""},
	{"Test",		"Security",		""},
	{"Test",		"Http",		    ""},
	{"Test",		"Https",		""},
	{"Test",		"ShowQr",		""},
	{"Test",		"File",		    ""},
   	{"Test",    	"Led",          ""},
	{"Test",    	"ShowString",   ""},
	{"Test",    	"TMSTest",      ""},
	{"Test",    	"M1 Test",      ""},
	{"Test",    	"Gui Color",      ""},
	{"Test",    	"Sign Page",      ""},

	{"Security",	"InitDukpt",	""},
	{"Security",	"SetMainKey",	""},
	{"Security",	"PinTest",		""},
	{"Security",	"RsaTest",		""},



	{"Settings" ,	"Net Select" ,""},
	{"Settings" ,	"WIFI Settings" , "WIFI Menu"},
	{"Settings",	"TimeSet",		""},

	
	{"Others",		"View AID",		""},
	{"Others",		"View CAPK",	""},
};



int sdk_power_proc_page(void *pval)
{
	int msgret;
	st_gui_message *pmsg = (st_gui_message *) pval;

	if (pmsg->message_id == 0x00000001){
		msgret = gui_messagebox_show("Prompt" ,"shutdown?" , "cancel" , "confirm" ,5000);

		if (msgret == 1){
			osl_power_off();
		}
		return 1;
	}
	return 0;
}




static void ShowString()
{
	st_gui_message pmsg;
	int msg_ret; 

	gui_post_message(GUI_GUIPAINT, 0 , 0);  // Send a paint message


	while(1){
		msg_ret = gui_get_message(&pmsg, 500);		// Get the message 
		if(msg_ret == 0){
			if (pmsg.message_id == GUI_GUIPAINT) {			// 	If it is a paint message, draw the page	
				gui_begin_batch_paint();
				gui_clear_dc();
				gui_text_out_ex(0, GUI_LINE_TOP(0), "welcome");
				gui_text_out_ex(0, GUI_LINE_TOP(1), "\xE6\xAC\xA2\xE8\xBF\x8E");
				gui_text_out_ex(0, GUI_LINE_TOP(2), "\xD8\xAA\xD8\xB1\xD8\xAD\xD9\x8A\xD8\xA8");
				gui_text_out_ex(0, GUI_LINE_TOP(3), "\xED\x99\x98\xEC\x98\x81\xED\x95\xA9\xEB\x8B\x88\xEB\x8B\xA4");
				gui_text_out_ex(0, GUI_LINE_TOP(4), "\xD8\xAE\xD9\x88\xD8\xB4\x20\xD8\xA2\xD9\x85\xD8\xAF\xDB\x8C\xD8\xAF");
				//gui_text_out_ex(0, GUI_LINE_TOP(5), "§Ø§Ö§Ý§Ñ§ß§ß§í§Û");
				gui_end_batch_paint();
			}
			else if (pmsg.message_id == GUI_KEYPRESS){		// Handling key messages
				if(pmsg.wparam == GUI_KEY_OK){
					break;
				}
				else if(pmsg.wparam == GUI_KEY_QUIT){
					break;
				}
			}
			gui_proc_default_msg(&pmsg);				//  Let the system handle some common messages
		}
	}
}


// The menu callback function, as long as all the menu operations of this function are registered, 
// this function will be called, and the selected menu name will be returned. 
// It is mainly determined in this function that the response menu name is processed differently.
static int _menu_proc(char *pid)
{
	int ret;
	char buff[20]={0};
	int pos = 0;
	char msg[256];


	 if (strcmp(pid , "Sale") == 0){
		upay_consum();
	}
	else if (strcmp(pid , "Version") == 0){
		sprintf(msg , "app:%s\r\n", Sys_GetAppVer());
		sprintf(msg + strlen(msg), "hardware:%s\r\n", sec_get_hw_ver());
		sprintf(msg + strlen(msg), "fireware:%s\r\n", sec_get_fw_ver());
		sprintf(msg + strlen(msg), "emv:%s\r\n", EMV_GetVersion());
		gui_messagebox_show( "Version" , msg , "" , "confirm" , 0);

	}
	else if (strcmp(pid , "CodePay") == 0){	
		upay_barscan();
	}
	else if (strcmp(pid , "Print") == 0)	{
		sdk_print();
	}
	else if (strcmp(pid , "TimeSet") == 0)	{
		time_set_page();
	}
	else if (strcmp(pid , "InitDukpt") == 0)	{
		dukptTest();
	}
	else if (strcmp(pid , "SetMainKey") == 0)	{
		mkskTest();
	}
	else if (strcmp(pid , "PinTest") == 0)	{
		PinTest();
	}
	else if (strcmp(pid , "RsaTest") == 0)	{
		RsaTest();
	}
	else if (strcmp(pid , "Http") == 0)	{
		sdk_http_test();
	}
	else if (strcmp(pid , "Https") == 0)	{
		//test2();
		sdk_https_test();		
	}
	else if (strcmp(pid , "ShowQr") == 0){
		showQrTest();
	}
	else if (strcmp(pid , "File") == 0){
		fileTest();
	}
    else if(strcmp(pid, "Led") == 0){
        	sdk_driver_led();
   	}
	else if (strcmp(pid, "Open Log") == 0){
		//LogOutSet_Show();
	}
	else if (strcmp(pid, "ShowString") == 0){
		ShowString();
	}
	else if (strcmp(pid, "TMSTest") == 0)
	{
		argot_action("#1#");
	}
	else if (strcmp(pid, "View AID") == 0)
	{
		EMV_ShowAID_Prm();
	}
	else if (strcmp(pid, "View CAPK") == 0)
	{
		EMV_ShowCAPK_Prm();
	}

	else if (strcmp(pid, "M1 Test") == 0)
	{
		//sdk_M1test();
	}


	else if (strcmp(pid, "Gui Color") == 0)
	{
		gui_titlecolorfore(0xFFFFFF);
		gui_titlecolorback(0x0000FF);
		gui_menuhightlinecolor(0x00EE00);
	}	
	else if (strcmp(pid, "Sign Page") == 0)
	{
		sign_page();
	}

	
	return 0;
}


void get_yyyymmdd_str(char *buff)
{
	char d[32]={0};
	Sys_GetDateTime(d);
	sprintf(buff , "%c%c%c%c-%c%c-%c%c", d[0],d[1],d[2],d[3], d[4],d[5], d[6],d[7]);
}


void get_hhmmss_str(char *buff)
{
	char d[32]={0};
	Sys_GetDateTime(d);
	sprintf(buff, "%c%c:%c%c:%c%c", d[8],d[9],d[10],d[11],d[12],d[13]);
}


#include "math.h"


void draw_circle()
{
	int r = 50;
	int x,y;
	int ly = 0;
	int top = 50;
	int left = 100;
	int i,count;
	for(x = -r ; x <= r ; x ++){
		y = sqrt(r * r - x * x);

		if(y > ly){
			i = ly;
			count = y;
		}
		else{
			i = y;
			count = ly;
		}

		for(i; i <= count; i ++){
			gui_pixel(left + x, top + i);
			gui_pixel(left+ x, top - i);
		}

		ly = y;
	}
}

void draw_ellipse()
{	
	int a = 50;
	int b = 30;
	int x,y;
	int ly = 0;
	int top = 110;
	int left = 100;
	int i,count;

	for(x = -a ; x <= a ; x ++){
		y = sqrt((1 - ((x * x * 1.0) / (a * a))) * (b * b));

		if(y > ly){
			i = ly;
			count = y;
		}
		else{
			i = y;
			count = ly;
		}

		for(i; i <= count; i ++){
			gui_pixel(left + x, top + i);
			gui_pixel(left+ x, top - i);
		}

		ly = y;
	}
}


void standby_pagepaint2()
{
	gui_begin_batch_paint();
	gui_clear_dc();
	draw_circle();
	draw_ellipse();
	gui_end_batch_paint();
}


void standby_pagepaint()
{
	char data[32]={0};
	int logowidth;
	int logoheight;
	int logoleft;
	int logotop;
	char * pbmp;	

	gui_begin_batch_paint();
	gui_clear_dc();

    logoleft = 30;
	logotop = 16;

	pbmp = gui_load_bmp(LOGOIMG , &logowidth , &logoheight);

	if (pbmp != 0){
		gui_out_bits(logoleft, logotop, pbmp , logowidth , logoheight, 0);
		gui_bmp_free(pbmp);
	}

	
	//gui_set_font(0);
	//gui_set_font(1);
	//gui_set_text_zoom(1);

#if 1
	get_yyyymmdd_str(data);	
	gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(3), data);

	
	get_hhmmss_str(data);	
	gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(4), data);

	sprintf(data, "Version:%s", Sys_GetAppVer()+9);
	gui_text_out((gui_get_width() - gui_get_text_width(data)) / 2, GUI_LINE_TOP(5), data);
#else
	strcpy(data,"abc123");
	gui_set_text_zoom(1);
	gui_set_font(0);
	gui_text_out(0, GUI_LINE_TOP(3), data);
	gui_set_font(1);
	gui_text_out(80, GUI_LINE_TOP(3), data);
	gui_set_font(2);
	gui_text_out(0, GUI_LINE_TOP(4), data);
	gui_set_font(3);
	gui_text_out(80, GUI_LINE_TOP(4), data);

	gui_set_font(0);
#endif	

	

	gui_end_batch_paint();
}


void sdk_main_page()
{
	st_gui_message pmsg;
	static int xgui_init_flag = 0;
	char time_cur[20];
	char time_last[20];
	int i;
#ifdef WIN32
	sign_page();
#endif

	if(xgui_init_flag == 0){
		xgui_init_flag = 1;
		gui_main_menu_func_add((void *)_menu_proc);		// Registration menu callback processing
		for(i = 0; i < sizeof(_menu_def)/sizeof(st_gui_menu_item_def); i ++){	// Add menu items cyclically
			gui_main_menu_item_add(_menu_def + i);	
		}
	}

	gui_post_message(GUI_GUIPAINT, 0 , 0);

	while(1){

		/*
		if( mf_power_ac_online()==1){ // Disable screen off when external power is available
			setbacklightflag(0);
		}
		else{
			setbacklightflag(1);
		}*/


		if (gui_get_message(&pmsg, 300) == 0) {

			if (pmsg.message_id == GUI_GUIPAINT) {
				standby_pagepaint();
			}
			else if (pmsg.message_id == GUI_KEYPRESS) {
				if (pmsg.wparam == GUI_KEY_OK || pmsg.wparam == GUI_KEY_QUIT){
					gui_main_menu_show(MAIN_MENU_PAGE , 0);	
					gui_post_message(GUI_GUIPAINT, 0 , 0);
				}
				else if ( pmsg.wparam == GUI_KEY_1 )
				{
					sdk_simple_page();
					gui_post_message(GUI_GUIPAINT, 0 , 0);
				}
			}
			else{
				gui_proc_default_msg(&pmsg);
			}
		}	

		Sys_GetDateTime(time_cur);
		if ( strcmp(time_last,time_cur) != 0 ){
			strcpy(time_last, time_cur );
			gui_post_message(GUI_GUIPAINT, 0 , 0);
		}
	}
}


