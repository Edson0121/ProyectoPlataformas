
/**
*@file sqlData.h
*@version 1.0
*@date 22/7/2021
*@brief Header containing all functions related to the database
*/

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

/**
*@brief getVersion Get sqlite3 version
*param
*return 
*/
void getVersion()
{
   printf ("SQLITE LIB Version: %s\n", sqlite3_libversion());
   printf ("SQLITE LIB Version (int): %d\n", sqlite3_libversion_number());
}

/**
*@brief checkDB Check if a database exists and is accessible
*param
*@return result State of connection
*/
int checkDB()
{
   sqlite3 *db;
   int res, result;

   result = 1;

   /* Open database */
   res = sqlite3_open("MES.db", &db);
   if (res)
   {
      fprintf(stderr, "Check_DB: DB NOK %s\n", sqlite3_errmsg(db));
      result = 0;
   }
   else
   {
      fprintf(stderr, "Check_DB: DB OK\n");
   }

   sqlite3_close(db);
   
   return result;
}

/**
*@brief createTable SQL table builder
*@param sql Query to create a table
*@return result State of connection
*/
int createTable(char *sql)
{
   sqlite3 *db;
   char *error = 0;
   int res, result;

   result = 1;
   res = sqlite3_open("MES.db", &db);

   /* Execute SQL statement */
   res = sqlite3_exec(db, sql, NULL, 0, &error);
   if (res != SQLITE_OK)
     {
       fprintf(stderr, "Create_Table_Error: %s\n", error);
       sqlite3_free(error);
       result = 0;
     }
   else
     {
       fprintf(stdout, "Create_Table: Tabla creada!\n");
     }

   sqlite3_close(db);

   return result;
}

/**
*@brief queryInterval Function that does the handling of data for the execution intervals
*@param currentValue Current device value
*@param sensorID Device ID to process
*@return result State of connection
*/
int queryInterval(int currentValue, int sensorID)
{
   sqlite3 *db;
   char *error = 0;
   int res, result;
   char insertQuery[2048] = { };
   char updateQuery[2048] = { };

   result = 1;

   /* Open database */
   res = sqlite3_open("MES.db", &db);

  /* Execute SQL statement */
   //int charcheck = 
   sprintf(updateQuery,"UPDATE tIntervalos SET endTime = DATETIME('NOW', 'LOCALTIME'), endCount = %d WHERE sensorID = %d AND endCount IS NULL;",currentValue,sensorID);
   res = sqlite3_exec(db, updateQuery, NULL, 0, &error);
   if (res != SQLITE_OK)
     {
       fprintf(stderr, "Query_Interval_Error: %s\n", error);
       sqlite3_free(error);
     }
   else
     {
       fprintf(stdout, "Query_Interval: UPDATE OK!\n");
     }
 

  //int charcheck2 = 
  sprintf(insertQuery,"INSERT INTO tIntervalos VALUES (DATETIME('NOW', 'LOCALTIME'), NULL, %d, NULL, %d);",currentValue,sensorID);
  res = sqlite3_exec(db, insertQuery, NULL, 0, &error);
   if (res != SQLITE_OK)
     {
       fprintf(stderr, "Query_Interval_Error: %s\n", error);
       sqlite3_free(error);
     }
   else
     {
       fprintf(stdout, "Query_Interval: INSERT OK!\n");
     }
  sqlite3_close(db);

   return result;

}

/**
*@brief deviceManagement Handling devices in the database
*@param deviceID Device ID to process data
*@param scanClass Scan time using for storage data
*@param theoreticalUPM KPI value reference
*@param name Device name
*@param isInsert 0: if need update the device, 1: if need create the device
*@return result State of connection
*/
int deviceManagement(int deviceID, int scanClass, int theoreticalUPM, char *name, int isInsert)
{
   sqlite3 *db;
   char *error = 0;
   int res, result;
   char insertQuery[2048] = { };
   char updateQuery[2048] = { };
   char *query;
  

   result = 1;

   /* Open database */
   res = sqlite3_open("MES.db", &db);

  /* Execute SQL statement */
   //int charcheck =  
   sprintf(updateQuery,"UPDATE tDispositivo SET scanClass = %d, theoreticalUPM = %d WHERE id = %d ;",scanClass,theoreticalUPM,deviceID);
   //int charcheck2 = 
   sprintf(insertQuery,"INSERT INTO tDispositivo VALUES (%d, %d, %d, '%s');",deviceID,scanClass,theoreticalUPM,name);
 
   char *type;
 
   if (isInsert != 1)
   {
      query = updateQuery;
      type = "UPDATE";
   }
   else
   {
      query = insertQuery;
      type = "INSERT";
   }

   res = sqlite3_exec(db, query, NULL, 0, &error);
   if (res != SQLITE_OK)
     {
       fprintf(stderr, "Query_Device_Error: %s\n", error);
       sqlite3_free(error);
       result = 0;
     }
   else
     {
       fprintf(stdout, "Query_Device %s OK!\n",type);
     }

   return result;

}


/**
*@brief calculateIndicator Processing the calculation of the indicator for a 
						   specific device and data time range
*@param deviceID Device ID for analysis
*@param startTime Indicator query start time
*@param endTime Indicator query end time
*@return The calculation of the indicator for the data entered
*/
float calculateIndicator(char* startTime, char* endTime, int deviceID)
{
	sqlite3 *db;
	sqlite3_stmt* stmt = NULL;
	char *error = 0;
	int res; 
	float setpointUPM, result;
	char selectQuery[2048] = { };
	char setpointQuery[2048] = { };
	
	setpointUPM = 90;

	// Open database
	res = sqlite3_open("MES.db", &db);
  if (res)
  {
      fprintf(stderr, "CalculateIndicator: DB NOK %s\n", sqlite3_errmsg(db));
      result = 0;
   }
   else{
     sqlite3_free(error);
   }

	//int charcheck =  
  sprintf(setpointQuery,"SELECT theoreticalUPM FROM tDispositivo WHERE id = %d",deviceID);
	res = sqlite3_prepare_v2(db, setpointQuery, -1, &stmt, 0);
	
	sqlite3_bind_int (stmt, 1, 2);
	
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		setpointUPM = sqlite3_column_int(stmt, 0);
		printf("Theoretical UPM: %f\n", setpointUPM);
	}

	// Prepare SQL statement (SUM(endCount-startCount)/SUM(Cast((JulianDay(endTime) - JulianDay(startTime)) * 24 * 60 * 60 As Float))/%f
	//int charcheck2 =  
  sprintf(selectQuery,"SELECT SUM(endCount-startCount)/SUM(Cast((JulianDay(endTime) - JulianDay(startTime)) * 24 * 60 * 60 As Float)) FROM tIntervalos WHERE startTime >= '%s' AND endTime <= '%s'", startTime, endTime);
	printf("Start: %s  End: %s",startTime, endTime);
	// Execute statement
	res = sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0);

	sqlite3_bind_int (stmt, 1, 2);
	
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		float realUPM = sqlite3_column_double(stmt, 0);
    realUPM = realUPM*60.0;
		printf("Real UMP: %f\n", realUPM);
		
		result = (realUPM/setpointUPM)*100.0;
    printf("KPI %f\n", result );
	}
	else{
    printf("ERROR");
  }
	sqlite3_finalize(stmt);

	sqlite3_close(db);

	return result;

}

/**
*@brief getDeviceData Function that gets the run configuration for a specific device
*@param deviceID Device ID to consult
*@return scanClass The scan class stored in the database
*@return theoreticalUPM The Theoretical UPM stored in the database
*/
void  getDeviceData(int deviceID, int *scanClass, int *theoreticalUPM)
{
	sqlite3 *db;
	sqlite3_stmt* stmt = NULL;
	char *error = 0;
	int res, result;
	char selectQuery[2048] = { };

	// Open database
	res = sqlite3_open("MES.db", &db);

	// Prepare SQL statement
	//int charcheck = 
  sprintf(selectQuery,"SELECT scanClass, theoreticalUPM FROM tDispositivo WHERE id = %d",deviceID);
	
	// Execute statement
	res = sqlite3_prepare_v2(db, selectQuery, -1, &stmt, 0);

  if (res)
  {
      fprintf(stderr, "GetDeviceData: DB NOK %s\n", sqlite3_errmsg(db));
      result = 0;
   }

	sqlite3_bind_int (stmt, 1, 2);
	
	if (sqlite3_step(stmt) != SQLITE_DONE) {
		  *scanClass = sqlite3_column_int(stmt, 0);
		  *theoreticalUPM = sqlite3_column_int(stmt, 1);
      result = 1;
	}
  else{
    sqlite3_free(error);
  }
	
	sqlite3_finalize(stmt);

	sqlite3_close(db);

    printf("Result: %d\n", result);

}





