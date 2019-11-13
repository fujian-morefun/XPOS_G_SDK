#include "sdk_security.h"
#include "libapi_xpos/inc/libapi_security.h"
#include "libapi_xpos/inc/libapi_gui.h"
#include "libapi_xpos/inc/libapi_system.h"
//#include "pub/osl/inc/osl_log.h"


#define TEST_GUI_KEY_SIZE	16


static void showHexData(char*title,  char * data, int size)
{
	char msg[256]={0};
	int i;

	for(i = 0;i < size; i ++){
		sprintf(msg + strlen(msg), "%02X ", data[i]);
	}
	gui_messagebox_show(title, msg, "", "confirm" , 0);
}



void dukptTest()
{
	char init_key[TEST_GUI_KEY_SIZE] ={0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
	char init_ksn[10] ={0xFF,0xFF,0x98,0x76,0x54,0x32,0x10,0xE0,0x00,0x00};
	char key[TEST_GUI_KEY_SIZE]={0};
	char ksn[10]={0};
	char indata[8]={0x31,0x31,0x31,0x31,0x31,0x31,0x31,0x31};
	char outdata[8]={0};
	int gid = 0;
	int i = 0;
	
	dukpt_init_ipek(gid, init_ksn, init_key);
	
	for(i=0; i<3; i++)
	{
		dukpt_prepare_key(gid, ksn);
		showHexData("ksn", ksn, 10);

		showHexData("indata_enc", indata, 8);
		dukpt_3des_run_ex(MKSK_ENCRYPT, indata, 8, outdata, DES_MODE_ECB, DUKPT_DES_KEY_PIN);
		showHexData("outdata_enc", outdata, 8);
		dukpt_3des_run_ex(MKSK_DECRYPT, outdata, 8, indata, DES_MODE_ECB, DUKPT_DES_KEY_PIN);
		showHexData("outdata_dec", indata, 8);
	}
}


void mkskTest()
{
	// Test key in plaintext
	char maink[TEST_GUI_KEY_SIZE] ={0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11};
	char pink[TEST_GUI_KEY_SIZE] ={0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22};
	char mack[TEST_GUI_KEY_SIZE] ={0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33};
	char magk[TEST_GUI_KEY_SIZE] ={0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0x44};
	char ind[16] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};

	char keyciphertext[TEST_GUI_KEY_SIZE];
	char outd[16];
	char kvc[8];		//kvc is Key plaintext encryption eight 0x00
	int gid =0;		// Key index 0-9

	// Save the terminal master key plaintext
	mksk_save_plaintext_key(MKSK_MAINKEY_TYPE, gid, maink, kvc);	

	//Here you can check the kvc, it is main key plaintext encryption eight 0x00,
	showHexData("MAIN KEY KVC", kvc , 4);

	// Simulate server encryption key
	Util_Des(2, maink, (char *)pink, (char *)keyciphertext);
	Util_Des(2, maink, (char *)pink+8, (char *)keyciphertext+8);


	//Save the pin key ciphertext
	mksk_save_encrypted_key(MKSK_PINENC_TYPE, gid, keyciphertext, kvc);

	//Here you can check the kvc, it is pin key plaintext encryption eight 0x00,
	showHexData("PIN KEY KVC", kvc , 4);


	// Same save magkey and mackey
	Util_Des(2, maink, (char *)mack, (char *)keyciphertext);
	Util_Des(2, maink, (char *)mack+8, (char *)keyciphertext+8);

	mksk_save_encrypted_key(MKSK_MACENC_TYPE, gid, keyciphertext, kvc);


	Util_Des(2, maink, (char *)magk, (char *)keyciphertext);
	Util_Des(2, maink, (char *)magk+8, (char *)keyciphertext+8);
	mksk_save_encrypted_key(MKSK_MAGDEC_TYPE, gid, keyciphertext, kvc);


	// Use the pin key to encrypt data
	mksk_3des_run(MKSK_PINENC_TYPE, gid, MKSK_ENCRYPT, ind, sizeof(ind), outd);		
	showHexData("ENCRYPT", outd , sizeof(ind));

	// Use the pin key to decrypt data
	mksk_3des_run(MKSK_PINENC_TYPE, gid, MKSK_ENCRYPT, ind, sizeof(ind), outd);
	showHexData("DECRYPT", outd , sizeof(ind));

}




static int _input_pin_page(char *title, int min, int max, int timeover)
{
	st_gui_message pmsg;
	int len = 0;
	int ret = -1;
	unsigned int quitTick = Sys_TimerOpen(timeover);	
	unsigned char pwdbuff[32]={0};


	gui_post_message(GUI_GUIPAINT, 0 , 0);

	while(1){
		if (Sys_TimerCheck(quitTick) == 0)	{
			ret = -2;
			break;
		}

		if (gui_get_message(&pmsg, 100) == 0) {
			if (pmsg.message_id == GUI_GUIPAINT) {
				gui_begin_batch_paint();
				gui_clear_dc();
				gui_text_out(0, GUI_LINE_TOP(0), title);
				gui_text_out(0, GUI_LINE_TOP(1), pwdbuff);
				gui_page_op_paint("cancel" , "confirm");
				gui_end_batch_paint();
			}
			else if (pmsg.message_id == GUI_KEYPRESS) {
				quitTick = Sys_TimerOpen(timeover);
				if (pmsg.wparam == GUI_KEY_XING){
					if (len < max)	{
						pwdbuff[len] = pmsg.wparam;
						len ++;
						pwdbuff[len] = 0;
						gui_post_message(GUI_GUIPAINT, 0 , 0);
					}
				}
				else if (pmsg.wparam == GUI_KEY_BACKSPACE)	{
					if (len > 0){
						len --;
						pwdbuff[len] = 0;
						gui_post_message(GUI_GUIPAINT, 0 , 0);
					}
				}
				else if (pmsg.wparam == GUI_KEY_OK)	{
					if (len >=min && len <= max){
						ret = 0;
						break;
					}
				}
				else if (pmsg.wparam == GUI_KEY_QUIT)	{
					ret = -1;
					break;
				}
			}
			else{
				gui_proc_default_msg(&pmsg);	
			}
		}
	}	
	return ret ;
}



void PinTest()
{
	char pinblock[32]={0};
	char ksn[32]={0};
	int ret;
	int pinlen = 6;
	int timeover = 30000;
	char *pan = "6223676590001877823";

	sec_set_pin_mode(1, 6);
	ret = _input_pin_page("input pin", 0, pinlen, timeover);
	if(ret == 0){
		sec_encrypt_pin_proc(SEC_MKSK_FIELD, SEC_PIN_FORMAT0, 0, pan, pinblock, 0);
	}
	sec_set_pin_mode(0, 0);
	
	showHexData("pinblock", pinblock, 16);
	showHexData("ksn", ksn, 10);
}



void PrintHexData(char *title,  char * data, int size)
{
	char msg[1024]={0};
	int i;

	sprintf(msg + strlen(msg) , "------ %s ------\r\n" , title);

	for(i = 0;i < size; i ++){
		sprintf(msg + strlen(msg), "%02X ", data[i]);
		if(i % 16 == 15) sprintf(msg + strlen(msg), "\r\n");
	}
	
	printf("%s", msg);
}

static int asc2bcd(char asc)
{
	if(asc >='0' && asc <= '9') return asc - '0';
	if(asc >='A' && asc <= 'F') return asc - 'A' + 10;
	if(asc >='a' && asc <= 'f') return asc - 'a' + 10;

	return 0;
}

void str2bcd(char * str, char * bcd, int size)
{
	int i;
	for(i = 0; i < size / 2; i++){
		bcd[i] = (asc2bcd(str[2 * i ]) <<4) + ( asc2bcd(str[ 2 * i +1]) & 0x0F);
	}
}

void RsaTest()
{
	
#if	0			// 2048
	char *nstr = "B113E4269A91A28C1A7400732D7ABC974AB5C2115D94EEFA07F63FDEF1B8B73AAD63220C73C9315B5316FDB39ACEF09AD53D5DF4BA543398186509F17BD17742F8D82978E1377E52ED2662E53E0654A5ACEAF6A11E5529288AED1FF380F2D25D2F56E33A06936197BA551BD6D505A2F572F52E1240210628BF5EA08F9D538B14A4448BEF85A772DA7F3FB44E924FA8C81332CAC9369A59F7CE2A8A6C649ECC72BC61467346C844CDB84EC746BF4385D5630D508DB6C21D59ACC7E711DDA3A94B9D482DDA7622CA12D677992150BDE28268D2432F0E1E5C39F0B0EBD34A759680F5C41E5DC8E8B1F534DC1506466E44BC7245CE5B945B9992410012604C394037";
	char *pstr = "EDBBD8132AC6095E46E9641F143C3849DA5BD3B06EABD2DF59C97822E068B1CD148B2E221C9E8770C84C997BA9E9C1D81D439E22D5B30BD9213FF2E486258584409D6B7E28DEA89CD53FEB9563770D08705A85E84324B02EF2DB54A6F206BAD89CEBB08AEEDEDC5C63C8ED46DCAD9F0AB85C4DD49D4662395E588E4D8E11170B";
	char *qstr = "BEAEF5A2530FB911D1D113CF9255DE7BD56345BDD7688A4BBA4D260829FB1C0F20726251D4B36EB9D65016455448E15B3C71A71F8D11C4CBB4486FAB8E893BDDF066C75B67FF15DCDBB47E3F8C4A9C7CFD3F1AFC5F8575E2C9D05C1CCE27DEB476FBDC68B4549573FE2D6136C4B8143D6669A7D19335BD381FD7AB5CCA578705";
	//	m:  11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
	//  puc: 62EA3EEF90C1F67F9A2ABDF88C52FF4FA7BCDFFD55A45EA1192C505B92A51F8351120F550B047FABB87803E752DE2D8926944071DCB587B2CF07BC25C6C86D896E5BBB97F50578AACE474F631E84283BD17FB0C208966C609CEAD6CC3E088E2E889DF7A93D07533EF83249D77C408CB69F28FB05F32260C08D81BDE6455F605C6F50CCAB0CC0B8F21832F7A9AC5DF1F6B2084851D10A64A7E2DA1494C2DA3C4F2F21F090EEC626DBB6EE3C22D3A8324B8A1801F901045EAC8F172FA4175821EAF7E86917CD889B9FE83F1C5B626008742A7EF8E3DB5FDFB2514C6C6D045A8D6BF4709920DD677C26EE4BFF8B18BA36E5B259C8F21548744FAAEB7D6B54F655BB
	//  prc: 15B49F084F919BD0DECAC4ADCB7AD781D3002784CDACF8707D143D866D6A7C5309D7BC8EA22420A7EDFE8C635E121ADF4C7E58D3C32BAA8246D37B47430EED74A9A4DA860D53373B204F9A31BD87EB9A738D8F228DD820AAFA102819540A398564CC17E0F9F17CD7A321D25F9351913C211A83245E86F3DA344186ED4EE206B0E0E02A6AACF051227494EBD95B2B1A5F278203EB36BC7C2C9FA09E95C6E99E4E255358D5BD6D28517E539D19C43855BD899EFD9D318D9C5239F317611E4E94B91620AE8BE0CD10CD55A85F43B1BBC5729493C63ECDF66D0CFFF7F58B18F515736CCA9B2065259E7604EF148445DC8DCB6001C6634019E08AED83ED7CD5D3E939
#else			// 1024	
	char *nstr = "90210A09279E9496A8CE376BAE0F294E9D635D0AEB58337A286324462DAF76C9BA470E3F94CD9C804CD38F44FA123CBC14387DBD248EAB791F1A650179B149B8F2B9456420F6270426BFFC38B24F951873214F05D4D32EBA89BDDE38B52C72E2DA693A299A46341317AD6053EB17E16A9BCB4A83DFDA8233D99C90DDA38DAE9F";
	char *pstr = "CA4D60B91AE12E37B7DDEEFCAACF741BB96EDB1949531D398778E53BA21A2C08270D4D92ABEAB4460ECABE7AD8EFF708B08A6EE1E934F8556D809DB848ED685B";
	char *qstr = "B662BD5F6AF643CFE68BDAE859FB1C0A137E150FD31962FA7CD8E8EC8FF54FF00307277E70D57CF9782510CB27D20374DAA0FF41C2160CB92A725C6394ABC60D";
	//	m:  1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
	//	puc: 16662278776C8F1A3DD61286B3AE8014DDDD06C502C8E753BDF9BA7C4C549B6D23441EEBF4FB0C8E6327C08D98332CCF75EBDF877C177623EE0159F82FD0D0715865A9E7E833E2B8546365D75C82168F202911C19D6871750B983FA8859CF6C4EEC85FFBF77A4EE8908C77940EEE85EF6A3626D29AFB73777FA080B947509E52
	//	prc: 02AB0DDA85D4A8EF3FA218F50868D139FCC3EA6D53844294534C40B1A68C55F655A93344F7DC1FA6A8EF79647EB6671C244B8CDA9C129B601C663EE7B0752DE3B6F3D7195B5CC78E47C663C6002D04F7F3CF58C7A9FD07653FF7BD597E4D6B8B16F50E0C9E9AF7B7359367304CCF1D2FCE721FBA7063798EA383C70F0729ABF2
#endif	

	
	int rsa_size = strlen(nstr) / 2;
	int ret;

	char n[300]={0};
	char p[200]={0};
	char q[200]={0};
	
	char ind[256]={0};
	char outd1[256]={0};
	char outd2[256]={0};
	int tick1,tick2;

	memset(ind, 0x11, sizeof(ind));
	str2bcd(nstr,n, rsa_size * 2);
	str2bcd(pstr,p, rsa_size);
	str2bcd(qstr,q, rsa_size);

	sec_save_rsa_pri_key(0, rsa_size, p, q);
	sec_save_rsa_puk_key(1, rsa_size, n);
	
	tick1 = osl_GetTick();
	ret = sec_rsa_block(0, ind, outd1, rsa_size);
	printf("tick:%d\r\n", osl_GetTick()- tick1);

	PrintHexData("outd1", outd1, rsa_size);
	printf("ret1:%d\r\n", ret);

	ret = sec_rsa_block(1, ind, outd2, rsa_size);
	PrintHexData("outd2", outd2, rsa_size);
	printf("ret2:%d\r\n", ret);
	


}


