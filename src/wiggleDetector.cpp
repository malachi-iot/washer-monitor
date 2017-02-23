//
// Created by malachi on 2/22/17.
//

#include <Arduino.h>
#include <fact/CircularBuffer.h>

namespace util = FactUtilEmbedded;

extern const int SENSOR_PIN;

volatile uint16_t wigglesDetected = 0;

util::layer1::CircularBuffer<uint16_t, 60> wigglesPerSecond;
util::layer1::CircularBuffer<uint32_t, 60> wigglesPerMinute;

void wiggleDetector()
{
    wigglesDetected++;
}


// FIX: Some kind of odd alignment issue if I use a uint8_t here, so kludging it
// and using a uint32_t .  I expect is related to above CircularBuffers perhaps not landing
// on perfect alignments
//__attribute__((section(".iram0.text"))) uint8_t minutesCounter = 0;
uint32_t minutesCounter = 0;

uint32_t getTotalWigglesInLast60Seconds();

void printWiggleStatus()
{
    uint16_t wd = wigglesDetected;

    if(wd > 0)
    {
        wigglesDetected = 0;
    }

    wigglesPerSecond.put(wd);

    uint32_t wd60s = getTotalWigglesInLast60Seconds();

    Serial.print("Wiggles detected/second (");
    Serial.print(minutesCounter);
    Serial.print("): ");
    Serial.print(wd);
    Serial.print(" of ");
    Serial.print(wd60s);
    Serial.println(" in the last 60 seconds");

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

    auto array = wigglesPerSecond.getUnderlyingArray();

    for(int i = 0; i < 60; i++)
    {
        total += array[i];
    }

    return total;
}
