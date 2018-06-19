#include "Main.h"

int main()
{
	int  curtime[6];
	char date[20];

	ConDatabase();
	while(1)
	{
		get_current_num_time(curtime);
		sprintf_s(date, "%04d-%02d-%02d %02d:%02d:%02d", curtime[0], curtime[1], curtime[2], curtime[3], curtime[4], curtime[5]);

		if((curtime[5] == 0) || (curtime[5] == 1) || (curtime[5] == 2) || (curtime[5] == 3) || (curtime[5] == 4) || 
		   (curtime[5] == 5) || (curtime[5] == 6) || (curtime[5] == 7) || (curtime[5] == 8) || (curtime[5] == 9) || (curtime[5] == 10) )
		{
			printf("%04d-%02d-%02d %02d:%02d:%02d \n", curtime[0], curtime[1], curtime[2], curtime[3], curtime[4], curtime[5]);
			//SelectDB(date);
			
     	}
	
		Sleep(1000); //10 Sec
	}

	return 0;
}

void ConDatabase()
{
	
	//----------------------------- DB initialize --------------------------------------------
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &SQLHandle);
 
	SQLSetEnvAttr(SQLHandle,SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
 
	SQLAllocHandle(SQL_HANDLE_DBC, SQLHandle, &SQLHandle);

	//----------------------------------Setting connection------------------------------------
	
	SQLCHAR rc[1024];
	

	switch(SQLDriverConnect (SQLHandle, NULL,(SQLCHAR*)"DRIVER={SQL Server};SERVER=localhost;DATABASE=Databasename;UID=ID;PWD=Password;" ,
				SQL_NTS, rc, 1024, NULL,SQL_DRIVER_NOPROMPT))
	{
		case SQL_SUCCESS_WITH_INFO:
			//show_error(SQL_HANDLE_DBC, SQLHandle);
			printf ("\n Connection Database Success ! \n");
			break;

		case SQL_INVALID_HANDLE:

		case SQL_ERROR:
			show_error(SQL_HANDLE_DBC, SQLHandle);
			printf ("\n Connection Database Failed ! \n ");
			SQLReturn = -1;
			break;

		default:
			break;
	}

	if(SQLReturn == -1)
		return;

}
void CloseDatabase()
{
	//--------------------------------------- close db connection -----------------------------
	SQLFreeHandle(SQL_HANDLE_STMT, SQLHandle );
	SQLDisconnect(SQLHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLHandle);
}
void show_error(unsigned int handletype, const SQLHANDLE &handle)
{
	SQLCHAR sqlstate[1024];
	SQLCHAR message[1024];
	if(SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
	printf("Message: %s \n",message);
	printf("SQLSTATE: %d\n",sqlstate);
}


void get_current_num_time(int int_time[6])
{   
	time_t timer;
	struct tm newtime;
	
	timer = time(NULL);

	localtime_s(&newtime, &timer);
	int_time[0] = newtime.tm_year + 1900;
	int_time[1] = newtime.tm_mon + 1;
	int_time[2] = newtime.tm_mday;
	int_time[3] = newtime.tm_hour;
	int_time[4] = newtime.tm_min;
	int_time[5] = newtime.tm_sec;
}
void SelectDB(char insert_time[30])
 {
	ConDatabase();

	char name[10]; 
	double ratio;
	char query[1024];
	double ratiomasuk[33];

	memset(ratiomasuk, 0x20, sizeof(ratiomasuk));
	memset(query, 0x20, sizeof(query));

	char brandname_cmp[40][15] = {"MAC.U", "YAN.F", "CAR.F", "ROM.U", "NMS.F", "KIN.F", "IOC.F", "ROH.F", "WRT.F", "EF1NA.P2", "SLF.DSA", "NMS.KR", "LMF.DWM", "EF8T0001", "EF1NA.P1", "C02.DKP", 
								  "ROH.U", "MRP.F", "CMS.F", "MSG.KR", "SDM.F", "SEP.F", "JIM.F", "KOB.P", "KOB.U", "HIY.F", "ISN.U", "SIT.F", "BRB.F", "MT.F", "MSG.BOF", "MAC.F", "LMF.DSA" };

	SQLLEN SQLLen;
	SQLRETURN ret;
	

	SQLAllocHandle(SQL_HANDLE_STMT, SQLHandle, &SQLHandle);


	ret = SQLExecDirect(SQLHandle, (SQLCHAR*)"SELECT BRAND_NAME, RATIO_MI FROM RAWMIX_RATIO WHERE BRAND_NAME <> '' ORDER BY BRAND_SEQ ASC", SQL_NTS);
	
	if (ret == SQL_SUCCESS)
	{
		while(SQLFetch(SQLHandle)==SQL_SUCCESS)
			{
				SQLGetData(SQLHandle, 1, SQL_C_CHAR, name, sizeof(name), &SQLLen);
				SQLGetData(SQLHandle, 2, SQL_C_DOUBLE, &ratio, sizeof(ratio), &SQLLen);
				
				printf ("%s\t | %.2f \n", name, ratio);

				for(int i = 0; i < 22; i++){
					if(strncmp(name,brandname_cmp[i],sizeof(brandname_cmp)) == 0){

						ratiomasuk[i] = ratio;
					}
				}
			}
	}

	ConDatabase();

	SQLAllocHandle(SQL_HANDLE_STMT, SQLHandle, &SQLHandle);
	
	sprintf_s(query, "INSERT INTO databasename.dbo.RAWMIX_RATIO_TEST"
		"(DATE, [MAC.U], [YAN.F], [CAR.F], [ROM.U], [NMS.F], [KIN.F], [IOC.F], [ROH.F], [WRT.F], [EF1NA.P2], [SLF.DSA], [NMS.KR],[LMF.DWM], [EF8T0001], [EF1NA.P1], [C02.DKP],"
		"[ROH.U], [MRP.F], [CMS.F], [MSG.KR], [SDM.F], [SEP.F], [JIM.F], [KOB.P], [KOB.U], [HIY.F], [ISN.U], [SIT.F], [BRB.F], [MT.F], [MSG.BOF], [MAC.F], [LMF.DSA]) VALUES "
		"('%s', %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f,"
		"%.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f)",
	 insert_time,ratiomasuk[0],ratiomasuk[1],ratiomasuk[2],ratiomasuk[3],ratiomasuk[4],ratiomasuk[5],ratiomasuk[6],ratiomasuk[7],ratiomasuk[8],ratiomasuk[9],ratiomasuk[10],ratiomasuk[11],ratiomasuk[12],ratiomasuk[13],ratiomasuk[14],ratiomasuk[15], 
	ratiomasuk[16],ratiomasuk[17],ratiomasuk[18],ratiomasuk[19],ratiomasuk[20],ratiomasuk[21],ratiomasuk[22],ratiomasuk[23],ratiomasuk[24],ratiomasuk[25],ratiomasuk[26],ratiomasuk[27],ratiomasuk[28],ratiomasuk[29],ratiomasuk[30],ratiomasuk[31],ratiomasuk[32]) ;

	if(SQL_SUCCESS!=SQLExecDirect(SQLHandle, (SQLCHAR*)query, SQL_NTS)){
		printf("\n INSERT ERROR \n",query);
	}
		
	
	CloseDatabase();	
}
