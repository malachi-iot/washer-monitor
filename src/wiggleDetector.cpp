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

// Utilize this if we want idle state to be HIGH (utilizing "weak" internal pullup)
// This mode means wiggle detection will go to LOW/ground when activated
// UNTESTED
#define WIGGLE_IDLE_HIGH

void wiggleDetector()
{
    wigglesDetected++;
}

void wiggleDetector_setup()
{
#ifdef WIGGLE_IDLE_HIGH
    pinMode(SENSOR_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), wiggleDetector, FALLING);
#else
    pinMode(SENSOR_PIN, INPUT);

    attachInterrupt(digitalPinToInterrupt(SENSOR_PIN), wiggleDetector, RISING);
#endif
}


// FIX: Some kind of odd alignment issue if I use a uint8_t here, so kludging it
// and using a uint32_t .  I expect is related to above CircularBuffers perhaps not landing
// on perfect alignments
//__attribute__((section(".iram0.text"))) uint8_t minutesCounter = 0;
uint32_t minutesCounter = 0;

int wiggleTimeoutTimer = -1;
extern SimpleTimer timer;

// In seconds
#define WIGGLE_TIMEOUT (60 * 5)

void wiggle_stop_event()
{
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
    // represents number of wiggles detected since last we entered detected state
    static uint32_t wd_timeout = 0;

    uint16_t wd = wigglesDetected;

    // FIX: we should divide up things that have side effects outside of
    // printWiggleStatus
    if(wd > 0)
    {
        // we must get more than a "little" bit of wiggling to think that stuff is shaking
        // will need fine tuning
        // also wiggleTimeoutTimer == -1 means we are not initialized/listening for a timeout
        if(wd > 5 && wiggleTimeoutTimer != -1)
        {
            state_change(State::Detected);
            timer.restartTimer(wiggleTimeoutTimer);
        }

        // Side effect
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
