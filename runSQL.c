
/**
*@file runSQL.c
*@version 1.0
*@date 22/7/2021
*@brief Script for database creation and data testing
*/

#include<stdio.h>
#include<time.h>
#include"sqlData.h"
#define MAX_SIZE 80

/**
*@brief main Main function that evaluates if it is necessary 
			 to create the DB, the system tables and has examples 
			 of tests of execution of functions of the main header.
*param 
*return 
*/
void main() {
	int dbRunning, tableIntervalCreated, tableDeviceCreated, dataUpdated, reCreateTable;
	float indicator;

	printf("Do you want to create the database?  0-NO , 1-YES ");
	scanf("%d", &reCreateTable);

	getVersion();
	dbRunning = checkDB();

	if (dbRunning != 0) {
		if (reCreateTable != 0){
			char *sqlIntervalo;
			char *sqlDispositivo;
			
			   /*  SQL statement */
			sqlIntervalo = "CREATE TABLE tIntervalos ("
			     "`startTime` DATETIME, "
			     "`endTime` DATETIME, "
			     "`startCount` NUMBER, "
			     "`endCount` NUMBER, "
			     "`sensorID` NUMBER)";

 			sqlDispositivo = "CREATE TABLE tDispositivo ("
			     "`id` NUMBER, "
			     "`scanClass` NUMBER, "
			     "`theoreticalUPM` NUMBER, "
			     "`name` TEXT)";

			tableIntervalCreated = createTable(sqlIntervalo);
			tableDeviceCreated = createTable(sqlDispositivo);

			dataUpdated = deviceManagement(1,30,60,"Arduino",1);
		}

		
	}
	time_t endDate = time(0);
	time_t startDate = endDate - (3600)*24;
	
	int scanClass;
	int theoreticalUPM;
	int deviceID = 1;

	char startTime_STR[80];
	char endTime_STR[80];
	
	//Time format for get data
	struct tm * sTime = localtime( & startDate );
	strftime(startTime_STR, 80, "%Y-%m-%d %H:%M:%S", sTime);

	struct tm * eTime = localtime( & endDate );
	strftime(endTime_STR, 80, "%Y-%m-%d %H:%M:%S", eTime);

	printf("Start time is %s\n", startTime_STR);
	printf("End time is %s\n", endTime_STR);

	//Get data for specific device
	getDeviceData(deviceID,  &scanClass, &theoreticalUPM);
	printf("DEVICE DATA  --> ScanClass: %d ; TheoreticalUPM: %d\n", scanClass, theoreticalUPM);

	//Get the indicator calculation for a specific device in the given date range
	indicator = calculateIndicator(startTime_STR, endTime_STR, deviceID);
	printf("Performance KPI: %f\n", indicator);
}
