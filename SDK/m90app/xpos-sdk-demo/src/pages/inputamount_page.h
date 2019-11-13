#pragma once

//Press the Enter key to return (some interfaces return directly to the entered value)
#define INPUT_INPUT_RET_OK			0
//Press the Cancel button to return
#define INPUT_INPUT_RET_QUIT		-1
//Timeout exit
#define INPUT_INPUT_RET_TIMEOVER	-2

/**
* Input amount interface.The maximum amount allowed is 9999999.99
* @title      [IN] title
* @maxLength  [IN] Maximum length over 9 by default is 9
* @timeOver   [IN] time-out period£¬seconds
* @return :   Timeout exit return: INPUT_INPUT_RET_TIMEOVER£¬Press cancel to exit return: INPUT_INPUT_RET_QUIT£¬Otherwise, return the input amount¡£unit is divide
*			
*/
long long inputamount_page(char* title, int maxLength, int timeover);