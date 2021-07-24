/**
*@file ReadArduino.h
*@version 1.0
*@date 22/7/2021
*@brief Function to be able to use the count performed by the arduino
 as a parameter for other variables
*/

#include <stdio.h>
#include <stdlib.h>

/**
*@brief getArduinoData This funtion opens the file /dev/ttyUSB to read the information get by it,
in this case that information is the counting
*@return counting
*/
int getArduinoData() 
{
   FILE *fp;
   int count;

   fp = fopen("/dev/ttyUSB0", "r"); // read mode

   if (fp == NULL)
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }

   while (fscanf(fp, " %d", &count) == 1) { }
   return count;

   fclose(fp);
}
