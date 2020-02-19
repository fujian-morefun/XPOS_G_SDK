#pragma once

#include "../libapi_pub.h"

typedef void * ui_touchpanel;

/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_create
Input : 		
Output : Nothing
return:  handle of ui_touchpanel
*************************************************************************************/
LIB_EXPORT ui_touchpanel uitp_create();
/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_destory
Input : 		
Output : Nothing
return:  
*************************************************************************************/
LIB_EXPORT void uitp_destory(ui_touchpanel tp);
/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_watermark_set
Input : watermark  text out on wincenter
Output : Nothing
return:  
*************************************************************************************/
LIB_EXPORT int uitp_watermark_set( ui_touchpanel t , const char *watermark);

/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_paint   
Input :  
Output : Nothing
return:  
*************************************************************************************/
LIB_EXPORT int uitp_paint( ui_touchpanel t );
/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_clear
Input : 		
Output : Nothing
return: 
*************************************************************************************/
LIB_EXPORT int uitp_clear( ui_touchpanel t );

/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_uiloop
Input : 		
Output : Nothing
return:  
*************************************************************************************/
LIB_EXPORT int uitp_uiloop( ui_touchpanel t );

/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:uitp_print_add
Input : 		
Output : Nothing
return:  
*************************************************************************************/
LIB_EXPORT int uitp_print_add( ui_touchpanel t );
