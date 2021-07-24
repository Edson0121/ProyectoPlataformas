/**
*@file SensorArduino.c
*@version 1.0
*@date 22/7/2021
*@brief Function to get arduino counting 
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Set and define sensor pins
#define TRIGGER_PIN 13
#define ECHO_PIN 12

// Variables to count
int counter = 0;
int currentState = 0;
int previousState = 0;

// Data collection frequency (ms)
int freq = 100;

/**
*@brief setup This funtion applies Serial.begin (9600) that initiates
 communication between Arduino and PC.
Then we set the trigger pin for the output and the echo pin for the input.
*/

void setup()
{
    // Serial communication
    Serial.begin(9600); 

    // Set pins
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

/**
*@brief loop This funtion reads the time between the HIGH and LOW pin establish,
the distance is set based on the formula of the speed of sound
and is adapted to cm / ms 
For the counting, when the sensor detects an object within the given limit
it defines it as 1 in the count and this is adding if the previous value is detected
if not is set as the first one,then it is printed
*/

void loop()
{
    // Measured variables
    long time, distance;

    // Measure time
    // Trigger pin in low for 2 us
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    // Trigger pin in high for 10 us
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    // Trigger pin stays in low
    digitalWrite(TRIGGER_PIN, LOW);

    // Time
    time = pulseIn(ECHO_PIN, HIGH); // Reads the time between HIGH and LOW

    // Distance
    distance = (time/2) / 29.1; // cm

    // Limit the distance of the project
    if(distance <= 10)
    {
        currentState = 1; // Counting
    }
    else
    {
        currentState = 0; // Not Counting
    }

    // Check state
    if(currentState != previousState) // Verifies if it's a new count
    {
        if(currentState == 1) // New count
        {
            counter++;
            Serial.println(counter);
        }
        previousState = currentState;
    }
    // Delay for then start the loop again
    delay(freq); 
}
