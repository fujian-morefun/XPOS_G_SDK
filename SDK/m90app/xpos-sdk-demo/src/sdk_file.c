#include "sdk_file.h"
//#include "sdk_log.h"
#include "libapi_xpos/inc/libapi_file.h"
#include "libapi_xpos/inc/libapi_gui.h"


#define FILE_NAME	"test.ini"
#define FILE_RECORD_NAME	"record.ini"

void fileTest()
{
	int ret = 0;
	int fp;
	char *str = "1234567890";
	char buff[16]={0};
	char buffer[32]={0};

	ret = UFile_OpenCreate(FILE_NAME, FILE_PRIVATE, FILE_CREAT, &fp, 0);//File open / create
	if( ret == UFILE_SUCCESS){
		UFile_Write(fp, str, strlen(str));// write 10 byte "1234567890"
		UFile_Close(fp);				  // Close the file
		sprintf(buffer, "Write: %s", str);
		gui_messagebox_show( "FileTest" , buffer , "" , "confirm" , 0);
	}
	else{
		gui_messagebox_show( "FileTest" , "File open or create fail" , "" , "confirm" , 0);
	}

	ret = UFile_OpenCreate(FILE_NAME, FILE_PRIVATE, FILE_RDWR, &fp, 0);
	if( ret == UFILE_SUCCESS){
		UFile_Lseek(fp, 0, 0);	// seek 0 
		memset(buff, 0, sizeof(buff));
		UFile_Read(fp, buff, 5);		// read 5 byte
		sprintf(buffer, "Read 5: %s", buff);
		gui_messagebox_show( "FileTest" , buffer , "" , "confirm" , 0);

		UFile_Lseek(fp, 8, 0);	// move 8 
		memset(buff, 0, sizeof(buff));
		UFile_Read(fp, buff, 2);		// read 2 byte
		//sdk_log_out("buff2:%s\r\n", buff);// log out "90"
		sprintf(buffer, "Read 2: %s", buff);
		gui_messagebox_show( "FileTest" , buffer , "" , "confirm" , 0);
		UFile_Close(fp);				// Close the file
	}
	else{
		gui_messagebox_show( "FileTest" , "File open or create fail" , "" , "confirm" , 0);
	}

	UFile_Remove(FILE_NAME, FILE_PUBLIC);//Delete file

}
typedef struct tagRecord
{
	char szDate[14];
	char nIndex[2];
}TEST_RECORD;

void record_Test()
{
	int nRet;
	char szDispBuf[256];
	char szFileName[32 + 1];
	byte szDateTime[14 + 1];
	char szBuf[14 + 1];
	FILE_HANDLE nFileHandle;
	int nRecLen;
	int nRecNum=0;
	int i=0;
	TEST_RECORD szRecord={0};
	DBSEARCOND stSearchCond;

	memset(szDispBuf, 0, sizeof(szDispBuf));
	memset(szFileName, 0, sizeof(szFileName));
	memset(szDateTime, 0, sizeof(szDateTime));

	strcpy(szFileName,FILE_RECORD_NAME);

	Sys_GetDateTime(szDateTime);//20200721174830


	nRet = UFile_OpenCreate(szFileName,FILE_PRIVATE,FILE_CREAT,&nFileHandle,0);
	if (nRet != UFILE_SUCCESS)
	{
		gui_messagebox_show( "FileTest" , "File create fail" , "" , "confirm" , 0);
		return -1;
	}
	UFile_Close(nFileHandle);

	nRet = UFile_OpenCreate(szFileName,FILE_PRIVATE,FILE_RDWR,&nFileHandle,0);
	if (nRet != UFILE_SUCCESS)
	{
		UFile_Remove(szFileName,FILE_PRIVATE);
		gui_messagebox_show( "FileTest" , "File open fail" , "" , "confirm" , 0);
		return -1;
	}

	nRet = UFile_Write(nFileHandle,(char*)szDateTime, sizeof(szDateTime) - 1);
	if (nRet != sizeof(szDateTime) - 1)
	{
		UFile_Close(nFileHandle);
		UFile_Remove(szFileName,FILE_PRIVATE);
		gui_messagebox_show( "FileTest" , "File write fail" , "" , "confirm" , 0);
		return -1;
	}

	nRet = UFile_Lseek(nFileHandle,4,FILE_SEEK_SET);
	if (nRet != UFILE_SUCCESS)
	{
		UFile_Close(nFileHandle);
		UFile_Remove(szFileName,FILE_PRIVATE);
		gui_messagebox_show( "FileTest" , "File seek fail" , "" , "confirm" , 0);
		return -1;		
	}
	
	nRet = UFile_Delete(nFileHandle, 4);
	if (nRet != UFILE_SUCCESS)
	{
		UFile_Close(nFileHandle);
		UFile_Remove(szFileName,FILE_PRIVATE);
		gui_messagebox_show( "FileTest" , "File delete fail" , "" , "confirm" , 0);
		return -1;		
	}

	memset(szBuf, 0, sizeof(szBuf));
	nRet = UFile_Read(nFileHandle,szBuf,20);
	if (nRet < 0)
	{
		gui_messagebox_show( "FileTest" , "File read fail" , "" , "confirm" , 0);
		return -1;		
	}
	

	UFile_Close(nFileHandle);
	UFile_Remove(szFileName,FILE_PRIVATE);

	
	memset(szDateTime,0x00,sizeof(szDateTime));
	Sys_GetDateTime(szDateTime);//20200721180430
	nRecLen = strlen(szDateTime);
	nRet = UFile_OpenCreate(szFileName,FILE_PRIVATE,FILE_APPEND,&nFileHandle,nRecLen);
	if (nRet != UFILE_SUCCESS)
	{
		gui_messagebox_show( "FileTest" , "Record create fail" , "" , "confirm" , 0);
		return -1;
	}
	UFile_Close(nFileHandle);

	nRet = UFile_GetNumberOfRecords(szFileName,FILE_PRIVATE,nRecLen);
	if (nRet < 0)
	{
		UFile_Remove(szFileName,FILE_PRIVATE);
		gui_messagebox_show( "FileTest" , "Record get number fail" , "" , "confirm" , 0);
		return -1;
	}


	UFile_Remove(szFileName,FILE_PRIVATE);
	nRet = UFile_OpenCreate(szFileName,FILE_PRIVATE,FILE_APPEND,&nFileHandle,nRecLen);
	if (nRet != UFILE_SUCCESS)
	{
		gui_messagebox_show( "FileTest" , "Record create fail" , "" , "confirm" , 0);
		return -1;
	}
	nRecLen = sizeof(szRecord);
	nRecNum = 5;
	for (i = 0; i < nRecNum; i++)
	{
		memset(&szRecord, 0, sizeof(szRecord));
		Sys_GetDateTime(szDateTime);
		memcpy(szRecord.szDate , szDateTime, strlen((char*)szDateTime));
		sprintf(szRecord.nIndex,"%02d",i);
		nRet = UFile_AppendRecord(szFileName,FILE_PRIVATE,&szRecord,sizeof(szRecord));
		gui_messagebox_show( "FileTest" , &szRecord , "" , "confirm" , 0);
		if (nRet != UFILE_SUCCESS)
		{
			gui_messagebox_show( "FileTest" , "Record append fail" , "" , "confirm" , 0);
			return -1;
		}
	}

	memset(&stSearchCond, 0, sizeof(stSearchCond));
	stSearchCond.Length = 2;
	stSearchCond.StartOffset = 14;
	stSearchCond.SearchKey = (byte*)"03";

	memset(&szRecord, 0, sizeof(szRecord));
	Sys_GetDateTime(szDateTime);
	memcpy(szRecord.szDate, szDateTime, 14);
	memcpy(szRecord.nIndex, stSearchCond.SearchKey, 2);
	
	nRet = UFile_UpdateRecord(szFileName,FILE_PRIVATE,&szRecord,nRecLen,&stSearchCond);
	gui_messagebox_show( "FileTest" , &szRecord , "" , "confirm" , 0);
	memset(&szRecord, 0, sizeof(szRecord));
	UFile_GetRecord(szFileName,FILE_PRIVATE,&szRecord,nRecLen,&stSearchCond);
	gui_messagebox_show( "FileTest" , &szRecord , "" , "confirm" , 0);

	for (i = 0; i < nRecNum; i++)
	{
		memset(&szRecord, 0, sizeof(szRecord));
		nRet = UFile_GetRecordByIndex(szFileName,FILE_PRIVATE,&szRecord,nRecLen,i);
		gui_messagebox_show( "FileTest" , &szRecord , "" , "confirm" , 0);
		if (nRet != UFILE_SUCCESS)
		{
			gui_messagebox_show( "FileTest" , "Record index fail" , "" , "confirm" , 0);
			return -1;
		}
		memset(szDispBuf, 0, sizeof(szDispBuf));
		sprintf(szDispBuf, "GetRecordByIndex[%d]", i);
		gui_messagebox_show( "FileTest" , szDispBuf , "" , "confirm" , 0);
	}

	UFile_Remove(szFileName,FILE_PRIVATE);
	return 0;
}
