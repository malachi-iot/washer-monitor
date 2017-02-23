//
// Created by malachi on 2/22/17.
//

#include <Arduino.h>
#include <fact/CircularBuffer.h>

namespace util = FactUtilEmbedded;

extern const int SENSOR_PIN;

volatile uint16_t wigglesDetected = 0;

uint16_t wigglesPerSecondBuf[60]; // for last 60 seconds
util::CircularBuffer<uint16_t> wigglesPerSecond(wigglesPerSecondBuf, 60);
uint32_t wigglesPerMinuteBuf[60];
util::CircularBuffer<uint32_t> wigglesPerMinute(wigglesPerMinuteBuf, 60);

void wiggleDetector()
{
    wigglesDetected++;
}


uint8_t minutesCounter = 0;

uint32_t getTotalWigglesInLast60Seconds();

void printWiggleStatus()
{
    uint16_t wd = wigglesDetected;
    wigglesPerSecond.put(wd);
    uint32_t wd60s = getTotalWigglesInLast60Seconds();

    Serial.print("Wiggles detected/second: ");
    Serial.print(wd);
    Serial.print(" of ");
    Serial.print(wd60s);
    Serial.println(" in the last 60 seconds");

    if(wd > 0)
    {
        wigglesDetected = 0;
    }

    if(++minutesCounter == 60)
    {
        wigglesPerMinute.put(wd60s);
        Serial.print("Wiggles in last minute: ");
        Serial.println(wd60s);
        minutesCounter = 0;
    }
}

uint32_t getTotalWigglesInLast60Seconds()
{
    uint32_t total = 0;

    for(int i = 0; i < 60; i++)
    {
        total += wigglesPerSecondBuf[i];
    }

    return total;
}
