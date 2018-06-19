#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

SQLHANDLE SQLHandle;
SQLRETURN SQLReturn;




void get_current_num_time(int int_time[6]);
void CloseDatabase();
void ConDatabase();
void show_error(unsigned int handletype, const SQLHANDLE &handle);


void SelectDB(char insert_time[30]);
//void insert(char insert_time[30]);
