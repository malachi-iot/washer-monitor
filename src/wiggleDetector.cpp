//
// Created by malachi on 2/22/17.
//

#include <Arduino.h>

extern const int SENSOR_PIN;

volatile int wigglesDetected = 0;

void wiggleDetector()
{
    wigglesDetected++;
}


void printWiggleStatus()
{
    if(wigglesDetected > 0)
    {
        Serial.print("Wiggles detected: ");
        Serial.println(wigglesDetected);
        wigglesDetected = 0;
    }
}
