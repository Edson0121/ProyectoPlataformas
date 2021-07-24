
/**
*@file runScheduler.c
*@version 1.0
*@date 22/7/2021
*@brief Procedure that collects the data from the arduino and send it to the DB
*/

#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include "sqlData.h"
#include "ReadArduino.h"


int scanClass;
int theoreticalUPM;
int deviceID = 1;


/**
*@brief get_tv_cur_minus_given Time control function since the last run
*@param tv Control time
*@param tp_given Initial time
*@param sign revious evaluation
*@return 
*/
int get_tv_cur_minus_given(struct timeval *tv, struct timeval *tp_given, int *sign)
{
    struct timeval tp_cur;

    gettimeofday(&tp_cur,NULL);

    tv->tv_sec  = tp_cur.tv_sec - tp_given->tv_sec;
    tv->tv_usec = tp_cur.tv_usec - tp_given->tv_usec;

    if(tv->tv_sec > 0) {
        *sign = 1;
        if(tv->tv_usec < 0) {
            tv->tv_sec--;
            tv->tv_usec = 1000000 + tv->tv_usec;
        }
    }else
    if(tv->tv_sec == 0) {
        if(tv->tv_usec == 0)
            *sign = 0;
        else
        if(tv->tv_usec < 0) {
            *sign = -1;
            tv->tv_usec *= -1;
        }else
            *sign = 1;
    }else {
        *sign = -1;
        if(tv->tv_usec > 0) {
            tv->tv_sec++;
            tv->tv_usec = 1000000 - tv->tv_usec;
        }else
        if(tv->tv_usec < 0)
            tv->tv_usec *= -1;
    return 0;
        }
}

/**
*@brief threadproc Function that evaluates the scan class, 
				   in case of complying with the indicated time, 
				   the data is stored in the DB and the time control is reset.
*param
*return 
*/
void *threadproc(void *arg)
{
    (void)arg;
    struct timeval  tv_last_run;
    struct timeval  tv_diff;
    int sign;

    while(1)
    {
        get_tv_cur_minus_given(&tv_diff, &tv_last_run, &sign);

        if(tv_diff.tv_sec >= scanClass)
        {
            gettimeofday(&tv_last_run, NULL);
            int dataUpdated;
            int count;

            count = getArduinoData();
            dataUpdated = queryInterval(count, 1);
        }
    }
}

/**
*@brief main Main function that calls an independent 
			 thread of execution that allows independent 
			 data storage processing.
*param 
*return 
*/
int main()
{
	int runningTime;
	
	printf("Enter the execution time in seconds: ");
	scanf("%d", &runningTime);

    getDeviceData(deviceID, &scanClass, &theoreticalUPM);

	pthread_t tid;
	pthread_create(&tid, NULL, &threadproc, NULL);
	
	sleep(runningTime);
	printf("Testing\n");
    
    return 0;
}
