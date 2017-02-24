//
// Created by malachi on 2/22/17.
//

#include <Arduino.h>
#include <fact/CircularBuffer.h>
#include <SimpleTimer.h>

#include "state.h"

namespace util = FactUtilEmbedded;

const int SENSOR_PIN = 13;

volatile uint16_t wigglesDetected = 0;

util::layer1::CircularBuffer<uint16_t, 60> wigglesPerSecond;
util::layer1::CircularBuffer<uint32_t, 60> wigglesPerMinute;

void wiggleDetector()
{
    wigglesDetected++;
}

void wiggleDetector_setup()
{
    pinMode(SENSOR_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), wiggleDetector, RISING);
}


// FIX: Some kind of odd alignment issue if I use a uint8_t here, so kludging it
// and using a uint32_t .  I expect is related to above CircularBuffers perhaps not landing
// on perfect alignments
//__attribute__((section(".iram0.text"))) uint8_t minutesCounter = 0;
uint32_t minutesCounter = 0;

int wiggleTimeoutTimer = -1;
extern SimpleTimer timer;
void mqtt_send_status(const char* status);

#define WIGGLE_TIMEOUT 10

void wiggle_stop_event()
{
    // get here when enough time has passed without wiggle threshold being exceeded
    mqtt_send_status("DONE");

    state_change(State::NotifyingTimeout);

    // specifically do not requeue stop event, instead let threshold manager do that
    wiggleTimeoutTimer = -1;
}

void wiggle_set_detector_timeout()
{
    wiggleTimeoutTimer = timer.setTimeout(WIGGLE_TIMEOUT * 1000, wiggle_stop_event);
}

uint32_t getTotalWigglesInLast60Seconds();
uint32_t getTotalWigglesInLast5Minutes();

void printWiggleStatus()
{
    uint16_t wd = wigglesDetected;

    if(wd > 0)
    {
        // we must get more than a "little" bit of wiggling to think that stuff is shaking
        // will need fine tuning
        // also wiggleTimeoutTimer == -1 means we are not initialized/listening for a timeout
        if(wd > 300 && wiggleTimeoutTimer != -1)
            timer.restartTimer(wiggleTimeoutTimer);

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

template <class TArray>
uint32_t array_sum(const TArray& array, uint16_t size)
{
    uint32_t total = 0;

    for(int i = 0; i < size; i++)
    {
        total += array[i];
    }

    return total;
}

uint32_t getTotalWigglesInLast60Seconds()
{
    auto array = wigglesPerSecond.getUnderlyingArray();

    return array_sum(array, 60);
}
