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

/*************************************************************************************
Copyright: Fujian MoreFun Electronic Technology Co., Ltd.
Author:George
Functions:check_app_key
Input : sha256 value of app KEY 
Output: null
return: check result: 1 success 0 fail
Remarks: Nothing
*************************************************************************************/
LIB_EXPORT int check_app_key(unsigned char *sHashValue);

LIB_EXPORT int rki_self_test();