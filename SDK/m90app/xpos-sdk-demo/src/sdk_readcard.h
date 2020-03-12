#pragma once

#include "emvapi/inc/emv_api.h"

#define SUCC 0
#define FAIL (-1)
typedef enum
{
	NO='0',
	YES='1'
}YESORNO;

void add_capk();

int upay_readcard_proc(st_read_card_in *card_in, st_read_card_out *card_out);

void init_service_prmacqkey(int binitial);
void clear_service_prmacqkey(void);