#pragma once

#include "libapi_pub.h"

#define RKI_MODE_Mosambee  1


LIB_EXPORT int rki_init( int mode);


/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:yangjy
Functions:rki_dukpt_load
Input : keydata
		keydata length
return: 0 Successfully , -1  FAIL      
Remarks: Nothing
*************************************************************************************/
LIB_EXPORT int rki_dukpt_load( const char *keydata ,int len );


LIB_EXPORT int rki_self_test();